/********************************************************************************
** Form generated from reading UI file 'landscape.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LANDSCAPE_H
#define UI_LANDSCAPE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LandscapeUI
{
public:
    QGridLayout *gridLayout;
    QLabel *label;
    QPushButton *exitButton;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout;
    QRadioButton *radioAButton;
    QRadioButton *radioBButton;
    QRadioButton *radioCButton;
    QWidget *choiceWidget;
    QButtonGroup *buttonGroup;

    void setupUi(QWidget *LandscapeUI)
    {
        if (LandscapeUI->objectName().isEmpty())
            LandscapeUI->setObjectName(QString::fromUtf8("LandscapeUI"));
        LandscapeUI->resize(514, 265);
        gridLayout = new QGridLayout(LandscapeUI);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(LandscapeUI);
        label->setObjectName(QString::fromUtf8("label"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        label->setStyleSheet(QString::fromUtf8("font-weight: bold;"));
        label->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

        gridLayout->addWidget(label, 0, 0, 1, 2);

        exitButton = new QPushButton(LandscapeUI);
        exitButton->setObjectName(QString::fromUtf8("exitButton"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(exitButton->sizePolicy().hasHeightForWidth());
        exitButton->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(exitButton, 0, 2, 1, 1);

        groupBox = new QGroupBox(LandscapeUI);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy2);
        verticalLayout = new QVBoxLayout(groupBox);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        radioAButton = new QRadioButton(groupBox);
        buttonGroup = new QButtonGroup(LandscapeUI);
        buttonGroup->setObjectName(QString::fromUtf8("buttonGroup"));
        buttonGroup->addButton(radioAButton);
        radioAButton->setObjectName(QString::fromUtf8("radioAButton"));
        radioAButton->setChecked(true);

        verticalLayout->addWidget(radioAButton);

        radioBButton = new QRadioButton(groupBox);
        buttonGroup->addButton(radioBButton);
        radioBButton->setObjectName(QString::fromUtf8("radioBButton"));

        verticalLayout->addWidget(radioBButton);

        radioCButton = new QRadioButton(groupBox);
        buttonGroup->addButton(radioCButton);
        radioCButton->setObjectName(QString::fromUtf8("radioCButton"));

        verticalLayout->addWidget(radioCButton);


        gridLayout->addWidget(groupBox, 1, 2, 1, 1);

        choiceWidget = new QWidget(LandscapeUI);
        choiceWidget->setObjectName(QString::fromUtf8("choiceWidget"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(choiceWidget->sizePolicy().hasHeightForWidth());
        choiceWidget->setSizePolicy(sizePolicy3);

        gridLayout->addWidget(choiceWidget, 1, 0, 1, 2);


        retranslateUi(LandscapeUI);

        QMetaObject::connectSlotsByName(LandscapeUI);
    } // setupUi

    void retranslateUi(QWidget *LandscapeUI)
    {
        LandscapeUI->setWindowTitle(QApplication::translate("LandscapeUI", "Form", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("LandscapeUI", "Landscape mode", 0, QApplication::UnicodeUTF8));
        exitButton->setText(QApplication::translate("LandscapeUI", "Exit", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("LandscapeUI", "Choices", 0, QApplication::UnicodeUTF8));
        radioAButton->setText(QApplication::translate("LandscapeUI", "Long description explaining choice A", 0, QApplication::UnicodeUTF8));
        radioBButton->setText(QApplication::translate("LandscapeUI", "Long description explaining choice B", 0, QApplication::UnicodeUTF8));
        radioCButton->setText(QApplication::translate("LandscapeUI", "Long description explaining choice C", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class LandscapeUI: public Ui_LandscapeUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LANDSCAPE_H
