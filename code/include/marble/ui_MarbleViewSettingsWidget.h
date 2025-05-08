/********************************************************************************
** Form generated from reading UI file 'MarbleViewSettingsWidget.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MARBLEVIEWSETTINGSWIDGET_H
#define UI_MARBLEVIEWSETTINGSWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QFontComboBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MarbleViewSettingsWidget
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox_units;
    QGridLayout *gridLayout;
    QLabel *label_distanceUnit;
    QComboBox *kcfg_distanceUnit;
    QLabel *label_angleUnit;
    QComboBox *kcfg_angleUnit;
    QGroupBox *groupBox_mapQuality;
    QGridLayout *gridLayout1;
    QLabel *label_stillQuality;
    QComboBox *kcfg_stillQuality;
    QLabel *label_animationQuality;
    QComboBox *kcfg_animationQuality;
    QLabel *label_graphicsSystem;
    QComboBox *kcfg_graphicsSystem;
    QSpacerItem *verticalSpacer;
    QGroupBox *groupBox_textLabels;
    QVBoxLayout *vboxLayout;
    QHBoxLayout *hboxLayout;
    QLabel *label_labelLocalization;
    QComboBox *kcfg_labelLocalization;
    QHBoxLayout *hboxLayout1;
    QLabel *label_mapFont;
    QFontComboBox *kcfg_mapFont;
    QSpacerItem *spacerItem;

    void setupUi(QWidget *MarbleViewSettingsWidget)
    {
        if (MarbleViewSettingsWidget->objectName().isEmpty())
            MarbleViewSettingsWidget->setObjectName(QString::fromUtf8("MarbleViewSettingsWidget"));
        MarbleViewSettingsWidget->resize(541, 430);
        verticalLayout = new QVBoxLayout(MarbleViewSettingsWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        groupBox_units = new QGroupBox(MarbleViewSettingsWidget);
        groupBox_units->setObjectName(QString::fromUtf8("groupBox_units"));
        gridLayout = new QGridLayout(groupBox_units);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label_distanceUnit = new QLabel(groupBox_units);
        label_distanceUnit->setObjectName(QString::fromUtf8("label_distanceUnit"));
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_distanceUnit->sizePolicy().hasHeightForWidth());
        label_distanceUnit->setSizePolicy(sizePolicy);
        label_distanceUnit->setMinimumSize(QSize(118, 0));

        gridLayout->addWidget(label_distanceUnit, 0, 0, 1, 1);

        kcfg_distanceUnit = new QComboBox(groupBox_units);
        kcfg_distanceUnit->setObjectName(QString::fromUtf8("kcfg_distanceUnit"));
        kcfg_distanceUnit->setEnabled(true);
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(kcfg_distanceUnit->sizePolicy().hasHeightForWidth());
        kcfg_distanceUnit->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(kcfg_distanceUnit, 0, 1, 1, 1);

        label_angleUnit = new QLabel(groupBox_units);
        label_angleUnit->setObjectName(QString::fromUtf8("label_angleUnit"));
        sizePolicy.setHeightForWidth(label_angleUnit->sizePolicy().hasHeightForWidth());
        label_angleUnit->setSizePolicy(sizePolicy);

        gridLayout->addWidget(label_angleUnit, 1, 0, 1, 1);

        kcfg_angleUnit = new QComboBox(groupBox_units);
        kcfg_angleUnit->setObjectName(QString::fromUtf8("kcfg_angleUnit"));
        kcfg_angleUnit->setEnabled(true);

        gridLayout->addWidget(kcfg_angleUnit, 1, 1, 1, 1);


        verticalLayout->addWidget(groupBox_units);

        groupBox_mapQuality = new QGroupBox(MarbleViewSettingsWidget);
        groupBox_mapQuality->setObjectName(QString::fromUtf8("groupBox_mapQuality"));
        gridLayout1 = new QGridLayout(groupBox_mapQuality);
        gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
        label_stillQuality = new QLabel(groupBox_mapQuality);
        label_stillQuality->setObjectName(QString::fromUtf8("label_stillQuality"));
        sizePolicy.setHeightForWidth(label_stillQuality->sizePolicy().hasHeightForWidth());
        label_stillQuality->setSizePolicy(sizePolicy);
        label_stillQuality->setMinimumSize(QSize(118, 0));

        gridLayout1->addWidget(label_stillQuality, 0, 0, 1, 1);

        kcfg_stillQuality = new QComboBox(groupBox_mapQuality);
        kcfg_stillQuality->setObjectName(QString::fromUtf8("kcfg_stillQuality"));

        gridLayout1->addWidget(kcfg_stillQuality, 0, 1, 1, 1);

        label_animationQuality = new QLabel(groupBox_mapQuality);
        label_animationQuality->setObjectName(QString::fromUtf8("label_animationQuality"));
        sizePolicy.setHeightForWidth(label_animationQuality->sizePolicy().hasHeightForWidth());
        label_animationQuality->setSizePolicy(sizePolicy);

        gridLayout1->addWidget(label_animationQuality, 1, 0, 1, 1);

        kcfg_animationQuality = new QComboBox(groupBox_mapQuality);
        kcfg_animationQuality->setObjectName(QString::fromUtf8("kcfg_animationQuality"));

        gridLayout1->addWidget(kcfg_animationQuality, 1, 1, 1, 1);

        label_graphicsSystem = new QLabel(groupBox_mapQuality);
        label_graphicsSystem->setObjectName(QString::fromUtf8("label_graphicsSystem"));

        gridLayout1->addWidget(label_graphicsSystem, 3, 0, 1, 1);

        kcfg_graphicsSystem = new QComboBox(groupBox_mapQuality);
        kcfg_graphicsSystem->setObjectName(QString::fromUtf8("kcfg_graphicsSystem"));

        gridLayout1->addWidget(kcfg_graphicsSystem, 3, 1, 1, 1);

        verticalSpacer = new QSpacerItem(20, 5, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout1->addItem(verticalSpacer, 2, 0, 1, 1);


        verticalLayout->addWidget(groupBox_mapQuality);

        groupBox_textLabels = new QGroupBox(MarbleViewSettingsWidget);
        groupBox_textLabels->setObjectName(QString::fromUtf8("groupBox_textLabels"));
        vboxLayout = new QVBoxLayout(groupBox_textLabels);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        hboxLayout = new QHBoxLayout();
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        label_labelLocalization = new QLabel(groupBox_textLabels);
        label_labelLocalization->setObjectName(QString::fromUtf8("label_labelLocalization"));
        sizePolicy.setHeightForWidth(label_labelLocalization->sizePolicy().hasHeightForWidth());
        label_labelLocalization->setSizePolicy(sizePolicy);
        label_labelLocalization->setMinimumSize(QSize(118, 0));

        hboxLayout->addWidget(label_labelLocalization);

        kcfg_labelLocalization = new QComboBox(groupBox_textLabels);
        kcfg_labelLocalization->setObjectName(QString::fromUtf8("kcfg_labelLocalization"));
        kcfg_labelLocalization->setEnabled(false);

        hboxLayout->addWidget(kcfg_labelLocalization);


        vboxLayout->addLayout(hboxLayout);

        hboxLayout1 = new QHBoxLayout();
        hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
        label_mapFont = new QLabel(groupBox_textLabels);
        label_mapFont->setObjectName(QString::fromUtf8("label_mapFont"));
        sizePolicy.setHeightForWidth(label_mapFont->sizePolicy().hasHeightForWidth());
        label_mapFont->setSizePolicy(sizePolicy);
        label_mapFont->setMinimumSize(QSize(118, 0));

        hboxLayout1->addWidget(label_mapFont);

        kcfg_mapFont = new QFontComboBox(groupBox_textLabels);
        kcfg_mapFont->setObjectName(QString::fromUtf8("kcfg_mapFont"));

        hboxLayout1->addWidget(kcfg_mapFont);


        vboxLayout->addLayout(hboxLayout1);


        verticalLayout->addWidget(groupBox_textLabels);

        spacerItem = new QSpacerItem(446, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(spacerItem);

#ifndef QT_NO_SHORTCUT
        label_distanceUnit->setBuddy(kcfg_distanceUnit);
        label_angleUnit->setBuddy(kcfg_angleUnit);
        label_stillQuality->setBuddy(kcfg_stillQuality);
        label_animationQuality->setBuddy(kcfg_animationQuality);
        label_graphicsSystem->setBuddy(kcfg_graphicsSystem);
        label_labelLocalization->setBuddy(kcfg_labelLocalization);
        label_mapFont->setBuddy(kcfg_mapFont);
#endif // QT_NO_SHORTCUT

        retranslateUi(MarbleViewSettingsWidget);

        kcfg_stillQuality->setCurrentIndex(3);
        kcfg_animationQuality->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(MarbleViewSettingsWidget);
    } // setupUi

    void retranslateUi(QWidget *MarbleViewSettingsWidget)
    {
        groupBox_units->setTitle(QApplication::translate("MarbleViewSettingsWidget", "&Units", 0, QApplication::UnicodeUTF8));
        label_distanceUnit->setText(QApplication::translate("MarbleViewSettingsWidget", "&Distance:", 0, QApplication::UnicodeUTF8));
        kcfg_distanceUnit->clear();
        kcfg_distanceUnit->insertItems(0, QStringList()
         << QApplication::translate("MarbleViewSettingsWidget", "Kilometer, Meter", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MarbleViewSettingsWidget", "Miles, Feet", 0, QApplication::UnicodeUTF8)
        );
#ifndef QT_NO_TOOLTIP
        kcfg_distanceUnit->setToolTip(QApplication::translate("MarbleViewSettingsWidget", "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">The unit that gets used to measure altitude, lengths and distances (e.g. km, mi, ft).</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        label_angleUnit->setText(QApplication::translate("MarbleViewSettingsWidget", "An&gle:", 0, QApplication::UnicodeUTF8));
        kcfg_angleUnit->clear();
        kcfg_angleUnit->insertItems(0, QStringList()
         << QApplication::translate("MarbleViewSettingsWidget", "Degree (DMS)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MarbleViewSettingsWidget", "Degree (Decimal)", 0, QApplication::UnicodeUTF8)
        );
#ifndef QT_NO_TOOLTIP
        kcfg_angleUnit->setToolTip(QApplication::translate("MarbleViewSettingsWidget", "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Specifies the notation of angles in coordinates: By default the Degree-Minute-Second notation (e.g. 54\302\26030'00\" ) gets used. As an alternative you can choose decimal degrees (e.g. 54.5\302\260).</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        groupBox_mapQuality->setTitle(QApplication::translate("MarbleViewSettingsWidget", "Map &Quality", 0, QApplication::UnicodeUTF8));
        label_stillQuality->setText(QApplication::translate("MarbleViewSettingsWidget", "&Still image:", 0, QApplication::UnicodeUTF8));
        kcfg_stillQuality->clear();
        kcfg_stillQuality->insertItems(0, QStringList()
         << QApplication::translate("MarbleViewSettingsWidget", "Outline Quality", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MarbleViewSettingsWidget", "Low Quality", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MarbleViewSettingsWidget", "Normal", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MarbleViewSettingsWidget", "High Quality", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MarbleViewSettingsWidget", "Print Quality", 0, QApplication::UnicodeUTF8)
        );
#ifndef QT_NO_TOOLTIP
        kcfg_stillQuality->setToolTip(QApplication::translate("MarbleViewSettingsWidget", "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Specifies the map quality that gets displayed while there is no user input. Usually this allows for high map quality as speed is no concern.</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        label_animationQuality->setText(QApplication::translate("MarbleViewSettingsWidget", "During &animations:", 0, QApplication::UnicodeUTF8));
        kcfg_animationQuality->clear();
        kcfg_animationQuality->insertItems(0, QStringList()
         << QApplication::translate("MarbleViewSettingsWidget", "Outline Quality", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MarbleViewSettingsWidget", "Low Quality", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MarbleViewSettingsWidget", "Normal", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MarbleViewSettingsWidget", "High Quality", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MarbleViewSettingsWidget", "Print Quality", 0, QApplication::UnicodeUTF8)
        );
#ifndef QT_NO_TOOLTIP
        kcfg_animationQuality->setToolTip(QApplication::translate("MarbleViewSettingsWidget", "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Specifies the map quality that gets displayed during map animations (e.g. while dragging the globe). </p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Especially on slow machines it is advisable to set this option to \"low quality\" as this will give better speed.</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        label_graphicsSystem->setText(QApplication::translate("MarbleViewSettingsWidget", "G&raphics system:", 0, QApplication::UnicodeUTF8));
        kcfg_graphicsSystem->clear();
        kcfg_graphicsSystem->insertItems(0, QStringList()
         << QApplication::translate("MarbleViewSettingsWidget", "Native", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MarbleViewSettingsWidget", "Raster (Software rendering)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MarbleViewSettingsWidget", "OpenGL", 0, QApplication::UnicodeUTF8)
        );
#ifndef QT_NO_TOOLTIP
        kcfg_graphicsSystem->setToolTip(QApplication::translate("MarbleViewSettingsWidget", "Specifies the graphics system that Qt uses to draw the map.\n"
"Changing this setting might be a good idea if there are performance issues with the system's graphics drivers.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        groupBox_textLabels->setTitle(QApplication::translate("MarbleViewSettingsWidget", "&Text Labels", 0, QApplication::UnicodeUTF8));
        label_labelLocalization->setText(QApplication::translate("MarbleViewSettingsWidget", "&Place names:", 0, QApplication::UnicodeUTF8));
        kcfg_labelLocalization->clear();
        kcfg_labelLocalization->insertItems(0, QStringList()
         << QApplication::translate("MarbleViewSettingsWidget", "Custom & Native Language", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MarbleViewSettingsWidget", "Custom Language", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MarbleViewSettingsWidget", "Native Language", 0, QApplication::UnicodeUTF8)
        );
#ifndef QT_NO_TOOLTIP
        kcfg_labelLocalization->setToolTip(QApplication::translate("MarbleViewSettingsWidget", "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Places often have different names in different languages. The label on the map can show the name in the user's language. Alternatively it can display the name in the official language and glyphs of that place. </p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        label_mapFont->setText(QApplication::translate("MarbleViewSettingsWidget", "Default map &font:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        kcfg_mapFont->setToolTip(QApplication::translate("MarbleViewSettingsWidget", "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">The default font that gets used on the map.</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        Q_UNUSED(MarbleViewSettingsWidget);
    } // retranslateUi

};

namespace Ui {
    class MarbleViewSettingsWidget: public Ui_MarbleViewSettingsWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MARBLEVIEWSETTINGSWIDGET_H
