#pragma once
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <unordered_map>
#include <memory>
#include <array>
#include "StageInfo.h"

class GameInfo
{
public:
	struct patternHeader
	{
		int game;
		int difficulty;
		int shotType;
		bool operator==(const patternHeader& other)const;
	};
	enum class game :int
	{
		th10 = 10,
		th11 = 11
	};
private:

	class CSVReader
	{
	public:
		CSVReader(std::string filename);
		patternHeader GetHeader();
		std::vector<std::string> ReadRow();
		std::vector<int> ReadIntRow();
	private:
		std::ifstream fin;
	};

	static std::unordered_map<patternHeader,std::string> patternFilenameMap;
	static std::string DiffList[4];
	static std::unordered_map<int,std::vector<std::string>> shotTypeMap;

	std::vector<std::string> shotTypeList;
	//StageInfo stageInfo[6];
	//StageInfo PatternInfo[6];//·��
	//StageInfo delta[6];//��·�ߵĲ�ֵ

	std::array<std::unique_ptr<StageInfo>, 6> stageInfo;
	std::array<std::unique_ptr<StageInfo>, 6> PatternInfo;
	std::array<std::unique_ptr<StageInfo>, 6> delta;


	int difficulty;
	int shotType;

	void SetPattern(patternHeader header);
public:
	//GameInfo(std::string gameName);
	GameInfo(game gameName);
	~GameInfo();
	void SetInfo(int diff, int shot);
	void SetData(int stage, int score, std::vector<int> speical);
	void UpdateDelta(int stage);
	patternHeader GetHeader();
	static void ScanCSV();
	static void InitShotTypes();
	void DisplayInfo();
	std::string ShotType();
	std::string Difficulty();
	std::string GameName();

	game gameName;
	
	
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