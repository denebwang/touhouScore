﻿#pragma once

#include <QtWidgets/QMainWindow>
#include <QTimer>
#include "GameInfo.h"
#include "MemoryReader.h"
#include "ui_mainwindow.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = Q_NULLPTR);
    ~MainWindow(); 
    void ScanGame();
    void UpdateInfo();
    void InitChart();
    void ShowInfo();
    void UpdatePattern();
private:
    Ui::MainWindowClass ui;
    QTimer* GameScanTimer;
    QTimer* InfoUpdateTimer;
    MemoryReader* mr;
    GameInfo* gameInfo;
   
signals:
    void FoundGame(bool found);
    void ReadSuccees();
    void NewShottype();
};