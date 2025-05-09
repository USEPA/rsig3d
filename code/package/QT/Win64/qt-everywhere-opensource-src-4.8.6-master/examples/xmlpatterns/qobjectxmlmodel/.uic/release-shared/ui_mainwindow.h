/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QSplitter>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QTextEdit>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include <QtWebKit/QWebView>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionAbout;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout_4;
    QLabel *label;
    QTabWidget *inheritanceTab;
    QWidget *wholeTreeTab;
    QVBoxLayout *verticalLayout;
    QSplitter *splitter;
    QTextEdit *wholeTree;
    QTextEdit *wholeTreeOutput;
    QWidget *htmlTab;
    QVBoxLayout *verticalLayout_2;
    QSplitter *splitter_2;
    QTextEdit *htmlQueryEdit;
    QWebView *htmlOutput;
    QMenuBar *menubar;
    QMenu *menuHelp;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(800, 600);
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        centralwidget->setGeometry(QRect(0, 29, 800, 549));
        verticalLayout_4 = new QVBoxLayout(centralwidget);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font;
        font.setItalic(true);
        label->setFont(font);

        verticalLayout_4->addWidget(label);

        inheritanceTab = new QTabWidget(centralwidget);
        inheritanceTab->setObjectName(QString::fromUtf8("inheritanceTab"));
        inheritanceTab->setAutoFillBackground(true);
        wholeTreeTab = new QWidget();
        wholeTreeTab->setObjectName(QString::fromUtf8("wholeTreeTab"));
        wholeTreeTab->setGeometry(QRect(0, 0, 778, 475));
        verticalLayout = new QVBoxLayout(wholeTreeTab);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        splitter = new QSplitter(wholeTreeTab);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setOrientation(Qt::Vertical);
        wholeTree = new QTextEdit(splitter);
        wholeTree->setObjectName(QString::fromUtf8("wholeTree"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(wholeTree->sizePolicy().hasHeightForWidth());
        wholeTree->setSizePolicy(sizePolicy);
        wholeTree->setReadOnly(true);
        wholeTree->setAcceptRichText(false);
        splitter->addWidget(wholeTree);
        wholeTreeOutput = new QTextEdit(splitter);
        wholeTreeOutput->setObjectName(QString::fromUtf8("wholeTreeOutput"));
        wholeTreeOutput->setReadOnly(true);
        wholeTreeOutput->setAcceptRichText(false);
        splitter->addWidget(wholeTreeOutput);

        verticalLayout->addWidget(splitter);

        inheritanceTab->addTab(wholeTreeTab, QString());
        htmlTab = new QWidget();
        htmlTab->setObjectName(QString::fromUtf8("htmlTab"));
        htmlTab->setGeometry(QRect(0, 0, 778, 475));
        verticalLayout_2 = new QVBoxLayout(htmlTab);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        splitter_2 = new QSplitter(htmlTab);
        splitter_2->setObjectName(QString::fromUtf8("splitter_2"));
        splitter_2->setOrientation(Qt::Vertical);
        htmlQueryEdit = new QTextEdit(splitter_2);
        htmlQueryEdit->setObjectName(QString::fromUtf8("htmlQueryEdit"));
        htmlQueryEdit->setReadOnly(true);
        htmlQueryEdit->setAcceptRichText(false);
        splitter_2->addWidget(htmlQueryEdit);
        htmlOutput = new QWebView(splitter_2);
        htmlOutput->setObjectName(QString::fromUtf8("htmlOutput"));
        htmlOutput->setUrl(QUrl(QString::fromUtf8("about:blank")));
        splitter_2->addWidget(htmlOutput);

        verticalLayout_2->addWidget(splitter_2);

        inheritanceTab->addTab(htmlTab, QString());

        verticalLayout_4->addWidget(inheritanceTab);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 29));
        menuHelp = new QMenu(menubar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        statusbar->setGeometry(QRect(0, 578, 800, 22));
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuHelp->menuAction());
        menuHelp->addAction(actionAbout);

        retranslateUi(MainWindow);

        inheritanceTab->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "QObjectXmlModel Example", 0, QApplication::UnicodeUTF8));
        actionAbout->setText(QApplication::translate("MainWindow", "About", 0, QApplication::UnicodeUTF8));
        actionAbout->setShortcut(QApplication::translate("MainWindow", "Ctrl+B", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindow", "See the About menu entry for a description of this example.", 0, QApplication::UnicodeUTF8));
        wholeTree->setHtml(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"></p></body></html>", 0, QApplication::UnicodeUTF8));
        wholeTreeOutput->setHtml(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"></p></body></html>", 0, QApplication::UnicodeUTF8));
        inheritanceTab->setTabText(inheritanceTab->indexOf(wholeTreeTab), QApplication::translate("MainWindow", "Whole QObjectTree", 0, QApplication::UnicodeUTF8));
        htmlQueryEdit->setHtml(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"></p></body></html>", 0, QApplication::UnicodeUTF8));
        inheritanceTab->setTabText(inheritanceTab->indexOf(htmlTab), QApplication::translate("MainWindow", "Statistics in HTML ", 0, QApplication::UnicodeUTF8));
        menuHelp->setTitle(QApplication::translate("MainWindow", "Help", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
