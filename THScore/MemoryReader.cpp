#include "MemoryReader.h"
#include <iostream>
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
		throw std::exception("ReadProcessMemory failed");
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
	switch (ReadInt(ShotTypeAddr1))
	{
	case 0://灵梦
		switch (ReadInt(ShotTypeAddr2))
		{
		case 0:
			return 0;//梦A
			break;
		case 1:
			return 1;//梦B
			break;
		case 2:
			return 2;//梦C
			break;
		default:
			logger->error("Wrong sub shottype num {0}", ReadInt(ShotTypeAddr2));
			break;
		}
		break;
	case 1://马铃薯
		switch (ReadInt(ShotTypeAddr2))
		{
		case 0:
			return 3;//魔A
			break;
		case 1:
			return 4;//魔B
			break;
		case 2:
			return 5;//魔C
			break;
		default:
			logger->error("Wrong sub shottype num { 0 }", ReadInt(ShotTypeAddr2));
			break;
		}
		break;
	default:
		logger->error("Wrong shottype num{ 0 }", ReadInt(ShotTypeAddr1));
		break;
	}
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
	switch (ReadInt(ShotTypeAddr1))
	{
	case 0://灵梦
		switch (ReadInt(ShotTypeAddr2))
		{
		case 0:
			return 0;//梦A
			break;
		case 1:
			return 1;//梦B
			break;
		case 2:
			return 2;//梦C
			break;
		default:
			logger->error("Wrong sub shottype num {0}", ReadInt(ShotTypeAddr2));
			break;
		}
		break;
	case 1://马铃薯
		switch (ReadInt(ShotTypeAddr2))
		{
		case 0:
			return 3;//魔A
			break;
		case 1:
			return 4;//魔B
			break;
		case 2:
			return 5;//魔C
			break;
		default:
			logger->error("Wrong sub shottype num { 0 }", ReadInt(ShotTypeAddr2));
			break;
		}
		break;
	default:
		logger->error("Wrong shottype num{ 0 }", ReadInt(ShotTypeAddr1));
		break;
	}
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
