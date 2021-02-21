/********************************************************************************
** Form generated from reading UI file 'mainwindowjtCkcR.ui'
**
** Created by: Qt User Interface Compiler version 6.0.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef MAINWINDOWJTCKCR_H
#define MAINWINDOWJTCKCR_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindowClass
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QFrame *headerFrame;
    QHBoxLayout *horizontalLayout;
    QLabel *gameNameLabel;
    QLabel *DiffLabel;
    QLabel *ShottypeLabel;
    QTableWidget *tableWidget;
    QMenuBar *menuBar;

    void setupUi(QMainWindow *MainWindowClass)
    {
        if (MainWindowClass->objectName().isEmpty())
            MainWindowClass->setObjectName(QString::fromUtf8("MainWindowClass"));
        MainWindowClass->resize(500, 400);
        MainWindowClass->setMinimumSize(QSize(0, 0));
        MainWindowClass->setMaximumSize(QSize(16777215, 16777215));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icon/main.png"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindowClass->setWindowIcon(icon);
        MainWindowClass->setTabShape(QTabWidget::Rounded);
        centralWidget = new QWidget(MainWindowClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
        centralWidget->setSizePolicy(sizePolicy);
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        headerFrame = new QFrame(centralWidget);
        headerFrame->setObjectName(QString::fromUtf8("headerFrame"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(headerFrame->sizePolicy().hasHeightForWidth());
        headerFrame->setSizePolicy(sizePolicy1);
        headerFrame->setMaximumSize(QSize(16777215, 80));
        headerFrame->setFrameShape(QFrame::Panel);
        headerFrame->setFrameShadow(QFrame::Sunken);
        headerFrame->setLineWidth(2);
        headerFrame->setMidLineWidth(0);
        horizontalLayout = new QHBoxLayout(headerFrame);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        gameNameLabel = new QLabel(headerFrame);
        gameNameLabel->setObjectName(QString::fromUtf8("gameNameLabel"));
        gameNameLabel->setFrameShape(QFrame::NoFrame);
        gameNameLabel->setFrameShadow(QFrame::Raised);
        gameNameLabel->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(gameNameLabel);

        DiffLabel = new QLabel(headerFrame);
        DiffLabel->setObjectName(QString::fromUtf8("DiffLabel"));
        DiffLabel->setFrameShape(QFrame::NoFrame);
        DiffLabel->setFrameShadow(QFrame::Raised);
        DiffLabel->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(DiffLabel);

        ShottypeLabel = new QLabel(headerFrame);
        ShottypeLabel->setObjectName(QString::fromUtf8("ShottypeLabel"));
        ShottypeLabel->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(ShottypeLabel);


        verticalLayout->addWidget(headerFrame);

        tableWidget = new QTableWidget(centralWidget);
        if (tableWidget->columnCount() < 1)
            tableWidget->setColumnCount(1);
        if (tableWidget->rowCount() < 1)
            tableWidget->setRowCount(1);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget->setItem(0, 0, __qtablewidgetitem);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
        tableWidget->setFocusPolicy(Qt::NoFocus);
        tableWidget->setFrameShape(QFrame::Panel);
        tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        tableWidget->setAutoScroll(false);
        tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableWidget->setTabKeyNavigation(false);
        tableWidget->setDragEnabled(false);
        tableWidget->setSelectionMode(QAbstractItemView::NoSelection);
        tableWidget->setShowGrid(true);
        tableWidget->setGridStyle(Qt::SolidLine);
        tableWidget->setCornerButtonEnabled(false);
        tableWidget->setRowCount(1);
        tableWidget->setColumnCount(1);
        tableWidget->horizontalHeader()->setCascadingSectionResizes(false);
        tableWidget->horizontalHeader()->setDefaultSectionSize(100);
        tableWidget->horizontalHeader()->setHighlightSections(false);
        tableWidget->horizontalHeader()->setProperty("showSortIndicator", QVariant(false));
        tableWidget->horizontalHeader()->setStretchLastSection(true);
        tableWidget->verticalHeader()->setVisible(false);

        verticalLayout->addWidget(tableWidget);

        MainWindowClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindowClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 500, 22));
        MainWindowClass->setMenuBar(menuBar);

        retranslateUi(MainWindowClass);

        QMetaObject::connectSlotsByName(MainWindowClass);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindowClass)
    {
        MainWindowClass->setWindowTitle(QCoreApplication::translate("MainWindowClass", "THScore", nullptr));
        gameNameLabel->setText(QCoreApplication::translate("MainWindowClass", "\346\270\270\346\210\217", nullptr));
        DiffLabel->setText(QCoreApplication::translate("MainWindowClass", "\351\232\276\345\272\246", nullptr));
        ShottypeLabel->setText(QCoreApplication::translate("MainWindowClass", "\346\234\272\344\275\223", nullptr));

        const bool __sortingEnabled = tableWidget->isSortingEnabled();
        tableWidget->setSortingEnabled(false);
        QTableWidgetItem *___qtablewidgetitem = tableWidget->item(0, 0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MainWindowClass", "\346\255\243\345\234\250\346\211\253\346\217\217\346\270\270\346\210\217\350\277\233\347\250\213...", nullptr));
        tableWidget->setSortingEnabled(__sortingEnabled);

    } // retranslateUi

};

namespace Ui {
    class MainWindowClass: public Ui_MainWindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // MAINWINDOWJTCKCR_H
