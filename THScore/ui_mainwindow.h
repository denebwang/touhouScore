/********************************************************************************
** Form generated from reading UI file 'mainwindowjEwmJs.ui'
**
** Created by: Qt User Interface Compiler version 6.0.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef MAINWINDOWJEWMJS_H
#define MAINWINDOWJEWMJS_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QToolBar>
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
    QStackedWidget *stackedWidget;
    QWidget *defaultPage;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QWidget *tablePage;
    QHBoxLayout *horizontalLayout_3;
    QTableWidget *tableWidget;
    QMenuBar *menuBar;
    QToolBar *toolBar;

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

        stackedWidget = new QStackedWidget(centralWidget);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        defaultPage = new QWidget();
        defaultPage->setObjectName(QString::fromUtf8("defaultPage"));
        horizontalLayout_2 = new QHBoxLayout(defaultPage);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label = new QLabel(defaultPage);
        label->setObjectName(QString::fromUtf8("label"));
        label->setTextFormat(Qt::RichText);
        label->setAlignment(Qt::AlignCenter);

        horizontalLayout_2->addWidget(label);

        stackedWidget->addWidget(defaultPage);
        tablePage = new QWidget();
        tablePage->setObjectName(QString::fromUtf8("tablePage"));
        horizontalLayout_3 = new QHBoxLayout(tablePage);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        tableWidget = new QTableWidget(tablePage);
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

        horizontalLayout_3->addWidget(tableWidget);

        stackedWidget->addWidget(tablePage);

        verticalLayout->addWidget(stackedWidget);

        MainWindowClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindowClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 500, 22));
        MainWindowClass->setMenuBar(menuBar);
        toolBar = new QToolBar(MainWindowClass);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        toolBar->setMovable(false);
        toolBar->setAllowedAreas(Qt::TopToolBarArea);
        MainWindowClass->addToolBar(Qt::TopToolBarArea, toolBar);

        retranslateUi(MainWindowClass);

        stackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindowClass);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindowClass)
    {
        MainWindowClass->setWindowTitle(QCoreApplication::translate("MainWindowClass", "THScore", nullptr));
        gameNameLabel->setText(QCoreApplication::translate("MainWindowClass", "\346\270\270\346\210\217", nullptr));
        DiffLabel->setText(QCoreApplication::translate("MainWindowClass", "\351\232\276\345\272\246", nullptr));
        ShottypeLabel->setText(QCoreApplication::translate("MainWindowClass", "\346\234\272\344\275\223", nullptr));
        label->setText(QCoreApplication::translate("MainWindowClass", "Serching for games...", nullptr));

        const bool __sortingEnabled = tableWidget->isSortingEnabled();
        tableWidget->setSortingEnabled(false);
        tableWidget->setSortingEnabled(__sortingEnabled);

        toolBar->setWindowTitle(QCoreApplication::translate("MainWindowClass", "toolBar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindowClass: public Ui_MainWindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // MAINWINDOWJEWMJS_H
