#pragma once
#include "MemoryReader.h"
#include "UFOInfo.h"
#include <vector>
#include <QWidget>
#include <QTimer>
#include "ui_UFOWindow.h"

class UFOWindow : public QWidget
{
	Q_OBJECT

public:
	UFOWindow(MemoryReader* mr, QWidget* parent = Q_NULLPTR);
	~UFOWindow();

	//slot
	void ReadUFO();
	void ShowInfo();
	void OnRetry();
private:
	Ui::UFOWindow ui;
	TH12Reader* mr;
	std::vector<UFOInfo> ufos;
	QTimer* updateTimer;
	bool UFOactive;
};
