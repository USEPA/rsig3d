/********************************************************************************
** Form generated from reading UI file 'RoutingProfileSettingsDialog.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ROUTINGPROFILESETTINGSDIALOG_H
#define UI_ROUTINGPROFILESETTINGSDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QFormLayout>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListView>
#include <QtGui/QPushButton>
#include <QtGui/QStackedWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RoutingProfileSettingsDialog
{
public:
    QGridLayout *gridLayout;
    QFormLayout *formLayout;
    QLabel *label;
    QLineEdit *name;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_4;
    QListView *services;
    QVBoxLayout *verticalLayout;
    QLabel *label_5;
    QStackedWidget *settingsStack;
    QWidget *noConfigAvailablePage;
    QVBoxLayout *verticalLayout_3;
    QLabel *noConfigDescriptionLabel;
    QLabel *noConfigStatusLabel;
    QLabel *label_2;
    QWidget *selectServicePage;
    QGridLayout *gridLayout_3;
    QLabel *label_3;
    QWidget *configurePage;
    QGridLayout *gridLayout_4;
    QPushButton *configureButton;
    QLabel *descriptionLabel;
    QLabel *statusLabel;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *RoutingProfileSettingsDialog)
    {
        if (RoutingProfileSettingsDialog->objectName().isEmpty())
            RoutingProfileSettingsDialog->setObjectName(QString::fromUtf8("RoutingProfileSettingsDialog"));
        RoutingProfileSettingsDialog->resize(488, 274);
        gridLayout = new QGridLayout(RoutingProfileSettingsDialog);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        formLayout = new QFormLayout();
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
        label = new QLabel(RoutingProfileSettingsDialog);
        label->setObjectName(QString::fromUtf8("label"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label);

        name = new QLineEdit(RoutingProfileSettingsDialog);
        name->setObjectName(QString::fromUtf8("name"));

        formLayout->setWidget(0, QFormLayout::FieldRole, name);


        gridLayout->addLayout(formLayout, 0, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        label_4 = new QLabel(RoutingProfileSettingsDialog);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        verticalLayout_2->addWidget(label_4);

        services = new QListView(RoutingProfileSettingsDialog);
        services->setObjectName(QString::fromUtf8("services"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(services->sizePolicy().hasHeightForWidth());
        services->setSizePolicy(sizePolicy);
        services->setResizeMode(QListView::Adjust);

        verticalLayout_2->addWidget(services);


        horizontalLayout->addLayout(verticalLayout_2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label_5 = new QLabel(RoutingProfileSettingsDialog);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        verticalLayout->addWidget(label_5);

        settingsStack = new QStackedWidget(RoutingProfileSettingsDialog);
        settingsStack->setObjectName(QString::fromUtf8("settingsStack"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(settingsStack->sizePolicy().hasHeightForWidth());
        settingsStack->setSizePolicy(sizePolicy1);
        noConfigAvailablePage = new QWidget();
        noConfigAvailablePage->setObjectName(QString::fromUtf8("noConfigAvailablePage"));
        verticalLayout_3 = new QVBoxLayout(noConfigAvailablePage);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        noConfigDescriptionLabel = new QLabel(noConfigAvailablePage);
        noConfigDescriptionLabel->setObjectName(QString::fromUtf8("noConfigDescriptionLabel"));
        sizePolicy1.setHeightForWidth(noConfigDescriptionLabel->sizePolicy().hasHeightForWidth());
        noConfigDescriptionLabel->setSizePolicy(sizePolicy1);
        noConfigDescriptionLabel->setWordWrap(true);

        verticalLayout_3->addWidget(noConfigDescriptionLabel);

        noConfigStatusLabel = new QLabel(noConfigAvailablePage);
        noConfigStatusLabel->setObjectName(QString::fromUtf8("noConfigStatusLabel"));
        sizePolicy1.setHeightForWidth(noConfigStatusLabel->sizePolicy().hasHeightForWidth());
        noConfigStatusLabel->setSizePolicy(sizePolicy1);
        noConfigStatusLabel->setWordWrap(true);

        verticalLayout_3->addWidget(noConfigStatusLabel);

        label_2 = new QLabel(noConfigAvailablePage);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        sizePolicy1.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy1);

        verticalLayout_3->addWidget(label_2);

        settingsStack->addWidget(noConfigAvailablePage);
        selectServicePage = new QWidget();
        selectServicePage->setObjectName(QString::fromUtf8("selectServicePage"));
        gridLayout_3 = new QGridLayout(selectServicePage);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        label_3 = new QLabel(selectServicePage);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setWordWrap(true);

        gridLayout_3->addWidget(label_3, 0, 0, 1, 1);

        settingsStack->addWidget(selectServicePage);
        configurePage = new QWidget();
        configurePage->setObjectName(QString::fromUtf8("configurePage"));
        gridLayout_4 = new QGridLayout(configurePage);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        configureButton = new QPushButton(configurePage);
        configureButton->setObjectName(QString::fromUtf8("configureButton"));

        gridLayout_4->addWidget(configureButton, 2, 1, 1, 1);

        descriptionLabel = new QLabel(configurePage);
        descriptionLabel->setObjectName(QString::fromUtf8("descriptionLabel"));
        descriptionLabel->setWordWrap(true);

        gridLayout_4->addWidget(descriptionLabel, 0, 1, 1, 1);

        statusLabel = new QLabel(configurePage);
        statusLabel->setObjectName(QString::fromUtf8("statusLabel"));
        statusLabel->setWordWrap(true);

        gridLayout_4->addWidget(statusLabel, 1, 1, 1, 1);

        settingsStack->addWidget(configurePage);

        verticalLayout->addWidget(settingsStack);


        horizontalLayout->addLayout(verticalLayout);


        gridLayout->addLayout(horizontalLayout, 1, 0, 1, 1);

        buttonBox = new QDialogButtonBox(RoutingProfileSettingsDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        gridLayout->addWidget(buttonBox, 2, 0, 1, 1);


        retranslateUi(RoutingProfileSettingsDialog);

        settingsStack->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(RoutingProfileSettingsDialog);
    } // setupUi

    void retranslateUi(QDialog *RoutingProfileSettingsDialog)
    {
        RoutingProfileSettingsDialog->setWindowTitle(QApplication::translate("RoutingProfileSettingsDialog", "Configure Routing Profile", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("RoutingProfileSettingsDialog", "Name:", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("RoutingProfileSettingsDialog", "Active Route Services", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("RoutingProfileSettingsDialog", "Service Settings", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("RoutingProfileSettingsDialog", "No configuration available", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("RoutingProfileSettingsDialog", "Select Service to Configure", 0, QApplication::UnicodeUTF8));
        configureButton->setText(QApplication::translate("RoutingProfileSettingsDialog", "Configure", 0, QApplication::UnicodeUTF8));
        descriptionLabel->setText(QString());
        statusLabel->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class RoutingProfileSettingsDialog: public Ui_RoutingProfileSettingsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ROUTINGPROFILESETTINGSDIALOG_H
