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
			throw std::logic_error("Wrong sub shot type num");
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
			throw std::logic_error("Wrong sub shot type num");
			break;
		}
		break;
	default:
		logger->error("Wrong shottype num{ 0 }", ReadInt(ShotTypeAddr1));
		throw std::logic_error("Wrong shot type num");
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

int TH10Reader::GetLocalFrame()
{
	return ReadInt(LocalFrameAddr);
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
			throw std::logic_error("Wrong sub shot type num");
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
			throw std::logic_error("Wrong sub shot type num");
			break;
		}
		break;
	default:
		logger->error("Wrong shottype num{ 0 }", ReadInt(ShotTypeAddr1));
		throw std::logic_error("Wrong shottype num");
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
	std::vector<int> specials;
	specials.push_back(ReadInt(PIVAddr));
	specials.push_back(ReadInt(GrazeAddr));
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
