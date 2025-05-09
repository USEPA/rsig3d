/********************************************************************************
** Form generated from reading UI file 'sessionwidget.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SESSIONWIDGET_H
#define UI_SESSIONWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SessionWidget
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *sessionIdLayout;
    QLabel *sessionIdLabel;
    QLabel *sessionId;
    QHBoxLayout *sessionStateLayout;
    QLabel *sessionStateLabel;
    QLabel *sessionState;
    QHBoxLayout *configurationLayout;
    QLabel *configurationLabel;
    QLabel *configuration;
    QHBoxLayout *bearerLayout;
    QLabel *bearerLabel;
    QLabel *bearer;
    QHBoxLayout *interfaceNameLayout;
    QLabel *interfaceNameLabel;
    QLabel *interfaceName;
    QHBoxLayout *interfaceGuidLayout;
    QLabel *interfaceGuidLabel;
    QLabel *interfaceGuid;
    QHBoxLayout *lastErrorLayout;
    QLabel *lastErrorLabel;
    QLabel *lastError;
    QHBoxLayout *errorStringLayout;
    QLabel *errorStringLabel;
    QLabel *errorString;
    QHBoxLayout *horizontalLayout_3;
    QLabel *rxData;
    QLabel *txData;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_3;
    QLabel *activeTime;
    QHBoxLayout *horizontalLayout;
    QPushButton *openSessionButton;
    QPushButton *openSyncSessionButton;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *closeSessionButton;
    QPushButton *stopSessionButton;

    void setupUi(QWidget *SessionWidget)
    {
        if (SessionWidget->objectName().isEmpty())
            SessionWidget->setObjectName(QString::fromUtf8("SessionWidget"));
        SessionWidget->resize(340, 276);
        verticalLayout_2 = new QVBoxLayout(SessionWidget);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        sessionIdLayout = new QHBoxLayout();
        sessionIdLayout->setObjectName(QString::fromUtf8("sessionIdLayout"));
        sessionIdLabel = new QLabel(SessionWidget);
        sessionIdLabel->setObjectName(QString::fromUtf8("sessionIdLabel"));

        sessionIdLayout->addWidget(sessionIdLabel);

        sessionId = new QLabel(SessionWidget);
        sessionId->setObjectName(QString::fromUtf8("sessionId"));
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(sessionId->sizePolicy().hasHeightForWidth());
        sessionId->setSizePolicy(sizePolicy);

        sessionIdLayout->addWidget(sessionId);


        verticalLayout->addLayout(sessionIdLayout);

        sessionStateLayout = new QHBoxLayout();
        sessionStateLayout->setObjectName(QString::fromUtf8("sessionStateLayout"));
        sessionStateLabel = new QLabel(SessionWidget);
        sessionStateLabel->setObjectName(QString::fromUtf8("sessionStateLabel"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(sessionStateLabel->sizePolicy().hasHeightForWidth());
        sessionStateLabel->setSizePolicy(sizePolicy1);

        sessionStateLayout->addWidget(sessionStateLabel);

        sessionState = new QLabel(SessionWidget);
        sessionState->setObjectName(QString::fromUtf8("sessionState"));
        sizePolicy.setHeightForWidth(sessionState->sizePolicy().hasHeightForWidth());
        sessionState->setSizePolicy(sizePolicy);

        sessionStateLayout->addWidget(sessionState);


        verticalLayout->addLayout(sessionStateLayout);

        configurationLayout = new QHBoxLayout();
        configurationLayout->setObjectName(QString::fromUtf8("configurationLayout"));
        configurationLabel = new QLabel(SessionWidget);
        configurationLabel->setObjectName(QString::fromUtf8("configurationLabel"));

        configurationLayout->addWidget(configurationLabel);

        configuration = new QLabel(SessionWidget);
        configuration->setObjectName(QString::fromUtf8("configuration"));
        sizePolicy.setHeightForWidth(configuration->sizePolicy().hasHeightForWidth());
        configuration->setSizePolicy(sizePolicy);

        configurationLayout->addWidget(configuration);


        verticalLayout->addLayout(configurationLayout);

        bearerLayout = new QHBoxLayout();
        bearerLayout->setObjectName(QString::fromUtf8("bearerLayout"));
        bearerLabel = new QLabel(SessionWidget);
        bearerLabel->setObjectName(QString::fromUtf8("bearerLabel"));

        bearerLayout->addWidget(bearerLabel);

        bearer = new QLabel(SessionWidget);
        bearer->setObjectName(QString::fromUtf8("bearer"));
        sizePolicy.setHeightForWidth(bearer->sizePolicy().hasHeightForWidth());
        bearer->setSizePolicy(sizePolicy);

        bearerLayout->addWidget(bearer);


        verticalLayout->addLayout(bearerLayout);

        interfaceNameLayout = new QHBoxLayout();
        interfaceNameLayout->setObjectName(QString::fromUtf8("interfaceNameLayout"));
        interfaceNameLabel = new QLabel(SessionWidget);
        interfaceNameLabel->setObjectName(QString::fromUtf8("interfaceNameLabel"));

        interfaceNameLayout->addWidget(interfaceNameLabel);

        interfaceName = new QLabel(SessionWidget);
        interfaceName->setObjectName(QString::fromUtf8("interfaceName"));
        sizePolicy.setHeightForWidth(interfaceName->sizePolicy().hasHeightForWidth());
        interfaceName->setSizePolicy(sizePolicy);

        interfaceNameLayout->addWidget(interfaceName);


        verticalLayout->addLayout(interfaceNameLayout);

        interfaceGuidLayout = new QHBoxLayout();
        interfaceGuidLayout->setObjectName(QString::fromUtf8("interfaceGuidLayout"));
        interfaceGuidLabel = new QLabel(SessionWidget);
        interfaceGuidLabel->setObjectName(QString::fromUtf8("interfaceGuidLabel"));

        interfaceGuidLayout->addWidget(interfaceGuidLabel);

        interfaceGuid = new QLabel(SessionWidget);
        interfaceGuid->setObjectName(QString::fromUtf8("interfaceGuid"));
        sizePolicy.setHeightForWidth(interfaceGuid->sizePolicy().hasHeightForWidth());
        interfaceGuid->setSizePolicy(sizePolicy);

        interfaceGuidLayout->addWidget(interfaceGuid);


        verticalLayout->addLayout(interfaceGuidLayout);

        lastErrorLayout = new QHBoxLayout();
        lastErrorLayout->setObjectName(QString::fromUtf8("lastErrorLayout"));
        lastErrorLabel = new QLabel(SessionWidget);
        lastErrorLabel->setObjectName(QString::fromUtf8("lastErrorLabel"));

        lastErrorLayout->addWidget(lastErrorLabel);

        lastError = new QLabel(SessionWidget);
        lastError->setObjectName(QString::fromUtf8("lastError"));
        sizePolicy.setHeightForWidth(lastError->sizePolicy().hasHeightForWidth());
        lastError->setSizePolicy(sizePolicy);

        lastErrorLayout->addWidget(lastError);


        verticalLayout->addLayout(lastErrorLayout);

        errorStringLayout = new QHBoxLayout();
        errorStringLayout->setObjectName(QString::fromUtf8("errorStringLayout"));
        errorStringLabel = new QLabel(SessionWidget);
        errorStringLabel->setObjectName(QString::fromUtf8("errorStringLabel"));

        errorStringLayout->addWidget(errorStringLabel);

        errorString = new QLabel(SessionWidget);
        errorString->setObjectName(QString::fromUtf8("errorString"));
        sizePolicy.setHeightForWidth(errorString->sizePolicy().hasHeightForWidth());
        errorString->setSizePolicy(sizePolicy);

        errorStringLayout->addWidget(errorString);


        verticalLayout->addLayout(errorStringLayout);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        rxData = new QLabel(SessionWidget);
        rxData->setObjectName(QString::fromUtf8("rxData"));
        rxData->setAlignment(Qt::AlignCenter);

        horizontalLayout_3->addWidget(rxData);

        txData = new QLabel(SessionWidget);
        txData->setObjectName(QString::fromUtf8("txData"));
        txData->setAlignment(Qt::AlignCenter);

        horizontalLayout_3->addWidget(txData);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_3 = new QLabel(SessionWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_4->addWidget(label_3);

        activeTime = new QLabel(SessionWidget);
        activeTime->setObjectName(QString::fromUtf8("activeTime"));
        sizePolicy.setHeightForWidth(activeTime->sizePolicy().hasHeightForWidth());
        activeTime->setSizePolicy(sizePolicy);

        horizontalLayout_4->addWidget(activeTime);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        openSessionButton = new QPushButton(SessionWidget);
        openSessionButton->setObjectName(QString::fromUtf8("openSessionButton"));

        horizontalLayout->addWidget(openSessionButton);

        openSyncSessionButton = new QPushButton(SessionWidget);
        openSyncSessionButton->setObjectName(QString::fromUtf8("openSyncSessionButton"));

        horizontalLayout->addWidget(openSyncSessionButton);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        closeSessionButton = new QPushButton(SessionWidget);
        closeSessionButton->setObjectName(QString::fromUtf8("closeSessionButton"));

        horizontalLayout_2->addWidget(closeSessionButton);

        stopSessionButton = new QPushButton(SessionWidget);
        stopSessionButton->setObjectName(QString::fromUtf8("stopSessionButton"));

        horizontalLayout_2->addWidget(stopSessionButton);


        verticalLayout->addLayout(horizontalLayout_2);


        verticalLayout_2->addLayout(verticalLayout);


        retranslateUi(SessionWidget);

        QMetaObject::connectSlotsByName(SessionWidget);
    } // setupUi

    void retranslateUi(QWidget *SessionWidget)
    {
        SessionWidget->setWindowTitle(QApplication::translate("SessionWidget", "Session Details", 0, QApplication::UnicodeUTF8));
        sessionIdLabel->setText(QApplication::translate("SessionWidget", "Session ID:", 0, QApplication::UnicodeUTF8));
        sessionId->setText(QString());
        sessionStateLabel->setText(QApplication::translate("SessionWidget", "Session State:", 0, QApplication::UnicodeUTF8));
        sessionState->setText(QApplication::translate("SessionWidget", "Invalid", 0, QApplication::UnicodeUTF8));
        configurationLabel->setText(QApplication::translate("SessionWidget", "Configuration:", 0, QApplication::UnicodeUTF8));
        configuration->setText(QString());
        bearerLabel->setText(QApplication::translate("SessionWidget", "Bearer:", 0, QApplication::UnicodeUTF8));
        bearer->setText(QString());
        interfaceNameLabel->setText(QApplication::translate("SessionWidget", "Interface Name:", 0, QApplication::UnicodeUTF8));
        interfaceName->setText(QString());
        interfaceGuidLabel->setText(QApplication::translate("SessionWidget", "Interface GUID:", 0, QApplication::UnicodeUTF8));
        interfaceGuid->setText(QString());
        lastErrorLabel->setText(QApplication::translate("SessionWidget", "Last Error:", 0, QApplication::UnicodeUTF8));
        lastError->setText(QString());
        errorStringLabel->setText(QApplication::translate("SessionWidget", "Error String:", 0, QApplication::UnicodeUTF8));
        errorString->setText(QString());
        rxData->setText(QApplication::translate("SessionWidget", "0", 0, QApplication::UnicodeUTF8));
        txData->setText(QApplication::translate("SessionWidget", "0", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("SessionWidget", "Active Time:", 0, QApplication::UnicodeUTF8));
        activeTime->setText(QApplication::translate("SessionWidget", "0 seconds", 0, QApplication::UnicodeUTF8));
        openSessionButton->setText(QApplication::translate("SessionWidget", "Open", 0, QApplication::UnicodeUTF8));
        openSyncSessionButton->setText(QApplication::translate("SessionWidget", "Blocking Open", 0, QApplication::UnicodeUTF8));
        closeSessionButton->setText(QApplication::translate("SessionWidget", "Close", 0, QApplication::UnicodeUTF8));
        stopSessionButton->setText(QApplication::translate("SessionWidget", "Stop", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SessionWidget: public Ui_SessionWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SESSIONWIDGET_H
