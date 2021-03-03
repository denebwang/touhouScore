#include "GameInfo.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <vector>
#include <array>
#include <memory>
#include <exception>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include "logger.h"
#include "Enums.h"
#include "spdlog/sinks/rotating_file_sink.h"

void GameInfo::SetPattern(patternHeader header)
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
			stageInfo[stage - 1].SetData(section, 1, score, specials);
		}
	}
	catch (std::out_of_range e)
	{
		logger->error("can't find csv file: {0}", e.what());

	}
	for (auto& stage : stageInfo)
	{
		if (!stage.CheckValid())
		{
			logger->error("Stage{0} is not filled correctly!", stage.GetStage());
			if (stage.CheckEmpty())
			{
				stage.SetDeault(specialNames.size());
			}
			else
				throw std::runtime_error("GameInfo::SetPattern invalid");
		}
		stage.SetInitSection();
	}
}

GameInfo::GameInfo(Game game)
{
	//设为-1防止均未0时不读取路线
	difficulty = -1;
	shotType = -1;
	currentStage = 0;
	switch (game)
	{
	case Game::th10:
		this->game = game;
		shotTypeList = shotTypeMap.at(10);
		stageInfo =
		{
			1,2,3,4,5,6
		};
		//for (size_t i = 0; i < 6; i++)
		//{
		//	stageInfo[i].reset(new TH10Info(i+1));
		//}
		specialNames = { "Faith" };
		break;
	case Game::th11:
		this->game = game;
		shotTypeList = shotTypeMap.at(11);
		//for (size_t i = 0; i < 6; i++)
		//{
		//	stageInfo[i].reset(new TH11Info(i + 1));
		//}
		specialNames = { "Faith" , "Graze" };
		break;
	default:
		logger->warn("{0} is not supported yet!", game);
		specialNames = {};
		this->game = Game::invalid;
		break;
	}
}

GameInfo::~GameInfo()
{
}


bool GameInfo::CheckRetry(int stage)
{
	if (stage < currentStage)//推把了
	{
		for (auto& si : stageInfo)
		{
			si.ResetAll();
			return true;
		}
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
	if (stage < 1)
	{
		return false;
	}

	stageInfo[stage - 1].SetData(0, score, speical);
	if (currentStage != stage)
	{
		currentStage = stage;
		return true;
	}
	return false;
	//UpdateDelta(stage);


}

bool GameInfo::TestSection(int bossHP, int timeLeft, int frameCount)
{
	bool sectionChanged = false;
	if (currentStage < 1)
	{
		return sectionChanged;
	}
	Section current = stageInfo[currentStage - 1].GetCurrentSection();
	static int hp = 0, time = 0;
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
			//todo: 击破动画结束后再切换
			if (bossHP < 0)//击破
			{
				if (stageInfo[currentStage - 1].SetCurrentSection(Section::Bonus))
					sectionChanged = true;
			}
			break;
		case Section::Bonus:
			break;
		default:
			break;
		}
		break;
	case Game::th11:
		//todo: th11的代码
		break;
	default:
		break;
	}
	hp = bossHP;
	time = timeLeft;
	return sectionChanged;
}

