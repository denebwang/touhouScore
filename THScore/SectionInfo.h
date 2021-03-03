#pragma once
#include<vector>
#include <array>
#include<string>
#include "Enums.h"

class SectionInfo
{
public:
	SectionInfo(Section section, std::array<long long, 3>& scores, std::array<std::vector<int>, 3>& specials);
	SectionInfo(Section section, long long patternScore, std::vector<int> patternSpecials);
	~SectionInfo();

	void SetData(int mode, long long score, std::vector<int>& specials);
	void Reset(int mode = 0);


	const Section GetSection()const;
	const long long GetScore(int mode)const;
	//long long GetScore(int mode);
	const std::vector<int>& GetSpecials(int mode)const;
	//int stage;
	//longlong防止21亿溢出
	//0=游戏内信息,1=路线信息,2=与路线差值

	//long long score;
	//long long patternScore;
	//long long deltaScore;
	//std::vector<int> specials;
	//std::vector<int> patternSpecials;
	//std::vector<int> deltaSpecials;
private:
	Section section;
	std::array<long long, 3> scores;
	std::array<std::vector<int>, 3> specials;
};
//
//class TH10Info :public SectionInfo
//{
//public:
//
//	TH10Info(int stage = 0, int score = 0, int faith = 0);
//
//	// 通过 StageInfo 继承
//	void SetData(int score, std::vector<int>& specials) override;
//	void Reset() override;
//};
//
//class TH11Info :public SectionInfo
//{
//public:
//
//	TH11Info(int stage = 0, int score = 0, int faith = 0, int graze = 0);
//
//	// 通过 StageInfo 继承
//	void SetData(int score, std::vector<int>& specials) override;
//	void Reset() override;
//};
