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
#include "logger.h"
#include "spdlog/sinks/rotating_file_sink.h"

void GameInfo::SetPattern(patternHeader header)
{
	using namespace std;
	string filename = patternFilenameMap.at(header);
	CSVReader reader(filename);
	reader.ReadRow();
	reader.ReadRow();//skip header
	vector<int> rowinfo;
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
		this->gameName = game::th10;
		shotTypeList = shotTypeMap.at(10);
		for (size_t i = 0; i < 6; i++)
		{
			stageInfo[i].reset(new TH10Info(i+1));
			PatternInfo[i].reset(new TH10Info(i + 1));
			delta[i].reset(new TH10Info(i + 1));
		}
		break;
	case GameInfo::game::th11:
		this->gameName = game::th11;
		shotTypeList = shotTypeMap.at(11);
		for (size_t i = 0; i < 6; i++)
		{
			stageInfo[i].reset(new TH11Info(i + 1));
			PatternInfo[i].reset(new TH11Info(i + 1));
			delta[i].reset(new TH11Info(i + 1));
		}
		break;
	default:
		logger->warn("{0} is not supported yet!", gameName);
		break;
	}
}

GameInfo::~GameInfo()
{
}


void GameInfo::SetInfo(int diff, int shot)
{
	if (diff == difficulty && shot == shotType)
		return;
	shotType = shot;
	difficulty = diff;
	try 
	{
	SetPattern(GetHeader());
	}
	catch (std::out_of_range e)
	{
		logger->error("can't find csv file: {0}", e.what());
	}
}

void GameInfo::SetData(int stage, int score, std::vector<int> specials)
{
	if (stage == 0)return;
	
	stageInfo[stage-1]->SetData(score, specials);
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
	UpdateDelta(stage);

		
}

void GameInfo::UpdateDelta(int stage)
{
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
	if (stage == 6)//6面一起更新
		UpdateDelta(7);
}

GameInfo::patternHeader GameInfo::GetHeader()
{
	return patternHeader{static_cast<int>(gameName),difficulty,shotType};
}

GameInfo GameInfo::Create(std::string gameName, DWORD processID, MemoryReader*& mr)
{
	if (gameName == "th10")
	{
		mr = new TH10Reader(processID);
		return GameInfo(GameInfo::game::th10);
	}
	else if (gameName == "th11")
	{
		mr = new TH11Reader(processID);
		return GameInfo(GameInfo::game::th11);
	}
	else {
		logger->error("Game Not supported: {0}", gameName);
	}
}

void GameInfo::ScanCSV()
{
	using namespace std;
	namespace fs = filesystem;
	vector<string> filenames;
	for (auto& p : fs::directory_iterator("."))
		filenames.push_back(p.path().filename().string());
	for (string filename : filenames)
	{
		if (filename.substr(filename.find_last_of(".") + 1) == "csv")
		{
			logger->info("Found a csv file {0}", filename);
			CSVReader reader(filename);
			patternHeader header = reader.GetHeader();
			
			patternFilenameMap.insert(std::make_pair(header,filename));
		}
	}
}

void GameInfo::DisplayInfo()
{
	using namespace std;
	cout << setfill('-') << setw(80) << " " << endl;

	cout << setfill(' ') 
		<< setw(15) <<  GameName()
		<< setw(15) <<  Difficulty()
		<< setw(15) <<  ShotType()
		<< endl;
	cout << setfill('-') << setw(80) << " " << endl;
	cout << setfill(' ') << setw(10) << "Stage" << setw(15) << "Score";
	stageInfo[0]->DisplaySpecials();
	for (size_t i = 0; i < 6; i++)
	{
		stageInfo[i]->Display(0);
		PatternInfo[i]->Display(1);
		delta[i]->Display(2);
	}
}

std::string GameInfo::ShotType()
{
	return shotTypeList[shotType];
}

std::string GameInfo::Difficulty()
{
	return DiffList[difficulty];
}

std::string GameInfo::GameName()
{
	switch (gameName)
	{
	case GameInfo::game::th10:
		return std::string("東方風神録");
		break;
	case GameInfo::game::th11:
		return std::string("東方地霊殿");
		break;
	default:
		break;
	}
	return std::string();
}



void GameInfo::Init()
{
	//机体列表
	shotTypeMap.insert(std::make_pair<int, std::vector<std::string>>(10, { "Reimu A","Reimu B","Reimu C","Marisa A","Marisa B","Marisa C" }));
	shotTypeMap.insert(std::make_pair<int, std::vector<std::string>>(11, { "Reimu A","Reimu B","Reimu C","Marisa A","Marisa B","Marisa C" }));

	exeMap.insert(std::make_pair< std::string, std::vector<std::string >>("th10", { "th10.exe","th10chs.exe","th10cht.exe" }));
	exeMap.insert(std::make_pair< std::string, std::vector<std::string >>("th11", { "th11.exe","th11c" }));
}

std::unordered_map<int, std::vector<std::string>> GameInfo::shotTypeMap;
std::string GameInfo::DiffList[4] = { "Easy","Normal","Hard","Lunatic" };
std::unordered_map<GameInfo::patternHeader, std::string> GameInfo::patternFilenameMap;
std::unordered_map<std::string, std::vector<std::string>> GameInfo::exeMap;




GameInfo::CSVReader::CSVReader(std::string filename)
{
	fin.open(filename);
	if (!fin.is_open())
		logger->error("{0} is not opened.", filename);
}

GameInfo::patternHeader GameInfo::CSVReader::GetHeader()
{
	using namespace std;
	vector<int> headerData = ReadIntRow();
	patternHeader header = 
	{ 
		headerData[0],
		headerData[1],
		headerData[2],
	};
	return header;
}

std::vector<std::string> GameInfo::CSVReader::ReadRow()
{
	using namespace std;
	string line;
	getline(fin, line);
	istringstream iss(line);
	string tempinfo;
	vector<string> strings;
	while (getline(iss, tempinfo, ','))
		strings.push_back(tempinfo);
	return strings;
}

std::vector<int> GameInfo::CSVReader::ReadIntRow()
{
	using namespace std;
	vector<string> strings = ReadRow();
	vector<int> ints;
	stringstream ss;
	for (auto iter=strings.begin();iter!=strings.end();iter++)
	{
		int temp;
		ss << *iter;
		ss >> temp;
		ints.push_back(temp);
		ss.clear();
		ss.str("");
	}
	return ints;
}

bool GameInfo::patternHeader::operator==(const patternHeader& other)const
{
	if (this->game == other.game && this->difficulty == other.difficulty && this->shotType == other.shotType)
		return true;
	else return false;
}
