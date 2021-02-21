#include <vector>
#include <iomanip>
#include <iostream>


#include "logger.h"
#include "StageInfo.h"
StageInfo::StageInfo(int stage, int score, std::vector<int> specials)
{
	this->stage = stage;
	this->score = score;
	this->specials = specials;

}

StageInfo::~StageInfo()
{
}



TH10Info::TH10Info(const TH10Info& other)
{
	stage = other.stage;
	score = other.score;
	specials = other.specials;
}

TH10Info::TH10Info(int stage, int score, int faith) :StageInfo(stage, score, { faith })
{

}

TH10Info& TH10Info::operator=(const TH10Info& other)
{
	stage = other.stage;
	score = other.score;
	specials = other.specials;
	return *this;
}

void TH10Info::SetData(int score, std::vector<int>& specials)
{
	this->score = score;
	this->specials = specials;
}

void TH10Info::Reset()
{
	this->score = 0;
	this->specials = { 0 };
}

void TH10Info::DisplaySpecials()
{
	std::cout << std::setw(15) << "Faith\n";
}

void TH10Info::Display(int mode)
{
	if (mode == 0)
		std::cout << std::setw(10) << stage;
	else if (mode == 1)
		std::cout << std::setw(10) << "Pattern";
	else if (mode == 2)
		std::cout << std::setw(10) << "Delta";
	else {
		logger->error("Display mode error: {0}", mode);
		return;
	}
	std::cout << std::setw(15) << score
		<< std::setw(15) << specials[0]
		<< std::endl;
}


TH11Info::TH11Info(int stage, int score, int faith, int graze)
{
	this->stage = stage;
	this->score = score;
	this->specials = { faith,graze };
}

void TH11Info::SetData(int score, std::vector<int>& specials)
{
	this->score = score;
	this->specials = specials;
}

void TH11Info::Reset()
{
	this->score = 0;
	this->specials = { 0,0 };
}

void TH11Info::DisplaySpecials()
{
	std::cout << std::setw(15) << "Faith"
		<< std::setw(15) << "Graze\n";
}

void TH11Info::Display(int mode)
{
	
	if (mode == 0)
	{
		
		std::cout << std::setw(10) << stage;
	}
	else if (mode == 1)
	{
		
		std::cout << std::setw(10) << "Pattern";
	}
	else if (mode == 2)
	{
		
		std::cout << std::setw(10) << "Delta";
	}
	else {
		logger->error("Display mode error: {0}", mode);
		return;
	}
	std::cout << std::setw(15) << score
		<< std::setw(15) << specials[0]
		<< std::setw(15) << specials[1]
		<< std::endl;
}

