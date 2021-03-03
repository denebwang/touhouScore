#pragma once

#include "GameInfo.h"
#include <QWidget>
#include <QString>
#include <vector>
#include "ui_editorwindow.h"

class EditorWindow : public QWidget
{
	Q_OBJECT

public:
	EditorWindow(QWidget *parent = Q_NULLPTR);
	~EditorWindow();
	
private:
	Ui::EditorWindow ui;
	const int GetGameIndex(const QString& gameName);
	const int GetDiffIndex(const QString& diffName);
	const int GetShotIndex(const QString& shotName);
	GameInfo* gameInfo;

	int shot;
	int diff;
};
