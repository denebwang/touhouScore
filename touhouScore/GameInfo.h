#pragma once
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <unordered_map>
#include <memory>
#include <array>

class StageInfo
{
public:
	StageInfo(int stage = 0, int score = 0, int special = 0);
	~StageInfo();

	virtual void SetData(int score, int special) = 0;
	virtual void Reset()=0;
	virtual void DisplaySpecials() = 0;
	virtual void Display(int mode) = 0;
	int stage;
	int score;
	int special1;
};

class TH10Info:public StageInfo
{
public:
	const std::string specialName1="faith";

	TH10Info(const TH10Info& other);
	TH10Info(int stage = 0, int score = 0, int faith = 0);

	TH10Info& operator=(const TH10Info& other);
	// 通过 StageInfo 继承
	void SetData(int score, int faith) override;
	void Reset() override;
	void DisplaySpecials() override;
	void Display(int mode) override;
};

class TH11Info:public StageInfo
{
public:
	const std::string specialName1="faith";

	TH11Info(const TH10Info& other);
	TH11Info(int stage = 0, int score = 0, int faith = 0, int graze = 0);

	TH11Info& operator=(const TH10Info& other);
	// 通过 StageInfo 继承
	void SetData(int score, int faith) override;
	void Reset() override;
	void DisplaySpecials() override;
};

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
	//StageInfo PatternInfo[6];//路线
	//StageInfo delta[6];//和路线的差值

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
	void SetData(int stage, int score, int speical);
	void UpdateDelta();
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