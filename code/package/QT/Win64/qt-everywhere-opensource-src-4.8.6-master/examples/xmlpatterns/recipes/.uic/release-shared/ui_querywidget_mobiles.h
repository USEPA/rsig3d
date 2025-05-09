/********************************************************************************
** Form generated from reading UI file 'querywidget_mobiles.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QUERYWIDGET_MOBILES_H
#define UI_QUERYWIDGET_MOBILES_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QTabWidget>
#include <QtGui/QTextBrowser>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QueryWidget
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout_2;
    QTabWidget *tabWidget;
    QWidget *tab_1;
    QVBoxLayout *verticalLayout;
    QTextBrowser *inputTextEdit;
    QWidget *tab_2;
    QVBoxLayout *verticalLayout_3;
    QComboBox *defaultQueries;
    QTextBrowser *queryTextEdit;
    QWidget *tab_3;
    QVBoxLayout *verticalLayout_4;
    QTextBrowser *outputTextEdit;

    void setupUi(QMainWindow *QueryWidget)
    {
        if (QueryWidget->objectName().isEmpty())
            QueryWidget->setObjectName(QString::fromUtf8("QueryWidget"));
        QueryWidget->resize(453, 583);
        centralwidget = new QWidget(QueryWidget);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout_2 = new QVBoxLayout(centralwidget);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setTabsClosable(false);
        tab_1 = new QWidget();
        tab_1->setObjectName(QString::fromUtf8("tab_1"));
        verticalLayout = new QVBoxLayout(tab_1);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        inputTextEdit = new QTextBrowser(tab_1);
        inputTextEdit->setObjectName(QString::fromUtf8("inputTextEdit"));
        inputTextEdit->setTextInteractionFlags(Qt::NoTextInteraction);
        inputTextEdit->setOpenLinks(false);

        verticalLayout->addWidget(inputTextEdit);

        tabWidget->addTab(tab_1, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        verticalLayout_3 = new QVBoxLayout(tab_2);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        defaultQueries = new QComboBox(tab_2);
        defaultQueries->setObjectName(QString::fromUtf8("defaultQueries"));

        verticalLayout_3->addWidget(defaultQueries);

        queryTextEdit = new QTextBrowser(tab_2);
        queryTextEdit->setObjectName(QString::fromUtf8("queryTextEdit"));
        queryTextEdit->setTextInteractionFlags(Qt::NoTextInteraction);
        queryTextEdit->setOpenLinks(false);

        verticalLayout_3->addWidget(queryTextEdit);

        tabWidget->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        verticalLayout_4 = new QVBoxLayout(tab_3);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        outputTextEdit = new QTextBrowser(tab_3);
        outputTextEdit->setObjectName(QString::fromUtf8("outputTextEdit"));
        outputTextEdit->setTextInteractionFlags(Qt::NoTextInteraction);
        outputTextEdit->setOpenLinks(false);

        verticalLayout_4->addWidget(outputTextEdit);

        tabWidget->addTab(tab_3, QString());

        verticalLayout_2->addWidget(tabWidget);

        QueryWidget->setCentralWidget(centralwidget);

        retranslateUi(QueryWidget);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(QueryWidget);
    } // setupUi

    void retranslateUi(QMainWindow *QueryWidget)
    {
        QueryWidget->setWindowTitle(QApplication::translate("QueryWidget", "Recipes XQuery Example", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_1), QApplication::translate("QueryWidget", "Input Document", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("QueryWidget", "Query selection", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("QueryWidget", "Ouput Document", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class QueryWidget: public Ui_QueryWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QUERYWIDGET_MOBILES_H
