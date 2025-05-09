/*
*********************************************************************
**
** Copyright (C) 2014 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the tools applications of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
*********************************************************************
*/

/********************************************************************************
** Form generated from reading UI file 'paletteeditoradvanced.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PALETTEEDITORADVANCED_H
#define UI_PALETTEEDITORADVANCED_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_PaletteEditorAdvanced
{
public:
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout;
    QLabel *paletteComboLabel;
    QComboBox *paletteCombo;
    QGroupBox *autoGroupBox;
    QVBoxLayout *verticalLayout;
    QCheckBox *checkBuildInactive;
    QCheckBox *checkBuildDisabled;
    QGroupBox *groupCentral;
    QVBoxLayout *verticalLayout_2;
    QComboBox *comboCentral;
    QHBoxLayout *layoutCentral;
    QSpacerItem *horizontalSpacer;
    QLabel *labelCentral;
    QGroupBox *groupEffect;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_2;
    QCheckBox *checkBuildEffect;
    QComboBox *comboEffect;
    QHBoxLayout *layoutEffect;
    QSpacerItem *horizontalSpacer_2;
    QLabel *labelEffect;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *PaletteEditorAdvanced)
    {
        if (PaletteEditorAdvanced->objectName().isEmpty())
            PaletteEditorAdvanced->setObjectName(QString::fromUtf8("PaletteEditorAdvanced"));
        PaletteEditorAdvanced->resize(239, 344);
        verticalLayout_4 = new QVBoxLayout(PaletteEditorAdvanced);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        paletteComboLabel = new QLabel(PaletteEditorAdvanced);
        paletteComboLabel->setObjectName(QString::fromUtf8("paletteComboLabel"));

        horizontalLayout->addWidget(paletteComboLabel);

        paletteCombo = new QComboBox(PaletteEditorAdvanced);
        paletteCombo->setObjectName(QString::fromUtf8("paletteCombo"));

        horizontalLayout->addWidget(paletteCombo);


        verticalLayout_4->addLayout(horizontalLayout);

        autoGroupBox = new QGroupBox(PaletteEditorAdvanced);
        autoGroupBox->setObjectName(QString::fromUtf8("autoGroupBox"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(autoGroupBox->sizePolicy().hasHeightForWidth());
        autoGroupBox->setSizePolicy(sizePolicy);
        verticalLayout = new QVBoxLayout(autoGroupBox);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        checkBuildInactive = new QCheckBox(autoGroupBox);
        checkBuildInactive->setObjectName(QString::fromUtf8("checkBuildInactive"));
        checkBuildInactive->setChecked(true);

        verticalLayout->addWidget(checkBuildInactive);

        checkBuildDisabled = new QCheckBox(autoGroupBox);
        checkBuildDisabled->setObjectName(QString::fromUtf8("checkBuildDisabled"));
        checkBuildDisabled->setChecked(true);

        verticalLayout->addWidget(checkBuildDisabled);


        verticalLayout_4->addWidget(autoGroupBox);

        groupCentral = new QGroupBox(PaletteEditorAdvanced);
        groupCentral->setObjectName(QString::fromUtf8("groupCentral"));
        verticalLayout_2 = new QVBoxLayout(groupCentral);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        comboCentral = new QComboBox(groupCentral);
        comboCentral->setObjectName(QString::fromUtf8("comboCentral"));

        verticalLayout_2->addWidget(comboCentral);

        layoutCentral = new QHBoxLayout();
        layoutCentral->setObjectName(QString::fromUtf8("layoutCentral"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        layoutCentral->addItem(horizontalSpacer);

        labelCentral = new QLabel(groupCentral);
        labelCentral->setObjectName(QString::fromUtf8("labelCentral"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(labelCentral->sizePolicy().hasHeightForWidth());
        labelCentral->setSizePolicy(sizePolicy1);
        labelCentral->setMinimumSize(QSize(0, 0));

        layoutCentral->addWidget(labelCentral);


        verticalLayout_2->addLayout(layoutCentral);


        verticalLayout_4->addWidget(groupCentral);

        groupEffect = new QGroupBox(PaletteEditorAdvanced);
        groupEffect->setObjectName(QString::fromUtf8("groupEffect"));
        verticalLayout_3 = new QVBoxLayout(groupEffect);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        checkBuildEffect = new QCheckBox(groupEffect);
        checkBuildEffect->setObjectName(QString::fromUtf8("checkBuildEffect"));
        checkBuildEffect->setChecked(true);

        horizontalLayout_2->addWidget(checkBuildEffect);

        comboEffect = new QComboBox(groupEffect);
        comboEffect->setObjectName(QString::fromUtf8("comboEffect"));
        comboEffect->setEnabled(false);

        horizontalLayout_2->addWidget(comboEffect);


        verticalLayout_3->addLayout(horizontalLayout_2);

        layoutEffect = new QHBoxLayout();
        layoutEffect->setObjectName(QString::fromUtf8("layoutEffect"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        layoutEffect->addItem(horizontalSpacer_2);

        labelEffect = new QLabel(groupEffect);
        labelEffect->setObjectName(QString::fromUtf8("labelEffect"));
        sizePolicy1.setHeightForWidth(labelEffect->sizePolicy().hasHeightForWidth());
        labelEffect->setSizePolicy(sizePolicy1);
        labelEffect->setMinimumSize(QSize(0, 0));

        layoutEffect->addWidget(labelEffect);


        verticalLayout_3->addLayout(layoutEffect);


        verticalLayout_4->addWidget(groupEffect);

        buttonBox = new QDialogButtonBox(PaletteEditorAdvanced);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout_4->addWidget(buttonBox);

#ifndef QT_NO_SHORTCUT
        paletteComboLabel->setBuddy(paletteCombo);
#endif // QT_NO_SHORTCUT

        retranslateUi(PaletteEditorAdvanced);
        QObject::connect(buttonBox, SIGNAL(accepted()), PaletteEditorAdvanced, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), PaletteEditorAdvanced, SLOT(reject()));
        QObject::connect(checkBuildEffect, SIGNAL(toggled(bool)), comboEffect, SLOT(setDisabled(bool)));

        QMetaObject::connectSlotsByName(PaletteEditorAdvanced);
    } // setupUi

    void retranslateUi(QDialog *PaletteEditorAdvanced)
    {
        PaletteEditorAdvanced->setWindowTitle(QApplication::translate("PaletteEditorAdvanced", "Tune Palette", 0, QApplication::UnicodeUTF8));
        paletteComboLabel->setText(QApplication::translate("PaletteEditorAdvanced", "Select &Palette:", 0, QApplication::UnicodeUTF8));
        paletteCombo->clear();
        paletteCombo->insertItems(0, QStringList()
         << QApplication::translate("PaletteEditorAdvanced", "Active Palette", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PaletteEditorAdvanced", "Inactive Palette", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PaletteEditorAdvanced", "Disabled Palette", 0, QApplication::UnicodeUTF8)
        );
        autoGroupBox->setTitle(QApplication::translate("PaletteEditorAdvanced", "Auto", 0, QApplication::UnicodeUTF8));
        checkBuildInactive->setText(QApplication::translate("PaletteEditorAdvanced", "Build inactive palette from active", 0, QApplication::UnicodeUTF8));
        checkBuildDisabled->setText(QApplication::translate("PaletteEditorAdvanced", "Build disabled palette from active", 0, QApplication::UnicodeUTF8));
        groupCentral->setTitle(QApplication::translate("PaletteEditorAdvanced", "Central color &roles", 0, QApplication::UnicodeUTF8));
        comboCentral->clear();
        comboCentral->insertItems(0, QStringList()
         << QApplication::translate("PaletteEditorAdvanced", "Window", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PaletteEditorAdvanced", "WindowText", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PaletteEditorAdvanced", "Base", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PaletteEditorAdvanced", "AlternateBase", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PaletteEditorAdvanced", "ToolTipBase", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PaletteEditorAdvanced", "ToolTipText", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PaletteEditorAdvanced", "Text", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PaletteEditorAdvanced", "Button", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PaletteEditorAdvanced", "ButtonText", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PaletteEditorAdvanced", "BrightText", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PaletteEditorAdvanced", "Highlight", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PaletteEditorAdvanced", "HighlightedText", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PaletteEditorAdvanced", "Link", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PaletteEditorAdvanced", "LinkVisited", 0, QApplication::UnicodeUTF8)
        );
#ifndef QT_NO_TOOLTIP
        comboCentral->setToolTip(QApplication::translate("PaletteEditorAdvanced", "Choose central color role", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        comboCentral->setWhatsThis(QApplication::translate("PaletteEditorAdvanced", "<b>Select a color role.</b><p>Available central roles are: <ul> <li>Window - general background color.</li> <li>WindowText - general foreground color. </li> <li>Base - used as background color for e.g. text entry widgets, usually white or another light color. </li> <li>Text - the foreground color used with Base. Usually this is the same as WindowText, in what case it must provide good contrast both with Window and Base. </li> <li>Button - general button background color, where buttons need a background different from Window, as in the Macintosh style. </li> <li>ButtonText - a foreground color used with the Button color. </li> <li>Highlight - a color to indicate a selected or highlighted item. </li> <li>HighlightedText - a text color that contrasts to Highlight. </li> <li>BrightText - a text color that is very different from WindowText and contrasts well with e.g. black. </li> </ul> </p>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        labelCentral->setText(QApplication::translate("PaletteEditorAdvanced", "&Select Color:", 0, QApplication::UnicodeUTF8));
        groupEffect->setTitle(QApplication::translate("PaletteEditorAdvanced", "3-D shadow &effects", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        checkBuildEffect->setToolTip(QApplication::translate("PaletteEditorAdvanced", "Generate shadings", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        checkBuildEffect->setWhatsThis(QApplication::translate("PaletteEditorAdvanced", "Check to let 3D-effect colors be calculated from button-color.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        checkBuildEffect->setText(QApplication::translate("PaletteEditorAdvanced", "Build &from button color", 0, QApplication::UnicodeUTF8));
        comboEffect->clear();
        comboEffect->insertItems(0, QStringList()
         << QApplication::translate("PaletteEditorAdvanced", "Light", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PaletteEditorAdvanced", "Midlight", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PaletteEditorAdvanced", "Mid", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PaletteEditorAdvanced", "Dark", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PaletteEditorAdvanced", "Shadow", 0, QApplication::UnicodeUTF8)
        );
#ifndef QT_NO_TOOLTIP
        comboEffect->setToolTip(QApplication::translate("PaletteEditorAdvanced", "Choose 3D-effect color role", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        comboEffect->setWhatsThis(QApplication::translate("PaletteEditorAdvanced", "<b>Select a color role.</b><p>Available effect roles are: <ul> <li>Light - lighter than Button color. </li> <li>Midlight - between Button and Light. </li> <li>Mid - between Button and Dark. </li> <li>Dark - darker than Button. </li> <li>Shadow - a very dark color. </li> </ul>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        labelEffect->setText(QApplication::translate("PaletteEditorAdvanced", "Select Co&lor:", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class PaletteEditorAdvanced: public Ui_PaletteEditorAdvanced {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PALETTEEDITORADVANCED_H
