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
	int ReadInt(DWORD address);
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

private:
	const static DWORD ScoreAddr = 0x00474C44;
	const static DWORD StageAddr = 0x00474C7C;
	const static DWORD FaithAddr = 0x00474C4C;
	const static DWORD DifficultyAddr = 0x00474C74;
	const static DWORD ShotTypeAddr1 = 0x00474C68;
	const static DWORD ShotTypeAddr2 = 0x00474C6C;
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
private:
	const static DWORD ScoreAddr = 0x004A56E4;
	const static DWORD StageAddr = 0x004A572C;
	const static DWORD DifficultyAddr = 0x004A5720;
	const static DWORD ShotTypeAddr1 = 0x004A5710;
	const static DWORD ShotTypeAddr2 = 0x004C3260;
	const static DWORD FaithAddr = 0x004A56F0;
	const static DWORD GrazeAddr = 0x004A5754;

};

