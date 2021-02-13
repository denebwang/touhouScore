#include "GameInfo.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <vector>
#include <exception>
#include "logger.h"
#include "spdlog/sinks/rotating_file_sink.h"
//auto GIlogger = spdlog::rotating_logger_mt("GameInfo.cpp", "logs/log.txt", 1024 * 1024 * 5, 5);

StageInfo::StageInfo(int stage, int score, int faith)
{
	this->stage = stage;
	this->score = score;
	this->faith = faith;

}

StageInfo::~StageInfo()
{
}

void StageInfo::SetData(int score, int faith)
{
	this->score = score;
	this->faith = faith;
}

void StageInfo::Reset()
{
	*this = StageInfo();
}

void GameInfo::SetPattern(patternHeader header)
{
	using namespace std;
	string filename = patternFilenameMap.at(header);
	CSVReader reader(filename);
	reader.ReadRow();
	reader.ReadRow();//skip header
	vector<string> rowinfo;
	for (int i = 0; i < 6; i++)
	{
		rowinfo = reader.ReadRow();
		stringstream ssStage, ssScore, ssFaith;
		ssStage << rowinfo[0];
		ssScore << rowinfo[1];
		ssFaith << rowinfo[2];
		ssStage >> PatternInfo[i].stage;
		ssScore >> PatternInfo[i].score;
		ssFaith >> PatternInfo[i].faith;
	}
}

GameInfo::GameInfo(std::string gameName)
{
	this->gameName = gameName;
	for (size_t i = 0; i < 6; i++)
	{
		stageInfo[i].stage = i+1;
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

void GameInfo::SetData(int stage, int score, int faith)
{
	stageInfo[stage-1].SetData(score, faith);
	if (stageInfo[stage].score != 0)
	{
		for (size_t i = stage; i < 6; i++)
		{
			stageInfo[i].Reset();
		}
	}
		
}

void GameInfo::UpdateDelta()
{
	for (size_t i = 0; i < 5; i++)
	{
		if (stageInfo[i+1].score == 0)
			continue;
		int dScore = stageInfo[i].score - PatternInfo[i].score;
		int dFaith = stageInfo[i].faith - PatternInfo[i].faith;

		delta[i].SetData(dScore, dFaith);
	}
}

GameInfo::patternHeader GameInfo::GetHeader()
{
	return patternHeader{gameName,difficulty,shotType};
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

	cout << setfill(' ') << setw(15) << "Game: " << gameName
		<< setw(15) << "ShotType: " << Difficulty()
		<< setw(15) << "ShotType: " << ShotType()
		<< endl;
	cout << setfill('-') << setw(80) << " " << endl;
	cout << setfill(' ') << setw(20) << "Stage" << setw(20) << "Score" << setw(20) << "Faith\n";
	for (size_t i = 0; i < 6; i++)
	{
		cout << setw(20) << stageInfo[i].stage
			 << setw(20) << stageInfo[i].score
			 << setw(20) << stageInfo[i].faith
			 << endl;
		cout << setw(20) << "Pattern"
			 << setw(20) << PatternInfo[i].score
			 << setw(20) << PatternInfo[i].faith
			 << endl;
		cout << setw(20) << "Delta"
			 << setw(20) << delta[i].score
			 << setw(20) << delta[i].faith
			 << endl;
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

std::string GameInfo::shotTypeList[6] = { "Reimu A","Reimu B","Reimu C","Marisa A","Marisa B","Marisa C" };
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
	string game;
	int diff, shot;
	vector<string> headerStrings = ReadRow();
	game = headerStrings[0];
	stringstream ss;
	ss << headerStrings[1];
	ss >> diff;
	ss.clear();
	ss.str("");
	ss << headerStrings[2];
	ss >> shot;
	patternHeader header = { game,diff,shot };
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

bool GameInfo::patternHeader::operator==(const patternHeader& other)const
{
	if (this->gameName == other.gameName && this->difficulty == other.difficulty && this->shotType == other.shotType)
		return true;
	else return false;
}

