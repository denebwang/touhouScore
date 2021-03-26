#include "mainwindow.h"
#include "GameInfo.h"
#include "SectionInfo.h"
#include "MemoryReader.h"
#include "logger.h"
#include "editorwindow.h"
#include "UFOWindow.h"
#include <string>
#include <vector>
#include <exception>
#include <Windows.h>
#include <tlhelp32.h>
#include <QToolBar>
#include <QAction>
#include <QTimer>
#include <QLocale>
#include <QString>
#include <QStringList>
#include <QBrush>
#include <QColor>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QAbstractScrollArea>
#include <QMessageBox>

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

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	//工具栏
	QAction* patternEditAction = ui.toolBar->addAction(tr("Pattern Edit"));
	connect(patternEditAction, &QAction::triggered, []()
		{
			EditorWindow* editer = new EditorWindow();
			editer->setAttribute(Qt::WA_DeleteOnClose);
			editer->show();
		});
	//初始化表格
	ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.tableWidget->item(0, 0)->setTextAlignment(Qt::AlignCenter);
	ui.tableWidget->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
	//定时扫描游戏
	GameScanTimer = new QTimer(this);
	GameScanTimer->setInterval(1000);
	connect(GameScanTimer, &QTimer::timeout, this, &MainWindow::ScanGame);
	GameScanTimer->start();
	//更新游戏信息
	InfoUpdateTimer = new QTimer(this);
	InfoUpdateTimer->setInterval(16);
	connect(InfoUpdateTimer, &QTimer::timeout, this, &MainWindow::UpdateInfo);
	void (QTimer:: * timerStart)() = &QTimer::start;
	connect(this, &MainWindow::FoundGame, this, &MainWindow::ReadInfo);
	//connect(this, &MainWindow::FoundGame, this, &MainWindow::InitChart);
	connect(this, &MainWindow::FoundGame, InfoUpdateTimer, timerStart);
	connect(this, &MainWindow::NewShottype, this, &MainWindow::InitChart);
	connect(this, &MainWindow::Retry, this, &MainWindow::RestoreChart);
	connect(this, &MainWindow::Retry, this, &MainWindow::UpdateBackground);
	connect(this, &MainWindow::ReadSuccees, this, &MainWindow::ShowScore);
	connect(this, &MainWindow::NewSection, this, &MainWindow::ShowDelta);
	connect(this, &MainWindow::NewSection, this, &MainWindow::UpdateBackground);
	connect(this, &MainWindow::NewStage, this, &MainWindow::UpdateLastBonus);
	//connect(this, &MainWindow::NewShottype, this, &MainWindow::UpdatePattern);

	//指针初始置为null
	mr = nullptr;
	gameInfo = nullptr;
	currentSectionIndex = 0;
}

MainWindow::~MainWindow()
{
	if (mr != nullptr)
		delete mr;
	if (gameInfo != nullptr)
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
		try
		{
			gameInfo = GameInfo::Create(gameName, procId, mr);
			if (gameName == "th12")
			{
				UFOWindow* ufowin = new UFOWindow(mr);
				connect(InfoUpdateTimer, &QTimer::timeout, ufowin, &UFOWindow::ReadUFO);
				connect(InfoUpdateTimer, &QTimer::timeout, ufowin, &UFOWindow::ShowInfo);
				connect(this, &MainWindow::Retry, ufowin, &UFOWindow::OnRetry);
				connect(this, &MainWindow::NewShottype, ufowin, &UFOWindow::OnShottypeChanged);
				ufowin->setAttribute(Qt::WA_DeleteOnClose);
				ufowin->show();
			}
			ui.stackedWidget->setCurrentIndex(1);
			GameScanTimer->stop();
			emit FoundGame(isFound);
		}
		catch (std::runtime_error& e)
		{
			QMessageBox::warning(this, tr("Game not Supported"), QString(tr("%1 is not supported yet!")).arg(QString::fromStdString(gameName)));
		}
	}
}

