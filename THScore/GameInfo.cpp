#include "GameInfo.h"
#include <filesystem>
#include <vector>
#include <array>
#include <exception>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QCoreApplication>
#include "logger.h"
#include "Enums.h"
#include "csvIO.h"
//#include "spdlog/sinks/rotating_file_sink.h"

bool GameInfo::SetPattern(PatternHeader header)
{
	using namespace std;
	//先将原有的清空
	for (auto& si : stageInfo)
	{
		si.ClearSection();
	}
	try
	{
		std::filesystem::path path = patternFileMap.at(header);
		CSVReader reader(path);
		reader.DiscardRow();
		reader.DiscardRow();//skip header
		vector<long long> rowinfo;//只有score会超过21亿
		//0=stage,1=section,2=score,3+=special
		while (!reader.AtEnd())
		{
			rowinfo = reader.ReadLongLongRow();
			int stage = rowinfo[0];
			Section section = static_cast<Section>(rowinfo[1]);
			long long score = rowinfo[2];
			std::vector<int> specials;
			for (auto iter = next(rowinfo.begin(), 3); iter != rowinfo.end(); iter++)
			{
				specials.push_back(*iter);
			}
			try
			{
				stageInfo[stage - 1].SetData(section, 1, score, specials);
			}
			catch (std::out_of_range& e)
			{
				logger->error("Pattern fiel {0} stage out of range: {1}", path.string(), stage);
			}

		}
	}
	catch (std::out_of_range& e)
	{
		logger->error("can't find csv file");
		//生成默认路线
		for (auto& stage : stageInfo)
		{
			stage.SetDeault(specialNames.size());
		}
		std::rethrow_exception(std::current_exception());
	}
	catch (std::runtime_error& e)
	{
		logger->error(e.what());
	}
	for (auto& stage : stageInfo)
	{
		if (!stage.CheckValid())
		{
			logger->warn("Stage{0} is not filled correctly!", stage.GetStage());
			if (stage.CheckEmpty())
			{
				stage.SetDeault(specialNames.size());
				logger->info("Stage {0} is empty, auto filling data...", stage.GetStage());
			}
			else
			{
				throw std::runtime_error("Pattern invalid");
			}
		}
		stage.SetInitSection();
	}
	return true;
}

void GameInfo::SetPattern(int stage, Section section, long  long score, std::vector<int>& speical)
{
	try
	{
		stageInfo[stage - 1].SetData(section, 1, score, speical);
	}
	catch (std::out_of_range& e)
	{
		logger->error("Stage out of range: {0}", stage);
	}
}

GameInfo::GameInfo(Game game)
{
	//设为-1防止均未0时不读取路线
	difficulty = -1;
	shotType = -1;
	currentStage = 1;
	stageInfo = { 1,2,3,4,5,6 };
	this->game = game;
	switch (game)
	{
	case Game::th10:
		shotTypeList = shotTypeMap.at(10);
		specialNames =
		{
			QCoreApplication::translate("MainWindow","Faith")
		};
		break;
	case Game::th11:
		shotTypeList = shotTypeMap.at(11);
		specialNames =
		{
			QCoreApplication::translate("MainWindow","Faith") ,
			QCoreApplication::translate("MainWindow","Graze")
		};
		break;
	case Game::th12:
		shotTypeList = shotTypeMap.at(12);
		specialNames =
		{
			QCoreApplication::translate("MainWindow","Point Item Value") ,
			QCoreApplication::translate("MainWindow","Graze")
		};
		break;
	default:
		logger->warn("{0} is not supported yet!", game);
		specialNames = {};
		this->game = Game::invalid;
		throw std::runtime_error(" Game invalid");
		break;
	}
}

GameInfo::~GameInfo()
{
}

bool GameInfo::CheckRetry(int stage, int frame)
{
	if (stage < 1 || stage > 6)
		return false;
	if (stage < currentStage || (stage == 1 && frame < 60))//推把了
	{
		Clear();
		return true;
	}
	return false;
}

bool GameInfo::SetInfo(int diff, int shot)
{
	if (diff == difficulty && shot == shotType)
		return false;
	shotType = shot;
	difficulty = diff;
	SetPattern(GetHeader());
	return true;
}

