﻿#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <array>
#include <filesystem>
#include "SectionInfo.h"
#include "StageInfo.h"
#include "MemoryReader.h"
#include "Enums.h"
#include <QString>
#include <QStringList>
#include <QFile>
#include <QTextStream>

class GameInfo
{
public:
	//用于查找路线
	struct patternHeader
	{
		int game;
		int difficulty;
		int shotType;
		bool operator==(const patternHeader& other)const;
	};

private:
	class CSVReader
	{
	public:
		CSVReader(const std::filesystem::path& name);
		~CSVReader();
		patternHeader GetHeader();
		std::vector<QString> ReadRow();
		std::vector<long long> ReadLongLongRow();
		bool AtEnd();
		void DiscardRow();
	private:
		QFile* file;
		QTextStream* ts;
	};

	static std::unordered_map<patternHeader, std::filesystem::path> patternFileMap;
	static QString DiffList[4];
	static std::unordered_map<int,std::vector<QString>> shotTypeMap;
	
	std::vector<QString> shotTypeList;
	//StageInfo stageInfo[6];
	//StageInfo PatternInfo[6];//路线
	//StageInfo delta[6];//和路线的差值

	std::array<StageInfo, 6> stageInfo;


	int difficulty;
	int shotType;

	int currentStage;

	void SetPattern(patternHeader header);
public:
	//GameInfo(std::string gameName);
	GameInfo(Game game);
	~GameInfo();
	bool SetInfo(int diff, int shot);//需要更新路线信息时返回true
	void SetData(int stage, long long score, std::vector<int>& speical);
	void TestSection(int bossHP, int timeLeft,int frameCount);
	void UpdateDelta(int stage);
	patternHeader GetHeader();
	static GameInfo* Create(std::string gameName, DWORD processID, MemoryReader*& mr);
	static void ScanCSV();
	static void Init();
	
	QString ShotType();
	QString Difficulty();
	QString GameName();

	//用于和表格交互	
	std::vector<QString> specialNames;

	int ColumnCount();
	int RowCount();
	QStringList GetColumnHeader();
	int GetCurrentStage();
	int GetStageSectionCount(int index);
	QStringList GetSectionNames(int index);
	Section GetCurrentSection(int index);
	int GetCurrentSectionIndex(int index);
	const std::vector<SectionInfo>& GetSectionInfos(int index);
	
	Game game;
	static std::unordered_map<std::string, std::vector<std::wstring>> exeMap;//游戏文件名
	
};

namespace std {
	template <>
	struct hash<GameInfo::patternHeader> {
		std::size_t operator()(const GameInfo::patternHeader& header) const {
			using std::size_t;
			using std::hash;
			using std::string;
			// Compute individual hash values for first,
			// second and third and combine them using XOR
			// and bit shifting:
			return ((hash<int>()(header.game)
				^ (hash<int>()(header.difficulty) << 1)) >> 1)
				^ (hash<int>()(header.shotType) << 1);
		}
	};
}