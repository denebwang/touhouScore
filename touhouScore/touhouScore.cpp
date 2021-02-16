#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <filesystem>
#include "MemoryReader.h"
#include "GameInfo.h"
#include"logger.h"

using namespace std;
namespace fs = filesystem;  
bool GetProcessIDByName(const char* processName, DWORD& processId);
BOOL SetPrivilage();
void ClearScreen(HANDLE HOutput); 

class comma_numpunct : public std::numpunct<char>
{
protected:
    virtual char do_thousands_sep() const
    {
        return ',';
    }

    virtual std::string do_grouping() const
    {
        return "\03";
    }
};

int main(void)
{
    logger->info("Started!");
    SetConsoleTitle("THScore");
    //创建输出缓冲
    HANDLE HOutput = GetStdHandle(STD_OUTPUT_HANDLE);

    HANDLE HOutbuffer = CreateConsoleScreenBuffer(
        GENERIC_WRITE | GENERIC_READ,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL, CONSOLE_TEXTMODE_BUFFER, NULL
    );
    SetConsoleActiveScreenBuffer(HOutbuffer);

    CONSOLE_CURSOR_INFO CursorInfo;
    GetConsoleCursorInfo(HOutput, &CursorInfo);
    CursorInfo.bVisible = false;
    SetConsoleCursorInfo(HOutput, &CursorInfo);
    SetConsoleCursorInfo(HOutbuffer, &CursorInfo);

    //SetConsoleTextAttribute(HOutput, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
    //调整privilage
    SetPrivilage();


    GameInfo::Init();
    GameInfo::ScanCSV();
    DWORD procId = 0;
    string gameName;
    //while (!isFound)
    //{
    //    procId = GetProcessIDByName("th10.exe", isFound);
    //    if (isFound)
    //        break;
    //    Sleep(1000);
    //}
    bool isFound = false;
    do
    {
        for (auto iter = GameInfo::exeMap.begin(); iter != GameInfo::exeMap.end(); iter++)
        {

            for (auto stringIter = iter->second.begin(); stringIter != iter->second.end(); stringIter++)
            {
                if (GetProcessIDByName(stringIter->c_str(), procId))
                {
                    gameName = iter->first;
                    isFound = true;
                    break;
                }
            }
            if (isFound)
                break;
        }
        Sleep(1000);
    } while (!isFound);

    MemoryReader* mr = nullptr;
    GameInfo gameInfo = GameInfo::Create(gameName, procId, mr);

    std::locale comma_locale(std::locale(), new comma_numpunct());
    cout.imbue(comma_locale);
    
        while (true)
        {
            try 
            {
                int diff = mr->GetDiff();
                int shotType = mr->GetShotType();
                int stage = mr->GetStage();
                unsigned int score = mr->GetScore();
                vector<int> specials = mr->GetSpecials();
                gameInfo.SetInfo(diff, shotType);
                gameInfo.SetData(stage, score, specials);
            }
            catch (...)
            {
                auto exptr = std::current_exception();
                try {
                    rethrow_exception(exptr);
                }
                catch (exception& e)
                {
                    logger->error("Caught an exception: {0}, quitting", e.what());
                    break;
                }
            }
            //gameInfo.UpdateDelta();
            ClearScreen(HOutput);

            DWORD bytes = 0;
            char chars[10000];
            gameInfo.DisplayInfo();
            ReadConsoleOutputCharacterA(HOutput, chars, 10000, { 0,0 }, &bytes);
            WriteConsoleOutputCharacterA(HOutbuffer, chars, 10000, { 0,0 }, &bytes);
            Sleep(50);
        }
    delete mr;
    CloseHandle(HOutput);
    CloseHandle(HOutbuffer);
    return 0;
}

bool GetProcessIDByName(const char* processName,DWORD& processID)
{
    HANDLE hProcessSnap;
    //HANDLE hProcess;
    PROCESSENTRY32 pe32;

    // Take a snapshot of all processes in the system.
    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE)
    {
        logger->error("INVALID_HANDLE_VALUE in CreateToolhelp32Snapshot()");
        return(FALSE);
    }

    // Set the size of the structure before using it.
    pe32.dwSize = sizeof(PROCESSENTRY32);

    // Retrieve information about the first process,
    // and exit if unsuccessful
    if (!Process32First(hProcessSnap, &pe32))
    {
        logger->error("Process32First Error");
        CloseHandle(hProcessSnap);          // clean the snapshot object
        return(FALSE);
    }
    bool found = false;
    // Now walk the snapshot of processes
    do
    {
        if (strcmp(processName, pe32.szExeFile) == 0)
        {
            found = true;
            break;
        }
    } while (Process32Next(hProcessSnap, &pe32));

    processID = pe32.th32ProcessID;
    logger->info("Found ProcessID {0} of {1}", processID, processName);
    CloseHandle(hProcessSnap);
    return found;
}

BOOL SetPrivilage()
{
    HANDLE token_handle;
    //打开访问令牌
    if (!OpenProcessToken(
        GetCurrentProcess(),       //要修改权限的进程句柄
        TOKEN_ALL_ACCESS,          //要对令牌进行何种操作
        &token_handle              //访问令牌
    ))
        logger->error("openProcessToken error");
    
    LUID luid;
    if (!LookupPrivilegeValue(NULL,                 //查看的系统，本地为NULL
        SE_DEBUG_NAME,        //要查看的特权名称
        &luid                 //用来接收标识符
    ))
    {
        logger->error("lookupPrivilegevalue error");
    }
    
    TOKEN_PRIVILEGES tkp;
    tkp.PrivilegeCount = 1;
    tkp.Privileges[0].Luid = luid;
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    //调整访问令牌权限
    if (!AdjustTokenPrivileges(token_handle,    //令牌句柄
        FALSE,           //是否禁用权限
        &tkp,            //新的特权的权限信息
        sizeof(tkp),     //特权信息大小
        NULL,            //用来接收特权信息当前状态的buffer
        NULL             //缓冲区大小
    ))
    {
        logger->error("adjust error");
    }
    
    logger->info("SetPrivilage sucessful");
    return TRUE;
}

void ClearScreen(HANDLE HOutput)
{

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    COORD topLeft = { 0, 0 };

    // std::cout uses a buffer to batch writes to the underlying console.
    // We need to flush that to the console because we're circumventing
    // std::cout entirely; after we clear the console, we don't want
    // stale buffered text to randomly be written out.
    std::cout.flush();

    // Figure out the current width and height of the console window
    if (!GetConsoleScreenBufferInfo(HOutput, &csbi)) {
        // TODO: Handle failure!
        abort();
    }
    DWORD length = csbi.dwSize.X * csbi.dwSize.Y;

    DWORD written;

    // Flood-fill the console with spaces to clear it
    FillConsoleOutputCharacter(HOutput, TEXT(' '), length, topLeft, &written);

    // Reset the attributes of every character to the default.
    // This clears all background colour formatting, if any.
    FillConsoleOutputAttribute(HOutput, csbi.wAttributes, length, topLeft, &written);

    // Move the cursor back to the top left for the next sequence of writes
    SetConsoleCursorPosition(HOutput, topLeft);
}