bool GameInfo::SetData(int stage, long long score, std::vector<int>& speical)
{	//return true if stage changed
	if (stage < 1 || stage > 6)
	{
		return false;
	}
	stageInfo[stage - 1].SetData(0, score, speical);
	if (currentStage > stage)
	{
		//推把清空
			Clear();
			currentStage = stage;
			return true;
	}
	else if (currentStage < stage)
	{
		//避免由于换面导致结算加不到
		stageInfo[currentStage - 1].SetData(0, score, speical);
		UpdateDelta(currentStage);
		currentStage = stage;
		return true;
	}
	else return false;
}

bool GameInfo::TestSection(int bossHP, int timeLeft, int frameCount, int localFrame)
{
	static int lastHP = 0;
	bool sectionChanged = false;
	if (currentStage < 1)
	{
		return sectionChanged;
	}
	Section current = stageInfo[currentStage - 1].GetCurrentSection();
	static int lastframe = 0;
	switch (game)
	{
	case Game::invalid:
		break;
	case Game::th10:
		switch (current)
		{
		case Section::All:
			break;
		case Section::Mid:
			if (bossHP > 100 && bossHP != 10000)//有时道中击破后血量保持为10000,有时为一个小正数
			{
				switch (currentStage)
				{
				case 1:
					if (bossHP > 6000)//道中非血量5850
					{
						if (stageInfo[currentStage - 1].SetCurrentSection(Section::Boss))
							sectionChanged = true;
					}
					break;
				case 2:
					if (bossHP > 3000)//道中符血量2300
					{
						if (stageInfo[currentStage - 1].SetCurrentSection(Section::Boss))
							sectionChanged = true;
					}
					break;
				case 3:
					if (bossHP > 3000)//道中对话血量2500
					{
						if (stageInfo[currentStage - 1].SetCurrentSection(Section::Boss))
							sectionChanged = true;
					}
					break;
				case 4:
					if (frameCount > 8500)//道中非血量比关底还多
					{
						if (stageInfo[currentStage - 1].SetCurrentSection(Section::Boss))
							sectionChanged = true;
					}
					break;
				case 5:
					if (bossHP > 12000 && frameCount > 5000)//道中血量12900,约4000帧开始减少;关底血量12400
					{
						if (stageInfo[currentStage - 1].SetCurrentSection(Section::Boss))
							sectionChanged = true;
					}
					break;
				case 6:
					if (stageInfo[currentStage - 1].SetCurrentSection(Section::Boss))
						sectionChanged = true;
					break;
				default:
					logger->error("Current stage error: {0}", currentStage);
				}
			}
			break;
		case Section::Boss:

			if (bossHP <= 0)//击破
			{
				if (localFrame > 200 && localFrame < 300)//结算后localframe才重新计数，延迟一段时间用来吃消弹
				{
					if (stageInfo[currentStage - 1].SetCurrentSection(Section::Bonus))
						sectionChanged = true;
				}
			}
			break;
		case Section::Bonus:
			//防止推把不回退
			//if (frameCount < 60)
			//{
			//	stageInfo[currentStage - 1].SetInitSection();
			//}
			break;
		default:
			break;
		}
		break;
	case Game::th11:
		switch (current)
		{
		case Section::All:
			break;
		case Section::Mid:
			if (bossHP > 100 && bossHP != 10000)//有时道中击破后血量保持为10000,有时为一个小正数
			{
				switch (currentStage)
				{
				case 1:
					if (bossHP > 9000 && frameCount > 5000)//道中非血量9200
					{
						if (stageInfo[currentStage - 1].SetCurrentSection(Section::Boss))
							sectionChanged = true;
					}
					break;
				case 2:
					if (bossHP > 9000)//道中符血量7200
					{
						if (stageInfo[currentStage - 1].SetCurrentSection(Section::Boss))
							sectionChanged = true;
					}
					break;
				case 3:
					if (bossHP > 8000 && frameCount > 5500)//道中血量13000+
					{
						if (stageInfo[currentStage - 1].SetCurrentSection(Section::Boss))
							sectionChanged = true;
					}
					break;
				case 4:
					if (bossHP > 8500 && frameCount > 8200)
					{
						if (stageInfo[currentStage - 1].SetCurrentSection(Section::Boss))
							sectionChanged = true;
					}
					break;
				case 5:
					if (bossHP > 12000 && frameCount > 10000 && localFrame < 100)//道中血量15000;关底血量12000+
					{
						if (stageInfo[currentStage - 1].SetCurrentSection(Section::Boss))
							sectionChanged = true;
					}
					break;
				case 6:
					if (bossHP > 13000)
					{
						if (stageInfo[currentStage - 1].SetCurrentSection(Section::Boss))
							sectionChanged = true;
					}
					break;
				default:
					logger->error("Current stage error: {0}", currentStage);
				}
			}
			break;
		case Section::Boss:

			if (bossHP <= 0)//击破
			{
				if (localFrame > 100/*(currentStage == 6 ? 200 : 100)*/ && localFrame < 300)//结算后localframe才重新计数，延迟一段时间用来吃消弹
				{
					if (stageInfo[currentStage - 1].SetCurrentSection(Section::Bonus))
						sectionChanged = true;
				}
			}
			break;
		case Section::Bonus:
			if (frameCount < 60)
			{
				stageInfo[currentStage - 1].SetInitSection();
			}
			break;
		default:
			break;
		}
		break;
	case Game::th12:
		switch (current)
		{
		case Section::All:
			break;
		case Section::Mid:
			if (bossHP > 100)
			{
				switch (currentStage)
				{
				case 1:
					if (bossHP > 10000)
					{
						if (stageInfo[currentStage - 1].SetCurrentSection(Section::Boss))
							sectionChanged = true;
					}
					break;
				case 2:
					if (bossHP > 9000)
					{
						if (stageInfo[currentStage - 1].SetCurrentSection(Section::Boss))
							sectionChanged = true;
					}
					break;
				case 3:
					if (bossHP > 10000)
					{
						if (stageInfo[currentStage - 1].SetCurrentSection(Section::Boss))
							sectionChanged = true;
					}
					break;
				case 4:
					if (bossHP > 8000)
					{
						if (stageInfo[currentStage - 1].SetCurrentSection(Section::Boss))
							sectionChanged = true;
					}
					break;
				case 5:
					if (bossHP > 11000)
					{
						if (stageInfo[currentStage - 1].SetCurrentSection(Section::Boss))
							sectionChanged = true;
					}
					break;
				case 6:
					if (bossHP > 10000)
					{
						if (stageInfo[currentStage - 1].SetCurrentSection(Section::Boss))
							sectionChanged = true;
					}
					break;
				default:
					logger->error("Current stage error: {0}", currentStage);
				}
			}

			break;
		case Section::Boss:
			if (bossHP <= 0)//击破
			{
				//自己做一个计时
				static int frame = 0;
				if (frame > 0)
				{
					if (frameCount > frame + 180)//多等一会儿，不然掉的道具吃不到
					{
						if (stageInfo[currentStage - 1].SetCurrentSection(Section::Bonus))
							sectionChanged = true;
						frame = 0;
					}
				}
				else
				{
					frame = frameCount;
				}
			}
			break;
		case Section::Bonus:
			if (frameCount < 60)
			{
				stageInfo[currentStage - 1].SetInitSection();
				sectionChanged = true;
			}
			break;
		default:
			break;
		}
		break;
	default:
		logger->error("Incorrect Game {0} in TestSection", static_cast<int>(game));
		break;
	}
	lastHP = bossHP;
	return sectionChanged;
}

