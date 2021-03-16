#pragma once
#include "Enums.h"
#include <QString>
class UFOInfo
{
public:
	UFOInfo(int ufo1, int ufo2, int ufo3);
	UFOInfo(UFO ufo);
	~UFOInfo();
	//getters
	UFO GetUFOType();
	QString GetUFOTypeName();
	int GetPowerItemCount();
	int GetPointItemCount();
	int GetBonusScore();
	int GetStage();
	int GetPIV();
	int GetPower();
	//setters
	void SetUFOType(int ufo1, int ufo2, int ufo3);
	void SetPowerItemCount(int value);
	void SetPointItemCount(int value);
	void SetStage(int stage);
	void SetPIV(int value);
	void SetPower(int value);
private:
	UFO ufo;//飞蝶类型
	int powerItemCount;
	int pointItemCount;
	int stage;
	int PIV;//最大得点
	int power;//用于计算是否满p
};
