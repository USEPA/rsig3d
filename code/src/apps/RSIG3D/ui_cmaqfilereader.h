/********************************************************************************
** Form generated from reading UI file 'cmaqfilereader.ui'
**
** Created: Thu Jul 23 13:53:07 2026
**      by: Qt User Interface Compiler version 4.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CMAQFILEREADER_H
#define UI_CMAQFILEREADER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QScrollArea>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CMAQFileReader
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QGroupBox *groupBox_localremote;
    QRadioButton *remoteButton;
    QRadioButton *localButton;
    QSpacerItem *horizontalSpacer_3;
    QLabel *userLabel;
    QLineEdit *userText;
    QLabel *hostLabel;
    QLineEdit *hostText;
    QPushButton *connectButton;
    QSpacerItem *horizontalSpacer_4;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_2;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QVBoxLayout *verticalLayout_4;
    QGroupBox *dataDirectoryGroupbox;
    QVBoxLayout *verticalLayout_3;
    QLineEdit *dataDirectoryText;
    QGroupBox *dataFileGroupbox;
    QVBoxLayout *verticalLayout_5;
    QListWidget *dataDirectoryListingText;
    QGroupBox *dataVariableGroupbox;
    QVBoxLayout *verticalLayout_6;
    QListWidget *dataVariableListingText;
    QGroupBox *htDirectoryGroupbox;
    QVBoxLayout *verticalLayout_7;
    QLineEdit *htDirectoryText;
    QGroupBox *htDirectoryListingGroupbox;
    QVBoxLayout *verticalLayout_8;
    QListWidget *htDirectoryListingText;
    QGroupBox *zfDirectoryGroupbox;
    QVBoxLayout *verticalLayout_9;
    QLineEdit *zfDirectoryText;
    QGroupBox *zfDirectoryListingGroupbox;
    QVBoxLayout *verticalLayout_10;
    QListWidget *zfDirectoryListingText;
    QGroupBox *wwindDirectoryGroupbox;
    QVBoxLayout *verticalLayout_11;
    QLineEdit *wwindDirectoryText;
    QGroupBox *wwindDirectoryListingGroupbox;
    QVBoxLayout *verticalLayout_12;
    QListWidget *wwindDirectoryListingText;
    QSpacerItem *verticalSpacer;
    QGroupBox *groupBox_3;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *readButton;
    QPushButton *cancelButton;
    QSpacerItem *horizontalSpacer_2;

    void setupUi(QDialog *CMAQFileReader)
    {
        if (CMAQFileReader->objectName().isEmpty())
            CMAQFileReader->setObjectName(QString::fromUtf8("CMAQFileReader"));
        CMAQFileReader->resize(1117, 1071);
        CMAQFileReader->setMinimumSize(QSize(700, 700));
        CMAQFileReader->setModal(true);
        verticalLayout = new QVBoxLayout(CMAQFileReader);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        groupBox = new QGroupBox(CMAQFileReader);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy);
        groupBox->setMinimumSize(QSize(1100, 45));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        groupBox_localremote = new QGroupBox(groupBox);
        groupBox_localremote->setObjectName(QString::fromUtf8("groupBox_localremote"));
        groupBox_localremote->setMinimumSize(QSize(400, 30));
        groupBox_localremote->setStyleSheet(QString::fromUtf8(""));
        groupBox_localremote->setFlat(false);
        remoteButton = new QRadioButton(groupBox_localremote);
        remoteButton->setObjectName(QString::fromUtf8("remoteButton"));
        remoteButton->setGeometry(QRect(180, 3, 170, 30));
        QSizePolicy sizePolicy1(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(remoteButton->sizePolicy().hasHeightForWidth());
        remoteButton->setSizePolicy(sizePolicy1);
        remoteButton->setMinimumSize(QSize(170, 20));
        localButton = new QRadioButton(groupBox_localremote);
        localButton->setObjectName(QString::fromUtf8("localButton"));
        localButton->setGeometry(QRect(0, 2, 170, 30));
        sizePolicy1.setHeightForWidth(localButton->sizePolicy().hasHeightForWidth());
        localButton->setSizePolicy(sizePolicy1);
        localButton->setMinimumSize(QSize(170, 20));

        gridLayout->addWidget(groupBox_localremote, 0, 0, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_3, 0, 1, 1, 1);

        userLabel = new QLabel(groupBox);
        userLabel->setObjectName(QString::fromUtf8("userLabel"));

        gridLayout->addWidget(userLabel, 0, 2, 1, 1);

        userText = new QLineEdit(groupBox);
        userText->setObjectName(QString::fromUtf8("userText"));
        sizePolicy.setHeightForWidth(userText->sizePolicy().hasHeightForWidth());
        userText->setSizePolicy(sizePolicy);

        gridLayout->addWidget(userText, 0, 3, 1, 1);

        hostLabel = new QLabel(groupBox);
        hostLabel->setObjectName(QString::fromUtf8("hostLabel"));

        gridLayout->addWidget(hostLabel, 0, 4, 1, 1);

        hostText = new QLineEdit(groupBox);
        hostText->setObjectName(QString::fromUtf8("hostText"));
        sizePolicy.setHeightForWidth(hostText->sizePolicy().hasHeightForWidth());
        hostText->setSizePolicy(sizePolicy);

        gridLayout->addWidget(hostText, 0, 5, 1, 1);

        connectButton = new QPushButton(groupBox);
        connectButton->setObjectName(QString::fromUtf8("connectButton"));
        sizePolicy.setHeightForWidth(connectButton->sizePolicy().hasHeightForWidth());
        connectButton->setSizePolicy(sizePolicy);

        gridLayout->addWidget(connectButton, 0, 6, 1, 1);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_4, 0, 7, 1, 1);


        verticalLayout->addWidget(groupBox);

        groupBox_2 = new QGroupBox(CMAQFileReader);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(groupBox_2->sizePolicy().hasHeightForWidth());
        groupBox_2->setSizePolicy(sizePolicy2);
        verticalLayout_2 = new QVBoxLayout(groupBox_2);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        scrollArea = new QScrollArea(groupBox_2);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(scrollArea->sizePolicy().hasHeightForWidth());
        scrollArea->setSizePolicy(sizePolicy3);
        scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 1060, 942));
        verticalLayout_4 = new QVBoxLayout(scrollAreaWidgetContents);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        dataDirectoryGroupbox = new QGroupBox(scrollAreaWidgetContents);
        dataDirectoryGroupbox->setObjectName(QString::fromUtf8("dataDirectoryGroupbox"));
        QSizePolicy sizePolicy4(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(dataDirectoryGroupbox->sizePolicy().hasHeightForWidth());
        dataDirectoryGroupbox->setSizePolicy(sizePolicy4);
        dataDirectoryGroupbox->setMinimumSize(QSize(0, 55));
        dataDirectoryGroupbox->setStyleSheet(QString::fromUtf8("QGroupBox {\n"
"margin-left: 0px;\n"
"padding-left: 0px;\n"
"}\n"
"QGroupBox::title {\n"
"subcontrol-origin: margin;\n"
"subcontrol-position: left top;\n"
"left: 0px;\n"
"padding: 0px;\n"
"}"));
        dataDirectoryGroupbox->setFlat(false);
        verticalLayout_3 = new QVBoxLayout(dataDirectoryGroupbox);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(-1, 26, 6, 0);
        dataDirectoryText = new QLineEdit(dataDirectoryGroupbox);
        dataDirectoryText->setObjectName(QString::fromUtf8("dataDirectoryText"));
        sizePolicy3.setHeightForWidth(dataDirectoryText->sizePolicy().hasHeightForWidth());
        dataDirectoryText->setSizePolicy(sizePolicy3);
        dataDirectoryText->setFrame(true);

        verticalLayout_3->addWidget(dataDirectoryText);


        verticalLayout_4->addWidget(dataDirectoryGroupbox);

        dataFileGroupbox = new QGroupBox(scrollAreaWidgetContents);
        dataFileGroupbox->setObjectName(QString::fromUtf8("dataFileGroupbox"));
        sizePolicy4.setHeightForWidth(dataFileGroupbox->sizePolicy().hasHeightForWidth());
        dataFileGroupbox->setSizePolicy(sizePolicy4);
        dataFileGroupbox->setMinimumSize(QSize(0, 130));
        dataFileGroupbox->setStyleSheet(QString::fromUtf8("QGroupBox {\n"
"margin-left: 0px;\n"
"padding-left: 0px;\n"
"}\n"
"QGroupBox::title {\n"
"subcontrol-origin: margin;\n"
"subcontrol-position: left top;\n"
"left: 0px;\n"
"padding: 0px;\n"
"}"));
        dataFileGroupbox->setFlat(false);
        verticalLayout_5 = new QVBoxLayout(dataFileGroupbox);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        verticalLayout_5->setContentsMargins(-1, 26, -1, 0);
        dataDirectoryListingText = new QListWidget(dataFileGroupbox);
        dataDirectoryListingText->setObjectName(QString::fromUtf8("dataDirectoryListingText"));
        QSizePolicy sizePolicy5(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(dataDirectoryListingText->sizePolicy().hasHeightForWidth());
        dataDirectoryListingText->setSizePolicy(sizePolicy5);
        dataDirectoryListingText->setMinimumSize(QSize(0, 100));
        dataDirectoryListingText->setMaximumSize(QSize(16777215, 100));

        verticalLayout_5->addWidget(dataDirectoryListingText);


        verticalLayout_4->addWidget(dataFileGroupbox);

        dataVariableGroupbox = new QGroupBox(scrollAreaWidgetContents);
        dataVariableGroupbox->setObjectName(QString::fromUtf8("dataVariableGroupbox"));
        sizePolicy2.setHeightForWidth(dataVariableGroupbox->sizePolicy().hasHeightForWidth());
        dataVariableGroupbox->setSizePolicy(sizePolicy2);
        dataVariableGroupbox->setMinimumSize(QSize(0, 130));
        dataVariableGroupbox->setStyleSheet(QString::fromUtf8("QGroupBox {\n"
"margin-left: 0px;\n"
"padding-left: 0px;\n"
"}\n"
"QGroupBox::title {\n"
"subcontrol-origin: margin;\n"
"subcontrol-position: left top;\n"
"left: 0px;\n"
"padding: 0px;\n"
"}"));
        dataVariableGroupbox->setFlat(false);
        verticalLayout_6 = new QVBoxLayout(dataVariableGroupbox);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        verticalLayout_6->setContentsMargins(-1, 26, -1, 0);
        dataVariableListingText = new QListWidget(dataVariableGroupbox);
        dataVariableListingText->setObjectName(QString::fromUtf8("dataVariableListingText"));
        sizePolicy5.setHeightForWidth(dataVariableListingText->sizePolicy().hasHeightForWidth());
        dataVariableListingText->setSizePolicy(sizePolicy5);
        dataVariableListingText->setMinimumSize(QSize(0, 100));
        dataVariableListingText->setMaximumSize(QSize(16777215, 100));

        verticalLayout_6->addWidget(dataVariableListingText);


        verticalLayout_4->addWidget(dataVariableGroupbox);

        htDirectoryGroupbox = new QGroupBox(scrollAreaWidgetContents);
        htDirectoryGroupbox->setObjectName(QString::fromUtf8("htDirectoryGroupbox"));
        sizePolicy2.setHeightForWidth(htDirectoryGroupbox->sizePolicy().hasHeightForWidth());
        htDirectoryGroupbox->setSizePolicy(sizePolicy2);
        htDirectoryGroupbox->setMinimumSize(QSize(0, 55));
        htDirectoryGroupbox->setStyleSheet(QString::fromUtf8("QGroupBox {\n"
"margin-left: 0px;\n"
"padding-left: 0px;\n"
"}\n"
"QGroupBox::title {\n"
"subcontrol-origin: margin;\n"
"subcontrol-position: left top;\n"
"left: 0px;\n"
"padding: 0px;\n"
"}"));
        htDirectoryGroupbox->setFlat(false);
        verticalLayout_7 = new QVBoxLayout(htDirectoryGroupbox);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        verticalLayout_7->setContentsMargins(-1, 26, -1, 0);
        htDirectoryText = new QLineEdit(htDirectoryGroupbox);
        htDirectoryText->setObjectName(QString::fromUtf8("htDirectoryText"));
        sizePolicy3.setHeightForWidth(htDirectoryText->sizePolicy().hasHeightForWidth());
        htDirectoryText->setSizePolicy(sizePolicy3);

        verticalLayout_7->addWidget(htDirectoryText);


        verticalLayout_4->addWidget(htDirectoryGroupbox);

        htDirectoryListingGroupbox = new QGroupBox(scrollAreaWidgetContents);
        htDirectoryListingGroupbox->setObjectName(QString::fromUtf8("htDirectoryListingGroupbox"));
        sizePolicy2.setHeightForWidth(htDirectoryListingGroupbox->sizePolicy().hasHeightForWidth());
        htDirectoryListingGroupbox->setSizePolicy(sizePolicy2);
        htDirectoryListingGroupbox->setMinimumSize(QSize(0, 130));
        htDirectoryListingGroupbox->setStyleSheet(QString::fromUtf8("QGroupBox {\n"
"margin-left: 0px;\n"
"padding-left: 0px;\n"
"}\n"
"QGroupBox::title {\n"
"subcontrol-origin: margin;\n"
"subcontrol-position: left top;\n"
"left: 0px;\n"
"padding: 0px;\n"
"}"));
        htDirectoryListingGroupbox->setTitle(QString::fromUtf8("Optional: Select a subdirectory or GRIDCRO2D file (containing HT) for time-varying elevations:"));
        htDirectoryListingGroupbox->setFlat(false);
        verticalLayout_8 = new QVBoxLayout(htDirectoryListingGroupbox);
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        verticalLayout_8->setContentsMargins(-1, 26, 9, 0);
        htDirectoryListingText = new QListWidget(htDirectoryListingGroupbox);
        htDirectoryListingText->setObjectName(QString::fromUtf8("htDirectoryListingText"));
        sizePolicy5.setHeightForWidth(htDirectoryListingText->sizePolicy().hasHeightForWidth());
        htDirectoryListingText->setSizePolicy(sizePolicy5);
        htDirectoryListingText->setMinimumSize(QSize(0, 100));
        htDirectoryListingText->setMaximumSize(QSize(16777215, 100));

        verticalLayout_8->addWidget(htDirectoryListingText);


        verticalLayout_4->addWidget(htDirectoryListingGroupbox);

        zfDirectoryGroupbox = new QGroupBox(scrollAreaWidgetContents);
        zfDirectoryGroupbox->setObjectName(QString::fromUtf8("zfDirectoryGroupbox"));
        sizePolicy2.setHeightForWidth(zfDirectoryGroupbox->sizePolicy().hasHeightForWidth());
        zfDirectoryGroupbox->setSizePolicy(sizePolicy2);
        zfDirectoryGroupbox->setMinimumSize(QSize(0, 55));
        zfDirectoryGroupbox->setMaximumSize(QSize(16777215, 125));
        zfDirectoryGroupbox->setStyleSheet(QString::fromUtf8("QGroupBox {\n"
"margin-left: 0px;\n"
"padding-left: 0px;\n"
"}\n"
"QGroupBox::title {\n"
"subcontrol-origin: margin;\n"
"subcontrol-position: left top;\n"
"left: 0px;\n"
"padding: 0px;\n"
"}"));
        zfDirectoryGroupbox->setFlat(false);
        verticalLayout_9 = new QVBoxLayout(zfDirectoryGroupbox);
        verticalLayout_9->setObjectName(QString::fromUtf8("verticalLayout_9"));
        verticalLayout_9->setContentsMargins(-1, 26, -1, 0);
        zfDirectoryText = new QLineEdit(zfDirectoryGroupbox);
        zfDirectoryText->setObjectName(QString::fromUtf8("zfDirectoryText"));
        sizePolicy3.setHeightForWidth(zfDirectoryText->sizePolicy().hasHeightForWidth());
        zfDirectoryText->setSizePolicy(sizePolicy3);

        verticalLayout_9->addWidget(zfDirectoryText);


        verticalLayout_4->addWidget(zfDirectoryGroupbox);

        zfDirectoryListingGroupbox = new QGroupBox(scrollAreaWidgetContents);
        zfDirectoryListingGroupbox->setObjectName(QString::fromUtf8("zfDirectoryListingGroupbox"));
        sizePolicy2.setHeightForWidth(zfDirectoryListingGroupbox->sizePolicy().hasHeightForWidth());
        zfDirectoryListingGroupbox->setSizePolicy(sizePolicy2);
        zfDirectoryListingGroupbox->setMinimumSize(QSize(0, 130));
        zfDirectoryListingGroupbox->setMaximumSize(QSize(16777215, 1250));
        zfDirectoryListingGroupbox->setStyleSheet(QString::fromUtf8("QGroupBox {\n"
"margin-left: 0px;\n"
"padding-left: 0px;\n"
"}\n"
"QGroupBox::title {\n"
"subcontrol-origin: margin;\n"
"subcontrol-position: left top;\n"
"left: 0px;\n"
"padding: 0px;\n"
"}"));
        zfDirectoryListingGroupbox->setTitle(QString::fromUtf8("Optional: Select a subdirectory or METCRO3D file (containing ZF) for time-varying elevations:"));
        zfDirectoryListingGroupbox->setFlat(false);
        verticalLayout_10 = new QVBoxLayout(zfDirectoryListingGroupbox);
        verticalLayout_10->setObjectName(QString::fromUtf8("verticalLayout_10"));
        verticalLayout_10->setContentsMargins(-1, 26, -1, 0);
        zfDirectoryListingText = new QListWidget(zfDirectoryListingGroupbox);
        zfDirectoryListingText->setObjectName(QString::fromUtf8("zfDirectoryListingText"));
        sizePolicy5.setHeightForWidth(zfDirectoryListingText->sizePolicy().hasHeightForWidth());
        zfDirectoryListingText->setSizePolicy(sizePolicy5);
        zfDirectoryListingText->setMinimumSize(QSize(0, 100));
        zfDirectoryListingText->setMaximumSize(QSize(16777215, 100));

        verticalLayout_10->addWidget(zfDirectoryListingText);


        verticalLayout_4->addWidget(zfDirectoryListingGroupbox);

        wwindDirectoryGroupbox = new QGroupBox(scrollAreaWidgetContents);
        wwindDirectoryGroupbox->setObjectName(QString::fromUtf8("wwindDirectoryGroupbox"));
        sizePolicy2.setHeightForWidth(wwindDirectoryGroupbox->sizePolicy().hasHeightForWidth());
        wwindDirectoryGroupbox->setSizePolicy(sizePolicy2);
        wwindDirectoryGroupbox->setMinimumSize(QSize(0, 55));
        wwindDirectoryGroupbox->setStyleSheet(QString::fromUtf8("QGroupBox {\n"
"margin-left: 0px;\n"
"padding-left: 0px;\n"
"}\n"
"QGroupBox::title {\n"
"subcontrol-origin: margin;\n"
"subcontrol-position: left top;\n"
"left: 0px;\n"
"padding: 0px;\n"
"}"));
        wwindDirectoryGroupbox->setFlat(false);
        verticalLayout_11 = new QVBoxLayout(wwindDirectoryGroupbox);
        verticalLayout_11->setObjectName(QString::fromUtf8("verticalLayout_11"));
        verticalLayout_11->setContentsMargins(-1, 26, -1, 0);
        wwindDirectoryText = new QLineEdit(wwindDirectoryGroupbox);
        wwindDirectoryText->setObjectName(QString::fromUtf8("wwindDirectoryText"));
        sizePolicy3.setHeightForWidth(wwindDirectoryText->sizePolicy().hasHeightForWidth());
        wwindDirectoryText->setSizePolicy(sizePolicy3);

        verticalLayout_11->addWidget(wwindDirectoryText);


        verticalLayout_4->addWidget(wwindDirectoryGroupbox);

        wwindDirectoryListingGroupbox = new QGroupBox(scrollAreaWidgetContents);
        wwindDirectoryListingGroupbox->setObjectName(QString::fromUtf8("wwindDirectoryListingGroupbox"));
        sizePolicy2.setHeightForWidth(wwindDirectoryListingGroupbox->sizePolicy().hasHeightForWidth());
        wwindDirectoryListingGroupbox->setSizePolicy(sizePolicy2);
        wwindDirectoryListingGroupbox->setMinimumSize(QSize(0, 130));
        wwindDirectoryListingGroupbox->setMaximumSize(QSize(16777215, 1250));
        wwindDirectoryListingGroupbox->setStyleSheet(QString::fromUtf8("QGroupBox {\n"
"margin-left: 0px;\n"
"padding-left: 0px;\n"
"}\n"
"QGroupBox::title {\n"
"subcontrol-origin: margin;\n"
"subcontrol-position: left top;\n"
"left: 0px;\n"
"padding: 0px;\n"
"}"));
        wwindDirectoryListingGroupbox->setTitle(QString::fromUtf8("Optional: Select a subdirectory or METCRO3D/CONC file (containing WWIND or W_VEL) for 3D wind:"));
        wwindDirectoryListingGroupbox->setFlat(false);
        verticalLayout_12 = new QVBoxLayout(wwindDirectoryListingGroupbox);
        verticalLayout_12->setSpacing(6);
        verticalLayout_12->setObjectName(QString::fromUtf8("verticalLayout_12"));
        verticalLayout_12->setContentsMargins(-1, 26, -1, 0);
        wwindDirectoryListingText = new QListWidget(wwindDirectoryListingGroupbox);
        wwindDirectoryListingText->setObjectName(QString::fromUtf8("wwindDirectoryListingText"));
        sizePolicy5.setHeightForWidth(wwindDirectoryListingText->sizePolicy().hasHeightForWidth());
        wwindDirectoryListingText->setSizePolicy(sizePolicy5);
        wwindDirectoryListingText->setMinimumSize(QSize(0, 100));
        wwindDirectoryListingText->setMaximumSize(QSize(16777215, 100));

        verticalLayout_12->addWidget(wwindDirectoryListingText);


        verticalLayout_4->addWidget(wwindDirectoryListingGroupbox);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_4->addItem(verticalSpacer);

        scrollArea->setWidget(scrollAreaWidgetContents);
        dataDirectoryGroupbox->raise();
        dataFileGroupbox->raise();
        dataVariableGroupbox->raise();
        htDirectoryGroupbox->raise();
        htDirectoryListingGroupbox->raise();
        zfDirectoryGroupbox->raise();
        zfDirectoryListingGroupbox->raise();
        wwindDirectoryGroupbox->raise();
        wwindDirectoryListingGroupbox->raise();

        verticalLayout_2->addWidget(scrollArea);


        verticalLayout->addWidget(groupBox_2);

        groupBox_3 = new QGroupBox(CMAQFileReader);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        QSizePolicy sizePolicy6(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy6.setHorizontalStretch(0);
        sizePolicy6.setVerticalStretch(0);
        sizePolicy6.setHeightForWidth(groupBox_3->sizePolicy().hasHeightForWidth());
        groupBox_3->setSizePolicy(sizePolicy6);
        groupBox_3->setMinimumSize(QSize(0, 39));
        horizontalLayout = new QHBoxLayout(groupBox_3);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        readButton = new QPushButton(groupBox_3);
        readButton->setObjectName(QString::fromUtf8("readButton"));
        sizePolicy1.setHeightForWidth(readButton->sizePolicy().hasHeightForWidth());
        readButton->setSizePolicy(sizePolicy1);
        readButton->setMaximumSize(QSize(200, 16777215));

        horizontalLayout->addWidget(readButton);

        cancelButton = new QPushButton(groupBox_3);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));
        sizePolicy1.setHeightForWidth(cancelButton->sizePolicy().hasHeightForWidth());
        cancelButton->setSizePolicy(sizePolicy1);
        cancelButton->setMaximumSize(QSize(200, 16777215));

        horizontalLayout->addWidget(cancelButton);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        verticalLayout->addWidget(groupBox_3);


        retranslateUi(CMAQFileReader);

        QMetaObject::connectSlotsByName(CMAQFileReader);
    } // setupUi

    void retranslateUi(QDialog *CMAQFileReader)
    {
        CMAQFileReader->setWindowTitle(QApplication::translate("CMAQFileReader", "CMAQ File Reader", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QString());
        groupBox_localremote->setTitle(QString());
        remoteButton->setText(QApplication::translate("CMAQFileReader", "Remote", 0, QApplication::UnicodeUTF8));
        localButton->setText(QApplication::translate("CMAQFileReader", "Local", 0, QApplication::UnicodeUTF8));
        userLabel->setText(QApplication::translate("CMAQFileReader", "User:", 0, QApplication::UnicodeUTF8));
        hostLabel->setText(QApplication::translate("CMAQFileReader", "Host:", 0, QApplication::UnicodeUTF8));
        connectButton->setText(QApplication::translate("CMAQFileReader", "Connect", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QString());
        dataDirectoryGroupbox->setTitle(QApplication::translate("CMAQFileReader", "Data File Directory:", 0, QApplication::UnicodeUTF8));
        dataDirectoryText->setText(QString());
        dataFileGroupbox->setTitle(QApplication::translate("CMAQFileReader", "Select a subdirectory or data file:", 0, QApplication::UnicodeUTF8));
        dataVariableGroupbox->setTitle(QApplication::translate("CMAQFileReader", "Select a data variable to read:", 0, QApplication::UnicodeUTF8));
        htDirectoryGroupbox->setTitle(QApplication::translate("CMAQFileReader", "Optional GRIDCRO2D File Directory:", 0, QApplication::UnicodeUTF8));
        zfDirectoryGroupbox->setTitle(QApplication::translate("CMAQFileReader", "Optional METCRO3D File Directory:", 0, QApplication::UnicodeUTF8));
        wwindDirectoryGroupbox->setTitle(QApplication::translate("CMAQFileReader", "Optional METCRO3D/CONC File Directory:", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QString());
        readButton->setText(QApplication::translate("CMAQFileReader", "Read Variable", 0, QApplication::UnicodeUTF8));
        cancelButton->setText(QApplication::translate("CMAQFileReader", "Cancel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class CMAQFileReader: public Ui_CMAQFileReader {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CMAQFILEREADER_H
