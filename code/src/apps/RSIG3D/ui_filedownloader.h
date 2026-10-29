/********************************************************************************
** Form generated from reading UI file 'filedownloader.ui'
**
** Created: Thu Jul 23 13:53:07 2026
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
#include <QtGui/QGridLayout>
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
    QGridLayout *gridLayout;
    QLabel *label;
    QToolButton *fileDownloaderDirButton;
    QLineEdit *fileDownloaderDirLineEdit;
    QLabel *label_4;
    QLCDNumber *fileDownloaderRequiredSpace;
    QLabel *label_2;
    QLCDNumber *fileDownloaderAvailableSpace;
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
        groupBox->setMinimumSize(QSize(0, 160));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 3);

        fileDownloaderDirButton = new QToolButton(groupBox);
        fileDownloaderDirButton->setObjectName(QString::fromUtf8("fileDownloaderDirButton"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(fileDownloaderDirButton->sizePolicy().hasHeightForWidth());
        fileDownloaderDirButton->setSizePolicy(sizePolicy1);
        fileDownloaderDirButton->setMinimumSize(QSize(72, 22));
        fileDownloaderDirButton->setMaximumSize(QSize(72, 20));

        gridLayout->addWidget(fileDownloaderDirButton, 1, 0, 1, 1);

        fileDownloaderDirLineEdit = new QLineEdit(groupBox);
        fileDownloaderDirLineEdit->setObjectName(QString::fromUtf8("fileDownloaderDirLineEdit"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(fileDownloaderDirLineEdit->sizePolicy().hasHeightForWidth());
        fileDownloaderDirLineEdit->setSizePolicy(sizePolicy2);
        fileDownloaderDirLineEdit->setMinimumSize(QSize(300, 22));
        fileDownloaderDirLineEdit->setMaximumSize(QSize(1024, 22));

        gridLayout->addWidget(fileDownloaderDirLineEdit, 1, 1, 1, 2);

        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 4, 0, 1, 2);

        fileDownloaderRequiredSpace = new QLCDNumber(groupBox);
        fileDownloaderRequiredSpace->setObjectName(QString::fromUtf8("fileDownloaderRequiredSpace"));
        fileDownloaderRequiredSpace->setFrameShape(QFrame::NoFrame);
        fileDownloaderRequiredSpace->setFrameShadow(QFrame::Plain);
        fileDownloaderRequiredSpace->setSmallDecimalPoint(false);
        fileDownloaderRequiredSpace->setNumDigits(4);
        fileDownloaderRequiredSpace->setSegmentStyle(QLCDNumber::Flat);

        gridLayout->addWidget(fileDownloaderRequiredSpace, 4, 2, 1, 1);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 3, 0, 1, 2);

        fileDownloaderAvailableSpace = new QLCDNumber(groupBox);
        fileDownloaderAvailableSpace->setObjectName(QString::fromUtf8("fileDownloaderAvailableSpace"));
        fileDownloaderAvailableSpace->setFrameShape(QFrame::NoFrame);
        fileDownloaderAvailableSpace->setFrameShadow(QFrame::Plain);
        fileDownloaderAvailableSpace->setNumDigits(4);
        fileDownloaderAvailableSpace->setSegmentStyle(QLCDNumber::Flat);

        gridLayout->addWidget(fileDownloaderAvailableSpace, 3, 2, 1, 1);


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
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(fileDownloaderListWidget->sizePolicy().hasHeightForWidth());
        fileDownloaderListWidget->setSizePolicy(sizePolicy3);

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
        sizePolicy.setHeightForWidth(fileDownloaderDownloadButton->sizePolicy().hasHeightForWidth());
        fileDownloaderDownloadButton->setSizePolicy(sizePolicy);
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
        label->setText(QApplication::translate("FileOrientedDownloader", "<b>Files will be downloaded to</b>", 0, QApplication::UnicodeUTF8));
        fileDownloaderDirButton->setText(QApplication::translate("FileOrientedDownloader", "Directory:", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("FileOrientedDownloader", "Required disk space (GB):", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("FileOrientedDownloader", "Available disk space (GB):", 0, QApplication::UnicodeUTF8));
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
