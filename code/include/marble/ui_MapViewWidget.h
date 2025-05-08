/********************************************************************************
** Form generated from reading UI file 'MapViewWidget.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAPVIEWWIDGET_H
#define UI_MAPVIEWWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QFrame>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QListView>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MapViewWidget
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *projectionLabel_2;
    QComboBox *projectionComboBox;
    QFrame *line;
    QLabel *celestialBodyLabel;
    QComboBox *celestialBodyComboBox;
    QSpacerItem *spacer;
    QLabel *mapThemeLabel;
    QListView *marbleThemeSelectView;
    QComboBox *mapThemeComboBox;

    void setupUi(QWidget *MapViewWidget)
    {
        if (MapViewWidget->objectName().isEmpty())
            MapViewWidget->setObjectName(QString::fromUtf8("MapViewWidget"));
        MapViewWidget->resize(222, 426);
        verticalLayout = new QVBoxLayout(MapViewWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        projectionLabel_2 = new QLabel(MapViewWidget);
        projectionLabel_2->setObjectName(QString::fromUtf8("projectionLabel_2"));

        verticalLayout->addWidget(projectionLabel_2);

        projectionComboBox = new QComboBox(MapViewWidget);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/map-globe.png"), QSize(), QIcon::Normal, QIcon::Off);
        projectionComboBox->addItem(icon, QString());
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/map-flat.png"), QSize(), QIcon::Normal, QIcon::Off);
        projectionComboBox->addItem(icon1, QString());
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/icons/map-mercator.png"), QSize(), QIcon::Normal, QIcon::Off);
        projectionComboBox->addItem(icon2, QString());
        projectionComboBox->setObjectName(QString::fromUtf8("projectionComboBox"));
        projectionComboBox->setEnabled(false);

        verticalLayout->addWidget(projectionComboBox);

        line = new QFrame(MapViewWidget);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(line);

        celestialBodyLabel = new QLabel(MapViewWidget);
        celestialBodyLabel->setObjectName(QString::fromUtf8("celestialBodyLabel"));
        celestialBodyLabel->setEnabled(true);

        verticalLayout->addWidget(celestialBodyLabel);

        celestialBodyComboBox = new QComboBox(MapViewWidget);
        celestialBodyComboBox->setObjectName(QString::fromUtf8("celestialBodyComboBox"));
        celestialBodyComboBox->setEnabled(true);

        verticalLayout->addWidget(celestialBodyComboBox);

        spacer = new QSpacerItem(164, 7, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout->addItem(spacer);

        mapThemeLabel = new QLabel(MapViewWidget);
        mapThemeLabel->setObjectName(QString::fromUtf8("mapThemeLabel"));

        verticalLayout->addWidget(mapThemeLabel);

        marbleThemeSelectView = new QListView(MapViewWidget);
        marbleThemeSelectView->setObjectName(QString::fromUtf8("marbleThemeSelectView"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(marbleThemeSelectView->sizePolicy().hasHeightForWidth());
        marbleThemeSelectView->setSizePolicy(sizePolicy);
        marbleThemeSelectView->setMinimumSize(QSize(0, 140));
        marbleThemeSelectView->setContextMenuPolicy(Qt::CustomContextMenu);
        marbleThemeSelectView->setAutoFillBackground(true);

        verticalLayout->addWidget(marbleThemeSelectView);

        mapThemeComboBox = new QComboBox(MapViewWidget);
        mapThemeComboBox->setObjectName(QString::fromUtf8("mapThemeComboBox"));

        verticalLayout->addWidget(mapThemeComboBox);

#ifndef QT_NO_SHORTCUT
        projectionLabel_2->setBuddy(projectionComboBox);
        celestialBodyLabel->setBuddy(projectionComboBox);
        mapThemeLabel->setBuddy(marbleThemeSelectView);
#endif // QT_NO_SHORTCUT

        retranslateUi(MapViewWidget);

        QMetaObject::connectSlotsByName(MapViewWidget);
    } // setupUi

    void retranslateUi(QWidget *MapViewWidget)
    {
        MapViewWidget->setWindowTitle(QApplication::translate("MapViewWidget", "Map View", 0, QApplication::UnicodeUTF8));
        projectionLabel_2->setText(QApplication::translate("MapViewWidget", "&Projection", 0, QApplication::UnicodeUTF8));
        projectionComboBox->setItemText(0, QApplication::translate("MapViewWidget", "Globe", 0, QApplication::UnicodeUTF8));
        projectionComboBox->setItemText(1, QApplication::translate("MapViewWidget", "Flat Map", 0, QApplication::UnicodeUTF8));
        projectionComboBox->setItemText(2, QApplication::translate("MapViewWidget", "Mercator", 0, QApplication::UnicodeUTF8));

        celestialBodyLabel->setText(QApplication::translate("MapViewWidget", "&Celestial Body", 0, QApplication::UnicodeUTF8));
        mapThemeLabel->setText(QApplication::translate("MapViewWidget", "&Theme", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_WHATSTHIS
        marbleThemeSelectView->setWhatsThis(QApplication::translate("MapViewWidget", "Here you can choose the preferred map view from different topics.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
    } // retranslateUi

};

namespace Ui {
    class MapViewWidget: public Ui_MapViewWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAPVIEWWIDGET_H
