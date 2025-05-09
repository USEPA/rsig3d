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
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QTabWidget>
#include <QtGui/QTextEdit>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "colorbutton.h"
#include "previewframe.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *fileSaveAction;
    QAction *fileExitAction;
    QAction *helpAboutAction;
    QAction *helpAboutQtAction;
    QWidget *widget;
    QGridLayout *gridLayout;
    QTextEdit *helpView;
    QTabWidget *mainTabWidget;
    QWidget *appearanceTab;
    QGridLayout *gridLayout_5;
    QGroupBox *guiStyleGroup;
    QHBoxLayout *hboxLayout;
    QLabel *guiStyleLabel;
    QComboBox *guiStyleCombo;
    QGroupBox *previewGroup;
    QGridLayout *gridLayout_4;
    QLabel *paletteLabel;
    QComboBox *paletteCombo;
    PreviewFrame *previewFrame;
    QGroupBox *buildPaletteGroup;
    QVBoxLayout *verticalLayout_2;
    QWidget *colorConfig;
    QHBoxLayout *horizontalLayout_2;
    QLabel *labelMainColor;
    ColorButton *buttonMainColor;
    QLabel *labelWindowColor;
    ColorButton *buttonWindowColor;
    QSpacerItem *spacer;
    QPushButton *tunePaletteButton;
    QLabel *kdeNoteLabel;
    QWidget *fontsTab;
    QVBoxLayout *vboxLayout;
    QGroupBox *defaultFontGroup;
    QGridLayout *gridLayout1;
    QComboBox *fontStyleCombo;
    QComboBox *fontFamilyCombo;
    QComboBox *pointSizeCombo;
    QLabel *fontStyleLabel;
    QLabel *pointSizeLabel;
    QLabel *fontFamilyLabel;
    QLineEdit *sampleLineEdit;
    QGroupBox *fontSubstitutionGroup;
    QVBoxLayout *vboxLayout1;
    QHBoxLayout *hboxLayout1;
    QLabel *familySubstitutionLabel;
    QComboBox *familySubstitutionCombo;
    QFrame *Line1;
    QLabel *substitutionsLabel;
    QListWidget *substitutionsListBox;
    QHBoxLayout *hboxLayout2;
    QPushButton *upSubstitutionButton;
    QPushButton *downSubstitutionButton;
    QPushButton *removeSubstitutionButton;
    QFrame *Line2;
    QHBoxLayout *hboxLayout3;
    QLabel *chooseSubstitutionLabel;
    QComboBox *chooseSubstitutionCombo;
    QPushButton *addSubstitutionButton;
    QWidget *interfaceTab;
    QVBoxLayout *vboxLayout2;
    QGroupBox *feelSettingsGroup;
    QGridLayout *gridLayout2;
    QSpinBox *doubleClickIntervalSpinBox;
    QLabel *doubleClickIntervalLabel;
    QSpinBox *cursorFlashTimeSpinBox;
    QLabel *cursorFlashTimeLabel;
    QSpinBox *wheelScrollLinesSpinBox;
    QLabel *wheelScrollLinesLabel;
    QCheckBox *resolveLinksCheckBox;
    QGroupBox *guiEffectsGroup;
    QVBoxLayout *vboxLayout3;
    QCheckBox *effectsCheckBox;
    QFrame *effectsFrame;
    QGridLayout *gridLayout3;
    QLabel *menuEffectLabel;
    QLabel *comboEffectLabel;
    QLabel *toolTipEffectLabel;
    QLabel *toolBoxEffectLabel;
    QComboBox *menuEffectCombo;
    QComboBox *comboEffectCombo;
    QComboBox *toolTipEffectCombo;
    QComboBox *toolBoxEffectCombo;
    QGroupBox *globalStrutGroup;
    QGridLayout *gridLayout4;
    QLabel *strutWidthLabel;
    QLabel *strutHeightLabel;
    QSpinBox *strutWidthSpinBox;
    QSpinBox *strutHeightSpinBox;
    QCheckBox *rtlExtensionsCheckBox;
    QLabel *inputStyleLabel;
    QComboBox *inputStyleCombo;
    QLabel *inputMethodLabel;
    QComboBox *inputMethodCombo;
    QSpacerItem *spacerItem;
    QWidget *printerTab;
    QVBoxLayout *vboxLayout4;
    QCheckBox *fontEmbeddingCheckBox;
    QGroupBox *fontPathsGroup;
    QVBoxLayout *vboxLayout5;
    QGridLayout *gridLayout5;
    QPushButton *upFontpathButton;
    QPushButton *removeFontpathButton;
    QPushButton *downFontpathButton;
    QListWidget *fontpathListBox;
    QGridLayout *gridLayout6;
    QSpacerItem *spacerItem1;
    QPushButton *addFontPathButton;
    QPushButton *browseFontPathButton;
    QLabel *browseFontPathLabel;
    QLineEdit *fontPathLineEdit;
    QWidget *phononTab;
    QVBoxLayout *verticalLayout;
    QGroupBox *aboutPhononGroup;
    QGridLayout *gridLayout_2;
    QLabel *phononVersionBuddyLabel;
    QLabel *phononVersionLabel;
    QLabel *phononWebsiteBuddyLabel;
    QLabel *phononWebsiteLabel;
    QGroupBox *aboutGStreamerGroup;
    QGridLayout *gridLayout7;
    QLabel *gstVersionBuddyLabel;
    QLabel *gstVersionLabel;
    QLabel *gstWebsiteBuddyLabel;
    QLabel *gstWebsiteLabel;
    QGroupBox *gstBackendGroup;
    QGridLayout *gridLayout_3;
    QLabel *audiosinkLabel;
    QComboBox *audiosinkCombo;
    QLabel *videomodeLabel;
    QComboBox *videomodeCombo;
    QLabel *gstBackendNoteLabel;
    QLabel *phononLabel;
    QSpacerItem *verticalSpacer;
    QMenuBar *mainMenu;
    QMenu *fileMenu;
    QMenu *saveMenu;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(815, 716);
        fileSaveAction = new QAction(MainWindow);
        fileSaveAction->setObjectName(QString::fromUtf8("fileSaveAction"));
        fileExitAction = new QAction(MainWindow);
        fileExitAction->setObjectName(QString::fromUtf8("fileExitAction"));
        helpAboutAction = new QAction(MainWindow);
        helpAboutAction->setObjectName(QString::fromUtf8("helpAboutAction"));
        helpAboutQtAction = new QAction(MainWindow);
        helpAboutQtAction->setObjectName(QString::fromUtf8("helpAboutQtAction"));
        widget = new QWidget(MainWindow);
        widget->setObjectName(QString::fromUtf8("widget"));
        gridLayout = new QGridLayout(widget);
        gridLayout->setContentsMargins(8, 8, 8, 8);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        helpView = new QTextEdit(widget);
        helpView->setObjectName(QString::fromUtf8("helpView"));
        helpView->setMinimumSize(QSize(200, 0));
        helpView->setReadOnly(true);

        gridLayout->addWidget(helpView, 0, 0, 1, 1);

        mainTabWidget = new QTabWidget(widget);
        mainTabWidget->setObjectName(QString::fromUtf8("mainTabWidget"));
        appearanceTab = new QWidget();
        appearanceTab->setObjectName(QString::fromUtf8("appearanceTab"));
        gridLayout_5 = new QGridLayout(appearanceTab);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        guiStyleGroup = new QGroupBox(appearanceTab);
        guiStyleGroup->setObjectName(QString::fromUtf8("guiStyleGroup"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(guiStyleGroup->sizePolicy().hasHeightForWidth());
        guiStyleGroup->setSizePolicy(sizePolicy);
        guiStyleGroup->setMinimumSize(QSize(0, 0));
        hboxLayout = new QHBoxLayout(guiStyleGroup);
        hboxLayout->setSpacing(4);
        hboxLayout->setContentsMargins(8, 8, 8, 8);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        guiStyleLabel = new QLabel(guiStyleGroup);
        guiStyleLabel->setObjectName(QString::fromUtf8("guiStyleLabel"));
        guiStyleLabel->setMinimumSize(QSize(0, 0));

        hboxLayout->addWidget(guiStyleLabel);

        guiStyleCombo = new QComboBox(guiStyleGroup);
        guiStyleCombo->setObjectName(QString::fromUtf8("guiStyleCombo"));

        hboxLayout->addWidget(guiStyleCombo);


        gridLayout_5->addWidget(guiStyleGroup, 0, 0, 1, 1);

        previewGroup = new QGroupBox(appearanceTab);
        previewGroup->setObjectName(QString::fromUtf8("previewGroup"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(previewGroup->sizePolicy().hasHeightForWidth());
        previewGroup->setSizePolicy(sizePolicy1);
        gridLayout_4 = new QGridLayout(previewGroup);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        paletteLabel = new QLabel(previewGroup);
        paletteLabel->setObjectName(QString::fromUtf8("paletteLabel"));

        gridLayout_4->addWidget(paletteLabel, 0, 0, 1, 1);

        paletteCombo = new QComboBox(previewGroup);
        paletteCombo->setObjectName(QString::fromUtf8("paletteCombo"));

        gridLayout_4->addWidget(paletteCombo, 0, 1, 1, 1);

        previewFrame = new PreviewFrame(previewGroup);
        previewFrame->setObjectName(QString::fromUtf8("previewFrame"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(previewFrame->sizePolicy().hasHeightForWidth());
        previewFrame->setSizePolicy(sizePolicy2);
        previewFrame->setMinimumSize(QSize(410, 260));

        gridLayout_4->addWidget(previewFrame, 1, 0, 1, 2);


        gridLayout_5->addWidget(previewGroup, 3, 0, 1, 1);

        buildPaletteGroup = new QGroupBox(appearanceTab);
        buildPaletteGroup->setObjectName(QString::fromUtf8("buildPaletteGroup"));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Maximum);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(buildPaletteGroup->sizePolicy().hasHeightForWidth());
        buildPaletteGroup->setSizePolicy(sizePolicy3);
        buildPaletteGroup->setMinimumSize(QSize(400, 0));
        verticalLayout_2 = new QVBoxLayout(buildPaletteGroup);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        colorConfig = new QWidget(buildPaletteGroup);
        colorConfig->setObjectName(QString::fromUtf8("colorConfig"));
        horizontalLayout_2 = new QHBoxLayout(colorConfig);
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        labelMainColor = new QLabel(colorConfig);
        labelMainColor->setObjectName(QString::fromUtf8("labelMainColor"));

        horizontalLayout_2->addWidget(labelMainColor);

        buttonMainColor = new ColorButton(colorConfig);
        buttonMainColor->setObjectName(QString::fromUtf8("buttonMainColor"));

        horizontalLayout_2->addWidget(buttonMainColor);

        labelWindowColor = new QLabel(colorConfig);
        labelWindowColor->setObjectName(QString::fromUtf8("labelWindowColor"));
        QSizePolicy sizePolicy4(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(labelWindowColor->sizePolicy().hasHeightForWidth());
        labelWindowColor->setSizePolicy(sizePolicy4);
        labelWindowColor->setMinimumSize(QSize(50, 0));
        labelWindowColor->setLineWidth(1);
        labelWindowColor->setMidLineWidth(0);
        labelWindowColor->setAlignment(Qt::AlignVCenter);
        labelWindowColor->setMargin(0);

        horizontalLayout_2->addWidget(labelWindowColor);

        buttonWindowColor = new ColorButton(colorConfig);
        buttonWindowColor->setObjectName(QString::fromUtf8("buttonWindowColor"));

        horizontalLayout_2->addWidget(buttonWindowColor);

        spacer = new QSpacerItem(70, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(spacer);

        tunePaletteButton = new QPushButton(colorConfig);
        tunePaletteButton->setObjectName(QString::fromUtf8("tunePaletteButton"));

        horizontalLayout_2->addWidget(tunePaletteButton);


        verticalLayout_2->addWidget(colorConfig);

        kdeNoteLabel = new QLabel(buildPaletteGroup);
        kdeNoteLabel->setObjectName(QString::fromUtf8("kdeNoteLabel"));

        verticalLayout_2->addWidget(kdeNoteLabel);


        gridLayout_5->addWidget(buildPaletteGroup, 1, 0, 1, 1);

        mainTabWidget->addTab(appearanceTab, QString());
        fontsTab = new QWidget();
        fontsTab->setObjectName(QString::fromUtf8("fontsTab"));
        vboxLayout = new QVBoxLayout(fontsTab);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        defaultFontGroup = new QGroupBox(fontsTab);
        defaultFontGroup->setObjectName(QString::fromUtf8("defaultFontGroup"));
        gridLayout1 = new QGridLayout(defaultFontGroup);
        gridLayout1->setSpacing(4);
        gridLayout1->setContentsMargins(8, 8, 8, 8);
        gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
        fontStyleCombo = new QComboBox(defaultFontGroup);
        fontStyleCombo->setObjectName(QString::fromUtf8("fontStyleCombo"));
        fontStyleCombo->setAutoCompletion(true);
        fontStyleCombo->setDuplicatesEnabled(false);

        gridLayout1->addWidget(fontStyleCombo, 1, 1, 1, 1);

        fontFamilyCombo = new QComboBox(defaultFontGroup);
        fontFamilyCombo->setObjectName(QString::fromUtf8("fontFamilyCombo"));
        fontFamilyCombo->setAutoCompletion(true);
        fontFamilyCombo->setDuplicatesEnabled(false);

        gridLayout1->addWidget(fontFamilyCombo, 0, 1, 1, 1);

        pointSizeCombo = new QComboBox(defaultFontGroup);
        pointSizeCombo->setObjectName(QString::fromUtf8("pointSizeCombo"));
        pointSizeCombo->setEditable(true);
        pointSizeCombo->setAutoCompletion(true);
        pointSizeCombo->setDuplicatesEnabled(false);

        gridLayout1->addWidget(pointSizeCombo, 2, 1, 1, 1);

        fontStyleLabel = new QLabel(defaultFontGroup);
        fontStyleLabel->setObjectName(QString::fromUtf8("fontStyleLabel"));

        gridLayout1->addWidget(fontStyleLabel, 1, 0, 1, 1);

        pointSizeLabel = new QLabel(defaultFontGroup);
        pointSizeLabel->setObjectName(QString::fromUtf8("pointSizeLabel"));

        gridLayout1->addWidget(pointSizeLabel, 2, 0, 1, 1);

        fontFamilyLabel = new QLabel(defaultFontGroup);
        fontFamilyLabel->setObjectName(QString::fromUtf8("fontFamilyLabel"));

        gridLayout1->addWidget(fontFamilyLabel, 0, 0, 1, 1);

        sampleLineEdit = new QLineEdit(defaultFontGroup);
        sampleLineEdit->setObjectName(QString::fromUtf8("sampleLineEdit"));
        sampleLineEdit->setAlignment(Qt::AlignHCenter);

        gridLayout1->addWidget(sampleLineEdit, 3, 0, 1, 2);


        vboxLayout->addWidget(defaultFontGroup);

        fontSubstitutionGroup = new QGroupBox(fontsTab);
        fontSubstitutionGroup->setObjectName(QString::fromUtf8("fontSubstitutionGroup"));
        vboxLayout1 = new QVBoxLayout(fontSubstitutionGroup);
        vboxLayout1->setSpacing(4);
        vboxLayout1->setContentsMargins(8, 8, 8, 8);
        vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
        hboxLayout1 = new QHBoxLayout();
        hboxLayout1->setSpacing(4);
#ifndef Q_OS_MAC
        hboxLayout1->setContentsMargins(0, 0, 0, 0);
#endif
        hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
        familySubstitutionLabel = new QLabel(fontSubstitutionGroup);
        familySubstitutionLabel->setObjectName(QString::fromUtf8("familySubstitutionLabel"));

        hboxLayout1->addWidget(familySubstitutionLabel);

        familySubstitutionCombo = new QComboBox(fontSubstitutionGroup);
        familySubstitutionCombo->setObjectName(QString::fromUtf8("familySubstitutionCombo"));
        familySubstitutionCombo->setEditable(true);
        familySubstitutionCombo->setAutoCompletion(true);
        familySubstitutionCombo->setDuplicatesEnabled(false);

        hboxLayout1->addWidget(familySubstitutionCombo);


        vboxLayout1->addLayout(hboxLayout1);

        Line1 = new QFrame(fontSubstitutionGroup);
        Line1->setObjectName(QString::fromUtf8("Line1"));
        Line1->setFrameShape(QFrame::HLine);
        Line1->setFrameShadow(QFrame::Sunken);
        Line1->setFrameShape(QFrame::HLine);

        vboxLayout1->addWidget(Line1);

        substitutionsLabel = new QLabel(fontSubstitutionGroup);
        substitutionsLabel->setObjectName(QString::fromUtf8("substitutionsLabel"));

        vboxLayout1->addWidget(substitutionsLabel);

        substitutionsListBox = new QListWidget(fontSubstitutionGroup);
        substitutionsListBox->setObjectName(QString::fromUtf8("substitutionsListBox"));

        vboxLayout1->addWidget(substitutionsListBox);

        hboxLayout2 = new QHBoxLayout();
        hboxLayout2->setSpacing(4);
        hboxLayout2->setContentsMargins(0, 0, 0, 0);
        hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
        upSubstitutionButton = new QPushButton(fontSubstitutionGroup);
        upSubstitutionButton->setObjectName(QString::fromUtf8("upSubstitutionButton"));

        hboxLayout2->addWidget(upSubstitutionButton);

        downSubstitutionButton = new QPushButton(fontSubstitutionGroup);
        downSubstitutionButton->setObjectName(QString::fromUtf8("downSubstitutionButton"));

        hboxLayout2->addWidget(downSubstitutionButton);

        removeSubstitutionButton = new QPushButton(fontSubstitutionGroup);
        removeSubstitutionButton->setObjectName(QString::fromUtf8("removeSubstitutionButton"));

        hboxLayout2->addWidget(removeSubstitutionButton);


        vboxLayout1->addLayout(hboxLayout2);

        Line2 = new QFrame(fontSubstitutionGroup);
        Line2->setObjectName(QString::fromUtf8("Line2"));
        Line2->setFrameShape(QFrame::HLine);
        Line2->setFrameShadow(QFrame::Sunken);
        Line2->setFrameShape(QFrame::HLine);

        vboxLayout1->addWidget(Line2);

        hboxLayout3 = new QHBoxLayout();
        hboxLayout3->setSpacing(4);
        hboxLayout3->setContentsMargins(0, 0, 0, 0);
        hboxLayout3->setObjectName(QString::fromUtf8("hboxLayout3"));
        chooseSubstitutionLabel = new QLabel(fontSubstitutionGroup);
        chooseSubstitutionLabel->setObjectName(QString::fromUtf8("chooseSubstitutionLabel"));

        hboxLayout3->addWidget(chooseSubstitutionLabel);

        chooseSubstitutionCombo = new QComboBox(fontSubstitutionGroup);
        chooseSubstitutionCombo->setObjectName(QString::fromUtf8("chooseSubstitutionCombo"));
        chooseSubstitutionCombo->setAutoCompletion(true);
        chooseSubstitutionCombo->setDuplicatesEnabled(false);

        hboxLayout3->addWidget(chooseSubstitutionCombo);

        addSubstitutionButton = new QPushButton(fontSubstitutionGroup);
        addSubstitutionButton->setObjectName(QString::fromUtf8("addSubstitutionButton"));

        hboxLayout3->addWidget(addSubstitutionButton);


        vboxLayout1->addLayout(hboxLayout3);


        vboxLayout->addWidget(fontSubstitutionGroup);

        mainTabWidget->addTab(fontsTab, QString());
        interfaceTab = new QWidget();
        interfaceTab->setObjectName(QString::fromUtf8("interfaceTab"));
        vboxLayout2 = new QVBoxLayout(interfaceTab);
        vboxLayout2->setObjectName(QString::fromUtf8("vboxLayout2"));
        feelSettingsGroup = new QGroupBox(interfaceTab);
        feelSettingsGroup->setObjectName(QString::fromUtf8("feelSettingsGroup"));
        gridLayout2 = new QGridLayout(feelSettingsGroup);
        gridLayout2->setSpacing(4);
        gridLayout2->setContentsMargins(8, 8, 8, 8);
        gridLayout2->setObjectName(QString::fromUtf8("gridLayout2"));
        doubleClickIntervalSpinBox = new QSpinBox(feelSettingsGroup);
        doubleClickIntervalSpinBox->setObjectName(QString::fromUtf8("doubleClickIntervalSpinBox"));
        doubleClickIntervalSpinBox->setMinimum(10);
        doubleClickIntervalSpinBox->setMaximum(10000);

        gridLayout2->addWidget(doubleClickIntervalSpinBox, 0, 1, 1, 1);

        doubleClickIntervalLabel = new QLabel(feelSettingsGroup);
        doubleClickIntervalLabel->setObjectName(QString::fromUtf8("doubleClickIntervalLabel"));

        gridLayout2->addWidget(doubleClickIntervalLabel, 0, 0, 1, 1);

        cursorFlashTimeSpinBox = new QSpinBox(feelSettingsGroup);
        cursorFlashTimeSpinBox->setObjectName(QString::fromUtf8("cursorFlashTimeSpinBox"));
        cursorFlashTimeSpinBox->setMinimum(9);
        cursorFlashTimeSpinBox->setMaximum(10000);

        gridLayout2->addWidget(cursorFlashTimeSpinBox, 1, 1, 1, 1);

        cursorFlashTimeLabel = new QLabel(feelSettingsGroup);
        cursorFlashTimeLabel->setObjectName(QString::fromUtf8("cursorFlashTimeLabel"));

        gridLayout2->addWidget(cursorFlashTimeLabel, 1, 0, 1, 1);

        wheelScrollLinesSpinBox = new QSpinBox(feelSettingsGroup);
        wheelScrollLinesSpinBox->setObjectName(QString::fromUtf8("wheelScrollLinesSpinBox"));
        wheelScrollLinesSpinBox->setMinimum(1);
        wheelScrollLinesSpinBox->setMaximum(20);

        gridLayout2->addWidget(wheelScrollLinesSpinBox, 2, 1, 1, 1);

        wheelScrollLinesLabel = new QLabel(feelSettingsGroup);
        wheelScrollLinesLabel->setObjectName(QString::fromUtf8("wheelScrollLinesLabel"));

        gridLayout2->addWidget(wheelScrollLinesLabel, 2, 0, 1, 1);

        resolveLinksCheckBox = new QCheckBox(feelSettingsGroup);
        resolveLinksCheckBox->setObjectName(QString::fromUtf8("resolveLinksCheckBox"));

        gridLayout2->addWidget(resolveLinksCheckBox, 3, 0, 1, 2);


        vboxLayout2->addWidget(feelSettingsGroup);

        guiEffectsGroup = new QGroupBox(interfaceTab);
        guiEffectsGroup->setObjectName(QString::fromUtf8("guiEffectsGroup"));
        vboxLayout3 = new QVBoxLayout(guiEffectsGroup);
        vboxLayout3->setSpacing(4);
        vboxLayout3->setContentsMargins(8, 8, 8, 8);
        vboxLayout3->setObjectName(QString::fromUtf8("vboxLayout3"));
        effectsCheckBox = new QCheckBox(guiEffectsGroup);
        effectsCheckBox->setObjectName(QString::fromUtf8("effectsCheckBox"));

        vboxLayout3->addWidget(effectsCheckBox);

        effectsFrame = new QFrame(guiEffectsGroup);
        effectsFrame->setObjectName(QString::fromUtf8("effectsFrame"));
        gridLayout3 = new QGridLayout(effectsFrame);
        gridLayout3->setSpacing(4);
        gridLayout3->setObjectName(QString::fromUtf8("gridLayout3"));
        menuEffectLabel = new QLabel(effectsFrame);
        menuEffectLabel->setObjectName(QString::fromUtf8("menuEffectLabel"));

        gridLayout3->addWidget(menuEffectLabel, 0, 0, 1, 1);

        comboEffectLabel = new QLabel(effectsFrame);
        comboEffectLabel->setObjectName(QString::fromUtf8("comboEffectLabel"));

        gridLayout3->addWidget(comboEffectLabel, 1, 0, 1, 1);

        toolTipEffectLabel = new QLabel(effectsFrame);
        toolTipEffectLabel->setObjectName(QString::fromUtf8("toolTipEffectLabel"));

        gridLayout3->addWidget(toolTipEffectLabel, 2, 0, 1, 1);

        toolBoxEffectLabel = new QLabel(effectsFrame);
        toolBoxEffectLabel->setObjectName(QString::fromUtf8("toolBoxEffectLabel"));

        gridLayout3->addWidget(toolBoxEffectLabel, 3, 0, 1, 1);

        menuEffectCombo = new QComboBox(effectsFrame);
        menuEffectCombo->setObjectName(QString::fromUtf8("menuEffectCombo"));
        menuEffectCombo->setAutoCompletion(true);

        gridLayout3->addWidget(menuEffectCombo, 0, 1, 1, 1);

        comboEffectCombo = new QComboBox(effectsFrame);
        comboEffectCombo->setObjectName(QString::fromUtf8("comboEffectCombo"));

        gridLayout3->addWidget(comboEffectCombo, 1, 1, 1, 1);

        toolTipEffectCombo = new QComboBox(effectsFrame);
        toolTipEffectCombo->setObjectName(QString::fromUtf8("toolTipEffectCombo"));

        gridLayout3->addWidget(toolTipEffectCombo, 2, 1, 1, 1);

        toolBoxEffectCombo = new QComboBox(effectsFrame);
        toolBoxEffectCombo->setObjectName(QString::fromUtf8("toolBoxEffectCombo"));

        gridLayout3->addWidget(toolBoxEffectCombo, 3, 1, 1, 1);


        vboxLayout3->addWidget(effectsFrame);


        vboxLayout2->addWidget(guiEffectsGroup);

        globalStrutGroup = new QGroupBox(interfaceTab);
        globalStrutGroup->setObjectName(QString::fromUtf8("globalStrutGroup"));
        gridLayout4 = new QGridLayout(globalStrutGroup);
        gridLayout4->setSpacing(4);
        gridLayout4->setContentsMargins(8, 8, 8, 8);
        gridLayout4->setObjectName(QString::fromUtf8("gridLayout4"));
        strutWidthLabel = new QLabel(globalStrutGroup);
        strutWidthLabel->setObjectName(QString::fromUtf8("strutWidthLabel"));

        gridLayout4->addWidget(strutWidthLabel, 0, 0, 1, 1);

        strutHeightLabel = new QLabel(globalStrutGroup);
        strutHeightLabel->setObjectName(QString::fromUtf8("strutHeightLabel"));

        gridLayout4->addWidget(strutHeightLabel, 1, 0, 1, 1);

        strutWidthSpinBox = new QSpinBox(globalStrutGroup);
        strutWidthSpinBox->setObjectName(QString::fromUtf8("strutWidthSpinBox"));
        strutWidthSpinBox->setMaximum(1000);

        gridLayout4->addWidget(strutWidthSpinBox, 0, 1, 1, 1);

        strutHeightSpinBox = new QSpinBox(globalStrutGroup);
        strutHeightSpinBox->setObjectName(QString::fromUtf8("strutHeightSpinBox"));
        strutHeightSpinBox->setMaximum(1000);

        gridLayout4->addWidget(strutHeightSpinBox, 1, 1, 1, 1);


        vboxLayout2->addWidget(globalStrutGroup);

        rtlExtensionsCheckBox = new QCheckBox(interfaceTab);
        rtlExtensionsCheckBox->setObjectName(QString::fromUtf8("rtlExtensionsCheckBox"));

        vboxLayout2->addWidget(rtlExtensionsCheckBox);

        inputStyleLabel = new QLabel(interfaceTab);
        inputStyleLabel->setObjectName(QString::fromUtf8("inputStyleLabel"));

        vboxLayout2->addWidget(inputStyleLabel);

        inputStyleCombo = new QComboBox(interfaceTab);
        inputStyleCombo->setObjectName(QString::fromUtf8("inputStyleCombo"));

        vboxLayout2->addWidget(inputStyleCombo);

        inputMethodLabel = new QLabel(interfaceTab);
        inputMethodLabel->setObjectName(QString::fromUtf8("inputMethodLabel"));

        vboxLayout2->addWidget(inputMethodLabel);

        inputMethodCombo = new QComboBox(interfaceTab);
        inputMethodCombo->setObjectName(QString::fromUtf8("inputMethodCombo"));

        vboxLayout2->addWidget(inputMethodCombo);

        spacerItem = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        vboxLayout2->addItem(spacerItem);

        mainTabWidget->addTab(interfaceTab, QString());
        printerTab = new QWidget();
        printerTab->setObjectName(QString::fromUtf8("printerTab"));
        vboxLayout4 = new QVBoxLayout(printerTab);
        vboxLayout4->setObjectName(QString::fromUtf8("vboxLayout4"));
        fontEmbeddingCheckBox = new QCheckBox(printerTab);
        fontEmbeddingCheckBox->setObjectName(QString::fromUtf8("fontEmbeddingCheckBox"));
        fontEmbeddingCheckBox->setChecked(true);

        vboxLayout4->addWidget(fontEmbeddingCheckBox);

        fontPathsGroup = new QGroupBox(printerTab);
        fontPathsGroup->setObjectName(QString::fromUtf8("fontPathsGroup"));
        sizePolicy1.setHeightForWidth(fontPathsGroup->sizePolicy().hasHeightForWidth());
        fontPathsGroup->setSizePolicy(sizePolicy1);
        vboxLayout5 = new QVBoxLayout(fontPathsGroup);
        vboxLayout5->setSpacing(4);
        vboxLayout5->setContentsMargins(8, 8, 8, 8);
        vboxLayout5->setObjectName(QString::fromUtf8("vboxLayout5"));
        gridLayout5 = new QGridLayout();
        gridLayout5->setSpacing(4);
#ifndef Q_OS_MAC
        gridLayout5->setContentsMargins(0, 0, 0, 0);
#endif
        gridLayout5->setObjectName(QString::fromUtf8("gridLayout5"));
        upFontpathButton = new QPushButton(fontPathsGroup);
        upFontpathButton->setObjectName(QString::fromUtf8("upFontpathButton"));

        gridLayout5->addWidget(upFontpathButton, 1, 0, 1, 1);

        removeFontpathButton = new QPushButton(fontPathsGroup);
        removeFontpathButton->setObjectName(QString::fromUtf8("removeFontpathButton"));

        gridLayout5->addWidget(removeFontpathButton, 1, 2, 1, 1);

        downFontpathButton = new QPushButton(fontPathsGroup);
        downFontpathButton->setObjectName(QString::fromUtf8("downFontpathButton"));

        gridLayout5->addWidget(downFontpathButton, 1, 1, 1, 1);

        fontpathListBox = new QListWidget(fontPathsGroup);
        fontpathListBox->setObjectName(QString::fromUtf8("fontpathListBox"));

        gridLayout5->addWidget(fontpathListBox, 0, 0, 1, 3);


        vboxLayout5->addLayout(gridLayout5);

        gridLayout6 = new QGridLayout();
        gridLayout6->setSpacing(4);
        gridLayout6->setContentsMargins(0, 0, 0, 0);
        gridLayout6->setObjectName(QString::fromUtf8("gridLayout6"));
        spacerItem1 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        gridLayout6->addItem(spacerItem1, 2, 0, 1, 1);

        addFontPathButton = new QPushButton(fontPathsGroup);
        addFontPathButton->setObjectName(QString::fromUtf8("addFontPathButton"));

        gridLayout6->addWidget(addFontPathButton, 2, 2, 1, 1);

        browseFontPathButton = new QPushButton(fontPathsGroup);
        browseFontPathButton->setObjectName(QString::fromUtf8("browseFontPathButton"));

        gridLayout6->addWidget(browseFontPathButton, 2, 1, 1, 1);

        browseFontPathLabel = new QLabel(fontPathsGroup);
        browseFontPathLabel->setObjectName(QString::fromUtf8("browseFontPathLabel"));

        gridLayout6->addWidget(browseFontPathLabel, 0, 0, 1, 3);

        fontPathLineEdit = new QLineEdit(fontPathsGroup);
        fontPathLineEdit->setObjectName(QString::fromUtf8("fontPathLineEdit"));

        gridLayout6->addWidget(fontPathLineEdit, 1, 0, 1, 3);


        vboxLayout5->addLayout(gridLayout6);


        vboxLayout4->addWidget(fontPathsGroup);

        mainTabWidget->addTab(printerTab, QString());
        phononTab = new QWidget();
        phononTab->setObjectName(QString::fromUtf8("phononTab"));
        verticalLayout = new QVBoxLayout(phononTab);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        aboutPhononGroup = new QGroupBox(phononTab);
        aboutPhononGroup->setObjectName(QString::fromUtf8("aboutPhononGroup"));
        gridLayout_2 = new QGridLayout(aboutPhononGroup);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        phononVersionBuddyLabel = new QLabel(aboutPhononGroup);
        phononVersionBuddyLabel->setObjectName(QString::fromUtf8("phononVersionBuddyLabel"));

        gridLayout_2->addWidget(phononVersionBuddyLabel, 0, 0, 1, 1);

        phononVersionLabel = new QLabel(aboutPhononGroup);
        phononVersionLabel->setObjectName(QString::fromUtf8("phononVersionLabel"));

        gridLayout_2->addWidget(phononVersionLabel, 0, 1, 1, 1);

        phononWebsiteBuddyLabel = new QLabel(aboutPhononGroup);
        phononWebsiteBuddyLabel->setObjectName(QString::fromUtf8("phononWebsiteBuddyLabel"));

        gridLayout_2->addWidget(phononWebsiteBuddyLabel, 1, 0, 1, 1);

        phononWebsiteLabel = new QLabel(aboutPhononGroup);
        phononWebsiteLabel->setObjectName(QString::fromUtf8("phononWebsiteLabel"));
        phononWebsiteLabel->setOpenExternalLinks(true);

        gridLayout_2->addWidget(phononWebsiteLabel, 1, 1, 1, 1);


        verticalLayout->addWidget(aboutPhononGroup);

        aboutGStreamerGroup = new QGroupBox(phononTab);
        aboutGStreamerGroup->setObjectName(QString::fromUtf8("aboutGStreamerGroup"));
        gridLayout7 = new QGridLayout(aboutGStreamerGroup);
        gridLayout7->setObjectName(QString::fromUtf8("gridLayout7"));
        gstVersionBuddyLabel = new QLabel(aboutGStreamerGroup);
        gstVersionBuddyLabel->setObjectName(QString::fromUtf8("gstVersionBuddyLabel"));

        gridLayout7->addWidget(gstVersionBuddyLabel, 0, 0, 1, 1);

        gstVersionLabel = new QLabel(aboutGStreamerGroup);
        gstVersionLabel->setObjectName(QString::fromUtf8("gstVersionLabel"));

        gridLayout7->addWidget(gstVersionLabel, 0, 1, 1, 1);

        gstWebsiteBuddyLabel = new QLabel(aboutGStreamerGroup);
        gstWebsiteBuddyLabel->setObjectName(QString::fromUtf8("gstWebsiteBuddyLabel"));

        gridLayout7->addWidget(gstWebsiteBuddyLabel, 1, 0, 1, 1);

        gstWebsiteLabel = new QLabel(aboutGStreamerGroup);
        gstWebsiteLabel->setObjectName(QString::fromUtf8("gstWebsiteLabel"));
        gstWebsiteLabel->setOpenExternalLinks(true);

        gridLayout7->addWidget(gstWebsiteLabel, 1, 1, 1, 1);


        verticalLayout->addWidget(aboutGStreamerGroup);

        gstBackendGroup = new QGroupBox(phononTab);
        gstBackendGroup->setObjectName(QString::fromUtf8("gstBackendGroup"));
        gridLayout_3 = new QGridLayout(gstBackendGroup);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        audiosinkLabel = new QLabel(gstBackendGroup);
        audiosinkLabel->setObjectName(QString::fromUtf8("audiosinkLabel"));

        gridLayout_3->addWidget(audiosinkLabel, 0, 0, 1, 1);

        audiosinkCombo = new QComboBox(gstBackendGroup);
        audiosinkCombo->setObjectName(QString::fromUtf8("audiosinkCombo"));

        gridLayout_3->addWidget(audiosinkCombo, 0, 1, 1, 1);

        videomodeLabel = new QLabel(gstBackendGroup);
        videomodeLabel->setObjectName(QString::fromUtf8("videomodeLabel"));

        gridLayout_3->addWidget(videomodeLabel, 1, 0, 1, 1);

        videomodeCombo = new QComboBox(gstBackendGroup);
        videomodeCombo->setObjectName(QString::fromUtf8("videomodeCombo"));

        gridLayout_3->addWidget(videomodeCombo, 1, 1, 1, 1);

        gstBackendNoteLabel = new QLabel(gstBackendGroup);
        gstBackendNoteLabel->setObjectName(QString::fromUtf8("gstBackendNoteLabel"));
        gstBackendNoteLabel->setTextFormat(Qt::RichText);
        gstBackendNoteLabel->setScaledContents(false);
        gstBackendNoteLabel->setWordWrap(true);
        gstBackendNoteLabel->setMargin(2);

        gridLayout_3->addWidget(gstBackendNoteLabel, 2, 0, 1, 2);


        verticalLayout->addWidget(gstBackendGroup);

        phononLabel = new QLabel(phononTab);
        phononLabel->setObjectName(QString::fromUtf8("phononLabel"));

        verticalLayout->addWidget(phononLabel);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        mainTabWidget->addTab(phononTab, QString());

        gridLayout->addWidget(mainTabWidget, 0, 1, 1, 1);

        MainWindow->setCentralWidget(widget);
        mainMenu = new QMenuBar(MainWindow);
        mainMenu->setObjectName(QString::fromUtf8("mainMenu"));
        mainMenu->setGeometry(QRect(0, 0, 815, 19));
        fileMenu = new QMenu(mainMenu);
        fileMenu->setObjectName(QString::fromUtf8("fileMenu"));
        fileMenu->setGeometry(QRect(203, 114, 161, 110));
        saveMenu = new QMenu(mainMenu);
        saveMenu->setObjectName(QString::fromUtf8("saveMenu"));
        saveMenu->setGeometry(QRect(543, 98, 161, 106));
        MainWindow->setMenuBar(mainMenu);
#ifndef QT_NO_SHORTCUT
        guiStyleLabel->setBuddy(guiStyleCombo);
        paletteLabel->setBuddy(paletteCombo);
        labelMainColor->setBuddy(buttonMainColor);
        labelWindowColor->setBuddy(buttonWindowColor);
        fontStyleLabel->setBuddy(fontStyleCombo);
        pointSizeLabel->setBuddy(pointSizeCombo);
        fontFamilyLabel->setBuddy(fontFamilyCombo);
        familySubstitutionLabel->setBuddy(familySubstitutionCombo);
        chooseSubstitutionLabel->setBuddy(chooseSubstitutionCombo);
        doubleClickIntervalLabel->setBuddy(doubleClickIntervalSpinBox);
        cursorFlashTimeLabel->setBuddy(cursorFlashTimeSpinBox);
        wheelScrollLinesLabel->setBuddy(wheelScrollLinesSpinBox);
        menuEffectLabel->setBuddy(menuEffectCombo);
        comboEffectLabel->setBuddy(comboEffectCombo);
        toolTipEffectLabel->setBuddy(toolTipEffectCombo);
        toolBoxEffectLabel->setBuddy(toolBoxEffectCombo);
        strutWidthLabel->setBuddy(strutWidthSpinBox);
        strutHeightLabel->setBuddy(strutHeightSpinBox);
        audiosinkLabel->setBuddy(audiosinkCombo);
        videomodeLabel->setBuddy(videomodeCombo);
#endif // QT_NO_SHORTCUT
        QWidget::setTabOrder(helpView, mainTabWidget);
        QWidget::setTabOrder(mainTabWidget, guiStyleCombo);
        QWidget::setTabOrder(guiStyleCombo, tunePaletteButton);
        QWidget::setTabOrder(tunePaletteButton, paletteCombo);
        QWidget::setTabOrder(paletteCombo, fontFamilyCombo);
        QWidget::setTabOrder(fontFamilyCombo, fontStyleCombo);
        QWidget::setTabOrder(fontStyleCombo, pointSizeCombo);
        QWidget::setTabOrder(pointSizeCombo, sampleLineEdit);
        QWidget::setTabOrder(sampleLineEdit, familySubstitutionCombo);
        QWidget::setTabOrder(familySubstitutionCombo, substitutionsListBox);
        QWidget::setTabOrder(substitutionsListBox, upSubstitutionButton);
        QWidget::setTabOrder(upSubstitutionButton, downSubstitutionButton);
        QWidget::setTabOrder(downSubstitutionButton, removeSubstitutionButton);
        QWidget::setTabOrder(removeSubstitutionButton, chooseSubstitutionCombo);
        QWidget::setTabOrder(chooseSubstitutionCombo, addSubstitutionButton);
        QWidget::setTabOrder(addSubstitutionButton, doubleClickIntervalSpinBox);
        QWidget::setTabOrder(doubleClickIntervalSpinBox, cursorFlashTimeSpinBox);
        QWidget::setTabOrder(cursorFlashTimeSpinBox, wheelScrollLinesSpinBox);
        QWidget::setTabOrder(wheelScrollLinesSpinBox, resolveLinksCheckBox);
        QWidget::setTabOrder(resolveLinksCheckBox, effectsCheckBox);
        QWidget::setTabOrder(effectsCheckBox, menuEffectCombo);
        QWidget::setTabOrder(menuEffectCombo, comboEffectCombo);
        QWidget::setTabOrder(comboEffectCombo, toolTipEffectCombo);
        QWidget::setTabOrder(toolTipEffectCombo, toolBoxEffectCombo);
        QWidget::setTabOrder(toolBoxEffectCombo, strutWidthSpinBox);
        QWidget::setTabOrder(strutWidthSpinBox, strutHeightSpinBox);
        QWidget::setTabOrder(strutHeightSpinBox, rtlExtensionsCheckBox);
        QWidget::setTabOrder(rtlExtensionsCheckBox, inputStyleCombo);
        QWidget::setTabOrder(inputStyleCombo, inputMethodCombo);
        QWidget::setTabOrder(inputMethodCombo, fontEmbeddingCheckBox);
        QWidget::setTabOrder(fontEmbeddingCheckBox, fontpathListBox);
        QWidget::setTabOrder(fontpathListBox, upFontpathButton);
        QWidget::setTabOrder(upFontpathButton, downFontpathButton);
        QWidget::setTabOrder(downFontpathButton, removeFontpathButton);
        QWidget::setTabOrder(removeFontpathButton, fontPathLineEdit);
        QWidget::setTabOrder(fontPathLineEdit, browseFontPathButton);
        QWidget::setTabOrder(browseFontPathButton, addFontPathButton);
        QWidget::setTabOrder(addFontPathButton, audiosinkCombo);
        QWidget::setTabOrder(audiosinkCombo, videomodeCombo);

        mainMenu->addAction(fileMenu->menuAction());
        mainMenu->addSeparator();
        mainMenu->addAction(saveMenu->menuAction());
        fileMenu->addAction(fileSaveAction);
        fileMenu->addSeparator();
        fileMenu->addAction(fileExitAction);
        saveMenu->addAction(helpAboutAction);
        saveMenu->addAction(helpAboutQtAction);

        retranslateUi(MainWindow);
        QObject::connect(effectsCheckBox, SIGNAL(toggled(bool)), effectsFrame, SLOT(setEnabled(bool)));
        QObject::connect(fontEmbeddingCheckBox, SIGNAL(toggled(bool)), fontPathsGroup, SLOT(setEnabled(bool)));

        mainTabWidget->setCurrentIndex(0);
        menuEffectCombo->setCurrentIndex(0);
        inputStyleCombo->setCurrentIndex(0);
        inputMethodCombo->setCurrentIndex(-1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Qt Configuration", 0, QApplication::UnicodeUTF8));
        fileSaveAction->setText(QApplication::translate("MainWindow", "&Save", 0, QApplication::UnicodeUTF8));
        fileSaveAction->setIconText(QApplication::translate("MainWindow", "Save", 0, QApplication::UnicodeUTF8));
        fileSaveAction->setShortcut(QApplication::translate("MainWindow", "Ctrl+S", 0, QApplication::UnicodeUTF8));
        fileExitAction->setText(QApplication::translate("MainWindow", "E&xit", 0, QApplication::UnicodeUTF8));
        fileExitAction->setIconText(QApplication::translate("MainWindow", "Exit", 0, QApplication::UnicodeUTF8));
        fileExitAction->setShortcut(QApplication::translate("MainWindow", "Ctrl+Q", 0, QApplication::UnicodeUTF8));
        helpAboutAction->setText(QApplication::translate("MainWindow", "&About", 0, QApplication::UnicodeUTF8));
        helpAboutAction->setIconText(QApplication::translate("MainWindow", "About", 0, QApplication::UnicodeUTF8));
        helpAboutAction->setShortcut(QString());
        helpAboutQtAction->setText(QApplication::translate("MainWindow", "About &Qt", 0, QApplication::UnicodeUTF8));
        helpAboutQtAction->setIconText(QApplication::translate("MainWindow", "About Qt", 0, QApplication::UnicodeUTF8));
        guiStyleGroup->setTitle(QApplication::translate("MainWindow", "GUI Style", 0, QApplication::UnicodeUTF8));
        guiStyleLabel->setText(QApplication::translate("MainWindow", "Select GUI &Style:", 0, QApplication::UnicodeUTF8));
        previewGroup->setTitle(QApplication::translate("MainWindow", "Preview", 0, QApplication::UnicodeUTF8));
        paletteLabel->setText(QApplication::translate("MainWindow", "Select &Palette:", 0, QApplication::UnicodeUTF8));
        paletteCombo->clear();
        paletteCombo->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Active Palette", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Inactive Palette", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Disabled Palette", 0, QApplication::UnicodeUTF8)
        );
        buildPaletteGroup->setTitle(QApplication::translate("MainWindow", "Build Palette", 0, QApplication::UnicodeUTF8));
        labelMainColor->setText(QApplication::translate("MainWindow", "&Button Background:", 0, QApplication::UnicodeUTF8));
        labelWindowColor->setText(QApplication::translate("MainWindow", "Window Back&ground:", 0, QApplication::UnicodeUTF8));
        tunePaletteButton->setText(QApplication::translate("MainWindow", "&Tune Palette...", 0, QApplication::UnicodeUTF8));
        kdeNoteLabel->setText(QApplication::translate("MainWindow", "Please use the KDE Control Center to set the palette.", 0, QApplication::UnicodeUTF8));
        mainTabWidget->setTabText(mainTabWidget->indexOf(appearanceTab), QApplication::translate("MainWindow", "Appearance", 0, QApplication::UnicodeUTF8));
        defaultFontGroup->setTitle(QApplication::translate("MainWindow", "Default Font", 0, QApplication::UnicodeUTF8));
        fontStyleLabel->setText(QApplication::translate("MainWindow", "&Style:", 0, QApplication::UnicodeUTF8));
        pointSizeLabel->setText(QApplication::translate("MainWindow", "&Point Size:", 0, QApplication::UnicodeUTF8));
        fontFamilyLabel->setText(QApplication::translate("MainWindow", "F&amily:", 0, QApplication::UnicodeUTF8));
        sampleLineEdit->setText(QApplication::translate("MainWindow", "Sample Text", 0, QApplication::UnicodeUTF8));
        fontSubstitutionGroup->setTitle(QApplication::translate("MainWindow", "Font Substitution", 0, QApplication::UnicodeUTF8));
        familySubstitutionLabel->setText(QApplication::translate("MainWindow", "S&elect or Enter a Family:", 0, QApplication::UnicodeUTF8));
        substitutionsLabel->setText(QApplication::translate("MainWindow", "Current Substitutions:", 0, QApplication::UnicodeUTF8));
        upSubstitutionButton->setText(QApplication::translate("MainWindow", "Up", 0, QApplication::UnicodeUTF8));
        downSubstitutionButton->setText(QApplication::translate("MainWindow", "Down", 0, QApplication::UnicodeUTF8));
        removeSubstitutionButton->setText(QApplication::translate("MainWindow", "Remove", 0, QApplication::UnicodeUTF8));
        chooseSubstitutionLabel->setText(QApplication::translate("MainWindow", "Select s&ubstitute Family:", 0, QApplication::UnicodeUTF8));
        addSubstitutionButton->setText(QApplication::translate("MainWindow", "Add", 0, QApplication::UnicodeUTF8));
        mainTabWidget->setTabText(mainTabWidget->indexOf(fontsTab), QApplication::translate("MainWindow", "Fonts", 0, QApplication::UnicodeUTF8));
        feelSettingsGroup->setTitle(QApplication::translate("MainWindow", "Feel Settings", 0, QApplication::UnicodeUTF8));
        doubleClickIntervalSpinBox->setSuffix(QApplication::translate("MainWindow", " ms", 0, QApplication::UnicodeUTF8));
        doubleClickIntervalLabel->setText(QApplication::translate("MainWindow", "&Double Click Interval:", 0, QApplication::UnicodeUTF8));
        cursorFlashTimeSpinBox->setSpecialValueText(QApplication::translate("MainWindow", "No blinking", 0, QApplication::UnicodeUTF8));
        cursorFlashTimeSpinBox->setSuffix(QApplication::translate("MainWindow", " ms", 0, QApplication::UnicodeUTF8));
        cursorFlashTimeLabel->setText(QApplication::translate("MainWindow", "&Cursor Flash Time:", 0, QApplication::UnicodeUTF8));
        wheelScrollLinesSpinBox->setSuffix(QApplication::translate("MainWindow", " lines", 0, QApplication::UnicodeUTF8));
        wheelScrollLinesLabel->setText(QApplication::translate("MainWindow", "Wheel &Scroll Lines:", 0, QApplication::UnicodeUTF8));
        resolveLinksCheckBox->setText(QApplication::translate("MainWindow", "Resolve symlinks in URLs", 0, QApplication::UnicodeUTF8));
        guiEffectsGroup->setTitle(QApplication::translate("MainWindow", "GUI Effects", 0, QApplication::UnicodeUTF8));
        effectsCheckBox->setText(QApplication::translate("MainWindow", "&Enable", 0, QApplication::UnicodeUTF8));
        effectsCheckBox->setShortcut(QApplication::translate("MainWindow", "Alt+E", 0, QApplication::UnicodeUTF8));
        menuEffectLabel->setText(QApplication::translate("MainWindow", "&Menu Effect:", 0, QApplication::UnicodeUTF8));
        comboEffectLabel->setText(QApplication::translate("MainWindow", "C&omboBox Effect:", 0, QApplication::UnicodeUTF8));
        toolTipEffectLabel->setText(QApplication::translate("MainWindow", "&ToolTip Effect:", 0, QApplication::UnicodeUTF8));
        toolBoxEffectLabel->setText(QApplication::translate("MainWindow", "Tool&Box Effect:", 0, QApplication::UnicodeUTF8));
        menuEffectCombo->clear();
        menuEffectCombo->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Disable", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Animate", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Fade", 0, QApplication::UnicodeUTF8)
        );
        comboEffectCombo->clear();
        comboEffectCombo->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Disable", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Animate", 0, QApplication::UnicodeUTF8)
        );
        toolTipEffectCombo->clear();
        toolTipEffectCombo->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Disable", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Animate", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Fade", 0, QApplication::UnicodeUTF8)
        );
        toolBoxEffectCombo->clear();
        toolBoxEffectCombo->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Disable", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Animate", 0, QApplication::UnicodeUTF8)
        );
        globalStrutGroup->setTitle(QApplication::translate("MainWindow", "Global Strut", 0, QApplication::UnicodeUTF8));
        strutWidthLabel->setText(QApplication::translate("MainWindow", "Minimum &Width:", 0, QApplication::UnicodeUTF8));
        strutHeightLabel->setText(QApplication::translate("MainWindow", "Minimum Hei&ght:", 0, QApplication::UnicodeUTF8));
        strutWidthSpinBox->setSuffix(QApplication::translate("MainWindow", " pixels", 0, QApplication::UnicodeUTF8));
        strutHeightSpinBox->setSuffix(QApplication::translate("MainWindow", " pixels", 0, QApplication::UnicodeUTF8));
        rtlExtensionsCheckBox->setText(QApplication::translate("MainWindow", "Enhanced support for languages written right-to-left", 0, QApplication::UnicodeUTF8));
        inputStyleLabel->setText(QApplication::translate("MainWindow", "XIM Input Style:", 0, QApplication::UnicodeUTF8));
        inputStyleCombo->clear();
        inputStyleCombo->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "On The Spot", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Over The Spot", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Off The Spot", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Root", 0, QApplication::UnicodeUTF8)
        );
        inputMethodLabel->setText(QApplication::translate("MainWindow", "Default Input Method:", 0, QApplication::UnicodeUTF8));
        mainTabWidget->setTabText(mainTabWidget->indexOf(interfaceTab), QApplication::translate("MainWindow", "Interface", 0, QApplication::UnicodeUTF8));
        fontEmbeddingCheckBox->setText(QApplication::translate("MainWindow", "Enable Font embedding", 0, QApplication::UnicodeUTF8));
        fontPathsGroup->setTitle(QApplication::translate("MainWindow", "Font Paths", 0, QApplication::UnicodeUTF8));
        upFontpathButton->setText(QApplication::translate("MainWindow", "Up", 0, QApplication::UnicodeUTF8));
        removeFontpathButton->setText(QApplication::translate("MainWindow", "Remove", 0, QApplication::UnicodeUTF8));
        downFontpathButton->setText(QApplication::translate("MainWindow", "Down", 0, QApplication::UnicodeUTF8));
        addFontPathButton->setText(QApplication::translate("MainWindow", "Add", 0, QApplication::UnicodeUTF8));
        browseFontPathButton->setText(QApplication::translate("MainWindow", "Browse...", 0, QApplication::UnicodeUTF8));
        browseFontPathLabel->setText(QApplication::translate("MainWindow", "Press the <b>Browse</b> button or enter a directory and press Enter to add them to the list.", 0, QApplication::UnicodeUTF8));
        mainTabWidget->setTabText(mainTabWidget->indexOf(printerTab), QApplication::translate("MainWindow", "Printer", 0, QApplication::UnicodeUTF8));
        aboutPhononGroup->setTitle(QApplication::translate("MainWindow", "About Phonon", 0, QApplication::UnicodeUTF8));
        phononVersionBuddyLabel->setText(QApplication::translate("MainWindow", "Current Version:", 0, QApplication::UnicodeUTF8));
        phononVersionLabel->setText(QApplication::translate("MainWindow", "Not available", 0, QApplication::UnicodeUTF8));
        phononWebsiteBuddyLabel->setText(QApplication::translate("MainWindow", "Website:", 0, QApplication::UnicodeUTF8));
        phononWebsiteLabel->setText(QApplication::translate("MainWindow", "<a href=\"http://phonon.kde.org\">http://phonon.kde.org/</a>", 0, QApplication::UnicodeUTF8));
        aboutGStreamerGroup->setTitle(QApplication::translate("MainWindow", "About GStreamer", 0, QApplication::UnicodeUTF8));
        gstVersionBuddyLabel->setText(QApplication::translate("MainWindow", "Current Version:", 0, QApplication::UnicodeUTF8));
        gstVersionLabel->setText(QApplication::translate("MainWindow", "Not available", 0, QApplication::UnicodeUTF8));
        gstWebsiteBuddyLabel->setText(QApplication::translate("MainWindow", "Website:", 0, QApplication::UnicodeUTF8));
        gstWebsiteLabel->setText(QApplication::translate("MainWindow", "<a href=\"http://gstreamer.freedesktop.org/\">http://gstreamer.freedesktop.org/</a>", 0, QApplication::UnicodeUTF8));
        gstBackendGroup->setTitle(QApplication::translate("MainWindow", "GStreamer backend settings", 0, QApplication::UnicodeUTF8));
        audiosinkLabel->setText(QApplication::translate("MainWindow", "Preferred audio sink:", 0, QApplication::UnicodeUTF8));
        videomodeLabel->setText(QApplication::translate("MainWindow", "Preferred render method:", 0, QApplication::UnicodeUTF8));
        gstBackendNoteLabel->setText(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-style:italic;\">Note: changes to these settings may prevent applications from starting up correctly.</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        phononLabel->setText(QString());
        mainTabWidget->setTabText(mainTabWidget->indexOf(phononTab), QApplication::translate("MainWindow", "Phonon", 0, QApplication::UnicodeUTF8));
        fileMenu->setTitle(QApplication::translate("MainWindow", "&File", 0, QApplication::UnicodeUTF8));
        saveMenu->setTitle(QApplication::translate("MainWindow", "&Help", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
