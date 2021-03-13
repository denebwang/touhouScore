#include "editorwindow.h"
#include <vector>
#include <QStringList>
#include <QString>
#include <QLocale>
#include <QComboBox>
#include <QTableWidget>
#include <QListWidget>
#include <QTableWidgetItem>
#include <QFile>
#include <QTextStream>
#include <QHeaderView>
#include <QAbstractScrollArea>
#include <QPushButton>
#include <QMessageBox>
#include <Qt>
#include <exception>
#include <filesystem>
#include "logger.h"
#include "GameInfo.h"
#include "SectionInfo.h"
#include "ComboCell.h"

EditorWindow::EditorWindow(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.tableWidget->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
	gameInfo = nullptr;
	diff = -1;
	shot = -1;
	game = 0;
	SectionTypeList << tr("All") << tr("Mid+Boss") << tr("Mid+Boss+Bonus");
	ui.formWidget->setCurrentIndex(0);
	UpdatePatternList();
	ui.saveButton->setEnabled(false);
	loc = QLocale::English;
	QString unselected(tr("Unselected"));
	QStringList gameList;
	//空白即缺省
	gameList << unselected << "東方紅魔郷" << "東方妖々夢" << "東方永夜抄"
		<< "東方風神録" << "東方地霊殿" << "東方星蓮船" << "妖精大戦争"
		<< "東方神霊廟" << "東方輝針城" << "東方紺珠伝"
		<< "東方天空璋" << "東方鬼形獣" << "東方虹龍洞";
	ui.GameCombo->addItems(gameList);
	ui.DiffCombo->addItem(unselected);
	ui.ShotCombo->addItem(unselected);
	for (auto& str : GameInfo::DiffList)
	{
		ui.DiffCombo->addItem(str);
	}

	connect(ui.GameCombo, &QComboBox::currentTextChanged, [=](const QString& gameName)
		{
			if (gameName == unselected)
			{
				game = 0;
				delete gameInfo;
				gameInfo = nullptr;
				ui.ShotCombo->clear();
				ui.ShotCombo->addItem(unselected);
				ui.DiffCombo->setCurrentIndex(0);
				ui.ShotCombo->setCurrentIndex(0);
				ui.formWidget->setCurrentIndex(0);
			}
			else
			{
				//根据所选游戏更新机体
				try
				{
					game = GetGameIndex(gameName);
					if (gameInfo != nullptr)
					{
						delete gameInfo;
					}
					gameInfo = new GameInfo(static_cast<Game>(game));
				}
				catch (std::exception& e)
				{
					logger->warn("{0} not supported yet!", gameName.toUtf8().data());
					QMessageBox::warning(this, tr("Game not supported"), QString(tr("%1 is not supported yet")).arg(gameName));
					ui.GameCombo->setCurrentIndex(0);
					game = 0;
					UpdatePatternList();
					return;
				}
				ui.ShotCombo->clear();
				ui.ShotCombo->addItem(unselected);
				std::vector<QString> shottypeList = gameInfo->GetShotTypeList();
				for (auto& str : shottypeList)
				{
					ui.ShotCombo->addItem(str);
				}
			}
		});
	connect(ui.DiffCombo, &QComboBox::currentTextChanged, [=](const QString& diffName)
		{
			diff = GetDiffIndex(diffName);
			if (diffName == unselected || shot == -1)
			{
				UpdatePatternList();
				ui.formWidget->setCurrentIndex(0);
				ui.saveButton->setEnabled(false);
				return;
			}
			else
			{
				try
				{
					gameInfo->SetInfo(diff, shot);
				}
				catch (std::out_of_range& e)
				{
					QMessageBox::warning(this, tr("Pattern not Found or Invalid"), QString(tr("Pattern for %1 %2 %3 is not a valid pattern file."))
						.arg(gameInfo->GameName())
						.arg(gameInfo->Difficulty())
						.arg(gameInfo->ShotType()));
				}
				UpdatePattern();
			}
		});
	connect(ui.ShotCombo, &QComboBox::currentTextChanged, [=](const QString& shotName)
		{//todo: 筛选对应机体的路线
			if (gameInfo == nullptr)
			{
				return;
			}
			shot = GetShotIndex(shotName);
			if (diff == -1 || shotName == unselected)
			{
				UpdatePatternList();
				ui.formWidget->setCurrentIndex(0);
				ui.saveButton->setEnabled(false);
				return;
			}
			else
			{
				try
				{
					gameInfo->SetInfo(diff, shot);
				}
				catch (std::out_of_range& e)
				{
					QMessageBox::warning(this, tr("Pattern not Found or Invalid"), QString(tr("Pattern for %1 %2 %3 is not a valid pattern file."))
						.arg(gameInfo->GameName())
						.arg(gameInfo->Difficulty())
						.arg(gameInfo->ShotType()));
				}
				UpdatePattern();
			}
		});
	//选择对应路线自动设置
	connect(ui.listWidget, &QListWidget::itemDoubleClicked, [=](QListWidgetItem* item)
		{
			QStringList strList = item->text().split("-", Qt::SkipEmptyParts);
			ui.GameCombo->setCurrentText(strList[0]);
			ui.DiffCombo->setCurrentText(strList[1]);
			ui.ShotCombo->setCurrentText(strList[2]);
		});
	connect(ui.tableWidget, &QTableWidget::cellChanged, this, &EditorWindow::UpdateTable);
	connect(ui.saveButton, &QPushButton::pressed, this, &EditorWindow::SaveCSV);
}

