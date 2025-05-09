/********************************************************************************
** Form generated from reading UI file 'portrait.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PORTRAIT_H
#define UI_PORTRAIT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PortraitUI
{
public:
    QGridLayout *gridLayout;
    QLabel *label;
    QPushButton *exitButton;
    QLabel *helpLabel;
    QWidget *choiceWidget;

    void setupUi(QWidget *PortraitUI)
    {
        if (PortraitUI->objectName().isEmpty())
            PortraitUI->setObjectName(QString::fromUtf8("PortraitUI"));
        PortraitUI->resize(201, 300);
        gridLayout = new QGridLayout(PortraitUI);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(PortraitUI);
        label->setObjectName(QString::fromUtf8("label"));
        label->setStyleSheet(QString::fromUtf8("font-weight: bold;"));
        label->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

        gridLayout->addWidget(label, 0, 0, 1, 1);

        exitButton = new QPushButton(PortraitUI);
        exitButton->setObjectName(QString::fromUtf8("exitButton"));

        gridLayout->addWidget(exitButton, 3, 0, 1, 1);

        helpLabel = new QLabel(PortraitUI);
        helpLabel->setObjectName(QString::fromUtf8("helpLabel"));
        helpLabel->setWordWrap(true);

        gridLayout->addWidget(helpLabel, 2, 0, 1, 1);

        choiceWidget = new QWidget(PortraitUI);
        choiceWidget->setObjectName(QString::fromUtf8("choiceWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(choiceWidget->sizePolicy().hasHeightForWidth());
        choiceWidget->setSizePolicy(sizePolicy);

        gridLayout->addWidget(choiceWidget, 1, 0, 1, 1);


        retranslateUi(PortraitUI);

        QMetaObject::connectSlotsByName(PortraitUI);
    } // setupUi

    void retranslateUi(QWidget *PortraitUI)
    {
        PortraitUI->setWindowTitle(QApplication::translate("PortraitUI", "Form", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("PortraitUI", "Portrait mode", 0, QApplication::UnicodeUTF8));
        exitButton->setText(QApplication::translate("PortraitUI", "Exit", 0, QApplication::UnicodeUTF8));
        helpLabel->setText(QApplication::translate("PortraitUI", "Switch to landscape mode. In landscape mode you can change visible image.", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class PortraitUI: public Ui_PortraitUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PORTRAIT_H