void GameInfo::UpdateDelta(int stage)
{
	if (stage < 1 || stage > 6)
	{
		//logger->error("Stage incorrect in GameInfo::UpdateDelta: {0}", stage);
		return;
	}
	int index = stage - 1;
	long long dScore = stageInfo[index].GetScore(0) - stageInfo[index].GetScore(1);
	std::vector<int> dSpecial;
	for (auto iter1 = stageInfo[index].GetSpecials(0).begin(), iter2 = stageInfo[index].GetSpecials(1).begin();
		iter1 != stageInfo[index].GetSpecials(0).end() && iter2 != stageInfo[index].GetSpecials(1).end();
		iter1++, iter2++)
	{
		dSpecial.push_back(*iter1 - *iter2);
	}
	stageInfo[index].SetData(2, dScore, dSpecial);
}

void GameInfo::Clear()
{
	for (auto& stage : stageInfo)
	{
		stage.SetInitSection();
		stage.ResetAll(0);
		stage.ResetAll(2);
	}
}

PatternHeader GameInfo::GetHeader()
{
	return PatternHeader{ static_cast<int>(game),difficulty,shotType };
}

GameInfo* GameInfo::Create(std::string gameName, DWORD processID, MemoryReader*& mr)
{
	if (gameName == "th10")
	{
		mr = new TH10Reader(processID);
		return new GameInfo(Game::th10);
	}
	else if (gameName == "th11")
	{
		mr = new TH11Reader(processID);
		return new GameInfo(Game::th11);
	}
	else if (gameName == "th12")
	{
		mr = new TH12Reader(processID);
		return new GameInfo(Game::th12);
	}
	else {
		logger->error("Game Not supported: {0}", gameName);
		throw std::runtime_error("Not a supported game");
	}
}

