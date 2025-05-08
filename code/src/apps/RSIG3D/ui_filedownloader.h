/********************************************************************************
** Form generated from reading UI file 'filedownloader.ui'
**
** Created: Thu May 8 12:58:25 2025
**      by: Qt User Interface Compiler version 4.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FILEDOWNLOADER_H
#define UI_FILEDOWNLOADER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLCDNumber>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_FileOrientedDownloader
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QToolButton *fileDownloaderDirButton;
    QLineEdit *fileDownloaderDirLineEdit;
    QLabel *label;
    QLabel *label_2;
    QLCDNumber *fileDownloaderAvailableSpace;
    QLabel *label_4;
    QLCDNumber *fileDownloaderRequiredSpace;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_3;
    QListWidget *fileDownloaderListWidget;
    QGroupBox *groupBox_4;
    QVBoxLayout *verticalLayout_3;
    QLabel *fileDownloaderStatus;
    QProgressBar *fileDownloaderProgressBar;
    QGroupBox *groupBox_3;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *fileDownloaderDownloadButton;
    QPushButton *fileDownloaderCancelButton;
    QSpacerItem *horizontalSpacer_2;

    void setupUi(QDialog *FileOrientedDownloader)
    {
        if (FileOrientedDownloader->objectName().isEmpty())
            FileOrientedDownloader->setObjectName(QString::fromUtf8("FileOrientedDownloader"));
        FileOrientedDownloader->resize(556, 697);
        verticalLayout = new QVBoxLayout(FileOrientedDownloader);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        groupBox = new QGroupBox(FileOrientedDownloader);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy);
        groupBox->setMinimumSize(QSize(0, 105));
        fileDownloaderDirButton = new QToolButton(groupBox);
        fileDownloaderDirButton->setObjectName(QString::fromUtf8("fileDownloaderDirButton"));
        fileDownloaderDirButton->setGeometry(QRect(10, 30, 72, 22));
        fileDownloaderDirButton->setMinimumSize(QSize(72, 22));
        fileDownloaderDirButton->setMaximumSize(QSize(72, 20));
        fileDownloaderDirLineEdit = new QLineEdit(groupBox);
        fileDownloaderDirLineEdit->setObjectName(QString::fromUtf8("fileDownloaderDirLineEdit"));
        fileDownloaderDirLineEdit->setGeometry(QRect(90, 30, 441, 22));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(fileDownloaderDirLineEdit->sizePolicy().hasHeightForWidth());
        fileDownloaderDirLineEdit->setSizePolicy(sizePolicy1);
        fileDownloaderDirLineEdit->setMinimumSize(QSize(300, 22));
        fileDownloaderDirLineEdit->setMaximumSize(QSize(1024, 22));
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 10, 371, 16));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 60, 171, 21));
        fileDownloaderAvailableSpace = new QLCDNumber(groupBox);
        fileDownloaderAvailableSpace->setObjectName(QString::fromUtf8("fileDownloaderAvailableSpace"));
        fileDownloaderAvailableSpace->setGeometry(QRect(160, 56, 64, 24));
        fileDownloaderAvailableSpace->setFrameShape(QFrame::NoFrame);
        fileDownloaderAvailableSpace->setFrameShadow(QFrame::Plain);
        fileDownloaderAvailableSpace->setNumDigits(4);
        fileDownloaderAvailableSpace->setSegmentStyle(QLCDNumber::Flat);
        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(10, 80, 171, 21));
        fileDownloaderRequiredSpace = new QLCDNumber(groupBox);
        fileDownloaderRequiredSpace->setObjectName(QString::fromUtf8("fileDownloaderRequiredSpace"));
        fileDownloaderRequiredSpace->setGeometry(QRect(160, 80, 64, 24));
        fileDownloaderRequiredSpace->setFrameShape(QFrame::NoFrame);
        fileDownloaderRequiredSpace->setFrameShadow(QFrame::Plain);
        fileDownloaderRequiredSpace->setSmallDecimalPoint(false);
        fileDownloaderRequiredSpace->setNumDigits(4);
        fileDownloaderRequiredSpace->setSegmentStyle(QLCDNumber::Flat);

        verticalLayout->addWidget(groupBox);

        groupBox_2 = new QGroupBox(FileOrientedDownloader);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        verticalLayout_2 = new QVBoxLayout(groupBox_2);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        label_3 = new QLabel(groupBox_2);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        verticalLayout_2->addWidget(label_3);

        fileDownloaderListWidget = new QListWidget(groupBox_2);
        fileDownloaderListWidget->setObjectName(QString::fromUtf8("fileDownloaderListWidget"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(fileDownloaderListWidget->sizePolicy().hasHeightForWidth());
        fileDownloaderListWidget->setSizePolicy(sizePolicy2);

        verticalLayout_2->addWidget(fileDownloaderListWidget);

        groupBox_4 = new QGroupBox(groupBox_2);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        sizePolicy.setHeightForWidth(groupBox_4->sizePolicy().hasHeightForWidth());
        groupBox_4->setSizePolicy(sizePolicy);
        groupBox_4->setMinimumSize(QSize(0, 40));
        verticalLayout_3 = new QVBoxLayout(groupBox_4);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        fileDownloaderStatus = new QLabel(groupBox_4);
        fileDownloaderStatus->setObjectName(QString::fromUtf8("fileDownloaderStatus"));

        verticalLayout_3->addWidget(fileDownloaderStatus);

        fileDownloaderProgressBar = new QProgressBar(groupBox_4);
        fileDownloaderProgressBar->setObjectName(QString::fromUtf8("fileDownloaderProgressBar"));
        fileDownloaderProgressBar->setValue(0);
        fileDownloaderProgressBar->setInvertedAppearance(false);

        verticalLayout_3->addWidget(fileDownloaderProgressBar);


        verticalLayout_2->addWidget(groupBox_4);


        verticalLayout->addWidget(groupBox_2);

        groupBox_3 = new QGroupBox(FileOrientedDownloader);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        sizePolicy.setHeightForWidth(groupBox_3->sizePolicy().hasHeightForWidth());
        groupBox_3->setSizePolicy(sizePolicy);
        groupBox_3->setMinimumSize(QSize(0, 39));
        horizontalLayout = new QHBoxLayout(groupBox_3);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        fileDownloaderDownloadButton = new QPushButton(groupBox_3);
        fileDownloaderDownloadButton->setObjectName(QString::fromUtf8("fileDownloaderDownloadButton"));
        fileDownloaderDownloadButton->setMaximumSize(QSize(100, 16777215));

        horizontalLayout->addWidget(fileDownloaderDownloadButton);

        fileDownloaderCancelButton = new QPushButton(groupBox_3);
        fileDownloaderCancelButton->setObjectName(QString::fromUtf8("fileDownloaderCancelButton"));
        fileDownloaderCancelButton->setMaximumSize(QSize(100, 16777215));

        horizontalLayout->addWidget(fileDownloaderCancelButton);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        verticalLayout->addWidget(groupBox_3);


        retranslateUi(FileOrientedDownloader);

        QMetaObject::connectSlotsByName(FileOrientedDownloader);
    } // setupUi

    void retranslateUi(QDialog *FileOrientedDownloader)
    {
        FileOrientedDownloader->setWindowTitle(QApplication::translate("FileOrientedDownloader", "RSIG3D File Oriented Downloader", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QString());
        fileDownloaderDirButton->setText(QApplication::translate("FileOrientedDownloader", "Directory:", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("FileOrientedDownloader", "<b>Files will be downloaded to</b>", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("FileOrientedDownloader", "Available disk space (GB):", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("FileOrientedDownloader", "Required disk space (GB):", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QString());
        label_3->setText(QApplication::translate("FileOrientedDownloader", "<b>Available data products:</b>", 0, QApplication::UnicodeUTF8));
        groupBox_4->setTitle(QString());
        fileDownloaderStatus->setText(QApplication::translate("FileOrientedDownloader", "progress", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QString());
        fileDownloaderDownloadButton->setText(QApplication::translate("FileOrientedDownloader", "Download", 0, QApplication::UnicodeUTF8));
        fileDownloaderCancelButton->setText(QApplication::translate("FileOrientedDownloader", "Cancel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class FileOrientedDownloader: public Ui_FileOrientedDownloader {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILEDOWNLOADER_H