void GameInfo::UpdateDelta(int stage)
{
	if (stage < 1)
	{
		logger->error("Stage incorrect in GameInfo::UpdateDelta: {0}", stage);
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

GameInfo::patternHeader GameInfo::GetHeader()
{
	return patternHeader{ static_cast<int>(game),difficulty,shotType };
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
		//mr = new TH11Reader(processID);
		//return new GameInfo(GameInfo::Game::th11);
	}
	else {
		logger->error("Game Not supported: {0}", gameName);
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
	for (auto file : files)
	{
		QFileInfo fileInfo(file);
		if (fileInfo.suffix() == "csv")
		{
			logger->info("Found a csv file {0}", fileInfo.fileName().toUtf8().data());
			CSVReader reader(file);
			patternHeader header = reader.GetHeader();

			patternFileMap.insert(std::make_pair(header, file));
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
	switch (game)
	{
	case Game::th10:
		return "東方風神録";
		break;
	case Game::th11:
		return "東方地霊殿";
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
	list << "Stage" << "Section" << "" << "Score";
	for (auto iter = specialNames.begin(); iter != specialNames.end(); iter++)
	{
		list << *iter;
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
	return (sectionCount - 1) * 3;
}

QStringList GameInfo::GetSectionNames(int index) const
{
	if (index < 0)
	{
		index = 0;
	}
	return stageInfo[index].GetSectionNames();
}

Section GameInfo::GetCurrentSection(int index) const
{
	if (index < 0)
	{
		index = 0;
	}
	return stageInfo[index].GetCurrentSection();
}

const SectionInfo& GameInfo::GetCurrentSectionInfo(int index) const
{
	if (index < 0)
	{
		index = 0;
	}
	return stageInfo[index].GetCurrentSectionInfo();
}

const SectionInfo& GameInfo::GetPrevSectionInfo(int index) const
{
	if (index < 0)
	{
		index = 0;
	}
	return stageInfo[index].GetPrevSectionInfo();
}

int GameInfo::GetCurrentSectionIndex(int index) const
{
	if (index < 0)
	{
		index = 0;
	}
	return stageInfo[index].GetCurrentSectionIndex();
}

const std::vector<SectionInfo>& GameInfo::GetSectionInfos(int index) const
{
	if (index < 0)
	{
		index = 0;
	}
	return stageInfo[index].GetSectionInfos();
}

void GameInfo::Init()
{
	//机体列表
	shotTypeMap.insert(std::make_pair<int, std::vector<QString>>(10, { "Reimu A","Reimu B","Reimu C","Marisa A","Marisa B","Marisa C" }));
	shotTypeMap.insert(std::make_pair<int, std::vector<QString>>(11, { "Reimu A","Reimu B","Reimu C","Marisa A","Marisa B","Marisa C" }));

	exeMap.insert(std::make_pair< std::string, std::vector<std::wstring>>("th10", { L"th10.exe",L"th10chs.exe",L"th10cht.exe" }));
	exeMap.insert(std::make_pair< std::string, std::vector<std::wstring>>("th11", { L"th11.exe",L"th11c" }));
}

std::unordered_map<int, std::vector<QString>> GameInfo::shotTypeMap;
const QString GameInfo::DiffList[4] = { "Easy","Normal","Hard","Lunatic" };
std::unordered_map<GameInfo::patternHeader, std::filesystem::path> GameInfo::patternFileMap;
std::unordered_map<std::string, std::vector<std::wstring>> GameInfo::exeMap;




GameInfo::CSVReader::CSVReader(const std::filesystem::path& name)
{
	file = new QFile(name);
	if (!file->open(QIODevice::ReadOnly | QIODevice::Text))
	{
		throw std::runtime_error("Open csv file failed!");
	}
	ts = new QTextStream(file);
}

GameInfo::CSVReader::~CSVReader()
{
	delete ts;
	file->close();
	delete file;
}

GameInfo::patternHeader GameInfo::CSVReader::GetHeader()
{
	using namespace std;
	vector<long long> headerData = ReadLongLongRow();
	patternHeader header =
	{
		headerData[0],
		headerData[1],
		headerData[2],
	};
	return header;
}

std::vector<QString> GameInfo::CSVReader::ReadRow()
{
	using namespace std;

	//QTextStream textStream(file);
	QString line;
	line = ts->readLine();
	QStringList strList = line.split(",", Qt::SkipEmptyParts);
	vector<QString> strings;
	for (auto& str : strList)
	{
		strings.push_back(str);
	}
	return strings;
}

std::vector<long long> GameInfo::CSVReader::ReadLongLongRow()
{
	using namespace std;
	vector<QString> strings = ReadRow();
	vector<long long> ints;
	for (auto& str : strings)
	{
		ints.push_back(str.toLongLong());
	}
	return ints;
}

bool GameInfo::CSVReader::AtEnd()
{
	return ts->atEnd();
}

void GameInfo::CSVReader::DiscardRow()
{
	ts->readLine();
}

bool GameInfo::patternHeader::operator==(const patternHeader& other)const
{
	if (this->game == other.game && this->difficulty == other.difficulty && this->shotType == other.shotType)
		return true;
	else return false;
}