void GameInfo::ScanCSV()
{
	using namespace std;
	namespace fs = filesystem;
	vector<fs::path> files;
	try
	{
		for (auto& p : fs::directory_iterator("csv"))
			files.push_back(p.path());
	}
	catch (fs::filesystem_error e)
	{
		logger->error("ScanCSV error: {0}", e.what());
	}
	for (auto& file : files)
	{
		QFileInfo fileInfo(file);
		if (fileInfo.suffix() == "csv")
		{
			logger->info("Found a csv file {0}", fileInfo.fileName().toUtf8().data());
			CSVReader reader(file);
			QStringList strList = reader.ReadRow();
			int game = strList[0].toInt();
			int diff = strList[1].toInt();
			int shot = strList[2].toInt();
			PatternHeader header{ game,diff,shot };

			auto inserted = patternFileMap.insert(std::make_pair(header, file)).second;
			if (!inserted)
				logger->error("{0} is not recorded because another file({1}) for same shot already exists.",
					file.string(), patternFileMap.at(header).string());
		}
	}
}

QString GameInfo::ShotType()
{
	return shotTypeList[shotType];
}

QString GameInfo::Difficulty()
{
	return DiffList[difficulty];
}

QString GameInfo::GameName()
{
	return GameName(game);
}

QString GameInfo::GameName(Game game)
{
	switch (game)
	{
	case Game::th10:
		return "東方風神録";
		break;
	case Game::th11:
		return "東方地霊殿";
		break;
	case Game::th12:
		return "東方星蓮船";
		break;
	default:
		break;
	}
	return "ERROR";
}

const std::vector<QString>& GameInfo::GetShotTypeList(int gameNum)
{
	return shotTypeMap.at(gameNum);
}

const std::vector<QString>& GameInfo::GetShotTypeList()const
{
	return shotTypeList;
}

int GameInfo::ColumnCount()
{
	//stage+section+header+score+...
	return 4 + specialNames.size();
}

int GameInfo::RowCount()
{
	//每section3行数据
	return 3 * SectionCount();
}

int GameInfo::SectionCount()
{
	int count = 0;
	for (auto& stage : stageInfo)
	{
		count += stage.GetSectionCount();
	}
	return count;
}

QStringList GameInfo::GetColumnHeader() const
{
	QStringList list;
	list << QCoreApplication::translate("MainWindow", "Stage")
		<< QCoreApplication::translate("MainWindow", "Section") << ""
		<< QCoreApplication::translate("MainWindow", "Score");
	list += GetSpecialNames();
	return list;
}

QStringList GameInfo::GetSpecialNames() const
{
	QStringList list;
	for (auto& str : specialNames)
	{
		list << str;
	}
	return list;
}

int GameInfo::GetCurrentStage() const
{
	return currentStage;
}

int GameInfo::GetStageSectionCount(int index) const
{
	if (index < 0)
	{
		index = 0;
		logger->info("Index too small, reset to 0.");
	}
	else if (index > 5)
	{
		index = 5;
		logger->info("Index too big, reset to 5.");
	}
	return stageInfo[index].GetSectionCount();
}

int GameInfo::GetCurrenSectionRowIndex() const
{
	if (currentStage < 1)
	{
		return 0;
	}
	int sectionCount = 0;
	for (int index = 0; index < currentStage - 1; index++)
	{
		sectionCount += GetStageSectionCount(index);
	}
	sectionCount += stageInfo[currentStage - 1].GetCurrentSectionIndex() + 1;
	return sectionCount > 0 ? (sectionCount - 1) * 3 : 0;
}

QStringList GameInfo::GetSectionNames(int index) const
{
	if (index < 0)
	{
		index = 0;
		logger->info("Index too small, reset to 0.");
	}
	else if (index > 5)
	{
		index = 5;
		logger->info("Index too big, reset to 5.");
	}
	return stageInfo[index].GetSectionNames();
}

Section GameInfo::GetCurrentSection(int index) const
{
	if (index < 0)
	{
		index = 0;
		logger->info("Index too small, reset to 0.");
	}
	else if (index > 5)
	{
		index = 5;
		logger->info("Index too big, reset to 5.");
	}
	return stageInfo[index].GetCurrentSection();
}

