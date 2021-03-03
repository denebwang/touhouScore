#include "editorwindow.h"
#include <QStringList>
#include <QString>
#include <QComboBox>
#include <exception>
#include "logger.h"
#include "GameInfo.h"

EditorWindow::EditorWindow(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	gameInfo = nullptr;
	diff = 0;
	shot = 0;
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
			ui.ShotCombo->clear();
			ui.ShotCombo->addItem(unselected);
			std::vector<QString> shottypeList;
			try
			{
				shottypeList = GameInfo::GetShotTypeList(GetGameIndex(gameName));
			}
			catch (std::out_of_range& e)
			{
				logger->warn("{0} not supported yet!", gameName.toUtf8().data());
			}
			catch (std::runtime_error& e)
			{
				//logger->error("Game name error: {}", gameName.toUtf8().data());
			}
			for (auto& str : shottypeList)
			{
				ui.ShotCombo->addItem(str);
			}
		});
	connect(ui.GameCombo, &QComboBox::currentTextChanged, [=](const QString& gameName)
		{
			try
			{
				gameInfo = new GameInfo(static_cast<Game>(GetGameIndex(gameName)));
			}
			catch (std::exception& e)
			{
				return;
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
				ui.tableWidget->setColumnCount(gameInfo->ColumnCount());
				ui.tableWidget->setRowCount(gameInfo->SectionCount());
				//todo: 初始化表格
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
