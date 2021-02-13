#pragma once
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <unordered_map>

class StageInfo
{
public:
	StageInfo(int stage = 0, int score = 0, int faith = 0);
	~StageInfo();

	void SetData(int score, int faith);
	void Reset();
	int stage;
	int score;
	int faith;
};

class TH10Info:public StageInfo{};

class GameInfo
{
public:
	struct patternHeader
	{
		std::string gameName;
		int difficulty;
		int shotType;
		bool operator==(const patternHeader& other)const;
	};
private:

	class CSVReader
	{
	public:
		CSVReader(std::string filename);
		patternHeader GetHeader();
		std::vector<std::string> ReadRow();
	private:
		std::ifstream fin;
	};

	static std::unordered_map<patternHeader,std::string> patternFilenameMap;
	static std::string DiffList[4];
	static std::string shotTypeList[6];

	StageInfo stageInfo[6];
	StageInfo PatternInfo[6];//路线
	StageInfo delta[6];//和路线的差值

	int difficulty;
	int shotType;

	void SetPattern(patternHeader header);
public:
	GameInfo(std::string gameName);
	~GameInfo();
	void SetInfo(int diff, int shot);
	void SetData(int stage, int score, int faith);
	void UpdateDelta();
	patternHeader GetHeader();
	static void ScanCSV();
	void DisplayInfo();
	std::string ShotType();
	std::string Difficulty();

	std::string gameName;
	
	
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
			return ((hash<string>()(header.gameName)
				^ (hash<int>()(header.difficulty) << 1)) >> 1)
				^ (hash<int>()(header.shotType) << 1);
		}
	};
}