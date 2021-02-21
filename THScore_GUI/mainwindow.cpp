#include "mainwindow.h"
#include "GameInfo.h"
#include "MemoryReader.h"
#include "logger.h"
#include "Windows.h"
#include <tlhelp32.h>
#include <QTimer>
#include <QLocale>
#include <QString>
#include <QTableWidgetItem>

bool GetProcessIDByName(const WCHAR* processName, DWORD& processId) 
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
		if (wcscmp(processName, pe32.szExeFile) == 0)
		{
			found = true;
			break;
		}
	} while (Process32Next(hProcessSnap, &pe32));

	processId = pe32.th32ProcessID;
	//logger->info("Found ProcessID {0} of {1}", processID, processName);
	CloseHandle(hProcessSnap);
	return found;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    //初始化表格
    //ui.gameNameLabel->setText("ddcsb");
    ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui.tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.tableWidget->item(0,0)->setTextAlignment(Qt::AlignCenter);
	ui.tableWidget->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
	//ui.tableWidget->set(false);
	//QTableWidgetItem* item = new QTableWidgetItem;
	//item->setFlags(item->flags() & ~Qt::ItemIsEditable & ~Qt::ItemIsSelectable & ~Qt::ItemIsEnabled);//设置不可编辑、选中；
	//ui.tableWidget->setItemPrototype(item);
	//定时扫描游戏
    GameScanTimer = new QTimer(this);
    GameScanTimer->setInterval(1000);
	connect(GameScanTimer, &QTimer::timeout,this, &MainWindow::ScanGame);
	connect(this, &MainWindow::FoundGame, this, &MainWindow::InitChart);
	void (QTimer:: *timerStart)() = &QTimer::start;
	
	GameScanTimer->start();
	//更新游戏信息
	InfoUpdateTimer = new QTimer(this);
	InfoUpdateTimer->setInterval(100);
	connect(InfoUpdateTimer, &QTimer::timeout, this, &MainWindow::UpdateInfo);
	connect(this, &MainWindow::FoundGame, InfoUpdateTimer, timerStart);
	connect(this, &MainWindow::ReadSuccees, this, &MainWindow::ShowInfo);
	connect(this, &MainWindow::NewShottype, this, &MainWindow::UpdatePattern);
	//指针初始置为null
	mr = nullptr;
	gameInfo = nullptr;
}

MainWindow::~MainWindow()
{
	if (!(mr == nullptr))
		delete mr;
	if (!(gameInfo == nullptr))
	delete gameInfo;
}

void MainWindow::ScanGame()
{
	DWORD procId = 0;
	std::string gameName;
	bool isFound = false;
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
	if (isFound)
	{
		logger->info("Found {0}, ID {1}", gameName, procId);
		gameInfo = GameInfo::Create(gameName, procId, mr);
		GameScanTimer->stop();
		emit FoundGame(isFound);
	}
}

void MainWindow::UpdateInfo()
{
	try
	{
		int diff = mr->GetDiff();
		int shotType = mr->GetShotType();
		int stage = mr->GetStage();
		long long score = mr->GetScore();
		std::vector<int> specials = mr->GetSpecials();
		if (gameInfo->SetInfo(diff, shotType))
			emit NewShottype();
		gameInfo->SetData(stage, score, specials);
		gameInfo->UpdateDelta(stage);
	}
	catch (std::out_of_range& e)
	{
		logger->error("Caught an exception: {0}, quitting", e.what());
		logger->info("Possibly game closed");
		this->close();
	}
	catch (...)
	{
		auto exptr = std::current_exception();
		try {
			rethrow_exception(exptr);
		}
		catch (std::exception& e)
		{
			logger->error("Caught an exception: {0}, quitting", e.what());
			this->close();
		}
	}
	emit ReadSuccees();
}

