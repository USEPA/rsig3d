/********************************************************************************
** Form generated from reading UI file 'TimeControlWidget.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TIMECONTROLWIDGET_H
#define UI_TIMECONTROLWIDGET_H

#include <QtCore/QDate>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDateTimeEdit>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSlider>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_TimeControlWidget
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *time;
    QGridLayout *gridLayout;
    QLabel *newLabel;
    QLabel *refreshIntervalLabel;
    QSpinBox *refreshIntervalSpinBox;
    QToolButton *nowToolButton;
    QDateTimeEdit *newDateTimeEdit;
    QDateTimeEdit *currentDateTimeEdit;
    QLabel *currentLabel;
    QGroupBox *timeSpeed;
    QHBoxLayout *horizontalLayout_5;
    QSlider *speedSlider;
    QLabel *speedLabel;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *okButton;
    QPushButton *applyButton;
    QPushButton *cancelButton;

    void setupUi(QDialog *TimeControlWidget)
    {
        if (TimeControlWidget->objectName().isEmpty())
            TimeControlWidget->setObjectName(QString::fromUtf8("TimeControlWidget"));
        TimeControlWidget->resize(421, 217);
        verticalLayout = new QVBoxLayout(TimeControlWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        time = new QGroupBox(TimeControlWidget);
        time->setObjectName(QString::fromUtf8("time"));
        gridLayout = new QGridLayout(time);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        newLabel = new QLabel(time);
        newLabel->setObjectName(QString::fromUtf8("newLabel"));

        gridLayout->addWidget(newLabel, 1, 0, 1, 1);

        refreshIntervalLabel = new QLabel(time);
        refreshIntervalLabel->setObjectName(QString::fromUtf8("refreshIntervalLabel"));

        gridLayout->addWidget(refreshIntervalLabel, 2, 0, 1, 1);

        refreshIntervalSpinBox = new QSpinBox(time);
        refreshIntervalSpinBox->setObjectName(QString::fromUtf8("refreshIntervalSpinBox"));
        refreshIntervalSpinBox->setMinimum(1);
        refreshIntervalSpinBox->setMaximum(9999);

        gridLayout->addWidget(refreshIntervalSpinBox, 2, 1, 1, 1);

        nowToolButton = new QToolButton(time);
        nowToolButton->setObjectName(QString::fromUtf8("nowToolButton"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(2);
        sizePolicy.setHeightForWidth(nowToolButton->sizePolicy().hasHeightForWidth());
        nowToolButton->setSizePolicy(sizePolicy);

        gridLayout->addWidget(nowToolButton, 1, 2, 1, 1);

        newDateTimeEdit = new QDateTimeEdit(time);
        newDateTimeEdit->setObjectName(QString::fromUtf8("newDateTimeEdit"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(newDateTimeEdit->sizePolicy().hasHeightForWidth());
        newDateTimeEdit->setSizePolicy(sizePolicy1);
        newDateTimeEdit->setDate(QDate(2010, 1, 1));
        newDateTimeEdit->setCalendarPopup(true);

        gridLayout->addWidget(newDateTimeEdit, 1, 1, 1, 1);

        currentDateTimeEdit = new QDateTimeEdit(time);
        currentDateTimeEdit->setObjectName(QString::fromUtf8("currentDateTimeEdit"));
        currentDateTimeEdit->setEnabled(true);
        sizePolicy1.setHeightForWidth(currentDateTimeEdit->sizePolicy().hasHeightForWidth());
        currentDateTimeEdit->setSizePolicy(sizePolicy1);
        currentDateTimeEdit->setFocusPolicy(Qt::NoFocus);
        currentDateTimeEdit->setFrame(true);
        currentDateTimeEdit->setReadOnly(true);
        currentDateTimeEdit->setButtonSymbols(QAbstractSpinBox::NoButtons);
        currentDateTimeEdit->setKeyboardTracking(false);

        gridLayout->addWidget(currentDateTimeEdit, 0, 1, 1, 1);

        currentLabel = new QLabel(time);
        currentLabel->setObjectName(QString::fromUtf8("currentLabel"));
        currentLabel->setEnabled(true);

        gridLayout->addWidget(currentLabel, 0, 0, 1, 1);


        verticalLayout->addWidget(time);

        timeSpeed = new QGroupBox(TimeControlWidget);
        timeSpeed->setObjectName(QString::fromUtf8("timeSpeed"));
        horizontalLayout_5 = new QHBoxLayout(timeSpeed);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        speedSlider = new QSlider(timeSpeed);
        speedSlider->setObjectName(QString::fromUtf8("speedSlider"));
        speedSlider->setMinimum(-100);
        speedSlider->setMaximum(100);
        speedSlider->setSingleStep(1);
        speedSlider->setPageStep(20);
        speedSlider->setValue(1);
        speedSlider->setSliderPosition(1);
        speedSlider->setOrientation(Qt::Horizontal);
        speedSlider->setTickPosition(QSlider::NoTicks);

        horizontalLayout_5->addWidget(speedSlider);

        speedLabel = new QLabel(timeSpeed);
        speedLabel->setObjectName(QString::fromUtf8("speedLabel"));

        horizontalLayout_5->addWidget(speedLabel);


        verticalLayout->addWidget(timeSpeed);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        okButton = new QPushButton(TimeControlWidget);
        okButton->setObjectName(QString::fromUtf8("okButton"));

        horizontalLayout->addWidget(okButton);

        applyButton = new QPushButton(TimeControlWidget);
        applyButton->setObjectName(QString::fromUtf8("applyButton"));

        horizontalLayout->addWidget(applyButton);

        cancelButton = new QPushButton(TimeControlWidget);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));
        cancelButton->setFocusPolicy(Qt::StrongFocus);
        cancelButton->setCheckable(false);

        horizontalLayout->addWidget(cancelButton);


        verticalLayout->addLayout(horizontalLayout);

#ifndef QT_NO_SHORTCUT
        newLabel->setBuddy(newDateTimeEdit);
        refreshIntervalLabel->setBuddy(refreshIntervalSpinBox);
        currentLabel->setBuddy(currentDateTimeEdit);
#endif // QT_NO_SHORTCUT

        retranslateUi(TimeControlWidget);

        QMetaObject::connectSlotsByName(TimeControlWidget);
    } // setupUi

    void retranslateUi(QDialog *TimeControlWidget)
    {
        TimeControlWidget->setWindowTitle(QApplication::translate("TimeControlWidget", "Time Control", 0, QApplication::UnicodeUTF8));
        time->setTitle(QApplication::translate("TimeControlWidget", "Time", 0, QApplication::UnicodeUTF8));
        newLabel->setText(QApplication::translate("TimeControlWidget", "New Date Time      :", 0, QApplication::UnicodeUTF8));
        refreshIntervalLabel->setText(QApplication::translate("TimeControlWidget", "Refresh Interval     :", 0, QApplication::UnicodeUTF8));
        refreshIntervalSpinBox->setSuffix(QApplication::translate("TimeControlWidget", " seconds", 0, QApplication::UnicodeUTF8));
        nowToolButton->setText(QApplication::translate("TimeControlWidget", "Now", 0, QApplication::UnicodeUTF8));
        currentLabel->setText(QApplication::translate("TimeControlWidget", "Current Date Time :", 0, QApplication::UnicodeUTF8));
        timeSpeed->setTitle(QApplication::translate("TimeControlWidget", "Time Speed", 0, QApplication::UnicodeUTF8));
        speedLabel->setText(QString());
        okButton->setText(QApplication::translate("TimeControlWidget", "OK", 0, QApplication::UnicodeUTF8));
        applyButton->setText(QApplication::translate("TimeControlWidget", "Apply", 0, QApplication::UnicodeUTF8));
        cancelButton->setText(QApplication::translate("TimeControlWidget", "Cancel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class TimeControlWidget: public Ui_TimeControlWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TIMECONTROLWIDGET_H
