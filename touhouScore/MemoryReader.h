#pragma once
#include <Windows.h>
class MemoryReader
{
public:
	MemoryReader(DWORD processID);
	~MemoryReader();
	int GetScore();
	int GetStage();
	int GetFaith();
	int GetShotType();
	int GetDiff();
	int ReadInt(DWORD address);
private:
	HANDLE gameProcessHandle;
	const static DWORD ScoreAddr = 0x00474C44;
	const static DWORD StageAddr = 0x00474C7C;
	const static DWORD FaithAddr = 0x00474C4C;
	const static DWORD DifficultyAddr = 0x00474C74;
	const static DWORD ShotTypeAddr1 = 0x00474C68;
	const static DWORD ShotTypeAddr2 = 0x00474C6C;



};

