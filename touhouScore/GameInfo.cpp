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

StageInfo::StageInfo(int stage, int score, int special)
{
	this->stage = stage;
	this->score = score;
	this->special1 = special;

}

StageInfo::~StageInfo()
{
}


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
		PatternInfo[i]->special1 = rowinfo[2];
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
			stageInfo[i].reset(new TH10Info(i));
			PatternInfo[i].reset(new TH10Info(i));
			delta[i].reset(new TH10Info(i));
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

void GameInfo::SetData(int stage, int score, int special)
{
	stageInfo[stage-1]->SetData(score, special);
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

		
}

void GameInfo::UpdateDelta()
{
	for (size_t i = 0; i < 6; i++)
	{
		if (i < 5)
		{
			if (stageInfo[i+1]->score == 0)
				continue;
		}
		int dScore = stageInfo[i]->score - PatternInfo[i]->score;
		int dSpecial = stageInfo[i]->special1 - PatternInfo[i]->special1;

		delta[i]->SetData(dScore, dSpecial);
	}
}

GameInfo::patternHeader GameInfo::GetHeader()
{
	return patternHeader{static_cast<int>(gameName),difficulty,shotType};
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
		<< setw(20) <<  GameName()
		<< setw(20) <<  Difficulty()
		<< setw(20) <<  ShotType()
		<< endl;
	cout << setfill('-') << setw(80) << " " << endl;
	cout << setfill(' ') << setw(20) << "Stage" << setw(20) << "Score";
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
		return std::string("–|·½ïLÉñåh");
		break;
	default:
		break;
	}
	return std::string();
}

void GameInfo::InitShotTypes()
{
	shotTypeMap.insert(std::make_pair<int, std::vector<std::string>>(10, { "Reimu A","Reimu B","Reimu C","Marisa A","Marisa B","Marisa C" }));
	shotTypeMap.insert(std::make_pair<int, std::vector<std::string>>(11, { "Reimu A","Reimu B","Reimu C","Marisa A","Marisa B","Marisa C" }));
}

std::unordered_map<int, std::vector<std::string>> GameInfo::shotTypeMap;
std::string GameInfo::DiffList[4] = { "Easy","Normal","Hard","Lunatic" };
std::unordered_map<GameInfo::patternHeader, std::string> GameInfo::patternFilenameMap;




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

TH10Info::TH10Info(const TH10Info& other)
{
	stage = other.stage;
	score = other.score;
	special1 = other.special1;
}

TH10Info::TH10Info(int stage, int score, int faith) :StageInfo(stage, score, faith)
{
	
}

TH10Info& TH10Info::operator=(const TH10Info& other)
{
	stage = other.stage;
	score = other.score;
	special1 = other.special1;
	return *this;
}

void TH10Info::SetData(int score, int faith)
{
	this->score = score;
	this->special1 = faith;
}

void TH10Info::Reset()
{
	*this = TH10Info();
}

void TH10Info::DisplaySpecials()
{
	std::cout<< std::setw(20) << "Faith\n";
}

void TH10Info::Display(int mode)
{
	if(mode==0)
		std::cout << std::setw(20) << stage;
	else if (mode == 1)
		std::cout << std::setw(20) << "Pattern";
	else if (mode==2)
		std::cout << std::setw(20) << "Delta";
	else {
		logger->error("Display mode error: {0}", mode);
		return;
	}
	std::cout << std::setw(20) << score
		<< std::setw(20) << special1
		<< std::endl;
}
