/********************************************************************************
** Form generated from reading UI file 'downloaditem.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DOWNLOADITEM_H
#define UI_DOWNLOADITEM_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "squeezelabel.h"

QT_BEGIN_NAMESPACE

class Ui_DownloadItem
{
public:
    QHBoxLayout *horizontalLayout;
    QLabel *fileIcon;
    QVBoxLayout *verticalLayout_2;
    SqueezeLabel *fileNameLabel;
    QProgressBar *progressBar;
    SqueezeLabel *downloadInfoLabel;
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer;
    QPushButton *tryAgainButton;
    QPushButton *stopButton;
    QPushButton *openButton;
    QSpacerItem *verticalSpacer_2;

    void setupUi(QWidget *DownloadItem)
    {
        if (DownloadItem->objectName().isEmpty())
            DownloadItem->setObjectName(QString::fromUtf8("DownloadItem"));
        DownloadItem->resize(423, 110);
        horizontalLayout = new QHBoxLayout(DownloadItem);
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        fileIcon = new QLabel(DownloadItem);
        fileIcon->setObjectName(QString::fromUtf8("fileIcon"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(fileIcon->sizePolicy().hasHeightForWidth());
        fileIcon->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(fileIcon);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        fileNameLabel = new SqueezeLabel(DownloadItem);
        fileNameLabel->setObjectName(QString::fromUtf8("fileNameLabel"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(fileNameLabel->sizePolicy().hasHeightForWidth());
        fileNameLabel->setSizePolicy(sizePolicy1);

        verticalLayout_2->addWidget(fileNameLabel);

        progressBar = new QProgressBar(DownloadItem);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setValue(0);

        verticalLayout_2->addWidget(progressBar);

        downloadInfoLabel = new SqueezeLabel(DownloadItem);
        downloadInfoLabel->setObjectName(QString::fromUtf8("downloadInfoLabel"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(downloadInfoLabel->sizePolicy().hasHeightForWidth());
        downloadInfoLabel->setSizePolicy(sizePolicy2);

        verticalLayout_2->addWidget(downloadInfoLabel);


        horizontalLayout->addLayout(verticalLayout_2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalSpacer = new QSpacerItem(17, 1, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        tryAgainButton = new QPushButton(DownloadItem);
        tryAgainButton->setObjectName(QString::fromUtf8("tryAgainButton"));
        tryAgainButton->setEnabled(false);

        verticalLayout->addWidget(tryAgainButton);

        stopButton = new QPushButton(DownloadItem);
        stopButton->setObjectName(QString::fromUtf8("stopButton"));

        verticalLayout->addWidget(stopButton);

        openButton = new QPushButton(DownloadItem);
        openButton->setObjectName(QString::fromUtf8("openButton"));

        verticalLayout->addWidget(openButton);

        verticalSpacer_2 = new QSpacerItem(17, 5, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);


        horizontalLayout->addLayout(verticalLayout);


        retranslateUi(DownloadItem);

        QMetaObject::connectSlotsByName(DownloadItem);
    } // setupUi

    void retranslateUi(QWidget *DownloadItem)
    {
        DownloadItem->setWindowTitle(QApplication::translate("DownloadItem", "Form", 0, QApplication::UnicodeUTF8));
        fileIcon->setText(QApplication::translate("DownloadItem", "Ico", 0, QApplication::UnicodeUTF8));
        fileNameLabel->setProperty("text", QVariant(QApplication::translate("DownloadItem", "Filename", 0, QApplication::UnicodeUTF8)));
        downloadInfoLabel->setProperty("text", QVariant(QString()));
        tryAgainButton->setText(QApplication::translate("DownloadItem", "Try Again", 0, QApplication::UnicodeUTF8));
        stopButton->setText(QApplication::translate("DownloadItem", "Stop", 0, QApplication::UnicodeUTF8));
        openButton->setText(QApplication::translate("DownloadItem", "Open", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class DownloadItem: public Ui_DownloadItem {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DOWNLOADITEM_H
