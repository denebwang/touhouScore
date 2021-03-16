#pragma once
#include <string>
#include <vector>
#include <unordered_map>
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

//用于查找路线
struct PatternHeader
{
	int game;
	int difficulty;
	int shotType;
	bool operator==(const PatternHeader& other)const;
	//QString ToQString();
};

class GameInfo
{
private:
	class CSVReader
	{
	public:
		CSVReader(const std::filesystem::path& name);
		~CSVReader();
		PatternHeader GetHeader();
		std::vector<QString> ReadRow();
		std::vector<long long> ReadLongLongRow();
		bool AtEnd();
		void DiscardRow();
	private:
		QFile* file;
		QTextStream* ts;
	};

	static std::unordered_map<PatternHeader, std::filesystem::path> patternFileMap;

	static std::unordered_map<int, std::vector<QString>> shotTypeMap;

	std::vector<QString> shotTypeList;
	//StageInfo stageInfo[6];
	//StageInfo PatternInfo[6];//路线
	//StageInfo delta[6];//和路线的差值

	std::array<StageInfo, 6> stageInfo;

	int difficulty;
	int shotType;

	int currentStage;

	bool SetPattern(PatternHeader header);
public:
	//GameInfo(std::string gameName);
	GameInfo(Game game);
	~GameInfo();
	bool CheckRetry(int stage, int frame);
	bool SetInfo(int diff, int shot);//需要更新路线信息时返回true
	bool SetData(int stage, long long score, std::vector<int>& speical);
	void SetPattern(int stage, Section section, long long score, std::vector<int>& speical);
	bool TestSection(int bossHP, int timeLeft, int frameCount, int localFrame);
	void UpdateDelta(int stage);
	void Clear();
	PatternHeader GetHeader();
	static GameInfo* Create(std::string gameName, DWORD processID, MemoryReader*& mr);
	static void ScanCSV();
	static void Init();

	QString ShotType();
	QString Difficulty();
	QString GameName();
	static QString GameName(Game game);

	static const std::vector<QString>& GetShotTypeList(int gameNum);
	const std::vector<QString>& GetShotTypeList()const;

	//用于和表格交互
	std::vector<QString> specialNames;
	const static QString DiffList[4];

	int ColumnCount();
	int RowCount();
	int SectionCount();
	QStringList GetColumnHeader()const;
	QStringList GetSpecialNames()const;
	int GetCurrentStage()const;
	int GetStageSectionCount(int index)const;
	int GetCurrenSectionRowIndex()const;
	QStringList GetSectionNames(int index)const;
	Section GetCurrentSection(int index)const;
	const SectionInfo& GetCurrentSectionInfo(int index)const;
	const SectionInfo& GetPrevSectionInfo(int index)const;
	int GetCurrentSectionIndex(int index)const;
	const std::vector<SectionInfo>& GetSectionInfos(int index)const;
	StageInfo* GetStageInfo(int index);
	static const std::unordered_map< PatternHeader, std::filesystem::path >& GetPatternFileMap();

	Game game;
	static std::unordered_map<std::string, std::vector<std::wstring>> exeMap;//游戏文件名
};

namespace std {
	template <>
	struct hash<PatternHeader> {
		std::size_t operator()(const PatternHeader& header) const {
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