EditorWindow::~EditorWindow()
{
	if (gameInfo != nullptr)
	{
		delete gameInfo;
	}
}

void EditorWindow::UpdatePattern()
{
	disconnect(ui.tableWidget, &QTableWidget::cellChanged, this, &EditorWindow::UpdateTable);
	ui.formWidget->setCurrentIndex(1);
	ui.saveButton->setEnabled(true);
	ui.tableWidget->clear();
	ui.tableWidget->setColumnCount(gameInfo->ColumnCount() * 2 - 3);//每列增加一列用于放增量,并增加一列用于放置选择section组合的下拉框
	QStringList header, specials;
	header << tr("Stage") << "" << tr("Section") << tr("Score") << tr("Delta");
	specials = gameInfo->GetSpecialNames();
	for (auto& str : specials)
	{
		header << str << tr("Delta");
	}
	ui.tableWidget->setHorizontalHeaderLabels(header);
	ui.tableWidget->setRowCount(gameInfo->SectionCount());
	ui.tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
	ui.tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
	ComboCell* cbc = new ComboCell(ui.tableWidget);
	ui.tableWidget->setItemDelegateForColumn(1, cbc);
	//设置面数单元格合并
	int row = 0;

	for (int i = 0; i < 6; i++)
	{
		int sectionCount = gameInfo->GetStageSectionCount(i);
		ui.tableWidget->setSpan(row, 0, sectionCount, 1);
		QTableWidgetItem* newStage = new QTableWidgetItem(QString::number(i + 1));
		newStage->setFlags(newStage->flags() & ~Qt::ItemIsEditable & ~Qt::ItemIsSelectable);
		newStage->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(row, 0, newStage);

		//todo: 下拉框改变section
		QTableWidgetItem* newSection = new QTableWidgetItem(SectionTypeList[sectionCount - 1]);
		newStage->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->setItem(row, 1, newSection);
		ui.tableWidget->setSpan(row, 1, sectionCount, 1);

		std::vector<SectionInfo> sections = gameInfo->GetSectionInfos(i);
		for (int index = 0; index < sections.size(); index++)
		{
			int rowIndex = row + index;
			QTableWidgetItem* sectionNameItem = new QTableWidgetItem(sections[index].GetSectionName());
			sectionNameItem->setFlags(sectionNameItem->flags() & ~Qt::ItemIsEditable & ~Qt::ItemIsSelectable);
			ui.tableWidget->setItem(rowIndex, 2, sectionNameItem);
			long long score = sections[index].GetScore(1);
			ui.tableWidget->setItem(rowIndex, 3, new QTableWidgetItem(loc.toString(score)));
			long long dScore = rowIndex == 0 ? score :
				score - loc.toLongLong(ui.tableWidget->item(rowIndex - 1, 3)->text());
			ui.tableWidget->setItem(rowIndex, 4, new QTableWidgetItem(loc.toString(dScore)));//分数差值
			auto specials = sections[index].GetSpecials(1);
			for (int j = 0; j < specials.size(); j++)
			{
				int colIndex = 5 + j * 2;
				ui.tableWidget->setItem(rowIndex, colIndex, new QTableWidgetItem(loc.toString(specials[j])));
				int dSpecial = rowIndex == 0 ? specials[j] :
					specials[j] - loc.toInt(ui.tableWidget->item(rowIndex - 1, colIndex)->text());
				ui.tableWidget->setItem(rowIndex, colIndex + 1, new QTableWidgetItem(loc.toString(dSpecial)));
			}
		}
		row += sectionCount;
	}
	//填充数据

	ui.tableWidget->adjustSize();
	connect(ui.tableWidget, &QTableWidget::cellChanged, this, &EditorWindow::UpdateTable);
}

