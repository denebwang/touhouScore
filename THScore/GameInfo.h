#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <array>
#include <filesystem>
#include "StageInfo.h"
#include "MemoryReader.h"
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
	enum class game :int
	{
		invalid=0,
		th10 = 10,
		th11 = 11
	};
private:

	class CSVReader
	{
	public:
		CSVReader(const std::filesystem::path& name);
		~CSVReader();
		patternHeader GetHeader();
		std::vector<QString> ReadRow();
		std::vector<long long> ReadIntRow();
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

	std::array<std::unique_ptr<StageInfo>, 6> stageInfo;
	std::array<std::unique_ptr<StageInfo>, 6> PatternInfo;
	std::array<std::unique_ptr<StageInfo>, 6> delta;


	int difficulty;
	int shotType;

	int currentStage;

	void SetPattern(patternHeader header);
public:
	//GameInfo(std::string gameName);
	GameInfo(game gameName);
	~GameInfo();
	bool SetInfo(int diff, int shot);//需要更新路线信息时返回true
	void SetData(int stage, long long score, std::vector<int>& speical);
	void UpdateDelta(int stage);
	patternHeader GetHeader();
	static GameInfo* Create(std::string gameName, DWORD processID, MemoryReader*& mr);
	static void ScanCSV();
	static void Init();
	void DisplayInfo();
	QString ShotType();
	QString Difficulty();
	QString GameName();

	//用于和表格交互
	int ColumnCount();
	int RowCount();
	QStringList GetColumnHeader();
	std::vector<QString> specialNames;
	int CurrentStage();
	int CurrentScore();
	StageInfo& GetStage(int index);
	StageInfo& GetPattern(int index);
	StageInfo& GetDelta(int index);
	std::vector<int>& CurrentSpecials();
	
	game gameName;
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