#pragma once
#include "MemoryReader.h"
#include "GameInfo.h"
#include "UFOInfo.h"
#include <vector>
#include <unordered_map>
#include <filesystem>
#include <QWidget>
#include <QFile>
#include <QTimer>
#include <QTextStream>
#include <QString>
#include <QStringList>
#include "ui_UFOWindow.h"

class UFOWindow : public QWidget
{
	Q_OBJECT

public:
	UFOWindow(MemoryReader* mr, QWidget* parent = Q_NULLPTR);
	~UFOWindow();

	static void ScanCSV();
	//slot
	void ReadUFO();
	void ShowInfo();
	void OnRetry();
	void OnShottypeChanged(int diff, int shot);
private:
	Ui::UFOWindow ui;
	TH12Reader* mr;
	std::vector<UFOInfo> ufos;
	std::vector<UFOInfo> patternUFO;
	//QTimer* updateTimer;
	bool UFOactive;
	static std::unordered_map<PatternHeader, std::filesystem::path> UFOPatternFileMap;
};