const SectionInfo& GameInfo::GetCurrentSectionInfo(int index) const
{
	if (index < 0)
	{
		index = 0;
		logger->info("Index too small, reset to 0.");
	}
	else if (index > 5)
	{
		index = 5;
		logger->info("Index too big, reset to 5.");
	}
	return stageInfo[index].GetCurrentSectionInfo();
}

const SectionInfo& GameInfo::GetPrevSectionInfo(int index) const
{
	if (index < 0)
	{
		index = 0;
		logger->info("Index too small, reset to 0.");
	}
	else if (index > 5)
	{
		index = 5;
		logger->info("Index too big, reset to 5.");
	}
	return stageInfo[index].GetPrevSectionInfo();
}

int GameInfo::GetCurrentSectionIndex(int index) const
{
	if (index < 0)
	{
		index = 0;
		logger->info("Index too small, reset to 0.");
	}
	else if (index > 5)
	{
		index = 5;
		logger->info("Index too big, reset to 5.");
	}
	return stageInfo[index].GetCurrentSectionIndex();
}

const std::vector<SectionInfo>& GameInfo::GetSectionInfos(int index) const
{
	if (index < 0)
	{
		index = 0;
		logger->info("Index too small, reset to 0.");
	}
	else if (index > 5)
	{
		index = 5;
		logger->info("Index too big, reset to 5.");
	}
	return stageInfo[index].GetSectionInfos();
}

StageInfo* GameInfo::GetStageInfo(int index)
{
	if (index < 0)
	{
		index = 0;
		logger->info("Index too small, reset to 0.");
	}
	else if (index > 5)
	{
		index = 5;
		logger->info("Index too big, reset to 5.");
	}
	return &stageInfo[index];
}

const std::unordered_map< PatternHeader, std::filesystem::path >& GameInfo::GetPatternFileMap()
{
	return patternFileMap;
}

void GameInfo::Init()
{
	//机体列表
	shotTypeMap.insert(std::make_pair<int, std::vector<QString>>(10,
		{
			QCoreApplication::translate("MainWindow","Reimu A"),
			QCoreApplication::translate("MainWindow","Reimu B"),
			QCoreApplication::translate("MainWindow","Reimu C"),
			QCoreApplication::translate("MainWindow","Marisa A"),
			QCoreApplication::translate("MainWindow","Marisa B"),
			QCoreApplication::translate("MainWindow","Marisa C")
		}));
	shotTypeMap.insert(std::make_pair<int, std::vector<QString>>(11,
		{
			QCoreApplication::translate("MainWindow","Reimu A"),
			QCoreApplication::translate("MainWindow","Reimu B"),
			QCoreApplication::translate("MainWindow","Reimu C"),
			QCoreApplication::translate("MainWindow","Marisa A"),
			QCoreApplication::translate("MainWindow","Marisa B"),
			QCoreApplication::translate("MainWindow","Marisa C")
		}));
	shotTypeMap.insert(std::make_pair<int, std::vector<QString>>(12,
		{
			QCoreApplication::translate("MainWindow","Reimu A"),
			QCoreApplication::translate("MainWindow","Reimu B"),
			QCoreApplication::translate("MainWindow","Marisa A"),
			QCoreApplication::translate("MainWindow","Marisa B"),
			QCoreApplication::translate("MainWindow","Sanae A"),
			QCoreApplication::translate("MainWindow","Sanae B")
		}));

	exeMap.insert(std::make_pair< std::string, std::vector<std::wstring>>("th10", { L"th10.exe",L"th10chs.exe",L"th10cht.exe" }));
	exeMap.insert(std::make_pair< std::string, std::vector<std::wstring>>("th11", { L"th11.exe",L"th11c.exe" }));
	exeMap.insert(std::make_pair< std::string, std::vector<std::wstring>>("th12", { L"th12.exe",L"th12c.exe" }));
}

std::unordered_map<int, std::vector<QString>> GameInfo::shotTypeMap;
const QString GameInfo::DiffList[4] = { "Easy","Normal","Hard","Lunatic" };
std::unordered_map<PatternHeader, std::filesystem::path> GameInfo::patternFileMap;
std::unordered_map<std::string, std::vector<std::wstring>> GameInfo::exeMap;


bool PatternHeader::operator==(const PatternHeader& other)const
{
	if (this->game == other.game && this->difficulty == other.difficulty && this->shotType == other.shotType)
		return true;
	else return false;
}