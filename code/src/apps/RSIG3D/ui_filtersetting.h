/********************************************************************************
** Form generated from reading UI file 'filtersetting.ui'
**
** Created: Thu Jun 12 13:29:43 2025
**      by: Qt User Interface Compiler version 4.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FILTERSETTING_H
#define UI_FILTERSETTING_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSpinBox>
#include <QtGui/QTabWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FilterSettingDialog
{
public:
    QPushButton *acceptPushButton;
    QPushButton *cancelPushButton;
    QPushButton *resetPushButton;
    QTabWidget *tabWidget;
    QWidget *gaspTab;
    QDoubleSpinBox *aodstdDSpinBox;
    QDoubleSpinBox *mosDSpinBox;
    QLabel *label_17;
    QDoubleSpinBox *sigDSpinBox;
    QLabel *label_18;
    QDoubleSpinBox *sigDSpinBox2;
    QLabel *label_19;
    QLabel *label_20;
    QDoubleSpinBox *aodDSpinBox2;
    QDoubleSpinBox *clsDSpinBox2;
    QDoubleSpinBox *sfcDSpinBox;
    QDoubleSpinBox *sfcDSpinBox2;
    QDoubleSpinBox *aodDSpinBox;
    QDoubleSpinBox *mosDSpinBox2;
    QLabel *label_21;
    QLabel *label_22;
    QDoubleSpinBox *ch1DSpinBox;
    QDoubleSpinBox *ch1DSpinBox2;
    QLabel *label_23;
    QLabel *label_25;
    QDoubleSpinBox *aodstdDSpinBox2;
    QLabel *label_26;
    QDoubleSpinBox *clsDSpinBox;
    QLabel *label_27;
    QLabel *label_28;
    QDoubleSpinBox *scaDSpinBox;
    QDoubleSpinBox *scaDSpinBox2;
    QLabel *label_24;
    QWidget *calipsoTab;
    QSpinBox *mincadSpinBox;
    QLabel *label_29;
    QLabel *label_30;
    QDoubleSpinBox *maxuncetaintyDSpinBox;
    QLabel *label_31;
    QLabel *label_32;
    QLabel *label_35;
    QLabel *label_36;
    QComboBox *calipsoVersionL1Combobox;
    QComboBox *calipsoVersionL2Combobox;
    QLabel *label_2;
    QWidget *OMITab;
    QDoubleSpinBox *maxOMICloudFractionSpinBox;
    QLabel *label_OMI1;
    QDoubleSpinBox *maxOMISolarZenithAngleSpinBox;
    QLabel *label_OMI2;
    QCheckBox *OMIAllowNegativeCounts;
    QWidget *tropomiTab;
    QSpinBox *minTropomiQualitySpinBox;
    QLabel *label_33;
    QDoubleSpinBox *maxTropomiCloudFractionSpinBox;
    QLabel *label_34;
    QSpinBox *TropomiGroundPixelMinimumSpinBox;
    QLabel *TropomiGroundPixelMinimumLabel;
    QSpinBox *TropomiGroundPixelMaximumSpinBox;
    QLabel *TropomiGroundPixelMaximumLabel;
    QCheckBox *TropomiAllowNegativeCounts;
    QWidget *viirsnoaaTab;
    QLabel *viirs_label;
    QComboBox *viirsNoaaQAflagMenu;
    QWidget *pandoraTab;
    QLabel *pandora_label;
    QComboBox *pandoraQAflagMenu;
    QWidget *purpleairTab;
    QLabel *apikeyPurpleAirLabel;
    QLineEdit *apikeyPurpleAir;
    QLabel *PurpleAirLabel_3;
    QSpinBox *minPurpleairCountPercentage;
    QSpinBox *purpleairHumidityReplacementValue;
    QLabel *PurpleAirLabel_4;
    QCheckBox *usePurpleairHumidityReplacement;
    QGroupBox *groupBox;
    QRadioButton *purpleairLocationOutdoor;
    QRadioButton *purpleairLocationIndoor;
    QRadioButton *purpleairLocationIndoorOutdoor;
    QLabel *PurpleAirLabel_9;
    QGroupBox *groupBox_3;
    QLabel *PurpleAirLabel_8;
    QSpinBox *acceptablePurpleairChannelDifference;
    QLabel *PurpleAirLabel_7;
    QSpinBox *acceptablePurpleairChannelAgreement;
    QLabel *PurpleAirLabel_5;
    QLabel *PurpleAirLabel_6;
    QWidget *TEMPOTab;
    QLabel *TEMPO_label;
    QComboBox *TEMPOQAflagMenu;
    QDoubleSpinBox *maxTEMPOCloudFractionSpinBox;
    QLabel *label_37;
    QDoubleSpinBox *maxTEMPOSolarZenithAngleSpinBox;
    QLabel *label_38;
    QLabel *TEMPOKeyLabel;
    QLineEdit *TEMPOKey;
    QCheckBox *TEMPOAllowNegativeCounts;
    QWidget *OAQPSTab;
    QLabel *OAQPSKeyLabel;
    QLineEdit *OAQPSKey;

    void setupUi(QDialog *FilterSettingDialog)
    {
        if (FilterSettingDialog->objectName().isEmpty())
            FilterSettingDialog->setObjectName(QString::fromUtf8("FilterSettingDialog"));
        FilterSettingDialog->resize(900, 434);
        FilterSettingDialog->setMinimumSize(QSize(600, 434));
        FilterSettingDialog->setMaximumSize(QSize(1024, 438));
        acceptPushButton = new QPushButton(FilterSettingDialog);
        acceptPushButton->setObjectName(QString::fromUtf8("acceptPushButton"));
        acceptPushButton->setGeometry(QRect(284, 400, 92, 24));
        acceptPushButton->setMinimumSize(QSize(92, 24));
        acceptPushButton->setMaximumSize(QSize(92, 24));
        acceptPushButton->setAutoDefault(false);
        acceptPushButton->setDefault(false);
        cancelPushButton = new QPushButton(FilterSettingDialog);
        cancelPushButton->setObjectName(QString::fromUtf8("cancelPushButton"));
        cancelPushButton->setGeometry(QRect(148, 400, 92, 24));
        cancelPushButton->setMinimumSize(QSize(92, 24));
        cancelPushButton->setMaximumSize(QSize(92, 24));
        cancelPushButton->setAutoDefault(false);
        resetPushButton = new QPushButton(FilterSettingDialog);
        resetPushButton->setObjectName(QString::fromUtf8("resetPushButton"));
        resetPushButton->setGeometry(QRect(56, 400, 92, 24));
        resetPushButton->setMinimumSize(QSize(92, 24));
        resetPushButton->setMaximumSize(QSize(92, 24));
        resetPushButton->setAutoDefault(false);
        tabWidget = new QTabWidget(FilterSettingDialog);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(8, 4, 900, 375));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(tabWidget->sizePolicy().hasHeightForWidth());
        tabWidget->setSizePolicy(sizePolicy);
        tabWidget->setMinimumSize(QSize(700, 375));
        tabWidget->setMaximumSize(QSize(1024, 375));
        gaspTab = new QWidget();
        gaspTab->setObjectName(QString::fromUtf8("gaspTab"));
        aodstdDSpinBox = new QDoubleSpinBox(gaspTab);
        aodstdDSpinBox->setObjectName(QString::fromUtf8("aodstdDSpinBox"));
        aodstdDSpinBox->setGeometry(QRect(88, 76, 120, 24));
        aodstdDSpinBox->setMinimumSize(QSize(120, 24));
        aodstdDSpinBox->setMaximumSize(QSize(120, 24));
        aodstdDSpinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
        aodstdDSpinBox->setDecimals(3);
        aodstdDSpinBox->setMinimum(0);
        aodstdDSpinBox->setMaximum(2.55);
        mosDSpinBox = new QDoubleSpinBox(gaspTab);
        mosDSpinBox->setObjectName(QString::fromUtf8("mosDSpinBox"));
        mosDSpinBox->setGeometry(QRect(88, 196, 120, 24));
        mosDSpinBox->setMinimumSize(QSize(120, 24));
        mosDSpinBox->setMaximumSize(QSize(120, 24));
        mosDSpinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
        mosDSpinBox->setDecimals(3);
        mosDSpinBox->setMinimum(0);
        mosDSpinBox->setMaximum(0.425);
        label_17 = new QLabel(gaspTab);
        label_17->setObjectName(QString::fromUtf8("label_17"));
        label_17->setGeometry(QRect(12, 128, 69, 16));
        sigDSpinBox = new QDoubleSpinBox(gaspTab);
        sigDSpinBox->setObjectName(QString::fromUtf8("sigDSpinBox"));
        sigDSpinBox->setGeometry(QRect(88, 148, 120, 24));
        sigDSpinBox->setMinimumSize(QSize(120, 24));
        sigDSpinBox->setMaximumSize(QSize(120, 24));
        sigDSpinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
        sigDSpinBox->setDecimals(3);
        sigDSpinBox->setMinimum(-0.5);
        sigDSpinBox->setMaximum(0.52);
        sigDSpinBox->setValue(-0.5);
        label_18 = new QLabel(gaspTab);
        label_18->setObjectName(QString::fromUtf8("label_18"));
        label_18->setGeometry(QRect(12, 56, 69, 16));
        sigDSpinBox2 = new QDoubleSpinBox(gaspTab);
        sigDSpinBox2->setObjectName(QString::fromUtf8("sigDSpinBox2"));
        sigDSpinBox2->setGeometry(QRect(236, 148, 120, 24));
        sigDSpinBox2->setMinimumSize(QSize(120, 24));
        sigDSpinBox2->setMaximumSize(QSize(120, 24));
        sigDSpinBox2->setButtonSymbols(QAbstractSpinBox::NoButtons);
        sigDSpinBox2->setDecimals(3);
        sigDSpinBox2->setMinimum(-1e+08);
        sigDSpinBox2->setMaximum(1e+08);
        sigDSpinBox2->setValue(0.52);
        label_19 = new QLabel(gaspTab);
        label_19->setObjectName(QString::fromUtf8("label_19"));
        label_19->setGeometry(QRect(132, 32, 53, 16));
        label_20 = new QLabel(gaspTab);
        label_20->setObjectName(QString::fromUtf8("label_20"));
        label_20->setGeometry(QRect(12, 176, 69, 16));
        aodDSpinBox2 = new QDoubleSpinBox(gaspTab);
        aodDSpinBox2->setObjectName(QString::fromUtf8("aodDSpinBox2"));
        aodDSpinBox2->setGeometry(QRect(236, 52, 120, 24));
        aodDSpinBox2->setMinimumSize(QSize(120, 24));
        aodDSpinBox2->setMaximumSize(QSize(120, 24));
        aodDSpinBox2->setButtonSymbols(QAbstractSpinBox::NoButtons);
        aodDSpinBox2->setDecimals(3);
        aodDSpinBox2->setMinimum(-1e+08);
        aodDSpinBox2->setMaximum(1e+08);
        aodDSpinBox2->setValue(2.05);
        clsDSpinBox2 = new QDoubleSpinBox(gaspTab);
        clsDSpinBox2->setObjectName(QString::fromUtf8("clsDSpinBox2"));
        clsDSpinBox2->setGeometry(QRect(236, 100, 120, 24));
        clsDSpinBox2->setMinimumSize(QSize(120, 24));
        clsDSpinBox2->setMaximumSize(QSize(120, 24));
        clsDSpinBox2->setButtonSymbols(QAbstractSpinBox::NoButtons);
        clsDSpinBox2->setDecimals(3);
        clsDSpinBox2->setMinimum(-1e+08);
        clsDSpinBox2->setMaximum(1e+08);
        clsDSpinBox2->setValue(25);
        sfcDSpinBox = new QDoubleSpinBox(gaspTab);
        sfcDSpinBox->setObjectName(QString::fromUtf8("sfcDSpinBox"));
        sfcDSpinBox->setGeometry(QRect(88, 172, 120, 24));
        sfcDSpinBox->setMinimumSize(QSize(120, 24));
        sfcDSpinBox->setMaximumSize(QSize(120, 24));
        sfcDSpinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
        sfcDSpinBox->setDecimals(3);
        sfcDSpinBox->setMinimum(-0.1);
        sfcDSpinBox->setMaximum(0.41);
        sfcDSpinBox->setSingleStep(0.1);
        sfcDSpinBox->setValue(-0.1);
        sfcDSpinBox2 = new QDoubleSpinBox(gaspTab);
        sfcDSpinBox2->setObjectName(QString::fromUtf8("sfcDSpinBox2"));
        sfcDSpinBox2->setGeometry(QRect(236, 172, 120, 24));
        sfcDSpinBox2->setMinimumSize(QSize(120, 24));
        sfcDSpinBox2->setMaximumSize(QSize(120, 24));
        sfcDSpinBox2->setButtonSymbols(QAbstractSpinBox::NoButtons);
        sfcDSpinBox2->setDecimals(3);
        sfcDSpinBox2->setMinimum(-1e+08);
        sfcDSpinBox2->setMaximum(1e+08);
        sfcDSpinBox2->setValue(0.41);
        aodDSpinBox = new QDoubleSpinBox(gaspTab);
        aodDSpinBox->setObjectName(QString::fromUtf8("aodDSpinBox"));
        aodDSpinBox->setGeometry(QRect(88, 52, 120, 24));
        aodDSpinBox->setMinimumSize(QSize(120, 24));
        aodDSpinBox->setMaximumSize(QSize(120, 24));
        aodDSpinBox->setFrame(true);
        aodDSpinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
        aodDSpinBox->setDecimals(3);
        aodDSpinBox->setMinimum(-0.5);
        aodDSpinBox->setMaximum(2.05);
        aodDSpinBox->setSingleStep(0.01);
        aodDSpinBox->setValue(-0.5);
        mosDSpinBox2 = new QDoubleSpinBox(gaspTab);
        mosDSpinBox2->setObjectName(QString::fromUtf8("mosDSpinBox2"));
        mosDSpinBox2->setGeometry(QRect(236, 196, 120, 24));
        mosDSpinBox2->setMinimumSize(QSize(120, 24));
        mosDSpinBox2->setMaximumSize(QSize(120, 24));
        mosDSpinBox2->setButtonSymbols(QAbstractSpinBox::NoButtons);
        mosDSpinBox2->setDecimals(3);
        mosDSpinBox2->setMinimum(-1e+08);
        mosDSpinBox2->setMaximum(1e+08);
        mosDSpinBox2->setValue(0.425);
        label_21 = new QLabel(gaspTab);
        label_21->setObjectName(QString::fromUtf8("label_21"));
        label_21->setGeometry(QRect(12, 200, 69, 16));
        label_22 = new QLabel(gaspTab);
        label_22->setObjectName(QString::fromUtf8("label_22"));
        label_22->setGeometry(QRect(12, 8, 297, 16));
        label_22->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        ch1DSpinBox = new QDoubleSpinBox(gaspTab);
        ch1DSpinBox->setObjectName(QString::fromUtf8("ch1DSpinBox"));
        ch1DSpinBox->setGeometry(QRect(88, 124, 120, 24));
        ch1DSpinBox->setMinimumSize(QSize(120, 24));
        ch1DSpinBox->setMaximumSize(QSize(120, 24));
        ch1DSpinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
        ch1DSpinBox->setDecimals(3);
        ch1DSpinBox->setMinimum(0);
        ch1DSpinBox->setMaximum(0.425);
        ch1DSpinBox2 = new QDoubleSpinBox(gaspTab);
        ch1DSpinBox2->setObjectName(QString::fromUtf8("ch1DSpinBox2"));
        ch1DSpinBox2->setGeometry(QRect(236, 124, 120, 24));
        ch1DSpinBox2->setMinimumSize(QSize(120, 24));
        ch1DSpinBox2->setMaximumSize(QSize(120, 24));
        ch1DSpinBox2->setButtonSymbols(QAbstractSpinBox::NoButtons);
        ch1DSpinBox2->setDecimals(3);
        ch1DSpinBox2->setMinimum(-1e+08);
        ch1DSpinBox2->setMaximum(1e+08);
        ch1DSpinBox2->setValue(0.425);
        label_23 = new QLabel(gaspTab);
        label_23->setObjectName(QString::fromUtf8("label_23"));
        label_23->setGeometry(QRect(12, 104, 69, 16));
        label_25 = new QLabel(gaspTab);
        label_25->setObjectName(QString::fromUtf8("label_25"));
        label_25->setGeometry(QRect(12, 80, 69, 16));
        aodstdDSpinBox2 = new QDoubleSpinBox(gaspTab);
        aodstdDSpinBox2->setObjectName(QString::fromUtf8("aodstdDSpinBox2"));
        aodstdDSpinBox2->setGeometry(QRect(236, 76, 120, 24));
        aodstdDSpinBox2->setMinimumSize(QSize(120, 24));
        aodstdDSpinBox2->setMaximumSize(QSize(120, 24));
        aodstdDSpinBox2->setButtonSymbols(QAbstractSpinBox::NoButtons);
        aodstdDSpinBox2->setDecimals(3);
        aodstdDSpinBox2->setMinimum(-1e+08);
        aodstdDSpinBox2->setMaximum(1e+08);
        aodstdDSpinBox2->setSingleStep(0.01);
        aodstdDSpinBox2->setValue(2.55);
        label_26 = new QLabel(gaspTab);
        label_26->setObjectName(QString::fromUtf8("label_26"));
        label_26->setGeometry(QRect(278, 32, 61, 16));
        clsDSpinBox = new QDoubleSpinBox(gaspTab);
        clsDSpinBox->setObjectName(QString::fromUtf8("clsDSpinBox"));
        clsDSpinBox->setGeometry(QRect(88, 100, 120, 24));
        clsDSpinBox->setMinimumSize(QSize(120, 24));
        clsDSpinBox->setMaximumSize(QSize(120, 24));
        clsDSpinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
        clsDSpinBox->setDecimals(3);
        clsDSpinBox->setMinimum(0);
        clsDSpinBox->setMaximum(25);
        label_27 = new QLabel(gaspTab);
        label_27->setObjectName(QString::fromUtf8("label_27"));
        label_27->setGeometry(QRect(20, 264, 353, 85));
        label_27->setTextFormat(Qt::RichText);
        label_27->setWordWrap(true);
        label_28 = new QLabel(gaspTab);
        label_28->setObjectName(QString::fromUtf8("label_28"));
        label_28->setGeometry(QRect(12, 152, 69, 16));
        scaDSpinBox = new QDoubleSpinBox(gaspTab);
        scaDSpinBox->setObjectName(QString::fromUtf8("scaDSpinBox"));
        scaDSpinBox->setGeometry(QRect(88, 220, 120, 24));
        scaDSpinBox->setMinimumSize(QSize(120, 24));
        scaDSpinBox->setMaximumSize(QSize(120, 24));
        scaDSpinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
        scaDSpinBox->setDecimals(3);
        scaDSpinBox->setMinimum(0);
        scaDSpinBox->setMaximum(0.425);
        scaDSpinBox2 = new QDoubleSpinBox(gaspTab);
        scaDSpinBox2->setObjectName(QString::fromUtf8("scaDSpinBox2"));
        scaDSpinBox2->setGeometry(QRect(236, 220, 120, 24));
        scaDSpinBox2->setMinimumSize(QSize(120, 24));
        scaDSpinBox2->setMaximumSize(QSize(120, 24));
        scaDSpinBox2->setButtonSymbols(QAbstractSpinBox::NoButtons);
        scaDSpinBox2->setDecimals(3);
        scaDSpinBox2->setMinimum(-1e+08);
        scaDSpinBox2->setMaximum(1e+08);
        scaDSpinBox2->setValue(0.425);
        label_24 = new QLabel(gaspTab);
        label_24->setObjectName(QString::fromUtf8("label_24"));
        label_24->setGeometry(QRect(12, 224, 69, 16));
        tabWidget->addTab(gaspTab, QString());
        calipsoTab = new QWidget();
        calipsoTab->setObjectName(QString::fromUtf8("calipsoTab"));
        mincadSpinBox = new QSpinBox(calipsoTab);
        mincadSpinBox->setObjectName(QString::fromUtf8("mincadSpinBox"));
        mincadSpinBox->setGeometry(QRect(236, 44, 50, 24));
        mincadSpinBox->setMinimumSize(QSize(50, 24));
        mincadSpinBox->setMaximumSize(QSize(50, 24));
        mincadSpinBox->setAccelerated(true);
        mincadSpinBox->setMinimum(20);
        mincadSpinBox->setMaximum(100);
        label_29 = new QLabel(calipsoTab);
        label_29->setObjectName(QString::fromUtf8("label_29"));
        label_29->setGeometry(QRect(24, 68, 345, 73));
        label_29->setTextFormat(Qt::RichText);
        label_29->setWordWrap(true);
        label_30 = new QLabel(calipsoTab);
        label_30->setObjectName(QString::fromUtf8("label_30"));
        label_30->setGeometry(QRect(24, 12, 233, 16));
        maxuncetaintyDSpinBox = new QDoubleSpinBox(calipsoTab);
        maxuncetaintyDSpinBox->setObjectName(QString::fromUtf8("maxuncetaintyDSpinBox"));
        maxuncetaintyDSpinBox->setGeometry(QRect(268, 172, 80, 24));
        maxuncetaintyDSpinBox->setMinimumSize(QSize(80, 24));
        maxuncetaintyDSpinBox->setMaximumSize(QSize(80, 24));
        maxuncetaintyDSpinBox->setDecimals(3);
        maxuncetaintyDSpinBox->setMaximum(100);
        maxuncetaintyDSpinBox->setSingleStep(0.1);
        maxuncetaintyDSpinBox->setValue(99);
        label_31 = new QLabel(calipsoTab);
        label_31->setObjectName(QString::fromUtf8("label_31"));
        label_31->setGeometry(QRect(24, 160, 241, 49));
        label_31->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_31->setWordWrap(true);
        label_32 = new QLabel(calipsoTab);
        label_32->setObjectName(QString::fromUtf8("label_32"));
        label_32->setGeometry(QRect(24, 48, 209, 16));
        label_35 = new QLabel(calipsoTab);
        label_35->setObjectName(QString::fromUtf8("label_35"));
        label_35->setGeometry(QRect(24, 232, 201, 25));
        label_35->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_35->setWordWrap(true);
        label_36 = new QLabel(calipsoTab);
        label_36->setObjectName(QString::fromUtf8("label_36"));
        label_36->setGeometry(QRect(24, 268, 201, 25));
        label_36->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_36->setWordWrap(true);
        calipsoVersionL1Combobox = new QComboBox(calipsoTab);
        calipsoVersionL1Combobox->setObjectName(QString::fromUtf8("calipsoVersionL1Combobox"));
        calipsoVersionL1Combobox->setGeometry(QRect(152, 236, 69, 22));
        calipsoVersionL2Combobox = new QComboBox(calipsoTab);
        calipsoVersionL2Combobox->setObjectName(QString::fromUtf8("calipsoVersionL2Combobox"));
        calipsoVersionL2Combobox->setGeometry(QRect(152, 272, 69, 22));
        label_2 = new QLabel(calipsoTab);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(236, 232, 241, 73));
        label_2->setTextFormat(Qt::RichText);
        label_2->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        tabWidget->addTab(calipsoTab, QString());
        OMITab = new QWidget();
        OMITab->setObjectName(QString::fromUtf8("OMITab"));
        maxOMICloudFractionSpinBox = new QDoubleSpinBox(OMITab);
        maxOMICloudFractionSpinBox->setObjectName(QString::fromUtf8("maxOMICloudFractionSpinBox"));
        maxOMICloudFractionSpinBox->setGeometry(QRect(36, 132, 120, 24));
        maxOMICloudFractionSpinBox->setMinimumSize(QSize(120, 24));
        maxOMICloudFractionSpinBox->setMaximumSize(QSize(120, 24));
        maxOMICloudFractionSpinBox->setDecimals(3);
        maxOMICloudFractionSpinBox->setMaximum(1);
        maxOMICloudFractionSpinBox->setSingleStep(0.1);
        maxOMICloudFractionSpinBox->setValue(1);
        label_OMI1 = new QLabel(OMITab);
        label_OMI1->setObjectName(QString::fromUtf8("label_OMI1"));
        label_OMI1->setGeometry(QRect(28, 116, 265, 16));
        maxOMISolarZenithAngleSpinBox = new QDoubleSpinBox(OMITab);
        maxOMISolarZenithAngleSpinBox->setObjectName(QString::fromUtf8("maxOMISolarZenithAngleSpinBox"));
        maxOMISolarZenithAngleSpinBox->setGeometry(QRect(36, 192, 120, 24));
        maxOMISolarZenithAngleSpinBox->setMinimumSize(QSize(120, 24));
        maxOMISolarZenithAngleSpinBox->setMaximumSize(QSize(120, 24));
        maxOMISolarZenithAngleSpinBox->setDecimals(3);
        maxOMISolarZenithAngleSpinBox->setMaximum(90);
        maxOMISolarZenithAngleSpinBox->setSingleStep(1);
        maxOMISolarZenithAngleSpinBox->setValue(90);
        label_OMI2 = new QLabel(OMITab);
        label_OMI2->setObjectName(QString::fromUtf8("label_OMI2"));
        label_OMI2->setGeometry(QRect(28, 176, 265, 16));
        OMIAllowNegativeCounts = new QCheckBox(OMITab);
        OMIAllowNegativeCounts->setObjectName(QString::fromUtf8("OMIAllowNegativeCounts"));
        OMIAllowNegativeCounts->setGeometry(QRect(36, 225, 400, 45));
        tabWidget->addTab(OMITab, QString());
        tropomiTab = new QWidget();
        tropomiTab->setObjectName(QString::fromUtf8("tropomiTab"));
        minTropomiQualitySpinBox = new QSpinBox(tropomiTab);
        minTropomiQualitySpinBox->setObjectName(QString::fromUtf8("minTropomiQualitySpinBox"));
        minTropomiQualitySpinBox->setGeometry(QRect(36, 72, 109, 25));
        minTropomiQualitySpinBox->setMinimum(50);
        minTropomiQualitySpinBox->setMaximum(100);
        minTropomiQualitySpinBox->setValue(75);
        label_33 = new QLabel(tropomiTab);
        label_33->setObjectName(QString::fromUtf8("label_33"));
        label_33->setGeometry(QRect(24, 48, 265, 16));
        maxTropomiCloudFractionSpinBox = new QDoubleSpinBox(tropomiTab);
        maxTropomiCloudFractionSpinBox->setObjectName(QString::fromUtf8("maxTropomiCloudFractionSpinBox"));
        maxTropomiCloudFractionSpinBox->setGeometry(QRect(36, 132, 120, 24));
        maxTropomiCloudFractionSpinBox->setMinimumSize(QSize(120, 24));
        maxTropomiCloudFractionSpinBox->setMaximumSize(QSize(120, 24));
        maxTropomiCloudFractionSpinBox->setDecimals(3);
        maxTropomiCloudFractionSpinBox->setMaximum(1);
        maxTropomiCloudFractionSpinBox->setSingleStep(0.1);
        maxTropomiCloudFractionSpinBox->setValue(1);
        label_34 = new QLabel(tropomiTab);
        label_34->setObjectName(QString::fromUtf8("label_34"));
        label_34->setGeometry(QRect(28, 116, 265, 16));
        TropomiGroundPixelMinimumSpinBox = new QSpinBox(tropomiTab);
        TropomiGroundPixelMinimumSpinBox->setObjectName(QString::fromUtf8("TropomiGroundPixelMinimumSpinBox"));
        TropomiGroundPixelMinimumSpinBox->setGeometry(QRect(36, 192, 109, 25));
        TropomiGroundPixelMinimumSpinBox->setMinimum(0);
        TropomiGroundPixelMinimumSpinBox->setMaximum(449);
        TropomiGroundPixelMinimumSpinBox->setValue(0);
        TropomiGroundPixelMinimumLabel = new QLabel(tropomiTab);
        TropomiGroundPixelMinimumLabel->setObjectName(QString::fromUtf8("TropomiGroundPixelMinimumLabel"));
        TropomiGroundPixelMinimumLabel->setGeometry(QRect(24, 168, 265, 16));
        TropomiGroundPixelMaximumSpinBox = new QSpinBox(tropomiTab);
        TropomiGroundPixelMaximumSpinBox->setObjectName(QString::fromUtf8("TropomiGroundPixelMaximumSpinBox"));
        TropomiGroundPixelMaximumSpinBox->setGeometry(QRect(36, 252, 109, 25));
        TropomiGroundPixelMaximumSpinBox->setMinimum(0);
        TropomiGroundPixelMaximumSpinBox->setMaximum(449);
        TropomiGroundPixelMaximumSpinBox->setValue(0);
        TropomiGroundPixelMaximumLabel = new QLabel(tropomiTab);
        TropomiGroundPixelMaximumLabel->setObjectName(QString::fromUtf8("TropomiGroundPixelMaximumLabel"));
        TropomiGroundPixelMaximumLabel->setGeometry(QRect(24, 228, 265, 16));
        TropomiAllowNegativeCounts = new QCheckBox(tropomiTab);
        TropomiAllowNegativeCounts->setObjectName(QString::fromUtf8("TropomiAllowNegativeCounts"));
        TropomiAllowNegativeCounts->setGeometry(QRect(36, 275, 400, 45));
        tabWidget->addTab(tropomiTab, QString());
        viirsnoaaTab = new QWidget();
        viirsnoaaTab->setObjectName(QString::fromUtf8("viirsnoaaTab"));
        viirs_label = new QLabel(viirsnoaaTab);
        viirs_label->setObjectName(QString::fromUtf8("viirs_label"));
        viirs_label->setGeometry(QRect(24, 48, 145, 21));
        viirsNoaaQAflagMenu = new QComboBox(viirsnoaaTab);
        viirsNoaaQAflagMenu->setObjectName(QString::fromUtf8("viirsNoaaQAflagMenu"));
        viirsNoaaQAflagMenu->setGeometry(QRect(24, 72, 125, 22));
        tabWidget->addTab(viirsnoaaTab, QString());
        pandoraTab = new QWidget();
        pandoraTab->setObjectName(QString::fromUtf8("pandoraTab"));
        pandora_label = new QLabel(pandoraTab);
        pandora_label->setObjectName(QString::fromUtf8("pandora_label"));
        pandora_label->setGeometry(QRect(24, 48, 145, 21));
        pandoraQAflagMenu = new QComboBox(pandoraTab);
        pandoraQAflagMenu->setObjectName(QString::fromUtf8("pandoraQAflagMenu"));
        pandoraQAflagMenu->setGeometry(QRect(24, 72, 125, 22));
        tabWidget->addTab(pandoraTab, QString());
        purpleairTab = new QWidget();
        purpleairTab->setObjectName(QString::fromUtf8("purpleairTab"));
        apikeyPurpleAirLabel = new QLabel(purpleairTab);
        apikeyPurpleAirLabel->setObjectName(QString::fromUtf8("apikeyPurpleAirLabel"));
        apikeyPurpleAirLabel->setGeometry(QRect(20, 28, 101, 16));
        apikeyPurpleAir = new QLineEdit(purpleairTab);
        apikeyPurpleAir->setObjectName(QString::fromUtf8("apikeyPurpleAir"));
        apikeyPurpleAir->setGeometry(QRect(176, 24, 341, 20));
        apikeyPurpleAir->setMaxLength(40);
        PurpleAirLabel_3 = new QLabel(purpleairTab);
        PurpleAirLabel_3->setObjectName(QString::fromUtf8("PurpleAirLabel_3"));
        PurpleAirLabel_3->setGeometry(QRect(16, 68, 125, 37));
        minPurpleairCountPercentage = new QSpinBox(purpleairTab);
        minPurpleairCountPercentage->setObjectName(QString::fromUtf8("minPurpleairCountPercentage"));
        minPurpleairCountPercentage->setGeometry(QRect(176, 68, 109, 25));
        minPurpleairCountPercentage->setMinimum(1);
        minPurpleairCountPercentage->setMaximum(100);
        minPurpleairCountPercentage->setValue(75);
        purpleairHumidityReplacementValue = new QSpinBox(purpleairTab);
        purpleairHumidityReplacementValue->setObjectName(QString::fromUtf8("purpleairHumidityReplacementValue"));
        purpleairHumidityReplacementValue->setGeometry(QRect(176, 136, 109, 25));
        purpleairHumidityReplacementValue->setMinimum(1);
        purpleairHumidityReplacementValue->setMaximum(100);
        purpleairHumidityReplacementValue->setValue(50);
        PurpleAirLabel_4 = new QLabel(purpleairTab);
        PurpleAirLabel_4->setObjectName(QString::fromUtf8("PurpleAirLabel_4"));
        PurpleAirLabel_4->setGeometry(QRect(164, 108, 125, 37));
        usePurpleairHumidityReplacement = new QCheckBox(purpleairTab);
        usePurpleairHumidityReplacement->setObjectName(QString::fromUtf8("usePurpleairHumidityReplacement"));
        usePurpleairHumidityReplacement->setGeometry(QRect(8, 128, 205, 45));
        groupBox = new QGroupBox(purpleairTab);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(392, 64, 120, 117));
        purpleairLocationOutdoor = new QRadioButton(groupBox);
        purpleairLocationOutdoor->setObjectName(QString::fromUtf8("purpleairLocationOutdoor"));
        purpleairLocationOutdoor->setGeometry(QRect(8, 24, 82, 17));
        purpleairLocationIndoor = new QRadioButton(groupBox);
        purpleairLocationIndoor->setObjectName(QString::fromUtf8("purpleairLocationIndoor"));
        purpleairLocationIndoor->setGeometry(QRect(8, 48, 82, 17));
        purpleairLocationIndoorOutdoor = new QRadioButton(groupBox);
        purpleairLocationIndoorOutdoor->setObjectName(QString::fromUtf8("purpleairLocationIndoorOutdoor"));
        purpleairLocationIndoorOutdoor->setGeometry(QRect(8, 72, 82, 17));
        PurpleAirLabel_9 = new QLabel(purpleairTab);
        PurpleAirLabel_9->setObjectName(QString::fromUtf8("PurpleAirLabel_9"));
        PurpleAirLabel_9->setGeometry(QRect(292, 132, 45, 33));
        groupBox_3 = new QGroupBox(purpleairTab);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setGeometry(QRect(12, 184, 501, 101));
        groupBox_3->setAutoFillBackground(false);
        groupBox_3->setFlat(false);
        PurpleAirLabel_8 = new QLabel(groupBox_3);
        PurpleAirLabel_8->setObjectName(QString::fromUtf8("PurpleAirLabel_8"));
        PurpleAirLabel_8->setGeometry(QRect(284, 64, 45, 33));
        acceptablePurpleairChannelDifference = new QSpinBox(groupBox_3);
        acceptablePurpleairChannelDifference->setObjectName(QString::fromUtf8("acceptablePurpleairChannelDifference"));
        acceptablePurpleairChannelDifference->setGeometry(QRect(168, 24, 109, 25));
        acceptablePurpleairChannelDifference->setMinimum(1);
        acceptablePurpleairChannelDifference->setValue(5);
        PurpleAirLabel_7 = new QLabel(groupBox_3);
        PurpleAirLabel_7->setObjectName(QString::fromUtf8("PurpleAirLabel_7"));
        PurpleAirLabel_7->setGeometry(QRect(8, 64, 161, 33));
        acceptablePurpleairChannelAgreement = new QSpinBox(groupBox_3);
        acceptablePurpleairChannelAgreement->setObjectName(QString::fromUtf8("acceptablePurpleairChannelAgreement"));
        acceptablePurpleairChannelAgreement->setGeometry(QRect(168, 68, 109, 25));
        acceptablePurpleairChannelAgreement->setMinimum(1);
        acceptablePurpleairChannelAgreement->setValue(70);
        PurpleAirLabel_5 = new QLabel(groupBox_3);
        PurpleAirLabel_5->setObjectName(QString::fromUtf8("PurpleAirLabel_5"));
        PurpleAirLabel_5->setGeometry(QRect(8, 20, 157, 33));
        PurpleAirLabel_6 = new QLabel(groupBox_3);
        PurpleAirLabel_6->setObjectName(QString::fromUtf8("PurpleAirLabel_6"));
        PurpleAirLabel_6->setGeometry(QRect(284, 20, 53, 33));
        tabWidget->addTab(purpleairTab, QString());
        TEMPOTab = new QWidget();
        TEMPOTab->setObjectName(QString::fromUtf8("TEMPOTab"));
        TEMPO_label = new QLabel(TEMPOTab);
        TEMPO_label->setObjectName(QString::fromUtf8("TEMPO_label"));
        TEMPO_label->setGeometry(QRect(24, 48, 145, 21));
        TEMPOQAflagMenu = new QComboBox(TEMPOTab);
        TEMPOQAflagMenu->setObjectName(QString::fromUtf8("TEMPOQAflagMenu"));
        TEMPOQAflagMenu->setGeometry(QRect(24, 72, 125, 22));
        maxTEMPOCloudFractionSpinBox = new QDoubleSpinBox(TEMPOTab);
        maxTEMPOCloudFractionSpinBox->setObjectName(QString::fromUtf8("maxTEMPOCloudFractionSpinBox"));
        maxTEMPOCloudFractionSpinBox->setGeometry(QRect(36, 132, 120, 24));
        maxTEMPOCloudFractionSpinBox->setMinimumSize(QSize(120, 24));
        maxTEMPOCloudFractionSpinBox->setMaximumSize(QSize(120, 24));
        maxTEMPOCloudFractionSpinBox->setDecimals(3);
        maxTEMPOCloudFractionSpinBox->setMaximum(1);
        maxTEMPOCloudFractionSpinBox->setSingleStep(0.1);
        maxTEMPOCloudFractionSpinBox->setValue(1);
        label_37 = new QLabel(TEMPOTab);
        label_37->setObjectName(QString::fromUtf8("label_37"));
        label_37->setGeometry(QRect(28, 116, 265, 16));
        maxTEMPOSolarZenithAngleSpinBox = new QDoubleSpinBox(TEMPOTab);
        maxTEMPOSolarZenithAngleSpinBox->setObjectName(QString::fromUtf8("maxTEMPOSolarZenithAngleSpinBox"));
        maxTEMPOSolarZenithAngleSpinBox->setGeometry(QRect(36, 192, 120, 24));
        maxTEMPOSolarZenithAngleSpinBox->setMinimumSize(QSize(120, 24));
        maxTEMPOSolarZenithAngleSpinBox->setMaximumSize(QSize(120, 24));
        maxTEMPOSolarZenithAngleSpinBox->setDecimals(3);
        maxTEMPOSolarZenithAngleSpinBox->setMaximum(90);
        maxTEMPOSolarZenithAngleSpinBox->setSingleStep(1);
        maxTEMPOSolarZenithAngleSpinBox->setValue(90);
        label_38 = new QLabel(TEMPOTab);
        label_38->setObjectName(QString::fromUtf8("label_38"));
        label_38->setGeometry(QRect(28, 176, 265, 16));
        TEMPOKeyLabel = new QLabel(TEMPOTab);
        TEMPOKeyLabel->setObjectName(QString::fromUtf8("TEMPOKeyLabel"));
        TEMPOKeyLabel->setGeometry(QRect(20, 28, 101, 16));
        TEMPOKey = new QLineEdit(TEMPOTab);
        TEMPOKey->setObjectName(QString::fromUtf8("TEMPOKey"));
        TEMPOKey->setGeometry(QRect(176, 24, 341, 20));
        TEMPOKey->setMaxLength(40);
        TEMPOAllowNegativeCounts = new QCheckBox(TEMPOTab);
        TEMPOAllowNegativeCounts->setObjectName(QString::fromUtf8("TEMPOAllowNegativeCounts"));
        TEMPOAllowNegativeCounts->setGeometry(QRect(36, 225, 400, 45));
        tabWidget->addTab(TEMPOTab, QString());
        OAQPSTab = new QWidget();
        OAQPSTab->setObjectName(QString::fromUtf8("OAQPSTab"));
        OAQPSKeyLabel = new QLabel(OAQPSTab);
        OAQPSKeyLabel->setObjectName(QString::fromUtf8("OAQPSKeyLabel"));
        OAQPSKeyLabel->setGeometry(QRect(20, 28, 101, 16));
        OAQPSKey = new QLineEdit(OAQPSTab);
        OAQPSKey->setObjectName(QString::fromUtf8("OAQPSKey"));
        OAQPSKey->setGeometry(QRect(176, 24, 341, 20));
        OAQPSKey->setMaxLength(40);
        tabWidget->addTab(OAQPSTab, QString());
        QWidget::setTabOrder(aodDSpinBox, aodDSpinBox2);
        QWidget::setTabOrder(aodDSpinBox2, aodstdDSpinBox);
        QWidget::setTabOrder(aodstdDSpinBox, aodstdDSpinBox2);
        QWidget::setTabOrder(aodstdDSpinBox2, clsDSpinBox);
        QWidget::setTabOrder(clsDSpinBox, clsDSpinBox2);
        QWidget::setTabOrder(clsDSpinBox2, ch1DSpinBox);
        QWidget::setTabOrder(ch1DSpinBox, ch1DSpinBox2);
        QWidget::setTabOrder(ch1DSpinBox2, sigDSpinBox);
        QWidget::setTabOrder(sigDSpinBox, sigDSpinBox2);
        QWidget::setTabOrder(sigDSpinBox2, sfcDSpinBox);
        QWidget::setTabOrder(sfcDSpinBox, sfcDSpinBox2);
        QWidget::setTabOrder(sfcDSpinBox2, mosDSpinBox);
        QWidget::setTabOrder(mosDSpinBox, mosDSpinBox2);
        QWidget::setTabOrder(mosDSpinBox2, resetPushButton);
        QWidget::setTabOrder(resetPushButton, cancelPushButton);
        QWidget::setTabOrder(cancelPushButton, acceptPushButton);
        QWidget::setTabOrder(acceptPushButton, mincadSpinBox);
        QWidget::setTabOrder(mincadSpinBox, maxuncetaintyDSpinBox);
        QWidget::setTabOrder(maxuncetaintyDSpinBox, tabWidget);

        retranslateUi(FilterSettingDialog);

        tabWidget->setCurrentIndex(4);
        calipsoVersionL1Combobox->setCurrentIndex(0);
        calipsoVersionL2Combobox->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(FilterSettingDialog);
    } // setupUi

    void retranslateUi(QDialog *FilterSettingDialog)
    {
        FilterSettingDialog->setWindowTitle(QApplication::translate("FilterSettingDialog", "Data Filters", 0, QApplication::UnicodeUTF8));
        acceptPushButton->setText(QApplication::translate("FilterSettingDialog", "Accept", 0, QApplication::UnicodeUTF8));
        cancelPushButton->setText(QApplication::translate("FilterSettingDialog", "Cancel", 0, QApplication::UnicodeUTF8));
        resetPushButton->setText(QApplication::translate("FilterSettingDialog", "Reset", 0, QApplication::UnicodeUTF8));
        label_17->setText(QApplication::translate("FilterSettingDialog", "<html><head/><body><p align=\"right\">CH1:</p></body></html>", 0, QApplication::UnicodeUTF8));
        label_18->setText(QApplication::translate("FilterSettingDialog", "<html><head/><body><p align=\"right\">AOD:</p></body></html>", 0, QApplication::UnicodeUTF8));
        label_19->setText(QApplication::translate("FilterSettingDialog", "<html><head/><body><p><span style=\" font-weight:600;\">min</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        label_20->setText(QApplication::translate("FilterSettingDialog", "<html><head/><body><p align=\"right\">SFC:</p></body></html>", 0, QApplication::UnicodeUTF8));
        label_21->setText(QApplication::translate("FilterSettingDialog", "<html><head/><body><p align=\"right\">MOS:</p></body></html>", 0, QApplication::UnicodeUTF8));
        label_22->setText(QApplication::translate("FilterSettingDialog", "<html><head/><body><p><span style=\" font-weight:600;\">GASP Filter Settings:</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        label_23->setText(QApplication::translate("FilterSettingDialog", "<html><head/><body><p align=\"right\">CLS:</p></body></html>", 0, QApplication::UnicodeUTF8));
        label_25->setText(QApplication::translate("FilterSettingDialog", "<html><head/><body><p align=\"right\">AODSTD:</p></body></html>", 0, QApplication::UnicodeUTF8));
        label_26->setText(QApplication::translate("FilterSettingDialog", "<html><head/><body><p><span style=\" font-weight:600;\">max</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        label_27->setText(QApplication::translate("FilterSettingDialog", "<html><head/><body><p><span style=\" color:#313dd2;\">These filter settings are used to selectively filter out data that is outside each specified range. </span></p><p><span style=\" color:#313dd2;\">Hit 'Reset' to restore the filter settings to the default values.</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        label_28->setText(QApplication::translate("FilterSettingDialog", "<html><head/><body><p align=\"right\">SIG:</p></body></html>", 0, QApplication::UnicodeUTF8));
        label_24->setText(QApplication::translate("FilterSettingDialog", "<html><head/><body><p align=\"right\">SCA:</p></body></html>", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(gaspTab), QApplication::translate("FilterSettingDialog", "GASP Filter", 0, QApplication::UnicodeUTF8));
        label_29->setText(QApplication::translate("FilterSettingDialog", "<html><head/><body><p><span style=\" color:#0000ff;\">20 means least likely (but better than a 50/50 chance of correctly distinguished aerosol/cloud feature). </span></p><p><span style=\" color:#0000ff;\">100 means highest likelyhood of correctly distiinguished aerosol/cloud feature.</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        label_30->setText(QApplication::translate("FilterSettingDialog", "<html><head/><body><p><span style=\" font-weight:600;\">CALIPSO Data Filtering Options:</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        label_31->setText(QApplication::translate("FilterSettingDialog", "<html><head/><body><p>Maximum accepted uncertainty of measurement (in absolute units - not percentage) [0, 100]:</p></body></html>", 0, QApplication::UnicodeUTF8));
        label_32->setText(QApplication::translate("FilterSettingDialog", "<html><head/><body><p>Minimum accepted |CAD Score| [20, 100]:</p></body></html>", 0, QApplication::UnicodeUTF8));
        label_35->setText(QApplication::translate("FilterSettingDialog", "<html><head/><body><p>L1 data product version:</p></body></html>", 0, QApplication::UnicodeUTF8));
        label_36->setText(QApplication::translate("FilterSettingDialog", "<html><head/><body><p>L2 data product version:</p></body></html>", 0, QApplication::UnicodeUTF8));
        calipsoVersionL1Combobox->clear();
        calipsoVersionL1Combobox->insertItems(0, QStringList()
         << QApplication::translate("FilterSettingDialog", "Latest", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("FilterSettingDialog", "4.11", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("FilterSettingDialog", "4.10", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("FilterSettingDialog", "4.00", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("FilterSettingDialog", "3.41", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("FilterSettingDialog", "3.30", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("FilterSettingDialog", "3.02", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("FilterSettingDialog", "3.01", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("FilterSettingDialog", "3.0", 0, QApplication::UnicodeUTF8)
        );
        calipsoVersionL2Combobox->clear();
        calipsoVersionL2Combobox->insertItems(0, QStringList()
         << QApplication::translate("FilterSettingDialog", "Latest", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("FilterSettingDialog", "4.21", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("FilterSettingDialog", "4.20", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("FilterSettingDialog", "4.10", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("FilterSettingDialog", "4.00", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("FilterSettingDialog", "3.41", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("FilterSettingDialog", "3.30", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("FilterSettingDialog", "3.02", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("FilterSettingDialog", "3.01", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("FilterSettingDialog", "3.0", 0, QApplication::UnicodeUTF8)
        );
        label_2->setText(QApplication::translate("FilterSettingDialog", "<html><head/><body><p><span style=\" color:#0000ff;\">L1 version 4.11 and L2 version 4.21 are<br>only available for July 1, 2020 or later.<br><br>\"Latest\" will always return the most up-to-date<br> product from the CALIPSO science team.</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(calipsoTab), QApplication::translate("FilterSettingDialog", "CALIPSO Filter", 0, QApplication::UnicodeUTF8));
        label_OMI1->setText(QApplication::translate("FilterSettingDialog", "<html><head/><body><p>Maximum cloud fraction: [0.0 - 1.0]</p></body></html>", 0, QApplication::UnicodeUTF8));
        label_OMI2->setText(QApplication::translate("FilterSettingDialog", "<html><head/><body><p>Maximum solar zenith angle: [0.0 - 90.0]</p></body></html>", 0, QApplication::UnicodeUTF8));
        OMIAllowNegativeCounts->setText(QApplication::translate("FilterSettingDialog", "Allow retrieval of negative molecules/cm2?", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(OMITab), QApplication::translate("FilterSettingDialog", "OMI Filter", 0, QApplication::UnicodeUTF8));
        label_33->setText(QApplication::translate("FilterSettingDialog", "<html><head/><body><p>Minimum quality flag: [50-100]</p></body></html>", 0, QApplication::UnicodeUTF8));
        label_34->setText(QApplication::translate("FilterSettingDialog", "<html><head/><body><p>Maximum cloud fraction: [0.0 - 1.0]</p></body></html>", 0, QApplication::UnicodeUTF8));
        TropomiGroundPixelMinimumLabel->setText(QApplication::translate("FilterSettingDialog", "<html><head/><body><p>Ground Pixel Minimum: [0-449]</p></body></html>", 0, QApplication::UnicodeUTF8));
        TropomiGroundPixelMaximumLabel->setText(QApplication::translate("FilterSettingDialog", "<html><head/><body><p>Ground Pixel Maximum: [0-449]</p></body></html>", 0, QApplication::UnicodeUTF8));
        TropomiAllowNegativeCounts->setText(QApplication::translate("FilterSettingDialog", "Allow retrieval of negative molecules/cm2?", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tropomiTab), QApplication::translate("FilterSettingDialog", "TROPOMI Filter", 0, QApplication::UnicodeUTF8));
        viirs_label->setText(QApplication::translate("FilterSettingDialog", "Minimum quality flag", 0, QApplication::UnicodeUTF8));
        viirsNoaaQAflagMenu->clear();
        viirsNoaaQAflagMenu->insertItems(0, QStringList()
         << QApplication::translate("FilterSettingDialog", "high", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("FilterSettingDialog", "medium", 0, QApplication::UnicodeUTF8)
        );
        tabWidget->setTabText(tabWidget->indexOf(viirsnoaaTab), QApplication::translate("FilterSettingDialog", "VIIRS NOAA Filter", 0, QApplication::UnicodeUTF8));
        pandora_label->setText(QApplication::translate("FilterSettingDialog", "Minimum quality flag", 0, QApplication::UnicodeUTF8));
        pandoraQAflagMenu->clear();
        pandoraQAflagMenu->insertItems(0, QStringList()
         << QApplication::translate("FilterSettingDialog", "high", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("FilterSettingDialog", "medium", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("FilterSettingDialog", "low", 0, QApplication::UnicodeUTF8)
        );
        tabWidget->setTabText(tabWidget->indexOf(pandoraTab), QApplication::translate("FilterSettingDialog", "Pandora Filter", 0, QApplication::UnicodeUTF8));
        apikeyPurpleAirLabel->setText(QApplication::translate("FilterSettingDialog", "PurpleAir API key:", 0, QApplication::UnicodeUTF8));
        PurpleAirLabel_3->setText(QApplication::translate("FilterSettingDialog", "Minimum aggregation\n"
"count percentage:", 0, QApplication::UnicodeUTF8));
        PurpleAirLabel_4->setText(QApplication::translate("FilterSettingDialog", "Replacement value:", 0, QApplication::UnicodeUTF8));
        usePurpleairHumidityReplacement->setText(QApplication::translate("FilterSettingDialog", "Use humidity replacement\n"
"in EPA PM2.5 correction\n"
"equation", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("FilterSettingDialog", "Location", 0, QApplication::UnicodeUTF8));
        purpleairLocationOutdoor->setText(QApplication::translate("FilterSettingDialog", "Outdoor", 0, QApplication::UnicodeUTF8));
        purpleairLocationIndoor->setText(QApplication::translate("FilterSettingDialog", "Indoor", 0, QApplication::UnicodeUTF8));
        purpleairLocationIndoorOutdoor->setText(QApplication::translate("FilterSettingDialog", "Both", 0, QApplication::UnicodeUTF8));
        PurpleAirLabel_9->setText(QApplication::translate("FilterSettingDialog", "%", 0, QApplication::UnicodeUTF8));
        PurpleAirLabel_8->setText(QApplication::translate("FilterSettingDialog", "%", 0, QApplication::UnicodeUTF8));
        PurpleAirLabel_7->setText(QApplication::translate("FilterSettingDialog", "Acceptable channel agreement", 0, QApplication::UnicodeUTF8));
        PurpleAirLabel_5->setText(QApplication::translate("FilterSettingDialog", "Acceptable channel difference", 0, QApplication::UnicodeUTF8));
        PurpleAirLabel_6->setText(QApplication::translate("FilterSettingDialog", "ug/m3", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(purpleairTab), QApplication::translate("FilterSettingDialog", "PurpleAir Filter", 0, QApplication::UnicodeUTF8));
        TEMPO_label->setText(QApplication::translate("FilterSettingDialog", "Minimum quality flag", 0, QApplication::UnicodeUTF8));
        TEMPOQAflagMenu->clear();
        TEMPOQAflagMenu->insertItems(0, QStringList()
         << QApplication::translate("FilterSettingDialog", "normal", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("FilterSettingDialog", "suspect", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("FilterSettingDialog", "bad", 0, QApplication::UnicodeUTF8)
        );
        label_37->setText(QApplication::translate("FilterSettingDialog", "<html><head/><body><p>Maximum cloud fraction: [0.0 - 1.0]</p></body></html>", 0, QApplication::UnicodeUTF8));
        label_38->setText(QApplication::translate("FilterSettingDialog", "<html><head/><body><p>Maximum solar zenith angle: [0.0 - 90.0]</p></body></html>", 0, QApplication::UnicodeUTF8));
        TEMPOKeyLabel->setText(QApplication::translate("FilterSettingDialog", "TEMPO Key:", 0, QApplication::UnicodeUTF8));
        TEMPOAllowNegativeCounts->setText(QApplication::translate("FilterSettingDialog", "Allow retrieval of negative molecules/cm2?", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(TEMPOTab), QApplication::translate("FilterSettingDialog", "TEMPO Filter", 0, QApplication::UnicodeUTF8));
        OAQPSKeyLabel->setText(QApplication::translate("FilterSettingDialog", "OAQPS Key:", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(OAQPSTab), QApplication::translate("FilterSettingDialog", "OAQPS Filter", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class FilterSettingDialog: public Ui_FilterSettingDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILTERSETTING_H
