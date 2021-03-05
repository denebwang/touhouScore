#include <vector>
#include <array>
#include <exception>
#include "logger.h"
#include "SectionInfo.h"
#include "Enums.h"


SectionInfo::SectionInfo(Section section, long long patternScore, std::vector<int> patternSpecials)
{
	this->section = section;
	this->scores = { 0,patternScore,0 };
	int size = patternSpecials.size();
	std::vector<int> zeros(size);
	this->specials = { zeros,patternSpecials,zeros };
}

SectionInfo::SectionInfo(Section section /*= Section::All*/, std::array<long long, 3>& scores /*= { 0,0,0 }*/, std::array<std::vector<int>, 3>& specials /*= {}*/)
{
	this->section = section;
	this->scores = scores;
	this->specials = specials;
}

SectionInfo::~SectionInfo()
{
}

void SectionInfo::SetData(int mode, long long score, const std::vector<int>& specials)
{
	try
	{
		this->scores[mode] = score;
		this->specials[mode] = specials;
	}
	catch (std::out_of_range& e)
	{
		logger->error("SectionInfo::SetData mode out of range: mode == {0}", mode);
	}
}

void SectionInfo::SetSection(Section section)
{
	this->section = section;
}

void SectionInfo::Reset(int mode)
{
	try
	{
		scores[mode] = 0;
		int size = specials[mode].size();
		specials[mode].clear();
		specials[mode].resize(size);
	}
	catch (std::out_of_range& e)
	{
		logger->error("SectionInfo::Reset mode out of range: mode == {0}", mode);
	}
}

const Section SectionInfo::GetSection()const
{
	return section;
}

QString SectionInfo::GetSectionName() const
{
	switch (section)
	{
	default:
		logger->error("SectionInfo::GetSectionName() error");
		break;
	case Section::All:
		return "All";
		break;
	case Section::Mid:
		return "Mid";
		break;
	case Section::Boss:
		return "Boss";
		break;
	case Section::Bonus:
		return "Bonus";
		break;
	}
}

const long long SectionInfo::GetScore(int mode)const
{
	return scores[mode];
}

const std::vector<int>& SectionInfo::GetSpecials(int mode)const
{
	return specials[mode];
}

//
//TH10Info::TH10Info(int stage, int score, int faith) :SectionInfo(stage, score, { faith })
//{
//
//}
//
//void TH10Info::SetData(int score, std::vector<int>& specials)
//{
//	this->score = score;
//	this->specials = specials;
//}
//
//void TH10Info::Reset()
//{
//	this->score = 0;
//	this->specials = { 0 };
//}
//
//
//TH11Info::TH11Info(int stage, int score, int faith, int graze)
//{
//	this->stage = stage;
//	this->score = score;
//	this->specials = { faith,graze };
//}
//
//void TH11Info::SetData(int score, std::vector<int>& specials)
//{
//	this->score = score;
//	this->specials = specials;
//}
//
//void TH11Info::Reset()
//{
//	this->score = 0;
//	this->specials = { 0,0 };
//}
