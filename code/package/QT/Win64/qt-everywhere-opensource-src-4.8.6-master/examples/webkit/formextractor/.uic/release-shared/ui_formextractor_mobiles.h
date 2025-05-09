/********************************************************************************
** Form generated from reading UI file 'formextractor_mobiles.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FORMEXTRACTOR_MOBILES_H
#define UI_FORMEXTRACTOR_MOBILES_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QSpacerItem>
#include <QtGui/QTabWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include <QtWebKit/QWebView>

QT_BEGIN_NAMESPACE

class Ui_Form
{
public:
    QHBoxLayout *horizontalLayout;
    QTabWidget *tabWidget;
    QWidget *tabWebForm;
    QVBoxLayout *verticalLayout_4;
    QWebView *webView;
    QWidget *tabData;
    QGridLayout *gridLayout;
    QLabel *firstNameLabel;
    QLineEdit *firstNameEdit;
    QLabel *lastNameLabel;
    QLineEdit *lastNameEdit;
    QLabel *genderLabel;
    QLineEdit *genderEdit;
    QLabel *updatesLabel;
    QLineEdit *updatesEdit;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *Form)
    {
        if (Form->objectName().isEmpty())
            Form->setObjectName(QString::fromUtf8("Form"));
        Form->resize(242, 313);
        horizontalLayout = new QHBoxLayout(Form);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        tabWidget = new QTabWidget(Form);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWebForm = new QWidget();
        tabWebForm->setObjectName(QString::fromUtf8("tabWebForm"));
        verticalLayout_4 = new QVBoxLayout(tabWebForm);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        webView = new QWebView(tabWebForm);
        webView->setObjectName(QString::fromUtf8("webView"));
        webView->setMinimumSize(QSize(200, 150));
        webView->setMaximumSize(QSize(400, 16777215));
        webView->setUrl(QUrl(QString::fromUtf8("about:blank")));

        verticalLayout_4->addWidget(webView);

        tabWidget->addTab(tabWebForm, QString());
        tabData = new QWidget();
        tabData->setObjectName(QString::fromUtf8("tabData"));
        gridLayout = new QGridLayout(tabData);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        firstNameLabel = new QLabel(tabData);
        firstNameLabel->setObjectName(QString::fromUtf8("firstNameLabel"));

        gridLayout->addWidget(firstNameLabel, 0, 0, 1, 1);

        firstNameEdit = new QLineEdit(tabData);
        firstNameEdit->setObjectName(QString::fromUtf8("firstNameEdit"));
        firstNameEdit->setReadOnly(true);

        gridLayout->addWidget(firstNameEdit, 0, 1, 1, 1);

        lastNameLabel = new QLabel(tabData);
        lastNameLabel->setObjectName(QString::fromUtf8("lastNameLabel"));

        gridLayout->addWidget(lastNameLabel, 1, 0, 1, 1);

        lastNameEdit = new QLineEdit(tabData);
        lastNameEdit->setObjectName(QString::fromUtf8("lastNameEdit"));
        lastNameEdit->setReadOnly(true);

        gridLayout->addWidget(lastNameEdit, 1, 1, 1, 1);

        genderLabel = new QLabel(tabData);
        genderLabel->setObjectName(QString::fromUtf8("genderLabel"));

        gridLayout->addWidget(genderLabel, 2, 0, 1, 1);

        genderEdit = new QLineEdit(tabData);
        genderEdit->setObjectName(QString::fromUtf8("genderEdit"));
        genderEdit->setReadOnly(true);

        gridLayout->addWidget(genderEdit, 2, 1, 1, 1);

        updatesLabel = new QLabel(tabData);
        updatesLabel->setObjectName(QString::fromUtf8("updatesLabel"));

        gridLayout->addWidget(updatesLabel, 3, 0, 1, 1);

        updatesEdit = new QLineEdit(tabData);
        updatesEdit->setObjectName(QString::fromUtf8("updatesEdit"));
        updatesEdit->setReadOnly(true);

        gridLayout->addWidget(updatesEdit, 3, 1, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 4, 1, 1, 1);

        tabWidget->addTab(tabData, QString());

        horizontalLayout->addWidget(tabWidget);


        retranslateUi(Form);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(Form);
    } // setupUi

    void retranslateUi(QWidget *Form)
    {
        Form->setWindowTitle(QApplication::translate("Form", "Form", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tabWebForm), QApplication::translate("Form", "Web Form", 0, QApplication::UnicodeUTF8));
        firstNameLabel->setText(QApplication::translate("Form", "First Name", 0, QApplication::UnicodeUTF8));
        lastNameLabel->setText(QApplication::translate("Form", "Last Name", 0, QApplication::UnicodeUTF8));
        genderLabel->setText(QApplication::translate("Form", "Gender", 0, QApplication::UnicodeUTF8));
        updatesLabel->setText(QApplication::translate("Form", "Receive Updates", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tabData), QApplication::translate("Form", "Extracted Data", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Form: public Ui_Form {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORMEXTRACTOR_MOBILES_H
