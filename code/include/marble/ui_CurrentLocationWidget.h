/********************************************************************************
** Form generated from reading UI file 'CurrentLocationWidget.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CURRENTLOCATIONWIDGET_H
#define UI_CURRENTLOCATIONWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QSpacerItem>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CurrentLocationWidget
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *projectionLabel_3;
    QComboBox *positionTrackingComboBox;
    QHBoxLayout *horizontalLayout;
    QCheckBox *showTrackCheckBox;
    QToolButton *clearTrackButton;
    QToolButton *openTrackButton;
    QToolButton *saveTrackButton;
    QLabel *locationLabel;
    QSpacerItem *verticalSpacer;
    QLabel *recenterLabel;
    QComboBox *recenterComboBox;
    QCheckBox *autoZoomCheckBox;

    void setupUi(QWidget *CurrentLocationWidget)
    {
        if (CurrentLocationWidget->objectName().isEmpty())
            CurrentLocationWidget->setObjectName(QString::fromUtf8("CurrentLocationWidget"));
        CurrentLocationWidget->resize(259, 268);
        verticalLayout = new QVBoxLayout(CurrentLocationWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        projectionLabel_3 = new QLabel(CurrentLocationWidget);
        projectionLabel_3->setObjectName(QString::fromUtf8("projectionLabel_3"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(projectionLabel_3->sizePolicy().hasHeightForWidth());
        projectionLabel_3->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(projectionLabel_3);

        positionTrackingComboBox = new QComboBox(CurrentLocationWidget);
        positionTrackingComboBox->setObjectName(QString::fromUtf8("positionTrackingComboBox"));

        verticalLayout->addWidget(positionTrackingComboBox);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        showTrackCheckBox = new QCheckBox(CurrentLocationWidget);
        showTrackCheckBox->setObjectName(QString::fromUtf8("showTrackCheckBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(showTrackCheckBox->sizePolicy().hasHeightForWidth());
        showTrackCheckBox->setSizePolicy(sizePolicy1);

        horizontalLayout->addWidget(showTrackCheckBox);

        clearTrackButton = new QToolButton(CurrentLocationWidget);
        clearTrackButton->setObjectName(QString::fromUtf8("clearTrackButton"));
        sizePolicy.setHeightForWidth(clearTrackButton->sizePolicy().hasHeightForWidth());
        clearTrackButton->setSizePolicy(sizePolicy);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/16x16/edit-clear-list.png"), QSize(), QIcon::Normal, QIcon::Off);
        clearTrackButton->setIcon(icon);
        clearTrackButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

        horizontalLayout->addWidget(clearTrackButton);

        openTrackButton = new QToolButton(CurrentLocationWidget);
        openTrackButton->setObjectName(QString::fromUtf8("openTrackButton"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/16x16/document-open.png"), QSize(), QIcon::Normal, QIcon::Off);
        openTrackButton->setIcon(icon1);

        horizontalLayout->addWidget(openTrackButton);

        saveTrackButton = new QToolButton(CurrentLocationWidget);
        saveTrackButton->setObjectName(QString::fromUtf8("saveTrackButton"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/icons/16x16/document-save.png"), QSize(), QIcon::Normal, QIcon::Off);
        saveTrackButton->setIcon(icon2);

        horizontalLayout->addWidget(saveTrackButton);


        verticalLayout->addLayout(horizontalLayout);

        locationLabel = new QLabel(CurrentLocationWidget);
        locationLabel->setObjectName(QString::fromUtf8("locationLabel"));
        locationLabel->setEnabled(false);
        locationLabel->setAutoFillBackground(false);
        locationLabel->setFrameShape(QFrame::StyledPanel);
        locationLabel->setFrameShadow(QFrame::Sunken);
        locationLabel->setTextFormat(Qt::RichText);
        locationLabel->setWordWrap(true);

        verticalLayout->addWidget(locationLabel);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        recenterLabel = new QLabel(CurrentLocationWidget);
        recenterLabel->setObjectName(QString::fromUtf8("recenterLabel"));
        recenterLabel->setEnabled(false);
        sizePolicy.setHeightForWidth(recenterLabel->sizePolicy().hasHeightForWidth());
        recenterLabel->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(recenterLabel);

        recenterComboBox = new QComboBox(CurrentLocationWidget);
        recenterComboBox->setObjectName(QString::fromUtf8("recenterComboBox"));
        recenterComboBox->setEnabled(false);

        verticalLayout->addWidget(recenterComboBox);

        autoZoomCheckBox = new QCheckBox(CurrentLocationWidget);
        autoZoomCheckBox->setObjectName(QString::fromUtf8("autoZoomCheckBox"));
        autoZoomCheckBox->setEnabled(false);

        verticalLayout->addWidget(autoZoomCheckBox);

#ifndef QT_NO_SHORTCUT
        projectionLabel_3->setBuddy(positionTrackingComboBox);
#endif // QT_NO_SHORTCUT

        retranslateUi(CurrentLocationWidget);

        QMetaObject::connectSlotsByName(CurrentLocationWidget);
    } // setupUi

    void retranslateUi(QWidget *CurrentLocationWidget)
    {
        CurrentLocationWidget->setWindowTitle(QApplication::translate("CurrentLocationWidget", "Current Location", 0, QApplication::UnicodeUTF8));
        projectionLabel_3->setText(QApplication::translate("CurrentLocationWidget", "Position Tracking", 0, QApplication::UnicodeUTF8));
        positionTrackingComboBox->clear();
        positionTrackingComboBox->insertItems(0, QStringList()
         << QApplication::translate("CurrentLocationWidget", "Disabled", 0, QApplication::UnicodeUTF8)
        );
        showTrackCheckBox->setText(QApplication::translate("CurrentLocationWidget", "Show Track", 0, QApplication::UnicodeUTF8));
        clearTrackButton->setText(QApplication::translate("CurrentLocationWidget", "Clear Track", 0, QApplication::UnicodeUTF8));
        openTrackButton->setText(QApplication::translate("CurrentLocationWidget", "...", 0, QApplication::UnicodeUTF8));
        saveTrackButton->setText(QApplication::translate("CurrentLocationWidget", "...", 0, QApplication::UnicodeUTF8));
        locationLabel->setText(QApplication::translate("CurrentLocationWidget", "No position available.", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        recenterLabel->setToolTip(QApplication::translate("CurrentLocationWidget", "Select the method to use to re-center your GPS location on map.", "Select the method to use to recenter your GPS location on map.", QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        recenterLabel->setText(QApplication::translate("CurrentLocationWidget", "Map Adjustment", 0, QApplication::UnicodeUTF8));
        recenterComboBox->clear();
        recenterComboBox->insertItems(0, QStringList()
         << QApplication::translate("CurrentLocationWidget", "Disabled", "Disable Auto Recenter", QApplication::UnicodeUTF8)
         << QApplication::translate("CurrentLocationWidget", "Keep at Center", "Always keep gps location at the center of the map", QApplication::UnicodeUTF8)
         << QApplication::translate("CurrentLocationWidget", "When required", "Auto center when required", QApplication::UnicodeUTF8)
        );
#ifndef QT_NO_TOOLTIP
        autoZoomCheckBox->setToolTip(QApplication::translate("CurrentLocationWidget", "Auto Zoom to appropriate zoom level", "Auto Zoom to appropriate zoom level", QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        autoZoomCheckBox->setText(QApplication::translate("CurrentLocationWidget", "Auto Zoom", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class CurrentLocationWidget: public Ui_CurrentLocationWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CURRENTLOCATIONWIDGET_H
