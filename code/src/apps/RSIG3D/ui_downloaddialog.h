/********************************************************************************
** Form generated from reading UI file 'downloaddialog.ui'
**
** Created: Thu Jul 23 13:53:07 2026
**      by: Qt User Interface Compiler version 4.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DOWNLOADDIALOG_H
#define UI_DOWNLOADDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTextBrowser>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_DownloadDialog
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QToolButton *showlogToolButton;
    QLabel *sourceTitleLabel;
    QLabel *sourceLabel;
    QLabel *dataTitleLabel;
    QCheckBox *closeonfinishCheckBox;
    QLabel *dataLabel;
    QTextBrowser *logTextBrowser;
    QHBoxLayout *horizontalLayout;
    QLabel *progressLabel;
    QProgressBar *progressBar;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *okPushButton;
    QPushButton *cancelPushButton;
    QSpacerItem *verticalSpacer;

    void setupUi(QDialog *DownloadDialog)
    {
        if (DownloadDialog->objectName().isEmpty())
            DownloadDialog->setObjectName(QString::fromUtf8("DownloadDialog"));
        DownloadDialog->resize(480, 453);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(DownloadDialog->sizePolicy().hasHeightForWidth());
        DownloadDialog->setSizePolicy(sizePolicy);
        DownloadDialog->setMinimumSize(QSize(480, 420));
        DownloadDialog->setMaximumSize(QSize(480, 4200));
        gridLayout = new QGridLayout(DownloadDialog);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        groupBox = new QGroupBox(DownloadDialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        sizePolicy.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy);
        groupBox->setMinimumSize(QSize(459, 110));
        groupBox->setMaximumSize(QSize(459, 110));
        showlogToolButton = new QToolButton(groupBox);
        showlogToolButton->setObjectName(QString::fromUtf8("showlogToolButton"));
        showlogToolButton->setGeometry(QRect(370, 72, 72, 22));
        QSizePolicy sizePolicy1(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(showlogToolButton->sizePolicy().hasHeightForWidth());
        showlogToolButton->setSizePolicy(sizePolicy1);
        sourceTitleLabel = new QLabel(groupBox);
        sourceTitleLabel->setObjectName(QString::fromUtf8("sourceTitleLabel"));
        sourceTitleLabel->setGeometry(QRect(10, 12, 81, 16));
        sizePolicy1.setHeightForWidth(sourceTitleLabel->sizePolicy().hasHeightForWidth());
        sourceTitleLabel->setSizePolicy(sizePolicy1);
        sourceTitleLabel->setMinimumSize(QSize(60, 0));
        sourceTitleLabel->setTextFormat(Qt::RichText);
        sourceLabel = new QLabel(groupBox);
        sourceLabel->setObjectName(QString::fromUtf8("sourceLabel"));
        sourceLabel->setGeometry(QRect(70, 12, 380, 16));
        dataTitleLabel = new QLabel(groupBox);
        dataTitleLabel->setObjectName(QString::fromUtf8("dataTitleLabel"));
        dataTitleLabel->setGeometry(QRect(10, 40, 61, 16));
        sizePolicy1.setHeightForWidth(dataTitleLabel->sizePolicy().hasHeightForWidth());
        dataTitleLabel->setSizePolicy(sizePolicy1);
        dataTitleLabel->setMinimumSize(QSize(60, 0));
        closeonfinishCheckBox = new QCheckBox(groupBox);
        closeonfinishCheckBox->setObjectName(QString::fromUtf8("closeonfinishCheckBox"));
        closeonfinishCheckBox->setGeometry(QRect(10, 70, 151, 20));
        sizePolicy1.setHeightForWidth(closeonfinishCheckBox->sizePolicy().hasHeightForWidth());
        closeonfinishCheckBox->setSizePolicy(sizePolicy1);
        dataLabel = new QLabel(groupBox);
        dataLabel->setObjectName(QString::fromUtf8("dataLabel"));
        dataLabel->setGeometry(QRect(70, 40, 380, 16));

        verticalLayout->addWidget(groupBox);

        logTextBrowser = new QTextBrowser(DownloadDialog);
        logTextBrowser->setObjectName(QString::fromUtf8("logTextBrowser"));
        logTextBrowser->setMinimumSize(QSize(0, 220));
        logTextBrowser->setMaximumSize(QSize(16777215, 220));

        verticalLayout->addWidget(logTextBrowser);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        progressLabel = new QLabel(DownloadDialog);
        progressLabel->setObjectName(QString::fromUtf8("progressLabel"));
        progressLabel->setTextFormat(Qt::PlainText);

        horizontalLayout->addWidget(progressLabel);

        progressBar = new QProgressBar(DownloadDialog);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        sizePolicy1.setHeightForWidth(progressBar->sizePolicy().hasHeightForWidth());
        progressBar->setSizePolicy(sizePolicy1);
        progressBar->setMinimumSize(QSize(0, 18));
        progressBar->setMaximumSize(QSize(16777215, 18));
        progressBar->setValue(0);

        horizontalLayout->addWidget(progressBar);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        okPushButton = new QPushButton(DownloadDialog);
        okPushButton->setObjectName(QString::fromUtf8("okPushButton"));
        sizePolicy1.setHeightForWidth(okPushButton->sizePolicy().hasHeightForWidth());
        okPushButton->setSizePolicy(sizePolicy1);
        okPushButton->setMinimumSize(QSize(0, 24));
        okPushButton->setMaximumSize(QSize(16777215, 24));
        okPushButton->setDefault(true);

        horizontalLayout_2->addWidget(okPushButton);

        cancelPushButton = new QPushButton(DownloadDialog);
        cancelPushButton->setObjectName(QString::fromUtf8("cancelPushButton"));
        sizePolicy1.setHeightForWidth(cancelPushButton->sizePolicy().hasHeightForWidth());
        cancelPushButton->setSizePolicy(sizePolicy1);
        cancelPushButton->setMinimumSize(QSize(0, 24));
        cancelPushButton->setMaximumSize(QSize(16777215, 24));

        horizontalLayout_2->addWidget(cancelPushButton);


        verticalLayout->addLayout(horizontalLayout_2);

        verticalSpacer = new QSpacerItem(20, 18, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);


        retranslateUi(DownloadDialog);

        QMetaObject::connectSlotsByName(DownloadDialog);
    } // setupUi

    void retranslateUi(QDialog *DownloadDialog)
    {
        DownloadDialog->setWindowTitle(QApplication::translate("DownloadDialog", "Downloading ......", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QString());
        showlogToolButton->setText(QApplication::translate("DownloadDialog", "show log", 0, QApplication::UnicodeUTF8));
        sourceTitleLabel->setText(QApplication::translate("DownloadDialog", "<b>Source:</b>", 0, QApplication::UnicodeUTF8));
        sourceLabel->setText(QString());
        dataTitleLabel->setText(QApplication::translate("DownloadDialog", "<b>Data:</b>", 0, QApplication::UnicodeUTF8));
        closeonfinishCheckBox->setText(QApplication::translate("DownloadDialog", "Close on finish", 0, QApplication::UnicodeUTF8));
        dataLabel->setText(QApplication::translate("DownloadDialog", "<html><head/><body><p><br/></p></body></html>", 0, QApplication::UnicodeUTF8));
        progressLabel->setText(QApplication::translate("DownloadDialog", "Progress:", 0, QApplication::UnicodeUTF8));
        progressBar->setFormat(QApplication::translate("DownloadDialog", "%p%", 0, QApplication::UnicodeUTF8));
        okPushButton->setText(QApplication::translate("DownloadDialog", "OK", 0, QApplication::UnicodeUTF8));
        cancelPushButton->setText(QApplication::translate("DownloadDialog", "Cancel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class DownloadDialog: public Ui_DownloadDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DOWNLOADDIALOG_H