void MainWindow::InitChart()
{
	//更新表格
	int columnCount = gameInfo->ColumnCount();
	int rowCount = gameInfo->RowCount();
	ui.tableWidget->setColumnCount(columnCount);
	ui.tableWidget->setRowCount(rowCount);
	ui.tableWidget->setHorizontalHeaderLabels(gameInfo->GetColumnHeader());
	//ui.tableWidget->item(0, 0)->setData(Qt::DisplayRole,"");//清空文字
	this->adjustSize();
	//填充初始数据
	
	//面数
	for (int i = 0; i < 6; i++)
	{
		QTableWidgetItem* newItem = new QTableWidgetItem(QString::number(i + 1));
		newItem->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(i * 3, 0, newItem);
		ui.tableWidget->setSpan(i * 3, 0, 3, 1);
	}
	//其他数据：仅初始化
	for (int col = 1; col < columnCount; col++)
	{
		for (int row = 0; row < rowCount; row++)
		{
			QTableWidgetItem* newItem = new QTableWidgetItem("0");
			newItem->setTextAlignment(Qt::AlignCenter);
			ui.tableWidget->setItem(row, col, newItem);
		}
	}
	//类别
	for (int row = 0; row < rowCount; row += 3)
	{
		ui.tableWidget->item(row, 1)->setData(Qt::DisplayRole, "Game");
		ui.tableWidget->item(row+1, 1)->setData(Qt::DisplayRole, "Pattern");
		ui.tableWidget->item(row+2, 1)->setData(Qt::DisplayRole, "Delta");
	}





	

	//更新游戏信息
	UpdateInfo();
	ui.gameNameLabel->setText(gameInfo->GameName());
	ui.DiffLabel->setText(gameInfo->Difficulty());
	ui.ShottypeLabel->setText(gameInfo->ShotType());

	//InfoUpdateTimer->start();
	
}

void MainWindow::ShowInfo()
{
	static QLocale loc = QLocale::English;
	//int index = (gameInfo->CurrentStage() - 1);
	int index;
	int rowIndex;
	for (int stage = 1; stage < 7; stage++)
	{
		index = stage - 1;
		rowIndex = index * 3;
	 
		//游戏内信息显示
		int rowBias = 0;
		//分数 col=2
		ui.tableWidget->item(rowIndex + rowBias, 2)->setData(Qt::DisplayRole, loc.toString(gameInfo->GetStage(index).score));
		//其他 col=2+index
		std::vector<int> specials = gameInfo->GetStage(index).specials;
		for (int i = 0; i < specials.size(); i++)
		{
			ui.tableWidget->item(rowIndex + rowBias, 2 + 1 + i)->setData(Qt::DisplayRole, loc.toString(specials[i]));
		}
		//差值显示
		rowBias = 2;
		//分数 col=2
		ui.tableWidget->item(rowIndex + rowBias, 2)->setData(Qt::DisplayRole, loc.toString(gameInfo->GetDelta(index).score));
		//其他 col=2+index
		specials = gameInfo->GetDelta(index).specials;
		for (int i = 0; i < specials.size(); i++)
		{
			ui.tableWidget->item(rowIndex + rowBias, 2 + 1 + i)->setData(Qt::DisplayRole, loc.toString(specials[i]));
		}
	}



}

void MainWindow::UpdatePattern()
{//路线信息显示
	static QLocale loc = QLocale::English;
	int rowBias = 1;
	for (int row = 0; row < ui.tableWidget->rowCount(); row += 3)
	{
		//分数 col=2
		ui.tableWidget->item(row + rowBias, 2)->setData(Qt::DisplayRole, loc.toString(gameInfo->GetPattern(row / 3).score));
		//其他 col=2+index
		std::vector<int> specials = gameInfo->GetPattern(row / 3).specials;
		for (int i = 0; i < specials.size(); i++)
		{
			ui.tableWidget->item(row + rowBias, 2 + 1 + i)->setData(Qt::DisplayRole, loc.toString(specials[i]));
		}
	}
}
