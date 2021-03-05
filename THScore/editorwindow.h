#pragma once

#include "GameInfo.h"
#include <QWidget>
#include <QString>
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
	void UpdateSectionType(int row,int col);
private:
	QStringList SectionTypeList;
	Ui::EditorWindow ui;
	const int GetGameIndex(const QString& gameName);
	const int GetDiffIndex(const QString& diffName);
	const int GetShotIndex(const QString& shotName);
	void SetPatternList();
	GameInfo* gameInfo;
	QLocale loc;
	std::vector<GameInfo::patternHeader> patterns;
	int game;
	int shot;
	int diff;
};
