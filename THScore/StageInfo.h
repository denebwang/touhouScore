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
	void SetDeault(int specialCount);
	//与section交互:
	void SetData(int mode, long long score, std::vector<int>& specials);
	void SetData(Section section, int mode, long long score, std::vector<int>& specials);
	void Reset(Section section, int mode);
	void Reset(int mode);
	void ResetAll(int mode = 0);
	//todo: get和set中状态向all坍缩
	long long GetScore(Section section, int mode) const;
	long long GetScore(int mode) const;
	const std::vector<int>& GetSpecials(Section section, int mode) const;
	const std::vector<int>& GetSpecials(int mode) const;
	QStringList GetSectionNames() const;

	void ClearSection();
	bool CheckEmpty();
	bool CheckValid();
	bool CheckSectionExist(Section section);
	bool SetCurrentSection(Section section);
	void SetInitSection();
	Section GetCurrentSection()const;
	const SectionInfo& GetCurrentSectionInfo()const;
	int GetCurrentSectionIndex() const ;
	//Section GetPrevSection(bool& previousStage);
	const SectionInfo& GetPrevSectionInfo()const;
	int GetStage()const;
	int GetSectionCount()const;
	const std::vector<SectionInfo>& GetSectionInfos()const;

private:
	std::vector<SectionInfo> sections;
	int stage;
	Section currentSection;
};

