#include "StageInfo.h"
#include "logger.h"
#include "Enums.h"
#include <vector>
#include <exception>
#include <QStringList>

StageInfo::StageInfo(int stage)
{
	this->stage = stage;
	currentSection = static_cast<Section>(0);
}

void StageInfo::Init(Game game, int stage)
{
	this->stage = stage;
	switch (game)
	{
	default:
		break;
	case Game::invalid:
		break;
	case Game::th10:

		break;
	case Game::th11:
		break;
	}
}

void StageInfo::SetDeault(int specialCount)
{
	if (sections.size() != 0)
	{
		sections.clear();
	}
	sections.push_back(SectionInfo(Section::All, 0, std::vector<int>(specialCount)));
}

void StageInfo::SetData(Section section, int mode, long long score, const std::vector<int>& specials)
{
	for (auto& si : sections)
	{
		if (si.GetSection() == section)
		{
			si.SetData(mode, score, specials);
			return;
		}
	}
	//没找到：未添加或bonus不存在
	if (mode == 1)//设置路线状态
	{
		for (auto& si : sections)
		{
			if (si.GetSection() == Section::All)
			{
				logger->error("Pattern file incorrect: other section type after all type");
				throw std::runtime_error("Pattern Read error");
			}
		}
		//没有则添加
		SectionInfo newSection(section, score, specials);
		sections.push_back(newSection);
	}
	else if (section == Section::Bonus)
	{
		SetData(Section::Boss, mode, score, specials);
	}
	else
	{
		//throw std::out_of_range("Section error in StageInfo::SetData");
		logger->error("Tring to set data to a stage with no section");
	}

}

void StageInfo::SetData(int mode, long long score, std::vector<int>& specials)
{
	SetData(currentSection, mode, score, specials);
}

void StageInfo::Reset(Section section, int mode)
{
	for (auto& si : sections)
	{
		if (si.GetSection() == section)
		{
			si.Reset(mode);
		}
	}
}

void StageInfo::Reset(int mode)
{
	Reset(currentSection, mode);
}

void StageInfo::ResetAll(int mode)
{
	for (auto& si : sections)
	{
		si.Reset(mode);
	}
}

void StageInfo::ClearSection()
{
	sections.clear();
}

bool StageInfo::CheckEmpty()
{
	return (sections.size() == 0);
}

bool StageInfo::CheckValid()
{
	int size = sections.size();
	switch (size)
	{
	case 1://all
		if (sections.front().GetSection() == Section::All)
			return true;
		break;
	case 2://mid+boss
		if (sections[0].GetSection() == Section::Mid && sections[1].GetSection() == Section::Boss)
			return true;
		break;
	case 3:
		if (sections[0].GetSection() == Section::Mid && sections[1].GetSection() == Section::Boss && sections[2].GetSection() == Section::Bonus)
			return true;
		break;
	default:
		break;
	}
	return false;
}

bool StageInfo::CheckSectionExist(Section section)
{
	for (auto& si : sections)
	{
		if (si.GetSection() == section)
		{
			return true;
		}
	}
	return false;
}

bool StageInfo::SetCurrentSection(Section section)
{
	currentSection = section;
	return CheckSectionExist(section);
}

void StageInfo::SetInitSection()
{
	SetCurrentSection(sections.front().GetSection());
}

Section StageInfo::GetCurrentSection() const
{
	return currentSection;
}

const SectionInfo& StageInfo::GetCurrentSectionInfo() const
{
	for (auto& si : sections)
	{
		if ((si.GetSection() == currentSection) || (si.GetSection() == Section::Bonus) || (si.GetSection() == Section::All))
		{
			return si;
		}
	}
	return sections.back();
}

int StageInfo::GetCurrentSectionIndex() const
{
	for (int i = 0; i < sections.size(); i++)
	{
		if (sections[i].GetSection() == currentSection)
		{
			return i;
		}
	}
	return -1;
}

const SectionInfo& StageInfo::GetPrevSectionInfo() const
{
	for (auto iter = sections.begin(); iter != sections.end(); iter++)
	{
		if (iter->GetSection() == currentSection)
		{
			if (iter != sections.begin())
			{
				return *prev(iter);
			}
			else
			{
				return *iter;
			}
		}
	}
	logger->error("StageInfo::GetPrevSectionInfo() doesn't return a section with a current section: {0}, section size {1}", static_cast<int>(currentSection), sections.size());
	throw std::logic_error("StageInfo::GetPrevSectionInfo() doesn't return a section");
}

//Section StageInfo::GetPrevSection(bool& previousStage)
//{
//	previousStage = false;
//	if (currentSection==Section::All)
//	{
//		return currentSection;
//	}
//	else if (currentSection == Section::Mid)
//	{
//		previousStage = true;
//		return Section::Bonus;
//	}
//	Section previous = static_cast<Section>(static_cast<int>(currentSection) - 1);
//}

int StageInfo::GetStage() const
{
	return stage;
}

int StageInfo::GetSectionCount() const
{
	return sections.size();
}

const std::vector<SectionInfo>& StageInfo::GetSectionInfos()const
{
	return sections;
}

long long StageInfo::GetScore(Section section, int mode) const
{
	for (auto& si : sections)
	{
		if (si.GetSection() == section)
		{
			return si.GetScore(mode);
		}
	}
	//没找到
	if (section == Section::Bonus)
	{
		return GetScore(Section::Boss, mode);
	}
	else
	{
		for (auto& si : sections)
		{
			if (si.GetSection() == Section::All)
			{
				return GetScore(Section::All, mode);
			}
		}
		throw std::out_of_range("Section error in StageInfo::GetScore");
	}
}

long long StageInfo::GetScore(int mode) const
{
	return GetScore(currentSection, mode);
}

const std::vector<int>& StageInfo::GetSpecials(Section section, int mode) const
{
	for (auto& si : sections)
	{
		if (si.GetSection() == section)
		{
			return si.GetSpecials(mode);
		}
	}

	if (section == Section::Bonus)
	{
		return GetSpecials(Section::Boss, mode);
	}
	else
	{
		for (auto& si : sections)
		{
			if (si.GetSection() == Section::All)
			{
				return GetSpecials(Section::All, mode);
			}
		}
		throw std::out_of_range("Section error in StageInfo::GetSpecials");
	}
}

const std::vector<int>& StageInfo::GetSpecials(int mode) const
{
	return GetSpecials(currentSection, mode);
}

QStringList StageInfo::GetSectionNames() const
{
	QStringList list;
	for (auto& var : sections)
	{
		list << var.GetSectionName();
	}
	return list;
}
