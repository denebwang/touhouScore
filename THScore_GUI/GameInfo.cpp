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
#include "spdlog/sinks/rotating_file_sink.h"

void GameInfo::SetPattern(patternHeader header)
{
	using namespace std;
	std::filesystem::path path = patternFileMap.at(header);
	CSVReader reader(path);
	reader.DiscardRow();
	reader.DiscardRow();//skip header
	vector<long long> rowinfo;//只有score会超过21亿，直接转回来没问题
	for (int i = 0; i < 6; i++)
	{
		rowinfo = reader.ReadIntRow();
		PatternInfo[i]->stage = rowinfo[0];
		PatternInfo[i]->score = rowinfo[1];
		PatternInfo[i]->specials.clear();
		for (auto iter = next(rowinfo.begin(),2); iter !=rowinfo.end(); iter++)
		{
			PatternInfo[i]->specials.push_back(*iter);
		}
		
	}
}

GameInfo::GameInfo(game gameName)
{
	difficulty = -1;
	shotType = -1;
	switch (gameName)
	{
	case GameInfo::game::th10:
		this->gameName = gameName;
		shotTypeList = shotTypeMap.at(10);
		for (size_t i = 0; i < 6; i++)
		{
			stageInfo[i].reset(new TH10Info(i+1));
			PatternInfo[i].reset(new TH10Info(i + 1));
			delta[i].reset(new TH10Info(i + 1));
		}
		specialNames = { "Faith" };
		break;
	case GameInfo::game::th11:
		this->gameName = gameName;
		shotTypeList = shotTypeMap.at(11);
		for (size_t i = 0; i < 6; i++)
		{
			stageInfo[i].reset(new TH11Info(i + 1));
			PatternInfo[i].reset(new TH11Info(i + 1));
			delta[i].reset(new TH11Info(i + 1));
		}
		specialNames = { "Faith" , "Graze" };
		break;
	default:
		logger->warn("{0} is not supported yet!", gameName);
		this->gameName = game::invalid;
		break;
	}
}

GameInfo::~GameInfo()
{
}


bool GameInfo::SetInfo(int diff, int shot)
{
	if (diff == difficulty && shot == shotType)
		return false;
	shotType = shot;
	difficulty = diff;
	try 
	{
	SetPattern(GetHeader());
	}
	catch (std::out_of_range e)
	{
		logger->error("can't find csv file: {0}", e.what());
		//todo: 清空当前路线
	}
	return true;
}

void GameInfo::SetData(int stage, long long score, std::vector<int>& specials)
{
	currentStage = stage;
	if (stage == 0)return;
	
	stageInfo[stage-1]->SetData(score, specials);
	//清空没到的面
	if (stage < 6) 
	{
		if (stageInfo[stage]->score != 0)
		{
			for (size_t i = stage; i < 6; i++)
			{
				stageInfo[i]->Reset();
			}
		}
	}
	//UpdateDelta(stage);

		
}

void GameInfo::UpdateDelta(int stage)
{
	if (stage < 1)
		return;
		//throw std::runtime_error("Stage Incorrect");
	//清空本面和没到的面
	if (stage < 6) 
	{
		for (size_t i = stage; i < 6; i++)
		{
			if(delta[i-1]->score!=0)
				delta[i-1]->Reset();
		}
	}
	if (stage == 1)
		return;	
	if (stage == 6)//6面直接更新
		UpdateDelta(7);
	int index = stage - 2;//更新前一面的差值
	if (stageInfo[index]->score == 0)//练习模式前一面不更新
		return;
	int dScore = stageInfo[index]->score - PatternInfo[index]->score;
	std::vector<int> dSpecial;
	for (auto iter1 = stageInfo[index]->specials.begin(), iter2 = PatternInfo[index]->specials.begin();
		iter1 != stageInfo[index]->specials.end() && iter2 != PatternInfo[index]->specials.end();
		iter1++, iter2++)
	{
		dSpecial.push_back(*iter1 - *iter2);
	}

	delta[index]->SetData(dScore, dSpecial);

}

GameInfo::patternHeader GameInfo::GetHeader()
{
	return patternHeader{static_cast<int>(gameName),difficulty,shotType};
}

GameInfo* GameInfo::Create(std::string gameName, DWORD processID, MemoryReader*& mr)
{
	if (gameName == "th10")
	{
		mr = new TH10Reader(processID);
		return new GameInfo(GameInfo::game::th10);
	}
	else if (gameName == "th11")
	{
		mr = new TH11Reader(processID);
		return new GameInfo(GameInfo::game::th11);
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
	for (auto& p : fs::directory_iterator("csv"))
		files.push_back(p.path());
	for (auto file : files)
	{
		QFileInfo fileInfo(file);
		if (fileInfo.suffix()=="csv")
		{
			logger->info("Found a csv file {0}", fileInfo.fileName().toUtf8().data());
			CSVReader reader(file);
			patternHeader header = reader.GetHeader();
			
			patternFileMap.insert(std::make_pair(header,file));
		}
	}
}

void GameInfo::DisplayInfo()
{
	//using namespace std;
	//cout << setfill('-') << setw(80) << " " << endl;

	//cout << setfill(' ') 
	//	<< setw(15) <<  GameName()
	//	<< setw(15) <<  Difficulty()
	//	<< setw(15) <<  ShotType()
	//	<< endl;
	//cout << setfill('-') << setw(80) << " " << endl;
	//cout << setfill(' ') << setw(10) << "Stage" << setw(15) << "Score";
	//stageInfo[0]->DisplaySpecials();
	//for (size_t i = 0; i < 6; i++)
	//{
	//	stageInfo[i]->Display(0);
	//	PatternInfo[i]->Display(1);
	//	delta[i]->Display(2);
	//}
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
	switch (gameName)
	{
	case GameInfo::game::th10:
		return "東方風神録";
		break;
	case GameInfo::game::th11:
		return "東方地霊殿";
		break;
	default:
		break;
	}
	return "";
}



int GameInfo::ColumnCount()
{
	//stage+xx+score+...
	return 3 + stageInfo[0]->specials.size();
}

int GameInfo::RowCount()
{
	//每面3行数据
	return 6 * 3;
}

QStringList GameInfo::GetColumnHeader()
{
	QStringList list;
	list  << "Stage" << " " << "Score";
	for (auto iter = specialNames.begin(); iter != specialNames.end(); iter++)
	{
		list << *iter;
	}
	return list;
}

int GameInfo::CurrentStage()
{
	return currentStage;
}

int GameInfo::CurrentScore()
{
	return stageInfo[currentStage - 1]->score;
}

StageInfo& GameInfo::GetStage(int stage)
{
	return *stageInfo[stage];
}

StageInfo& GameInfo::GetPattern(int index)
{
	return *PatternInfo[index];
}

StageInfo& GameInfo::GetDelta(int index)
{
	return *delta[index];
}

std::vector<int>& GameInfo::CurrentSpecials()
{
	return stageInfo[currentStage - 1]->specials;
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
QString GameInfo::DiffList[4] = { "Easy","Normal","Hard","Lunatic" };
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
	vector<long long> headerData = ReadIntRow();
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
	for each (QString str in strList)
	{
		strings.push_back(str);
	}
	return strings;
}

std::vector<long long> GameInfo::CSVReader::ReadIntRow()
{
	using namespace std;
	vector<QString> strings = ReadRow();
	vector<long long> ints;
	for each (QString str in strings)
	{
		ints.push_back(str.toLongLong());
	}
	return ints;
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
