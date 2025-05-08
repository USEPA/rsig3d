/********************************************************************************
** Form generated from reading UI file 'RoutingWidget.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ROUTINGWIDGET_H
#define UI_ROUTINGWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QListView>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RoutingWidget
{
public:
    QVBoxLayout *routingLayout;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *addViaButton;
    QPushButton *reverseRouteButton;
    QPushButton *clearRouteButton;
    QHBoxLayout *horizontalLayout;
    QComboBox *routingProfileComboBox;
    QSpacerItem *horizontalSpacer;
    QToolButton *configureButton;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *searchButton;
    QSpacerItem *horizontalSpacer_2;
    QLabel *resultLabel;
    QPushButton *showInstructionsButton;
    QListView *directionsListView;
    QHBoxLayout *horizontalLayout_3;
    QComboBox *routeComboBox;
    QToolButton *openRouteButton;
    QToolButton *saveRouteButton;

    void setupUi(QWidget *RoutingWidget)
    {
        if (RoutingWidget->objectName().isEmpty())
            RoutingWidget->setObjectName(QString::fromUtf8("RoutingWidget"));
        RoutingWidget->resize(245, 414);
        routingLayout = new QVBoxLayout(RoutingWidget);
        routingLayout->setObjectName(QString::fromUtf8("routingLayout"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        addViaButton = new QPushButton(RoutingWidget);
        addViaButton->setObjectName(QString::fromUtf8("addViaButton"));
        addViaButton->setCheckable(false);

        horizontalLayout_4->addWidget(addViaButton);

        reverseRouteButton = new QPushButton(RoutingWidget);
        reverseRouteButton->setObjectName(QString::fromUtf8("reverseRouteButton"));
        reverseRouteButton->setCheckable(false);

        horizontalLayout_4->addWidget(reverseRouteButton);

        clearRouteButton = new QPushButton(RoutingWidget);
        clearRouteButton->setObjectName(QString::fromUtf8("clearRouteButton"));
        clearRouteButton->setCheckable(false);

        horizontalLayout_4->addWidget(clearRouteButton);


        routingLayout->addLayout(horizontalLayout_4);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        routingProfileComboBox = new QComboBox(RoutingWidget);
        routingProfileComboBox->setObjectName(QString::fromUtf8("routingProfileComboBox"));

        horizontalLayout->addWidget(routingProfileComboBox);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        configureButton = new QToolButton(RoutingWidget);
        configureButton->setObjectName(QString::fromUtf8("configureButton"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/16x16/configure.png"), QSize(), QIcon::Normal, QIcon::Off);
        configureButton->setIcon(icon);

        horizontalLayout->addWidget(configureButton);


        routingLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        searchButton = new QPushButton(RoutingWidget);
        searchButton->setObjectName(QString::fromUtf8("searchButton"));

        horizontalLayout_2->addWidget(searchButton);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        resultLabel = new QLabel(RoutingWidget);
        resultLabel->setObjectName(QString::fromUtf8("resultLabel"));

        horizontalLayout_2->addWidget(resultLabel);

        showInstructionsButton = new QPushButton(RoutingWidget);
        showInstructionsButton->setObjectName(QString::fromUtf8("showInstructionsButton"));
        showInstructionsButton->setCheckable(false);

        horizontalLayout_2->addWidget(showInstructionsButton);


        routingLayout->addLayout(horizontalLayout_2);

        directionsListView = new QListView(RoutingWidget);
        directionsListView->setObjectName(QString::fromUtf8("directionsListView"));
        directionsListView->setMinimumSize(QSize(0, 200));
        directionsListView->setAlternatingRowColors(true);
        directionsListView->setResizeMode(QListView::Adjust);
        directionsListView->setWordWrap(true);

        routingLayout->addWidget(directionsListView);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        routeComboBox = new QComboBox(RoutingWidget);
        routeComboBox->setObjectName(QString::fromUtf8("routeComboBox"));

        horizontalLayout_3->addWidget(routeComboBox);

        openRouteButton = new QToolButton(RoutingWidget);
        openRouteButton->setObjectName(QString::fromUtf8("openRouteButton"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/16x16/document-open.png"), QSize(), QIcon::Normal, QIcon::Off);
        openRouteButton->setIcon(icon1);

        horizontalLayout_3->addWidget(openRouteButton);

        saveRouteButton = new QToolButton(RoutingWidget);
        saveRouteButton->setObjectName(QString::fromUtf8("saveRouteButton"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/icons/16x16/document-save.png"), QSize(), QIcon::Normal, QIcon::Off);
        saveRouteButton->setIcon(icon2);

        horizontalLayout_3->addWidget(saveRouteButton);


        routingLayout->addLayout(horizontalLayout_3);


        retranslateUi(RoutingWidget);

        QMetaObject::connectSlotsByName(RoutingWidget);
    } // setupUi

    void retranslateUi(QWidget *RoutingWidget)
    {
        addViaButton->setText(QApplication::translate("RoutingWidget", "Add Via", 0, QApplication::UnicodeUTF8));
        reverseRouteButton->setText(QApplication::translate("RoutingWidget", "Reverse", 0, QApplication::UnicodeUTF8));
        clearRouteButton->setText(QApplication::translate("RoutingWidget", "Clear", 0, QApplication::UnicodeUTF8));
        routingProfileComboBox->clear();
        routingProfileComboBox->insertItems(0, QStringList()
         << QApplication::translate("RoutingWidget", "Car (fastest)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("RoutingWidget", "Car (shortest)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("RoutingWidget", "Bicycle", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("RoutingWidget", "Pedestrian", 0, QApplication::UnicodeUTF8)
        );
        configureButton->setText(QString());
        searchButton->setText(QApplication::translate("RoutingWidget", "Search", 0, QApplication::UnicodeUTF8));
        resultLabel->setText(QApplication::translate("RoutingWidget", "No results.", 0, QApplication::UnicodeUTF8));
        showInstructionsButton->setText(QApplication::translate("RoutingWidget", "Show Details", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        routeComboBox->setToolTip(QApplication::translate("RoutingWidget", "Choose alternative routes", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        openRouteButton->setText(QString());
        saveRouteButton->setText(QString());
        Q_UNUSED(RoutingWidget);
    } // retranslateUi

};

namespace Ui {
    class RoutingWidget: public Ui_RoutingWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ROUTINGWIDGET_H
