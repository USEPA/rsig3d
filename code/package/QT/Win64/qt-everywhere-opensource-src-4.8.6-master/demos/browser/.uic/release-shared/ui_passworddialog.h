/********************************************************************************
** Form generated from reading UI file 'passworddialog.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PASSWORDDIALOG_H
#define UI_PASSWORDDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>

QT_BEGIN_NAMESPACE

class Ui_PasswordDialog
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *hboxLayout;
    QLabel *iconLabel;
    QLabel *introLabel;
    QLabel *label;
    QLineEdit *userNameLineEdit;
    QLabel *lblPassword;
    QLineEdit *passwordLineEdit;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *PasswordDialog)
    {
        if (PasswordDialog->objectName().isEmpty())
            PasswordDialog->setObjectName(QString::fromUtf8("PasswordDialog"));
        PasswordDialog->resize(399, 148);
        gridLayout = new QGridLayout(PasswordDialog);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        hboxLayout = new QHBoxLayout();
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        iconLabel = new QLabel(PasswordDialog);
        iconLabel->setObjectName(QString::fromUtf8("iconLabel"));

        hboxLayout->addWidget(iconLabel);

        introLabel = new QLabel(PasswordDialog);
        introLabel->setObjectName(QString::fromUtf8("introLabel"));
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(introLabel->sizePolicy().hasHeightForWidth());
        introLabel->setSizePolicy(sizePolicy);

        hboxLayout->addWidget(introLabel);


        gridLayout->addLayout(hboxLayout, 0, 0, 1, 2);

        label = new QLabel(PasswordDialog);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 1, 0, 1, 1);

        userNameLineEdit = new QLineEdit(PasswordDialog);
        userNameLineEdit->setObjectName(QString::fromUtf8("userNameLineEdit"));

        gridLayout->addWidget(userNameLineEdit, 1, 1, 1, 1);

        lblPassword = new QLabel(PasswordDialog);
        lblPassword->setObjectName(QString::fromUtf8("lblPassword"));

        gridLayout->addWidget(lblPassword, 2, 0, 1, 1);

        passwordLineEdit = new QLineEdit(PasswordDialog);
        passwordLineEdit->setObjectName(QString::fromUtf8("passwordLineEdit"));
        passwordLineEdit->setEchoMode(QLineEdit::Password);

        gridLayout->addWidget(passwordLineEdit, 2, 1, 1, 1);

        buttonBox = new QDialogButtonBox(PasswordDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        gridLayout->addWidget(buttonBox, 3, 0, 1, 2);


        retranslateUi(PasswordDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), PasswordDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), PasswordDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(PasswordDialog);
    } // setupUi

    void retranslateUi(QDialog *PasswordDialog)
    {
        PasswordDialog->setWindowTitle(QApplication::translate("PasswordDialog", "Authentication Required", 0, QApplication::UnicodeUTF8));
        iconLabel->setText(QApplication::translate("PasswordDialog", "DUMMY ICON", 0, QApplication::UnicodeUTF8));
        introLabel->setText(QApplication::translate("PasswordDialog", "INTRO TEXT DUMMY", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("PasswordDialog", "Username:", 0, QApplication::UnicodeUTF8));
        lblPassword->setText(QApplication::translate("PasswordDialog", "Password:", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class PasswordDialog: public Ui_PasswordDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PASSWORDDIALOG_H
