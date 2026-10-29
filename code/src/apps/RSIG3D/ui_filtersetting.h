/********************************************************************************
** Form generated from reading UI file 'filtersetting.ui'
**
** Created: Thu Jul 23 13:53:07 2026
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
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QTabWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FilterSettingDialog
{
public:
    QVBoxLayout *verticalLayout;
    QTabWidget *tabWidget;
    QWidget *gaspTab;
    QGroupBox *groupBox_filterGasp;
    QGridLayout *gridLayout;
    QSpacerItem *horizontalSpacer_2;
    QDoubleSpinBox *aodDSpinBox;
    QLabel *label_26;
    QDoubleSpinBox *aodDSpinBox2;
    QLabel *label_25;
    QDoubleSpinBox *aodstdDSpinBox;
    QDoubleSpinBox *aodstdDSpinBox2;
    QLabel *label_18;
    QLabel *label_19;
    QLabel *label_21;
    QDoubleSpinBox *mosDSpinBox;
    QDoubleSpinBox *mosDSpinBox2;
    QLabel *label_28;
    QDoubleSpinBox *ch1DSpinBox;
    QDoubleSpinBox *clsDSpinBox2;
    QDoubleSpinBox *ch1DSpinBox2;
    QDoubleSpinBox *sigDSpinBox;
    QDoubleSpinBox *sigDSpinBox2;
    QLabel *label_17;
    QLabel *label_23;
    QLabel *label_20;
    QDoubleSpinBox *sfcDSpinBox;
    QDoubleSpinBox *sfcDSpinBox2;
    QDoubleSpinBox *clsDSpinBox;
    QLabel *label_27;
    QDoubleSpinBox *scaDSpinBox;
    QDoubleSpinBox *scaDSpinBox2;
    QLabel *label_24;
    QLabel *label_22;
    QSpacerItem *verticalSpacer;
    QWidget *calipsoTab;
    QGroupBox *groupBox_filterCalipso;
    QGridLayout *gridLayout_2;
    QSpacerItem *horizontalSpacer_3;
    QLabel *label_30;
    QLabel *label_32;
    QSpinBox *mincadSpinBox;
    QLabel *label_29;
    QDoubleSpinBox *maxuncetaintyDSpinBox;
    QLabel *label_31;
    QLabel *label_35;
    QComboBox *calipsoVersionL1Combobox;
    QLabel *label_2;
    QLabel *label_36;
    QComboBox *calipsoVersionL2Combobox;
    QSpacerItem *verticalSpacer_2;
    QWidget *OMITab;
    QGroupBox *groupBox_filterOmi;
    QGridLayout *gridLayout_3;
    QDoubleSpinBox *maxOMICloudFractionSpinBox;
    QLabel *label_OMI2;
    QLabel *label_OMI1;
    QCheckBox *OMIAllowNegativeCounts;
    QDoubleSpinBox *maxOMISolarZenithAngleSpinBox;
    QSpacerItem *horizontalSpacer_4;
    QSpacerItem *verticalSpacer_3;
    QWidget *tropomiTab;
    QGroupBox *groupBox_filterTropomi;
    QGridLayout *gridLayout_4;
    QLabel *label_34;
    QSpinBox *TropomiGroundPixelMinimumSpinBox;
    QDoubleSpinBox *maxTropomiCloudFractionSpinBox;
    QLabel *TropomiGroundPixelMinimumLabel;
    QLabel *label_33;
    QSpinBox *minTropomiQualitySpinBox;
    QSpacerItem *verticalSpacer_4;
    QSpinBox *TropomiGroundPixelMaximumSpinBox;
    QLabel *TropomiGroundPixelMaximumLabel;
    QCheckBox *TropomiAllowNegativeCounts;
    QSpacerItem *horizontalSpacer_5;
    QWidget *viirsTab;
    QGroupBox *groupBox_filterViirs;
    QGridLayout *gridLayout_5;
    QComboBox *viirsNoaaQAflagMenu;
    QLabel *viirs_label;
    QSpacerItem *verticalSpacer_5;
    QSpacerItem *horizontalSpacer_6;
    QWidget *pandoraTab;
    QGroupBox *groupBox_filterPandora;
    QGridLayout *gridLayout_6;
    QComboBox *pandoraQAflagMenu;
    QLabel *pandora_label;
    QSpacerItem *verticalSpacer_6;
    QSpacerItem *horizontalSpacer_7;
    QWidget *purpleairTab;
    QGroupBox *groupBox_filterPurpleair;
    QGridLayout *gridLayout_7;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout_2;
    QSpinBox *acceptablePurpleairChannelDifference;
    QLabel *PurpleAirLabel_6;
    QSpacerItem *horizontalSpacer_11;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout_8;
    QLabel *PurpleAirLabel_4;
    QSpinBox *purpleairHumidityReplacementValue;
    QLabel *PurpleAirLabel_9;
    QSpacerItem *horizontalSpacer_9;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_9;
    QSpacerItem *verticalSpacer_9;
    QRadioButton *purpleairLocationOutdoor;
    QRadioButton *purpleairLocationIndoor;
    QRadioButton *purpleairLocationIndoorOutdoor;
    QSpacerItem *horizontalSpacer_13;
    QSpacerItem *verticalSpacer_13;
    QLabel *PurpleAirLabel_3;
    QLabel *apikeyPurpleAirLabel;
    QCheckBox *usePurpleairHumidityReplacement;
    QLineEdit *apikeyPurpleAir;
    QSpacerItem *horizontalSpacer_8;
    QSpacerItem *verticalSpacer_7;
    QSpinBox *minPurpleairCountPercentage;
    QLabel *PurpleAirLabel_5;
    QSpacerItem *horizontalSpacer_10;
    QGroupBox *groupBox_4;
    QHBoxLayout *horizontalLayout_3;
    QSpinBox *acceptablePurpleairChannelAgreement;
    QLabel *PurpleAirLabel_8;
    QSpacerItem *horizontalSpacer_12;
    QLabel *PurpleAirLabel_7;
    QSpacerItem *horizontalSpacer_14;
    QWidget *TEMPOTab;
    QGroupBox *groupBox_filterTempo;
    QGridLayout *gridLayout_10;
    QLineEdit *TEMPOKey;
    QLabel *TEMPOKeyLabel;
    QComboBox *TEMPOQAflagMenu;
    QDoubleSpinBox *maxTEMPOCloudFractionSpinBox;
    QLabel *label_37;
    QDoubleSpinBox *maxTEMPOSolarZenithAngleSpinBox;
    QCheckBox *TEMPOAllowNegativeCounts;
    QLabel *TEMPO_label;
    QSpacerItem *horizontalSpacer_15;
    QLabel *label_38;
    QSpacerItem *verticalSpacer_8;
    QWidget *OAQPSTab;
    QGroupBox *groupBox_filterOaqps;
    QGridLayout *gridLayout_12;
    QLineEdit *OAQPSKey;
    QLabel *OAQPSKeyLabel;
    QSpacerItem *horizontalSpacer_17;
    QSpacerItem *verticalSpacer_11;
    QWidget *HRRRTab;
    QGroupBox *groupBox_filterHrrr;
    QGridLayout *gridLayout_11;
    QLabel *HRRRStrideSpinBoxLabel;
    QSpacerItem *horizontalSpacer_16;
    QSpinBox *HRRRStrideSpinBox;
    QSpacerItem *verticalSpacer_10;
    QGroupBox *groupBox_filterButtons;
    QHBoxLayout *horizontalLayout;
    QPushButton *resetPushButton;
    QPushButton *cancelPushButton;
    QPushButton *acceptPushButton;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *verticalSpacer_12;

    void setupUi(QDialog *FilterSettingDialog)
    {
        if (FilterSettingDialog->objectName().isEmpty())
            FilterSettingDialog->setObjectName(QString::fromUtf8("FilterSettingDialog"));
        FilterSettingDialog->resize(1818, 900);
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(FilterSettingDialog->sizePolicy().hasHeightForWidth());
        FilterSettingDialog->setSizePolicy(sizePolicy);
        FilterSettingDialog->setMinimumSize(QSize(1800, 900));
        FilterSettingDialog->setMaximumSize(QSize(10000, 10000));
        verticalLayout = new QVBoxLayout(FilterSettingDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        tabWidget = new QTabWidget(FilterSettingDialog);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        sizePolicy.setHeightForWidth(tabWidget->sizePolicy().hasHeightForWidth());
        tabWidget->setSizePolicy(sizePolicy);
        tabWidget->setMinimumSize(QSize(1800, 800));
        tabWidget->setMaximumSize(QSize(10000, 10000));
        gaspTab = new QWidget();
        gaspTab->setObjectName(QString::fromUtf8("gaspTab"));
        sizePolicy.setHeightForWidth(gaspTab->sizePolicy().hasHeightForWidth());
        gaspTab->setSizePolicy(sizePolicy);
        gaspTab->setMinimumSize(QSize(0, 800));
        groupBox_filterGasp = new QGroupBox(gaspTab);
        groupBox_filterGasp->setObjectName(QString::fromUtf8("groupBox_filterGasp"));
        groupBox_filterGasp->setGeometry(QRect(4, 0, 1800, 700));
        sizePolicy.setHeightForWidth(groupBox_filterGasp->sizePolicy().hasHeightForWidth());
        groupBox_filterGasp->setSizePolicy(sizePolicy);
        groupBox_filterGasp->setMinimumSize(QSize(1800, 700));
        gridLayout = new QGridLayout(groupBox_filterGasp);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 5, 3, 1, 1);

        aodDSpinBox = new QDoubleSpinBox(groupBox_filterGasp);
        aodDSpinBox->setObjectName(QString::fromUtf8("aodDSpinBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(aodDSpinBox->sizePolicy().hasHeightForWidth());
        aodDSpinBox->setSizePolicy(sizePolicy1);
        aodDSpinBox->setMinimumSize(QSize(120, 24));
        aodDSpinBox->setMaximumSize(QSize(120, 24));
        aodDSpinBox->setFrame(true);
        aodDSpinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
        aodDSpinBox->setDecimals(3);
        aodDSpinBox->setMinimum(-0.5);
        aodDSpinBox->setMaximum(2.05);
        aodDSpinBox->setSingleStep(0.01);
        aodDSpinBox->setValue(-0.5);

        gridLayout->addWidget(aodDSpinBox, 2, 1, 1, 1);

        label_26 = new QLabel(groupBox_filterGasp);
        label_26->setObjectName(QString::fromUtf8("label_26"));

        gridLayout->addWidget(label_26, 1, 2, 1, 1);

        aodDSpinBox2 = new QDoubleSpinBox(groupBox_filterGasp);
        aodDSpinBox2->setObjectName(QString::fromUtf8("aodDSpinBox2"));
        sizePolicy1.setHeightForWidth(aodDSpinBox2->sizePolicy().hasHeightForWidth());
        aodDSpinBox2->setSizePolicy(sizePolicy1);
        aodDSpinBox2->setMinimumSize(QSize(120, 24));
        aodDSpinBox2->setMaximumSize(QSize(120, 24));
        aodDSpinBox2->setButtonSymbols(QAbstractSpinBox::NoButtons);
        aodDSpinBox2->setDecimals(3);
        aodDSpinBox2->setMinimum(-1e+08);
        aodDSpinBox2->setMaximum(1e+08);
        aodDSpinBox2->setValue(2.05);

        gridLayout->addWidget(aodDSpinBox2, 2, 2, 1, 1);

        label_25 = new QLabel(groupBox_filterGasp);
        label_25->setObjectName(QString::fromUtf8("label_25"));

        gridLayout->addWidget(label_25, 3, 0, 1, 1);

        aodstdDSpinBox = new QDoubleSpinBox(groupBox_filterGasp);
        aodstdDSpinBox->setObjectName(QString::fromUtf8("aodstdDSpinBox"));
        sizePolicy1.setHeightForWidth(aodstdDSpinBox->sizePolicy().hasHeightForWidth());
        aodstdDSpinBox->setSizePolicy(sizePolicy1);
        aodstdDSpinBox->setMinimumSize(QSize(120, 24));
        aodstdDSpinBox->setMaximumSize(QSize(120, 24));
        aodstdDSpinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
        aodstdDSpinBox->setDecimals(3);
        aodstdDSpinBox->setMinimum(0);
        aodstdDSpinBox->setMaximum(2.55);

        gridLayout->addWidget(aodstdDSpinBox, 3, 1, 1, 1);

        aodstdDSpinBox2 = new QDoubleSpinBox(groupBox_filterGasp);
        aodstdDSpinBox2->setObjectName(QString::fromUtf8("aodstdDSpinBox2"));
        sizePolicy1.setHeightForWidth(aodstdDSpinBox2->sizePolicy().hasHeightForWidth());
        aodstdDSpinBox2->setSizePolicy(sizePolicy1);
        aodstdDSpinBox2->setMinimumSize(QSize(120, 24));
        aodstdDSpinBox2->setMaximumSize(QSize(120, 24));
        aodstdDSpinBox2->setButtonSymbols(QAbstractSpinBox::NoButtons);
        aodstdDSpinBox2->setDecimals(3);
        aodstdDSpinBox2->setMinimum(-1e+08);
        aodstdDSpinBox2->setMaximum(1e+08);
        aodstdDSpinBox2->setSingleStep(0.01);
        aodstdDSpinBox2->setValue(2.55);

        gridLayout->addWidget(aodstdDSpinBox2, 3, 2, 1, 1);

        label_18 = new QLabel(groupBox_filterGasp);
        label_18->setObjectName(QString::fromUtf8("label_18"));

        gridLayout->addWidget(label_18, 2, 0, 1, 1);

        label_19 = new QLabel(groupBox_filterGasp);
        label_19->setObjectName(QString::fromUtf8("label_19"));

        gridLayout->addWidget(label_19, 1, 1, 1, 1);

        label_21 = new QLabel(groupBox_filterGasp);
        label_21->setObjectName(QString::fromUtf8("label_21"));

        gridLayout->addWidget(label_21, 8, 0, 1, 1);

        mosDSpinBox = new QDoubleSpinBox(groupBox_filterGasp);
        mosDSpinBox->setObjectName(QString::fromUtf8("mosDSpinBox"));
        sizePolicy1.setHeightForWidth(mosDSpinBox->sizePolicy().hasHeightForWidth());
        mosDSpinBox->setSizePolicy(sizePolicy1);
        mosDSpinBox->setMinimumSize(QSize(120, 24));
        mosDSpinBox->setMaximumSize(QSize(120, 24));
        mosDSpinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
        mosDSpinBox->setDecimals(3);
        mosDSpinBox->setMinimum(0);
        mosDSpinBox->setMaximum(0.425);

        gridLayout->addWidget(mosDSpinBox, 8, 1, 1, 1);

        mosDSpinBox2 = new QDoubleSpinBox(groupBox_filterGasp);
        mosDSpinBox2->setObjectName(QString::fromUtf8("mosDSpinBox2"));
        sizePolicy1.setHeightForWidth(mosDSpinBox2->sizePolicy().hasHeightForWidth());
        mosDSpinBox2->setSizePolicy(sizePolicy1);
        mosDSpinBox2->setMinimumSize(QSize(120, 24));
        mosDSpinBox2->setMaximumSize(QSize(120, 24));
        mosDSpinBox2->setButtonSymbols(QAbstractSpinBox::NoButtons);
        mosDSpinBox2->setDecimals(3);
        mosDSpinBox2->setMinimum(-1e+08);
        mosDSpinBox2->setMaximum(1e+08);
        mosDSpinBox2->setValue(0.425);

        gridLayout->addWidget(mosDSpinBox2, 8, 2, 1, 1);

        label_28 = new QLabel(groupBox_filterGasp);
        label_28->setObjectName(QString::fromUtf8("label_28"));

        gridLayout->addWidget(label_28, 6, 0, 1, 1);

        ch1DSpinBox = new QDoubleSpinBox(groupBox_filterGasp);
        ch1DSpinBox->setObjectName(QString::fromUtf8("ch1DSpinBox"));
        sizePolicy1.setHeightForWidth(ch1DSpinBox->sizePolicy().hasHeightForWidth());
        ch1DSpinBox->setSizePolicy(sizePolicy1);
        ch1DSpinBox->setMinimumSize(QSize(120, 24));
        ch1DSpinBox->setMaximumSize(QSize(120, 24));
        ch1DSpinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
        ch1DSpinBox->setDecimals(3);
        ch1DSpinBox->setMinimum(0);
        ch1DSpinBox->setMaximum(0.425);

        gridLayout->addWidget(ch1DSpinBox, 5, 1, 1, 1);

        clsDSpinBox2 = new QDoubleSpinBox(groupBox_filterGasp);
        clsDSpinBox2->setObjectName(QString::fromUtf8("clsDSpinBox2"));
        sizePolicy1.setHeightForWidth(clsDSpinBox2->sizePolicy().hasHeightForWidth());
        clsDSpinBox2->setSizePolicy(sizePolicy1);
        clsDSpinBox2->setMinimumSize(QSize(120, 24));
        clsDSpinBox2->setMaximumSize(QSize(120, 24));
        clsDSpinBox2->setButtonSymbols(QAbstractSpinBox::NoButtons);
        clsDSpinBox2->setDecimals(3);
        clsDSpinBox2->setMinimum(-1e+08);
        clsDSpinBox2->setMaximum(1e+08);
        clsDSpinBox2->setValue(25);

        gridLayout->addWidget(clsDSpinBox2, 4, 2, 1, 1);

        ch1DSpinBox2 = new QDoubleSpinBox(groupBox_filterGasp);
        ch1DSpinBox2->setObjectName(QString::fromUtf8("ch1DSpinBox2"));
        sizePolicy1.setHeightForWidth(ch1DSpinBox2->sizePolicy().hasHeightForWidth());
        ch1DSpinBox2->setSizePolicy(sizePolicy1);
        ch1DSpinBox2->setMinimumSize(QSize(120, 24));
        ch1DSpinBox2->setMaximumSize(QSize(120, 24));
        ch1DSpinBox2->setButtonSymbols(QAbstractSpinBox::NoButtons);
        ch1DSpinBox2->setDecimals(3);
        ch1DSpinBox2->setMinimum(-1e+08);
        ch1DSpinBox2->setMaximum(1e+08);
        ch1DSpinBox2->setValue(0.425);

        gridLayout->addWidget(ch1DSpinBox2, 5, 2, 1, 1);

        sigDSpinBox = new QDoubleSpinBox(groupBox_filterGasp);
        sigDSpinBox->setObjectName(QString::fromUtf8("sigDSpinBox"));
        sizePolicy1.setHeightForWidth(sigDSpinBox->sizePolicy().hasHeightForWidth());
        sigDSpinBox->setSizePolicy(sizePolicy1);
        sigDSpinBox->setMinimumSize(QSize(120, 24));
        sigDSpinBox->setMaximumSize(QSize(120, 24));
        sigDSpinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
        sigDSpinBox->setDecimals(3);
        sigDSpinBox->setMinimum(-0.5);
        sigDSpinBox->setMaximum(0.52);
        sigDSpinBox->setValue(-0.5);

        gridLayout->addWidget(sigDSpinBox, 6, 1, 1, 1);

        sigDSpinBox2 = new QDoubleSpinBox(groupBox_filterGasp);
        sigDSpinBox2->setObjectName(QString::fromUtf8("sigDSpinBox2"));
        sizePolicy1.setHeightForWidth(sigDSpinBox2->sizePolicy().hasHeightForWidth());
        sigDSpinBox2->setSizePolicy(sizePolicy1);
        sigDSpinBox2->setMinimumSize(QSize(120, 24));
        sigDSpinBox2->setMaximumSize(QSize(120, 24));
        sigDSpinBox2->setButtonSymbols(QAbstractSpinBox::NoButtons);
        sigDSpinBox2->setDecimals(3);
        sigDSpinBox2->setMinimum(-1e+08);
        sigDSpinBox2->setMaximum(1e+08);
        sigDSpinBox2->setValue(0.52);

        gridLayout->addWidget(sigDSpinBox2, 6, 2, 1, 1);

        label_17 = new QLabel(groupBox_filterGasp);
        label_17->setObjectName(QString::fromUtf8("label_17"));

        gridLayout->addWidget(label_17, 5, 0, 1, 1);

        label_23 = new QLabel(groupBox_filterGasp);
        label_23->setObjectName(QString::fromUtf8("label_23"));

        gridLayout->addWidget(label_23, 4, 0, 1, 1);

        label_20 = new QLabel(groupBox_filterGasp);
        label_20->setObjectName(QString::fromUtf8("label_20"));

        gridLayout->addWidget(label_20, 7, 0, 1, 1);

        sfcDSpinBox = new QDoubleSpinBox(groupBox_filterGasp);
        sfcDSpinBox->setObjectName(QString::fromUtf8("sfcDSpinBox"));
        sizePolicy1.setHeightForWidth(sfcDSpinBox->sizePolicy().hasHeightForWidth());
        sfcDSpinBox->setSizePolicy(sizePolicy1);
        sfcDSpinBox->setMinimumSize(QSize(120, 24));
        sfcDSpinBox->setMaximumSize(QSize(120, 24));
        sfcDSpinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
        sfcDSpinBox->setDecimals(3);
        sfcDSpinBox->setMinimum(-0.1);
        sfcDSpinBox->setMaximum(0.41);
        sfcDSpinBox->setSingleStep(0.1);
        sfcDSpinBox->setValue(-0.1);

        gridLayout->addWidget(sfcDSpinBox, 7, 1, 1, 1);

        sfcDSpinBox2 = new QDoubleSpinBox(groupBox_filterGasp);
        sfcDSpinBox2->setObjectName(QString::fromUtf8("sfcDSpinBox2"));
        sizePolicy1.setHeightForWidth(sfcDSpinBox2->sizePolicy().hasHeightForWidth());
        sfcDSpinBox2->setSizePolicy(sizePolicy1);
        sfcDSpinBox2->setMinimumSize(QSize(120, 24));
        sfcDSpinBox2->setMaximumSize(QSize(120, 24));
        sfcDSpinBox2->setButtonSymbols(QAbstractSpinBox::NoButtons);
        sfcDSpinBox2->setDecimals(3);
        sfcDSpinBox2->setMinimum(-1e+08);
        sfcDSpinBox2->setMaximum(1e+08);
        sfcDSpinBox2->setValue(0.41);

        gridLayout->addWidget(sfcDSpinBox2, 7, 2, 1, 1);

        clsDSpinBox = new QDoubleSpinBox(groupBox_filterGasp);
        clsDSpinBox->setObjectName(QString::fromUtf8("clsDSpinBox"));
        sizePolicy1.setHeightForWidth(clsDSpinBox->sizePolicy().hasHeightForWidth());
        clsDSpinBox->setSizePolicy(sizePolicy1);
        clsDSpinBox->setMinimumSize(QSize(120, 24));
        clsDSpinBox->setMaximumSize(QSize(120, 24));
        clsDSpinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
        clsDSpinBox->setDecimals(3);
        clsDSpinBox->setMinimum(0);
        clsDSpinBox->setMaximum(25);

        gridLayout->addWidget(clsDSpinBox, 4, 1, 1, 1);

        label_27 = new QLabel(groupBox_filterGasp);
        label_27->setObjectName(QString::fromUtf8("label_27"));
        label_27->setTextFormat(Qt::RichText);
        label_27->setWordWrap(true);

        gridLayout->addWidget(label_27, 10, 0, 1, 3);

        scaDSpinBox = new QDoubleSpinBox(groupBox_filterGasp);
        scaDSpinBox->setObjectName(QString::fromUtf8("scaDSpinBox"));
        sizePolicy1.setHeightForWidth(scaDSpinBox->sizePolicy().hasHeightForWidth());
        scaDSpinBox->setSizePolicy(sizePolicy1);
        scaDSpinBox->setMinimumSize(QSize(120, 24));
        scaDSpinBox->setMaximumSize(QSize(120, 24));
        scaDSpinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
        scaDSpinBox->setDecimals(3);
        scaDSpinBox->setMinimum(0);
        scaDSpinBox->setMaximum(0.425);

        gridLayout->addWidget(scaDSpinBox, 9, 1, 1, 1);

        scaDSpinBox2 = new QDoubleSpinBox(groupBox_filterGasp);
        scaDSpinBox2->setObjectName(QString::fromUtf8("scaDSpinBox2"));
        sizePolicy1.setHeightForWidth(scaDSpinBox2->sizePolicy().hasHeightForWidth());
        scaDSpinBox2->setSizePolicy(sizePolicy1);
        scaDSpinBox2->setMinimumSize(QSize(120, 24));
        scaDSpinBox2->setMaximumSize(QSize(120, 24));
        scaDSpinBox2->setButtonSymbols(QAbstractSpinBox::NoButtons);
        scaDSpinBox2->setDecimals(3);
        scaDSpinBox2->setMinimum(-1e+08);
        scaDSpinBox2->setMaximum(1e+08);
        scaDSpinBox2->setValue(0.425);

        gridLayout->addWidget(scaDSpinBox2, 9, 2, 1, 1);

        label_24 = new QLabel(groupBox_filterGasp);
        label_24->setObjectName(QString::fromUtf8("label_24"));

        gridLayout->addWidget(label_24, 9, 0, 1, 1);

        label_22 = new QLabel(groupBox_filterGasp);
        label_22->setObjectName(QString::fromUtf8("label_22"));
        label_22->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout->addWidget(label_22, 0, 0, 1, 3);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 11, 0, 1, 1);

        tabWidget->addTab(gaspTab, QString());
        calipsoTab = new QWidget();
        calipsoTab->setObjectName(QString::fromUtf8("calipsoTab"));
        calipsoTab->setMinimumSize(QSize(1800, 800));
        groupBox_filterCalipso = new QGroupBox(calipsoTab);
        groupBox_filterCalipso->setObjectName(QString::fromUtf8("groupBox_filterCalipso"));
        groupBox_filterCalipso->setGeometry(QRect(0, 4, 1800, 700));
        sizePolicy.setHeightForWidth(groupBox_filterCalipso->sizePolicy().hasHeightForWidth());
        groupBox_filterCalipso->setSizePolicy(sizePolicy);
        groupBox_filterCalipso->setMinimumSize(QSize(1800, 700));
        gridLayout_2 = new QGridLayout(groupBox_filterCalipso);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_3, 3, 4, 1, 1);

        label_30 = new QLabel(groupBox_filterCalipso);
        label_30->setObjectName(QString::fromUtf8("label_30"));

        gridLayout_2->addWidget(label_30, 0, 0, 1, 3);

        label_32 = new QLabel(groupBox_filterCalipso);
        label_32->setObjectName(QString::fromUtf8("label_32"));

        gridLayout_2->addWidget(label_32, 1, 0, 1, 2);

        mincadSpinBox = new QSpinBox(groupBox_filterCalipso);
        mincadSpinBox->setObjectName(QString::fromUtf8("mincadSpinBox"));
        sizePolicy1.setHeightForWidth(mincadSpinBox->sizePolicy().hasHeightForWidth());
        mincadSpinBox->setSizePolicy(sizePolicy1);
        mincadSpinBox->setMinimumSize(QSize(80, 30));
        mincadSpinBox->setMaximumSize(QSize(50, 24));
        mincadSpinBox->setAccelerated(true);
        mincadSpinBox->setMinimum(20);
        mincadSpinBox->setMaximum(100);

        gridLayout_2->addWidget(mincadSpinBox, 1, 2, 1, 2);

        label_29 = new QLabel(groupBox_filterCalipso);
        label_29->setObjectName(QString::fromUtf8("label_29"));
        label_29->setTextFormat(Qt::RichText);
        label_29->setWordWrap(true);

        gridLayout_2->addWidget(label_29, 2, 0, 1, 4);

        maxuncetaintyDSpinBox = new QDoubleSpinBox(groupBox_filterCalipso);
        maxuncetaintyDSpinBox->setObjectName(QString::fromUtf8("maxuncetaintyDSpinBox"));
        sizePolicy1.setHeightForWidth(maxuncetaintyDSpinBox->sizePolicy().hasHeightForWidth());
        maxuncetaintyDSpinBox->setSizePolicy(sizePolicy1);
        maxuncetaintyDSpinBox->setMinimumSize(QSize(80, 30));
        maxuncetaintyDSpinBox->setMaximumSize(QSize(80, 24));
        maxuncetaintyDSpinBox->setDecimals(3);
        maxuncetaintyDSpinBox->setMaximum(100);
        maxuncetaintyDSpinBox->setSingleStep(0.1);
        maxuncetaintyDSpinBox->setValue(99);

        gridLayout_2->addWidget(maxuncetaintyDSpinBox, 3, 3, 1, 1);

        label_31 = new QLabel(groupBox_filterCalipso);
        label_31->setObjectName(QString::fromUtf8("label_31"));
        label_31->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_31->setWordWrap(true);

        gridLayout_2->addWidget(label_31, 3, 0, 1, 3);

        label_35 = new QLabel(groupBox_filterCalipso);
        label_35->setObjectName(QString::fromUtf8("label_35"));
        label_35->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_35->setWordWrap(true);

        gridLayout_2->addWidget(label_35, 5, 0, 2, 2);

        calipsoVersionL1Combobox = new QComboBox(groupBox_filterCalipso);
        calipsoVersionL1Combobox->setObjectName(QString::fromUtf8("calipsoVersionL1Combobox"));
        sizePolicy1.setHeightForWidth(calipsoVersionL1Combobox->sizePolicy().hasHeightForWidth());
        calipsoVersionL1Combobox->setSizePolicy(sizePolicy1);
        calipsoVersionL1Combobox->setMinimumSize(QSize(0, 30));

        gridLayout_2->addWidget(calipsoVersionL1Combobox, 5, 2, 1, 1);

        label_2 = new QLabel(groupBox_filterCalipso);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setTextFormat(Qt::RichText);
        label_2->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

        gridLayout_2->addWidget(label_2, 9, 0, 1, 1);

        label_36 = new QLabel(groupBox_filterCalipso);
        label_36->setObjectName(QString::fromUtf8("label_36"));
        label_36->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_36->setWordWrap(true);

        gridLayout_2->addWidget(label_36, 7, 0, 2, 2);

        calipsoVersionL2Combobox = new QComboBox(groupBox_filterCalipso);
        calipsoVersionL2Combobox->setObjectName(QString::fromUtf8("calipsoVersionL2Combobox"));
        sizePolicy1.setHeightForWidth(calipsoVersionL2Combobox->sizePolicy().hasHeightForWidth());
        calipsoVersionL2Combobox->setSizePolicy(sizePolicy1);
        calipsoVersionL2Combobox->setMinimumSize(QSize(0, 30));

        gridLayout_2->addWidget(calipsoVersionL2Combobox, 7, 2, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer_2, 10, 0, 1, 1);

        tabWidget->addTab(calipsoTab, QString());
        OMITab = new QWidget();
        OMITab->setObjectName(QString::fromUtf8("OMITab"));
        sizePolicy.setHeightForWidth(OMITab->sizePolicy().hasHeightForWidth());
        OMITab->setSizePolicy(sizePolicy);
        OMITab->setMinimumSize(QSize(1800, 800));
        groupBox_filterOmi = new QGroupBox(OMITab);
        groupBox_filterOmi->setObjectName(QString::fromUtf8("groupBox_filterOmi"));
        groupBox_filterOmi->setGeometry(QRect(0, 4, 565, 700));
        sizePolicy.setHeightForWidth(groupBox_filterOmi->sizePolicy().hasHeightForWidth());
        groupBox_filterOmi->setSizePolicy(sizePolicy);
        groupBox_filterOmi->setMinimumSize(QSize(0, 700));
        gridLayout_3 = new QGridLayout(groupBox_filterOmi);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        maxOMICloudFractionSpinBox = new QDoubleSpinBox(groupBox_filterOmi);
        maxOMICloudFractionSpinBox->setObjectName(QString::fromUtf8("maxOMICloudFractionSpinBox"));
        sizePolicy1.setHeightForWidth(maxOMICloudFractionSpinBox->sizePolicy().hasHeightForWidth());
        maxOMICloudFractionSpinBox->setSizePolicy(sizePolicy1);
        maxOMICloudFractionSpinBox->setMinimumSize(QSize(120, 24));
        maxOMICloudFractionSpinBox->setMaximumSize(QSize(120, 24));
        maxOMICloudFractionSpinBox->setDecimals(3);
        maxOMICloudFractionSpinBox->setMaximum(1);
        maxOMICloudFractionSpinBox->setSingleStep(0.1);
        maxOMICloudFractionSpinBox->setValue(1);

        gridLayout_3->addWidget(maxOMICloudFractionSpinBox, 1, 0, 1, 1);

        label_OMI2 = new QLabel(groupBox_filterOmi);
        label_OMI2->setObjectName(QString::fromUtf8("label_OMI2"));

        gridLayout_3->addWidget(label_OMI2, 2, 0, 1, 1);

        label_OMI1 = new QLabel(groupBox_filterOmi);
        label_OMI1->setObjectName(QString::fromUtf8("label_OMI1"));

        gridLayout_3->addWidget(label_OMI1, 0, 0, 1, 1);

        OMIAllowNegativeCounts = new QCheckBox(groupBox_filterOmi);
        OMIAllowNegativeCounts->setObjectName(QString::fromUtf8("OMIAllowNegativeCounts"));
        sizePolicy1.setHeightForWidth(OMIAllowNegativeCounts->sizePolicy().hasHeightForWidth());
        OMIAllowNegativeCounts->setSizePolicy(sizePolicy1);

        gridLayout_3->addWidget(OMIAllowNegativeCounts, 4, 0, 1, 1);

        maxOMISolarZenithAngleSpinBox = new QDoubleSpinBox(groupBox_filterOmi);
        maxOMISolarZenithAngleSpinBox->setObjectName(QString::fromUtf8("maxOMISolarZenithAngleSpinBox"));
        sizePolicy1.setHeightForWidth(maxOMISolarZenithAngleSpinBox->sizePolicy().hasHeightForWidth());
        maxOMISolarZenithAngleSpinBox->setSizePolicy(sizePolicy1);
        maxOMISolarZenithAngleSpinBox->setMinimumSize(QSize(120, 24));
        maxOMISolarZenithAngleSpinBox->setMaximumSize(QSize(120, 24));
        maxOMISolarZenithAngleSpinBox->setDecimals(3);
        maxOMISolarZenithAngleSpinBox->setMaximum(90);
        maxOMISolarZenithAngleSpinBox->setSingleStep(1);
        maxOMISolarZenithAngleSpinBox->setValue(90);

        gridLayout_3->addWidget(maxOMISolarZenithAngleSpinBox, 3, 0, 1, 1);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_4, 2, 1, 1, 1);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_3->addItem(verticalSpacer_3, 5, 0, 1, 1);

        tabWidget->addTab(OMITab, QString());
        tropomiTab = new QWidget();
        tropomiTab->setObjectName(QString::fromUtf8("tropomiTab"));
        tropomiTab->setMinimumSize(QSize(1800, 800));
        groupBox_filterTropomi = new QGroupBox(tropomiTab);
        groupBox_filterTropomi->setObjectName(QString::fromUtf8("groupBox_filterTropomi"));
        groupBox_filterTropomi->setGeometry(QRect(0, 4, 593, 700));
        sizePolicy.setHeightForWidth(groupBox_filterTropomi->sizePolicy().hasHeightForWidth());
        groupBox_filterTropomi->setSizePolicy(sizePolicy);
        groupBox_filterTropomi->setMinimumSize(QSize(0, 700));
        gridLayout_4 = new QGridLayout(groupBox_filterTropomi);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        label_34 = new QLabel(groupBox_filterTropomi);
        label_34->setObjectName(QString::fromUtf8("label_34"));

        gridLayout_4->addWidget(label_34, 2, 0, 1, 1);

        TropomiGroundPixelMinimumSpinBox = new QSpinBox(groupBox_filterTropomi);
        TropomiGroundPixelMinimumSpinBox->setObjectName(QString::fromUtf8("TropomiGroundPixelMinimumSpinBox"));
        sizePolicy1.setHeightForWidth(TropomiGroundPixelMinimumSpinBox->sizePolicy().hasHeightForWidth());
        TropomiGroundPixelMinimumSpinBox->setSizePolicy(sizePolicy1);
        TropomiGroundPixelMinimumSpinBox->setMinimum(0);
        TropomiGroundPixelMinimumSpinBox->setMaximum(449);
        TropomiGroundPixelMinimumSpinBox->setValue(0);

        gridLayout_4->addWidget(TropomiGroundPixelMinimumSpinBox, 5, 0, 1, 1);

        maxTropomiCloudFractionSpinBox = new QDoubleSpinBox(groupBox_filterTropomi);
        maxTropomiCloudFractionSpinBox->setObjectName(QString::fromUtf8("maxTropomiCloudFractionSpinBox"));
        QSizePolicy sizePolicy2(QSizePolicy::Maximum, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(maxTropomiCloudFractionSpinBox->sizePolicy().hasHeightForWidth());
        maxTropomiCloudFractionSpinBox->setSizePolicy(sizePolicy2);
        maxTropomiCloudFractionSpinBox->setMinimumSize(QSize(120, 24));
        maxTropomiCloudFractionSpinBox->setMaximumSize(QSize(120, 24));
        maxTropomiCloudFractionSpinBox->setDecimals(3);
        maxTropomiCloudFractionSpinBox->setMaximum(1);
        maxTropomiCloudFractionSpinBox->setSingleStep(0.1);
        maxTropomiCloudFractionSpinBox->setValue(1);

        gridLayout_4->addWidget(maxTropomiCloudFractionSpinBox, 3, 0, 1, 1);

        TropomiGroundPixelMinimumLabel = new QLabel(groupBox_filterTropomi);
        TropomiGroundPixelMinimumLabel->setObjectName(QString::fromUtf8("TropomiGroundPixelMinimumLabel"));

        gridLayout_4->addWidget(TropomiGroundPixelMinimumLabel, 4, 0, 1, 1);

        label_33 = new QLabel(groupBox_filterTropomi);
        label_33->setObjectName(QString::fromUtf8("label_33"));

        gridLayout_4->addWidget(label_33, 0, 0, 1, 1);

        minTropomiQualitySpinBox = new QSpinBox(groupBox_filterTropomi);
        minTropomiQualitySpinBox->setObjectName(QString::fromUtf8("minTropomiQualitySpinBox"));
        sizePolicy1.setHeightForWidth(minTropomiQualitySpinBox->sizePolicy().hasHeightForWidth());
        minTropomiQualitySpinBox->setSizePolicy(sizePolicy1);
        minTropomiQualitySpinBox->setMinimum(50);
        minTropomiQualitySpinBox->setMaximum(100);
        minTropomiQualitySpinBox->setValue(75);

        gridLayout_4->addWidget(minTropomiQualitySpinBox, 1, 0, 1, 1);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_4->addItem(verticalSpacer_4, 9, 0, 1, 1);

        TropomiGroundPixelMaximumSpinBox = new QSpinBox(groupBox_filterTropomi);
        TropomiGroundPixelMaximumSpinBox->setObjectName(QString::fromUtf8("TropomiGroundPixelMaximumSpinBox"));
        sizePolicy1.setHeightForWidth(TropomiGroundPixelMaximumSpinBox->sizePolicy().hasHeightForWidth());
        TropomiGroundPixelMaximumSpinBox->setSizePolicy(sizePolicy1);
        TropomiGroundPixelMaximumSpinBox->setMinimum(0);
        TropomiGroundPixelMaximumSpinBox->setMaximum(449);
        TropomiGroundPixelMaximumSpinBox->setValue(0);

        gridLayout_4->addWidget(TropomiGroundPixelMaximumSpinBox, 7, 0, 1, 1);

        TropomiGroundPixelMaximumLabel = new QLabel(groupBox_filterTropomi);
        TropomiGroundPixelMaximumLabel->setObjectName(QString::fromUtf8("TropomiGroundPixelMaximumLabel"));

        gridLayout_4->addWidget(TropomiGroundPixelMaximumLabel, 6, 0, 1, 1);

        TropomiAllowNegativeCounts = new QCheckBox(groupBox_filterTropomi);
        TropomiAllowNegativeCounts->setObjectName(QString::fromUtf8("TropomiAllowNegativeCounts"));
        sizePolicy1.setHeightForWidth(TropomiAllowNegativeCounts->sizePolicy().hasHeightForWidth());
        TropomiAllowNegativeCounts->setSizePolicy(sizePolicy1);

        gridLayout_4->addWidget(TropomiAllowNegativeCounts, 8, 0, 1, 1);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_4->addItem(horizontalSpacer_5, 4, 1, 1, 1);

        tabWidget->addTab(tropomiTab, QString());
        viirsTab = new QWidget();
        viirsTab->setObjectName(QString::fromUtf8("viirsTab"));
        viirsTab->setMinimumSize(QSize(1800, 800));
        groupBox_filterViirs = new QGroupBox(viirsTab);
        groupBox_filterViirs->setObjectName(QString::fromUtf8("groupBox_filterViirs"));
        groupBox_filterViirs->setGeometry(QRect(4, 4, 397, 700));
        sizePolicy.setHeightForWidth(groupBox_filterViirs->sizePolicy().hasHeightForWidth());
        groupBox_filterViirs->setSizePolicy(sizePolicy);
        groupBox_filterViirs->setMinimumSize(QSize(0, 700));
        gridLayout_5 = new QGridLayout(groupBox_filterViirs);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        viirsNoaaQAflagMenu = new QComboBox(groupBox_filterViirs);
        viirsNoaaQAflagMenu->setObjectName(QString::fromUtf8("viirsNoaaQAflagMenu"));
        sizePolicy1.setHeightForWidth(viirsNoaaQAflagMenu->sizePolicy().hasHeightForWidth());
        viirsNoaaQAflagMenu->setSizePolicy(sizePolicy1);

        gridLayout_5->addWidget(viirsNoaaQAflagMenu, 1, 0, 1, 1);

        viirs_label = new QLabel(groupBox_filterViirs);
        viirs_label->setObjectName(QString::fromUtf8("viirs_label"));

        gridLayout_5->addWidget(viirs_label, 0, 0, 1, 1);

        verticalSpacer_5 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_5->addItem(verticalSpacer_5, 2, 0, 1, 1);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_5->addItem(horizontalSpacer_6, 1, 1, 1, 1);

        tabWidget->addTab(viirsTab, QString());
        pandoraTab = new QWidget();
        pandoraTab->setObjectName(QString::fromUtf8("pandoraTab"));
        pandoraTab->setMinimumSize(QSize(1800, 800));
        groupBox_filterPandora = new QGroupBox(pandoraTab);
        groupBox_filterPandora->setObjectName(QString::fromUtf8("groupBox_filterPandora"));
        groupBox_filterPandora->setGeometry(QRect(0, 4, 473, 700));
        sizePolicy.setHeightForWidth(groupBox_filterPandora->sizePolicy().hasHeightForWidth());
        groupBox_filterPandora->setSizePolicy(sizePolicy);
        groupBox_filterPandora->setMinimumSize(QSize(0, 700));
        gridLayout_6 = new QGridLayout(groupBox_filterPandora);
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        pandoraQAflagMenu = new QComboBox(groupBox_filterPandora);
        pandoraQAflagMenu->setObjectName(QString::fromUtf8("pandoraQAflagMenu"));
        sizePolicy1.setHeightForWidth(pandoraQAflagMenu->sizePolicy().hasHeightForWidth());
        pandoraQAflagMenu->setSizePolicy(sizePolicy1);

        gridLayout_6->addWidget(pandoraQAflagMenu, 1, 0, 1, 1);

        pandora_label = new QLabel(groupBox_filterPandora);
        pandora_label->setObjectName(QString::fromUtf8("pandora_label"));

        gridLayout_6->addWidget(pandora_label, 0, 0, 1, 1);

        verticalSpacer_6 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_6->addItem(verticalSpacer_6, 2, 0, 1, 1);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_6->addItem(horizontalSpacer_7, 0, 1, 1, 1);

        tabWidget->addTab(pandoraTab, QString());
        purpleairTab = new QWidget();
        purpleairTab->setObjectName(QString::fromUtf8("purpleairTab"));
        purpleairTab->setMinimumSize(QSize(1800, 800));
        groupBox_filterPurpleair = new QGroupBox(purpleairTab);
        groupBox_filterPurpleair->setObjectName(QString::fromUtf8("groupBox_filterPurpleair"));
        groupBox_filterPurpleair->setGeometry(QRect(0, 4, 1800, 700));
        sizePolicy.setHeightForWidth(groupBox_filterPurpleair->sizePolicy().hasHeightForWidth());
        groupBox_filterPurpleair->setSizePolicy(sizePolicy);
        groupBox_filterPurpleair->setMinimumSize(QSize(1800, 700));
        gridLayout_7 = new QGridLayout(groupBox_filterPurpleair);
        gridLayout_7->setObjectName(QString::fromUtf8("gridLayout_7"));
        gridLayout_7->setVerticalSpacing(10);
        groupBox = new QGroupBox(groupBox_filterPurpleair);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        horizontalLayout_2 = new QHBoxLayout(groupBox);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        acceptablePurpleairChannelDifference = new QSpinBox(groupBox);
        acceptablePurpleairChannelDifference->setObjectName(QString::fromUtf8("acceptablePurpleairChannelDifference"));
        sizePolicy1.setHeightForWidth(acceptablePurpleairChannelDifference->sizePolicy().hasHeightForWidth());
        acceptablePurpleairChannelDifference->setSizePolicy(sizePolicy1);
        acceptablePurpleairChannelDifference->setMaximumSize(QSize(200, 16777215));
        acceptablePurpleairChannelDifference->setMinimum(1);
        acceptablePurpleairChannelDifference->setValue(5);

        horizontalLayout_2->addWidget(acceptablePurpleairChannelDifference);

        PurpleAirLabel_6 = new QLabel(groupBox);
        PurpleAirLabel_6->setObjectName(QString::fromUtf8("PurpleAirLabel_6"));

        horizontalLayout_2->addWidget(PurpleAirLabel_6);

        horizontalSpacer_11 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_11);


        gridLayout_7->addWidget(groupBox, 4, 1, 1, 1);

        groupBox_3 = new QGroupBox(groupBox_filterPurpleair);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        sizePolicy.setHeightForWidth(groupBox_3->sizePolicy().hasHeightForWidth());
        groupBox_3->setSizePolicy(sizePolicy);
        groupBox_3->setMaximumSize(QSize(16777215, 60));
        gridLayout_8 = new QGridLayout(groupBox_3);
        gridLayout_8->setObjectName(QString::fromUtf8("gridLayout_8"));
        PurpleAirLabel_4 = new QLabel(groupBox_3);
        PurpleAirLabel_4->setObjectName(QString::fromUtf8("PurpleAirLabel_4"));
        PurpleAirLabel_4->setMaximumSize(QSize(16777215, 60));

        gridLayout_8->addWidget(PurpleAirLabel_4, 0, 0, 1, 1);

        purpleairHumidityReplacementValue = new QSpinBox(groupBox_3);
        purpleairHumidityReplacementValue->setObjectName(QString::fromUtf8("purpleairHumidityReplacementValue"));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(purpleairHumidityReplacementValue->sizePolicy().hasHeightForWidth());
        purpleairHumidityReplacementValue->setSizePolicy(sizePolicy3);
        purpleairHumidityReplacementValue->setMinimumSize(QSize(0, 30));
        purpleairHumidityReplacementValue->setMaximumSize(QSize(16777215, 60));
        purpleairHumidityReplacementValue->setMinimum(1);
        purpleairHumidityReplacementValue->setMaximum(100);
        purpleairHumidityReplacementValue->setValue(50);

        gridLayout_8->addWidget(purpleairHumidityReplacementValue, 0, 1, 1, 1);

        PurpleAirLabel_9 = new QLabel(groupBox_3);
        PurpleAirLabel_9->setObjectName(QString::fromUtf8("PurpleAirLabel_9"));

        gridLayout_8->addWidget(PurpleAirLabel_9, 0, 2, 1, 1);

        horizontalSpacer_9 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_8->addItem(horizontalSpacer_9, 0, 3, 1, 1);


        gridLayout_7->addWidget(groupBox_3, 3, 1, 1, 1);

        groupBox_2 = new QGroupBox(groupBox_filterPurpleair);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        sizePolicy.setHeightForWidth(groupBox_2->sizePolicy().hasHeightForWidth());
        groupBox_2->setSizePolicy(sizePolicy);
        groupBox_2->setMinimumSize(QSize(100, 80));
        groupBox_2->setMaximumSize(QSize(200, 16777215));
        gridLayout_9 = new QGridLayout(groupBox_2);
        gridLayout_9->setObjectName(QString::fromUtf8("gridLayout_9"));
        verticalSpacer_9 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_9->addItem(verticalSpacer_9, 4, 0, 1, 1);

        purpleairLocationOutdoor = new QRadioButton(groupBox_2);
        purpleairLocationOutdoor->setObjectName(QString::fromUtf8("purpleairLocationOutdoor"));
        sizePolicy1.setHeightForWidth(purpleairLocationOutdoor->sizePolicy().hasHeightForWidth());
        purpleairLocationOutdoor->setSizePolicy(sizePolicy1);

        gridLayout_9->addWidget(purpleairLocationOutdoor, 1, 0, 1, 1);

        purpleairLocationIndoor = new QRadioButton(groupBox_2);
        purpleairLocationIndoor->setObjectName(QString::fromUtf8("purpleairLocationIndoor"));
        sizePolicy1.setHeightForWidth(purpleairLocationIndoor->sizePolicy().hasHeightForWidth());
        purpleairLocationIndoor->setSizePolicy(sizePolicy1);

        gridLayout_9->addWidget(purpleairLocationIndoor, 2, 0, 1, 1);

        purpleairLocationIndoorOutdoor = new QRadioButton(groupBox_2);
        purpleairLocationIndoorOutdoor->setObjectName(QString::fromUtf8("purpleairLocationIndoorOutdoor"));
        sizePolicy1.setHeightForWidth(purpleairLocationIndoorOutdoor->sizePolicy().hasHeightForWidth());
        purpleairLocationIndoorOutdoor->setSizePolicy(sizePolicy1);

        gridLayout_9->addWidget(purpleairLocationIndoorOutdoor, 3, 0, 1, 1);

        horizontalSpacer_13 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_9->addItem(horizontalSpacer_13, 1, 1, 1, 1);

        verticalSpacer_13 = new QSpacerItem(20, 15, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_9->addItem(verticalSpacer_13, 0, 0, 1, 1);


        gridLayout_7->addWidget(groupBox_2, 1, 0, 1, 1);

        PurpleAirLabel_3 = new QLabel(groupBox_filterPurpleair);
        PurpleAirLabel_3->setObjectName(QString::fromUtf8("PurpleAirLabel_3"));

        gridLayout_7->addWidget(PurpleAirLabel_3, 2, 0, 1, 1);

        apikeyPurpleAirLabel = new QLabel(groupBox_filterPurpleair);
        apikeyPurpleAirLabel->setObjectName(QString::fromUtf8("apikeyPurpleAirLabel"));

        gridLayout_7->addWidget(apikeyPurpleAirLabel, 0, 0, 1, 1);

        usePurpleairHumidityReplacement = new QCheckBox(groupBox_filterPurpleair);
        usePurpleairHumidityReplacement->setObjectName(QString::fromUtf8("usePurpleairHumidityReplacement"));
        sizePolicy3.setHeightForWidth(usePurpleairHumidityReplacement->sizePolicy().hasHeightForWidth());
        usePurpleairHumidityReplacement->setSizePolicy(sizePolicy3);
        usePurpleairHumidityReplacement->setMinimumSize(QSize(0, 75));
        usePurpleairHumidityReplacement->setMaximumSize(QSize(16777215, 80));

        gridLayout_7->addWidget(usePurpleairHumidityReplacement, 3, 0, 1, 1);

        apikeyPurpleAir = new QLineEdit(groupBox_filterPurpleair);
        apikeyPurpleAir->setObjectName(QString::fromUtf8("apikeyPurpleAir"));
        sizePolicy.setHeightForWidth(apikeyPurpleAir->sizePolicy().hasHeightForWidth());
        apikeyPurpleAir->setSizePolicy(sizePolicy);
        apikeyPurpleAir->setMinimumSize(QSize(600, 0));
        apikeyPurpleAir->setMaximumSize(QSize(16777215, 40));
        apikeyPurpleAir->setMaxLength(40);

        gridLayout_7->addWidget(apikeyPurpleAir, 0, 1, 1, 3);

        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_7->addItem(horizontalSpacer_8, 3, 8, 1, 1);

        verticalSpacer_7 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_7->addItem(verticalSpacer_7, 8, 0, 1, 1);

        minPurpleairCountPercentage = new QSpinBox(groupBox_filterPurpleair);
        minPurpleairCountPercentage->setObjectName(QString::fromUtf8("minPurpleairCountPercentage"));
        sizePolicy1.setHeightForWidth(minPurpleairCountPercentage->sizePolicy().hasHeightForWidth());
        minPurpleairCountPercentage->setSizePolicy(sizePolicy1);
        minPurpleairCountPercentage->setMaximumSize(QSize(200, 16777215));
        minPurpleairCountPercentage->setMinimum(1);
        minPurpleairCountPercentage->setMaximum(100);
        minPurpleairCountPercentage->setValue(75);

        gridLayout_7->addWidget(minPurpleairCountPercentage, 2, 1, 1, 1);

        PurpleAirLabel_5 = new QLabel(groupBox_filterPurpleair);
        PurpleAirLabel_5->setObjectName(QString::fromUtf8("PurpleAirLabel_5"));

        gridLayout_7->addWidget(PurpleAirLabel_5, 4, 0, 1, 1);

        horizontalSpacer_10 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_7->addItem(horizontalSpacer_10, 5, 5, 1, 1);

        groupBox_4 = new QGroupBox(groupBox_filterPurpleair);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        sizePolicy.setHeightForWidth(groupBox_4->sizePolicy().hasHeightForWidth());
        groupBox_4->setSizePolicy(sizePolicy);
        horizontalLayout_3 = new QHBoxLayout(groupBox_4);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        acceptablePurpleairChannelAgreement = new QSpinBox(groupBox_4);
        acceptablePurpleairChannelAgreement->setObjectName(QString::fromUtf8("acceptablePurpleairChannelAgreement"));
        sizePolicy1.setHeightForWidth(acceptablePurpleairChannelAgreement->sizePolicy().hasHeightForWidth());
        acceptablePurpleairChannelAgreement->setSizePolicy(sizePolicy1);
        acceptablePurpleairChannelAgreement->setMaximumSize(QSize(200, 16777215));
        acceptablePurpleairChannelAgreement->setMinimum(1);
        acceptablePurpleairChannelAgreement->setValue(70);

        horizontalLayout_3->addWidget(acceptablePurpleairChannelAgreement);

        PurpleAirLabel_8 = new QLabel(groupBox_4);
        PurpleAirLabel_8->setObjectName(QString::fromUtf8("PurpleAirLabel_8"));

        horizontalLayout_3->addWidget(PurpleAirLabel_8);

        horizontalSpacer_12 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_12);


        gridLayout_7->addWidget(groupBox_4, 5, 1, 2, 1);

        PurpleAirLabel_7 = new QLabel(groupBox_filterPurpleair);
        PurpleAirLabel_7->setObjectName(QString::fromUtf8("PurpleAirLabel_7"));

        gridLayout_7->addWidget(PurpleAirLabel_7, 5, 0, 1, 1);

        horizontalSpacer_14 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_7->addItem(horizontalSpacer_14, 1, 1, 1, 1);

        tabWidget->addTab(purpleairTab, QString());
        TEMPOTab = new QWidget();
        TEMPOTab->setObjectName(QString::fromUtf8("TEMPOTab"));
        TEMPOTab->setMinimumSize(QSize(1800, 800));
        groupBox_filterTempo = new QGroupBox(TEMPOTab);
        groupBox_filterTempo->setObjectName(QString::fromUtf8("groupBox_filterTempo"));
        groupBox_filterTempo->setGeometry(QRect(4, 4, 1800, 700));
        sizePolicy.setHeightForWidth(groupBox_filterTempo->sizePolicy().hasHeightForWidth());
        groupBox_filterTempo->setSizePolicy(sizePolicy);
        groupBox_filterTempo->setMinimumSize(QSize(1800, 700));
        gridLayout_10 = new QGridLayout(groupBox_filterTempo);
        gridLayout_10->setObjectName(QString::fromUtf8("gridLayout_10"));
        TEMPOKey = new QLineEdit(groupBox_filterTempo);
        TEMPOKey->setObjectName(QString::fromUtf8("TEMPOKey"));
        QSizePolicy sizePolicy4(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(TEMPOKey->sizePolicy().hasHeightForWidth());
        TEMPOKey->setSizePolicy(sizePolicy4);
        TEMPOKey->setMinimumSize(QSize(600, 0));
        TEMPOKey->setMaximumSize(QSize(16777215, 60));
        TEMPOKey->setMaxLength(40);

        gridLayout_10->addWidget(TEMPOKey, 0, 1, 1, 1);

        TEMPOKeyLabel = new QLabel(groupBox_filterTempo);
        TEMPOKeyLabel->setObjectName(QString::fromUtf8("TEMPOKeyLabel"));

        gridLayout_10->addWidget(TEMPOKeyLabel, 0, 0, 1, 1);

        TEMPOQAflagMenu = new QComboBox(groupBox_filterTempo);
        TEMPOQAflagMenu->setObjectName(QString::fromUtf8("TEMPOQAflagMenu"));
        sizePolicy1.setHeightForWidth(TEMPOQAflagMenu->sizePolicy().hasHeightForWidth());
        TEMPOQAflagMenu->setSizePolicy(sizePolicy1);
        TEMPOQAflagMenu->setMaximumSize(QSize(120, 24));

        gridLayout_10->addWidget(TEMPOQAflagMenu, 1, 1, 1, 1);

        maxTEMPOCloudFractionSpinBox = new QDoubleSpinBox(groupBox_filterTempo);
        maxTEMPOCloudFractionSpinBox->setObjectName(QString::fromUtf8("maxTEMPOCloudFractionSpinBox"));
        maxTEMPOCloudFractionSpinBox->setMinimumSize(QSize(120, 24));
        maxTEMPOCloudFractionSpinBox->setMaximumSize(QSize(120, 24));
        maxTEMPOCloudFractionSpinBox->setDecimals(3);
        maxTEMPOCloudFractionSpinBox->setMaximum(1);
        maxTEMPOCloudFractionSpinBox->setSingleStep(0.1);
        maxTEMPOCloudFractionSpinBox->setValue(1);

        gridLayout_10->addWidget(maxTEMPOCloudFractionSpinBox, 2, 1, 1, 1);

        label_37 = new QLabel(groupBox_filterTempo);
        label_37->setObjectName(QString::fromUtf8("label_37"));

        gridLayout_10->addWidget(label_37, 2, 0, 1, 1);

        maxTEMPOSolarZenithAngleSpinBox = new QDoubleSpinBox(groupBox_filterTempo);
        maxTEMPOSolarZenithAngleSpinBox->setObjectName(QString::fromUtf8("maxTEMPOSolarZenithAngleSpinBox"));
        maxTEMPOSolarZenithAngleSpinBox->setMinimumSize(QSize(120, 24));
        maxTEMPOSolarZenithAngleSpinBox->setMaximumSize(QSize(120, 24));
        maxTEMPOSolarZenithAngleSpinBox->setDecimals(3);
        maxTEMPOSolarZenithAngleSpinBox->setMaximum(90);
        maxTEMPOSolarZenithAngleSpinBox->setSingleStep(1);
        maxTEMPOSolarZenithAngleSpinBox->setValue(90);

        gridLayout_10->addWidget(maxTEMPOSolarZenithAngleSpinBox, 3, 1, 1, 1);

        TEMPOAllowNegativeCounts = new QCheckBox(groupBox_filterTempo);
        TEMPOAllowNegativeCounts->setObjectName(QString::fromUtf8("TEMPOAllowNegativeCounts"));

        gridLayout_10->addWidget(TEMPOAllowNegativeCounts, 4, 0, 1, 1);

        TEMPO_label = new QLabel(groupBox_filterTempo);
        TEMPO_label->setObjectName(QString::fromUtf8("TEMPO_label"));

        gridLayout_10->addWidget(TEMPO_label, 1, 0, 1, 1);

        horizontalSpacer_15 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_10->addItem(horizontalSpacer_15, 1, 2, 1, 1);

        label_38 = new QLabel(groupBox_filterTempo);
        label_38->setObjectName(QString::fromUtf8("label_38"));

        gridLayout_10->addWidget(label_38, 3, 0, 1, 1);

        verticalSpacer_8 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_10->addItem(verticalSpacer_8, 5, 0, 1, 1);

        tabWidget->addTab(TEMPOTab, QString());
        OAQPSTab = new QWidget();
        OAQPSTab->setObjectName(QString::fromUtf8("OAQPSTab"));
        OAQPSTab->setMinimumSize(QSize(1800, 800));
        groupBox_filterOaqps = new QGroupBox(OAQPSTab);
        groupBox_filterOaqps->setObjectName(QString::fromUtf8("groupBox_filterOaqps"));
        groupBox_filterOaqps->setGeometry(QRect(8, 8, 993, 700));
        groupBox_filterOaqps->setMinimumSize(QSize(0, 700));
        groupBox_filterOaqps->setMaximumSize(QSize(1800, 16777215));
        gridLayout_12 = new QGridLayout(groupBox_filterOaqps);
        gridLayout_12->setObjectName(QString::fromUtf8("gridLayout_12"));
        OAQPSKey = new QLineEdit(groupBox_filterOaqps);
        OAQPSKey->setObjectName(QString::fromUtf8("OAQPSKey"));
        sizePolicy1.setHeightForWidth(OAQPSKey->sizePolicy().hasHeightForWidth());
        OAQPSKey->setSizePolicy(sizePolicy1);
        OAQPSKey->setMinimumSize(QSize(600, 0));
        OAQPSKey->setMaximumSize(QSize(16777215, 200));
        OAQPSKey->setMaxLength(40);

        gridLayout_12->addWidget(OAQPSKey, 0, 1, 1, 1);

        OAQPSKeyLabel = new QLabel(groupBox_filterOaqps);
        OAQPSKeyLabel->setObjectName(QString::fromUtf8("OAQPSKeyLabel"));

        gridLayout_12->addWidget(OAQPSKeyLabel, 0, 0, 1, 1);

        horizontalSpacer_17 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_12->addItem(horizontalSpacer_17, 0, 2, 1, 1);

        verticalSpacer_11 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_12->addItem(verticalSpacer_11, 1, 1, 1, 1);

        tabWidget->addTab(OAQPSTab, QString());
        HRRRTab = new QWidget();
        HRRRTab->setObjectName(QString::fromUtf8("HRRRTab"));
        HRRRTab->setMinimumSize(QSize(1800, 800));
        groupBox_filterHrrr = new QGroupBox(HRRRTab);
        groupBox_filterHrrr->setObjectName(QString::fromUtf8("groupBox_filterHrrr"));
        groupBox_filterHrrr->setGeometry(QRect(4, 8, 841, 700));
        sizePolicy.setHeightForWidth(groupBox_filterHrrr->sizePolicy().hasHeightForWidth());
        groupBox_filterHrrr->setSizePolicy(sizePolicy);
        groupBox_filterHrrr->setMinimumSize(QSize(0, 700));
        gridLayout_11 = new QGridLayout(groupBox_filterHrrr);
        gridLayout_11->setObjectName(QString::fromUtf8("gridLayout_11"));
        HRRRStrideSpinBoxLabel = new QLabel(groupBox_filterHrrr);
        HRRRStrideSpinBoxLabel->setObjectName(QString::fromUtf8("HRRRStrideSpinBoxLabel"));

        gridLayout_11->addWidget(HRRRStrideSpinBoxLabel, 0, 0, 1, 1);

        horizontalSpacer_16 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_11->addItem(horizontalSpacer_16, 0, 2, 1, 1);

        HRRRStrideSpinBox = new QSpinBox(groupBox_filterHrrr);
        HRRRStrideSpinBox->setObjectName(QString::fromUtf8("HRRRStrideSpinBox"));
        sizePolicy1.setHeightForWidth(HRRRStrideSpinBox->sizePolicy().hasHeightForWidth());
        HRRRStrideSpinBox->setSizePolicy(sizePolicy1);
        HRRRStrideSpinBox->setMinimum(1);
        HRRRStrideSpinBox->setMaximum(100);
        HRRRStrideSpinBox->setValue(1);

        gridLayout_11->addWidget(HRRRStrideSpinBox, 0, 1, 1, 1);

        verticalSpacer_10 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_11->addItem(verticalSpacer_10, 1, 0, 1, 1);

        tabWidget->addTab(HRRRTab, QString());

        verticalLayout->addWidget(tabWidget);

        groupBox_filterButtons = new QGroupBox(FilterSettingDialog);
        groupBox_filterButtons->setObjectName(QString::fromUtf8("groupBox_filterButtons"));
        sizePolicy.setHeightForWidth(groupBox_filterButtons->sizePolicy().hasHeightForWidth());
        groupBox_filterButtons->setSizePolicy(sizePolicy);
        groupBox_filterButtons->setMinimumSize(QSize(1800, 75));
        groupBox_filterButtons->setMaximumSize(QSize(16777215, 75));
        horizontalLayout = new QHBoxLayout(groupBox_filterButtons);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        resetPushButton = new QPushButton(groupBox_filterButtons);
        resetPushButton->setObjectName(QString::fromUtf8("resetPushButton"));
        sizePolicy1.setHeightForWidth(resetPushButton->sizePolicy().hasHeightForWidth());
        resetPushButton->setSizePolicy(sizePolicy1);
        resetPushButton->setMinimumSize(QSize(92, 24));
        resetPushButton->setMaximumSize(QSize(92, 24));
        resetPushButton->setAutoDefault(false);

        horizontalLayout->addWidget(resetPushButton);

        cancelPushButton = new QPushButton(groupBox_filterButtons);
        cancelPushButton->setObjectName(QString::fromUtf8("cancelPushButton"));
        sizePolicy1.setHeightForWidth(cancelPushButton->sizePolicy().hasHeightForWidth());
        cancelPushButton->setSizePolicy(sizePolicy1);
        cancelPushButton->setMinimumSize(QSize(92, 24));
        cancelPushButton->setMaximumSize(QSize(92, 24));
        cancelPushButton->setAutoDefault(false);

        horizontalLayout->addWidget(cancelPushButton);

        acceptPushButton = new QPushButton(groupBox_filterButtons);
        acceptPushButton->setObjectName(QString::fromUtf8("acceptPushButton"));
        sizePolicy1.setHeightForWidth(acceptPushButton->sizePolicy().hasHeightForWidth());
        acceptPushButton->setSizePolicy(sizePolicy1);
        acceptPushButton->setMinimumSize(QSize(92, 24));
        acceptPushButton->setMaximumSize(QSize(92, 24));
        acceptPushButton->setAutoDefault(false);
        acceptPushButton->setDefault(false);

        horizontalLayout->addWidget(acceptPushButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addWidget(groupBox_filterButtons);

        verticalSpacer_12 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_12);


        retranslateUi(FilterSettingDialog);

        tabWidget->setCurrentIndex(9);
        calipsoVersionL1Combobox->setCurrentIndex(0);
        calipsoVersionL2Combobox->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(FilterSettingDialog);
    } // setupUi

    void retranslateUi(QDialog *FilterSettingDialog)
    {
        FilterSettingDialog->setWindowTitle(QApplication::translate("FilterSettingDialog", "Data Filters", 0, QApplication::UnicodeUTF8));
        groupBox_filterGasp->setTitle(QString());
        label_26->setText(QApplication::translate("FilterSettingDialog", "<html><head/><body><p><span style=\" font-weight:600;\">max</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        label_25->setText(QApplication::translate("FilterSettingDialog", "<html><head/><body><p align=\"right\">AODSTD:</p></body></html>", 0, QApplication::UnicodeUTF8));
        label_18->setText(QApplication::translate("FilterSettingDialog", "<html><head/><body><p align=\"right\">AOD:</p></body></html>", 0, QApplication::UnicodeUTF8));
        label_19->setText(QApplication::translate("FilterSettingDialog", "<html><head/><body><p><span style=\" font-weight:600;\">min</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        label_21->setText(QApplication::translate("FilterSettingDialog", "<html><head/><body><p align=\"right\">MOS:</p></body></html>", 0, QApplication::UnicodeUTF8));
        label_28->setText(QApplication::translate("FilterSettingDialog", "<html><head/><body><p align=\"right\">SIG:</p></body></html>", 0, QApplication::UnicodeUTF8));
        label_17->setText(QApplication::translate("FilterSettingDialog", "<html><head/><body><p align=\"right\">CH1:</p></body></html>", 0, QApplication::UnicodeUTF8));
        label_23->setText(QApplication::translate("FilterSettingDialog", "<html><head/><body><p align=\"right\">CLS:</p></body></html>", 0, QApplication::UnicodeUTF8));
        label_20->setText(QApplication::translate("FilterSettingDialog", "<html><head/><body><p align=\"right\">SFC:</p></body></html>", 0, QApplication::UnicodeUTF8));
        label_27->setText(QApplication::translate("FilterSettingDialog", "<html><head/><body><p><span style=\" color:#313dd2;\">These filter settings are used to selectively filter out data that is outside each specified range. </span></p><p><span style=\" color:#313dd2;\">Hit 'Reset' to restore the filter settings to the default values.</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        label_24->setText(QApplication::translate("FilterSettingDialog", "<html><head/><body><p align=\"right\">SCA:</p></body></html>", 0, QApplication::UnicodeUTF8));
        label_22->setText(QApplication::translate("FilterSettingDialog", "<html><head/><body><p><span style=\" font-weight:600;\">GASP Filter Settings:</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(gaspTab), QApplication::translate("FilterSettingDialog", "GASP Filter", 0, QApplication::UnicodeUTF8));
        groupBox_filterCalipso->setTitle(QString());
        label_30->setText(QApplication::translate("FilterSettingDialog", "<html><head/><body><p><span style=\" font-weight:600;\">CALIPSO Data Filtering Options:</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        label_32->setText(QApplication::translate("FilterSettingDialog", "<html><head/><body><p>Minimum accepted |CAD Score| [20, 100]:</p></body></html>", 0, QApplication::UnicodeUTF8));
        label_29->setText(QApplication::translate("FilterSettingDialog", "<html><head/><body><p><span style=\" color:#0000ff;\">20 means least likely (but better than a 50/50 chance of correctly distinguished aerosol/cloud feature). </span></p><p><span style=\" color:#0000ff;\">100 means highest likelyhood of correctly distiinguished aerosol/cloud feature.</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        label_31->setText(QApplication::translate("FilterSettingDialog", "<html><head/><body><p>Maximum accepted uncertainty of measurement (in absolute units - not percentage) [0, 100]:</p></body></html>", 0, QApplication::UnicodeUTF8));
        label_35->setText(QApplication::translate("FilterSettingDialog", "<html><head/><body><p>L1 data product version:</p></body></html>", 0, QApplication::UnicodeUTF8));
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
        label_2->setText(QApplication::translate("FilterSettingDialog", "<html><head/><body><p><span style=\" color:#0000ff;\">L1 version 4.11 and L2 version 4.21 are<br>only available for July 1, 2020 or later.<br><br>\"Latest\" will always return the most up-to-date<br> product from the CALIPSO science team.</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        label_36->setText(QApplication::translate("FilterSettingDialog", "<html><head/><body><p>L2 data product version:</p></body></html>", 0, QApplication::UnicodeUTF8));
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
        tabWidget->setTabText(tabWidget->indexOf(calipsoTab), QApplication::translate("FilterSettingDialog", "CALIPSO Filter", 0, QApplication::UnicodeUTF8));
        groupBox_filterOmi->setTitle(QString());
        label_OMI2->setText(QApplication::translate("FilterSettingDialog", "<html><head/><body><p>Maximum solar zenith angle: [0.0 - 90.0]</p></body></html>", 0, QApplication::UnicodeUTF8));
        label_OMI1->setText(QApplication::translate("FilterSettingDialog", "<html><head/><body><p>Maximum cloud fraction: [0.0 - 1.0]</p></body></html>", 0, QApplication::UnicodeUTF8));
        OMIAllowNegativeCounts->setText(QApplication::translate("FilterSettingDialog", "Allow retrieval of negative molecules/cm2?", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(OMITab), QApplication::translate("FilterSettingDialog", "OMI Filter", 0, QApplication::UnicodeUTF8));
        groupBox_filterTropomi->setTitle(QString());
        label_34->setText(QApplication::translate("FilterSettingDialog", "<html><head/><body><p>Maximum cloud fraction: [0.0 - 1.0]</p></body></html>", 0, QApplication::UnicodeUTF8));
        TropomiGroundPixelMinimumLabel->setText(QApplication::translate("FilterSettingDialog", "<html><head/><body><p>Ground Pixel Minimum: [0-449]</p></body></html>", 0, QApplication::UnicodeUTF8));
        label_33->setText(QApplication::translate("FilterSettingDialog", "<html><head/><body><p>Minimum quality flag: [50-100]</p></body></html>", 0, QApplication::UnicodeUTF8));
        TropomiGroundPixelMaximumLabel->setText(QApplication::translate("FilterSettingDialog", "<html><head/><body><p>Ground Pixel Maximum: [0-449]</p></body></html>", 0, QApplication::UnicodeUTF8));
        TropomiAllowNegativeCounts->setText(QApplication::translate("FilterSettingDialog", "Allow retrieval of negative molecules/cm2?", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tropomiTab), QApplication::translate("FilterSettingDialog", "TROPOMI Filter", 0, QApplication::UnicodeUTF8));
        groupBox_filterViirs->setTitle(QString());
        viirsNoaaQAflagMenu->clear();
        viirsNoaaQAflagMenu->insertItems(0, QStringList()
         << QApplication::translate("FilterSettingDialog", "high", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("FilterSettingDialog", "medium", 0, QApplication::UnicodeUTF8)
        );
        viirs_label->setText(QApplication::translate("FilterSettingDialog", "Minimum quality flag", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(viirsTab), QApplication::translate("FilterSettingDialog", "VIIRS Filter", 0, QApplication::UnicodeUTF8));
        groupBox_filterPandora->setTitle(QString());
        pandoraQAflagMenu->clear();
        pandoraQAflagMenu->insertItems(0, QStringList()
         << QApplication::translate("FilterSettingDialog", "high", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("FilterSettingDialog", "medium", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("FilterSettingDialog", "low", 0, QApplication::UnicodeUTF8)
        );
        pandora_label->setText(QApplication::translate("FilterSettingDialog", "Minimum quality flag", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(pandoraTab), QApplication::translate("FilterSettingDialog", "Pandora Filter", 0, QApplication::UnicodeUTF8));
        groupBox_filterPurpleair->setTitle(QString());
        groupBox->setTitle(QString());
        PurpleAirLabel_6->setText(QApplication::translate("FilterSettingDialog", "ug/m3", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QString());
        PurpleAirLabel_4->setText(QApplication::translate("FilterSettingDialog", "Replacement value:", 0, QApplication::UnicodeUTF8));
        PurpleAirLabel_9->setText(QApplication::translate("FilterSettingDialog", "%", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("FilterSettingDialog", "Location", 0, QApplication::UnicodeUTF8));
        purpleairLocationOutdoor->setText(QApplication::translate("FilterSettingDialog", "Outdoor", 0, QApplication::UnicodeUTF8));
        purpleairLocationIndoor->setText(QApplication::translate("FilterSettingDialog", "Indoor", 0, QApplication::UnicodeUTF8));
        purpleairLocationIndoorOutdoor->setText(QApplication::translate("FilterSettingDialog", "Both", 0, QApplication::UnicodeUTF8));
        PurpleAirLabel_3->setText(QApplication::translate("FilterSettingDialog", "Minimum aggregation\n"
"count percentage:", 0, QApplication::UnicodeUTF8));
        apikeyPurpleAirLabel->setText(QApplication::translate("FilterSettingDialog", "PurpleAir API key:", 0, QApplication::UnicodeUTF8));
        usePurpleairHumidityReplacement->setText(QApplication::translate("FilterSettingDialog", "Use humidity replacement\n"
"in EPA PM2.5 correction\n"
"equation", 0, QApplication::UnicodeUTF8));
        PurpleAirLabel_5->setText(QApplication::translate("FilterSettingDialog", "Acceptable channel difference", 0, QApplication::UnicodeUTF8));
        groupBox_4->setTitle(QString());
        PurpleAirLabel_8->setText(QApplication::translate("FilterSettingDialog", "%", 0, QApplication::UnicodeUTF8));
        PurpleAirLabel_7->setText(QApplication::translate("FilterSettingDialog", "Acceptable channel agreement", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(purpleairTab), QApplication::translate("FilterSettingDialog", "PurpleAir Filter", 0, QApplication::UnicodeUTF8));
        groupBox_filterTempo->setTitle(QString());
        TEMPOKeyLabel->setText(QApplication::translate("FilterSettingDialog", "TEMPO Key:", 0, QApplication::UnicodeUTF8));
        TEMPOQAflagMenu->clear();
        TEMPOQAflagMenu->insertItems(0, QStringList()
         << QApplication::translate("FilterSettingDialog", "normal", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("FilterSettingDialog", "suspect", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("FilterSettingDialog", "bad", 0, QApplication::UnicodeUTF8)
        );
        label_37->setText(QApplication::translate("FilterSettingDialog", "<html><head/><body><p>Maximum cloud fraction: [0.0 - 1.0]</p></body></html>", 0, QApplication::UnicodeUTF8));
        TEMPOAllowNegativeCounts->setText(QApplication::translate("FilterSettingDialog", "Allow retrieval of negative molecules/cm2?", 0, QApplication::UnicodeUTF8));
        TEMPO_label->setText(QApplication::translate("FilterSettingDialog", "Minimum quality flag", 0, QApplication::UnicodeUTF8));
        label_38->setText(QApplication::translate("FilterSettingDialog", "<html><head/><body><p>Maximum solar zenith angle: [0.0 - 90.0]</p></body></html>", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(TEMPOTab), QApplication::translate("FilterSettingDialog", "TEMPO Filter", 0, QApplication::UnicodeUTF8));
        groupBox_filterOaqps->setTitle(QString());
        OAQPSKeyLabel->setText(QApplication::translate("FilterSettingDialog", "OAQPS Key:", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(OAQPSTab), QApplication::translate("FilterSettingDialog", "OAQPS Filter", 0, QApplication::UnicodeUTF8));
        groupBox_filterHrrr->setTitle(QString());
        HRRRStrideSpinBoxLabel->setText(QApplication::translate("FilterSettingDialog", "<html><head/><body><p>Row/Column Stride: [1-100]</p></body></html>", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(HRRRTab), QApplication::translate("FilterSettingDialog", "HRRR Filter", 0, QApplication::UnicodeUTF8));
        groupBox_filterButtons->setTitle(QString());
        resetPushButton->setText(QApplication::translate("FilterSettingDialog", "Reset", 0, QApplication::UnicodeUTF8));
        cancelPushButton->setText(QApplication::translate("FilterSettingDialog", "Cancel", 0, QApplication::UnicodeUTF8));
        acceptPushButton->setText(QApplication::translate("FilterSettingDialog", "Accept", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class FilterSettingDialog: public Ui_FilterSettingDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILTERSETTING_H
