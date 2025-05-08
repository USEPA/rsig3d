/********************************************************************************
** Form generated from reading UI file 'PrintOptions.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PRINTOPTIONS_H
#define UI_PRINTOPTIONS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QSpacerItem>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PrintOptions
{
public:
    QGridLayout *gridLayout_3;
    QGroupBox *mapGroupBox;
    QGridLayout *gridLayout;
    QCheckBox *mapCheckBox;
    QSpacerItem *horizontalSpacer;
    QCheckBox *backgroundCheckBox;
    QSpacerItem *horizontalSpacer_2;
    QCheckBox *legendCheckBox;
    QSpacerItem *verticalSpacer;
    QGroupBox *routeGroupBox;
    QGridLayout *gridLayout_2;
    QCheckBox *routeSummaryCheckBox;
    QCheckBox *drivingInstructionsCheckBox;
    QCheckBox *drivingInstructionsAdviceCheckBox;
    QSpacerItem *verticalSpacer_2;

    void setupUi(QWidget *PrintOptions)
    {
        if (PrintOptions->objectName().isEmpty())
            PrintOptions->setObjectName(QString::fromUtf8("PrintOptions"));
        PrintOptions->resize(333, 136);
        gridLayout_3 = new QGridLayout(PrintOptions);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        mapGroupBox = new QGroupBox(PrintOptions);
        mapGroupBox->setObjectName(QString::fromUtf8("mapGroupBox"));
        gridLayout = new QGridLayout(mapGroupBox);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        mapCheckBox = new QCheckBox(mapGroupBox);
        mapCheckBox->setObjectName(QString::fromUtf8("mapCheckBox"));
        mapCheckBox->setChecked(true);

        gridLayout->addWidget(mapCheckBox, 0, 0, 1, 2);

        horizontalSpacer = new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 1, 0, 1, 1);

        backgroundCheckBox = new QCheckBox(mapGroupBox);
        backgroundCheckBox->setObjectName(QString::fromUtf8("backgroundCheckBox"));

        gridLayout->addWidget(backgroundCheckBox, 1, 1, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(0, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 1, 2, 1, 1);

        legendCheckBox = new QCheckBox(mapGroupBox);
        legendCheckBox->setObjectName(QString::fromUtf8("legendCheckBox"));
        legendCheckBox->setChecked(false);

        gridLayout->addWidget(legendCheckBox, 2, 0, 1, 2);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 3, 0, 1, 1);


        gridLayout_3->addWidget(mapGroupBox, 0, 0, 1, 1);

        routeGroupBox = new QGroupBox(PrintOptions);
        routeGroupBox->setObjectName(QString::fromUtf8("routeGroupBox"));
        gridLayout_2 = new QGridLayout(routeGroupBox);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        routeSummaryCheckBox = new QCheckBox(routeGroupBox);
        routeSummaryCheckBox->setObjectName(QString::fromUtf8("routeSummaryCheckBox"));

        gridLayout_2->addWidget(routeSummaryCheckBox, 0, 0, 1, 1);

        drivingInstructionsCheckBox = new QCheckBox(routeGroupBox);
        drivingInstructionsCheckBox->setObjectName(QString::fromUtf8("drivingInstructionsCheckBox"));

        gridLayout_2->addWidget(drivingInstructionsCheckBox, 1, 0, 1, 1);

        drivingInstructionsAdviceCheckBox = new QCheckBox(routeGroupBox);
        drivingInstructionsAdviceCheckBox->setObjectName(QString::fromUtf8("drivingInstructionsAdviceCheckBox"));

        gridLayout_2->addWidget(drivingInstructionsAdviceCheckBox, 2, 0, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer_2, 3, 0, 1, 1);


        gridLayout_3->addWidget(routeGroupBox, 0, 1, 1, 1);


        retranslateUi(PrintOptions);
        QObject::connect(mapCheckBox, SIGNAL(toggled(bool)), backgroundCheckBox, SLOT(setEnabled(bool)));

        QMetaObject::connectSlotsByName(PrintOptions);
    } // setupUi

    void retranslateUi(QWidget *PrintOptions)
    {
        PrintOptions->setWindowTitle(QApplication::translate("PrintOptions", "Marble", 0, QApplication::UnicodeUTF8));
        mapGroupBox->setTitle(QApplication::translate("PrintOptions", "Map", 0, QApplication::UnicodeUTF8));
        mapCheckBox->setText(QApplication::translate("PrintOptions", "Visible Globe Region", 0, QApplication::UnicodeUTF8));
        backgroundCheckBox->setText(QApplication::translate("PrintOptions", "Outer Space", 0, QApplication::UnicodeUTF8));
        legendCheckBox->setText(QApplication::translate("PrintOptions", "Legend", 0, QApplication::UnicodeUTF8));
        routeGroupBox->setTitle(QApplication::translate("PrintOptions", "Route", 0, QApplication::UnicodeUTF8));
        routeSummaryCheckBox->setText(QApplication::translate("PrintOptions", "Route Summary", 0, QApplication::UnicodeUTF8));
        drivingInstructionsCheckBox->setText(QApplication::translate("PrintOptions", "Driving Instructions", 0, QApplication::UnicodeUTF8));
        drivingInstructionsAdviceCheckBox->setText(QApplication::translate("PrintOptions", "Footer", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class PrintOptions: public Ui_PrintOptions {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PRINTOPTIONS_H
