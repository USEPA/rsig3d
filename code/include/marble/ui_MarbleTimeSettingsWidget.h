/********************************************************************************
** Form generated from reading UI file 'MarbleTimeSettingsWidget.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MARBLETIMESETTINGSWIDGET_H
#define UI_MARBLETIMESETTINGSWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QRadioButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MarbleTimeSettingsWidget
{
public:
    QVBoxLayout *verticalLayout_2;
    QGroupBox *timeZone;
    QVBoxLayout *verticalLayout;
    QRadioButton *kcfg_utc;
    QRadioButton *kcfg_systemTimezone;
    QHBoxLayout *hboxLayout;
    QRadioButton *kcfg_customTimezone;
    QComboBox *kcfg_chosenTimezone;
    QGroupBox *whenMarbleStarts;
    QVBoxLayout *verticalLayout_4;
    QRadioButton *kcfg_systemTime;
    QRadioButton *kcfg_lastSessionTime;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *MarbleTimeSettingsWidget)
    {
        if (MarbleTimeSettingsWidget->objectName().isEmpty())
            MarbleTimeSettingsWidget->setObjectName(QString::fromUtf8("MarbleTimeSettingsWidget"));
        MarbleTimeSettingsWidget->resize(485, 305);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MarbleTimeSettingsWidget->sizePolicy().hasHeightForWidth());
        MarbleTimeSettingsWidget->setSizePolicy(sizePolicy);
        verticalLayout_2 = new QVBoxLayout(MarbleTimeSettingsWidget);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        timeZone = new QGroupBox(MarbleTimeSettingsWidget);
        timeZone->setObjectName(QString::fromUtf8("timeZone"));
        verticalLayout = new QVBoxLayout(timeZone);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        kcfg_utc = new QRadioButton(timeZone);
        kcfg_utc->setObjectName(QString::fromUtf8("kcfg_utc"));
        kcfg_utc->setChecked(false);

        verticalLayout->addWidget(kcfg_utc);

        kcfg_systemTimezone = new QRadioButton(timeZone);
        kcfg_systemTimezone->setObjectName(QString::fromUtf8("kcfg_systemTimezone"));
        kcfg_systemTimezone->setChecked(true);

        verticalLayout->addWidget(kcfg_systemTimezone);

        hboxLayout = new QHBoxLayout();
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        kcfg_customTimezone = new QRadioButton(timeZone);
        kcfg_customTimezone->setObjectName(QString::fromUtf8("kcfg_customTimezone"));

        hboxLayout->addWidget(kcfg_customTimezone);

        kcfg_chosenTimezone = new QComboBox(timeZone);
        kcfg_chosenTimezone->setObjectName(QString::fromUtf8("kcfg_chosenTimezone"));
        kcfg_chosenTimezone->setEnabled(false);
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(kcfg_chosenTimezone->sizePolicy().hasHeightForWidth());
        kcfg_chosenTimezone->setSizePolicy(sizePolicy1);

        hboxLayout->addWidget(kcfg_chosenTimezone);


        verticalLayout->addLayout(hboxLayout);


        verticalLayout_2->addWidget(timeZone);

        whenMarbleStarts = new QGroupBox(MarbleTimeSettingsWidget);
        whenMarbleStarts->setObjectName(QString::fromUtf8("whenMarbleStarts"));
        verticalLayout_4 = new QVBoxLayout(whenMarbleStarts);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        kcfg_systemTime = new QRadioButton(whenMarbleStarts);
        kcfg_systemTime->setObjectName(QString::fromUtf8("kcfg_systemTime"));
        kcfg_systemTime->setChecked(true);

        verticalLayout_4->addWidget(kcfg_systemTime);

        kcfg_lastSessionTime = new QRadioButton(whenMarbleStarts);
        kcfg_lastSessionTime->setObjectName(QString::fromUtf8("kcfg_lastSessionTime"));

        verticalLayout_4->addWidget(kcfg_lastSessionTime);


        verticalLayout_2->addWidget(whenMarbleStarts);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);


        retranslateUi(MarbleTimeSettingsWidget);
        QObject::connect(kcfg_customTimezone, SIGNAL(toggled(bool)), kcfg_chosenTimezone, SLOT(setEnabled(bool)));

        QMetaObject::connectSlotsByName(MarbleTimeSettingsWidget);
    } // setupUi

    void retranslateUi(QWidget *MarbleTimeSettingsWidget)
    {
        timeZone->setTitle(QApplication::translate("MarbleTimeSettingsWidget", "Time Zone", 0, QApplication::UnicodeUTF8));
        kcfg_utc->setText(QApplication::translate("MarbleTimeSettingsWidget", "UTC", 0, QApplication::UnicodeUTF8));
        kcfg_systemTimezone->setText(QApplication::translate("MarbleTimeSettingsWidget", "System time zone", 0, QApplication::UnicodeUTF8));
        kcfg_customTimezone->setText(QApplication::translate("MarbleTimeSettingsWidget", "Choose Time zone", 0, QApplication::UnicodeUTF8));
        kcfg_chosenTimezone->clear();
        kcfg_chosenTimezone->insertItems(0, QStringList()
         << QApplication::translate("MarbleTimeSettingsWidget", "Universal Time Coordinated", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MarbleTimeSettingsWidget", "European Central Time", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MarbleTimeSettingsWidget", "Eastern European Time", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MarbleTimeSettingsWidget", "Egypt Standard Time", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MarbleTimeSettingsWidget", "Eastern African Time", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MarbleTimeSettingsWidget", "Middle East Time", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MarbleTimeSettingsWidget", "Near East Time", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MarbleTimeSettingsWidget", "Pakistan Lahore Time", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MarbleTimeSettingsWidget", "India Standard Time", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MarbleTimeSettingsWidget", "Bangladesh Standard Time", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MarbleTimeSettingsWidget", "Vietnam Standard Time", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MarbleTimeSettingsWidget", "China Taiwan Time", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MarbleTimeSettingsWidget", "Japan Standard Time", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MarbleTimeSettingsWidget", "Australia Central Time", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MarbleTimeSettingsWidget", "Australia Eastern Time", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MarbleTimeSettingsWidget", "Solomon Standard Time", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MarbleTimeSettingsWidget", "New Zealand Standard Time", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MarbleTimeSettingsWidget", "Midway Islands Time", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MarbleTimeSettingsWidget", "Hawaii Standard Time", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MarbleTimeSettingsWidget", "Alaska Standard Time", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MarbleTimeSettingsWidget", "Pacific Standard Time", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MarbleTimeSettingsWidget", "Phoenix Standard Time", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MarbleTimeSettingsWidget", "Mountain Standard Time", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MarbleTimeSettingsWidget", "Central Standard Time", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MarbleTimeSettingsWidget", "Eastern Standard Time", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MarbleTimeSettingsWidget", "Indiana Eastern Standard Time", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MarbleTimeSettingsWidget", "Puerto Rico and US Virgin Islands Time", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MarbleTimeSettingsWidget", "Canada Newfoundland Time", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MarbleTimeSettingsWidget", "Argentina Standard Time", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MarbleTimeSettingsWidget", "Brazil Eastern Time", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MarbleTimeSettingsWidget", "Central African Time", 0, QApplication::UnicodeUTF8)
        );
        whenMarbleStarts->setTitle(QApplication::translate("MarbleTimeSettingsWidget", "When Marble starts", 0, QApplication::UnicodeUTF8));
        kcfg_systemTime->setText(QApplication::translate("MarbleTimeSettingsWidget", "Load system time", 0, QApplication::UnicodeUTF8));
        kcfg_lastSessionTime->setText(QApplication::translate("MarbleTimeSettingsWidget", "Load last session time", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(MarbleTimeSettingsWidget);
    } // retranslateUi

};

namespace Ui {
    class MarbleTimeSettingsWidget: public Ui_MarbleTimeSettingsWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MARBLETIMESETTINGSWIDGET_H
