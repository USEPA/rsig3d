/********************************************************************************
** Form generated from reading UI file 'MarblePluginSettingsWidget.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MARBLEPLUGINSETTINGSWIDGET_H
#define UI_MARBLEPLUGINSETTINGSWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QListView>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MarblePluginSettingsWidget
{
public:
    QVBoxLayout *vboxLayout;
    QGroupBox *groupBox_plugins;
    QVBoxLayout *vboxLayout1;
    QListView *m_pluginListView;

    void setupUi(QWidget *MarblePluginSettingsWidget)
    {
        if (MarblePluginSettingsWidget->objectName().isEmpty())
            MarblePluginSettingsWidget->setObjectName(QString::fromUtf8("MarblePluginSettingsWidget"));
        MarblePluginSettingsWidget->resize(425, 302);
        vboxLayout = new QVBoxLayout(MarblePluginSettingsWidget);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        groupBox_plugins = new QGroupBox(MarblePluginSettingsWidget);
        groupBox_plugins->setObjectName(QString::fromUtf8("groupBox_plugins"));
        vboxLayout1 = new QVBoxLayout(groupBox_plugins);
        vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
        m_pluginListView = new QListView(groupBox_plugins);
        m_pluginListView->setObjectName(QString::fromUtf8("m_pluginListView"));

        vboxLayout1->addWidget(m_pluginListView);


        vboxLayout->addWidget(groupBox_plugins);


        retranslateUi(MarblePluginSettingsWidget);

        QMetaObject::connectSlotsByName(MarblePluginSettingsWidget);
    } // setupUi

    void retranslateUi(QWidget *MarblePluginSettingsWidget)
    {
#ifndef QT_NO_TOOLTIP
        groupBox_plugins->setToolTip(QApplication::translate("MarblePluginSettingsWidget", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'DejaVu Sans'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Sans Serif';\">This is Marble's machine room for advanced users. Here you can select and deselect the functionality that is provided through plugins. </span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Sans Serif';\">Deselecting a plugin will remove the functionality from the map, menus and toolbars. </span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-"
                        "indent:0px;\"><span style=\" font-family:'Sans Serif';\">Also some of the plugins might allow for tweaking their configuration a bit. </span></p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        groupBox_plugins->setTitle(QApplication::translate("MarblePluginSettingsWidget", "P&lugins", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(MarblePluginSettingsWidget);
    } // retranslateUi

};

namespace Ui {
    class MarblePluginSettingsWidget: public Ui_MarblePluginSettingsWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MARBLEPLUGINSETTINGSWIDGET_H
