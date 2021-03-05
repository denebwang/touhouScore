/********************************************************************************
** Form generated from reading UI file 'editorwindowpyCGuu.ui'
**
** Created by: Qt User Interface Compiler version 6.0.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef EDITORWINDOWPYCGUU_H
#define EDITORWINDOWPYCGUU_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_EditorWindow
{
public:
    QVBoxLayout *verticalLayout;
    QWidget *titleWidget;
    QHBoxLayout *horizontalLayout_4;
    QWidget *GameWidget;
    QHBoxLayout *horizontalLayout;
    QLabel *GameLabel;
    QComboBox *GameCombo;
    QWidget *DiffWidget;
    QHBoxLayout *horizontalLayout_2;
    QLabel *DiffLabel;
    QComboBox *DiffCombo;
    QWidget *ShotWidget;
    QHBoxLayout *horizontalLayout_3;
    QLabel *ShotLabel;
    QComboBox *ShotCombo;
    QStackedWidget *formWidget;
    QWidget *listPage;
    QVBoxLayout *verticalLayout_3;
    QListWidget *listWidget;
    QWidget *tablePage;
    QVBoxLayout *verticalLayout_2;
    QTableWidget *tableWidget;

    void setupUi(QWidget *EditorWindow)
    {
        if (EditorWindow->objectName().isEmpty())
            EditorWindow->setObjectName(QString::fromUtf8("EditorWindow"));
        EditorWindow->resize(800, 600);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(EditorWindow->sizePolicy().hasHeightForWidth());
        EditorWindow->setSizePolicy(sizePolicy);
        verticalLayout = new QVBoxLayout(EditorWindow);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        titleWidget = new QWidget(EditorWindow);
        titleWidget->setObjectName(QString::fromUtf8("titleWidget"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Maximum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(titleWidget->sizePolicy().hasHeightForWidth());
        titleWidget->setSizePolicy(sizePolicy1);
        horizontalLayout_4 = new QHBoxLayout(titleWidget);
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        GameWidget = new QWidget(titleWidget);
        GameWidget->setObjectName(QString::fromUtf8("GameWidget"));
        sizePolicy1.setHeightForWidth(GameWidget->sizePolicy().hasHeightForWidth());
        GameWidget->setSizePolicy(sizePolicy1);
        horizontalLayout = new QHBoxLayout(GameWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        GameLabel = new QLabel(GameWidget);
        GameLabel->setObjectName(QString::fromUtf8("GameLabel"));
        sizePolicy1.setHeightForWidth(GameLabel->sizePolicy().hasHeightForWidth());
        GameLabel->setSizePolicy(sizePolicy1);
        GameLabel->setMaximumSize(QSize(16777215, 16777215));
        GameLabel->setBaseSize(QSize(10, 50));
        GameLabel->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(GameLabel);

        GameCombo = new QComboBox(GameWidget);
        GameCombo->setObjectName(QString::fromUtf8("GameCombo"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(1);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(GameCombo->sizePolicy().hasHeightForWidth());
        GameCombo->setSizePolicy(sizePolicy2);

        horizontalLayout->addWidget(GameCombo);


        horizontalLayout_4->addWidget(GameWidget);

        DiffWidget = new QWidget(titleWidget);
        DiffWidget->setObjectName(QString::fromUtf8("DiffWidget"));
        sizePolicy1.setHeightForWidth(DiffWidget->sizePolicy().hasHeightForWidth());
        DiffWidget->setSizePolicy(sizePolicy1);
        horizontalLayout_2 = new QHBoxLayout(DiffWidget);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        DiffLabel = new QLabel(DiffWidget);
        DiffLabel->setObjectName(QString::fromUtf8("DiffLabel"));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Maximum);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(DiffLabel->sizePolicy().hasHeightForWidth());
        DiffLabel->setSizePolicy(sizePolicy3);

        horizontalLayout_2->addWidget(DiffLabel);

        DiffCombo = new QComboBox(DiffWidget);
        DiffCombo->setObjectName(QString::fromUtf8("DiffCombo"));
        QSizePolicy sizePolicy4(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy4.setHorizontalStretch(1);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(DiffCombo->sizePolicy().hasHeightForWidth());
        DiffCombo->setSizePolicy(sizePolicy4);

        horizontalLayout_2->addWidget(DiffCombo);


        horizontalLayout_4->addWidget(DiffWidget);

        ShotWidget = new QWidget(titleWidget);
        ShotWidget->setObjectName(QString::fromUtf8("ShotWidget"));
        sizePolicy1.setHeightForWidth(ShotWidget->sizePolicy().hasHeightForWidth());
        ShotWidget->setSizePolicy(sizePolicy1);
        horizontalLayout_3 = new QHBoxLayout(ShotWidget);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        ShotLabel = new QLabel(ShotWidget);
        ShotLabel->setObjectName(QString::fromUtf8("ShotLabel"));
        sizePolicy3.setHeightForWidth(ShotLabel->sizePolicy().hasHeightForWidth());
        ShotLabel->setSizePolicy(sizePolicy3);

        horizontalLayout_3->addWidget(ShotLabel);

        ShotCombo = new QComboBox(ShotWidget);
        ShotCombo->setObjectName(QString::fromUtf8("ShotCombo"));
        sizePolicy4.setHeightForWidth(ShotCombo->sizePolicy().hasHeightForWidth());
        ShotCombo->setSizePolicy(sizePolicy4);

        horizontalLayout_3->addWidget(ShotCombo);


        horizontalLayout_4->addWidget(ShotWidget);


        verticalLayout->addWidget(titleWidget);

        formWidget = new QStackedWidget(EditorWindow);
        formWidget->setObjectName(QString::fromUtf8("formWidget"));
        listPage = new QWidget();
        listPage->setObjectName(QString::fromUtf8("listPage"));
        verticalLayout_3 = new QVBoxLayout(listPage);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        listWidget = new QListWidget(listPage);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));

        verticalLayout_3->addWidget(listWidget);

        formWidget->addWidget(listPage);
        tablePage = new QWidget();
        tablePage->setObjectName(QString::fromUtf8("tablePage"));
        verticalLayout_2 = new QVBoxLayout(tablePage);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        tableWidget = new QTableWidget(tablePage);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
        sizePolicy.setHeightForWidth(tableWidget->sizePolicy().hasHeightForWidth());
        tableWidget->setSizePolicy(sizePolicy);
        tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        tableWidget->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
        tableWidget->setAutoScroll(false);
        tableWidget->setEditTriggers(QAbstractItemView::AllEditTriggers);
        tableWidget->setTextElideMode(Qt::ElideRight);
        tableWidget->verticalHeader()->setVisible(false);

        verticalLayout_2->addWidget(tableWidget);

        formWidget->addWidget(tablePage);

        verticalLayout->addWidget(formWidget);


        retranslateUi(EditorWindow);

        formWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(EditorWindow);
    } // setupUi

    void retranslateUi(QWidget *EditorWindow)
    {
        EditorWindow->setWindowTitle(QCoreApplication::translate("EditorWindow", "EditorWindow", nullptr));
        GameLabel->setText(QCoreApplication::translate("EditorWindow", "Game: ", nullptr));
        DiffLabel->setText(QCoreApplication::translate("EditorWindow", "Difficulty: ", nullptr));
        ShotLabel->setText(QCoreApplication::translate("EditorWindow", "Shottype: ", nullptr));
    } // retranslateUi

};

namespace Ui {
    class EditorWindow: public Ui_EditorWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // EDITORWINDOWPYCGUU_H
