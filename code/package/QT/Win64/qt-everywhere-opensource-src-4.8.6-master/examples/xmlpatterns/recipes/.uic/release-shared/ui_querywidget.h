/********************************************************************************
** Form generated from reading UI file 'querywidget.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QUERYWIDGET_H
#define UI_QUERYWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QTextEdit>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QueryWidget
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QVBoxLayout *vboxLayout;
    QGroupBox *inputGroupBox;
    QVBoxLayout *verticalLayout_4;
    QVBoxLayout *_2;
    QTextEdit *inputTextEdit;
    QGroupBox *queryGroupBox;
    QVBoxLayout *verticalLayout_5;
    QComboBox *defaultQueries;
    QTextEdit *queryTextEdit;
    QGroupBox *outputGroupBox;
    QVBoxLayout *verticalLayout_6;
    QVBoxLayout *_3;
    QTextEdit *outputTextEdit;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *QueryWidget)
    {
        if (QueryWidget->objectName().isEmpty())
            QueryWidget->setObjectName(QString::fromUtf8("QueryWidget"));
        QueryWidget->resize(570, 531);
        centralwidget = new QWidget(QueryWidget);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        vboxLayout = new QVBoxLayout();
#ifndef Q_OS_MAC
        vboxLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        vboxLayout->setContentsMargins(0, 0, 0, 0);
#endif
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        inputGroupBox = new QGroupBox(centralwidget);
        inputGroupBox->setObjectName(QString::fromUtf8("inputGroupBox"));
        inputGroupBox->setMinimumSize(QSize(550, 120));
        verticalLayout_4 = new QVBoxLayout(inputGroupBox);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        _2 = new QVBoxLayout();
#ifndef Q_OS_MAC
        _2->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        _2->setContentsMargins(0, 0, 0, 0);
#endif
        _2->setObjectName(QString::fromUtf8("_2"));
        inputTextEdit = new QTextEdit(inputGroupBox);
        inputTextEdit->setObjectName(QString::fromUtf8("inputTextEdit"));
        inputTextEdit->setReadOnly(true);

        _2->addWidget(inputTextEdit);


        verticalLayout_4->addLayout(_2);


        vboxLayout->addWidget(inputGroupBox);

        queryGroupBox = new QGroupBox(centralwidget);
        queryGroupBox->setObjectName(QString::fromUtf8("queryGroupBox"));
        queryGroupBox->setMinimumSize(QSize(550, 120));
        verticalLayout_5 = new QVBoxLayout(queryGroupBox);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        defaultQueries = new QComboBox(queryGroupBox);
        defaultQueries->setObjectName(QString::fromUtf8("defaultQueries"));

        verticalLayout_5->addWidget(defaultQueries);

        queryTextEdit = new QTextEdit(queryGroupBox);
        queryTextEdit->setObjectName(QString::fromUtf8("queryTextEdit"));
        queryTextEdit->setMinimumSize(QSize(400, 60));
        queryTextEdit->setReadOnly(true);
        queryTextEdit->setAcceptRichText(false);

        verticalLayout_5->addWidget(queryTextEdit);


        vboxLayout->addWidget(queryGroupBox);

        outputGroupBox = new QGroupBox(centralwidget);
        outputGroupBox->setObjectName(QString::fromUtf8("outputGroupBox"));
        outputGroupBox->setMinimumSize(QSize(550, 120));
        verticalLayout_6 = new QVBoxLayout(outputGroupBox);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        _3 = new QVBoxLayout();
#ifndef Q_OS_MAC
        _3->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        _3->setContentsMargins(0, 0, 0, 0);
#endif
        _3->setObjectName(QString::fromUtf8("_3"));
        outputTextEdit = new QTextEdit(outputGroupBox);
        outputTextEdit->setObjectName(QString::fromUtf8("outputTextEdit"));
        outputTextEdit->setMinimumSize(QSize(500, 80));
        outputTextEdit->setReadOnly(true);
        outputTextEdit->setAcceptRichText(false);

        _3->addWidget(outputTextEdit);


        verticalLayout_6->addLayout(_3);


        vboxLayout->addWidget(outputGroupBox);


        verticalLayout->addLayout(vboxLayout);

        QueryWidget->setCentralWidget(centralwidget);
        menubar = new QMenuBar(QueryWidget);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 570, 26));
        QueryWidget->setMenuBar(menubar);
        statusbar = new QStatusBar(QueryWidget);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        QueryWidget->setStatusBar(statusbar);

        retranslateUi(QueryWidget);

        QMetaObject::connectSlotsByName(QueryWidget);
    } // setupUi

    void retranslateUi(QMainWindow *QueryWidget)
    {
        QueryWidget->setWindowTitle(QApplication::translate("QueryWidget", "Recipes XQuery Example", 0, QApplication::UnicodeUTF8));
        inputGroupBox->setTitle(QApplication::translate("QueryWidget", "Input Document", 0, QApplication::UnicodeUTF8));
        queryGroupBox->setTitle(QApplication::translate("QueryWidget", "Select your query:", 0, QApplication::UnicodeUTF8));
        outputGroupBox->setTitle(QApplication::translate("QueryWidget", "Output Document", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class QueryWidget: public Ui_QueryWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QUERYWIDGET_H
