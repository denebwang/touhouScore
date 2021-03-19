#include "ufoeditwin.h"
#include "UFOWindow.h"
#include "logger.h"
#include "csvIO.h"
#include "Enums.h"
#include "ComboCell.h"
#include "GameInfo.h"
#include <filesystem>
#include <exception>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QString>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QPushButton>

UFOeditWin::UFOeditWin(int diff, int shot, QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	setPattern(diff, shot);
	tableRightClickMenu = new QMenu(ui.tableWidget);
	insertRowAction = new QAction(tr("insert"), ui.tableWidget);
	removeRowAction = new QAction(tr("remove"), ui.tableWidget);
	connect(insertRowAction, &QAction::triggered, this, &UFOeditWin::insertRow);
	connect(removeRowAction, &QAction::triggered, this, &UFOeditWin::removeRow);
	tableRightClickMenu->addAction(insertRowAction);
	tableRightClickMenu->addAction(removeRowAction);
	connect(ui.tableWidget, &QTableWidget::cellChanged, this, &UFOeditWin::setUFOdata);
	connect(ui.tableWidget, &QTableWidget::customContextMenuRequested, this, &UFOeditWin::onCustomContextMenuRequested);
	QStringList str;
	str << tr("Red") << tr("Blue") << tr("Green") << tr("Multicolored");
	ComboCell* cc = new ComboCell(str, ui.tableWidget);
	ui.tableWidget->setItemDelegateForColumn(1, cc);
	connect(ui.saveButton, &QPushButton::pressed, this, &UFOeditWin::save);
}

UFOeditWin::~UFOeditWin()
{
}

void UFOeditWin::setPattern(int diff, int shot)
{
	patterns.clear();
	std::filesystem::path filepath;
	try
	{
		filepath = UFOWindow::GetPatternFilePath(diff, shot);
	}
	catch (std::out_of_range& e)
	{
		logger->warn("UFO pattern file for {0} {1} is not found", diff, shot);
		return;
	}
	this->diff = diff;
	this->shot = shot;
	CSVReader reader(filepath);
	int ufoCount = 0;
	//跳过表头
	while (!reader.AtEnd())
	{
		while (reader.ReadRow()[0] != QString::number(ufoCount + 1))
		{
			continue;
		}
		auto data = reader.ReadIntRow();
		int stage = data[0];
		int ufoType = data[1];
		int pointItemCount = data[2];
		UFOInfo newUfo(static_cast<UFO>(ufoType));
		newUfo.SetStage(stage);
		newUfo.SetPointItemCount(pointItemCount);
		patterns.push_back(newUfo);
		ufoCount++;
	}
}

void UFOeditWin::setUFOdata(int row, int column)
{
	UFOInfo& ufo = patterns.at(row);
	switch (column)
	{
	case 0://stage
		ufo.SetStage(ui.tableWidget->item(row, column)->text().toInt());
		break;
	case 1://ufo类型
		ufo.SetUFOType(ui.tableWidget->item(row, column)->text());
		break;
	case 2://蓝点数
		ufo.SetPointItemCount(ui.tableWidget->item(row, column)->text().toInt());
		break;
	default:
		logger->error("UFO Editor: Wrong column index: {0}", column);
		break;
	}
}

void UFOeditWin::showChart()
{
	ui.tableWidget->setRowCount(patterns.size());
	//每行数据
	int row = 0;
	QTableWidgetItem* newItem;
	for (auto& ufo : patterns)
	{
		newItem = new QTableWidgetItem(QString::number(ufo.GetStage()));
		ui.tableWidget->setItem(row, 0, newItem);
		newItem = new QTableWidgetItem();
		newItem->setText(ufo.GetUFOTypeName());
		ui.tableWidget->setItem(row, 1, newItem);
		newItem = new QTableWidgetItem(QString::number(ufo.GetPointItemCount()));
		ui.tableWidget->setItem(row, 2, newItem);
		row++;
	}
}

void UFOeditWin::insertRow()
{
	if (menuRow<0)
	{
		menuRow = ui.tableWidget->rowCount() - 1;
	}
	patterns.insert(patterns.begin() + menuRow + 1, UFOInfo());
	ui.tableWidget->insertRow(menuRow + 1);
}

void UFOeditWin::removeRow()
{
	if (menuRow < 0)
	{
		menuRow = ui.tableWidget->rowCount()-1;
	}
	patterns.erase(patterns.begin() + menuRow);
	ui.tableWidget->removeRow(menuRow);
}

void UFOeditWin::onCustomContextMenuRequested(const QPoint& pos)
{
	auto index = ui.tableWidget->indexAt(pos);
	menuRow = index.row();
	tableRightClickMenu->exec(QCursor::pos());
}

void UFOeditWin::save()
{
	CSVWriter* writer = nullptr;
	try
	{
		QStringList patternName;
		patternName << GameInfo::DiffList[diff];
		patternName << GameInfo::GetShotTypeList(12)[shot];
		auto filename = patternName.join("-").append(".csv");
		writer = new CSVWriter(std::filesystem::path(L"csv/ufo"), filename);
	}
	catch (std::runtime_error& e)
	{
		logger->error(e.what());
		QMessageBox::warning(this, tr("Save CSV Failed"), tr("Please close any program that is used the file."));
		return;
	}
	QStringList line;
	line << QString::number(diff) << QString::number(shot);
	writer->WriteLine(line);
	line.clear();
	int ufoCount = 0;
	for (auto& ufo : patterns)
	{
		writer->WriteLine(QString::number(ufoCount + 1));
		line << QString::number(ufo.GetStage()) << QString::number(static_cast<int>(ufo.GetUFOType())) << QString::number(ufo.GetPointItemCount());
		writer->WriteLine(line);
		line.clear();
		ufoCount++;
	}
	delete writer;
	QMessageBox::information(this, tr("Save Success!"), tr("Pattern file saved"));
}
