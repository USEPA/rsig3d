/********************************************************************************
** Form generated from reading UI file 'cookies.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COOKIES_H
#define UI_COOKIES_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include "edittableview.h"
#include "searchlineedit.h"

QT_BEGIN_NAMESPACE

class Ui_CookiesDialog
{
public:
    QGridLayout *gridLayout;
    QSpacerItem *spacerItem;
    SearchLineEdit *search;
    EditTableView *cookiesTable;
    QHBoxLayout *hboxLayout;
    QPushButton *removeButton;
    QPushButton *removeAllButton;
    QSpacerItem *spacerItem1;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *CookiesDialog)
    {
        if (CookiesDialog->objectName().isEmpty())
            CookiesDialog->setObjectName(QString::fromUtf8("CookiesDialog"));
        CookiesDialog->resize(550, 370);
        gridLayout = new QGridLayout(CookiesDialog);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        spacerItem = new QSpacerItem(252, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(spacerItem, 0, 0, 1, 1);

        search = new SearchLineEdit(CookiesDialog);
        search->setObjectName(QString::fromUtf8("search"));

        gridLayout->addWidget(search, 0, 1, 1, 1);

        cookiesTable = new EditTableView(CookiesDialog);
        cookiesTable->setObjectName(QString::fromUtf8("cookiesTable"));

        gridLayout->addWidget(cookiesTable, 1, 0, 1, 2);

        hboxLayout = new QHBoxLayout();
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        removeButton = new QPushButton(CookiesDialog);
        removeButton->setObjectName(QString::fromUtf8("removeButton"));

        hboxLayout->addWidget(removeButton);

        removeAllButton = new QPushButton(CookiesDialog);
        removeAllButton->setObjectName(QString::fromUtf8("removeAllButton"));

        hboxLayout->addWidget(removeAllButton);

        spacerItem1 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(spacerItem1);

        buttonBox = new QDialogButtonBox(CookiesDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setStandardButtons(QDialogButtonBox::Ok);

        hboxLayout->addWidget(buttonBox);


        gridLayout->addLayout(hboxLayout, 2, 0, 1, 2);


        retranslateUi(CookiesDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), CookiesDialog, SLOT(accept()));

        QMetaObject::connectSlotsByName(CookiesDialog);
    } // setupUi

    void retranslateUi(QDialog *CookiesDialog)
    {
        CookiesDialog->setWindowTitle(QApplication::translate("CookiesDialog", "Cookies", 0, QApplication::UnicodeUTF8));
        removeButton->setText(QApplication::translate("CookiesDialog", "&Remove", 0, QApplication::UnicodeUTF8));
        removeAllButton->setText(QApplication::translate("CookiesDialog", "Remove &All Cookies", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class CookiesDialog: public Ui_CookiesDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COOKIES_H
