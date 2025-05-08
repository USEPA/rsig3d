/********************************************************************************
** Form generated from reading UI file 'SunControlWidget.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SUNCONTROLWIDGET_H
#define UI_SUNCONTROLWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_SunControlWidget
{
public:
    QGridLayout *gridLayout;
    QGroupBox *sunShading;
    QVBoxLayout *verticalLayout;
    QRadioButton *showShadow;
    QRadioButton *showNightMap;
    QGroupBox *sunZenith;
    QVBoxLayout *verticalLayout_2;
    QRadioButton *showZenith;
    QRadioButton *hideZenith;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *okButton;
    QPushButton *applyButton;
    QPushButton *cancelButton;

    void setupUi(QDialog *SunControlWidget)
    {
        if (SunControlWidget->objectName().isEmpty())
            SunControlWidget->setObjectName(QString::fromUtf8("SunControlWidget"));
        SunControlWidget->resize(293, 231);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(SunControlWidget->sizePolicy().hasHeightForWidth());
        SunControlWidget->setSizePolicy(sizePolicy);
        gridLayout = new QGridLayout(SunControlWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        sunShading = new QGroupBox(SunControlWidget);
        sunShading->setObjectName(QString::fromUtf8("sunShading"));
        sunShading->setCheckable(true);
        sunShading->setChecked(false);
        verticalLayout = new QVBoxLayout(sunShading);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        showShadow = new QRadioButton(sunShading);
        showShadow->setObjectName(QString::fromUtf8("showShadow"));
        showShadow->setChecked(true);

        verticalLayout->addWidget(showShadow);

        showNightMap = new QRadioButton(sunShading);
        showNightMap->setObjectName(QString::fromUtf8("showNightMap"));

        verticalLayout->addWidget(showNightMap);


        gridLayout->addWidget(sunShading, 0, 0, 1, 1);

        sunZenith = new QGroupBox(SunControlWidget);
        sunZenith->setObjectName(QString::fromUtf8("sunZenith"));
        sunZenith->setCheckable(false);
        verticalLayout_2 = new QVBoxLayout(sunZenith);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        showZenith = new QRadioButton(sunZenith);
        showZenith->setObjectName(QString::fromUtf8("showZenith"));

        verticalLayout_2->addWidget(showZenith);

        hideZenith = new QRadioButton(sunZenith);
        hideZenith->setObjectName(QString::fromUtf8("hideZenith"));
        hideZenith->setChecked(true);

        verticalLayout_2->addWidget(hideZenith);


        gridLayout->addWidget(sunZenith, 2, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        okButton = new QPushButton(SunControlWidget);
        okButton->setObjectName(QString::fromUtf8("okButton"));

        horizontalLayout->addWidget(okButton);

        applyButton = new QPushButton(SunControlWidget);
        applyButton->setObjectName(QString::fromUtf8("applyButton"));

        horizontalLayout->addWidget(applyButton);

        cancelButton = new QPushButton(SunControlWidget);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));
        cancelButton->setCheckable(false);

        horizontalLayout->addWidget(cancelButton);


        gridLayout->addLayout(horizontalLayout, 3, 0, 1, 1);

        gridLayout->setRowStretch(0, 2);

        retranslateUi(SunControlWidget);

        QMetaObject::connectSlotsByName(SunControlWidget);
    } // setupUi

    void retranslateUi(QDialog *SunControlWidget)
    {
        SunControlWidget->setWindowTitle(QApplication::translate("SunControlWidget", "Sun Control", 0, QApplication::UnicodeUTF8));
        sunShading->setTitle(QApplication::translate("SunControlWidget", "Sun Shading", 0, QApplication::UnicodeUTF8));
        showShadow->setText(QApplication::translate("SunControlWidget", "Shadow", 0, QApplication::UnicodeUTF8));
        showNightMap->setText(QApplication::translate("SunControlWidget", "Night Map", 0, QApplication::UnicodeUTF8));
        sunZenith->setTitle(QApplication::translate("SunControlWidget", "Sun is Zenith at", 0, QApplication::UnicodeUTF8));
        showZenith->setText(QApplication::translate("SunControlWidget", "Show", 0, QApplication::UnicodeUTF8));
        hideZenith->setText(QApplication::translate("SunControlWidget", "Hide", 0, QApplication::UnicodeUTF8));
        okButton->setText(QApplication::translate("SunControlWidget", "OK", 0, QApplication::UnicodeUTF8));
        applyButton->setText(QApplication::translate("SunControlWidget", "Apply", 0, QApplication::UnicodeUTF8));
        cancelButton->setText(QApplication::translate("SunControlWidget", "Cancel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SunControlWidget: public Ui_SunControlWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SUNCONTROLWIDGET_H
