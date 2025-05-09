/********************************************************************************
** Form generated from reading UI file 'previewer_mobiles.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PREVIEWER_MOBILES_H
#define UI_PREVIEWER_MOBILES_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QPushButton>
#include <QtGui/QTabWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include <QtWebKit/QWebView>

QT_BEGIN_NAMESPACE

class Ui_Form
{
public:
    QHBoxLayout *horizontalLayout_4;
    QTabWidget *tabWidget;
    QWidget *tabHTMLPreview;
    QVBoxLayout *verticalLayout;
    QWebView *webView;
    QWidget *tabHTMLEditor;
    QVBoxLayout *verticalLayout_2;
    QPlainTextEdit *plainTextEdit;
    QHBoxLayout *horizontalLayout;
    QPushButton *clearButton;
    QPushButton *previewButton;

    void setupUi(QWidget *Form)
    {
        if (Form->objectName().isEmpty())
            Form->setObjectName(QString::fromUtf8("Form"));
        Form->resize(259, 177);
        horizontalLayout_4 = new QHBoxLayout(Form);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        tabWidget = new QTabWidget(Form);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabHTMLPreview = new QWidget();
        tabHTMLPreview->setObjectName(QString::fromUtf8("tabHTMLPreview"));
        verticalLayout = new QVBoxLayout(tabHTMLPreview);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        webView = new QWebView(tabHTMLPreview);
        webView->setObjectName(QString::fromUtf8("webView"));
        webView->setUrl(QUrl(QString::fromUtf8("about:blank")));

        verticalLayout->addWidget(webView);

        tabWidget->addTab(tabHTMLPreview, QString());
        tabHTMLEditor = new QWidget();
        tabHTMLEditor->setObjectName(QString::fromUtf8("tabHTMLEditor"));
        verticalLayout_2 = new QVBoxLayout(tabHTMLEditor);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        plainTextEdit = new QPlainTextEdit(tabHTMLEditor);
        plainTextEdit->setObjectName(QString::fromUtf8("plainTextEdit"));

        verticalLayout_2->addWidget(plainTextEdit);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        clearButton = new QPushButton(tabHTMLEditor);
        clearButton->setObjectName(QString::fromUtf8("clearButton"));

        horizontalLayout->addWidget(clearButton);

        previewButton = new QPushButton(tabHTMLEditor);
        previewButton->setObjectName(QString::fromUtf8("previewButton"));

        horizontalLayout->addWidget(previewButton);


        verticalLayout_2->addLayout(horizontalLayout);

        tabWidget->addTab(tabHTMLEditor, QString());

        horizontalLayout_4->addWidget(tabWidget);


        retranslateUi(Form);
        QObject::connect(clearButton, SIGNAL(clicked()), plainTextEdit, SLOT(clear()));

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(Form);
    } // setupUi

    void retranslateUi(QWidget *Form)
    {
        Form->setWindowTitle(QApplication::translate("Form", "Form", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tabHTMLPreview), QApplication::translate("Form", "HTML Preview", 0, QApplication::UnicodeUTF8));
        clearButton->setText(QApplication::translate("Form", "Clear", 0, QApplication::UnicodeUTF8));
        previewButton->setText(QApplication::translate("Form", "Preview", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tabHTMLEditor), QApplication::translate("Form", "HTML Editor", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Form: public Ui_Form {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PREVIEWER_MOBILES_H
