#pragma once
#include <Windows.h>
class MemoryReader
{
public:
	MemoryReader(DWORD processID);
	~MemoryReader();
	virtual int GetScore() = 0;
	virtual int GetStage() = 0;
	virtual int GetSpecial1() = 0;
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
	int GetScore()override;
	int GetStage()override;
	int GetSpecial1()override;
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
	TH11Reader();
	~TH11Reader();
	int GetScore()override;
	int GetStage()override;
	int GetSpecial1()override;
	int GetShotType()override;
	int GetDiff()override;
private:

};

