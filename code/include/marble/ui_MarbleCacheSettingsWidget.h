/********************************************************************************
** Form generated from reading UI file 'MarbleCacheSettingsWidget.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MARBLECACHESETTINGSWIDGET_H
#define UI_MARBLECACHESETTINGSWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MarbleCacheSettingsWidget
{
public:
    QVBoxLayout *verticalLayout_2;
    QGroupBox *groupBox_cache;
    QGridLayout *gridLayout;
    QLabel *label_volatileCache;
    QSpinBox *kcfg_volatileTileCacheLimit;
    QLabel *label_MBVolatileCache;
    QPushButton *button_clearVolatileCache;
    QSpacerItem *spacerItem;
    QLabel *label_persistentCache;
    QSpinBox *kcfg_persistentTileCacheLimit;
    QLabel *label_MBPersistentCache;
    QPushButton *button_clearPersistentCache;
    QSpacerItem *spacerItem1;
    QGroupBox *groupBox_proxy;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label_proxyUrl;
    QLineEdit *kcfg_proxyUrl;
    QLabel *label_proxyPort;
    QSpinBox *kcfg_proxyPort;
    QSpacerItem *verticalSpacer_2;
    QGridLayout *proxyGridLayout;
    QLabel *label_proxyType;
    QHBoxLayout *horizontalLayout_2;
    QComboBox *kcfg_proxyType;
    QSpacerItem *horizontalSpacer;
    QCheckBox *kcfg_proxyAuth;
    QSpacerItem *horizontalSpacer_2;
    QLabel *label_username;
    QLineEdit *kcfg_proxyUser;
    QSpacerItem *horizontalSpacer_3;
    QLabel *label_password;
    QLineEdit *kcfg_proxyPass;
    QSpacerItem *verticalSpacer_3;
    QSpacerItem *horizontalSpacer_4;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *MarbleCacheSettingsWidget)
    {
        if (MarbleCacheSettingsWidget->objectName().isEmpty())
            MarbleCacheSettingsWidget->setObjectName(QString::fromUtf8("MarbleCacheSettingsWidget"));
        MarbleCacheSettingsWidget->resize(459, 380);
        verticalLayout_2 = new QVBoxLayout(MarbleCacheSettingsWidget);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        groupBox_cache = new QGroupBox(MarbleCacheSettingsWidget);
        groupBox_cache->setObjectName(QString::fromUtf8("groupBox_cache"));
        gridLayout = new QGridLayout(groupBox_cache);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label_volatileCache = new QLabel(groupBox_cache);
        label_volatileCache->setObjectName(QString::fromUtf8("label_volatileCache"));
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_volatileCache->sizePolicy().hasHeightForWidth());
        label_volatileCache->setSizePolicy(sizePolicy);
        label_volatileCache->setMinimumSize(QSize(118, 0));

        gridLayout->addWidget(label_volatileCache, 0, 0, 1, 1);

        kcfg_volatileTileCacheLimit = new QSpinBox(groupBox_cache);
        kcfg_volatileTileCacheLimit->setObjectName(QString::fromUtf8("kcfg_volatileTileCacheLimit"));
        kcfg_volatileTileCacheLimit->setAlignment(Qt::AlignRight);
        kcfg_volatileTileCacheLimit->setMaximum(999999);
        kcfg_volatileTileCacheLimit->setValue(100);

        gridLayout->addWidget(kcfg_volatileTileCacheLimit, 0, 1, 1, 1);

        label_MBVolatileCache = new QLabel(groupBox_cache);
        label_MBVolatileCache->setObjectName(QString::fromUtf8("label_MBVolatileCache"));

        gridLayout->addWidget(label_MBVolatileCache, 0, 2, 1, 1);

        button_clearVolatileCache = new QPushButton(groupBox_cache);
        button_clearVolatileCache->setObjectName(QString::fromUtf8("button_clearVolatileCache"));
        QSizePolicy sizePolicy1(QSizePolicy::Maximum, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(button_clearVolatileCache->sizePolicy().hasHeightForWidth());
        button_clearVolatileCache->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(button_clearVolatileCache, 0, 3, 1, 1);

        spacerItem = new QSpacerItem(10, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(spacerItem, 0, 5, 1, 1);

        label_persistentCache = new QLabel(groupBox_cache);
        label_persistentCache->setObjectName(QString::fromUtf8("label_persistentCache"));
        sizePolicy.setHeightForWidth(label_persistentCache->sizePolicy().hasHeightForWidth());
        label_persistentCache->setSizePolicy(sizePolicy);
        label_persistentCache->setMinimumSize(QSize(118, 0));

        gridLayout->addWidget(label_persistentCache, 1, 0, 1, 1);

        kcfg_persistentTileCacheLimit = new QSpinBox(groupBox_cache);
        kcfg_persistentTileCacheLimit->setObjectName(QString::fromUtf8("kcfg_persistentTileCacheLimit"));
        kcfg_persistentTileCacheLimit->setMinimumSize(QSize(100, 0));
        kcfg_persistentTileCacheLimit->setAlignment(Qt::AlignRight);
        kcfg_persistentTileCacheLimit->setMaximum(999999);
        kcfg_persistentTileCacheLimit->setValue(999999);

        gridLayout->addWidget(kcfg_persistentTileCacheLimit, 1, 1, 1, 1);

        label_MBPersistentCache = new QLabel(groupBox_cache);
        label_MBPersistentCache->setObjectName(QString::fromUtf8("label_MBPersistentCache"));

        gridLayout->addWidget(label_MBPersistentCache, 1, 2, 1, 1);

        button_clearPersistentCache = new QPushButton(groupBox_cache);
        button_clearPersistentCache->setObjectName(QString::fromUtf8("button_clearPersistentCache"));
        sizePolicy1.setHeightForWidth(button_clearPersistentCache->sizePolicy().hasHeightForWidth());
        button_clearPersistentCache->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(button_clearPersistentCache, 1, 3, 1, 2);

        spacerItem1 = new QSpacerItem(10, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(spacerItem1, 1, 5, 1, 1);


        verticalLayout_2->addWidget(groupBox_cache);

        groupBox_proxy = new QGroupBox(MarbleCacheSettingsWidget);
        groupBox_proxy->setObjectName(QString::fromUtf8("groupBox_proxy"));
        verticalLayout = new QVBoxLayout(groupBox_proxy);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label_proxyUrl = new QLabel(groupBox_proxy);
        label_proxyUrl->setObjectName(QString::fromUtf8("label_proxyUrl"));

        horizontalLayout->addWidget(label_proxyUrl);

        kcfg_proxyUrl = new QLineEdit(groupBox_proxy);
        kcfg_proxyUrl->setObjectName(QString::fromUtf8("kcfg_proxyUrl"));

        horizontalLayout->addWidget(kcfg_proxyUrl);

        label_proxyPort = new QLabel(groupBox_proxy);
        label_proxyPort->setObjectName(QString::fromUtf8("label_proxyPort"));

        horizontalLayout->addWidget(label_proxyPort);

        kcfg_proxyPort = new QSpinBox(groupBox_proxy);
        kcfg_proxyPort->setObjectName(QString::fromUtf8("kcfg_proxyPort"));
        kcfg_proxyPort->setMaximum(65535);
        kcfg_proxyPort->setValue(8081);

        horizontalLayout->addWidget(kcfg_proxyPort);


        verticalLayout->addLayout(horizontalLayout);

        verticalSpacer_2 = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout->addItem(verticalSpacer_2);

        proxyGridLayout = new QGridLayout();
        proxyGridLayout->setObjectName(QString::fromUtf8("proxyGridLayout"));
        label_proxyType = new QLabel(groupBox_proxy);
        label_proxyType->setObjectName(QString::fromUtf8("label_proxyType"));

        proxyGridLayout->addWidget(label_proxyType, 0, 1, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        kcfg_proxyType = new QComboBox(groupBox_proxy);
        kcfg_proxyType->setObjectName(QString::fromUtf8("kcfg_proxyType"));

        horizontalLayout_2->addWidget(kcfg_proxyType);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);


        proxyGridLayout->addLayout(horizontalLayout_2, 0, 2, 1, 1);

        kcfg_proxyAuth = new QCheckBox(groupBox_proxy);
        kcfg_proxyAuth->setObjectName(QString::fromUtf8("kcfg_proxyAuth"));

        proxyGridLayout->addWidget(kcfg_proxyAuth, 2, 0, 1, 3);

        horizontalSpacer_2 = new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        proxyGridLayout->addItem(horizontalSpacer_2, 3, 0, 1, 1);

        label_username = new QLabel(groupBox_proxy);
        label_username->setObjectName(QString::fromUtf8("label_username"));

        proxyGridLayout->addWidget(label_username, 3, 1, 1, 1);

        kcfg_proxyUser = new QLineEdit(groupBox_proxy);
        kcfg_proxyUser->setObjectName(QString::fromUtf8("kcfg_proxyUser"));
        kcfg_proxyUser->setEnabled(false);

        proxyGridLayout->addWidget(kcfg_proxyUser, 3, 2, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        proxyGridLayout->addItem(horizontalSpacer_3, 4, 0, 1, 1);

        label_password = new QLabel(groupBox_proxy);
        label_password->setObjectName(QString::fromUtf8("label_password"));

        proxyGridLayout->addWidget(label_password, 4, 1, 1, 1);

        kcfg_proxyPass = new QLineEdit(groupBox_proxy);
        kcfg_proxyPass->setObjectName(QString::fromUtf8("kcfg_proxyPass"));
        kcfg_proxyPass->setEnabled(false);
        kcfg_proxyPass->setEchoMode(QLineEdit::Password);

        proxyGridLayout->addWidget(kcfg_proxyPass, 4, 2, 1, 1);

        verticalSpacer_3 = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Fixed);

        proxyGridLayout->addItem(verticalSpacer_3, 1, 2, 1, 1);

        horizontalSpacer_4 = new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        proxyGridLayout->addItem(horizontalSpacer_4, 0, 0, 1, 1);


        verticalLayout->addLayout(proxyGridLayout);


        verticalLayout_2->addWidget(groupBox_proxy);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);

#ifndef QT_NO_SHORTCUT
        label_volatileCache->setBuddy(kcfg_volatileTileCacheLimit);
        label_persistentCache->setBuddy(kcfg_persistentTileCacheLimit);
        label_proxyUrl->setBuddy(kcfg_proxyUrl);
        label_proxyPort->setBuddy(kcfg_proxyPort);
        label_username->setBuddy(kcfg_proxyUser);
        label_password->setBuddy(kcfg_proxyPass);
#endif // QT_NO_SHORTCUT

        retranslateUi(MarbleCacheSettingsWidget);

        QMetaObject::connectSlotsByName(MarbleCacheSettingsWidget);
    } // setupUi

    void retranslateUi(QWidget *MarbleCacheSettingsWidget)
    {
#ifndef QT_NO_TOOLTIP
        groupBox_cache->setToolTip(QApplication::translate("MarbleCacheSettingsWidget", "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">There are two caches being used for Marble: The physical memory which is needed to keep map data in the computer's memory. Increasing the value will make the application more responsive. The hard disc memory cache is used by download contents from the Internet (e.g. Wikipedia data or map data). Decrease this value if you want to save space on the hard disc and if high usage of the Internet is not an issue. </p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        groupBox_cache->setTitle(QApplication::translate("MarbleCacheSettingsWidget", "C&ache", 0, QApplication::UnicodeUTF8));
        label_volatileCache->setText(QApplication::translate("MarbleCacheSettingsWidget", "&Physical memory:", 0, QApplication::UnicodeUTF8));
        label_MBVolatileCache->setText(QApplication::translate("MarbleCacheSettingsWidget", "MB", 0, QApplication::UnicodeUTF8));
        button_clearVolatileCache->setText(QApplication::translate("MarbleCacheSettingsWidget", "C&lear", 0, QApplication::UnicodeUTF8));
        label_persistentCache->setText(QApplication::translate("MarbleCacheSettingsWidget", "&Hard disc:", 0, QApplication::UnicodeUTF8));
        kcfg_persistentTileCacheLimit->setSpecialValueText(QApplication::translate("MarbleCacheSettingsWidget", "Unlimited", 0, QApplication::UnicodeUTF8));
        label_MBPersistentCache->setText(QApplication::translate("MarbleCacheSettingsWidget", "MB", 0, QApplication::UnicodeUTF8));
        button_clearPersistentCache->setText(QApplication::translate("MarbleCacheSettingsWidget", "Cl&ear", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        groupBox_proxy->setToolTip(QApplication::translate("MarbleCacheSettingsWidget", "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Proxy settings for your local intranet. Please leave empty if there is no proxy.</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        groupBox_proxy->setTitle(QApplication::translate("MarbleCacheSettingsWidget", "&Proxy", 0, QApplication::UnicodeUTF8));
        label_proxyUrl->setText(QApplication::translate("MarbleCacheSettingsWidget", "&Proxy:", 0, QApplication::UnicodeUTF8));
        kcfg_proxyUrl->setText(QString());
        label_proxyPort->setText(QApplication::translate("MarbleCacheSettingsWidget", "P&ort:", 0, QApplication::UnicodeUTF8));
        label_proxyType->setText(QApplication::translate("MarbleCacheSettingsWidget", "Proxy type:", 0, QApplication::UnicodeUTF8));
        kcfg_proxyType->clear();
        kcfg_proxyType->insertItems(0, QStringList()
         << QApplication::translate("MarbleCacheSettingsWidget", "Http", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MarbleCacheSettingsWidget", "Socks5", 0, QApplication::UnicodeUTF8)
        );
        kcfg_proxyAuth->setText(QApplication::translate("MarbleCacheSettingsWidget", "Requires authentication", 0, QApplication::UnicodeUTF8));
        label_username->setText(QApplication::translate("MarbleCacheSettingsWidget", "U&sername:", 0, QApplication::UnicodeUTF8));
        label_password->setText(QApplication::translate("MarbleCacheSettingsWidget", "&Password:", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(MarbleCacheSettingsWidget);
    } // retranslateUi

};

namespace Ui {
    class MarbleCacheSettingsWidget: public Ui_MarbleCacheSettingsWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MARBLECACHESETTINGSWIDGET_H
