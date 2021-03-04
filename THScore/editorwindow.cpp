#include "editorwindow.h"
#include <vector>
#include <QStringList>
#include <QString>
#include <QLocale>
#include <QComboBox>
#include <QTableWidget>
#include <QTreeWidgetItem>
#include <exception>
#include "logger.h"
#include "GameInfo.h"
#include "SectionInfo.h"

EditorWindow::EditorWindow(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.tableWidget->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
	gameInfo = nullptr;
	diff = 0;
	shot = 0;
	loc = QLocale::English;
	QString unselected("Unselected");
	QStringList gameList;
	//空白即缺省
	gameList << unselected << "東方紅魔郷" << "東方妖々夢" << "東方永夜抄"
		<< "東方風神録" << "東方地霊殿" << "東方星蓮船" << "妖精大戦争"
		<< "東方神霊廟" << "東方輝針城" << "東方紺珠伝"
		<< "東方天空璋" << "東方鬼形獣" << "東方虹龍洞";
	ui.GameCombo->addItems(gameList);
	ui.DiffCombo->addItem(unselected);
	for (auto& str : GameInfo::DiffList)
	{
		ui.DiffCombo->addItem(str);
	}

	//根据所选游戏更新机体
	connect(ui.GameCombo, &QComboBox::currentTextChanged, [=](const QString& gameName)
		{
			try
			{
				if (gameInfo != nullptr)
				{
					delete gameInfo;
				}
				gameInfo = new GameInfo(static_cast<Game>(GetGameIndex(gameName)));
			}
			catch (std::out_of_range& e)
			{
				logger->warn("{0} not supported yet!", gameName.toUtf8().data());
			}
			catch (std::exception& e)
			{
				return;
			}
			ui.ShotCombo->clear();
			ui.ShotCombo->addItem(unselected);
			std::vector<QString> shottypeList = gameInfo->GetShotTypeList();
			for (auto& str : shottypeList)
			{
				ui.ShotCombo->addItem(str);
			}
		});
	connect(ui.DiffCombo, &QComboBox::currentTextChanged, [=](const QString& diffName)
		{
			diff = GetDiffIndex(diffName);
		});
	connect(ui.ShotCombo, &QComboBox::currentTextChanged, [=](const QString& shotName)
		{
			if (shotName == "Unselected")
			{
				ui.formWidget->setCurrentIndex(0);
				//todo: 显示所有路线
			}
			else
			{
				shot = GetShotIndex(shotName);
				gameInfo->SetInfo(diff, shot);
				ui.formWidget->setCurrentIndex(1);
				ui.tableWidget->clear();
				ui.tableWidget->setColumnCount(gameInfo->ColumnCount() * 2 - 3);//每列增加一列用于放增量,并增加一列用于放置选择section组合的下拉框
				QStringList header, specials;
				header << "Stage" << "" << "Section" << "Score" << "Delta";
				specials = gameInfo->GetSpecialNames();
				for (auto& str : specials)
				{
					header << str << "Delta";
				}
				ui.tableWidget->setHorizontalHeaderLabels(header);
				ui.tableWidget->setRowCount(gameInfo->SectionCount());
				//todo: 初始化表格
				//设置面数单元格合并
				int row = 0;
				for (int i = 0; i < 6; i++)
				{
					int sectionCount = gameInfo->GetStageSectionCount(i);
					ui.tableWidget->setSpan(row, 0, sectionCount, 1);
					QTableWidgetItem* newStage = new QTableWidgetItem(QString::number(i + 1));
					newStage->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(row, 0, newStage);
					ui.tableWidget->setSpan(row, 1, sectionCount, 1);
					//todo: 下拉框改变section
					ui.tableWidget->setCellWidget(row, 1, new QComboBox);
					std::vector<SectionInfo> sections = gameInfo->GetSectionInfos(i);
					for (int index = 0; index < sections.size(); index++)
					{
						int rowIndex = row + index;
						ui.tableWidget->setItem(rowIndex, 2, new QTableWidgetItem(sections[index].GetSectionName()));
						long long score = sections[index].GetScore(1);
						ui.tableWidget->setItem(rowIndex, 3, new QTableWidgetItem(loc.toString(score)));
						long long dScore = rowIndex == 0 ? score :
							score - loc.toLongLong(ui.tableWidget->item(rowIndex - 1, 3)->text());
						ui.tableWidget->setItem(rowIndex, 4, new QTableWidgetItem(loc.toString(dScore)));//分数差值
						auto specials = sections[index].GetSpecials(1);
						for (int j = 0; j < specials.size(); j ++)
						{
							int colIndex = 5 + j * 2;
							ui.tableWidget->setItem(rowIndex, colIndex, new QTableWidgetItem(loc.toString(specials[j])));
							int dSpecial = rowIndex == 0 ? specials[j] :
								specials[j] - loc.toInt(ui.tableWidget->item(rowIndex - 1, colIndex)->text());
							ui.tableWidget->setItem(rowIndex, colIndex + 1 , new QTableWidgetItem(loc.toString(dSpecial)));
						}
					}
					row += sectionCount;
				}
				//填充数据



				ui.tableWidget->adjustSize();
			}

		});





}

EditorWindow::~EditorWindow()
{
}

const int EditorWindow::GetGameIndex(const QString& gameName)
{
	if (gameName == "東方紅魔郷")
	{
		return 6;
	}
	else if (gameName == "東方妖々夢")
	{
		return 7;
	}
	else if (gameName == "東方永夜抄")
	{
		return 8;
	}
	else if (gameName == "東方風神録")
	{
		return 10;
	}
	else if (gameName == "東方地霊殿")
	{
		return 11;
	}
	else if (gameName == "東方星蓮船")
	{
		return 12;
	}
	else if (gameName == "妖精大戦争")
	{
		return 128;
	}
	else if (gameName == "東方神霊廟")
	{
		return 13;
	}
	else if (gameName == "東方輝針城")
	{
		return 14;
	}
	else if (gameName == "東方紺珠伝")
	{
		return 15;
	}
	else if (gameName == "東方天空璋")
	{
		return 16;
	}
	else if (gameName == "東方鬼形獣")
	{
		return 17;
	}
	else if (gameName == "東方虹龍洞")
	{
		return 18;
	}
	else {
		throw std::runtime_error("Wrong Game Name");
	}
}

const int EditorWindow::GetDiffIndex(const QString& diffName)
{
	if (diffName == "Easy")
	{
		return 0;
	}
	else if (diffName == "Normal")
	{
		return 1;
	}
	else if (diffName == "Hard")
	{
		return 2;
	}
	else if (diffName == "Lunatic")
	{
		return 3;
	}
	else return diff;
}

const int EditorWindow::GetShotIndex(const QString& shotName)
{
	const std::vector<QString> shotList = gameInfo->GetShotTypeList();
	for (int i = 0; i < shotList.size(); i++)
	{
		if (shotList[i] == shotName)
		{
			return i;
		}
	}
	return shot;
}
