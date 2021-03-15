/********************************************************************************
** Form generated from reading UI file 'UFOWindowlvhwEK.ui'
**
** Created by: Qt User Interface Compiler version 6.0.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UFOWINDOWLVHWEK_H
#define UFOWINDOWLVHWEK_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UFOWindow
{
public:
    QHBoxLayout *horizontalLayout;
    QTableWidget *tableWidget;

    void setupUi(QWidget *UFOWindow)
    {
        if (UFOWindow->objectName().isEmpty())
            UFOWindow->setObjectName(QString::fromUtf8("UFOWindow"));
        UFOWindow->resize(420, 600);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(UFOWindow->sizePolicy().hasHeightForWidth());
        UFOWindow->setSizePolicy(sizePolicy);
        horizontalLayout = new QHBoxLayout(UFOWindow);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        tableWidget = new QTableWidget(UFOWindow);
        if (tableWidget->columnCount() < 4)
            tableWidget->setColumnCount(4);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
        sizePolicy.setHeightForWidth(tableWidget->sizePolicy().hasHeightForWidth());
        tableWidget->setSizePolicy(sizePolicy);
        tableWidget->setMaximumSize(QSize(16777215, 16777215));
        tableWidget->setFocusPolicy(Qt::NoFocus);
        tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        tableWidget->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
        tableWidget->setAutoScroll(false);
        tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableWidget->setSelectionMode(QAbstractItemView::NoSelection);
        tableWidget->setRowCount(0);
        tableWidget->setColumnCount(4);
        tableWidget->verticalHeader()->setVisible(false);

        horizontalLayout->addWidget(tableWidget);


        retranslateUi(UFOWindow);

        QMetaObject::connectSlotsByName(UFOWindow);
    } // setupUi

    void retranslateUi(QWidget *UFOWindow)
    {
        UFOWindow->setWindowTitle(QCoreApplication::translate("UFOWindow", "UFOWindow", nullptr));
    } // retranslateUi

};

namespace Ui {
    class UFOWindow: public Ui_UFOWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UFOWINDOWLVHWEK_H
