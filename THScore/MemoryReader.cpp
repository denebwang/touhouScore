#include "MemoryReader.h"
#include <exception>
#include "logger.h"
MemoryReader::MemoryReader(DWORD processID)
{
	gameProcessHandle = OpenProcess(PROCESS_VM_READ, FALSE, processID);
	logger->info("Opening process, status code {0}", GetLastError());
}

MemoryReader::~MemoryReader()
{
	CloseHandle(gameProcessHandle);
}

int MemoryReader::ReadInt(DWORD address)
{
	byte buffer[4];
	if (!ReadProcessMemory(gameProcessHandle, (LPCVOID)address, &buffer, 4, 0))
	{
		logger->error("read failed, error code {0}, while reading {1}", GetLastError(), address);
		throw std::runtime_error("ReadProcessMemory failed");
	}
	unsigned int value;
	memcpy(&value, buffer, sizeof(int));
	return value;
}

int MemoryReader::ReadIntFromPointer(DWORD ptr, DWORD offset)
{
	DWORD pointedAddr = ReadInt(ptr);
	if (pointedAddr != NULL)
		return ReadInt(pointedAddr + offset);
	else
	{
		return 0;
	}
}

TH10Reader::TH10Reader(DWORD processID) : MemoryReader(processID)
{
}

TH10Reader::~TH10Reader()
{
}

long long TH10Reader::GetScore()
{
	return long long(ReadInt(ScoreAddr)) * 10;
}

int TH10Reader::GetStage()
{
	return ReadInt(StageAddr);
}

std::vector<int> TH10Reader::GetSpecials()
{
	return { ReadInt(FaithAddr) * 10 };
}

int TH10Reader::GetShotType()
{
	int character = ReadInt(ShotTypeAddr1);
	int shot = ReadInt(ShotTypeAddr2);
	return character * 3 + shot;
}

int TH10Reader::GetDiff()
{
	return ReadInt(DifficultyAddr);
}

int TH10Reader::GetBossHP()
{
	return ReadIntFromPointer(BossHPptr, BossHPOffset);
}

int TH10Reader::GetStageFrame()
{
	return ReadInt(FrameCountAddr);
}

int TH10Reader::GetLocalFrame()
{
	return ReadInt(LocalFrameAddr);
}

int TH10Reader::GetPower()
{
	return ReadInt(PowerAddr) * 5;
}

TH11Reader::TH11Reader(DWORD processID) : MemoryReader(processID)
{
}

TH11Reader::~TH11Reader()
{
}

long long TH11Reader::GetScore()
{
	return long long(ReadInt(ScoreAddr)) * 10;
}

int TH11Reader::GetStage()
{
	return ReadInt(StageAddr);
}

int TH11Reader::GetShotType()
{
	int character = ReadInt(ShotTypeAddr1);
	int shot = ReadInt(ShotTypeAddr2);
	return character * 3 + shot;
}

int TH11Reader::GetDiff()
{
	return ReadInt(DifficultyAddr);
}

std::vector<int> TH11Reader::GetSpecials()
{
	int faith = ReadInt(FaithAddr) / 100;
	int graze = ReadInt(GrazeAddr);
	return { faith, graze };
}

int TH11Reader::GetBossHP()
{
	return ReadIntFromPointer(BossHPptr, BossHPOffset);
}

int TH11Reader::GetStageFrame()
{
	return ReadInt(FrameCountAddr);
}

int TH11Reader::GetLocalFrame()
{
	return ReadInt(LocalFrameAddr);
}

int TH11Reader::GetPower()
{
	return ReadInt(PowerAddr) * 5;
}

TH12Reader::TH12Reader(DWORD processID) :MemoryReader(processID)
{
}

TH12Reader::~TH12Reader()
{
}

long long TH12Reader::GetScore()
{
	return ((long long)(ReadInt(ScoreAddr))) * 10;
}

int TH12Reader::GetStage()
{
	return ReadInt(StageAddr);
}

std::vector<int> TH12Reader::GetSpecials()
{
	int PIV = (ReadInt(PIVAddr) / 1000) * 10;
	int graze = ReadInt(GrazeAddr);
	std::vector<int> specials;
	specials.push_back(PIV);
	specials.push_back(graze);
	return specials;
}

int TH12Reader::GetShotType()
{
	int character = ReadInt(ShotTypeAddr1);
	int shot = ReadInt(ShotTypeAddr2);
	return 2 * character + shot;
}

int TH12Reader::GetDiff()
{
	return ReadInt(DifficultyAddr);
}

int TH12Reader::GetBossHP()
{
	return ReadIntFromPointer(BossHPptr, BossHPOffset);
}

int TH12Reader::GetStageFrame()
{
	return ReadInt(FrameCountAddr);
}

int TH12Reader::GetLocalFrame()
{
	return 0;
}

int TH12Reader::GetPower()
{
	return ReadInt(PowerAddr);
}

int TH12Reader::GetUFOCount()
{
	return ReadInt(UFONumAddr);
}

int TH12Reader::GetUFO1()
{
	return ReadInt(UFO1Addr);
}

int TH12Reader::GetUFO2()
{
	return ReadInt(UFO2Addr);
}

int TH12Reader::GetUFO3()
{
	return ReadInt(UFO3Addr);
}

int TH12Reader::GetPowerCount()
{
	return ReadIntFromPointer(UFOptr, PowerItemOffset);
}

int TH12Reader::GetPointCount()
{
	return ReadIntFromPointer(UFOptr, PointItemOffset);
}