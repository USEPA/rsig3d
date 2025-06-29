/********************************************************************************
** Form generated from reading UI file 'dialog.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOG_H
#define UI_DIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QGridLayout *gridLayout;
    QPushButton *loadFromFileButton;
    QLabel *label;
    QPushButton *loadFromSharedMemoryButton;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QString::fromUtf8("Dialog"));
        Dialog->resize(451, 322);
        gridLayout = new QGridLayout(Dialog);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        loadFromFileButton = new QPushButton(Dialog);
        loadFromFileButton->setObjectName(QString::fromUtf8("loadFromFileButton"));

        gridLayout->addWidget(loadFromFileButton, 0, 0, 1, 1);

        label = new QLabel(Dialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setAlignment(Qt::AlignCenter);
        label->setWordWrap(true);

        gridLayout->addWidget(label, 1, 0, 1, 1);

        loadFromSharedMemoryButton = new QPushButton(Dialog);
        loadFromSharedMemoryButton->setObjectName(QString::fromUtf8("loadFromSharedMemoryButton"));

        gridLayout->addWidget(loadFromSharedMemoryButton, 2, 0, 1, 1);


        retranslateUi(Dialog);

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QApplication::translate("Dialog", "Dialog", 0, QApplication::UnicodeUTF8));
        loadFromFileButton->setText(QApplication::translate("Dialog", "Load Image From File...", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("Dialog", "Launch two of these dialogs.  In the first, press the top button and load an image from a file.  In the second, press the bottom button and display the loaded image from shared memory.", 0, QApplication::UnicodeUTF8));
        loadFromSharedMemoryButton->setText(QApplication::translate("Dialog", "Display Image From Shared Memory", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_H
