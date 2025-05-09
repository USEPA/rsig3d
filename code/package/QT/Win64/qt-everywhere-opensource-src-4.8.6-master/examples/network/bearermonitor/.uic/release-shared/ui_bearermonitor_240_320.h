/********************************************************************************
** Form generated from reading UI file 'bearermonitor_240_320.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BEARERMONITOR_240_320_H
#define UI_BEARERMONITOR_240_320_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QScrollArea>
#include <QtGui/QSpacerItem>
#include <QtGui/QTabWidget>
#include <QtGui/QTreeWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_BearerMonitor
{
public:
    QVBoxLayout *verticalLayout_5;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QVBoxLayout *verticalLayout_2;
    QGroupBox *systemState;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *onlineStateLayout;
    QLabel *onlineStateLabel;
    QLabel *onlineState;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout_9;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *configurationNameLayout;
    QLabel *configurationNameLabel;
    QLabel *configurationName;
    QHBoxLayout *configurationStateLayout;
    QLabel *configurationStateLabel;
    QLabel *configurationState;
    QHBoxLayout *configurationTypeLayout;
    QLabel *configurationTypeLabel;
    QLabel *configurationType;
    QHBoxLayout *configurationPurposeLayout;
    QLabel *configurationPurposeLabel;
    QLabel *configurationPurpose;
    QHBoxLayout *configurationIdentifierLayout;
    QLabel *configurationIdentifierLabel;
    QLabel *configurationIdentifier;
    QHBoxLayout *configurationRoamingLayout;
    QLabel *configurationRoamingLabel;
    QLabel *configurationRoaming;
    QHBoxLayout *configurationChildrenLayout;
    QLabel *configurationChildrenLabel;
    QLabel *configurationChildren;
    QGroupBox *nlaGroup;
    QHBoxLayout *horizontalLayout;
    QPushButton *registerButton;
    QPushButton *unregisterButton;
    QPushButton *newSessionButton;
    QPushButton *deleteSessionButton;
    QPushButton *scanButton;
    QProgressBar *progressBar;
    QSpacerItem *verticalSpacer;
    QTreeWidget *treeWidget;
    QGroupBox *sessionGroup;
    QVBoxLayout *verticalLayout_3;
    QTabWidget *tabWidget;
    QWidget *tab;

    void setupUi(QWidget *BearerMonitor)
    {
        if (BearerMonitor->objectName().isEmpty())
            BearerMonitor->setObjectName(QString::fromUtf8("BearerMonitor"));
        BearerMonitor->resize(240, 320);
        verticalLayout_5 = new QVBoxLayout(BearerMonitor);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        scrollArea = new QScrollArea(BearerMonitor);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setFrameShape(QFrame::NoFrame);
        scrollArea->setFrameShadow(QFrame::Plain);
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, -274, 206, 576));
        verticalLayout_2 = new QVBoxLayout(scrollAreaWidgetContents);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        systemState = new QGroupBox(scrollAreaWidgetContents);
        systemState->setObjectName(QString::fromUtf8("systemState"));
        systemState->setFlat(true);
        verticalLayout_4 = new QVBoxLayout(systemState);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(0, -1, 0, 0);
        onlineStateLayout = new QHBoxLayout();
        onlineStateLayout->setObjectName(QString::fromUtf8("onlineStateLayout"));
        onlineStateLabel = new QLabel(systemState);
        onlineStateLabel->setObjectName(QString::fromUtf8("onlineStateLabel"));

        onlineStateLayout->addWidget(onlineStateLabel);

        onlineState = new QLabel(systemState);
        onlineState->setObjectName(QString::fromUtf8("onlineState"));
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(onlineState->sizePolicy().hasHeightForWidth());
        onlineState->setSizePolicy(sizePolicy);

        onlineStateLayout->addWidget(onlineState);


        verticalLayout_4->addLayout(onlineStateLayout);


        verticalLayout_2->addWidget(systemState);

        groupBox = new QGroupBox(scrollAreaWidgetContents);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setFlat(true);
        horizontalLayout_9 = new QHBoxLayout(groupBox);
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        horizontalLayout_9->setContentsMargins(0, -1, 0, 0);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        configurationNameLayout = new QHBoxLayout();
        configurationNameLayout->setObjectName(QString::fromUtf8("configurationNameLayout"));
        configurationNameLabel = new QLabel(groupBox);
        configurationNameLabel->setObjectName(QString::fromUtf8("configurationNameLabel"));

        configurationNameLayout->addWidget(configurationNameLabel);

        configurationName = new QLabel(groupBox);
        configurationName->setObjectName(QString::fromUtf8("configurationName"));
        sizePolicy.setHeightForWidth(configurationName->sizePolicy().hasHeightForWidth());
        configurationName->setSizePolicy(sizePolicy);

        configurationNameLayout->addWidget(configurationName);


        verticalLayout->addLayout(configurationNameLayout);

        configurationStateLayout = new QHBoxLayout();
        configurationStateLayout->setObjectName(QString::fromUtf8("configurationStateLayout"));
        configurationStateLabel = new QLabel(groupBox);
        configurationStateLabel->setObjectName(QString::fromUtf8("configurationStateLabel"));

        configurationStateLayout->addWidget(configurationStateLabel);

        configurationState = new QLabel(groupBox);
        configurationState->setObjectName(QString::fromUtf8("configurationState"));
        sizePolicy.setHeightForWidth(configurationState->sizePolicy().hasHeightForWidth());
        configurationState->setSizePolicy(sizePolicy);

        configurationStateLayout->addWidget(configurationState);


        verticalLayout->addLayout(configurationStateLayout);

        configurationTypeLayout = new QHBoxLayout();
        configurationTypeLayout->setObjectName(QString::fromUtf8("configurationTypeLayout"));
        configurationTypeLabel = new QLabel(groupBox);
        configurationTypeLabel->setObjectName(QString::fromUtf8("configurationTypeLabel"));

        configurationTypeLayout->addWidget(configurationTypeLabel);

        configurationType = new QLabel(groupBox);
        configurationType->setObjectName(QString::fromUtf8("configurationType"));
        sizePolicy.setHeightForWidth(configurationType->sizePolicy().hasHeightForWidth());
        configurationType->setSizePolicy(sizePolicy);

        configurationTypeLayout->addWidget(configurationType);


        verticalLayout->addLayout(configurationTypeLayout);

        configurationPurposeLayout = new QHBoxLayout();
        configurationPurposeLayout->setObjectName(QString::fromUtf8("configurationPurposeLayout"));
        configurationPurposeLabel = new QLabel(groupBox);
        configurationPurposeLabel->setObjectName(QString::fromUtf8("configurationPurposeLabel"));

        configurationPurposeLayout->addWidget(configurationPurposeLabel);

        configurationPurpose = new QLabel(groupBox);
        configurationPurpose->setObjectName(QString::fromUtf8("configurationPurpose"));
        sizePolicy.setHeightForWidth(configurationPurpose->sizePolicy().hasHeightForWidth());
        configurationPurpose->setSizePolicy(sizePolicy);

        configurationPurposeLayout->addWidget(configurationPurpose);


        verticalLayout->addLayout(configurationPurposeLayout);

        configurationIdentifierLayout = new QHBoxLayout();
        configurationIdentifierLayout->setObjectName(QString::fromUtf8("configurationIdentifierLayout"));
        configurationIdentifierLabel = new QLabel(groupBox);
        configurationIdentifierLabel->setObjectName(QString::fromUtf8("configurationIdentifierLabel"));

        configurationIdentifierLayout->addWidget(configurationIdentifierLabel);

        configurationIdentifier = new QLabel(groupBox);
        configurationIdentifier->setObjectName(QString::fromUtf8("configurationIdentifier"));
        sizePolicy.setHeightForWidth(configurationIdentifier->sizePolicy().hasHeightForWidth());
        configurationIdentifier->setSizePolicy(sizePolicy);

        configurationIdentifierLayout->addWidget(configurationIdentifier);


        verticalLayout->addLayout(configurationIdentifierLayout);

        configurationRoamingLayout = new QHBoxLayout();
        configurationRoamingLayout->setObjectName(QString::fromUtf8("configurationRoamingLayout"));
        configurationRoamingLabel = new QLabel(groupBox);
        configurationRoamingLabel->setObjectName(QString::fromUtf8("configurationRoamingLabel"));

        configurationRoamingLayout->addWidget(configurationRoamingLabel);

        configurationRoaming = new QLabel(groupBox);
        configurationRoaming->setObjectName(QString::fromUtf8("configurationRoaming"));
        sizePolicy.setHeightForWidth(configurationRoaming->sizePolicy().hasHeightForWidth());
        configurationRoaming->setSizePolicy(sizePolicy);

        configurationRoamingLayout->addWidget(configurationRoaming);


        verticalLayout->addLayout(configurationRoamingLayout);

        configurationChildrenLayout = new QHBoxLayout();
        configurationChildrenLayout->setObjectName(QString::fromUtf8("configurationChildrenLayout"));
        configurationChildrenLabel = new QLabel(groupBox);
        configurationChildrenLabel->setObjectName(QString::fromUtf8("configurationChildrenLabel"));

        configurationChildrenLayout->addWidget(configurationChildrenLabel);

        configurationChildren = new QLabel(groupBox);
        configurationChildren->setObjectName(QString::fromUtf8("configurationChildren"));
        sizePolicy.setHeightForWidth(configurationChildren->sizePolicy().hasHeightForWidth());
        configurationChildren->setSizePolicy(sizePolicy);

        configurationChildrenLayout->addWidget(configurationChildren);


        verticalLayout->addLayout(configurationChildrenLayout);

        nlaGroup = new QGroupBox(groupBox);
        nlaGroup->setObjectName(QString::fromUtf8("nlaGroup"));
        horizontalLayout = new QHBoxLayout(nlaGroup);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        registerButton = new QPushButton(nlaGroup);
        registerButton->setObjectName(QString::fromUtf8("registerButton"));

        horizontalLayout->addWidget(registerButton);

        unregisterButton = new QPushButton(nlaGroup);
        unregisterButton->setObjectName(QString::fromUtf8("unregisterButton"));

        horizontalLayout->addWidget(unregisterButton);


        verticalLayout->addWidget(nlaGroup);

        newSessionButton = new QPushButton(groupBox);
        newSessionButton->setObjectName(QString::fromUtf8("newSessionButton"));

        verticalLayout->addWidget(newSessionButton);

        deleteSessionButton = new QPushButton(groupBox);
        deleteSessionButton->setObjectName(QString::fromUtf8("deleteSessionButton"));

        verticalLayout->addWidget(deleteSessionButton);

        scanButton = new QPushButton(groupBox);
        scanButton->setObjectName(QString::fromUtf8("scanButton"));

        verticalLayout->addWidget(scanButton);

        progressBar = new QProgressBar(groupBox);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setMaximum(0);
        progressBar->setValue(-1);
        progressBar->setTextVisible(false);
        progressBar->setInvertedAppearance(false);

        verticalLayout->addWidget(progressBar);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        treeWidget = new QTreeWidget(groupBox);
        treeWidget->setObjectName(QString::fromUtf8("treeWidget"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(treeWidget->sizePolicy().hasHeightForWidth());
        treeWidget->setSizePolicy(sizePolicy1);
        treeWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        treeWidget->header()->setVisible(false);

        verticalLayout->addWidget(treeWidget);


        horizontalLayout_9->addLayout(verticalLayout);


        verticalLayout_2->addWidget(groupBox);

        sessionGroup = new QGroupBox(scrollAreaWidgetContents);
        sessionGroup->setObjectName(QString::fromUtf8("sessionGroup"));
        sessionGroup->setFlat(true);
        verticalLayout_3 = new QVBoxLayout(sessionGroup);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, -1, 0, 0);
        tabWidget = new QTabWidget(sessionGroup);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(tabWidget->sizePolicy().hasHeightForWidth());
        tabWidget->setSizePolicy(sizePolicy2);
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        tabWidget->addTab(tab, QString());

        verticalLayout_3->addWidget(tabWidget);


        verticalLayout_2->addWidget(sessionGroup);

        scrollArea->setWidget(scrollAreaWidgetContents);

        verticalLayout_5->addWidget(scrollArea);


        retranslateUi(BearerMonitor);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(BearerMonitor);
    } // setupUi

    void retranslateUi(QWidget *BearerMonitor)
    {
        BearerMonitor->setWindowTitle(QApplication::translate("BearerMonitor", "BearerMonitor", 0, QApplication::UnicodeUTF8));
        systemState->setTitle(QApplication::translate("BearerMonitor", "System State", 0, QApplication::UnicodeUTF8));
        onlineStateLabel->setText(QApplication::translate("BearerMonitor", "Online State:", 0, QApplication::UnicodeUTF8));
        onlineState->setText(QString());
        groupBox->setTitle(QApplication::translate("BearerMonitor", "Configurations", 0, QApplication::UnicodeUTF8));
        configurationNameLabel->setText(QApplication::translate("BearerMonitor", "Name:", 0, QApplication::UnicodeUTF8));
        configurationName->setText(QString());
        configurationStateLabel->setText(QApplication::translate("BearerMonitor", "State:", 0, QApplication::UnicodeUTF8));
        configurationState->setText(QString());
        configurationTypeLabel->setText(QApplication::translate("BearerMonitor", "Type:", 0, QApplication::UnicodeUTF8));
        configurationType->setText(QApplication::translate("BearerMonitor", "Invalid", 0, QApplication::UnicodeUTF8));
        configurationPurposeLabel->setText(QApplication::translate("BearerMonitor", "Purpose:", 0, QApplication::UnicodeUTF8));
        configurationPurpose->setText(QApplication::translate("BearerMonitor", "Unknown", 0, QApplication::UnicodeUTF8));
        configurationIdentifierLabel->setText(QApplication::translate("BearerMonitor", "Identifier:", 0, QApplication::UnicodeUTF8));
        configurationIdentifier->setText(QString());
        configurationRoamingLabel->setText(QApplication::translate("BearerMonitor", "Roaming:", 0, QApplication::UnicodeUTF8));
        configurationRoaming->setText(QString());
        configurationChildrenLabel->setText(QApplication::translate("BearerMonitor", "Children:", 0, QApplication::UnicodeUTF8));
        configurationChildren->setText(QString());
        nlaGroup->setTitle(QApplication::translate("BearerMonitor", "Network Location Awareness", 0, QApplication::UnicodeUTF8));
        registerButton->setText(QApplication::translate("BearerMonitor", "Register", 0, QApplication::UnicodeUTF8));
        unregisterButton->setText(QApplication::translate("BearerMonitor", "Unregister", 0, QApplication::UnicodeUTF8));
        newSessionButton->setText(QApplication::translate("BearerMonitor", "New Session", 0, QApplication::UnicodeUTF8));
        deleteSessionButton->setText(QApplication::translate("BearerMonitor", "Delete Session", 0, QApplication::UnicodeUTF8));
        scanButton->setText(QApplication::translate("BearerMonitor", "Scan", 0, QApplication::UnicodeUTF8));
        progressBar->setFormat(QApplication::translate("BearerMonitor", "%p%", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem = treeWidget->headerItem();
        ___qtreewidgetitem->setText(0, QApplication::translate("BearerMonitor", "1", 0, QApplication::UnicodeUTF8));
        sessionGroup->setTitle(QApplication::translate("BearerMonitor", "Sessions", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("BearerMonitor", "Session 1", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class BearerMonitor: public Ui_BearerMonitor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BEARERMONITOR_240_320_H
