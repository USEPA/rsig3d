/********************************************************************************
** Form generated from reading UI file 'cmaqfilereader.ui'
**
** Created: Thu Jun 12 13:29:43 2025
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
    QHBoxLayout *horizontalLayout_2;
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
        CMAQFileReader->resize(700, 750);
        CMAQFileReader->setMinimumSize(QSize(700, 700));
        CMAQFileReader->setModal(true);
        verticalLayout = new QVBoxLayout(CMAQFileReader);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        groupBox = new QGroupBox(CMAQFileReader);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy);
        groupBox->setMinimumSize(QSize(0, 45));
        horizontalLayout_2 = new QHBoxLayout(groupBox);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        groupBox_localremote = new QGroupBox(groupBox);
        groupBox_localremote->setObjectName(QString::fromUtf8("groupBox_localremote"));
        groupBox_localremote->setMinimumSize(QSize(150, 30));
        groupBox_localremote->setFlat(false);
        remoteButton = new QRadioButton(groupBox_localremote);
        remoteButton->setObjectName(QString::fromUtf8("remoteButton"));
        remoteButton->setGeometry(QRect(80, 3, 71, 17));
        localButton = new QRadioButton(groupBox_localremote);
        localButton->setObjectName(QString::fromUtf8("localButton"));
        localButton->setGeometry(QRect(10, 4, 61, 17));

        horizontalLayout_2->addWidget(groupBox_localremote);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);

        userLabel = new QLabel(groupBox);
        userLabel->setObjectName(QString::fromUtf8("userLabel"));

        horizontalLayout_2->addWidget(userLabel);

        userText = new QLineEdit(groupBox);
        userText->setObjectName(QString::fromUtf8("userText"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(userText->sizePolicy().hasHeightForWidth());
        userText->setSizePolicy(sizePolicy1);

        horizontalLayout_2->addWidget(userText);

        hostLabel = new QLabel(groupBox);
        hostLabel->setObjectName(QString::fromUtf8("hostLabel"));

        horizontalLayout_2->addWidget(hostLabel);

        hostText = new QLineEdit(groupBox);
        hostText->setObjectName(QString::fromUtf8("hostText"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(hostText->sizePolicy().hasHeightForWidth());
        hostText->setSizePolicy(sizePolicy2);

        horizontalLayout_2->addWidget(hostText);

        connectButton = new QPushButton(groupBox);
        connectButton->setObjectName(QString::fromUtf8("connectButton"));

        horizontalLayout_2->addWidget(connectButton);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_4);


        verticalLayout->addWidget(groupBox);

        groupBox_2 = new QGroupBox(CMAQFileReader);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        verticalLayout_2 = new QVBoxLayout(groupBox_2);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        scrollArea = new QScrollArea(groupBox_2);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, -281, 643, 942));
        verticalLayout_4 = new QVBoxLayout(scrollAreaWidgetContents);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        dataDirectoryGroupbox = new QGroupBox(scrollAreaWidgetContents);
        dataDirectoryGroupbox->setObjectName(QString::fromUtf8("dataDirectoryGroupbox"));
        sizePolicy2.setHeightForWidth(dataDirectoryGroupbox->sizePolicy().hasHeightForWidth());
        dataDirectoryGroupbox->setSizePolicy(sizePolicy2);
        dataDirectoryGroupbox->setMinimumSize(QSize(0, 55));
        dataDirectoryGroupbox->setFlat(true);
        verticalLayout_3 = new QVBoxLayout(dataDirectoryGroupbox);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        dataDirectoryText = new QLineEdit(dataDirectoryGroupbox);
        dataDirectoryText->setObjectName(QString::fromUtf8("dataDirectoryText"));

        verticalLayout_3->addWidget(dataDirectoryText);


        verticalLayout_4->addWidget(dataDirectoryGroupbox);

        dataFileGroupbox = new QGroupBox(scrollAreaWidgetContents);
        dataFileGroupbox->setObjectName(QString::fromUtf8("dataFileGroupbox"));
        sizePolicy2.setHeightForWidth(dataFileGroupbox->sizePolicy().hasHeightForWidth());
        dataFileGroupbox->setSizePolicy(sizePolicy2);
        dataFileGroupbox->setMinimumSize(QSize(0, 130));
        dataFileGroupbox->setFlat(true);
        verticalLayout_5 = new QVBoxLayout(dataFileGroupbox);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        dataDirectoryListingText = new QListWidget(dataFileGroupbox);
        dataDirectoryListingText->setObjectName(QString::fromUtf8("dataDirectoryListingText"));
        sizePolicy2.setHeightForWidth(dataDirectoryListingText->sizePolicy().hasHeightForWidth());
        dataDirectoryListingText->setSizePolicy(sizePolicy2);
        dataDirectoryListingText->setMinimumSize(QSize(0, 100));
        dataDirectoryListingText->setMaximumSize(QSize(16777215, 100));

        verticalLayout_5->addWidget(dataDirectoryListingText);


        verticalLayout_4->addWidget(dataFileGroupbox);

        dataVariableGroupbox = new QGroupBox(scrollAreaWidgetContents);
        dataVariableGroupbox->setObjectName(QString::fromUtf8("dataVariableGroupbox"));
        sizePolicy.setHeightForWidth(dataVariableGroupbox->sizePolicy().hasHeightForWidth());
        dataVariableGroupbox->setSizePolicy(sizePolicy);
        dataVariableGroupbox->setMinimumSize(QSize(0, 130));
        dataVariableGroupbox->setFlat(true);
        verticalLayout_6 = new QVBoxLayout(dataVariableGroupbox);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        dataVariableListingText = new QListWidget(dataVariableGroupbox);
        dataVariableListingText->setObjectName(QString::fromUtf8("dataVariableListingText"));
        sizePolicy2.setHeightForWidth(dataVariableListingText->sizePolicy().hasHeightForWidth());
        dataVariableListingText->setSizePolicy(sizePolicy2);
        dataVariableListingText->setMinimumSize(QSize(0, 100));
        dataVariableListingText->setMaximumSize(QSize(16777215, 100));

        verticalLayout_6->addWidget(dataVariableListingText);


        verticalLayout_4->addWidget(dataVariableGroupbox);

        htDirectoryGroupbox = new QGroupBox(scrollAreaWidgetContents);
        htDirectoryGroupbox->setObjectName(QString::fromUtf8("htDirectoryGroupbox"));
        sizePolicy.setHeightForWidth(htDirectoryGroupbox->sizePolicy().hasHeightForWidth());
        htDirectoryGroupbox->setSizePolicy(sizePolicy);
        htDirectoryGroupbox->setMinimumSize(QSize(0, 55));
        htDirectoryGroupbox->setFlat(true);
        verticalLayout_7 = new QVBoxLayout(htDirectoryGroupbox);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        htDirectoryText = new QLineEdit(htDirectoryGroupbox);
        htDirectoryText->setObjectName(QString::fromUtf8("htDirectoryText"));

        verticalLayout_7->addWidget(htDirectoryText);


        verticalLayout_4->addWidget(htDirectoryGroupbox);

        htDirectoryListingGroupbox = new QGroupBox(scrollAreaWidgetContents);
        htDirectoryListingGroupbox->setObjectName(QString::fromUtf8("htDirectoryListingGroupbox"));
        sizePolicy.setHeightForWidth(htDirectoryListingGroupbox->sizePolicy().hasHeightForWidth());
        htDirectoryListingGroupbox->setSizePolicy(sizePolicy);
        htDirectoryListingGroupbox->setMinimumSize(QSize(0, 130));
        htDirectoryListingGroupbox->setFlat(true);
        verticalLayout_8 = new QVBoxLayout(htDirectoryListingGroupbox);
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        htDirectoryListingText = new QListWidget(htDirectoryListingGroupbox);
        htDirectoryListingText->setObjectName(QString::fromUtf8("htDirectoryListingText"));
        sizePolicy2.setHeightForWidth(htDirectoryListingText->sizePolicy().hasHeightForWidth());
        htDirectoryListingText->setSizePolicy(sizePolicy2);
        htDirectoryListingText->setMinimumSize(QSize(0, 100));
        htDirectoryListingText->setMaximumSize(QSize(16777215, 100));

        verticalLayout_8->addWidget(htDirectoryListingText);


        verticalLayout_4->addWidget(htDirectoryListingGroupbox);

        zfDirectoryGroupbox = new QGroupBox(scrollAreaWidgetContents);
        zfDirectoryGroupbox->setObjectName(QString::fromUtf8("zfDirectoryGroupbox"));
        sizePolicy.setHeightForWidth(zfDirectoryGroupbox->sizePolicy().hasHeightForWidth());
        zfDirectoryGroupbox->setSizePolicy(sizePolicy);
        zfDirectoryGroupbox->setMinimumSize(QSize(0, 55));
        zfDirectoryGroupbox->setMaximumSize(QSize(16777215, 125));
        zfDirectoryGroupbox->setFlat(true);
        verticalLayout_9 = new QVBoxLayout(zfDirectoryGroupbox);
        verticalLayout_9->setObjectName(QString::fromUtf8("verticalLayout_9"));
        zfDirectoryText = new QLineEdit(zfDirectoryGroupbox);
        zfDirectoryText->setObjectName(QString::fromUtf8("zfDirectoryText"));

        verticalLayout_9->addWidget(zfDirectoryText);


        verticalLayout_4->addWidget(zfDirectoryGroupbox);

        zfDirectoryListingGroupbox = new QGroupBox(scrollAreaWidgetContents);
        zfDirectoryListingGroupbox->setObjectName(QString::fromUtf8("zfDirectoryListingGroupbox"));
        zfDirectoryListingGroupbox->setMinimumSize(QSize(0, 130));
        zfDirectoryListingGroupbox->setMaximumSize(QSize(16777215, 125));
        zfDirectoryListingGroupbox->setFlat(true);
        verticalLayout_10 = new QVBoxLayout(zfDirectoryListingGroupbox);
        verticalLayout_10->setObjectName(QString::fromUtf8("verticalLayout_10"));
        zfDirectoryListingText = new QListWidget(zfDirectoryListingGroupbox);
        zfDirectoryListingText->setObjectName(QString::fromUtf8("zfDirectoryListingText"));
        sizePolicy2.setHeightForWidth(zfDirectoryListingText->sizePolicy().hasHeightForWidth());
        zfDirectoryListingText->setSizePolicy(sizePolicy2);
        zfDirectoryListingText->setMinimumSize(QSize(0, 100));
        zfDirectoryListingText->setMaximumSize(QSize(16777215, 100));

        verticalLayout_10->addWidget(zfDirectoryListingText);


        verticalLayout_4->addWidget(zfDirectoryListingGroupbox);

        wwindDirectoryGroupbox = new QGroupBox(scrollAreaWidgetContents);
        wwindDirectoryGroupbox->setObjectName(QString::fromUtf8("wwindDirectoryGroupbox"));
        sizePolicy.setHeightForWidth(wwindDirectoryGroupbox->sizePolicy().hasHeightForWidth());
        wwindDirectoryGroupbox->setSizePolicy(sizePolicy);
        wwindDirectoryGroupbox->setMinimumSize(QSize(0, 55));
        wwindDirectoryGroupbox->setFlat(true);
        verticalLayout_11 = new QVBoxLayout(wwindDirectoryGroupbox);
        verticalLayout_11->setObjectName(QString::fromUtf8("verticalLayout_11"));
        wwindDirectoryText = new QLineEdit(wwindDirectoryGroupbox);
        wwindDirectoryText->setObjectName(QString::fromUtf8("wwindDirectoryText"));

        verticalLayout_11->addWidget(wwindDirectoryText);


        verticalLayout_4->addWidget(wwindDirectoryGroupbox);

        wwindDirectoryListingGroupbox = new QGroupBox(scrollAreaWidgetContents);
        wwindDirectoryListingGroupbox->setObjectName(QString::fromUtf8("wwindDirectoryListingGroupbox"));
        wwindDirectoryListingGroupbox->setMinimumSize(QSize(0, 130));
        wwindDirectoryListingGroupbox->setMaximumSize(QSize(16777215, 125));
        wwindDirectoryListingGroupbox->setFlat(true);
        verticalLayout_12 = new QVBoxLayout(wwindDirectoryListingGroupbox);
        verticalLayout_12->setObjectName(QString::fromUtf8("verticalLayout_12"));
        wwindDirectoryListingText = new QListWidget(wwindDirectoryListingGroupbox);
        wwindDirectoryListingText->setObjectName(QString::fromUtf8("wwindDirectoryListingText"));
        sizePolicy2.setHeightForWidth(wwindDirectoryListingText->sizePolicy().hasHeightForWidth());
        wwindDirectoryListingText->setSizePolicy(sizePolicy2);
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
        sizePolicy.setHeightForWidth(groupBox_3->sizePolicy().hasHeightForWidth());
        groupBox_3->setSizePolicy(sizePolicy);
        groupBox_3->setMinimumSize(QSize(0, 39));
        horizontalLayout = new QHBoxLayout(groupBox_3);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        readButton = new QPushButton(groupBox_3);
        readButton->setObjectName(QString::fromUtf8("readButton"));
        readButton->setMaximumSize(QSize(110, 16777215));

        horizontalLayout->addWidget(readButton);

        cancelButton = new QPushButton(groupBox_3);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));
        cancelButton->setMaximumSize(QSize(100, 16777215));

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
        htDirectoryListingGroupbox->setTitle(QApplication::translate("CMAQFileReader", "Optional: Select a subdirectory or GRIDCRO2D file (containing HT) for time-varying elevations:", 0, QApplication::UnicodeUTF8));
        zfDirectoryGroupbox->setTitle(QApplication::translate("CMAQFileReader", "Optional METCRO3D File Directory:", 0, QApplication::UnicodeUTF8));
        zfDirectoryListingGroupbox->setTitle(QApplication::translate("CMAQFileReader", "Optional: Select a subdirectory or METCRO3D file (containing ZF) for time-varying elevations:", 0, QApplication::UnicodeUTF8));
        wwindDirectoryGroupbox->setTitle(QApplication::translate("CMAQFileReader", "Optional METCRO3D/CONC File Directory:", 0, QApplication::UnicodeUTF8));
        wwindDirectoryListingGroupbox->setTitle(QApplication::translate("CMAQFileReader", "Optional: Select a subdirectory or METCRO3D/CONC file (containing WWIND or W_VEL) for 3D wind vectors:", 0, QApplication::UnicodeUTF8));
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
