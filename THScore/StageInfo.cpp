﻿#include "StageInfo.h"
#include "logger.h"
#include "Enums.h"
#include <vector>
#include <exception>

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

void StageInfo::SetData(Section section, int mode, long long score, std::vector<int>& specials)
{
	for (auto &si : sections)
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
		for (auto &si : sections)
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
	else if (section==Section::Bonus)
	{
		SetData(Section::Boss, mode, score, specials);
	}
	else throw std::out_of_range("Section error in StageInfo::SetData");

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

void StageInfo::SetCurrentSection(Section section)
{
	currentSection = section;
}

void StageInfo::SetInitSection()
{
	SetCurrentSection(sections.front().GetSection());
}

Section StageInfo::GetCurrentSection()
{
	return currentSection;
}

int StageInfo::GetCurrentSectionIndex()
{
	for (int i =0; i < sections.size(); i++)
	{
		if (sections[i].GetSection() == currentSection)
		{
			return i;
		}
	}
	return -1;
}

Section StageInfo::GetPrevSection(bool& previousStage)
{
	previousStage = false;
	if (currentSection==Section::All)
	{
		return currentSection;
	}
	else if (currentSection == Section::Mid)
	{
		previousStage = true;
		return Section::Bonus;
	}
	Section previous = static_cast<Section>(static_cast<int>(currentSection) - 1);
}

int StageInfo::GetStage()
{
	return stage;
}

int StageInfo::GetSectionCount()
{
	return sections.size();
}

const std::vector<SectionInfo>& StageInfo::GetSectionInfos()const
{
	return sections;
}

long long StageInfo::GetScore(Section section, int mode)
{
	for (auto &si : sections)
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
		for (auto &si : sections)
		{
			if (si.GetSection() == Section::All)
			{
				return GetScore(Section::All, mode);
			}
		}
		throw std::out_of_range("Section error in StageInfo::GetScore");
	}
}

long long StageInfo::GetScore(int mode)
{
	return GetScore(currentSection, mode);
}

const std::vector<int>& StageInfo::GetSpecials(Section section, int mode)
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

const std::vector<int>& StageInfo::GetSpecials(int mode)
{
	return GetSpecials(currentSection, mode);
}

QStringList StageInfo::GetSectionNames()
{
	QStringList list;
	for (auto &var : sections)
	{	
		QString temp;
		switch (var.GetSection())
		{
		default:
			break;
		case Section::All:
			temp = "All";
			break;
		case Section::Mid:
			temp = "Mid";
			break;
		case Section::Boss:
			temp = "Boss";
			break;
		case Section::Bonus:
			temp = "Bonus";
			break;
		}
		list << temp;
	}
	return list;
}
