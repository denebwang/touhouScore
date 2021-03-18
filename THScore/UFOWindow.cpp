#include "UFOWindow.h"
#include "MemoryReader.h"
#include "csvIO.h"
#include "logger.h"
#include <exception>
#include <filesystem>
#include <QTimer>
#include <QHeaderView>
#include <QTableWidget>
#include <QScrollBar>
#include <QTableWidgetItem>
#include <QString>
#include <QFont>
#include <QAbstractSlider>
#include <QFileInfo>
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
	ScanCSV();

	ui.setupUi(this);
	ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	//ui.tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);



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

void UFOWindow::ScanCSV()
{
	using namespace std;
	namespace fs = filesystem;
	vector<fs::path> files;
	try
	{
		for (auto& p : fs::directory_iterator("csv/ufo"))
			files.push_back(p.path());
	}
	catch (fs::filesystem_error e)
	{
		logger->error("ScanCSV error: {0}", e.what());
	}
	for (auto& file : files)
	{
		QFileInfo fileInfo(file);
		if (fileInfo.suffix() == "csv")
		{
			logger->info("Found a csv file {0}", fileInfo.fileName().toUtf8().data());
			CSVReader reader(file);
			QStringList strList = reader.ReadRow();
			int game = 12;
			int diff = strList[0].toInt();
			int shot = strList[1].toInt();
			PatternHeader header{ game,diff,shot };

			UFOPatternFileMap.insert(std::make_pair(header, file));
		}
	}
}

const std::unordered_map < PatternHeader, std::filesystem::path>& UFOWindow::GetPatternFileMap()
{
	return UFOPatternFileMap;
}

std::filesystem::path& UFOWindow::GetPatternFilePath(int diff, int shot)
{
	PatternHeader header{ 12,diff,shot };
	return UFOPatternFileMap.at(header);
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
				//增加5行
				//飞碟计数
				int row = 5 * (ufos.size() - 1);
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
				//第一个
				newItem = new QTableWidgetItem(tr("est. Point Item"));
				newItem->setTextAlignment(Qt::AlignCenter);
				ui.tableWidget->setItem(row, 0, newItem);
				//第二个
				newItem = new QTableWidgetItem();
				newItem->setTextAlignment(Qt::AlignCenter);
				ui.tableWidget->setItem(row, 1, newItem);
				//第三个
				newItem = new QTableWidgetItem(tr("Delta"));
				newItem->setTextAlignment(Qt::AlignCenter);
				ui.tableWidget->setItem(row, 2, newItem);
				//第四个
				newItem = new QTableWidgetItem();
				newItem->setTextAlignment(Qt::AlignCenter);
				ui.tableWidget->setItem(row, 3, newItem);
				//第5行
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
	int row = 5 * (ufos.size() - 1);
	if (row < 0 || !UFOactive)
	{
		return;
	}
	UFOInfo* ufo = &ufos.back();
	//第2行
	row++;
	ui.tableWidget->item(row, 1)->setText(QString::number(ufo->GetStage()));
	ui.tableWidget->item(row, 3)->setText(ufo->GetUFOTypeName());
	//第3行
	row++;
	ui.tableWidget->item(row, 1)->setText(QString::number(ufo->GetPowerItemCount()));
	ui.tableWidget->item(row, 3)->setText(QString::number(ufo->GetPointItemCount()));
	//第4行
	row++;
	if (ufos.size() < patternUFO.size())
	{
		int patternPoint = patternUFO.at(ufos.size() - 1).GetPointItemCount();
		ui.tableWidget->item(row, 1)->setText(QString::number(patternPoint));
		int delta = ui.tableWidget->item(row - 1, 3)->text().toInt() - patternPoint;
		ui.tableWidget->item(row, 3)->setText(QString::number(delta));
	}
	//第5行
	row++;
	ui.tableWidget->item(row, 1)->setText(QString::number(ufo->GetBonusScore()));
}

void UFOWindow::OnRetry()
{
	ui.tableWidget->setRowCount(0);
	ufos.clear();
	UFOactive = false;
}

void UFOWindow::OnShottypeChanged(int diff, int shot)
{
	patternUFO.clear();
	PatternHeader header{ 12,diff,shot };
	std::filesystem::path file;
	try
	{
		file = UFOPatternFileMap.at(header);
	}
	catch (std::out_of_range& e)
	{
		return;
	}
	//读取飞碟数据
	CSVReader reader(file);
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
		patternUFO.push_back(newUfo);
		ufoCount++;
	}
}

std::unordered_map<PatternHeader, std::filesystem::path> UFOWindow::UFOPatternFileMap;