/********************************************************************************
** Form generated from reading UI file 'proxy.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PROXY_H
#define UI_PROXY_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>

QT_BEGIN_NAMESPACE

class Ui_ProxyDialog
{
public:
    QGridLayout *gridLayout;
    QLabel *iconLabel;
    QLabel *introLabel;
    QLabel *usernameLabel;
    QLineEdit *userNameLineEdit;
    QLabel *passwordLabel;
    QLineEdit *passwordLineEdit;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *ProxyDialog)
    {
        if (ProxyDialog->objectName().isEmpty())
            ProxyDialog->setObjectName(QString::fromUtf8("ProxyDialog"));
        ProxyDialog->resize(369, 144);
        gridLayout = new QGridLayout(ProxyDialog);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        iconLabel = new QLabel(ProxyDialog);
        iconLabel->setObjectName(QString::fromUtf8("iconLabel"));

        gridLayout->addWidget(iconLabel, 0, 0, 1, 1);

        introLabel = new QLabel(ProxyDialog);
        introLabel->setObjectName(QString::fromUtf8("introLabel"));
        introLabel->setWordWrap(true);

        gridLayout->addWidget(introLabel, 0, 1, 1, 2);

        usernameLabel = new QLabel(ProxyDialog);
        usernameLabel->setObjectName(QString::fromUtf8("usernameLabel"));

        gridLayout->addWidget(usernameLabel, 1, 0, 1, 2);

        userNameLineEdit = new QLineEdit(ProxyDialog);
        userNameLineEdit->setObjectName(QString::fromUtf8("userNameLineEdit"));

        gridLayout->addWidget(userNameLineEdit, 1, 2, 1, 1);

        passwordLabel = new QLabel(ProxyDialog);
        passwordLabel->setObjectName(QString::fromUtf8("passwordLabel"));

        gridLayout->addWidget(passwordLabel, 2, 0, 1, 2);

        passwordLineEdit = new QLineEdit(ProxyDialog);
        passwordLineEdit->setObjectName(QString::fromUtf8("passwordLineEdit"));
        passwordLineEdit->setEchoMode(QLineEdit::Password);

        gridLayout->addWidget(passwordLineEdit, 2, 2, 1, 1);

        buttonBox = new QDialogButtonBox(ProxyDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        gridLayout->addWidget(buttonBox, 3, 0, 1, 3);


        retranslateUi(ProxyDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), ProxyDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), ProxyDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(ProxyDialog);
    } // setupUi

    void retranslateUi(QDialog *ProxyDialog)
    {
        ProxyDialog->setWindowTitle(QApplication::translate("ProxyDialog", "Proxy Authentication", 0, QApplication::UnicodeUTF8));
        iconLabel->setText(QApplication::translate("ProxyDialog", "ICON", 0, QApplication::UnicodeUTF8));
        introLabel->setText(QApplication::translate("ProxyDialog", "Connect to proxy", 0, QApplication::UnicodeUTF8));
        usernameLabel->setText(QApplication::translate("ProxyDialog", "Username:", 0, QApplication::UnicodeUTF8));
        passwordLabel->setText(QApplication::translate("ProxyDialog", "Password:", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ProxyDialog: public Ui_ProxyDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROXY_H
