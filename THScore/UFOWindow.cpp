#include "UFOWindow.h"
#include "MemoryReader.h"
#include "logger.h"
#include <exception>
#include <QTimer>
#include <QHeaderView>
#include <QTableWidget>
#include <QScrollBar>
#include <QTableWidgetItem>
#include <QString>
#include <QFont>
#include <QAbstractSlider>
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
	//ui.tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

	connect(updateTimer, &QTimer::timeout, this, &UFOWindow::ShowInfo);

	//滚动条拉到最低
	auto* scrollBar = ui.tableWidget->verticalScrollBar();
	connect(scrollBar, &QAbstractSlider::rangeChanged, [=](int min, int max)
		{
			Q_UNUSED(min);
			scrollBar->setValue(max);
		});
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
			if (mr->GetUFOCount() == 0)
			{
				UFOactive = false;
				return;
			}
			int powerItem = mr->GetPowerCount();
			int pointItem = mr->GetPointCount();
			int PIV = mr->GetSpecials()[0];
			int power = mr->GetPower();
			ufos.back().SetPointItemCount(pointItem);
			ufos.back().SetPowerItemCount(powerItem);
			ufos.back().SetPIV(PIV);
			ufos.back().SetPower(power);
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
				//增加4行
				//飞碟计数
				int row = 4 * (ufos.size() - 1);
				QTableWidgetItem* newItem;
				ui.tableWidget->insertRow(row);
				newItem = new QTableWidgetItem(tr("UFO %1").arg(ufos.size()));
				newItem->setTextAlignment(Qt::AlignCenter);
				QFont font = newItem->font();
				font.setBold(true);
				newItem->setFont(font);
				ui.tableWidget->setItem(row, 0, newItem);
				ui.tableWidget->setSpan(row, 0, 1, 4);
				//第2行
				ui.tableWidget->insertRow(++row);
				//第一个
				newItem = new QTableWidgetItem(tr("Stage"));
				newItem->setTextAlignment(Qt::AlignCenter);
				ui.tableWidget->setItem(row, 0, newItem);
				//第二个
				newItem = new QTableWidgetItem();
				newItem->setTextAlignment(Qt::AlignCenter);
				ui.tableWidget->setItem(row, 1, newItem);
				//第三个
				newItem = new QTableWidgetItem(tr("UFO Type"));
				newItem->setTextAlignment(Qt::AlignCenter);
				ui.tableWidget->setItem(row, 2, newItem);
				//第四个
				newItem = new QTableWidgetItem();
				newItem->setTextAlignment(Qt::AlignCenter);
				ui.tableWidget->setItem(row, 3, newItem);
				//第3行
				ui.tableWidget->insertRow(++row);
				//第一个
				newItem = new QTableWidgetItem(tr("Power Item"));
				newItem->setTextAlignment(Qt::AlignCenter);
				ui.tableWidget->setItem(row, 0, newItem);
				//第二个
				newItem = new QTableWidgetItem();
				newItem->setTextAlignment(Qt::AlignCenter);
				ui.tableWidget->setItem(row, 1, newItem);
				//第三个
				newItem = new QTableWidgetItem(tr("Point Item"));
				newItem->setTextAlignment(Qt::AlignCenter);
				ui.tableWidget->setItem(row, 2, newItem);
				//第四个
				newItem = new QTableWidgetItem();
				newItem->setTextAlignment(Qt::AlignCenter);
				ui.tableWidget->setItem(row, 3, newItem);
				//第4行
				ui.tableWidget->insertRow(++row);
				newItem = new QTableWidgetItem(tr("Score"));
				newItem->setTextAlignment(Qt::AlignCenter);
				ui.tableWidget->setItem(row, 0, newItem);

				newItem = new QTableWidgetItem();
				newItem->setTextAlignment(Qt::AlignCenter);
				ui.tableWidget->setItem(row, 1, newItem);
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
	int row = 4 * (ufos.size() - 1);
	if (row < 0 || !UFOactive)
	{
		return;
	}
	UFOInfo* ufo = &ufos.back();
	row++;
	//第2行
	ui.tableWidget->item(row, 1)->setText(QString::number(ufo->GetStage()));
	ui.tableWidget->item(row, 3)->setText(ufo->GetUFOTypeName());
	row++;
	//第3行
	ui.tableWidget->item(row, 1)->setText(QString::number(ufo->GetPowerItemCount()));
	ui.tableWidget->item(row, 3)->setText(QString::number(ufo->GetPointItemCount()));
	row++;
	//第4行
	ui.tableWidget->item(row, 1)->setText(QString::number(ufo->GetBonusScore()));
}

void UFOWindow::OnRetry()
{
	ui.tableWidget->setRowCount(0);
	ufos.clear();
	UFOactive = false;
}
