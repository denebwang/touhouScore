#pragma once
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
	virtual int GetPower() = 0;
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
	int GetPower() override;
private:
	const DWORD ScoreAddr = 0x00474C44;
	const DWORD StageAddr = 0x00474C7C;
	const DWORD FaithAddr = 0x00474C4C;
	const DWORD DifficultyAddr = 0x00474C74;
	const DWORD ShotTypeAddr1 = 0x00474C68;
	const DWORD ShotTypeAddr2 = 0x00474C6C;
	const DWORD FrameCountAddr = 0x00474C88;
	const DWORD LocalFrameAddr = 0x00474C8C;
	const DWORD BossHPptr = 0x0047770C;
	const DWORD BossHPOffset = 0x9E8C;
	const DWORD PowerAddr = 0x00474C48;
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
	int GetPower() override;
private:
	const DWORD ScoreAddr = 0x004A56E4;
	const DWORD StageAddr = 0x004A5728;
	const DWORD DifficultyAddr = 0x004A5720;
	const DWORD ShotTypeAddr1 = 0x004A5710;
	const DWORD ShotTypeAddr2 = 0x004C3260;
	const DWORD FaithAddr = 0x004A56F0;
	const DWORD GrazeAddr = 0x004A5754;
	const DWORD FrameCountAddr = 0x004A5734;
	const DWORD LocalFrameAddr = 0x004A5738;
	const DWORD BossHPptr = 0x004A8D84;
	const DWORD BossHPOffset = 0x43F8;
	const DWORD PowerAddr = 0x004A56E8;
};

class TH12Reader :public MemoryReader
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
	int GetLocalFrame() override;//not implemented
	int GetPower() override;
	//ufo
	int GetUFOCount();
	int GetUFO1();
	int GetUFO2();
	int GetUFO3();
	int GetPowerCount();
	int GetPointCount();

private:
	const DWORD ScoreAddr = 0x004B0C44;
	const DWORD StageAddr = 0x004B0CB0;
	const DWORD DifficultyAddr = 0x004AEBD0;
	const DWORD ShotTypeAddr1 = 0x004B0C90;
	const DWORD ShotTypeAddr2 = 0x004B0C94;
	const DWORD PIVAddr = 0x004B0C78;
	const DWORD GrazeAddr = 0x004B0CDC;
	const DWORD FrameCountAddr = 0x004B0CBC;
	//const DWORD LocalFrameAddr = 0x004A5738;
	const DWORD BossHPptr = 0x004B43E4;
	const DWORD BossHPOffset = 0x6CF0;
	const DWORD PowerAddr = 0x004B0C48;
	const DWORD UFOptr = 0x004B4534;
	const DWORD PowerItemOffset = 0x48;
	const DWORD PointItemOffset = 0x4C;
	const DWORD UFO1Addr = 0x004B0C4C;
	const DWORD UFO2Addr = 0x004B0C50;
	const DWORD UFO3Addr = 0x004B0C54;
	const DWORD UFONumAddr = 0x004B0C58;
};