#pragma once
#include <vector>
#include <QWidget>
#include <QMenu>
#include <QAction>
#include "ui_ufoeditwin.h"
#include "UFOInfo.h"

class UFOeditWin : public QWidget
{
	Q_OBJECT

public:
	UFOeditWin(int diff, int shot, QWidget *parent = Q_NULLPTR);
	~UFOeditWin();
	//slot
	void setPattern(int diff, int shot);
	void setUFOdata(int row, int column);
	void showChart();
	void insertRow();
	void removeRow();
	void onCustomContextMenuRequested(const QPoint& pos);
	void save();
private:
	Ui::UFOeditWin ui;
	std::vector<UFOInfo> patterns;
	QMenu* tableRightClickMenu;
	QAction* insertRowAction;
	QAction* removeRowAction;
	int diff;
	int shot;
};
