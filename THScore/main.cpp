#include "mainwindow.h"
#include <windows.h>
#include "GameInfo.h"
#include "logger.h"
#include <QtWidgets/QApplication>

BOOL SetPrivilage();

int main(int argc, char* argv[])
{
	logger->flush_on(spdlog::level::info);
	GameInfo::Init();
	GameInfo::ScanCSV();
	SetPrivilage();

	QApplication a(argc, argv);
	MainWindow w;
	w.show();
	return a.exec();
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
	if (!LookupPrivilegeValue(NULL,                 //查看的系统，本地为
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

	logger->info("SetPrivilage successful");
	return TRUE;
}