void EditorWindow::UpdatePatternList()
{
	patterns.clear();
	ui.listWidget->clear();
	if (game == 0)//全部未选中
	{
		for (auto& pattern : GameInfo::GetPatternFileMap())
		{
			patterns.push_back(pattern.first);
		}
		SetPatternList();
	}
	else if (diff * shot > 0)//仅选中游戏
	{
		for (auto& pattern : GameInfo::GetPatternFileMap())
		{
			if (pattern.first.game == game)
			{
				patterns.push_back(pattern.first);
			}
		}
		SetPatternList();
	}
	else if (diff == -1)//选中了机体
	{
		for (auto& pattern : GameInfo::GetPatternFileMap())
		{
			if (pattern.first.game == game && pattern.first.shotType == shot)
			{
				patterns.push_back(pattern.first);
			}
		}
		SetPatternList();
	}
	else if (shot == -1)
	{
		for (auto& pattern : GameInfo::GetPatternFileMap())
		{
			if (pattern.first.game == game && pattern.first.difficulty == diff)
			{
				patterns.push_back(pattern.first);
			}
		}
		SetPatternList();
	}
}

void EditorWindow::UpdateSectionType(int row, int col)
{
	int type = SectionTypeList.indexOf(ui.tableWidget->item(row, col)->text());
	int stage = ui.tableWidget->item(row, 0)->text().toInt();
	std::vector<SectionInfo> sections = gameInfo->GetSectionInfos(stage - 1);
	StageInfo* si = gameInfo->GetStageInfo(stage - 1);
	si->ClearSection();
	switch (type)
	{
	case 0://all
	{
		SectionInfo last = sections.back();
		si->SetData(Section::All, 1, last.GetScore(1), last.GetSpecials(1));
		break;
	}
	case 1://m+b
	{
		switch (sections.size())
		{
		case 1:
		{
			si->SetData(Section::Mid, 1, 0, std::vector<int>(sections[0].GetSpecials(1).size()));
			si->SetData(Section::Boss, 1, sections[0].GetScore(1), sections[0].GetSpecials(1));
			break;
		}
		case 2:
		{
			si->SetData(Section::Mid, 1, sections[0].GetScore(1), sections[0].GetSpecials(1));
			si->SetData(Section::Boss, 1, sections[1].GetScore(1), sections[1].GetSpecials(1));
			break;
		}
		case 3:
			si->SetData(Section::Mid, 1, sections[0].GetScore(1), sections[0].GetSpecials(1));
			SectionInfo last = sections.back();
			si->SetData(Section::Boss, 1, last.GetScore(1), last.GetSpecials(1));
		}
		break;
	}
	case 2://m+b+b
	{
		switch (sections.size())
		{
		case 1:
			si->SetData(Section::Mid, 1, 0, std::vector<int>(sections[0].GetSpecials(1).size()));
			si->SetData(Section::Boss, 1, 0, std::vector<int>(sections[0].GetSpecials(1).size()));
			si->SetData(Section::Bonus, 1, sections[0].GetScore(1), sections[0].GetSpecials(1));
			break;
		case 2:
			si->SetData(Section::Mid, 1, sections[0].GetScore(1), sections[0].GetSpecials(1));
			si->SetData(Section::Boss, 1, 0, std::vector<int>(sections[0].GetSpecials(1).size()));
			si->SetData(Section::Bonus, 1, sections[1].GetScore(1), sections[1].GetSpecials(1));
			break;
		case 3:
			si->SetData(Section::Mid, 1, sections[0].GetScore(1), sections[0].GetSpecials(1));
			si->SetData(Section::Boss, 1, sections[1].GetScore(1), sections[1].GetSpecials(1));
			si->SetData(Section::Bonus, 1, sections[2].GetScore(1), sections[2].GetSpecials(1));
			break;
		default:
			break;
		}
		break;
	}
	default:
		logger->error("Wrong switch type in EditorWindow::UpdateSectionType,col=1");
		si->SetData(Section::All, 1, sections[0].GetScore(1), sections[0].GetSpecials(1));
		break;
	}
	UpdatePattern();
}

