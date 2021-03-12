﻿#pragma once
#include <Windows.h>
#include <vector>
class MemoryReader
{
public:
	MemoryReader(DWORD processID);
	~MemoryReader();
	virtual long long GetScore() = 0;
	virtual int GetStage() = 0;
	virtual std::vector<int> GetSpecials() = 0;
	virtual int GetShotType() = 0;
	virtual int GetDiff() = 0;
	virtual int GetBossHP() = 0;
	virtual int GetStageFrame() = 0;
	virtual int GetLocalFrame() = 0;
	int ReadInt(DWORD address);
	int ReadIntFromPointer(DWORD ptr, DWORD offset);
private:
	HANDLE gameProcessHandle;

};

class TH10Reader :public MemoryReader
{
public:
	TH10Reader(DWORD processID);
	~TH10Reader();
	long long GetScore()override;
	int GetStage()override;
	std::vector<int> GetSpecials()override;
	int GetShotType()override;
	int GetDiff()override;
	int GetBossHP() override;
	int GetStageFrame() override;
	int GetLocalFrame() override;

private:
	const static DWORD ScoreAddr = 0x00474C44;
	const static DWORD StageAddr = 0x00474C7C;
	const static DWORD FaithAddr = 0x00474C4C;
	const static DWORD DifficultyAddr = 0x00474C74;
	const static DWORD ShotTypeAddr1 = 0x00474C68;
	const static DWORD ShotTypeAddr2 = 0x00474C6C;
	const static DWORD FrameCountAddr = 0x00474C88;
	const static DWORD LocalFrameAddr = 0x00474C8C;
	const static DWORD BossHPptr = 0x0047770C;
	const static DWORD BossHPOffset = 0x9E8C;
};

class TH11Reader : public MemoryReader
{
public:
	TH11Reader(DWORD processID);
	~TH11Reader();
	long long GetScore()override;
	int GetStage()override;
	int GetShotType()override;
	int GetDiff()override;
	std::vector<int> GetSpecials()override;
	int GetBossHP() override;
	int GetStageFrame() override;
	int GetLocalFrame() override;

private:
	const static DWORD ScoreAddr = 0x004A56E4;
	const static DWORD StageAddr = 0x004A5728;
	const static DWORD DifficultyAddr = 0x004A5720;
	const static DWORD ShotTypeAddr1 = 0x004A5710;
	const static DWORD ShotTypeAddr2 = 0x004C3260;
	const static DWORD FaithAddr = 0x004A56F0;
	const static DWORD GrazeAddr = 0x004A5754;
	const static DWORD FrameCountAddr = 0x004A5734;
	const static DWORD LocalFrameAddr = 0x004A5738;
	const static DWORD BossHPptr = 0x004A8D84;
	const static DWORD BossHPOffset = 0x43F8;

};

class TH12Reader:public MemoryReader
{
public:
	TH12Reader(DWORD processID);
	~TH12Reader();

	long long GetScore() override;
	int GetStage() override;
	std::vector<int> GetSpecials() override;
	int GetShotType() override;
	int GetDiff() override;
	int GetBossHP() override;
	int GetStageFrame() override;
	int GetLocalFrame() override;

private:
	const static DWORD ScoreAddr = 0x004B0C44;
	const static DWORD StageAddr = 0x004B0CB0;
	const static DWORD DifficultyAddr = 0x004AEBD0;
	const static DWORD ShotTypeAddr1 = 0x004B0C90;
	const static DWORD ShotTypeAddr2 = 0x004B0C94;
	const static DWORD PIVAddr = 0x004B0C78;
	const static DWORD GrazeAddr = 0x004B0CDC;
	const static DWORD FrameCountAddr = 0x004B0CBC;
	//const static DWORD LocalFrameAddr = 0x004A5738;
	const static DWORD BossHPptr = 0x004B43E4;
	const static DWORD BossHPOffset = 0x6CF0;
	const static DWORD UFOptr = 0x004B4534;
	const static DWORD PowerItemOffset = 0x48;
	const static DWORD PointItemOffset = 0x4C;
	const static DWORD UFO1Addr = 0x004B0C4C;
	const static DWORD UFO2Addr = 0x004B0C50;
	const static DWORD UFO3Addr = 0x004B0C54;
	const static DWORD UFONumAddr = 0x004B0C58;

};