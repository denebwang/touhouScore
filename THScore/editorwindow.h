#pragma once

#include "GameInfo.h"
#include "Enums.h"
#include <QWidget>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <vector>
#include <QLocale>
#include "ui_editorwindow.h"

class EditorWindow : public QWidget
{
	Q_OBJECT

public:
	EditorWindow(QWidget* parent = Q_NULLPTR);
	~EditorWindow();
	void UpdatePattern();
	void UpdatePatternList();
	void UpdateSectionType(int row, int col);
	void UpdateTable(int row, int col);
	void SaveCSV();
private:
	class CSVWriter
	{
	public:
		CSVWriter(int game, int diff, int shot);
		~CSVWriter();
		void WriteLine(const QString& str);
	private:
		QFile* file;
		QTextStream* ts;
	};
	QStringList SectionTypeList;
	Ui::EditorWindow ui;
	const int GetGameIndex(const QString& gameName);
	const int GetDiffIndex(const QString& diffName);
	const int GetShotIndex(const QString& shotName);
	const Section GetSection(const QString& secName);
	void SetPatternList();
	GameInfo* gameInfo;
	QLocale loc;
	std::vector<GameInfo::patternHeader> patterns;
	int game;
	int shot;
	int diff;
};
