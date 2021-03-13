#include "UFOWindow.h"
#include "MemoryReader.h"
#include "logger.h"
#include <exception>
#include <QTimer>
#include <QHeaderView>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QString>
UFOWindow::UFOWindow(MemoryReader* mr, QWidget* parent)
	: QWidget(parent)
{
	if (mr == nullptr)
	{
		logger->error("TH12 memory reader is nullptr when initializing UFO window");
		throw std::runtime_error("null pointer");
	}
	this->mr = dynamic_cast<TH12Reader*>(mr);
	UFOactive = false;
	ui.setupUi(this);
	updateTimer = new QTimer(this);
	updateTimer->setInterval(100);
	updateTimer->start();
	ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

	connect(updateTimer, &QTimer::timeout, this, &ShowInfo);
}

UFOWindow::~UFOWindow()
{
}

void UFOWindow::ReadUFO()
{
	try
	{
		if (UFOactive)
		{
			int powerItem = mr->GetPowerCount();
			int pointItem = mr->GetPointCount();
			int PIV = mr->GetSpecials()[0];
			int power = mr->GetPower();
			ufos.back().SetPointItemCount(pointItem);
			ufos.back().SetPowerItemCount(powerItem);
			ufos.back().SetPIV(PIV);
			ufos.back().SetPower(power);
			if (mr->GetUFOCount() == 0)
				UFOactive = false;
		}
		else
		{
			if (mr->GetUFOCount() == 3)
			{
				int ufo1 = mr->GetUFO1();
				int ufo2 = mr->GetUFO2();
				int ufo3 = mr->GetUFO3();
				UFOInfo ufo(ufo1, ufo2, ufo3);
				ufo.SetStage(mr->GetStage());
				ufos.push_back(ufo);
				UFOactive = true;
				//增加三栏
				int row = 3 * (ufos.size() - 1);
				ui.tableWidget->insertRow(row);
				ui.tableWidget->setItem(row, 0, new QTableWidgetItem(tr("Stage")));
				ui.tableWidget->setItem(row, 1, new QTableWidgetItem());
				ui.tableWidget->setItem(row, 2, new QTableWidgetItem(tr("UFO Type")));
				ui.tableWidget->setItem(row, 3, new QTableWidgetItem());
				ui.tableWidget->insertRow(++row);
				ui.tableWidget->setItem(row, 0, new QTableWidgetItem(tr("Power Item")));
				ui.tableWidget->setItem(row, 1, new QTableWidgetItem());
				ui.tableWidget->setItem(row, 2, new QTableWidgetItem(tr("Point Item")));
				ui.tableWidget->setItem(row, 3, new QTableWidgetItem());
				ui.tableWidget->insertRow(++row);
				ui.tableWidget->setItem(row, 0, new QTableWidgetItem(tr("Score")));
				ui.tableWidget->setItem(row, 1, new QTableWidgetItem());
				ui.tableWidget->setSpan(row, 1, 1, 3);
			}
		}
	}
	catch (const std::exception&)
	{
	}
}

void UFOWindow::ShowInfo()
{
	int row = 3 * (ufos.size() - 1);
	if (row < 0 || !UFOactive)
	{
		return;
	}
	UFOInfo* ufo = &ufos.back();
	//第一行
	ui.tableWidget->item(row, 1)->setText(QString::number(ufo->GetStage()));
	ui.tableWidget->item(row, 3)->setText(ufo->GetUFOTypeName());
	//第二行
	ui.tableWidget->item(row, 1)->setText(QString::number(ufo->GetPowerItemCount()));
	ui.tableWidget->item(row, 3)->setText(QString::number(ufo->GetPointItemCount()));
	//第三行
	ui.tableWidget->item(row, 1)->setText(QString::number(ufo->GetBonusScore()));
}