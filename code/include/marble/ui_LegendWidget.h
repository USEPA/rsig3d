/********************************************************************************
** Form generated from reading UI file 'LegendWidget.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LEGENDWIDGET_H
#define UI_LEGENDWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "MarbleLegendBrowser.h"

QT_BEGIN_NAMESPACE

class Ui_LegendWidget
{
public:
    QVBoxLayout *verticalLayout;
    MarbleLegendBrowser *marbleLegendBrowser;

    void setupUi(QWidget *LegendWidget)
    {
        if (LegendWidget->objectName().isEmpty())
            LegendWidget->setObjectName(QString::fromUtf8("LegendWidget"));
        LegendWidget->resize(187, 332);
        verticalLayout = new QVBoxLayout(LegendWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        marbleLegendBrowser = new MarbleLegendBrowser(LegendWidget);
        marbleLegendBrowser->setObjectName(QString::fromUtf8("marbleLegendBrowser"));
        marbleLegendBrowser->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        verticalLayout->addWidget(marbleLegendBrowser);


        retranslateUi(LegendWidget);

        QMetaObject::connectSlotsByName(LegendWidget);
    } // setupUi

    void retranslateUi(QWidget *LegendWidget)
    {
        LegendWidget->setWindowTitle(QApplication::translate("LegendWidget", "Legend", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class LegendWidget: public Ui_LegendWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LEGENDWIDGET_H
