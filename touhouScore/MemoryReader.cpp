#include "MemoryReader.h"
#include <iostream>
#include "logger.h"
MemoryReader::MemoryReader(DWORD processID)
{
    gameProcessHandle = OpenProcess(PROCESS_VM_READ, FALSE, processID);
    logger->info("Opening process, status code {0}",GetLastError());
}

MemoryReader::~MemoryReader()
{
    CloseHandle(gameProcessHandle);
}


int MemoryReader::ReadInt(DWORD address)
{
    byte buffer[4];
    if (!ReadProcessMemory(gameProcessHandle, (LPCVOID)address, &buffer, 4, 0))
        logger->error("read failed, error code {0}", GetLastError());
    unsigned int value;
    memcpy(&value, buffer, sizeof(int));
    return value;
}

TH10Reader::TH10Reader(DWORD processID) : MemoryReader(processID)
{
}

TH10Reader::~TH10Reader()
{
}

int TH10Reader::GetScore()
{
    return ReadInt(ScoreAddr)*10;
}

int TH10Reader::GetStage()
{
    return ReadInt(StageAddr);
}

std::vector<int> TH10Reader::GetSpecials()
{
    return { ReadInt(FaithAddr)*10 };
}

int TH10Reader::GetShotType()
{
    switch (ReadInt(ShotTypeAddr1))
    {
    case 0://ÁéÃÎ
        switch (ReadInt(ShotTypeAddr2))
        {
        case 0:
            return 0;//ÃÎA
            break;
        case 1:
            return 1;//ÃÎB
            break;
        case 2:
            return 2;//ÃÎC
            break;
        default:
            logger->error("Wrong sub shottype num {0}", ReadInt(ShotTypeAddr2));
            break;
        }
        break;
    case 1://ÂíÁåÊí
        switch (ReadInt(ShotTypeAddr2))
        {
        case 0:
            return 3;//Ä§A
            break;
        case 1:
            return 4;//Ä§B
            break;
        case 2:
            return 5;//Ä§C
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

TH11Reader::TH11Reader(DWORD processID) : MemoryReader(processID)
{
}

TH11Reader::~TH11Reader()
{
}

int TH11Reader::GetScore()
{
    return ReadInt(ScoreAddr)*10;
}

int TH11Reader::GetStage()
{
    return ReadInt(StageAddr);
}

int TH11Reader::GetShotType()
{
    switch (ReadInt(ShotTypeAddr1))
    {
    case 0://ÁéÃÎ
        switch (ReadInt(ShotTypeAddr2))
        {
        case 0:
            return 0;//ÃÎA
            break;
        case 1:
            return 1;//ÃÎB
            break;
        case 2:
            return 2;//ÃÎC
            break;
        default:
            logger->error("Wrong sub shottype num {0}", ReadInt(ShotTypeAddr2));
            break;
        }
        break;
    case 1://ÂíÁåÊí
        switch (ReadInt(ShotTypeAddr2))
        {
        case 0:
            return 3;//Ä§A
            break;
        case 1:
            return 4;//Ä§B
            break;
        case 2:
            return 5;//Ä§C
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
