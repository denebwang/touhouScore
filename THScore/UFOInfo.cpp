#include "UFOInfo.h"
#include "Enums.h"
#include "logger.h"
#include <QCoreApplication>
UFOInfo::UFOInfo(int ufo1, int ufo2, int ufo3)
{
	SetUFOType(ufo1, ufo2, ufo3);
	powerItemCount = 0;
	pointItemCount = 0;
	stage = 0;
	PIV = 0;
	power = 0;
}

UFOInfo::UFOInfo(UFO ufo)
{
	this->ufo = ufo;
	powerItemCount = 0;
	pointItemCount = 0;
	stage = 0;
	PIV = 0;
	power = 0;
}

UFOInfo::~UFOInfo()
{
}

UFO UFOInfo::GetUFOType()
{
	return ufo;
}

QString UFOInfo::GetUFOTypeName()
{
	switch (ufo)
	{
	case UFO::red:
		return QCoreApplication::translate("UFOWindow", "Red");
	case UFO::blue:
		return QCoreApplication::translate("UFOWindow", "Blue");
	case UFO::green:
		return QCoreApplication::translate("UFOWindow", "Green");
	case UFO::multicolored:
		return QCoreApplication::translate("UFOWindow", "Multi");
	default:
		logger->error("Wrong UFO type: {0}", static_cast<int>(ufo));
		return QString();
		break;
	}
}

int UFOInfo::GetPowerItemCount()
{
	return powerItemCount;
}

int UFOInfo::GetPointItemCount()
{
	return pointItemCount;
}

int UFOInfo::GetBonusScore()
{
	int bonus, score;
	switch (ufo)
	{
	case UFO::red:
		bonus = (power == 400 ? 2 : 1);
		score = (bonus * powerItemCount + pointItemCount) * PIV;
		break;
	case UFO::blue:
		bonus = 8;
		score = bonus * pointItemCount * PIV;
		break;
	case UFO::green:
		bonus = 2;
		score = bonus * pointItemCount * PIV;
		break;
	case UFO::multicolored:
		bonus = 4;
		score = bonus * pointItemCount * PIV;
		break;
	default:
		score = 0;
		logger->error("Wrong UFO type: {0}", static_cast<int>(ufo));
		break;
	}
	return score;
}

int UFOInfo::GetStage()
{
	return stage;
}

int UFOInfo::GetPIV()
{
	return PIV;
}

int UFOInfo::GetPower()
{
	return power;
}

void UFOInfo::SetUFOType(int ufo1, int ufo2, int ufo3)
{
	//0 == 无, 1 == 红, 2 == 蓝, 3 == 绿
	if (ufo1 == ufo2)//只要判断前两个碟就行了
	{
		switch (ufo1)
		{
		case 1:
			ufo = UFO::red;
			break;
		case 2:
			ufo = UFO::blue;
			break;
		case 3:
			ufo = UFO::green;
			break;
		default:
			logger->error("Wrong UFO type: {0}", ufo1);
			break;
		}
	}
	else ufo = UFO::multicolored;
}

void UFOInfo::SetUFOType(QString& type)
{
	if (type == QCoreApplication::translate("UFOWindow", "Red"))
		ufo = UFO::red;
	else if (type == QCoreApplication::translate("UFOWindow", "Blue"))
		ufo = UFO::blue;
	else  if (type == QCoreApplication::translate("UFOWindow", "Green"))
		ufo = UFO::green;
	else if (type == QCoreApplication::translate("UFOWindow", "Multicolored"))
		ufo = UFO::multicolored;
	else logger->error("UFO type string error, string: {0}", type.toStdString());
}

void UFOInfo::SetPowerItemCount(int value)
{
	powerItemCount = value;
}

void UFOInfo::SetPointItemCount(int value)
{
	pointItemCount = value;
}

void UFOInfo::SetStage(int stage)
{
	this->stage = stage;
}

void UFOInfo::SetPIV(int value)
{
	PIV = value;
}

void UFOInfo::SetPower(int value)
{
	power = value;
}