void MainWindow::UpdateInfo()
{
	try
	{
		ReadInfo();
	}
	catch (std::runtime_error& e)
	{
		logger->error("Caught an exception: {0}", e.what());
		logger->info("Possibly game closed");
		delete gameInfo;
		delete mr;
		gameInfo = nullptr;
		mr = nullptr;
		GameScanTimer->start();
		InfoUpdateTimer->stop();
		ui.stackedWidget->setCurrentIndex(0);
		return;
	}
	catch (...)
	{
		auto exptr = std::current_exception();
		try {
			rethrow_exception(exptr);
		}
		catch (std::exception& e)
		{
			logger->error("Caught an exception: {0}", e.what());
			this->close();
		}
		return;
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
	ui.tableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
	this->adjustSize();
	//填充初始数据
	int rowOffset = 0;
	for (int i = 0; i < 6; i++)
	{
		//面数
		int stageSectionCount = gameInfo->GetStageSectionCount(i);
		QStringList SectionNames = gameInfo->GetSectionNames(i);
		QTableWidgetItem* newItem = new QTableWidgetItem(QString::number(i + 1));
		newItem->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(rowOffset, 0, newItem);
		ui.tableWidget->setSpan(rowOffset, 0, stageSectionCount * 3, 1);
		//ui.tableWidget->setColumnWidth(0, 20);
		//section
		for (int index = 0; index < SectionNames.size(); index++)
		{
			QTableWidgetItem* newItem = new QTableWidgetItem(SectionNames.at(index));
			newItem->setTextAlignment(Qt::AlignCenter);
			ui.tableWidget->setItem(rowOffset + index * 3, 1, newItem);
			ui.tableWidget->setSpan(rowOffset + index * 3, 1, 3, 1);
		}
		//类别
		for (int row = 0; row < rowCount; row += 3)
		{
			ui.tableWidget->setItem(row, 2, new QTableWidgetItem(tr("Game")));
			ui.tableWidget->setItem(row + 1, 2, new QTableWidgetItem(tr("Pattern")));
			ui.tableWidget->setItem(row + 2, 2, new QTableWidgetItem(tr("Delta")));
		}
		rowOffset += stageSectionCount * 3;
	}
	//其他数据：仅初始化
	for (int col = 3; col < columnCount; col++)
	{
		for (int row = 0; row < rowCount; row++)
		{
			QTableWidgetItem* newItem = new QTableWidgetItem("0");
			newItem->setTextAlignment(Qt::AlignCenter);
			ui.tableWidget->setItem(row, col, newItem);
			ui.tableWidget->setRowHidden(row, true);//初始关闭所有显示
		}
	}
	ui.gameNameLabel->setText(gameInfo->GameName());
	ui.DiffLabel->setText(gameInfo->Difficulty());
	ui.ShottypeLabel->setText(gameInfo->ShotType());

	//更新路线
	UpdatePattern();
	UpdateBackground();
}

void MainWindow::ShowScore()
{
	static QLocale loc = QLocale::English;
	int currentStage = gameInfo->GetCurrentStage();
	int row = gameInfo->GetCurrenSectionRowIndex();
	SectionInfo current = gameInfo->GetCurrentSectionInfo(currentStage - 1);
	//游戏内信息显示
	//分数 col=3
	ui.tableWidget->item(row, 3)->setData(Qt::DisplayRole, loc.toString(current.GetScore(0)));
	//其他 col=3+index
	std::vector<int> specials = current.GetSpecials(0);

	for (int i = 0; i < specials.size(); i++)
	{
		ui.tableWidget->item(row, 3 + 1 + i)->setData(Qt::DisplayRole, loc.toString(specials[i]));
	}
	//将当前section设为可见
	ui.tableWidget->setRowHidden(row, false);
}

void MainWindow::ShowDelta()
{
	static QLocale loc = QLocale::English;
	int currentStage = gameInfo->GetCurrentStage();
	int row = gameInfo->GetCurrenSectionRowIndex();
	if (row == 0)
	{
		return;
	}
	SectionInfo current = gameInfo->GetPrevSectionInfo(currentStage - 1);
	if (gameInfo->GetCurrentSectionIndex(currentStage - 1) == 0)//该面的第一个section，所以要显示前一面的最后一个section
	{
		current = gameInfo->GetCurrentSectionInfo(currentStage - 2);
	}

	//rowBias = 2;
	//分数 col=3
	QTableWidgetItem* scoreItem = ui.tableWidget->item(row - 1, 3);
	long long score = current.GetScore(2);
	scoreItem->setData(Qt::DisplayRole, loc.toString(score));
	SetDeltaColor(score, scoreItem);

	//其他 col=3+index
	std::vector<int> specials = current.GetSpecials(2);
	for (int i = 0; i < specials.size(); i++)
	{
		QTableWidgetItem* specialItem = ui.tableWidget->item(row - 1, 3 + 1 + i);
		int special = specials[i];
		specialItem->setData(Qt::DisplayRole, loc.toString(special));
		SetDeltaColor(special, specialItem);
	}
	ui.tableWidget->setRowHidden(row - 1, false);
	//隐藏pattern
	ui.tableWidget->setRowHidden(row - 2, true);
}

void MainWindow::UpdatePattern()
{	//路线信息显示
	static QLocale loc = QLocale::English;

	int rowBias = 1;
	int sectionCount = 0;

	for (int i = 0; i < 6; ++i)
	{
		const std::vector<SectionInfo> sections = gameInfo->GetSectionInfos(i);
		for (auto& section : sections)
		{
			//分数 col=3
			ui.tableWidget->item(sectionCount * 3 + rowBias, 3)->setData(Qt::DisplayRole, loc.toString(section.GetScore(1)));
			//其他 col=3+index
			const std::vector<int> specials = section.GetSpecials(1);
			for (int i = 0; i < specials.size(); i++)
			{
				ui.tableWidget->item(sectionCount * 3 + rowBias, 3 + 1 + i)->setData(Qt::DisplayRole, loc.toString(specials[i]));
			}
			ui.tableWidget->setRowHidden(sectionCount * 3 + rowBias, false);//初始显示所有路线
			sectionCount++;
		}
	}
}

void MainWindow::ReadInfo()
{
	int diff = mr->GetDiff();
	int shotType = mr->GetShotType();
	int stage = mr->GetStage();
	long long score = mr->GetScore();
	int bossHP = mr->GetBossHP();
	int frameCount = mr->GetStageFrame();
	int localFrame = mr->GetLocalFrame();
	std::vector<int> specials = mr->GetSpecials();
	if (gameInfo->CheckRetry(stage, frameCount))
	{
		emit Retry();
	}
	try
	{
		if (gameInfo->SetInfo(diff, shotType))
			emit NewShottype(diff, shotType);
	}
	catch (std::out_of_range& e)
	{
		emit NewShottype(diff, shotType);
	}/*
	catch (std::runtime_error& e)
	{
		QMessageBox::warning(this, tr("Pattern invalid"), QString(tr("Pattern for %1 %2 %3 is not a valid pattern file"))
			.arg(gameInfo->GameName())
			.arg(gameInfo->Difficulty())
			.arg(gameInfo->ShotType()));
	}*/
	if (gameInfo->SetData(stage, score, specials))
	{
		emit NewStage(stage);
		emit NewSection();
	}
	gameInfo->UpdateDelta(stage);
	if (gameInfo->TestSection(bossHP, NULL, frameCount, localFrame))
	{
		emit NewSection();
	}
}

void MainWindow::RestoreChart()
{
	static QBrush normal;
	int rowCount = ui.tableWidget->rowCount();
	int colCount = ui.tableWidget->columnCount();
	for (int row = 0; row < rowCount; row += 3)
	{
		ui.tableWidget->setRowHidden(row, true);
		ui.tableWidget->setRowHidden(row + 1, false);
		ui.tableWidget->setRowHidden(row + 2, true);
	}
}

void MainWindow::UpdateBackground()
{
	static QBrush highlighted(QColor(204, 229, 255));
	static QBrush normal;

	int currentRow = gameInfo->GetCurrenSectionRowIndex();
	int rowCount = ui.tableWidget->rowCount();
	int columnCount = ui.tableWidget->columnCount();
	for (int row = 0; row < rowCount; row += 3)
	{
		for (int col = 2; col < columnCount; col++)
		{
			if (ui.tableWidget->item(row, col) != nullptr)
			{
				if (row == currentRow)
				{

					ui.tableWidget->item(row, col)->setBackground(highlighted);
				}
				else
				{
					ui.tableWidget->item(row, col)->setBackground(normal);
				}
			}

		}

	}
}

void MainWindow::UpdateLastBonus()
{
	if (gameInfo->GetCurrentStage() < 2)
	{
		return;
	}
	//与showscore代码相同
	static QLocale loc = QLocale::English;
	int stage = gameInfo->GetCurrentStage() - 1;
	int row = gameInfo->GetCurrenSectionRowIndex() - 3;
	SectionInfo current = gameInfo->GetCurrentSectionInfo(stage - 1);
	//游戏内信息显示
	//分数 col=3
	ui.tableWidget->item(row, 3)->setData(Qt::DisplayRole, loc.toString(current.GetScore(0)));
	//其他 col=3+index
	std::vector<int> specials = current.GetSpecials(0);
	for (int i = 0; i < specials.size(); i++)
	{
		ui.tableWidget->item(row, 3 + 1 + i)->setData(Qt::DisplayRole, loc.toString(specials[i]));
	}
}

void MainWindow::SetDeltaColor(long long score, QTableWidgetItem* item)
{
	static QBrush red = QBrush(Qt::red);
	static QBrush green = QBrush(Qt::green);
	static QBrush defaultBrush = QBrush();
	if (score < 0)
	{
		item->setForeground(red);
	}
	else if (score > 0)
	{
		item->setForeground(green);
	}
	else
	{
		item->setForeground(defaultBrush);
	}
}