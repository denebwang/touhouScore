#pragma once
#include<vector>
#include<string>
class StageInfo
{
public:
	StageInfo(int stage = 0, int score = 0, std::vector<int> specials = {});
	~StageInfo();

	virtual void SetData(int score, std::vector<int>& specials) = 0;
	virtual void Reset() = 0;
	virtual void DisplaySpecials() = 0;
	virtual void Display(int mode) = 0;
	int stage;
	int score;
	std::vector<int> specials;
};

class TH10Info :public StageInfo
{
public:
	const std::string specialName1 = "faith";

	TH10Info(const TH10Info& other);
	TH10Info(int stage = 0, int score = 0, int faith = 0);

	TH10Info& operator=(const TH10Info& other);
	// 通过 StageInfo 继承
	void SetData(int score, std::vector<int>& specials) override;//special只有一个faith
	void Reset() override;
	void DisplaySpecials() override;
	void Display(int mode) override;
};

class TH11Info :public StageInfo
{
public:
	const std::string specialName1 = "faith";
	const std::string specialName2 = "graze";

	//TH11Info(const TH10Info& other);
	TH11Info(int stage = 0, int score = 0, int faith = 0, int graze = 0);

	//TH11Info& operator=(const TH10Info& other);
	// 通过 StageInfo 继承
	void SetData(int score, std::vector<int>& specials) override;
	void Reset() override;
	void DisplaySpecials() override;
	void Display(int mode) override;
};
