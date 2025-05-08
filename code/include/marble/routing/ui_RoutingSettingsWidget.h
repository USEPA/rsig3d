/********************************************************************************
** Form generated from reading UI file 'RoutingSettingsWidget.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ROUTINGSETTINGSWIDGET_H
#define UI_ROUTINGSETTINGSWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QListView>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RoutingSettingsWidget
{
public:
    QHBoxLayout *horizontalLayout;
    QListView *profilesList;
    QVBoxLayout *verticalLayout;
    QPushButton *addButton;
    QPushButton *configureButton;
    QPushButton *removeButton;
    QSpacerItem *verticalSpacer;
    QPushButton *moveUpButton;
    QPushButton *moveDownButton;

    void setupUi(QWidget *RoutingSettingsWidget)
    {
        if (RoutingSettingsWidget->objectName().isEmpty())
            RoutingSettingsWidget->setObjectName(QString::fromUtf8("RoutingSettingsWidget"));
        RoutingSettingsWidget->resize(400, 300);
        horizontalLayout = new QHBoxLayout(RoutingSettingsWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        profilesList = new QListView(RoutingSettingsWidget);
        profilesList->setObjectName(QString::fromUtf8("profilesList"));

        horizontalLayout->addWidget(profilesList);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        addButton = new QPushButton(RoutingSettingsWidget);
        addButton->setObjectName(QString::fromUtf8("addButton"));

        verticalLayout->addWidget(addButton);

        configureButton = new QPushButton(RoutingSettingsWidget);
        configureButton->setObjectName(QString::fromUtf8("configureButton"));
        configureButton->setEnabled(false);

        verticalLayout->addWidget(configureButton);

        removeButton = new QPushButton(RoutingSettingsWidget);
        removeButton->setObjectName(QString::fromUtf8("removeButton"));
        removeButton->setEnabled(false);

        verticalLayout->addWidget(removeButton);

        verticalSpacer = new QSpacerItem(20, 118, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        moveUpButton = new QPushButton(RoutingSettingsWidget);
        moveUpButton->setObjectName(QString::fromUtf8("moveUpButton"));
        moveUpButton->setEnabled(false);

        verticalLayout->addWidget(moveUpButton);

        moveDownButton = new QPushButton(RoutingSettingsWidget);
        moveDownButton->setObjectName(QString::fromUtf8("moveDownButton"));
        moveDownButton->setEnabled(false);

        verticalLayout->addWidget(moveDownButton);


        horizontalLayout->addLayout(verticalLayout);


        retranslateUi(RoutingSettingsWidget);

        QMetaObject::connectSlotsByName(RoutingSettingsWidget);
    } // setupUi

    void retranslateUi(QWidget *RoutingSettingsWidget)
    {
        addButton->setText(QApplication::translate("RoutingSettingsWidget", "&Add", 0, QApplication::UnicodeUTF8));
        configureButton->setText(QApplication::translate("RoutingSettingsWidget", "&Configure", 0, QApplication::UnicodeUTF8));
        removeButton->setText(QApplication::translate("RoutingSettingsWidget", "&Remove", 0, QApplication::UnicodeUTF8));
        moveUpButton->setText(QApplication::translate("RoutingSettingsWidget", "Move &Up", 0, QApplication::UnicodeUTF8));
        moveDownButton->setText(QApplication::translate("RoutingSettingsWidget", "Move &Down", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(RoutingSettingsWidget);
    } // retranslateUi

};

namespace Ui {
    class RoutingSettingsWidget: public Ui_RoutingSettingsWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ROUTINGSETTINGSWIDGET_H
