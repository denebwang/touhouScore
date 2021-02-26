#pragma once
#include <vector>
#include <memory>
#include <QString>
#include <QStringList>
#include "SectionInfo.h"
#include "Enums.h"
class StageInfo
{
public:
	StageInfo(int stage = 0);

	
	//初始化
	void Init(Game game, int stage);

	//与section交互:
	void SetData(int mode, long long score, std::vector<int>& specials);
	void SetData(Section section, int mode, long long score, std::vector<int>& specials);
	void Reset(Section section, int mode);
	void Reset(int mode);
	void ResetAll(int mode = 0);
	//todo: get和set中状态向all坍缩
	long long GetScore(Section section, int mode);
	long long GetScore(int mode);
	std::vector<int>& GetSpecials(Section section, int mode);
	std::vector<int>& GetSpecials(int mode);
	QStringList GetSectionNames();

	void ClearSection();
	bool CheckEmpty();
	bool CheckValid();
	void SetCurrentSection(Section section);
	void SetInitSection();
	Section GetCurrentSection();
	int GetCurrentSectionIndex();
	Section GetPrevSection(bool& previousStage);
	int GetStage();
	int GetSectionCount();
	const std::vector<SectionInfo>& GetSectionInfos()const;

private:
	std::vector<SectionInfo> sections;
	int stage;
	Section currentSection;
};

