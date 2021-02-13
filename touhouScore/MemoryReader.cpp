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

int MemoryReader::GetScore()
{
    return ReadInt(ScoreAddr);
}

int MemoryReader::GetStage()
{
    return ReadInt(StageAddr);
}

int MemoryReader::GetFaith()
{
    return ReadInt(FaithAddr);
}

int MemoryReader::GetShotType()
{
    switch (ReadInt(ShotTypeAddr1))
    {
    case 0:
        switch (ReadInt(ShotTypeAddr2))
        {
        case 0:
            return 0;
            break;
        case 1:
            return 1;
            break;
        case 2:
            return 2;
            break;
        default:
            break;
        }
        break;
    case 1:
        switch (ReadInt(ShotTypeAddr2))
        {
        case 0:
            return 3;
            break;
        case 1:
            return 4;
            break;
        case 2:
            return 5;
            break;
        default:
            break;
        }
        break;
    default:
        logger->error("Shottype read error");
        break;
    }
}

int MemoryReader::GetDiff()
{
    return ReadInt(DifficultyAddr);
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
