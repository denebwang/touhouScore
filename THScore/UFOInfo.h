#pragma once
#include "Enums.h"
#include <QString>
class UFOInfo
{
public:
	UFOInfo(int ufo1, int ufo2, int ufo3);
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
	UFO ufo;//�ɵ�����
	int powerItemCount;
	int pointItemCount;
	int stage;
	int PIV;//���õ�
	int power;//���ڼ����Ƿ���p
};
