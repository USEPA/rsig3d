/********************************************************************************
** Form generated from reading UI file 'MarbleAboutDialog.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MARBLEABOUTDIALOG_H
#define UI_MARBLEABOUTDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QSpacerItem>
#include <QtGui/QTabWidget>
#include <QtGui/QTextBrowser>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MarbleAboutDialog
{
public:
    QVBoxLayout *vboxLayout;
    QLabel *m_pMarbleLogoLabel;
    QSpacerItem *spacerItem;
    QLabel *m_pMarbleTitleLabel;
    QLabel *m_pMarbleVersionLabel;
    QSpacerItem *spacerItem1;
    QTabWidget *tabWidget;
    QWidget *m_aboutTab;
    QVBoxLayout *vboxLayout1;
    QTextBrowser *m_pMarbleAboutBrowser;
    QWidget *m_authorsTab;
    QVBoxLayout *vboxLayout2;
    QTextBrowser *m_pMarbleAuthorsBrowser;
    QWidget *m_dataTab;
    QVBoxLayout *vboxLayout3;
    QTextBrowser *m_pMarbleDataBrowser;
    QWidget *m_licenseTab;
    QVBoxLayout *vboxLayout4;
    QTextBrowser *m_pMarbleLicenseBrowser;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *MarbleAboutDialog)
    {
        if (MarbleAboutDialog->objectName().isEmpty())
            MarbleAboutDialog->setObjectName(QString::fromUtf8("MarbleAboutDialog"));
        MarbleAboutDialog->resize(466, 402);
        vboxLayout = new QVBoxLayout(MarbleAboutDialog);
        vboxLayout->setSpacing(9);
#ifndef Q_OS_MAC
        vboxLayout->setContentsMargins(9, 9, 9, 9);
#endif
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        m_pMarbleLogoLabel = new QLabel(MarbleAboutDialog);
        m_pMarbleLogoLabel->setObjectName(QString::fromUtf8("m_pMarbleLogoLabel"));

        vboxLayout->addWidget(m_pMarbleLogoLabel);

        spacerItem = new QSpacerItem(20, 5, QSizePolicy::Minimum, QSizePolicy::Fixed);

        vboxLayout->addItem(spacerItem);

        m_pMarbleTitleLabel = new QLabel(MarbleAboutDialog);
        m_pMarbleTitleLabel->setObjectName(QString::fromUtf8("m_pMarbleTitleLabel"));

        vboxLayout->addWidget(m_pMarbleTitleLabel);

        m_pMarbleVersionLabel = new QLabel(MarbleAboutDialog);
        m_pMarbleVersionLabel->setObjectName(QString::fromUtf8("m_pMarbleVersionLabel"));

        vboxLayout->addWidget(m_pMarbleVersionLabel);

        spacerItem1 = new QSpacerItem(20, 5, QSizePolicy::Minimum, QSizePolicy::Fixed);

        vboxLayout->addItem(spacerItem1);

        tabWidget = new QTabWidget(MarbleAboutDialog);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        m_aboutTab = new QWidget();
        m_aboutTab->setObjectName(QString::fromUtf8("m_aboutTab"));
        vboxLayout1 = new QVBoxLayout(m_aboutTab);
#ifndef Q_OS_MAC
        vboxLayout1->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        vboxLayout1->setContentsMargins(9, 9, 9, 9);
#endif
        vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
        m_pMarbleAboutBrowser = new QTextBrowser(m_aboutTab);
        m_pMarbleAboutBrowser->setObjectName(QString::fromUtf8("m_pMarbleAboutBrowser"));
        m_pMarbleAboutBrowser->setOpenExternalLinks(true);

        vboxLayout1->addWidget(m_pMarbleAboutBrowser);

        tabWidget->addTab(m_aboutTab, QString());
        m_authorsTab = new QWidget();
        m_authorsTab->setObjectName(QString::fromUtf8("m_authorsTab"));
        vboxLayout2 = new QVBoxLayout(m_authorsTab);
#ifndef Q_OS_MAC
        vboxLayout2->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        vboxLayout2->setContentsMargins(9, 9, 9, 9);
#endif
        vboxLayout2->setObjectName(QString::fromUtf8("vboxLayout2"));
        m_pMarbleAuthorsBrowser = new QTextBrowser(m_authorsTab);
        m_pMarbleAuthorsBrowser->setObjectName(QString::fromUtf8("m_pMarbleAuthorsBrowser"));
        m_pMarbleAuthorsBrowser->setOpenExternalLinks(true);

        vboxLayout2->addWidget(m_pMarbleAuthorsBrowser);

        tabWidget->addTab(m_authorsTab, QString());
        m_dataTab = new QWidget();
        m_dataTab->setObjectName(QString::fromUtf8("m_dataTab"));
        vboxLayout3 = new QVBoxLayout(m_dataTab);
#ifndef Q_OS_MAC
        vboxLayout3->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        vboxLayout3->setContentsMargins(9, 9, 9, 9);
#endif
        vboxLayout3->setObjectName(QString::fromUtf8("vboxLayout3"));
        m_pMarbleDataBrowser = new QTextBrowser(m_dataTab);
        m_pMarbleDataBrowser->setObjectName(QString::fromUtf8("m_pMarbleDataBrowser"));
        m_pMarbleDataBrowser->setOpenExternalLinks(true);

        vboxLayout3->addWidget(m_pMarbleDataBrowser);

        tabWidget->addTab(m_dataTab, QString());
        m_licenseTab = new QWidget();
        m_licenseTab->setObjectName(QString::fromUtf8("m_licenseTab"));
        vboxLayout4 = new QVBoxLayout(m_licenseTab);
#ifndef Q_OS_MAC
        vboxLayout4->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        vboxLayout4->setContentsMargins(9, 9, 9, 9);
#endif
        vboxLayout4->setObjectName(QString::fromUtf8("vboxLayout4"));
        m_pMarbleLicenseBrowser = new QTextBrowser(m_licenseTab);
        m_pMarbleLicenseBrowser->setObjectName(QString::fromUtf8("m_pMarbleLicenseBrowser"));

        vboxLayout4->addWidget(m_pMarbleLicenseBrowser);

        tabWidget->addTab(m_licenseTab, QString());

        vboxLayout->addWidget(tabWidget);

        buttonBox = new QDialogButtonBox(MarbleAboutDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Close);

        vboxLayout->addWidget(buttonBox);


        retranslateUi(MarbleAboutDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), MarbleAboutDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), MarbleAboutDialog, SLOT(reject()));

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MarbleAboutDialog);
    } // setupUi

    void retranslateUi(QDialog *MarbleAboutDialog)
    {
        MarbleAboutDialog->setWindowTitle(QApplication::translate("MarbleAboutDialog", "About Marble", 0, QApplication::UnicodeUTF8));
        m_pMarbleLogoLabel->setText(QString());
        m_pMarbleTitleLabel->setText(QApplication::translate("MarbleAboutDialog", "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal; text-decoration:none;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600;\">Marble Virtual Globe</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        m_pMarbleVersionLabel->setText(QApplication::translate("MarbleAboutDialog", "Version Unknown", 0, QApplication::UnicodeUTF8));
        m_pMarbleAboutBrowser->setHtml(QApplication::translate("MarbleAboutDialog", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:9pt;\"></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:9pt;\">(c) 2007, The Marble Project</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:9pt; font-style:italic;\"></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span styl"
                        "e=\" font-size:9pt;\">http://edu.kde.org/marble</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(m_aboutTab), QApplication::translate("MarbleAboutDialog", "&About", 0, QApplication::UnicodeUTF8));
        m_pMarbleAuthorsBrowser->setHtml(QApplication::translate("MarbleAboutDialog", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:9pt;\"></p></body></html>", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(m_authorsTab), QApplication::translate("MarbleAboutDialog", "A&uthors", 0, QApplication::UnicodeUTF8));
        m_pMarbleDataBrowser->setHtml(QApplication::translate("MarbleAboutDialog", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:9pt;\"></p></body></html>", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(m_dataTab), QApplication::translate("MarbleAboutDialog", "&Data", 0, QApplication::UnicodeUTF8));
        m_pMarbleLicenseBrowser->setHtml(QApplication::translate("MarbleAboutDialog", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'MS Shell Dlg 2'; font-size:8pt;\"></p></body></html>", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(m_licenseTab), QApplication::translate("MarbleAboutDialog", "&License Agreement", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MarbleAboutDialog: public Ui_MarbleAboutDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MARBLEABOUTDIALOG_H