void EditorWindow::UpdateTable(int row, int col)
{
	if (col == 1)
		UpdateSectionType(row, col);
	if (col > 2)
	{
		long long data = loc.toLongLong(ui.tableWidget->item(row, col)->text());
		if (((col - 3) % 2) == 1)//处于delta位置上
		{
			long long score = loc.toLongLong(ui.tableWidget->item(row, col - 1)->text());
			long long newScore = 0;
			if (row == 0)
			{
				newScore = data;
			}
			else
			{
				long long last = loc.toLongLong(ui.tableWidget->item(row - 1, col - 1)->text());
				newScore = data + last;
			}
			if (newScore != score)
				ui.tableWidget->item(row, col - 1)->setData(Qt::DisplayRole, loc.toString(newScore));
		}
		else//直接编辑分数
		{
			int stage = 0;
			for (int i = row; i > -1; i--)
			{
				auto* item = ui.tableWidget->item(i, 0);
				if (item == nullptr)
				{
					continue;
				}
				stage = item->text().toInt();
				break;
			}
			if (stage == 0)
			{
				logger->error("Find stage number while writing data to info failed");
				throw std::runtime_error("EditorWindow::UpdateTable");
			}
			Section section = GetSection(ui.tableWidget->item(row, 2)->text());
			long long score = loc.toLongLong(ui.tableWidget->item(row, 3)->text());
			std::vector<int>specials;
			for (int column = 5; column < ui.tableWidget->columnCount(); column += 2)
			{
				specials.push_back(loc.toInt(ui.tableWidget->item(row, column)->text()));
			}
			gameInfo->SetPattern(stage, section, score, specials);
			//更新delta
			long long delta = loc.toLongLong(ui.tableWidget->item(row, col + 1)->text());
			long long newDelta;
			if (row == 0)
			{
				newDelta = data;
			}
			else
			{
				long long last = loc.toLongLong(ui.tableWidget->item(row - 1, col)->text());
				newDelta = data - last;
			}
			if (newDelta != delta)
			{
				ui.tableWidget->item(row, col + 1)->setData(Qt::DisplayRole, loc.toString(newDelta));
			}
			//更新下一行
			if (row + 1 < ui.tableWidget->rowCount())
			{
				long long next = loc.toLongLong(ui.tableWidget->item(row + 1, col)->text());
				long long nextDelta = loc.toLongLong(ui.tableWidget->item(row + 1, col + 1)->text());
				long long newNextDelta = next - data;
				if (newNextDelta != nextDelta)
				{
					ui.tableWidget->item(row + 1, col + 1)->setData(Qt::DisplayRole, loc.toString(newNextDelta));
				}
			}
		}
	}
}

void EditorWindow::SaveCSV()
{
	static QString delimiter(",");
	CSVWriter* writer = nullptr;
	try
	{
		writer = new CSVWriter(game, diff, shot);
	}
	catch (std::runtime_error& e)
	{
		logger->error(e.what());
		QMessageBox::warning(this, tr("Save CSV Failed"), tr("Please close any program that is used the file."));
		return;
	}
	QStringList line;
	line << QString::number(game) << QString::number(diff) << QString::number(shot);
	writer->WriteLine(line.join(delimiter));
	line.clear();
	line << "stage" << "section" << "score";
	line += gameInfo->GetSpecialNames();
	writer->WriteLine(line.join(delimiter));
	line.clear();
	for (int i = 0; i < 6; i++)
	{
		std::vector<SectionInfo> sections = gameInfo->GetSectionInfos(i);
		for (auto& section : sections)
		{
			line << QString::number(i + 1) << QString::number(static_cast<int>(section.GetSection()))
				<< QString::number(section.GetScore(1));
			std::vector<int>specials = section.GetSpecials(1);
			for (auto& spec : specials)
			{
				line << QString::number(spec);
			}
			writer->WriteLine(line.join(delimiter));
			line.clear();
		}
	}
	delete writer;
	QMessageBox::information(this, tr("Save Success!"), tr("Pattern file saved"));
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
	else return -1;
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
	return -1;
}

const Section EditorWindow::GetSection(const QString& secName)
{
	if (secName == tr("All"))
	{
		return Section::All;
	}
	if (secName == tr("Mid"))
	{
		return Section::Mid;
	}
	if (secName == tr("Boss"))
	{
		return Section::Boss;
	}
	if (secName == tr("Bonus"))
	{
		return Section::Bonus;
	}
	else throw std::logic_error("Wrong section name in EditorWindow::GetSection");
}

void EditorWindow::SetPatternList()
{
	for (auto& pattern : patterns)
	{
		QStringList patternName;
		patternName << GameInfo::GameName(static_cast<Game>(pattern.game));
		patternName << GameInfo::DiffList[pattern.difficulty];
		patternName << GameInfo::GetShotTypeList(pattern.game)[pattern.shotType];
		ui.listWidget->addItem(patternName.join("-"));
	}
}

EditorWindow::CSVWriter::CSVWriter(int game, int diff, int shot)
{
	QStringList patternName;
	patternName << GameInfo::GameName(static_cast<Game>(game));
	patternName << GameInfo::DiffList[diff];
	patternName << GameInfo::GetShotTypeList(game)[shot];
	auto filename = patternName.join("-").append(".csv").toStdWString();
	std::filesystem::path newFile(L"./csv");
	newFile /= filename;
	file = new QFile(newFile);
	if (!file->open(QIODevice::WriteOnly | QIODevice::Text))
	{
		throw std::runtime_error("Open csv file failed!");
	}
	ts = new QTextStream(file);
}

EditorWindow::CSVWriter::~CSVWriter()
{
	delete ts;
	file->close();
	delete file;
}

void EditorWindow::CSVWriter::WriteLine(const QString& str)
{
	*ts << str << Qt::endl;
}