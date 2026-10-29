/********************************************************************************
** Form generated from reading UI file 'controldockwidget.ui'
**
** Created: Thu Jul 23 13:53:07 2026
**      by: Qt User Interface Compiler version 4.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONTROLDOCKWIDGET_H
#define UI_CONTROLDOCKWIDGET_H

#include <QtCore/QDate>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDateEdit>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QFormLayout>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSlider>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QStackedWidget>
#include <QtGui/QTabWidget>
#include <QtGui/QTextBrowser>
#include <QtGui/QToolButton>
#include <QtGui/QTreeWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ControlUIForm
{
public:
    QWidget *layoutWidget;
    QVBoxLayout *mainVerticalLayout;
    QTabWidget *tabWidget;
    QWidget *mapTab;
    QGroupBox *groupBox_mapprojbound;
    QGridLayout *gridLayout;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *horizontalSpacer_4;
    QSpacerItem *horizontalSpacer_2;
    QGroupBox *boundariesGroupBox;
    QGridLayout *gridLayout_3;
    QRadioButton *maplineNone;
    QRadioButton *maplineStates_hires;
    QComboBox *maplineColorComboBox;
    QLabel *label_19;
    QRadioButton *maplineNations;
    QRadioButton *maplineCounties;
    QRadioButton *maplineStates;
    QSpacerItem *horizontalSpacer_15;
    QGroupBox *projectionsGroupBox;
    QGridLayout *gridLayout_2;
    QRadioButton *sphericalRadioButton;
    QRadioButton *lonlatRadioButton;
    QRadioButton *mercatorRadioButton;
    QRadioButton *stereographicRadioButton;
    QRadioButton *lambertconformalRadioButton;
    QSpacerItem *horizontalSpacer_14;
    QGroupBox *groupBox_mapserver;
    QHBoxLayout *horizontalLayout_2;
    QLabel *mapserverLabel;
    QComboBox *mapserverComboBox;
    QSpacerItem *horizontalSpacer_5;
    QGroupBox *groupBox_mapoptions;
    QGridLayout *gridLayout_6;
    QGroupBox *miscGroupBox;
    QGridLayout *gridLayout_4;
    QCheckBox *showScaleCheckBox;
    QCheckBox *showLonlatCheckBox;
    QCheckBox *showDataProbe;
    QCheckBox *showNavigatorCheckBox;
    QSpacerItem *horizontalSpacer_16;
    QSpacerItem *horizontalSpacer_12;
    QSpacerItem *horizontalSpacer_11;
    QSpacerItem *horizontalSpacer_13;
    QGroupBox *groupBox_mapother;
    QGridLayout *gridLayout_7;
    QSpacerItem *horizontalSpacer_22;
    QSpacerItem *horizontalSpacer_20;
    QSpacerItem *horizontalSpacer_21;
    QGroupBox *groupBox_20;
    QGridLayout *gridLayout_17;
    QCheckBox *chk_tooltips;
    QCheckBox *chkUseDataPrefs;
    QLabel *label_48;
    QSpinBox *guiFontsizeSpinbox;
    QGroupBox *groupBox_21;
    QGridLayout *gridLayout_16;
    QLabel *label_47;
    QCheckBox *chk_mapDoubleClickOption1;
    QCheckBox *chk_mapDoubleClickOption2;
    QSpacerItem *horizontalSpacer_23;
    QWidget *tab_2;
    QGroupBox *groupBox_datatab;
    QGroupBox *groupBox_scenario;
    QWidget *layoutWidget_2;
    QHBoxLayout *horizontalLayout;
    QLabel *scenarioLabel;
    QComboBox *scenarioComboBox;
    QLabel *timebaseLabel;
    QComboBox *timebaseComboBox;
    QSpacerItem *horizontalSpacer_3;
    QGroupBox *groupBox_vartree;
    QGridLayout *gridLayout_25;
    QTreeWidget *varTree;
    QPushButton *dataClearButton;
    QSpacerItem *horizontalSpacer_51;
    QGroupBox *groupBox_dataselections;
    QGridLayout *gridLayout_9;
    QPlainTextEdit *selectedData1;
    QSpacerItem *horizontalSpacer_24;
    QSpinBox *selectedLayer4;
    QSpinBox *selectedLayer1;
    QSpinBox *selectedLayer3;
    QSpinBox *selectedLayer5;
    QPlainTextEdit *selectedData5;
    QCheckBox *regridCheckbox2;
    QCheckBox *regridCheckbox3;
    QPlainTextEdit *selectedData2;
    QCheckBox *regridCheckbox4;
    QCheckBox *regridCheckbox5;
    QComboBox *aggregate2;
    QCheckBox *regridCheckbox1;
    QPlainTextEdit *selectedData3;
    QComboBox *trueColorComboBox;
    QLabel *label_8;
    QPlainTextEdit *selectedData4;
    QComboBox *aggregate5;
    QComboBox *aggregate3;
    QComboBox *aggregate1;
    QSpinBox *selectedLayer2;
    QSpacerItem *horizontalSpacer_9;
    QComboBox *aggregate4;
    QSpacerItem *horizontalSpacer_8;
    QSpacerItem *horizontalSpacer_18;
    QSpacerItem *horizontalSpacer_19;
    QLabel *label_selectedData;
    QLabel *label_aggregate;
    QLabel *label_regrid;
    QLabel *label_layer;
    QGroupBox *groupBox_datetime;
    QHBoxLayout *horizontalLayout_3;
    QGroupBox *dateGroupBox;
    QGridLayout *gridLayout_11;
    QLabel *dateLabel;
    QLabel *daysLabel;
    QSpinBox *durationSpinBox;
    QDateEdit *dateDateEdit;
    QSpacerItem *verticalSpacer_7;
    QGroupBox *boundingGroupBox;
    QGridLayout *gridLayout_12;
    QDoubleSpinBox *northDSpinBox;
    QDoubleSpinBox *westDSpinBox;
    QLabel *label_n;
    QDoubleSpinBox *southDSpinBox;
    QLabel *label_s;
    QLabel *label;
    QLabel *label_e;
    QGroupBox *groupBox_west;
    QDoubleSpinBox *eastDSpinBox;
    QLabel *label_w;
    QSpacerItem *verticalSpacer_8;
    QSpacerItem *horizontalSpacer_25;
    QGroupBox *groupBox_getdata;
    QHBoxLayout *horizontalLayout_4;
    QToolButton *filterToolButton;
    QToolButton *regridToolButton;
    QPushButton *checkServersPushButton;
    QSpacerItem *horizontalSpacer_26;
    QPushButton *getdataPushButton;
    QSpacerItem *horizontalSpacer_17;
    QWidget *propTab;
    QWidget *formLayoutWidget;
    QFormLayout *formLayout;
    QLabel *label_mode;
    QGroupBox *groupBox_mode;
    QHBoxLayout *horizontalLayout_5;
    QRadioButton *radioButton2D;
    QRadioButton *radioButton3D;
    QRadioButton *radioButtonPanelDisplay;
    QPushButton *autozoomView;
    QPushButton *resetView;
    QSpacerItem *horizontalSpacer_27;
    QComboBox *vizVarCombobox;
    QGroupBox *groupBox_visibility;
    QHBoxLayout *horizontalLayout_6;
    QCheckBox *vizCheckBox;
    QCheckBox *cellBoundaryCheckbox;
    QSpacerItem *horizontalSpacer_29;
    QLabel *label_opacity;
    QGroupBox *groupBox_opacity;
    QSlider *vizOpacitySlider;
    QLabel *opacity_display;
    QLabel *label_minmax;
    QGroupBox *groupBox_minmax;
    QHBoxLayout *horizontalLayout_7;
    QDoubleSpinBox *vizMinSpinbox;
    QDoubleSpinBox *vizMaxSpinbox;
    QCheckBox *vizDataDefault;
    QSpacerItem *horizontalSpacer_31;
    QLabel *label_threshold;
    QGroupBox *groupBox_threshold;
    QHBoxLayout *horizontalLayout_17;
    QCheckBox *thresholdCheckBox;
    QSlider *ThresholdSlider;
    QLabel *ThresholdLabel;
    QSpacerItem *horizontalSpacer_6;
    QLabel *label_colormap;
    QGroupBox *groupBox_colormap;
    QHBoxLayout *horizontalLayout_8;
    QComboBox *vizColormapCombobox;
    QSpacerItem *horizontalSpacer_32;
    QLabel *label_scalefactor;
    QGroupBox *groupBox_scalefactor;
    QHBoxLayout *horizontalLayout_9;
    QDoubleSpinBox *calipsoScaleFactor;
    QSpacerItem *horizontalSpacer_33;
    QLabel *label_ruler;
    QGroupBox *groupBox_ruler;
    QHBoxLayout *horizontalLayout_10;
    QCheckBox *rulerCheckBox;
    QSpacerItem *horizontalSpacer_35;
    QLabel *label_24;
    QDoubleSpinBox *rulerLonSpinBox;
    QLabel *label_25;
    QDoubleSpinBox *rulerLatSpinBox;
    QSpacerItem *horizontalSpacer_34;
    QLabel *WindLabel;
    QGroupBox *groupBox_windcontrols;
    QGridLayout *gridLayout_10;
    QRadioButton *windVectorArrowOption;
    QRadioButton *windVectorBarbOption;
    QDoubleSpinBox *windVectorScaleFactor;
    QDoubleSpinBox *windVectorTipSize;
    QLabel *windVectorScaleFactorLabel;
    QLabel *windVectorHeadSizeLabel;
    QCheckBox *autoSparseCheckBox;
    QDoubleSpinBox *windVectorSparseness;
    QLabel *windVectorSparsenessLabel;
    QSpacerItem *horizontalSpacer_36;
    QLabel *SpecialControlLabel;
    QGroupBox *groupBox_specialcontrols;
    QStackedWidget *specialControlsStackedWidget;
    QWidget *pageBLANK;
    QWidget *pageCMAQ;
    QGridLayout *gridLayout_13;
    QGroupBox *groupBox_cmaq;
    QGridLayout *gridLayout_15;
    QLabel *CMAQColLabel;
    QLabel *CMAQLayerLabel;
    QCheckBox *ShowCMAQLayer;
    QSlider *CMAQColSlider;
    QLabel *CMAQRowLabel;
    QSlider *CMAQRowSlider;
    QCheckBox *flattenCMAQ;
    QCheckBox *ShowCMAQCol;
    QCheckBox *ShowCMAQRow;
    QSlider *CMAQLayerSlider;
    QSpacerItem *verticalSpacer_10;
    QWidget *pageCEILOMETER;
    QGroupBox *groupBox_ceilometer;
    QGridLayout *gridLayout_14;
    QLabel *label_33;
    QSpacerItem *horizontalSpacer_37;
    QSlider *ceilometerSize;
    QCheckBox *ceilometerShowMixingLayer;
    QCheckBox *ceilometerShowLocations;
    QSlider *ceilometerPlotScroller;
    QDoubleSpinBox *ceilometerMinimumAltitude;
    QLabel *label_27;
    QLabel *label_28;
    QLabel *label_18;
    QDoubleSpinBox *ceilometerMaximumAltitude;
    QSpacerItem *verticalSpacer_16;
    QWidget *pageNEUBREW;
    QGroupBox *groupBox_neubrew;
    QGridLayout *gridLayout_18;
    QLabel *label_32;
    QCheckBox *neubrewShowLocations;
    QSlider *neubrewSize;
    QLabel *label_30;
    QSpacerItem *horizontalSpacer_38;
    QDoubleSpinBox *neubrewMinimumAltitude;
    QSlider *neubrewPlotScroller;
    QDoubleSpinBox *neubrewMaximumAltitude;
    QLabel *label_31;
    QLabel *label_29;
    QSpacerItem *verticalSpacer_12;
    QWidget *pageBEHR;
    QGroupBox *groupBox_behr;
    QGridLayout *gridLayout_29;
    QCheckBox *persistBEHR;
    QSpinBox *persistBEHRHours;
    QSpacerItem *horizontalSpacer_40;
    QSpacerItem *verticalSpacer_15;
    QWidget *pageMODIS;
    QGroupBox *groupBox_modis;
    QGridLayout *gridLayout_27;
    QCheckBox *persistMODIS;
    QSpinBox *persistMODISHours;
    QSpacerItem *horizontalSpacer_39;
    QSpacerItem *verticalSpacer_13;
    QWidget *pageVIIRS;
    QGroupBox *groupBox_viirs;
    QGridLayout *gridLayout_28;
    QCheckBox *persistVIIRS;
    QSpinBox *persistVIIRSHours;
    QSpacerItem *horizontalSpacer_41;
    QSpacerItem *verticalSpacer_14;
    QWidget *pageTROPOMI;
    QGroupBox *groupBox_tropomi;
    QGridLayout *gridLayout_5;
    QCheckBox *persistTROPOMI;
    QSpinBox *persistTROPOMIHours;
    QSpacerItem *horizontalSpacer_42;
    QSpacerItem *verticalSpacer_9;
    QWidget *pageHMS;
    QGroupBox *groupBox_hms;
    QGridLayout *gridLayout_26;
    QSpinBox *persistHMSHours;
    QSpacerItem *horizontalSpacer_43;
    QCheckBox *persistHMS;
    QSpacerItem *verticalSpacer_2;
    QWidget *pagePanel;
    QGroupBox *groupBox_panel;
    QGridLayout *gridLayout_19;
    QLabel *label_34;
    QLabel *label_21;
    QComboBox *panelDisplaySource2;
    QLabel *label_20;
    QComboBox *panelDisplaySource1;
    QListWidget *panelDisplayListWidget;
    QSpacerItem *horizontalSpacer_44;
    QSpacerItem *verticalSpacer_11;
    QWidget *pagePandora;
    QGroupBox *groupBox_pandora;
    QGridLayout *gridLayout_20;
    QSlider *pandoraPlotScroller;
    QDoubleSpinBox *pandoraMinimumAltitude;
    QLabel *label_38;
    QDoubleSpinBox *pandoraMaximumAltitude;
    QLabel *label_37;
    QLabel *label_35;
    QSpacerItem *horizontalSpacer_45;
    QLabel *label_36;
    QSlider *pandoraSize;
    QCheckBox *pandoraShowLocations;
    QWidget *pageTEMPO;
    QGroupBox *groupBox_tempo;
    QHBoxLayout *horizontalLayout_16;
    QCheckBox *persistTEMPO;
    QSpinBox *persistTEMPOHours;
    QSpacerItem *horizontalSpacer_46;
    QWidget *viewTab;
    QWidget *saveTab;
    QGroupBox *groupBox_savetab;
    QVBoxLayout *verticalLayout_2;
    QGroupBox *groupBox_saveformats;
    QGridLayout *gridLayout_21;
    QLabel *label_5;
    QSpacerItem *horizontalSpacer_49;
    QRadioButton *netcdfioapiRadioButton;
    QRadioButton *asciisheetRadioButton;
    QRadioButton *scenarioRadioButton;
    QRadioButton *originalFileRadioButton;
    QRadioButton *mpegRadioButton;
    QRadioButton *shapeRadioButton;
    QRadioButton *geotiffRadioButton;
    QRadioButton *allpngsRadioButton;
    QRadioButton *highrespngRadioButton;
    QRadioButton *binaryxdrRadioButton;
    QRadioButton *currentpngRadioButton;
    QLabel *label_6;
    QRadioButton *netcdfcoardsRadioButton;
    QRadioButton *kmlRadioButton;
    QRadioButton *screenRecordRadioButton;
    QSpacerItem *verticalSpacer_3;
    QRadioButton *geotiffrgbRadioButton;
    QGroupBox *groupBox_saveoptions;
    QGridLayout *gridLayout_23;
    QCheckBox *gzipCheckBox;
    QLabel *label_7;
    QCheckBox *saveMetadataCheckBox;
    QSpacerItem *horizontalSpacer_48;
    QGroupBox *groupBox_savefile;
    QGridLayout *gridLayout_22;
    QToolButton *writetofileToolButton;
    QLabel *label_26;
    QSpacerItem *horizontalSpacer_47;
    QLabel *filename_generated;
    QLineEdit *filenameLineEdit;
    QLineEdit *dirnameLineEdit;
    QPushButton *savevideoPushButton;
    QPushButton *savefilePushButton;
    QSpacerItem *verticalSpacer_5;
    QSpacerItem *verticalSpacer_4;
    QWidget *tab;
    QGroupBox *groupBox_rsigweb;
    QGridLayout *gridLayout_24;
    QSpacerItem *verticalSpacer_6;
    QPushButton *tipsPushButton;
    QPushButton *datausePushButton;
    QPushButton *openwebPushButton;
    QPushButton *emailPushButton;
    QPushButton *downloaderPushButton;
    QPushButton *exitPushButton;
    QLabel *label_3;
    QTextBrowser *rsigTextBrowser;
    QSpacerItem *horizontalSpacer_50;
    QGroupBox *groupBox_2;
    QPushButton *checknowPushButton;
    QGroupBox *groupBox_lowerControl;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox_timestep;
    QGridLayout *gridLayout_8;
    QToolButton *backButton24;
    QToolButton *backButton;
    QLabel *label_9;
    QPushButton *playButton;
    QToolButton *forwardButton;
    QToolButton *forwardButton24;
    QSlider *timestepSlider;
    QLabel *label_timestep;
    QSpinBox *animationSpeed;
    QTextBrowser *textBrowser;
    QToolButton *clearlogToolButton;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *ControlUIForm)
    {
        if (ControlUIForm->objectName().isEmpty())
            ControlUIForm->setObjectName(QString::fromUtf8("ControlUIForm"));
        ControlUIForm->setEnabled(true);
        ControlUIForm->resize(1067, 1037);
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ControlUIForm->sizePolicy().hasHeightForWidth());
        ControlUIForm->setSizePolicy(sizePolicy);
        ControlUIForm->setMinimumSize(QSize(200, 900));
        ControlUIForm->setAutoFillBackground(false);
        layoutWidget = new QWidget(ControlUIForm);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(9, 9, 726, 1018));
        mainVerticalLayout = new QVBoxLayout(layoutWidget);
        mainVerticalLayout->setSpacing(0);
        mainVerticalLayout->setObjectName(QString::fromUtf8("mainVerticalLayout"));
        mainVerticalLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        mainVerticalLayout->setContentsMargins(0, 0, 0, 0);
        tabWidget = new QTabWidget(layoutWidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setEnabled(true);
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(5);
        sizePolicy1.setHeightForWidth(tabWidget->sizePolicy().hasHeightForWidth());
        tabWidget->setSizePolicy(sizePolicy1);
        tabWidget->setMinimumSize(QSize(0, 675));
        tabWidget->setMaximumSize(QSize(16777215, 16777215));
        tabWidget->setSizeIncrement(QSize(4, 4));
        tabWidget->setStyleSheet(QString::fromUtf8("QTabBar::tab:selected { color: #0000FF; background-color:#CCCCCC}"));
        tabWidget->setTabShape(QTabWidget::Rounded);
        tabWidget->setElideMode(Qt::ElideNone);
        tabWidget->setUsesScrollButtons(true);
        mapTab = new QWidget();
        mapTab->setObjectName(QString::fromUtf8("mapTab"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(mapTab->sizePolicy().hasHeightForWidth());
        mapTab->setSizePolicy(sizePolicy2);
        mapTab->setStyleSheet(QString::fromUtf8("QTabBar::tab:selected { color: #ffff00; }"));
        groupBox_mapprojbound = new QGroupBox(mapTab);
        groupBox_mapprojbound->setObjectName(QString::fromUtf8("groupBox_mapprojbound"));
        groupBox_mapprojbound->setGeometry(QRect(0, 48, 800, 225));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(groupBox_mapprojbound->sizePolicy().hasHeightForWidth());
        groupBox_mapprojbound->setSizePolicy(sizePolicy3);
        groupBox_mapprojbound->setStyleSheet(QString::fromUtf8(""));
        groupBox_mapprojbound->setFlat(false);
        gridLayout = new QGridLayout(groupBox_mapprojbound);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setSizeConstraint(QLayout::SetNoConstraint);
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 4, 1, 1);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_4, 0, 2, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 0, 3, 1, 1);

        boundariesGroupBox = new QGroupBox(groupBox_mapprojbound);
        boundariesGroupBox->setObjectName(QString::fromUtf8("boundariesGroupBox"));
        sizePolicy.setHeightForWidth(boundariesGroupBox->sizePolicy().hasHeightForWidth());
        boundariesGroupBox->setSizePolicy(sizePolicy);
        boundariesGroupBox->setStyleSheet(QString::fromUtf8("margin-top:0"));
        boundariesGroupBox->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        boundariesGroupBox->setCheckable(false);
        boundariesGroupBox->setChecked(false);
        gridLayout_3 = new QGridLayout(boundariesGroupBox);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        maplineNone = new QRadioButton(boundariesGroupBox);
        maplineNone->setObjectName(QString::fromUtf8("maplineNone"));
        sizePolicy.setHeightForWidth(maplineNone->sizePolicy().hasHeightForWidth());
        maplineNone->setSizePolicy(sizePolicy);
        maplineNone->setMinimumSize(QSize(0, 30));
        maplineNone->setStyleSheet(QString::fromUtf8(""));

        gridLayout_3->addWidget(maplineNone, 1, 0, 1, 2);

        maplineStates_hires = new QRadioButton(boundariesGroupBox);
        maplineStates_hires->setObjectName(QString::fromUtf8("maplineStates_hires"));
        sizePolicy.setHeightForWidth(maplineStates_hires->sizePolicy().hasHeightForWidth());
        maplineStates_hires->setSizePolicy(sizePolicy);
        maplineStates_hires->setMinimumSize(QSize(0, 30));

        gridLayout_3->addWidget(maplineStates_hires, 4, 0, 1, 2);

        maplineColorComboBox = new QComboBox(boundariesGroupBox);
        maplineColorComboBox->setObjectName(QString::fromUtf8("maplineColorComboBox"));
        QSizePolicy sizePolicy4(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(maplineColorComboBox->sizePolicy().hasHeightForWidth());
        maplineColorComboBox->setSizePolicy(sizePolicy4);
        maplineColorComboBox->setMinimumSize(QSize(0, 30));

        gridLayout_3->addWidget(maplineColorComboBox, 6, 1, 1, 1);

        label_19 = new QLabel(boundariesGroupBox);
        label_19->setObjectName(QString::fromUtf8("label_19"));
        sizePolicy.setHeightForWidth(label_19->sizePolicy().hasHeightForWidth());
        label_19->setSizePolicy(sizePolicy);
        label_19->setMinimumSize(QSize(0, 30));

        gridLayout_3->addWidget(label_19, 6, 0, 1, 1);

        maplineNations = new QRadioButton(boundariesGroupBox);
        maplineNations->setObjectName(QString::fromUtf8("maplineNations"));
        sizePolicy.setHeightForWidth(maplineNations->sizePolicy().hasHeightForWidth());
        maplineNations->setSizePolicy(sizePolicy);
        maplineNations->setMinimumSize(QSize(0, 30));

        gridLayout_3->addWidget(maplineNations, 2, 0, 1, 2);

        maplineCounties = new QRadioButton(boundariesGroupBox);
        maplineCounties->setObjectName(QString::fromUtf8("maplineCounties"));
        sizePolicy.setHeightForWidth(maplineCounties->sizePolicy().hasHeightForWidth());
        maplineCounties->setSizePolicy(sizePolicy);
        maplineCounties->setMinimumSize(QSize(0, 30));

        gridLayout_3->addWidget(maplineCounties, 5, 0, 1, 2);

        maplineStates = new QRadioButton(boundariesGroupBox);
        maplineStates->setObjectName(QString::fromUtf8("maplineStates"));
        sizePolicy.setHeightForWidth(maplineStates->sizePolicy().hasHeightForWidth());
        maplineStates->setSizePolicy(sizePolicy);
        maplineStates->setMinimumSize(QSize(0, 30));

        gridLayout_3->addWidget(maplineStates, 3, 0, 1, 2);

        horizontalSpacer_15 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_15, 0, 0, 1, 1);


        gridLayout->addWidget(boundariesGroupBox, 0, 1, 1, 1);

        projectionsGroupBox = new QGroupBox(groupBox_mapprojbound);
        projectionsGroupBox->setObjectName(QString::fromUtf8("projectionsGroupBox"));
        sizePolicy.setHeightForWidth(projectionsGroupBox->sizePolicy().hasHeightForWidth());
        projectionsGroupBox->setSizePolicy(sizePolicy);
        projectionsGroupBox->setStyleSheet(QString::fromUtf8("margin-top:0"));
        projectionsGroupBox->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        projectionsGroupBox->setFlat(false);
        gridLayout_2 = new QGridLayout(projectionsGroupBox);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        sphericalRadioButton = new QRadioButton(projectionsGroupBox);
        sphericalRadioButton->setObjectName(QString::fromUtf8("sphericalRadioButton"));
        sizePolicy.setHeightForWidth(sphericalRadioButton->sizePolicy().hasHeightForWidth());
        sphericalRadioButton->setSizePolicy(sizePolicy);
        sphericalRadioButton->setMinimumSize(QSize(0, 20));

        gridLayout_2->addWidget(sphericalRadioButton, 2, 0, 1, 1);

        lonlatRadioButton = new QRadioButton(projectionsGroupBox);
        lonlatRadioButton->setObjectName(QString::fromUtf8("lonlatRadioButton"));
        sizePolicy.setHeightForWidth(lonlatRadioButton->sizePolicy().hasHeightForWidth());
        lonlatRadioButton->setSizePolicy(sizePolicy);
        lonlatRadioButton->setMinimumSize(QSize(0, 20));
        lonlatRadioButton->setStyleSheet(QString::fromUtf8(""));

        gridLayout_2->addWidget(lonlatRadioButton, 1, 0, 1, 1);

        mercatorRadioButton = new QRadioButton(projectionsGroupBox);
        mercatorRadioButton->setObjectName(QString::fromUtf8("mercatorRadioButton"));
        sizePolicy.setHeightForWidth(mercatorRadioButton->sizePolicy().hasHeightForWidth());
        mercatorRadioButton->setSizePolicy(sizePolicy);
        mercatorRadioButton->setMinimumSize(QSize(0, 30));

        gridLayout_2->addWidget(mercatorRadioButton, 3, 0, 1, 1);

        stereographicRadioButton = new QRadioButton(projectionsGroupBox);
        stereographicRadioButton->setObjectName(QString::fromUtf8("stereographicRadioButton"));
        sizePolicy.setHeightForWidth(stereographicRadioButton->sizePolicy().hasHeightForWidth());
        stereographicRadioButton->setSizePolicy(sizePolicy);
        stereographicRadioButton->setMinimumSize(QSize(0, 30));

        gridLayout_2->addWidget(stereographicRadioButton, 5, 0, 1, 1);

        lambertconformalRadioButton = new QRadioButton(projectionsGroupBox);
        lambertconformalRadioButton->setObjectName(QString::fromUtf8("lambertconformalRadioButton"));
        sizePolicy.setHeightForWidth(lambertconformalRadioButton->sizePolicy().hasHeightForWidth());
        lambertconformalRadioButton->setSizePolicy(sizePolicy);
        lambertconformalRadioButton->setMinimumSize(QSize(0, 30));

        gridLayout_2->addWidget(lambertconformalRadioButton, 4, 0, 1, 1);

        horizontalSpacer_14 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_14, 0, 0, 1, 1);


        gridLayout->addWidget(projectionsGroupBox, 0, 0, 1, 1);

        groupBox_mapserver = new QGroupBox(mapTab);
        groupBox_mapserver->setObjectName(QString::fromUtf8("groupBox_mapserver"));
        groupBox_mapserver->setGeometry(QRect(0, 4, 709, 50));
        QSizePolicy sizePolicy5(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(groupBox_mapserver->sizePolicy().hasHeightForWidth());
        groupBox_mapserver->setSizePolicy(sizePolicy5);
        horizontalLayout_2 = new QHBoxLayout(groupBox_mapserver);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        mapserverLabel = new QLabel(groupBox_mapserver);
        mapserverLabel->setObjectName(QString::fromUtf8("mapserverLabel"));
        sizePolicy5.setHeightForWidth(mapserverLabel->sizePolicy().hasHeightForWidth());
        mapserverLabel->setSizePolicy(sizePolicy5);
        mapserverLabel->setMinimumSize(QSize(100, 35));
        mapserverLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

        horizontalLayout_2->addWidget(mapserverLabel);

        mapserverComboBox = new QComboBox(groupBox_mapserver);
        mapserverComboBox->setObjectName(QString::fromUtf8("mapserverComboBox"));
        sizePolicy5.setHeightForWidth(mapserverComboBox->sizePolicy().hasHeightForWidth());
        mapserverComboBox->setSizePolicy(sizePolicy5);
        mapserverComboBox->setMinimumSize(QSize(40, 30));
        mapserverComboBox->setMaximumSize(QSize(16777215, 24));

        horizontalLayout_2->addWidget(mapserverComboBox);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_5);

        groupBox_mapoptions = new QGroupBox(mapTab);
        groupBox_mapoptions->setObjectName(QString::fromUtf8("groupBox_mapoptions"));
        groupBox_mapoptions->setGeometry(QRect(0, 264, 800, 177));
        sizePolicy.setHeightForWidth(groupBox_mapoptions->sizePolicy().hasHeightForWidth());
        groupBox_mapoptions->setSizePolicy(sizePolicy);
        gridLayout_6 = new QGridLayout(groupBox_mapoptions);
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        miscGroupBox = new QGroupBox(groupBox_mapoptions);
        miscGroupBox->setObjectName(QString::fromUtf8("miscGroupBox"));
        sizePolicy.setHeightForWidth(miscGroupBox->sizePolicy().hasHeightForWidth());
        miscGroupBox->setSizePolicy(sizePolicy);
        miscGroupBox->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        miscGroupBox->setFlat(false);
        gridLayout_4 = new QGridLayout(miscGroupBox);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        showScaleCheckBox = new QCheckBox(miscGroupBox);
        showScaleCheckBox->setObjectName(QString::fromUtf8("showScaleCheckBox"));
        sizePolicy4.setHeightForWidth(showScaleCheckBox->sizePolicy().hasHeightForWidth());
        showScaleCheckBox->setSizePolicy(sizePolicy4);
        showScaleCheckBox->setChecked(true);

        gridLayout_4->addWidget(showScaleCheckBox, 2, 0, 1, 1);

        showLonlatCheckBox = new QCheckBox(miscGroupBox);
        showLonlatCheckBox->setObjectName(QString::fromUtf8("showLonlatCheckBox"));
        sizePolicy4.setHeightForWidth(showLonlatCheckBox->sizePolicy().hasHeightForWidth());
        showLonlatCheckBox->setSizePolicy(sizePolicy4);
        showLonlatCheckBox->setStyleSheet(QString::fromUtf8(""));
        showLonlatCheckBox->setChecked(true);

        gridLayout_4->addWidget(showLonlatCheckBox, 1, 0, 1, 1);

        showDataProbe = new QCheckBox(miscGroupBox);
        showDataProbe->setObjectName(QString::fromUtf8("showDataProbe"));
        sizePolicy4.setHeightForWidth(showDataProbe->sizePolicy().hasHeightForWidth());
        showDataProbe->setSizePolicy(sizePolicy4);

        gridLayout_4->addWidget(showDataProbe, 4, 0, 1, 1);

        showNavigatorCheckBox = new QCheckBox(miscGroupBox);
        showNavigatorCheckBox->setObjectName(QString::fromUtf8("showNavigatorCheckBox"));
        sizePolicy4.setHeightForWidth(showNavigatorCheckBox->sizePolicy().hasHeightForWidth());
        showNavigatorCheckBox->setSizePolicy(sizePolicy4);

        gridLayout_4->addWidget(showNavigatorCheckBox, 3, 0, 1, 1);

        horizontalSpacer_16 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_4->addItem(horizontalSpacer_16, 0, 0, 1, 1);


        gridLayout_6->addWidget(miscGroupBox, 0, 0, 1, 1);

        horizontalSpacer_12 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_6->addItem(horizontalSpacer_12, 0, 2, 1, 1);

        horizontalSpacer_11 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_6->addItem(horizontalSpacer_11, 0, 3, 1, 1);

        horizontalSpacer_13 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_6->addItem(horizontalSpacer_13, 0, 1, 1, 1);

        groupBox_mapother = new QGroupBox(mapTab);
        groupBox_mapother->setObjectName(QString::fromUtf8("groupBox_mapother"));
        groupBox_mapother->setGeometry(QRect(0, 432, 800, 133));
        sizePolicy.setHeightForWidth(groupBox_mapother->sizePolicy().hasHeightForWidth());
        groupBox_mapother->setSizePolicy(sizePolicy);
        gridLayout_7 = new QGridLayout(groupBox_mapother);
        gridLayout_7->setObjectName(QString::fromUtf8("gridLayout_7"));
        horizontalSpacer_22 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_7->addItem(horizontalSpacer_22, 0, 4, 1, 1);

        horizontalSpacer_20 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_7->addItem(horizontalSpacer_20, 0, 2, 1, 1);

        horizontalSpacer_21 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_7->addItem(horizontalSpacer_21, 0, 5, 1, 1);

        groupBox_20 = new QGroupBox(groupBox_mapother);
        groupBox_20->setObjectName(QString::fromUtf8("groupBox_20"));
        sizePolicy3.setHeightForWidth(groupBox_20->sizePolicy().hasHeightForWidth());
        groupBox_20->setSizePolicy(sizePolicy3);
        gridLayout_17 = new QGridLayout(groupBox_20);
        gridLayout_17->setObjectName(QString::fromUtf8("gridLayout_17"));
        chk_tooltips = new QCheckBox(groupBox_20);
        chk_tooltips->setObjectName(QString::fromUtf8("chk_tooltips"));
        sizePolicy.setHeightForWidth(chk_tooltips->sizePolicy().hasHeightForWidth());
        chk_tooltips->setSizePolicy(sizePolicy);
        chk_tooltips->setChecked(true);

        gridLayout_17->addWidget(chk_tooltips, 0, 0, 1, 2);

        chkUseDataPrefs = new QCheckBox(groupBox_20);
        chkUseDataPrefs->setObjectName(QString::fromUtf8("chkUseDataPrefs"));
        sizePolicy.setHeightForWidth(chkUseDataPrefs->sizePolicy().hasHeightForWidth());
        chkUseDataPrefs->setSizePolicy(sizePolicy);
        chkUseDataPrefs->setChecked(false);

        gridLayout_17->addWidget(chkUseDataPrefs, 1, 0, 1, 2);

        label_48 = new QLabel(groupBox_20);
        label_48->setObjectName(QString::fromUtf8("label_48"));
        sizePolicy.setHeightForWidth(label_48->sizePolicy().hasHeightForWidth());
        label_48->setSizePolicy(sizePolicy);

        gridLayout_17->addWidget(label_48, 2, 0, 1, 1);

        guiFontsizeSpinbox = new QSpinBox(groupBox_20);
        guiFontsizeSpinbox->setObjectName(QString::fromUtf8("guiFontsizeSpinbox"));
        sizePolicy4.setHeightForWidth(guiFontsizeSpinbox->sizePolicy().hasHeightForWidth());
        guiFontsizeSpinbox->setSizePolicy(sizePolicy4);
        guiFontsizeSpinbox->setMinimumSize(QSize(0, 30));
        guiFontsizeSpinbox->setMinimum(2);
        guiFontsizeSpinbox->setMaximum(18);

        gridLayout_17->addWidget(guiFontsizeSpinbox, 2, 1, 1, 1);


        gridLayout_7->addWidget(groupBox_20, 0, 0, 1, 1);

        groupBox_21 = new QGroupBox(groupBox_mapother);
        groupBox_21->setObjectName(QString::fromUtf8("groupBox_21"));
        sizePolicy3.setHeightForWidth(groupBox_21->sizePolicy().hasHeightForWidth());
        groupBox_21->setSizePolicy(sizePolicy3);
        gridLayout_16 = new QGridLayout(groupBox_21);
        gridLayout_16->setObjectName(QString::fromUtf8("gridLayout_16"));
        label_47 = new QLabel(groupBox_21);
        label_47->setObjectName(QString::fromUtf8("label_47"));
        sizePolicy.setHeightForWidth(label_47->sizePolicy().hasHeightForWidth());
        label_47->setSizePolicy(sizePolicy);

        gridLayout_16->addWidget(label_47, 0, 0, 1, 1);

        chk_mapDoubleClickOption1 = new QCheckBox(groupBox_21);
        chk_mapDoubleClickOption1->setObjectName(QString::fromUtf8("chk_mapDoubleClickOption1"));
        sizePolicy.setHeightForWidth(chk_mapDoubleClickOption1->sizePolicy().hasHeightForWidth());
        chk_mapDoubleClickOption1->setSizePolicy(sizePolicy);

        gridLayout_16->addWidget(chk_mapDoubleClickOption1, 1, 0, 1, 1);

        chk_mapDoubleClickOption2 = new QCheckBox(groupBox_21);
        chk_mapDoubleClickOption2->setObjectName(QString::fromUtf8("chk_mapDoubleClickOption2"));
        sizePolicy.setHeightForWidth(chk_mapDoubleClickOption2->sizePolicy().hasHeightForWidth());
        chk_mapDoubleClickOption2->setSizePolicy(sizePolicy);

        gridLayout_16->addWidget(chk_mapDoubleClickOption2, 2, 0, 1, 1);


        gridLayout_7->addWidget(groupBox_21, 0, 1, 1, 1);

        horizontalSpacer_23 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_7->addItem(horizontalSpacer_23, 0, 3, 1, 1);

        tabWidget->addTab(mapTab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        sizePolicy3.setHeightForWidth(tab_2->sizePolicy().hasHeightForWidth());
        tab_2->setSizePolicy(sizePolicy3);
        groupBox_datatab = new QGroupBox(tab_2);
        groupBox_datatab->setObjectName(QString::fromUtf8("groupBox_datatab"));
        groupBox_datatab->setGeometry(QRect(0, 4, 621, 800));
        sizePolicy.setHeightForWidth(groupBox_datatab->sizePolicy().hasHeightForWidth());
        groupBox_datatab->setSizePolicy(sizePolicy);
        groupBox_datatab->setMinimumSize(QSize(500, 500));
        groupBox_scenario = new QGroupBox(groupBox_datatab);
        groupBox_scenario->setObjectName(QString::fromUtf8("groupBox_scenario"));
        groupBox_scenario->setGeometry(QRect(10, 3, 780, 50));
        sizePolicy.setHeightForWidth(groupBox_scenario->sizePolicy().hasHeightForWidth());
        groupBox_scenario->setSizePolicy(sizePolicy);
        groupBox_scenario->setMaximumSize(QSize(16777215, 50));
        layoutWidget_2 = new QWidget(groupBox_scenario);
        layoutWidget_2->setObjectName(QString::fromUtf8("layoutWidget_2"));
        layoutWidget_2->setGeometry(QRect(0, 0, 701, 49));
        horizontalLayout = new QHBoxLayout(layoutWidget_2);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(2, 0, 0, 0);
        scenarioLabel = new QLabel(layoutWidget_2);
        scenarioLabel->setObjectName(QString::fromUtf8("scenarioLabel"));
        sizePolicy5.setHeightForWidth(scenarioLabel->sizePolicy().hasHeightForWidth());
        scenarioLabel->setSizePolicy(sizePolicy5);
        scenarioLabel->setMaximumSize(QSize(16777215, 35));

        horizontalLayout->addWidget(scenarioLabel);

        scenarioComboBox = new QComboBox(layoutWidget_2);
        scenarioComboBox->setObjectName(QString::fromUtf8("scenarioComboBox"));
        sizePolicy5.setHeightForWidth(scenarioComboBox->sizePolicy().hasHeightForWidth());
        scenarioComboBox->setSizePolicy(sizePolicy5);
        scenarioComboBox->setMinimumSize(QSize(120, 24));
        scenarioComboBox->setMaximumSize(QSize(16777215, 35));

        horizontalLayout->addWidget(scenarioComboBox);

        timebaseLabel = new QLabel(layoutWidget_2);
        timebaseLabel->setObjectName(QString::fromUtf8("timebaseLabel"));
        sizePolicy5.setHeightForWidth(timebaseLabel->sizePolicy().hasHeightForWidth());
        timebaseLabel->setSizePolicy(sizePolicy5);
        timebaseLabel->setMaximumSize(QSize(16777215, 35));

        horizontalLayout->addWidget(timebaseLabel);

        timebaseComboBox = new QComboBox(layoutWidget_2);
        timebaseComboBox->setObjectName(QString::fromUtf8("timebaseComboBox"));
        sizePolicy5.setHeightForWidth(timebaseComboBox->sizePolicy().hasHeightForWidth());
        timebaseComboBox->setSizePolicy(sizePolicy5);
        timebaseComboBox->setMinimumSize(QSize(100, 24));
        timebaseComboBox->setMaximumSize(QSize(16777215, 35));

        horizontalLayout->addWidget(timebaseComboBox);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);

        groupBox_vartree = new QGroupBox(groupBox_datatab);
        groupBox_vartree->setObjectName(QString::fromUtf8("groupBox_vartree"));
        groupBox_vartree->setGeometry(QRect(10, 59, 780, 180));
        sizePolicy.setHeightForWidth(groupBox_vartree->sizePolicy().hasHeightForWidth());
        groupBox_vartree->setSizePolicy(sizePolicy);
        groupBox_vartree->setMaximumSize(QSize(16777215, 180));
        gridLayout_25 = new QGridLayout(groupBox_vartree);
        gridLayout_25->setContentsMargins(2, 2, 2, 2);
        gridLayout_25->setObjectName(QString::fromUtf8("gridLayout_25"));
        gridLayout_25->setVerticalSpacing(2);
        varTree = new QTreeWidget(groupBox_vartree);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QString::fromUtf8("1"));
        varTree->setHeaderItem(__qtreewidgetitem);
        varTree->setObjectName(QString::fromUtf8("varTree"));
        sizePolicy2.setHeightForWidth(varTree->sizePolicy().hasHeightForWidth());
        varTree->setSizePolicy(sizePolicy2);
        varTree->setMinimumSize(QSize(500, 122));
        varTree->setMaximumSize(QSize(1000, 600));
        QFont font;
        font.setPointSize(8);
        varTree->setFont(font);
        varTree->setStyleSheet(QString::fromUtf8("background-color:#FFFFFF"));
        varTree->header()->setVisible(false);

        gridLayout_25->addWidget(varTree, 1, 0, 1, 1);

        dataClearButton = new QPushButton(groupBox_vartree);
        dataClearButton->setObjectName(QString::fromUtf8("dataClearButton"));
        QSizePolicy sizePolicy6(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy6.setHorizontalStretch(0);
        sizePolicy6.setVerticalStretch(0);
        sizePolicy6.setHeightForWidth(dataClearButton->sizePolicy().hasHeightForWidth());
        dataClearButton->setSizePolicy(sizePolicy6);
        dataClearButton->setMaximumSize(QSize(75, 16777215));
        dataClearButton->setStyleSheet(QString::fromUtf8(""));

        gridLayout_25->addWidget(dataClearButton, 0, 0, 1, 1);

        horizontalSpacer_51 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_25->addItem(horizontalSpacer_51, 1, 1, 1, 1);

        groupBox_dataselections = new QGroupBox(groupBox_datatab);
        groupBox_dataselections->setObjectName(QString::fromUtf8("groupBox_dataselections"));
        groupBox_dataselections->setGeometry(QRect(10, 245, 780, 250));
        QSizePolicy sizePolicy7(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
        sizePolicy7.setHorizontalStretch(0);
        sizePolicy7.setVerticalStretch(0);
        sizePolicy7.setHeightForWidth(groupBox_dataselections->sizePolicy().hasHeightForWidth());
        groupBox_dataselections->setSizePolicy(sizePolicy7);
        groupBox_dataselections->setMinimumSize(QSize(700, 160));
        groupBox_dataselections->setMaximumSize(QSize(1000000, 250));
        groupBox_dataselections->setStyleSheet(QString::fromUtf8(""));
        groupBox_dataselections->setCheckable(false);
        gridLayout_9 = new QGridLayout(groupBox_dataselections);
        gridLayout_9->setObjectName(QString::fromUtf8("gridLayout_9"));
        gridLayout_9->setHorizontalSpacing(6);
        gridLayout_9->setVerticalSpacing(0);
        gridLayout_9->setContentsMargins(2, 0, 2, 0);
        selectedData1 = new QPlainTextEdit(groupBox_dataselections);
        selectedData1->setObjectName(QString::fromUtf8("selectedData1"));
        QSizePolicy sizePolicy8(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy8.setHorizontalStretch(0);
        sizePolicy8.setVerticalStretch(0);
        sizePolicy8.setHeightForWidth(selectedData1->sizePolicy().hasHeightForWidth());
        selectedData1->setSizePolicy(sizePolicy8);
        selectedData1->setMinimumSize(QSize(350, 30));
        selectedData1->setMaximumSize(QSize(16777215, 60));
        selectedData1->setFrameShape(QFrame::Box);
        selectedData1->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        selectedData1->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        selectedData1->setLineWrapMode(QPlainTextEdit::NoWrap);
        selectedData1->setReadOnly(true);
        selectedData1->setTextInteractionFlags(Qt::TextSelectableByMouse);
        selectedData1->setBackgroundVisible(false);

        gridLayout_9->addWidget(selectedData1, 1, 0, 1, 5);

        horizontalSpacer_24 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_9->addItem(horizontalSpacer_24, 5, 8, 1, 1);

        selectedLayer4 = new QSpinBox(groupBox_dataselections);
        selectedLayer4->setObjectName(QString::fromUtf8("selectedLayer4"));
        selectedLayer4->setEnabled(false);
        QSizePolicy sizePolicy9(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy9.setHorizontalStretch(0);
        sizePolicy9.setVerticalStretch(0);
        sizePolicy9.setHeightForWidth(selectedLayer4->sizePolicy().hasHeightForWidth());
        selectedLayer4->setSizePolicy(sizePolicy9);
        selectedLayer4->setFrame(true);
        selectedLayer4->setButtonSymbols(QAbstractSpinBox::NoButtons);
        selectedLayer4->setMinimum(-1);

        gridLayout_9->addWidget(selectedLayer4, 4, 6, 1, 1);

        selectedLayer1 = new QSpinBox(groupBox_dataselections);
        selectedLayer1->setObjectName(QString::fromUtf8("selectedLayer1"));
        selectedLayer1->setEnabled(false);
        sizePolicy9.setHeightForWidth(selectedLayer1->sizePolicy().hasHeightForWidth());
        selectedLayer1->setSizePolicy(sizePolicy9);
        selectedLayer1->setFrame(true);
        selectedLayer1->setButtonSymbols(QAbstractSpinBox::NoButtons);
        selectedLayer1->setMinimum(-1);

        gridLayout_9->addWidget(selectedLayer1, 1, 6, 1, 1);

        selectedLayer3 = new QSpinBox(groupBox_dataselections);
        selectedLayer3->setObjectName(QString::fromUtf8("selectedLayer3"));
        selectedLayer3->setEnabled(false);
        sizePolicy9.setHeightForWidth(selectedLayer3->sizePolicy().hasHeightForWidth());
        selectedLayer3->setSizePolicy(sizePolicy9);
        selectedLayer3->setFrame(true);
        selectedLayer3->setButtonSymbols(QAbstractSpinBox::NoButtons);
        selectedLayer3->setMinimum(-1);

        gridLayout_9->addWidget(selectedLayer3, 3, 6, 1, 1);

        selectedLayer5 = new QSpinBox(groupBox_dataselections);
        selectedLayer5->setObjectName(QString::fromUtf8("selectedLayer5"));
        selectedLayer5->setEnabled(false);
        sizePolicy9.setHeightForWidth(selectedLayer5->sizePolicy().hasHeightForWidth());
        selectedLayer5->setSizePolicy(sizePolicy9);
        selectedLayer5->setFrame(true);
        selectedLayer5->setButtonSymbols(QAbstractSpinBox::NoButtons);
        selectedLayer5->setMinimum(-1);

        gridLayout_9->addWidget(selectedLayer5, 5, 6, 1, 1);

        selectedData5 = new QPlainTextEdit(groupBox_dataselections);
        selectedData5->setObjectName(QString::fromUtf8("selectedData5"));
        sizePolicy8.setHeightForWidth(selectedData5->sizePolicy().hasHeightForWidth());
        selectedData5->setSizePolicy(sizePolicy8);
        selectedData5->setMinimumSize(QSize(350, 30));
        selectedData5->setMaximumSize(QSize(16777215, 60));
        selectedData5->setFrameShape(QFrame::Box);
        selectedData5->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        selectedData5->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        selectedData5->setLineWrapMode(QPlainTextEdit::NoWrap);
        selectedData5->setReadOnly(true);

        gridLayout_9->addWidget(selectedData5, 5, 0, 1, 5);

        regridCheckbox2 = new QCheckBox(groupBox_dataselections);
        regridCheckbox2->setObjectName(QString::fromUtf8("regridCheckbox2"));
        regridCheckbox2->setEnabled(true);
        sizePolicy9.setHeightForWidth(regridCheckbox2->sizePolicy().hasHeightForWidth());
        regridCheckbox2->setSizePolicy(sizePolicy9);

        gridLayout_9->addWidget(regridCheckbox2, 2, 7, 1, 1);

        regridCheckbox3 = new QCheckBox(groupBox_dataselections);
        regridCheckbox3->setObjectName(QString::fromUtf8("regridCheckbox3"));
        regridCheckbox3->setEnabled(true);
        sizePolicy9.setHeightForWidth(regridCheckbox3->sizePolicy().hasHeightForWidth());
        regridCheckbox3->setSizePolicy(sizePolicy9);

        gridLayout_9->addWidget(regridCheckbox3, 3, 7, 1, 1);

        selectedData2 = new QPlainTextEdit(groupBox_dataselections);
        selectedData2->setObjectName(QString::fromUtf8("selectedData2"));
        sizePolicy8.setHeightForWidth(selectedData2->sizePolicy().hasHeightForWidth());
        selectedData2->setSizePolicy(sizePolicy8);
        selectedData2->setMinimumSize(QSize(350, 30));
        selectedData2->setMaximumSize(QSize(16777215, 60));
        selectedData2->setFrameShape(QFrame::Box);
        selectedData2->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        selectedData2->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        selectedData2->setLineWrapMode(QPlainTextEdit::NoWrap);
        selectedData2->setReadOnly(true);

        gridLayout_9->addWidget(selectedData2, 2, 0, 1, 5);

        regridCheckbox4 = new QCheckBox(groupBox_dataselections);
        regridCheckbox4->setObjectName(QString::fromUtf8("regridCheckbox4"));
        regridCheckbox4->setEnabled(true);
        sizePolicy9.setHeightForWidth(regridCheckbox4->sizePolicy().hasHeightForWidth());
        regridCheckbox4->setSizePolicy(sizePolicy9);

        gridLayout_9->addWidget(regridCheckbox4, 4, 7, 1, 1);

        regridCheckbox5 = new QCheckBox(groupBox_dataselections);
        regridCheckbox5->setObjectName(QString::fromUtf8("regridCheckbox5"));
        regridCheckbox5->setEnabled(true);
        sizePolicy9.setHeightForWidth(regridCheckbox5->sizePolicy().hasHeightForWidth());
        regridCheckbox5->setSizePolicy(sizePolicy9);

        gridLayout_9->addWidget(regridCheckbox5, 5, 7, 1, 1);

        aggregate2 = new QComboBox(groupBox_dataselections);
        aggregate2->setObjectName(QString::fromUtf8("aggregate2"));
        sizePolicy9.setHeightForWidth(aggregate2->sizePolicy().hasHeightForWidth());
        aggregate2->setSizePolicy(sizePolicy9);
        QFont font1;
        font1.setPointSize(9);
        aggregate2->setFont(font1);

        gridLayout_9->addWidget(aggregate2, 2, 5, 1, 1);

        regridCheckbox1 = new QCheckBox(groupBox_dataselections);
        regridCheckbox1->setObjectName(QString::fromUtf8("regridCheckbox1"));
        regridCheckbox1->setEnabled(true);
        sizePolicy9.setHeightForWidth(regridCheckbox1->sizePolicy().hasHeightForWidth());
        regridCheckbox1->setSizePolicy(sizePolicy9);

        gridLayout_9->addWidget(regridCheckbox1, 1, 7, 1, 1);

        selectedData3 = new QPlainTextEdit(groupBox_dataselections);
        selectedData3->setObjectName(QString::fromUtf8("selectedData3"));
        sizePolicy8.setHeightForWidth(selectedData3->sizePolicy().hasHeightForWidth());
        selectedData3->setSizePolicy(sizePolicy8);
        selectedData3->setMinimumSize(QSize(350, 30));
        selectedData3->setMaximumSize(QSize(16777215, 60));
        selectedData3->setFrameShape(QFrame::Box);
        selectedData3->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        selectedData3->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        selectedData3->setLineWrapMode(QPlainTextEdit::NoWrap);
        selectedData3->setReadOnly(true);

        gridLayout_9->addWidget(selectedData3, 3, 0, 1, 5);

        trueColorComboBox = new QComboBox(groupBox_dataselections);
        trueColorComboBox->setObjectName(QString::fromUtf8("trueColorComboBox"));
        sizePolicy5.setHeightForWidth(trueColorComboBox->sizePolicy().hasHeightForWidth());
        trueColorComboBox->setSizePolicy(sizePolicy5);

        gridLayout_9->addWidget(trueColorComboBox, 6, 1, 1, 3);

        label_8 = new QLabel(groupBox_dataselections);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        gridLayout_9->addWidget(label_8, 6, 0, 1, 1);

        selectedData4 = new QPlainTextEdit(groupBox_dataselections);
        selectedData4->setObjectName(QString::fromUtf8("selectedData4"));
        sizePolicy8.setHeightForWidth(selectedData4->sizePolicy().hasHeightForWidth());
        selectedData4->setSizePolicy(sizePolicy8);
        selectedData4->setMinimumSize(QSize(350, 30));
        selectedData4->setMaximumSize(QSize(16777215, 60));
        selectedData4->setStyleSheet(QString::fromUtf8(""));
        selectedData4->setFrameShape(QFrame::Box);
        selectedData4->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        selectedData4->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        selectedData4->setLineWrapMode(QPlainTextEdit::NoWrap);
        selectedData4->setReadOnly(true);

        gridLayout_9->addWidget(selectedData4, 4, 0, 1, 5);

        aggregate5 = new QComboBox(groupBox_dataselections);
        aggregate5->setObjectName(QString::fromUtf8("aggregate5"));
        sizePolicy9.setHeightForWidth(aggregate5->sizePolicy().hasHeightForWidth());
        aggregate5->setSizePolicy(sizePolicy9);
        aggregate5->setFont(font1);

        gridLayout_9->addWidget(aggregate5, 5, 5, 1, 1);

        aggregate3 = new QComboBox(groupBox_dataselections);
        aggregate3->setObjectName(QString::fromUtf8("aggregate3"));
        sizePolicy9.setHeightForWidth(aggregate3->sizePolicy().hasHeightForWidth());
        aggregate3->setSizePolicy(sizePolicy9);
        aggregate3->setFont(font1);

        gridLayout_9->addWidget(aggregate3, 3, 5, 1, 1);

        aggregate1 = new QComboBox(groupBox_dataselections);
        aggregate1->setObjectName(QString::fromUtf8("aggregate1"));
        sizePolicy9.setHeightForWidth(aggregate1->sizePolicy().hasHeightForWidth());
        aggregate1->setSizePolicy(sizePolicy9);
        aggregate1->setFont(font1);

        gridLayout_9->addWidget(aggregate1, 1, 5, 1, 1);

        selectedLayer2 = new QSpinBox(groupBox_dataselections);
        selectedLayer2->setObjectName(QString::fromUtf8("selectedLayer2"));
        selectedLayer2->setEnabled(false);
        sizePolicy9.setHeightForWidth(selectedLayer2->sizePolicy().hasHeightForWidth());
        selectedLayer2->setSizePolicy(sizePolicy9);
        selectedLayer2->setFrame(true);
        selectedLayer2->setButtonSymbols(QAbstractSpinBox::NoButtons);
        selectedLayer2->setMinimum(-1);

        gridLayout_9->addWidget(selectedLayer2, 2, 6, 1, 1);

        horizontalSpacer_9 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_9->addItem(horizontalSpacer_9, 2, 8, 1, 1);

        aggregate4 = new QComboBox(groupBox_dataselections);
        aggregate4->setObjectName(QString::fromUtf8("aggregate4"));
        sizePolicy9.setHeightForWidth(aggregate4->sizePolicy().hasHeightForWidth());
        aggregate4->setSizePolicy(sizePolicy9);
        aggregate4->setFont(font1);

        gridLayout_9->addWidget(aggregate4, 4, 5, 1, 1);

        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_9->addItem(horizontalSpacer_8, 1, 8, 1, 1);

        horizontalSpacer_18 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_9->addItem(horizontalSpacer_18, 3, 8, 1, 1);

        horizontalSpacer_19 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_9->addItem(horizontalSpacer_19, 4, 8, 1, 1);

        label_selectedData = new QLabel(groupBox_dataselections);
        label_selectedData->setObjectName(QString::fromUtf8("label_selectedData"));
        QSizePolicy sizePolicy10(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy10.setHorizontalStretch(0);
        sizePolicy10.setVerticalStretch(0);
        sizePolicy10.setHeightForWidth(label_selectedData->sizePolicy().hasHeightForWidth());
        label_selectedData->setSizePolicy(sizePolicy10);

        gridLayout_9->addWidget(label_selectedData, 0, 0, 1, 5);

        label_aggregate = new QLabel(groupBox_dataselections);
        label_aggregate->setObjectName(QString::fromUtf8("label_aggregate"));

        gridLayout_9->addWidget(label_aggregate, 0, 5, 1, 1);

        label_regrid = new QLabel(groupBox_dataselections);
        label_regrid->setObjectName(QString::fromUtf8("label_regrid"));
        QSizePolicy sizePolicy11(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy11.setHorizontalStretch(0);
        sizePolicy11.setVerticalStretch(0);
        sizePolicy11.setHeightForWidth(label_regrid->sizePolicy().hasHeightForWidth());
        label_regrid->setSizePolicy(sizePolicy11);

        gridLayout_9->addWidget(label_regrid, 0, 7, 1, 1);

        label_layer = new QLabel(groupBox_dataselections);
        label_layer->setObjectName(QString::fromUtf8("label_layer"));

        gridLayout_9->addWidget(label_layer, 0, 6, 1, 1);

        groupBox_datetime = new QGroupBox(groupBox_datatab);
        groupBox_datetime->setObjectName(QString::fromUtf8("groupBox_datetime"));
        groupBox_datetime->setGeometry(QRect(10, 501, 525, 87));
        QSizePolicy sizePolicy12(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);
        sizePolicy12.setHorizontalStretch(0);
        sizePolicy12.setVerticalStretch(0);
        sizePolicy12.setHeightForWidth(groupBox_datetime->sizePolicy().hasHeightForWidth());
        groupBox_datetime->setSizePolicy(sizePolicy12);
        groupBox_datetime->setMinimumSize(QSize(350, 0));
        groupBox_datetime->setStyleSheet(QString::fromUtf8(""));
        horizontalLayout_3 = new QHBoxLayout(groupBox_datetime);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(2, 0, 0, 0);
        dateGroupBox = new QGroupBox(groupBox_datetime);
        dateGroupBox->setObjectName(QString::fromUtf8("dateGroupBox"));
        dateGroupBox->setEnabled(true);
        sizePolicy.setHeightForWidth(dateGroupBox->sizePolicy().hasHeightForWidth());
        dateGroupBox->setSizePolicy(sizePolicy);
        dateGroupBox->setMinimumSize(QSize(100, 85));
        dateGroupBox->setMaximumSize(QSize(400, 200));
        dateGroupBox->setStyleSheet(QString::fromUtf8(""));
        dateGroupBox->setFlat(false);
        gridLayout_11 = new QGridLayout(dateGroupBox);
        gridLayout_11->setContentsMargins(2, 2, 2, 2);
        gridLayout_11->setObjectName(QString::fromUtf8("gridLayout_11"));
        dateLabel = new QLabel(dateGroupBox);
        dateLabel->setObjectName(QString::fromUtf8("dateLabel"));
        sizePolicy6.setHeightForWidth(dateLabel->sizePolicy().hasHeightForWidth());
        dateLabel->setSizePolicy(sizePolicy6);
        dateLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_11->addWidget(dateLabel, 1, 0, 1, 1);

        daysLabel = new QLabel(dateGroupBox);
        daysLabel->setObjectName(QString::fromUtf8("daysLabel"));
        sizePolicy6.setHeightForWidth(daysLabel->sizePolicy().hasHeightForWidth());
        daysLabel->setSizePolicy(sizePolicy6);
        daysLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_11->addWidget(daysLabel, 2, 0, 1, 1);

        durationSpinBox = new QSpinBox(dateGroupBox);
        durationSpinBox->setObjectName(QString::fromUtf8("durationSpinBox"));
        sizePolicy5.setHeightForWidth(durationSpinBox->sizePolicy().hasHeightForWidth());
        durationSpinBox->setSizePolicy(sizePolicy5);
        durationSpinBox->setMinimumSize(QSize(0, 24));
        durationSpinBox->setMaximumSize(QSize(16777215, 35));
        durationSpinBox->setMinimum(1);
        durationSpinBox->setMaximum(30);

        gridLayout_11->addWidget(durationSpinBox, 2, 1, 1, 1);

        dateDateEdit = new QDateEdit(dateGroupBox);
        dateDateEdit->setObjectName(QString::fromUtf8("dateDateEdit"));
        sizePolicy5.setHeightForWidth(dateDateEdit->sizePolicy().hasHeightForWidth());
        dateDateEdit->setSizePolicy(sizePolicy5);
        dateDateEdit->setMinimumSize(QSize(100, 24));
        dateDateEdit->setMaximumSize(QSize(16777215, 35));
        dateDateEdit->setDateTime(QDateTime(QDate(2008, 7, 3), QTime(4, 0, 0)));
        dateDateEdit->setDate(QDate(2008, 7, 3));
        dateDateEdit->setTime(QTime(4, 0, 0));
        dateDateEdit->setMaximumTime(QTime(4, 59, 59));
        dateDateEdit->setMinimumTime(QTime(4, 0, 0));
        dateDateEdit->setCalendarPopup(true);
        dateDateEdit->setTimeSpec(Qt::UTC);

        gridLayout_11->addWidget(dateDateEdit, 1, 1, 1, 1);

        verticalSpacer_7 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_11->addItem(verticalSpacer_7, 3, 1, 1, 1);


        horizontalLayout_3->addWidget(dateGroupBox);

        boundingGroupBox = new QGroupBox(groupBox_datetime);
        boundingGroupBox->setObjectName(QString::fromUtf8("boundingGroupBox"));
        sizePolicy.setHeightForWidth(boundingGroupBox->sizePolicy().hasHeightForWidth());
        boundingGroupBox->setSizePolicy(sizePolicy);
        boundingGroupBox->setMinimumSize(QSize(300, 85));
        boundingGroupBox->setMaximumSize(QSize(400, 200));
        boundingGroupBox->setAutoFillBackground(false);
        boundingGroupBox->setStyleSheet(QString::fromUtf8(""));
        boundingGroupBox->setFlat(false);
        gridLayout_12 = new QGridLayout(boundingGroupBox);
        gridLayout_12->setContentsMargins(2, 2, 2, 2);
        gridLayout_12->setObjectName(QString::fromUtf8("gridLayout_12"));
        gridLayout_12->setHorizontalSpacing(9);
        gridLayout_12->setVerticalSpacing(3);
        northDSpinBox = new QDoubleSpinBox(boundingGroupBox);
        northDSpinBox->setObjectName(QString::fromUtf8("northDSpinBox"));
        sizePolicy5.setHeightForWidth(northDSpinBox->sizePolicy().hasHeightForWidth());
        northDSpinBox->setSizePolicy(sizePolicy5);
        northDSpinBox->setMinimumSize(QSize(50, 25));
        northDSpinBox->setMaximumSize(QSize(120, 50));
        northDSpinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
        northDSpinBox->setMinimum(-90);
        northDSpinBox->setMaximum(90);

        gridLayout_12->addWidget(northDSpinBox, 13, 5, 1, 1);

        westDSpinBox = new QDoubleSpinBox(boundingGroupBox);
        westDSpinBox->setObjectName(QString::fromUtf8("westDSpinBox"));
        sizePolicy5.setHeightForWidth(westDSpinBox->sizePolicy().hasHeightForWidth());
        westDSpinBox->setSizePolicy(sizePolicy5);
        westDSpinBox->setMinimumSize(QSize(50, 25));
        westDSpinBox->setMaximumSize(QSize(120, 50));
        westDSpinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
        westDSpinBox->setMinimum(-180);
        westDSpinBox->setMaximum(180);

        gridLayout_12->addWidget(westDSpinBox, 15, 4, 1, 1);

        label_n = new QLabel(boundingGroupBox);
        label_n->setObjectName(QString::fromUtf8("label_n"));
        sizePolicy6.setHeightForWidth(label_n->sizePolicy().hasHeightForWidth());
        label_n->setSizePolicy(sizePolicy6);
        label_n->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_12->addWidget(label_n, 13, 4, 1, 1);

        southDSpinBox = new QDoubleSpinBox(boundingGroupBox);
        southDSpinBox->setObjectName(QString::fromUtf8("southDSpinBox"));
        sizePolicy5.setHeightForWidth(southDSpinBox->sizePolicy().hasHeightForWidth());
        southDSpinBox->setSizePolicy(sizePolicy5);
        southDSpinBox->setMinimumSize(QSize(50, 25));
        southDSpinBox->setMaximumSize(QSize(120, 50));
        southDSpinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
        southDSpinBox->setMinimum(-90);
        southDSpinBox->setMaximum(90);

        gridLayout_12->addWidget(southDSpinBox, 16, 5, 1, 1);

        label_s = new QLabel(boundingGroupBox);
        label_s->setObjectName(QString::fromUtf8("label_s"));
        sizePolicy6.setHeightForWidth(label_s->sizePolicy().hasHeightForWidth());
        label_s->setSizePolicy(sizePolicy6);
        label_s->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_12->addWidget(label_s, 16, 4, 1, 1);

        label = new QLabel(boundingGroupBox);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout_12->addWidget(label, 13, 0, 1, 1);

        label_e = new QLabel(boundingGroupBox);
        label_e->setObjectName(QString::fromUtf8("label_e"));
        sizePolicy6.setHeightForWidth(label_e->sizePolicy().hasHeightForWidth());
        label_e->setSizePolicy(sizePolicy6);
        label_e->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_12->addWidget(label_e, 15, 0, 1, 2);

        groupBox_west = new QGroupBox(boundingGroupBox);
        groupBox_west->setObjectName(QString::fromUtf8("groupBox_west"));
        sizePolicy.setHeightForWidth(groupBox_west->sizePolicy().hasHeightForWidth());
        groupBox_west->setSizePolicy(sizePolicy);
        groupBox_west->setMinimumSize(QSize(40, 0));
        eastDSpinBox = new QDoubleSpinBox(groupBox_west);
        eastDSpinBox->setObjectName(QString::fromUtf8("eastDSpinBox"));
        eastDSpinBox->setGeometry(QRect(4, 0, 50, 30));
        sizePolicy5.setHeightForWidth(eastDSpinBox->sizePolicy().hasHeightForWidth());
        eastDSpinBox->setSizePolicy(sizePolicy5);
        eastDSpinBox->setMinimumSize(QSize(50, 25));
        eastDSpinBox->setMaximumSize(QSize(120, 50));
        eastDSpinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
        eastDSpinBox->setMinimum(-180);
        eastDSpinBox->setMaximum(180);
        label_w = new QLabel(groupBox_west);
        label_w->setObjectName(QString::fromUtf8("label_w"));
        label_w->setGeometry(QRect(60, 4, 25, 16));
        sizePolicy6.setHeightForWidth(label_w->sizePolicy().hasHeightForWidth());
        label_w->setSizePolicy(sizePolicy6);
        label_w->setMaximumSize(QSize(30, 16777215));
        label_w->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_12->addWidget(groupBox_west, 15, 6, 1, 1);

        verticalSpacer_8 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_12->addItem(verticalSpacer_8, 17, 5, 1, 1);


        horizontalLayout_3->addWidget(boundingGroupBox);

        horizontalSpacer_25 = new QSpacerItem(100, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_25);

        groupBox_getdata = new QGroupBox(groupBox_datatab);
        groupBox_getdata->setObjectName(QString::fromUtf8("groupBox_getdata"));
        groupBox_getdata->setGeometry(QRect(10, 602, 700, 50));
        sizePolicy.setHeightForWidth(groupBox_getdata->sizePolicy().hasHeightForWidth());
        groupBox_getdata->setSizePolicy(sizePolicy);
        groupBox_getdata->setMinimumSize(QSize(700, 50));
        groupBox_getdata->setAutoFillBackground(true);
        horizontalLayout_4 = new QHBoxLayout(groupBox_getdata);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(2, 0, 2, 0);
        filterToolButton = new QToolButton(groupBox_getdata);
        filterToolButton->setObjectName(QString::fromUtf8("filterToolButton"));
        sizePolicy5.setHeightForWidth(filterToolButton->sizePolicy().hasHeightForWidth());
        filterToolButton->setSizePolicy(sizePolicy5);
        filterToolButton->setMinimumSize(QSize(0, 15));
        filterToolButton->setMaximumSize(QSize(250, 40));

        horizontalLayout_4->addWidget(filterToolButton);

        regridToolButton = new QToolButton(groupBox_getdata);
        regridToolButton->setObjectName(QString::fromUtf8("regridToolButton"));
        sizePolicy5.setHeightForWidth(regridToolButton->sizePolicy().hasHeightForWidth());
        regridToolButton->setSizePolicy(sizePolicy5);
        regridToolButton->setMinimumSize(QSize(0, 15));
        regridToolButton->setMaximumSize(QSize(250, 40));

        horizontalLayout_4->addWidget(regridToolButton);

        checkServersPushButton = new QPushButton(groupBox_getdata);
        checkServersPushButton->setObjectName(QString::fromUtf8("checkServersPushButton"));
        sizePolicy5.setHeightForWidth(checkServersPushButton->sizePolicy().hasHeightForWidth());
        checkServersPushButton->setSizePolicy(sizePolicy5);
        checkServersPushButton->setMinimumSize(QSize(0, 15));
        checkServersPushButton->setMaximumSize(QSize(250, 40));
        QFont font2;
        font2.setBold(false);
        font2.setWeight(50);
        checkServersPushButton->setFont(font2);
        checkServersPushButton->setAutoFillBackground(false);
        checkServersPushButton->setAutoDefault(false);
        checkServersPushButton->setDefault(false);
        checkServersPushButton->setFlat(false);

        horizontalLayout_4->addWidget(checkServersPushButton);

        horizontalSpacer_26 = new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_26);

        getdataPushButton = new QPushButton(groupBox_getdata);
        getdataPushButton->setObjectName(QString::fromUtf8("getdataPushButton"));
        sizePolicy5.setHeightForWidth(getdataPushButton->sizePolicy().hasHeightForWidth());
        getdataPushButton->setSizePolicy(sizePolicy5);
        getdataPushButton->setMinimumSize(QSize(0, 15));
        getdataPushButton->setMaximumSize(QSize(250, 40));
        QFont font3;
        font3.setPointSize(11);
        font3.setBold(true);
        font3.setWeight(75);
        getdataPushButton->setFont(font3);
        getdataPushButton->setAutoFillBackground(false);
        getdataPushButton->setAutoDefault(false);
        getdataPushButton->setDefault(false);
        getdataPushButton->setFlat(false);

        horizontalLayout_4->addWidget(getdataPushButton);

        horizontalSpacer_17 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_17);

        tabWidget->addTab(tab_2, QString());
        propTab = new QWidget();
        propTab->setObjectName(QString::fromUtf8("propTab"));
        sizePolicy3.setHeightForWidth(propTab->sizePolicy().hasHeightForWidth());
        propTab->setSizePolicy(sizePolicy3);
        formLayoutWidget = new QWidget(propTab);
        formLayoutWidget->setObjectName(QString::fromUtf8("formLayoutWidget"));
        formLayoutWidget->setGeometry(QRect(7, 7, 961, 1280));
        formLayout = new QFormLayout(formLayoutWidget);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        formLayout->setVerticalSpacing(6);
        formLayout->setContentsMargins(0, 0, 0, 0);
        label_mode = new QLabel(formLayoutWidget);
        label_mode->setObjectName(QString::fromUtf8("label_mode"));
        label_mode->setMinimumSize(QSize(0, 25));

        formLayout->setWidget(0, QFormLayout::LabelRole, label_mode);

        groupBox_mode = new QGroupBox(formLayoutWidget);
        groupBox_mode->setObjectName(QString::fromUtf8("groupBox_mode"));
        sizePolicy.setHeightForWidth(groupBox_mode->sizePolicy().hasHeightForWidth());
        groupBox_mode->setSizePolicy(sizePolicy);
        groupBox_mode->setMinimumSize(QSize(780, 60));
        groupBox_mode->setMaximumSize(QSize(400, 16777215));
        groupBox_mode->setFlat(false);
        horizontalLayout_5 = new QHBoxLayout(groupBox_mode);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalLayout_5->setContentsMargins(0, -1, 0, -1);
        radioButton2D = new QRadioButton(groupBox_mode);
        radioButton2D->setObjectName(QString::fromUtf8("radioButton2D"));
        sizePolicy5.setHeightForWidth(radioButton2D->sizePolicy().hasHeightForWidth());
        radioButton2D->setSizePolicy(sizePolicy5);
        radioButton2D->setMinimumSize(QSize(30, 20));
        radioButton2D->setChecked(true);

        horizontalLayout_5->addWidget(radioButton2D);

        radioButton3D = new QRadioButton(groupBox_mode);
        radioButton3D->setObjectName(QString::fromUtf8("radioButton3D"));
        sizePolicy5.setHeightForWidth(radioButton3D->sizePolicy().hasHeightForWidth());
        radioButton3D->setSizePolicy(sizePolicy5);
        radioButton3D->setMinimumSize(QSize(30, 20));

        horizontalLayout_5->addWidget(radioButton3D);

        radioButtonPanelDisplay = new QRadioButton(groupBox_mode);
        radioButtonPanelDisplay->setObjectName(QString::fromUtf8("radioButtonPanelDisplay"));
        sizePolicy5.setHeightForWidth(radioButtonPanelDisplay->sizePolicy().hasHeightForWidth());
        radioButtonPanelDisplay->setSizePolicy(sizePolicy5);
        radioButtonPanelDisplay->setMinimumSize(QSize(50, 20));

        horizontalLayout_5->addWidget(radioButtonPanelDisplay);

        autozoomView = new QPushButton(groupBox_mode);
        autozoomView->setObjectName(QString::fromUtf8("autozoomView"));
        sizePolicy12.setHeightForWidth(autozoomView->sizePolicy().hasHeightForWidth());
        autozoomView->setSizePolicy(sizePolicy12);
        autozoomView->setMinimumSize(QSize(15, 20));
        autozoomView->setFlat(false);

        horizontalLayout_5->addWidget(autozoomView);

        resetView = new QPushButton(groupBox_mode);
        resetView->setObjectName(QString::fromUtf8("resetView"));
        sizePolicy12.setHeightForWidth(resetView->sizePolicy().hasHeightForWidth());
        resetView->setSizePolicy(sizePolicy12);
        resetView->setMinimumSize(QSize(15, 20));
        resetView->setFlat(false);

        horizontalLayout_5->addWidget(resetView);

        horizontalSpacer_27 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_27);


        formLayout->setWidget(0, QFormLayout::FieldRole, groupBox_mode);

        vizVarCombobox = new QComboBox(formLayoutWidget);
        vizVarCombobox->setObjectName(QString::fromUtf8("vizVarCombobox"));
        sizePolicy4.setHeightForWidth(vizVarCombobox->sizePolicy().hasHeightForWidth());
        vizVarCombobox->setSizePolicy(sizePolicy4);
        vizVarCombobox->setMinimumSize(QSize(400, 0));
        vizVarCombobox->setMaximumSize(QSize(800, 16777215));
        vizVarCombobox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
        vizVarCombobox->setFrame(true);
        vizVarCombobox->setModelColumn(0);

        formLayout->setWidget(1, QFormLayout::SpanningRole, vizVarCombobox);

        groupBox_visibility = new QGroupBox(formLayoutWidget);
        groupBox_visibility->setObjectName(QString::fromUtf8("groupBox_visibility"));
        sizePolicy.setHeightForWidth(groupBox_visibility->sizePolicy().hasHeightForWidth());
        groupBox_visibility->setSizePolicy(sizePolicy);
        groupBox_visibility->setMinimumSize(QSize(700, 40));
        groupBox_visibility->setMaximumSize(QSize(700, 70));
        QFont font4;
        font4.setKerning(true);
        groupBox_visibility->setFont(font4);
        groupBox_visibility->setAutoFillBackground(false);
        groupBox_visibility->setFlat(true);
        groupBox_visibility->setCheckable(false);
        horizontalLayout_6 = new QHBoxLayout(groupBox_visibility);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        vizCheckBox = new QCheckBox(groupBox_visibility);
        vizCheckBox->setObjectName(QString::fromUtf8("vizCheckBox"));
        sizePolicy6.setHeightForWidth(vizCheckBox->sizePolicy().hasHeightForWidth());
        vizCheckBox->setSizePolicy(sizePolicy6);
        vizCheckBox->setMinimumSize(QSize(60, 0));
        vizCheckBox->setMaximumSize(QSize(200, 16777215));

        horizontalLayout_6->addWidget(vizCheckBox);

        cellBoundaryCheckbox = new QCheckBox(groupBox_visibility);
        cellBoundaryCheckbox->setObjectName(QString::fromUtf8("cellBoundaryCheckbox"));
        sizePolicy6.setHeightForWidth(cellBoundaryCheckbox->sizePolicy().hasHeightForWidth());
        cellBoundaryCheckbox->setSizePolicy(sizePolicy6);
        cellBoundaryCheckbox->setMinimumSize(QSize(60, 0));
        cellBoundaryCheckbox->setMaximumSize(QSize(250, 16777215));

        horizontalLayout_6->addWidget(cellBoundaryCheckbox);

        horizontalSpacer_29 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_29);


        formLayout->setWidget(2, QFormLayout::SpanningRole, groupBox_visibility);

        label_opacity = new QLabel(formLayoutWidget);
        label_opacity->setObjectName(QString::fromUtf8("label_opacity"));

        formLayout->setWidget(3, QFormLayout::LabelRole, label_opacity);

        groupBox_opacity = new QGroupBox(formLayoutWidget);
        groupBox_opacity->setObjectName(QString::fromUtf8("groupBox_opacity"));
        sizePolicy.setHeightForWidth(groupBox_opacity->sizePolicy().hasHeightForWidth());
        groupBox_opacity->setSizePolicy(sizePolicy);
        groupBox_opacity->setMinimumSize(QSize(0, 32));
        groupBox_opacity->setFlat(true);
        vizOpacitySlider = new QSlider(groupBox_opacity);
        vizOpacitySlider->setObjectName(QString::fromUtf8("vizOpacitySlider"));
        vizOpacitySlider->setGeometry(QRect(8, 4, 265, 19));
        vizOpacitySlider->setMinimumSize(QSize(0, 0));
        vizOpacitySlider->setMaximumSize(QSize(265, 16777215));
        vizOpacitySlider->setValue(99);
        vizOpacitySlider->setOrientation(Qt::Horizontal);
        opacity_display = new QLabel(groupBox_opacity);
        opacity_display->setObjectName(QString::fromUtf8("opacity_display"));
        opacity_display->setGeometry(QRect(276, 0, 25, 25));
        opacity_display->setAlignment(Qt::AlignJustify|Qt::AlignVCenter);

        formLayout->setWidget(3, QFormLayout::FieldRole, groupBox_opacity);

        label_minmax = new QLabel(formLayoutWidget);
        label_minmax->setObjectName(QString::fromUtf8("label_minmax"));

        formLayout->setWidget(4, QFormLayout::LabelRole, label_minmax);

        groupBox_minmax = new QGroupBox(formLayoutWidget);
        groupBox_minmax->setObjectName(QString::fromUtf8("groupBox_minmax"));
        sizePolicy.setHeightForWidth(groupBox_minmax->sizePolicy().hasHeightForWidth());
        groupBox_minmax->setSizePolicy(sizePolicy);
        groupBox_minmax->setMinimumSize(QSize(700, 42));
        groupBox_minmax->setMaximumSize(QSize(700, 16777215));
        groupBox_minmax->setAutoFillBackground(false);
        groupBox_minmax->setFlat(true);
        horizontalLayout_7 = new QHBoxLayout(groupBox_minmax);
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        vizMinSpinbox = new QDoubleSpinBox(groupBox_minmax);
        vizMinSpinbox->setObjectName(QString::fromUtf8("vizMinSpinbox"));
        sizePolicy6.setHeightForWidth(vizMinSpinbox->sizePolicy().hasHeightForWidth());
        vizMinSpinbox->setSizePolicy(sizePolicy6);
        vizMinSpinbox->setMaximumSize(QSize(200, 16777215));
        vizMinSpinbox->setFocusPolicy(Qt::ClickFocus);
        vizMinSpinbox->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        vizMinSpinbox->setDecimals(25);

        horizontalLayout_7->addWidget(vizMinSpinbox);

        vizMaxSpinbox = new QDoubleSpinBox(groupBox_minmax);
        vizMaxSpinbox->setObjectName(QString::fromUtf8("vizMaxSpinbox"));
        sizePolicy6.setHeightForWidth(vizMaxSpinbox->sizePolicy().hasHeightForWidth());
        vizMaxSpinbox->setSizePolicy(sizePolicy6);
        vizMaxSpinbox->setMaximumSize(QSize(200, 16777215));
        vizMaxSpinbox->setFocusPolicy(Qt::ClickFocus);
        vizMaxSpinbox->setDecimals(25);

        horizontalLayout_7->addWidget(vizMaxSpinbox);

        vizDataDefault = new QCheckBox(groupBox_minmax);
        vizDataDefault->setObjectName(QString::fromUtf8("vizDataDefault"));
        QSizePolicy sizePolicy13(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
        sizePolicy13.setHorizontalStretch(0);
        sizePolicy13.setVerticalStretch(0);
        sizePolicy13.setHeightForWidth(vizDataDefault->sizePolicy().hasHeightForWidth());
        vizDataDefault->setSizePolicy(sizePolicy13);
        vizDataDefault->setChecked(true);

        horizontalLayout_7->addWidget(vizDataDefault);

        horizontalSpacer_31 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_31);


        formLayout->setWidget(4, QFormLayout::FieldRole, groupBox_minmax);

        label_threshold = new QLabel(formLayoutWidget);
        label_threshold->setObjectName(QString::fromUtf8("label_threshold"));

        formLayout->setWidget(5, QFormLayout::LabelRole, label_threshold);

        groupBox_threshold = new QGroupBox(formLayoutWidget);
        groupBox_threshold->setObjectName(QString::fromUtf8("groupBox_threshold"));
        sizePolicy.setHeightForWidth(groupBox_threshold->sizePolicy().hasHeightForWidth());
        groupBox_threshold->setSizePolicy(sizePolicy);
        groupBox_threshold->setMinimumSize(QSize(0, 32));
        groupBox_threshold->setFlat(true);
        horizontalLayout_17 = new QHBoxLayout(groupBox_threshold);
        horizontalLayout_17->setObjectName(QString::fromUtf8("horizontalLayout_17"));
        thresholdCheckBox = new QCheckBox(groupBox_threshold);
        thresholdCheckBox->setObjectName(QString::fromUtf8("thresholdCheckBox"));
        sizePolicy5.setHeightForWidth(thresholdCheckBox->sizePolicy().hasHeightForWidth());
        thresholdCheckBox->setSizePolicy(sizePolicy5);

        horizontalLayout_17->addWidget(thresholdCheckBox);

        ThresholdSlider = new QSlider(groupBox_threshold);
        ThresholdSlider->setObjectName(QString::fromUtf8("ThresholdSlider"));
        sizePolicy5.setHeightForWidth(ThresholdSlider->sizePolicy().hasHeightForWidth());
        ThresholdSlider->setSizePolicy(sizePolicy5);
        ThresholdSlider->setMinimumSize(QSize(200, 0));
        ThresholdSlider->setMaximumSize(QSize(400, 16777215));
        ThresholdSlider->setMaximum(100);
        ThresholdSlider->setValue(0);
        ThresholdSlider->setSliderPosition(0);
        ThresholdSlider->setOrientation(Qt::Horizontal);

        horizontalLayout_17->addWidget(ThresholdSlider);

        ThresholdLabel = new QLabel(groupBox_threshold);
        ThresholdLabel->setObjectName(QString::fromUtf8("ThresholdLabel"));
        ThresholdLabel->setAlignment(Qt::AlignJustify|Qt::AlignVCenter);

        horizontalLayout_17->addWidget(ThresholdLabel);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_17->addItem(horizontalSpacer_6);


        formLayout->setWidget(5, QFormLayout::FieldRole, groupBox_threshold);

        label_colormap = new QLabel(formLayoutWidget);
        label_colormap->setObjectName(QString::fromUtf8("label_colormap"));
        label_colormap->setEnabled(true);

        formLayout->setWidget(6, QFormLayout::LabelRole, label_colormap);

        groupBox_colormap = new QGroupBox(formLayoutWidget);
        groupBox_colormap->setObjectName(QString::fromUtf8("groupBox_colormap"));
        sizePolicy.setHeightForWidth(groupBox_colormap->sizePolicy().hasHeightForWidth());
        groupBox_colormap->setSizePolicy(sizePolicy);
        groupBox_colormap->setMinimumSize(QSize(0, 40));
        groupBox_colormap->setMaximumSize(QSize(700, 16777215));
        horizontalLayout_8 = new QHBoxLayout(groupBox_colormap);
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        vizColormapCombobox = new QComboBox(groupBox_colormap);
        vizColormapCombobox->setObjectName(QString::fromUtf8("vizColormapCombobox"));
        sizePolicy.setHeightForWidth(vizColormapCombobox->sizePolicy().hasHeightForWidth());
        vizColormapCombobox->setSizePolicy(sizePolicy);
        vizColormapCombobox->setMinimumSize(QSize(150, 30));
        vizColormapCombobox->setMaximumSize(QSize(400, 16777215));

        horizontalLayout_8->addWidget(vizColormapCombobox);

        horizontalSpacer_32 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_32);


        formLayout->setWidget(6, QFormLayout::FieldRole, groupBox_colormap);

        label_scalefactor = new QLabel(formLayoutWidget);
        label_scalefactor->setObjectName(QString::fromUtf8("label_scalefactor"));

        formLayout->setWidget(9, QFormLayout::LabelRole, label_scalefactor);

        groupBox_scalefactor = new QGroupBox(formLayoutWidget);
        groupBox_scalefactor->setObjectName(QString::fromUtf8("groupBox_scalefactor"));
        sizePolicy.setHeightForWidth(groupBox_scalefactor->sizePolicy().hasHeightForWidth());
        groupBox_scalefactor->setSizePolicy(sizePolicy);
        groupBox_scalefactor->setMinimumSize(QSize(300, 40));
        groupBox_scalefactor->setMaximumSize(QSize(300, 16777215));
        groupBox_scalefactor->setFlat(true);
        horizontalLayout_9 = new QHBoxLayout(groupBox_scalefactor);
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        calipsoScaleFactor = new QDoubleSpinBox(groupBox_scalefactor);
        calipsoScaleFactor->setObjectName(QString::fromUtf8("calipsoScaleFactor"));
        sizePolicy.setHeightForWidth(calipsoScaleFactor->sizePolicy().hasHeightForWidth());
        calipsoScaleFactor->setSizePolicy(sizePolicy);
        calipsoScaleFactor->setMinimumSize(QSize(90, 30));
        calipsoScaleFactor->setMaximumSize(QSize(70, 16777215));
        calipsoScaleFactor->setFocusPolicy(Qt::ClickFocus);
        calipsoScaleFactor->setKeyboardTracking(false);
        calipsoScaleFactor->setMinimum(1);
        calipsoScaleFactor->setMaximum(30);
        calipsoScaleFactor->setSingleStep(0.5);

        horizontalLayout_9->addWidget(calipsoScaleFactor);

        horizontalSpacer_33 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_9->addItem(horizontalSpacer_33);


        formLayout->setWidget(9, QFormLayout::FieldRole, groupBox_scalefactor);

        label_ruler = new QLabel(formLayoutWidget);
        label_ruler->setObjectName(QString::fromUtf8("label_ruler"));

        formLayout->setWidget(10, QFormLayout::LabelRole, label_ruler);

        groupBox_ruler = new QGroupBox(formLayoutWidget);
        groupBox_ruler->setObjectName(QString::fromUtf8("groupBox_ruler"));
        sizePolicy.setHeightForWidth(groupBox_ruler->sizePolicy().hasHeightForWidth());
        groupBox_ruler->setSizePolicy(sizePolicy);
        groupBox_ruler->setMinimumSize(QSize(0, 40));
        groupBox_ruler->setMaximumSize(QSize(800, 32));
        groupBox_ruler->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        groupBox_ruler->setFlat(true);
        horizontalLayout_10 = new QHBoxLayout(groupBox_ruler);
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        rulerCheckBox = new QCheckBox(groupBox_ruler);
        rulerCheckBox->setObjectName(QString::fromUtf8("rulerCheckBox"));
        sizePolicy5.setHeightForWidth(rulerCheckBox->sizePolicy().hasHeightForWidth());
        rulerCheckBox->setSizePolicy(sizePolicy5);
        rulerCheckBox->setChecked(true);

        horizontalLayout_10->addWidget(rulerCheckBox);

        horizontalSpacer_35 = new QSpacerItem(30, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_10->addItem(horizontalSpacer_35);

        label_24 = new QLabel(groupBox_ruler);
        label_24->setObjectName(QString::fromUtf8("label_24"));

        horizontalLayout_10->addWidget(label_24);

        rulerLonSpinBox = new QDoubleSpinBox(groupBox_ruler);
        rulerLonSpinBox->setObjectName(QString::fromUtf8("rulerLonSpinBox"));
        sizePolicy.setHeightForWidth(rulerLonSpinBox->sizePolicy().hasHeightForWidth());
        rulerLonSpinBox->setSizePolicy(sizePolicy);
        rulerLonSpinBox->setMinimumSize(QSize(100, 35));
        rulerLonSpinBox->setMaximumSize(QSize(30, 16777215));
        rulerLonSpinBox->setFocusPolicy(Qt::ClickFocus);
        rulerLonSpinBox->setMinimum(-180);
        rulerLonSpinBox->setMaximum(180);
        rulerLonSpinBox->setSingleStep(0.1);
        rulerLonSpinBox->setValue(-76.36);

        horizontalLayout_10->addWidget(rulerLonSpinBox);

        label_25 = new QLabel(groupBox_ruler);
        label_25->setObjectName(QString::fromUtf8("label_25"));

        horizontalLayout_10->addWidget(label_25);

        rulerLatSpinBox = new QDoubleSpinBox(groupBox_ruler);
        rulerLatSpinBox->setObjectName(QString::fromUtf8("rulerLatSpinBox"));
        sizePolicy.setHeightForWidth(rulerLatSpinBox->sizePolicy().hasHeightForWidth());
        rulerLatSpinBox->setSizePolicy(sizePolicy);
        rulerLatSpinBox->setMinimumSize(QSize(100, 35));
        rulerLatSpinBox->setMaximumSize(QSize(30, 16777215));
        rulerLatSpinBox->setFocusPolicy(Qt::ClickFocus);
        rulerLatSpinBox->setMinimum(-90);
        rulerLatSpinBox->setMaximum(90);
        rulerLatSpinBox->setSingleStep(0.1);
        rulerLatSpinBox->setValue(37.08);

        horizontalLayout_10->addWidget(rulerLatSpinBox);

        horizontalSpacer_34 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_10->addItem(horizontalSpacer_34);


        formLayout->setWidget(10, QFormLayout::FieldRole, groupBox_ruler);

        WindLabel = new QLabel(formLayoutWidget);
        WindLabel->setObjectName(QString::fromUtf8("WindLabel"));
        WindLabel->setMinimumSize(QSize(46, 0));

        formLayout->setWidget(11, QFormLayout::LabelRole, WindLabel);

        groupBox_windcontrols = new QGroupBox(formLayoutWidget);
        groupBox_windcontrols->setObjectName(QString::fromUtf8("groupBox_windcontrols"));
        sizePolicy.setHeightForWidth(groupBox_windcontrols->sizePolicy().hasHeightForWidth());
        groupBox_windcontrols->setSizePolicy(sizePolicy);
        groupBox_windcontrols->setMinimumSize(QSize(500, 60));
        groupBox_windcontrols->setMaximumSize(QSize(700, 16777215));
        gridLayout_10 = new QGridLayout(groupBox_windcontrols);
        gridLayout_10->setObjectName(QString::fromUtf8("gridLayout_10"));
        windVectorArrowOption = new QRadioButton(groupBox_windcontrols);
        windVectorArrowOption->setObjectName(QString::fromUtf8("windVectorArrowOption"));
        sizePolicy5.setHeightForWidth(windVectorArrowOption->sizePolicy().hasHeightForWidth());
        windVectorArrowOption->setSizePolicy(sizePolicy5);
        windVectorArrowOption->setChecked(true);

        gridLayout_10->addWidget(windVectorArrowOption, 1, 0, 1, 1);

        windVectorBarbOption = new QRadioButton(groupBox_windcontrols);
        windVectorBarbOption->setObjectName(QString::fromUtf8("windVectorBarbOption"));
        sizePolicy5.setHeightForWidth(windVectorBarbOption->sizePolicy().hasHeightForWidth());
        windVectorBarbOption->setSizePolicy(sizePolicy5);

        gridLayout_10->addWidget(windVectorBarbOption, 0, 0, 1, 1);

        windVectorScaleFactor = new QDoubleSpinBox(groupBox_windcontrols);
        windVectorScaleFactor->setObjectName(QString::fromUtf8("windVectorScaleFactor"));
        sizePolicy3.setHeightForWidth(windVectorScaleFactor->sizePolicy().hasHeightForWidth());
        windVectorScaleFactor->setSizePolicy(sizePolicy3);
        windVectorScaleFactor->setMinimumSize(QSize(50, 35));
        windVectorScaleFactor->setMaximumSize(QSize(70, 16777215));
        windVectorScaleFactor->setFocusPolicy(Qt::ClickFocus);
        windVectorScaleFactor->setKeyboardTracking(false);
        windVectorScaleFactor->setDecimals(0);
        windVectorScaleFactor->setMinimum(1);
        windVectorScaleFactor->setMaximum(100);
        windVectorScaleFactor->setSingleStep(1);
        windVectorScaleFactor->setValue(50);

        gridLayout_10->addWidget(windVectorScaleFactor, 1, 1, 1, 1);

        windVectorTipSize = new QDoubleSpinBox(groupBox_windcontrols);
        windVectorTipSize->setObjectName(QString::fromUtf8("windVectorTipSize"));
        sizePolicy3.setHeightForWidth(windVectorTipSize->sizePolicy().hasHeightForWidth());
        windVectorTipSize->setSizePolicy(sizePolicy3);
        windVectorTipSize->setMinimumSize(QSize(50, 35));
        windVectorTipSize->setMaximumSize(QSize(70, 16777215));
        windVectorTipSize->setFocusPolicy(Qt::ClickFocus);
        windVectorTipSize->setKeyboardTracking(false);
        windVectorTipSize->setDecimals(0);
        windVectorTipSize->setMinimum(2);
        windVectorTipSize->setMaximum(30);
        windVectorTipSize->setSingleStep(1);
        windVectorTipSize->setValue(10);

        gridLayout_10->addWidget(windVectorTipSize, 1, 2, 1, 1);

        windVectorScaleFactorLabel = new QLabel(groupBox_windcontrols);
        windVectorScaleFactorLabel->setObjectName(QString::fromUtf8("windVectorScaleFactorLabel"));

        gridLayout_10->addWidget(windVectorScaleFactorLabel, 0, 1, 1, 1);

        windVectorHeadSizeLabel = new QLabel(groupBox_windcontrols);
        windVectorHeadSizeLabel->setObjectName(QString::fromUtf8("windVectorHeadSizeLabel"));

        gridLayout_10->addWidget(windVectorHeadSizeLabel, 0, 2, 1, 1);

        autoSparseCheckBox = new QCheckBox(groupBox_windcontrols);
        autoSparseCheckBox->setObjectName(QString::fromUtf8("autoSparseCheckBox"));
        sizePolicy5.setHeightForWidth(autoSparseCheckBox->sizePolicy().hasHeightForWidth());
        autoSparseCheckBox->setSizePolicy(sizePolicy5);
        autoSparseCheckBox->setChecked(true);

        gridLayout_10->addWidget(autoSparseCheckBox, 0, 3, 1, 2);

        windVectorSparseness = new QDoubleSpinBox(groupBox_windcontrols);
        windVectorSparseness->setObjectName(QString::fromUtf8("windVectorSparseness"));
        sizePolicy.setHeightForWidth(windVectorSparseness->sizePolicy().hasHeightForWidth());
        windVectorSparseness->setSizePolicy(sizePolicy);
        windVectorSparseness->setMinimumSize(QSize(50, 35));
        windVectorSparseness->setMaximumSize(QSize(70, 16777215));
        windVectorSparseness->setFocusPolicy(Qt::ClickFocus);
        windVectorSparseness->setKeyboardTracking(false);
        windVectorSparseness->setDecimals(0);
        windVectorSparseness->setMinimum(1);
        windVectorSparseness->setMaximum(100);
        windVectorSparseness->setSingleStep(1);
        windVectorSparseness->setValue(10);

        gridLayout_10->addWidget(windVectorSparseness, 1, 4, 1, 1);

        windVectorSparsenessLabel = new QLabel(groupBox_windcontrols);
        windVectorSparsenessLabel->setObjectName(QString::fromUtf8("windVectorSparsenessLabel"));

        gridLayout_10->addWidget(windVectorSparsenessLabel, 1, 3, 1, 1);

        horizontalSpacer_36 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_10->addItem(horizontalSpacer_36, 1, 5, 1, 1);


        formLayout->setWidget(11, QFormLayout::FieldRole, groupBox_windcontrols);

        SpecialControlLabel = new QLabel(formLayoutWidget);
        SpecialControlLabel->setObjectName(QString::fromUtf8("SpecialControlLabel"));
        SpecialControlLabel->setMinimumSize(QSize(45, 1));

        formLayout->setWidget(12, QFormLayout::LabelRole, SpecialControlLabel);

        groupBox_specialcontrols = new QGroupBox(formLayoutWidget);
        groupBox_specialcontrols->setObjectName(QString::fromUtf8("groupBox_specialcontrols"));
        sizePolicy.setHeightForWidth(groupBox_specialcontrols->sizePolicy().hasHeightForWidth());
        groupBox_specialcontrols->setSizePolicy(sizePolicy);
        groupBox_specialcontrols->setMinimumSize(QSize(700, 400));
        groupBox_specialcontrols->setAutoFillBackground(false);
        groupBox_specialcontrols->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        groupBox_specialcontrols->setFlat(true);
        groupBox_specialcontrols->setCheckable(false);
        specialControlsStackedWidget = new QStackedWidget(groupBox_specialcontrols);
        specialControlsStackedWidget->setObjectName(QString::fromUtf8("specialControlsStackedWidget"));
        specialControlsStackedWidget->setEnabled(true);
        specialControlsStackedWidget->setGeometry(QRect(-10, 0, 700, 600));
        sizePolicy.setHeightForWidth(specialControlsStackedWidget->sizePolicy().hasHeightForWidth());
        specialControlsStackedWidget->setSizePolicy(sizePolicy);
        specialControlsStackedWidget->setMinimumSize(QSize(700, 400));
        specialControlsStackedWidget->setAutoFillBackground(false);
        pageBLANK = new QWidget();
        pageBLANK->setObjectName(QString::fromUtf8("pageBLANK"));
        sizePolicy.setHeightForWidth(pageBLANK->sizePolicy().hasHeightForWidth());
        pageBLANK->setSizePolicy(sizePolicy);
        specialControlsStackedWidget->addWidget(pageBLANK);
        pageCMAQ = new QWidget();
        pageCMAQ->setObjectName(QString::fromUtf8("pageCMAQ"));
        sizePolicy.setHeightForWidth(pageCMAQ->sizePolicy().hasHeightForWidth());
        pageCMAQ->setSizePolicy(sizePolicy);
        pageCMAQ->setMaximumSize(QSize(400, 200));
        gridLayout_13 = new QGridLayout(pageCMAQ);
        gridLayout_13->setSpacing(1);
        gridLayout_13->setContentsMargins(0, 0, 0, 0);
        gridLayout_13->setObjectName(QString::fromUtf8("gridLayout_13"));
        groupBox_cmaq = new QGroupBox(pageCMAQ);
        groupBox_cmaq->setObjectName(QString::fromUtf8("groupBox_cmaq"));
        sizePolicy13.setHeightForWidth(groupBox_cmaq->sizePolicy().hasHeightForWidth());
        groupBox_cmaq->setSizePolicy(sizePolicy13);
        groupBox_cmaq->setMinimumSize(QSize(400, 0));
        groupBox_cmaq->setMaximumSize(QSize(700, 1000));
        groupBox_cmaq->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        gridLayout_15 = new QGridLayout(groupBox_cmaq);
        gridLayout_15->setObjectName(QString::fromUtf8("gridLayout_15"));
        gridLayout_15->setContentsMargins(-1, 0, -1, -1);
        CMAQColLabel = new QLabel(groupBox_cmaq);
        CMAQColLabel->setObjectName(QString::fromUtf8("CMAQColLabel"));

        gridLayout_15->addWidget(CMAQColLabel, 1, 2, 1, 1);

        CMAQLayerLabel = new QLabel(groupBox_cmaq);
        CMAQLayerLabel->setObjectName(QString::fromUtf8("CMAQLayerLabel"));

        gridLayout_15->addWidget(CMAQLayerLabel, 2, 2, 1, 1);

        ShowCMAQLayer = new QCheckBox(groupBox_cmaq);
        ShowCMAQLayer->setObjectName(QString::fromUtf8("ShowCMAQLayer"));

        gridLayout_15->addWidget(ShowCMAQLayer, 2, 0, 1, 1);

        CMAQColSlider = new QSlider(groupBox_cmaq);
        CMAQColSlider->setObjectName(QString::fromUtf8("CMAQColSlider"));
        CMAQColSlider->setMinimum(1);
        CMAQColSlider->setOrientation(Qt::Horizontal);

        gridLayout_15->addWidget(CMAQColSlider, 1, 1, 1, 1);

        CMAQRowLabel = new QLabel(groupBox_cmaq);
        CMAQRowLabel->setObjectName(QString::fromUtf8("CMAQRowLabel"));
        CMAQRowLabel->setAlignment(Qt::AlignJustify|Qt::AlignVCenter);

        gridLayout_15->addWidget(CMAQRowLabel, 0, 2, 1, 1);

        CMAQRowSlider = new QSlider(groupBox_cmaq);
        CMAQRowSlider->setObjectName(QString::fromUtf8("CMAQRowSlider"));
        CMAQRowSlider->setMinimum(1);
        CMAQRowSlider->setOrientation(Qt::Horizontal);

        gridLayout_15->addWidget(CMAQRowSlider, 0, 1, 1, 1);

        flattenCMAQ = new QCheckBox(groupBox_cmaq);
        flattenCMAQ->setObjectName(QString::fromUtf8("flattenCMAQ"));

        gridLayout_15->addWidget(flattenCMAQ, 3, 0, 1, 2);

        ShowCMAQCol = new QCheckBox(groupBox_cmaq);
        ShowCMAQCol->setObjectName(QString::fromUtf8("ShowCMAQCol"));

        gridLayout_15->addWidget(ShowCMAQCol, 1, 0, 1, 1);

        ShowCMAQRow = new QCheckBox(groupBox_cmaq);
        ShowCMAQRow->setObjectName(QString::fromUtf8("ShowCMAQRow"));

        gridLayout_15->addWidget(ShowCMAQRow, 0, 0, 1, 1);

        CMAQLayerSlider = new QSlider(groupBox_cmaq);
        CMAQLayerSlider->setObjectName(QString::fromUtf8("CMAQLayerSlider"));
        CMAQLayerSlider->setMinimum(1);
        CMAQLayerSlider->setOrientation(Qt::Horizontal);

        gridLayout_15->addWidget(CMAQLayerSlider, 2, 1, 1, 1);

        verticalSpacer_10 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_15->addItem(verticalSpacer_10, 4, 0, 1, 1);


        gridLayout_13->addWidget(groupBox_cmaq, 1, 0, 1, 1);

        specialControlsStackedWidget->addWidget(pageCMAQ);
        pageCEILOMETER = new QWidget();
        pageCEILOMETER->setObjectName(QString::fromUtf8("pageCEILOMETER"));
        sizePolicy.setHeightForWidth(pageCEILOMETER->sizePolicy().hasHeightForWidth());
        pageCEILOMETER->setSizePolicy(sizePolicy);
        groupBox_ceilometer = new QGroupBox(pageCEILOMETER);
        groupBox_ceilometer->setObjectName(QString::fromUtf8("groupBox_ceilometer"));
        groupBox_ceilometer->setGeometry(QRect(8, 0, 700, 140));
        sizePolicy.setHeightForWidth(groupBox_ceilometer->sizePolicy().hasHeightForWidth());
        groupBox_ceilometer->setSizePolicy(sizePolicy);
        groupBox_ceilometer->setMinimumSize(QSize(700, 0));
        gridLayout_14 = new QGridLayout(groupBox_ceilometer);
        gridLayout_14->setObjectName(QString::fromUtf8("gridLayout_14"));
        label_33 = new QLabel(groupBox_ceilometer);
        label_33->setObjectName(QString::fromUtf8("label_33"));

        gridLayout_14->addWidget(label_33, 4, 3, 1, 1);

        horizontalSpacer_37 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_14->addItem(horizontalSpacer_37, 0, 6, 1, 1);

        ceilometerSize = new QSlider(groupBox_ceilometer);
        ceilometerSize->setObjectName(QString::fromUtf8("ceilometerSize"));
        sizePolicy5.setHeightForWidth(ceilometerSize->sizePolicy().hasHeightForWidth());
        ceilometerSize->setSizePolicy(sizePolicy5);
        ceilometerSize->setMinimumSize(QSize(150, 0));
        ceilometerSize->setMinimum(100);
        ceilometerSize->setMaximum(500);
        ceilometerSize->setSingleStep(10);
        ceilometerSize->setValue(200);
        ceilometerSize->setOrientation(Qt::Horizontal);

        gridLayout_14->addWidget(ceilometerSize, 4, 1, 1, 2);

        ceilometerShowMixingLayer = new QCheckBox(groupBox_ceilometer);
        ceilometerShowMixingLayer->setObjectName(QString::fromUtf8("ceilometerShowMixingLayer"));
        sizePolicy5.setHeightForWidth(ceilometerShowMixingLayer->sizePolicy().hasHeightForWidth());
        ceilometerShowMixingLayer->setSizePolicy(sizePolicy5);
        ceilometerShowMixingLayer->setCheckable(true);
        ceilometerShowMixingLayer->setChecked(true);
        ceilometerShowMixingLayer->setTristate(false);

        gridLayout_14->addWidget(ceilometerShowMixingLayer, 2, 0, 1, 1);

        ceilometerShowLocations = new QCheckBox(groupBox_ceilometer);
        ceilometerShowLocations->setObjectName(QString::fromUtf8("ceilometerShowLocations"));
        sizePolicy5.setHeightForWidth(ceilometerShowLocations->sizePolicy().hasHeightForWidth());
        ceilometerShowLocations->setSizePolicy(sizePolicy5);
        ceilometerShowLocations->setCheckable(true);
        ceilometerShowLocations->setChecked(true);
        ceilometerShowLocations->setTristate(false);

        gridLayout_14->addWidget(ceilometerShowLocations, 3, 0, 1, 1);

        ceilometerPlotScroller = new QSlider(groupBox_ceilometer);
        ceilometerPlotScroller->setObjectName(QString::fromUtf8("ceilometerPlotScroller"));
        sizePolicy5.setHeightForWidth(ceilometerPlotScroller->sizePolicy().hasHeightForWidth());
        ceilometerPlotScroller->setSizePolicy(sizePolicy5);
        ceilometerPlotScroller->setValue(0);
        ceilometerPlotScroller->setOrientation(Qt::Vertical);

        gridLayout_14->addWidget(ceilometerPlotScroller, 0, 4, 5, 1);

        ceilometerMinimumAltitude = new QDoubleSpinBox(groupBox_ceilometer);
        ceilometerMinimumAltitude->setObjectName(QString::fromUtf8("ceilometerMinimumAltitude"));
        sizePolicy5.setHeightForWidth(ceilometerMinimumAltitude->sizePolicy().hasHeightForWidth());
        ceilometerMinimumAltitude->setSizePolicy(sizePolicy5);
        ceilometerMinimumAltitude->setMinimumSize(QSize(50, 33));
        ceilometerMinimumAltitude->setFocusPolicy(Qt::ClickFocus);
        ceilometerMinimumAltitude->setMinimum(0);
        ceilometerMinimumAltitude->setMaximum(20);
        ceilometerMinimumAltitude->setValue(0);

        gridLayout_14->addWidget(ceilometerMinimumAltitude, 0, 1, 1, 1);

        label_27 = new QLabel(groupBox_ceilometer);
        label_27->setObjectName(QString::fromUtf8("label_27"));
        label_27->setMinimumSize(QSize(85, 0));

        gridLayout_14->addWidget(label_27, 1, 0, 1, 1);

        label_28 = new QLabel(groupBox_ceilometer);
        label_28->setObjectName(QString::fromUtf8("label_28"));
        label_28->setMinimumSize(QSize(85, 0));

        gridLayout_14->addWidget(label_28, 0, 0, 1, 1);

        label_18 = new QLabel(groupBox_ceilometer);
        label_18->setObjectName(QString::fromUtf8("label_18"));

        gridLayout_14->addWidget(label_18, 1, 6, 2, 1);

        ceilometerMaximumAltitude = new QDoubleSpinBox(groupBox_ceilometer);
        ceilometerMaximumAltitude->setObjectName(QString::fromUtf8("ceilometerMaximumAltitude"));
        sizePolicy5.setHeightForWidth(ceilometerMaximumAltitude->sizePolicy().hasHeightForWidth());
        ceilometerMaximumAltitude->setSizePolicy(sizePolicy5);
        ceilometerMaximumAltitude->setMinimumSize(QSize(50, 33));
        ceilometerMaximumAltitude->setFocusPolicy(Qt::ClickFocus);
        ceilometerMaximumAltitude->setMinimum(0.1);
        ceilometerMaximumAltitude->setMaximum(20);
        ceilometerMaximumAltitude->setValue(15);

        gridLayout_14->addWidget(ceilometerMaximumAltitude, 1, 1, 1, 1);

        verticalSpacer_16 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_14->addItem(verticalSpacer_16, 5, 1, 1, 1);

        specialControlsStackedWidget->addWidget(pageCEILOMETER);
        pageNEUBREW = new QWidget();
        pageNEUBREW->setObjectName(QString::fromUtf8("pageNEUBREW"));
        sizePolicy.setHeightForWidth(pageNEUBREW->sizePolicy().hasHeightForWidth());
        pageNEUBREW->setSizePolicy(sizePolicy);
        groupBox_neubrew = new QGroupBox(pageNEUBREW);
        groupBox_neubrew->setObjectName(QString::fromUtf8("groupBox_neubrew"));
        groupBox_neubrew->setGeometry(QRect(12, 4, 465, 130));
        sizePolicy.setHeightForWidth(groupBox_neubrew->sizePolicy().hasHeightForWidth());
        groupBox_neubrew->setSizePolicy(sizePolicy);
        gridLayout_18 = new QGridLayout(groupBox_neubrew);
        gridLayout_18->setObjectName(QString::fromUtf8("gridLayout_18"));
        label_32 = new QLabel(groupBox_neubrew);
        label_32->setObjectName(QString::fromUtf8("label_32"));

        gridLayout_18->addWidget(label_32, 3, 3, 1, 1);

        neubrewShowLocations = new QCheckBox(groupBox_neubrew);
        neubrewShowLocations->setObjectName(QString::fromUtf8("neubrewShowLocations"));
        sizePolicy5.setHeightForWidth(neubrewShowLocations->sizePolicy().hasHeightForWidth());
        neubrewShowLocations->setSizePolicy(sizePolicy5);
        neubrewShowLocations->setMinimumSize(QSize(120, 0));
        neubrewShowLocations->setCheckable(true);
        neubrewShowLocations->setChecked(true);
        neubrewShowLocations->setTristate(false);

        gridLayout_18->addWidget(neubrewShowLocations, 2, 0, 1, 2);

        neubrewSize = new QSlider(groupBox_neubrew);
        neubrewSize->setObjectName(QString::fromUtf8("neubrewSize"));
        sizePolicy5.setHeightForWidth(neubrewSize->sizePolicy().hasHeightForWidth());
        neubrewSize->setSizePolicy(sizePolicy5);
        neubrewSize->setMinimumSize(QSize(200, 0));
        neubrewSize->setMinimum(100);
        neubrewSize->setMaximum(500);
        neubrewSize->setSingleStep(10);
        neubrewSize->setValue(200);
        neubrewSize->setOrientation(Qt::Horizontal);

        gridLayout_18->addWidget(neubrewSize, 3, 0, 1, 3);

        label_30 = new QLabel(groupBox_neubrew);
        label_30->setObjectName(QString::fromUtf8("label_30"));

        gridLayout_18->addWidget(label_30, 0, 5, 3, 1);

        horizontalSpacer_38 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_18->addItem(horizontalSpacer_38, 0, 6, 1, 1);

        neubrewMinimumAltitude = new QDoubleSpinBox(groupBox_neubrew);
        neubrewMinimumAltitude->setObjectName(QString::fromUtf8("neubrewMinimumAltitude"));
        sizePolicy5.setHeightForWidth(neubrewMinimumAltitude->sizePolicy().hasHeightForWidth());
        neubrewMinimumAltitude->setSizePolicy(sizePolicy5);
        neubrewMinimumAltitude->setFocusPolicy(Qt::ClickFocus);
        neubrewMinimumAltitude->setMinimum(0);
        neubrewMinimumAltitude->setMaximum(20);
        neubrewMinimumAltitude->setValue(0);

        gridLayout_18->addWidget(neubrewMinimumAltitude, 0, 1, 1, 1);

        neubrewPlotScroller = new QSlider(groupBox_neubrew);
        neubrewPlotScroller->setObjectName(QString::fromUtf8("neubrewPlotScroller"));
        sizePolicy5.setHeightForWidth(neubrewPlotScroller->sizePolicy().hasHeightForWidth());
        neubrewPlotScroller->setSizePolicy(sizePolicy5);
        neubrewPlotScroller->setValue(0);
        neubrewPlotScroller->setOrientation(Qt::Vertical);

        gridLayout_18->addWidget(neubrewPlotScroller, 0, 4, 4, 1);

        neubrewMaximumAltitude = new QDoubleSpinBox(groupBox_neubrew);
        neubrewMaximumAltitude->setObjectName(QString::fromUtf8("neubrewMaximumAltitude"));
        sizePolicy5.setHeightForWidth(neubrewMaximumAltitude->sizePolicy().hasHeightForWidth());
        neubrewMaximumAltitude->setSizePolicy(sizePolicy5);
        neubrewMaximumAltitude->setFocusPolicy(Qt::ClickFocus);
        neubrewMaximumAltitude->setMinimum(0.1);
        neubrewMaximumAltitude->setMaximum(50);
        neubrewMaximumAltitude->setValue(50);

        gridLayout_18->addWidget(neubrewMaximumAltitude, 1, 1, 1, 1);

        label_31 = new QLabel(groupBox_neubrew);
        label_31->setObjectName(QString::fromUtf8("label_31"));
        label_31->setMinimumSize(QSize(85, 0));

        gridLayout_18->addWidget(label_31, 1, 0, 1, 1);

        label_29 = new QLabel(groupBox_neubrew);
        label_29->setObjectName(QString::fromUtf8("label_29"));
        label_29->setMinimumSize(QSize(100, 0));

        gridLayout_18->addWidget(label_29, 0, 0, 1, 1);

        verticalSpacer_12 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_18->addItem(verticalSpacer_12, 4, 3, 1, 1);

        specialControlsStackedWidget->addWidget(pageNEUBREW);
        pageBEHR = new QWidget();
        pageBEHR->setObjectName(QString::fromUtf8("pageBEHR"));
        sizePolicy.setHeightForWidth(pageBEHR->sizePolicy().hasHeightForWidth());
        pageBEHR->setSizePolicy(sizePolicy);
        groupBox_behr = new QGroupBox(pageBEHR);
        groupBox_behr->setObjectName(QString::fromUtf8("groupBox_behr"));
        groupBox_behr->setGeometry(QRect(12, 4, 700, 40));
        sizePolicy.setHeightForWidth(groupBox_behr->sizePolicy().hasHeightForWidth());
        groupBox_behr->setSizePolicy(sizePolicy);
        groupBox_behr->setMinimumSize(QSize(700, 0));
        gridLayout_29 = new QGridLayout(groupBox_behr);
        gridLayout_29->setObjectName(QString::fromUtf8("gridLayout_29"));
        persistBEHR = new QCheckBox(groupBox_behr);
        persistBEHR->setObjectName(QString::fromUtf8("persistBEHR"));
        sizePolicy5.setHeightForWidth(persistBEHR->sizePolicy().hasHeightForWidth());
        persistBEHR->setSizePolicy(sizePolicy5);

        gridLayout_29->addWidget(persistBEHR, 0, 0, 1, 1);

        persistBEHRHours = new QSpinBox(groupBox_behr);
        persistBEHRHours->setObjectName(QString::fromUtf8("persistBEHRHours"));
        persistBEHRHours->setEnabled(false);
        sizePolicy5.setHeightForWidth(persistBEHRHours->sizePolicy().hasHeightForWidth());
        persistBEHRHours->setSizePolicy(sizePolicy5);
        persistBEHRHours->setMinimum(1);
        persistBEHRHours->setMaximum(24);
        persistBEHRHours->setValue(24);

        gridLayout_29->addWidget(persistBEHRHours, 0, 1, 1, 1);

        horizontalSpacer_40 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_29->addItem(horizontalSpacer_40, 0, 2, 1, 1);

        verticalSpacer_15 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_29->addItem(verticalSpacer_15, 1, 1, 1, 1);

        specialControlsStackedWidget->addWidget(pageBEHR);
        pageMODIS = new QWidget();
        pageMODIS->setObjectName(QString::fromUtf8("pageMODIS"));
        sizePolicy.setHeightForWidth(pageMODIS->sizePolicy().hasHeightForWidth());
        pageMODIS->setSizePolicy(sizePolicy);
        groupBox_modis = new QGroupBox(pageMODIS);
        groupBox_modis->setObjectName(QString::fromUtf8("groupBox_modis"));
        groupBox_modis->setGeometry(QRect(16, 4, 700, 40));
        sizePolicy.setHeightForWidth(groupBox_modis->sizePolicy().hasHeightForWidth());
        groupBox_modis->setSizePolicy(sizePolicy);
        groupBox_modis->setMinimumSize(QSize(700, 0));
        groupBox_modis->setMaximumSize(QSize(700, 16777215));
        gridLayout_27 = new QGridLayout(groupBox_modis);
        gridLayout_27->setObjectName(QString::fromUtf8("gridLayout_27"));
        persistMODIS = new QCheckBox(groupBox_modis);
        persistMODIS->setObjectName(QString::fromUtf8("persistMODIS"));
        sizePolicy5.setHeightForWidth(persistMODIS->sizePolicy().hasHeightForWidth());
        persistMODIS->setSizePolicy(sizePolicy5);

        gridLayout_27->addWidget(persistMODIS, 0, 0, 1, 1);

        persistMODISHours = new QSpinBox(groupBox_modis);
        persistMODISHours->setObjectName(QString::fromUtf8("persistMODISHours"));
        persistMODISHours->setEnabled(false);
        sizePolicy5.setHeightForWidth(persistMODISHours->sizePolicy().hasHeightForWidth());
        persistMODISHours->setSizePolicy(sizePolicy5);
        persistMODISHours->setMinimum(1);
        persistMODISHours->setMaximum(24);
        persistMODISHours->setValue(24);

        gridLayout_27->addWidget(persistMODISHours, 0, 1, 1, 1);

        horizontalSpacer_39 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_27->addItem(horizontalSpacer_39, 0, 2, 1, 1);

        verticalSpacer_13 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_27->addItem(verticalSpacer_13, 1, 1, 1, 1);

        specialControlsStackedWidget->addWidget(pageMODIS);
        pageVIIRS = new QWidget();
        pageVIIRS->setObjectName(QString::fromUtf8("pageVIIRS"));
        sizePolicy.setHeightForWidth(pageVIIRS->sizePolicy().hasHeightForWidth());
        pageVIIRS->setSizePolicy(sizePolicy);
        groupBox_viirs = new QGroupBox(pageVIIRS);
        groupBox_viirs->setObjectName(QString::fromUtf8("groupBox_viirs"));
        groupBox_viirs->setGeometry(QRect(16, 4, 700, 40));
        sizePolicy.setHeightForWidth(groupBox_viirs->sizePolicy().hasHeightForWidth());
        groupBox_viirs->setSizePolicy(sizePolicy);
        groupBox_viirs->setMinimumSize(QSize(700, 0));
        gridLayout_28 = new QGridLayout(groupBox_viirs);
        gridLayout_28->setObjectName(QString::fromUtf8("gridLayout_28"));
        persistVIIRS = new QCheckBox(groupBox_viirs);
        persistVIIRS->setObjectName(QString::fromUtf8("persistVIIRS"));

        gridLayout_28->addWidget(persistVIIRS, 0, 0, 1, 1);

        persistVIIRSHours = new QSpinBox(groupBox_viirs);
        persistVIIRSHours->setObjectName(QString::fromUtf8("persistVIIRSHours"));
        persistVIIRSHours->setEnabled(false);
        persistVIIRSHours->setMinimum(1);
        persistVIIRSHours->setMaximum(24);
        persistVIIRSHours->setValue(24);

        gridLayout_28->addWidget(persistVIIRSHours, 0, 1, 1, 1);

        horizontalSpacer_41 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_28->addItem(horizontalSpacer_41, 0, 2, 1, 1);

        verticalSpacer_14 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_28->addItem(verticalSpacer_14, 1, 0, 1, 1);

        specialControlsStackedWidget->addWidget(pageVIIRS);
        pageTROPOMI = new QWidget();
        pageTROPOMI->setObjectName(QString::fromUtf8("pageTROPOMI"));
        sizePolicy.setHeightForWidth(pageTROPOMI->sizePolicy().hasHeightForWidth());
        pageTROPOMI->setSizePolicy(sizePolicy);
        groupBox_tropomi = new QGroupBox(pageTROPOMI);
        groupBox_tropomi->setObjectName(QString::fromUtf8("groupBox_tropomi"));
        groupBox_tropomi->setGeometry(QRect(16, 0, 700, 40));
        sizePolicy.setHeightForWidth(groupBox_tropomi->sizePolicy().hasHeightForWidth());
        groupBox_tropomi->setSizePolicy(sizePolicy);
        groupBox_tropomi->setMinimumSize(QSize(700, 0));
        gridLayout_5 = new QGridLayout(groupBox_tropomi);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        persistTROPOMI = new QCheckBox(groupBox_tropomi);
        persistTROPOMI->setObjectName(QString::fromUtf8("persistTROPOMI"));
        sizePolicy5.setHeightForWidth(persistTROPOMI->sizePolicy().hasHeightForWidth());
        persistTROPOMI->setSizePolicy(sizePolicy5);

        gridLayout_5->addWidget(persistTROPOMI, 0, 0, 1, 1);

        persistTROPOMIHours = new QSpinBox(groupBox_tropomi);
        persistTROPOMIHours->setObjectName(QString::fromUtf8("persistTROPOMIHours"));
        persistTROPOMIHours->setEnabled(false);
        sizePolicy5.setHeightForWidth(persistTROPOMIHours->sizePolicy().hasHeightForWidth());
        persistTROPOMIHours->setSizePolicy(sizePolicy5);
        persistTROPOMIHours->setMinimum(1);
        persistTROPOMIHours->setMaximum(24);
        persistTROPOMIHours->setValue(24);

        gridLayout_5->addWidget(persistTROPOMIHours, 0, 1, 1, 1);

        horizontalSpacer_42 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_5->addItem(horizontalSpacer_42, 0, 2, 1, 1);

        verticalSpacer_9 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_5->addItem(verticalSpacer_9, 1, 0, 1, 1);

        specialControlsStackedWidget->addWidget(pageTROPOMI);
        pageHMS = new QWidget();
        pageHMS->setObjectName(QString::fromUtf8("pageHMS"));
        sizePolicy.setHeightForWidth(pageHMS->sizePolicy().hasHeightForWidth());
        pageHMS->setSizePolicy(sizePolicy);
        groupBox_hms = new QGroupBox(pageHMS);
        groupBox_hms->setObjectName(QString::fromUtf8("groupBox_hms"));
        groupBox_hms->setGeometry(QRect(12, 4, 700, 40));
        sizePolicy.setHeightForWidth(groupBox_hms->sizePolicy().hasHeightForWidth());
        groupBox_hms->setSizePolicy(sizePolicy);
        groupBox_hms->setMinimumSize(QSize(700, 0));
        gridLayout_26 = new QGridLayout(groupBox_hms);
        gridLayout_26->setObjectName(QString::fromUtf8("gridLayout_26"));
        persistHMSHours = new QSpinBox(groupBox_hms);
        persistHMSHours->setObjectName(QString::fromUtf8("persistHMSHours"));
        persistHMSHours->setEnabled(false);
        sizePolicy5.setHeightForWidth(persistHMSHours->sizePolicy().hasHeightForWidth());
        persistHMSHours->setSizePolicy(sizePolicy5);
        persistHMSHours->setMinimum(1);
        persistHMSHours->setMaximum(24);
        persistHMSHours->setValue(24);

        gridLayout_26->addWidget(persistHMSHours, 0, 1, 1, 1);

        horizontalSpacer_43 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_26->addItem(horizontalSpacer_43, 0, 2, 1, 1);

        persistHMS = new QCheckBox(groupBox_hms);
        persistHMS->setObjectName(QString::fromUtf8("persistHMS"));
        sizePolicy5.setHeightForWidth(persistHMS->sizePolicy().hasHeightForWidth());
        persistHMS->setSizePolicy(sizePolicy5);

        gridLayout_26->addWidget(persistHMS, 0, 0, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_26->addItem(verticalSpacer_2, 1, 1, 1, 1);

        specialControlsStackedWidget->addWidget(pageHMS);
        pagePanel = new QWidget();
        pagePanel->setObjectName(QString::fromUtf8("pagePanel"));
        sizePolicy.setHeightForWidth(pagePanel->sizePolicy().hasHeightForWidth());
        pagePanel->setSizePolicy(sizePolicy);
        groupBox_panel = new QGroupBox(pagePanel);
        groupBox_panel->setObjectName(QString::fromUtf8("groupBox_panel"));
        groupBox_panel->setGeometry(QRect(12, 4, 700, 200));
        sizePolicy.setHeightForWidth(groupBox_panel->sizePolicy().hasHeightForWidth());
        groupBox_panel->setSizePolicy(sizePolicy);
        groupBox_panel->setMinimumSize(QSize(700, 200));
        gridLayout_19 = new QGridLayout(groupBox_panel);
        gridLayout_19->setObjectName(QString::fromUtf8("gridLayout_19"));
        label_34 = new QLabel(groupBox_panel);
        label_34->setObjectName(QString::fromUtf8("label_34"));

        gridLayout_19->addWidget(label_34, 0, 1, 1, 1);

        label_21 = new QLabel(groupBox_panel);
        label_21->setObjectName(QString::fromUtf8("label_21"));

        gridLayout_19->addWidget(label_21, 0, 0, 1, 1);

        panelDisplaySource2 = new QComboBox(groupBox_panel);
        panelDisplaySource2->setObjectName(QString::fromUtf8("panelDisplaySource2"));
        sizePolicy5.setHeightForWidth(panelDisplaySource2->sizePolicy().hasHeightForWidth());
        panelDisplaySource2->setSizePolicy(sizePolicy5);

        gridLayout_19->addWidget(panelDisplaySource2, 1, 1, 1, 1);

        label_20 = new QLabel(groupBox_panel);
        label_20->setObjectName(QString::fromUtf8("label_20"));

        gridLayout_19->addWidget(label_20, 2, 0, 1, 1);

        panelDisplaySource1 = new QComboBox(groupBox_panel);
        panelDisplaySource1->setObjectName(QString::fromUtf8("panelDisplaySource1"));
        sizePolicy5.setHeightForWidth(panelDisplaySource1->sizePolicy().hasHeightForWidth());
        panelDisplaySource1->setSizePolicy(sizePolicy5);

        gridLayout_19->addWidget(panelDisplaySource1, 1, 0, 1, 1);

        panelDisplayListWidget = new QListWidget(groupBox_panel);
        panelDisplayListWidget->setObjectName(QString::fromUtf8("panelDisplayListWidget"));
        sizePolicy5.setHeightForWidth(panelDisplayListWidget->sizePolicy().hasHeightForWidth());
        panelDisplayListWidget->setSizePolicy(sizePolicy5);
        panelDisplayListWidget->setMinimumSize(QSize(0, 200));
        panelDisplayListWidget->setStyleSheet(QString::fromUtf8("padding-bottom:10px"));

        gridLayout_19->addWidget(panelDisplayListWidget, 3, 0, 1, 2);

        horizontalSpacer_44 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_19->addItem(horizontalSpacer_44, 1, 2, 1, 1);

        verticalSpacer_11 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_19->addItem(verticalSpacer_11, 4, 0, 1, 1);

        specialControlsStackedWidget->addWidget(pagePanel);
        pagePandora = new QWidget();
        pagePandora->setObjectName(QString::fromUtf8("pagePandora"));
        sizePolicy.setHeightForWidth(pagePandora->sizePolicy().hasHeightForWidth());
        pagePandora->setSizePolicy(sizePolicy);
        groupBox_pandora = new QGroupBox(pagePandora);
        groupBox_pandora->setObjectName(QString::fromUtf8("groupBox_pandora"));
        groupBox_pandora->setGeometry(QRect(12, 4, 700, 50));
        sizePolicy.setHeightForWidth(groupBox_pandora->sizePolicy().hasHeightForWidth());
        groupBox_pandora->setSizePolicy(sizePolicy);
        groupBox_pandora->setMinimumSize(QSize(700, 50));
        gridLayout_20 = new QGridLayout(groupBox_pandora);
        gridLayout_20->setObjectName(QString::fromUtf8("gridLayout_20"));
        pandoraPlotScroller = new QSlider(groupBox_pandora);
        pandoraPlotScroller->setObjectName(QString::fromUtf8("pandoraPlotScroller"));
        sizePolicy5.setHeightForWidth(pandoraPlotScroller->sizePolicy().hasHeightForWidth());
        pandoraPlotScroller->setSizePolicy(sizePolicy5);
        pandoraPlotScroller->setValue(0);
        pandoraPlotScroller->setOrientation(Qt::Vertical);

        gridLayout_20->addWidget(pandoraPlotScroller, 0, 4, 4, 1);

        pandoraMinimumAltitude = new QDoubleSpinBox(groupBox_pandora);
        pandoraMinimumAltitude->setObjectName(QString::fromUtf8("pandoraMinimumAltitude"));
        sizePolicy.setHeightForWidth(pandoraMinimumAltitude->sizePolicy().hasHeightForWidth());
        pandoraMinimumAltitude->setSizePolicy(sizePolicy);
        pandoraMinimumAltitude->setFocusPolicy(Qt::ClickFocus);
        pandoraMinimumAltitude->setMinimum(0);
        pandoraMinimumAltitude->setMaximum(20);
        pandoraMinimumAltitude->setValue(0);

        gridLayout_20->addWidget(pandoraMinimumAltitude, 0, 2, 1, 2);

        label_38 = new QLabel(groupBox_pandora);
        label_38->setObjectName(QString::fromUtf8("label_38"));

        gridLayout_20->addWidget(label_38, 3, 3, 1, 1);

        pandoraMaximumAltitude = new QDoubleSpinBox(groupBox_pandora);
        pandoraMaximumAltitude->setObjectName(QString::fromUtf8("pandoraMaximumAltitude"));
        sizePolicy.setHeightForWidth(pandoraMaximumAltitude->sizePolicy().hasHeightForWidth());
        pandoraMaximumAltitude->setSizePolicy(sizePolicy);
        pandoraMaximumAltitude->setFocusPolicy(Qt::ClickFocus);
        pandoraMaximumAltitude->setMinimum(0.1);
        pandoraMaximumAltitude->setMaximum(20);
        pandoraMaximumAltitude->setValue(5);

        gridLayout_20->addWidget(pandoraMaximumAltitude, 1, 2, 1, 2);

        label_37 = new QLabel(groupBox_pandora);
        label_37->setObjectName(QString::fromUtf8("label_37"));
        label_37->setMinimumSize(QSize(85, 0));

        gridLayout_20->addWidget(label_37, 1, 0, 1, 1);

        label_35 = new QLabel(groupBox_pandora);
        label_35->setObjectName(QString::fromUtf8("label_35"));
        label_35->setMinimumSize(QSize(85, 0));

        gridLayout_20->addWidget(label_35, 0, 0, 1, 1);

        horizontalSpacer_45 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_20->addItem(horizontalSpacer_45, 0, 6, 1, 1);

        label_36 = new QLabel(groupBox_pandora);
        label_36->setObjectName(QString::fromUtf8("label_36"));

        gridLayout_20->addWidget(label_36, 0, 5, 3, 1);

        pandoraSize = new QSlider(groupBox_pandora);
        pandoraSize->setObjectName(QString::fromUtf8("pandoraSize"));
        sizePolicy5.setHeightForWidth(pandoraSize->sizePolicy().hasHeightForWidth());
        pandoraSize->setSizePolicy(sizePolicy5);
        pandoraSize->setMinimum(100);
        pandoraSize->setMaximum(500);
        pandoraSize->setSingleStep(10);
        pandoraSize->setValue(200);
        pandoraSize->setOrientation(Qt::Horizontal);

        gridLayout_20->addWidget(pandoraSize, 3, 0, 1, 3);

        pandoraShowLocations = new QCheckBox(groupBox_pandora);
        pandoraShowLocations->setObjectName(QString::fromUtf8("pandoraShowLocations"));
        sizePolicy5.setHeightForWidth(pandoraShowLocations->sizePolicy().hasHeightForWidth());
        pandoraShowLocations->setSizePolicy(sizePolicy5);
        pandoraShowLocations->setCheckable(true);
        pandoraShowLocations->setChecked(true);
        pandoraShowLocations->setTristate(false);

        gridLayout_20->addWidget(pandoraShowLocations, 2, 0, 1, 1);

        specialControlsStackedWidget->addWidget(pagePandora);
        pageTEMPO = new QWidget();
        pageTEMPO->setObjectName(QString::fromUtf8("pageTEMPO"));
        sizePolicy.setHeightForWidth(pageTEMPO->sizePolicy().hasHeightForWidth());
        pageTEMPO->setSizePolicy(sizePolicy);
        groupBox_tempo = new QGroupBox(pageTEMPO);
        groupBox_tempo->setObjectName(QString::fromUtf8("groupBox_tempo"));
        groupBox_tempo->setGeometry(QRect(12, 0, 700, 53));
        sizePolicy.setHeightForWidth(groupBox_tempo->sizePolicy().hasHeightForWidth());
        groupBox_tempo->setSizePolicy(sizePolicy);
        groupBox_tempo->setMinimumSize(QSize(700, 0));
        horizontalLayout_16 = new QHBoxLayout(groupBox_tempo);
        horizontalLayout_16->setObjectName(QString::fromUtf8("horizontalLayout_16"));
        persistTEMPO = new QCheckBox(groupBox_tempo);
        persistTEMPO->setObjectName(QString::fromUtf8("persistTEMPO"));
        persistTEMPO->setEnabled(false);

        horizontalLayout_16->addWidget(persistTEMPO);

        persistTEMPOHours = new QSpinBox(groupBox_tempo);
        persistTEMPOHours->setObjectName(QString::fromUtf8("persistTEMPOHours"));
        persistTEMPOHours->setEnabled(false);
        persistTEMPOHours->setMinimum(0);
        persistTEMPOHours->setMaximum(24);
        persistTEMPOHours->setValue(0);

        horizontalLayout_16->addWidget(persistTEMPOHours);

        horizontalSpacer_46 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_16->addItem(horizontalSpacer_46);

        specialControlsStackedWidget->addWidget(pageTEMPO);

        formLayout->setWidget(12, QFormLayout::FieldRole, groupBox_specialcontrols);

        groupBox_visibility->raise();
        label_mode->raise();
        vizVarCombobox->raise();
        label_opacity->raise();
        label_minmax->raise();
        groupBox_minmax->raise();
        label_scalefactor->raise();
        groupBox_mode->raise();
        groupBox_scalefactor->raise();
        groupBox_opacity->raise();
        groupBox_specialcontrols->raise();
        label_threshold->raise();
        groupBox_threshold->raise();
        SpecialControlLabel->raise();
        groupBox_ruler->raise();
        label_ruler->raise();
        groupBox_windcontrols->raise();
        WindLabel->raise();
        groupBox_colormap->raise();
        label_colormap->raise();
        tabWidget->addTab(propTab, QString());
        viewTab = new QWidget();
        viewTab->setObjectName(QString::fromUtf8("viewTab"));
        viewTab->setEnabled(true);
        sizePolicy2.setHeightForWidth(viewTab->sizePolicy().hasHeightForWidth());
        viewTab->setSizePolicy(sizePolicy2);
        tabWidget->addTab(viewTab, QString());
        saveTab = new QWidget();
        saveTab->setObjectName(QString::fromUtf8("saveTab"));
        sizePolicy3.setHeightForWidth(saveTab->sizePolicy().hasHeightForWidth());
        saveTab->setSizePolicy(sizePolicy3);
        groupBox_savetab = new QGroupBox(saveTab);
        groupBox_savetab->setObjectName(QString::fromUtf8("groupBox_savetab"));
        groupBox_savetab->setGeometry(QRect(0, 4, 800, 700));
        sizePolicy.setHeightForWidth(groupBox_savetab->sizePolicy().hasHeightForWidth());
        groupBox_savetab->setSizePolicy(sizePolicy);
        groupBox_savetab->setMinimumSize(QSize(800, 0));
        verticalLayout_2 = new QVBoxLayout(groupBox_savetab);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(-1, 0, -1, 0);
        groupBox_saveformats = new QGroupBox(groupBox_savetab);
        groupBox_saveformats->setObjectName(QString::fromUtf8("groupBox_saveformats"));
        sizePolicy.setHeightForWidth(groupBox_saveformats->sizePolicy().hasHeightForWidth());
        groupBox_saveformats->setSizePolicy(sizePolicy);
        groupBox_saveformats->setMinimumSize(QSize(800, 0));
        groupBox_saveformats->setFlat(true);
        gridLayout_21 = new QGridLayout(groupBox_saveformats);
        gridLayout_21->setObjectName(QString::fromUtf8("gridLayout_21"));
        gridLayout_21->setVerticalSpacing(3);
        gridLayout_21->setContentsMargins(-1, 0, -1, 0);
        label_5 = new QLabel(groupBox_saveformats);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        sizePolicy5.setHeightForWidth(label_5->sizePolicy().hasHeightForWidth());
        label_5->setSizePolicy(sizePolicy5);
        label_5->setMinimumSize(QSize(0, 40));
        label_5->setTextFormat(Qt::RichText);

        gridLayout_21->addWidget(label_5, 0, 0, 1, 1);

        horizontalSpacer_49 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_21->addItem(horizontalSpacer_49, 10, 2, 1, 1);

        netcdfioapiRadioButton = new QRadioButton(groupBox_saveformats);
        netcdfioapiRadioButton->setObjectName(QString::fromUtf8("netcdfioapiRadioButton"));
        netcdfioapiRadioButton->setEnabled(true);
        sizePolicy5.setHeightForWidth(netcdfioapiRadioButton->sizePolicy().hasHeightForWidth());
        netcdfioapiRadioButton->setSizePolicy(sizePolicy5);

        gridLayout_21->addWidget(netcdfioapiRadioButton, 5, 0, 1, 2);

        asciisheetRadioButton = new QRadioButton(groupBox_saveformats);
        asciisheetRadioButton->setObjectName(QString::fromUtf8("asciisheetRadioButton"));
        sizePolicy5.setHeightForWidth(asciisheetRadioButton->sizePolicy().hasHeightForWidth());
        asciisheetRadioButton->setSizePolicy(sizePolicy5);

        gridLayout_21->addWidget(asciisheetRadioButton, 3, 0, 1, 1);

        scenarioRadioButton = new QRadioButton(groupBox_saveformats);
        scenarioRadioButton->setObjectName(QString::fromUtf8("scenarioRadioButton"));
        sizePolicy5.setHeightForWidth(scenarioRadioButton->sizePolicy().hasHeightForWidth());
        scenarioRadioButton->setSizePolicy(sizePolicy5);

        gridLayout_21->addWidget(scenarioRadioButton, 1, 0, 1, 1);

        originalFileRadioButton = new QRadioButton(groupBox_saveformats);
        originalFileRadioButton->setObjectName(QString::fromUtf8("originalFileRadioButton"));
        originalFileRadioButton->setEnabled(true);
        sizePolicy5.setHeightForWidth(originalFileRadioButton->sizePolicy().hasHeightForWidth());
        originalFileRadioButton->setSizePolicy(sizePolicy5);

        gridLayout_21->addWidget(originalFileRadioButton, 8, 0, 1, 2);

        mpegRadioButton = new QRadioButton(groupBox_saveformats);
        mpegRadioButton->setObjectName(QString::fromUtf8("mpegRadioButton"));
        sizePolicy5.setHeightForWidth(mpegRadioButton->sizePolicy().hasHeightForWidth());
        mpegRadioButton->setSizePolicy(sizePolicy5);

        gridLayout_21->addWidget(mpegRadioButton, 14, 0, 1, 1);

        shapeRadioButton = new QRadioButton(groupBox_saveformats);
        shapeRadioButton->setObjectName(QString::fromUtf8("shapeRadioButton"));
        shapeRadioButton->setEnabled(false);
        sizePolicy5.setHeightForWidth(shapeRadioButton->sizePolicy().hasHeightForWidth());
        shapeRadioButton->setSizePolicy(sizePolicy5);

        gridLayout_21->addWidget(shapeRadioButton, 7, 0, 1, 2);

        geotiffRadioButton = new QRadioButton(groupBox_saveformats);
        geotiffRadioButton->setObjectName(QString::fromUtf8("geotiffRadioButton"));
        geotiffRadioButton->setEnabled(false);
        sizePolicy5.setHeightForWidth(geotiffRadioButton->sizePolicy().hasHeightForWidth());
        geotiffRadioButton->setSizePolicy(sizePolicy5);

        gridLayout_21->addWidget(geotiffRadioButton, 6, 0, 1, 2);

        allpngsRadioButton = new QRadioButton(groupBox_saveformats);
        allpngsRadioButton->setObjectName(QString::fromUtf8("allpngsRadioButton"));
        sizePolicy5.setHeightForWidth(allpngsRadioButton->sizePolicy().hasHeightForWidth());
        allpngsRadioButton->setSizePolicy(sizePolicy5);

        gridLayout_21->addWidget(allpngsRadioButton, 13, 0, 1, 2);

        highrespngRadioButton = new QRadioButton(groupBox_saveformats);
        highrespngRadioButton->setObjectName(QString::fromUtf8("highrespngRadioButton"));
        highrespngRadioButton->setEnabled(false);
        sizePolicy5.setHeightForWidth(highrespngRadioButton->sizePolicy().hasHeightForWidth());
        highrespngRadioButton->setSizePolicy(sizePolicy5);

        gridLayout_21->addWidget(highrespngRadioButton, 12, 0, 1, 2);

        binaryxdrRadioButton = new QRadioButton(groupBox_saveformats);
        binaryxdrRadioButton->setObjectName(QString::fromUtf8("binaryxdrRadioButton"));
        sizePolicy5.setHeightForWidth(binaryxdrRadioButton->sizePolicy().hasHeightForWidth());
        binaryxdrRadioButton->setSizePolicy(sizePolicy5);

        gridLayout_21->addWidget(binaryxdrRadioButton, 2, 0, 1, 1);

        currentpngRadioButton = new QRadioButton(groupBox_saveformats);
        currentpngRadioButton->setObjectName(QString::fromUtf8("currentpngRadioButton"));
        sizePolicy5.setHeightForWidth(currentpngRadioButton->sizePolicy().hasHeightForWidth());
        currentpngRadioButton->setSizePolicy(sizePolicy5);

        gridLayout_21->addWidget(currentpngRadioButton, 11, 0, 1, 1);

        label_6 = new QLabel(groupBox_saveformats);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        sizePolicy5.setHeightForWidth(label_6->sizePolicy().hasHeightForWidth());
        label_6->setSizePolicy(sizePolicy5);
        label_6->setMinimumSize(QSize(0, 40));
        label_6->setTextFormat(Qt::RichText);

        gridLayout_21->addWidget(label_6, 9, 0, 1, 1);

        netcdfcoardsRadioButton = new QRadioButton(groupBox_saveformats);
        netcdfcoardsRadioButton->setObjectName(QString::fromUtf8("netcdfcoardsRadioButton"));
        sizePolicy5.setHeightForWidth(netcdfcoardsRadioButton->sizePolicy().hasHeightForWidth());
        netcdfcoardsRadioButton->setSizePolicy(sizePolicy5);

        gridLayout_21->addWidget(netcdfcoardsRadioButton, 4, 0, 1, 1);

        kmlRadioButton = new QRadioButton(groupBox_saveformats);
        kmlRadioButton->setObjectName(QString::fromUtf8("kmlRadioButton"));
        sizePolicy5.setHeightForWidth(kmlRadioButton->sizePolicy().hasHeightForWidth());
        kmlRadioButton->setSizePolicy(sizePolicy5);

        gridLayout_21->addWidget(kmlRadioButton, 10, 0, 1, 1);

        screenRecordRadioButton = new QRadioButton(groupBox_saveformats);
        screenRecordRadioButton->setObjectName(QString::fromUtf8("screenRecordRadioButton"));
        screenRecordRadioButton->setEnabled(false);
        sizePolicy5.setHeightForWidth(screenRecordRadioButton->sizePolicy().hasHeightForWidth());
        screenRecordRadioButton->setSizePolicy(sizePolicy5);

        gridLayout_21->addWidget(screenRecordRadioButton, 15, 0, 1, 1);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_21->addItem(verticalSpacer_3, 16, 0, 1, 1);

        geotiffrgbRadioButton = new QRadioButton(groupBox_saveformats);
        geotiffrgbRadioButton->setObjectName(QString::fromUtf8("geotiffrgbRadioButton"));
        geotiffrgbRadioButton->setEnabled(false);
        sizePolicy5.setHeightForWidth(geotiffrgbRadioButton->sizePolicy().hasHeightForWidth());
        geotiffrgbRadioButton->setSizePolicy(sizePolicy5);

        gridLayout_21->addWidget(geotiffrgbRadioButton, 10, 1, 1, 1);


        verticalLayout_2->addWidget(groupBox_saveformats);

        groupBox_saveoptions = new QGroupBox(groupBox_savetab);
        groupBox_saveoptions->setObjectName(QString::fromUtf8("groupBox_saveoptions"));
        sizePolicy.setHeightForWidth(groupBox_saveoptions->sizePolicy().hasHeightForWidth());
        groupBox_saveoptions->setSizePolicy(sizePolicy);
        groupBox_saveoptions->setMinimumSize(QSize(800, 0));
        groupBox_saveoptions->setTitle(QString::fromUtf8(""));
        groupBox_saveoptions->setFlat(true);
        gridLayout_23 = new QGridLayout(groupBox_saveoptions);
        gridLayout_23->setObjectName(QString::fromUtf8("gridLayout_23"));
        gridLayout_23->setVerticalSpacing(3);
        gridLayout_23->setContentsMargins(-1, 0, -1, 0);
        gzipCheckBox = new QCheckBox(groupBox_saveoptions);
        gzipCheckBox->setObjectName(QString::fromUtf8("gzipCheckBox"));
        gzipCheckBox->setEnabled(false);
        sizePolicy5.setHeightForWidth(gzipCheckBox->sizePolicy().hasHeightForWidth());
        gzipCheckBox->setSizePolicy(sizePolicy5);

        gridLayout_23->addWidget(gzipCheckBox, 1, 0, 1, 1);

        label_7 = new QLabel(groupBox_saveoptions);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setTextFormat(Qt::RichText);

        gridLayout_23->addWidget(label_7, 0, 0, 1, 1);

        saveMetadataCheckBox = new QCheckBox(groupBox_saveoptions);
        saveMetadataCheckBox->setObjectName(QString::fromUtf8("saveMetadataCheckBox"));
        saveMetadataCheckBox->setEnabled(true);
        sizePolicy5.setHeightForWidth(saveMetadataCheckBox->sizePolicy().hasHeightForWidth());
        saveMetadataCheckBox->setSizePolicy(sizePolicy5);

        gridLayout_23->addWidget(saveMetadataCheckBox, 2, 0, 1, 1);

        horizontalSpacer_48 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_23->addItem(horizontalSpacer_48, 1, 1, 1, 1);


        verticalLayout_2->addWidget(groupBox_saveoptions);

        groupBox_savefile = new QGroupBox(groupBox_savetab);
        groupBox_savefile->setObjectName(QString::fromUtf8("groupBox_savefile"));
        sizePolicy.setHeightForWidth(groupBox_savefile->sizePolicy().hasHeightForWidth());
        groupBox_savefile->setSizePolicy(sizePolicy);
        groupBox_savefile->setMinimumSize(QSize(800, 0));
        gridLayout_22 = new QGridLayout(groupBox_savefile);
        gridLayout_22->setObjectName(QString::fromUtf8("gridLayout_22"));
        writetofileToolButton = new QToolButton(groupBox_savefile);
        writetofileToolButton->setObjectName(QString::fromUtf8("writetofileToolButton"));
        sizePolicy5.setHeightForWidth(writetofileToolButton->sizePolicy().hasHeightForWidth());
        writetofileToolButton->setSizePolicy(sizePolicy5);
        writetofileToolButton->setMinimumSize(QSize(72, 30));
        writetofileToolButton->setMaximumSize(QSize(200, 20));

        gridLayout_22->addWidget(writetofileToolButton, 0, 0, 1, 1);

        label_26 = new QLabel(groupBox_savefile);
        label_26->setObjectName(QString::fromUtf8("label_26"));

        gridLayout_22->addWidget(label_26, 1, 0, 1, 1);

        horizontalSpacer_47 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_22->addItem(horizontalSpacer_47, 1, 5, 1, 1);

        filename_generated = new QLabel(groupBox_savefile);
        filename_generated->setObjectName(QString::fromUtf8("filename_generated"));

        gridLayout_22->addWidget(filename_generated, 1, 4, 1, 1);

        filenameLineEdit = new QLineEdit(groupBox_savefile);
        filenameLineEdit->setObjectName(QString::fromUtf8("filenameLineEdit"));
        filenameLineEdit->setEnabled(true);
        filenameLineEdit->setMinimumSize(QSize(300, 35));
        filenameLineEdit->setMaximumSize(QSize(16777215, 22));

        gridLayout_22->addWidget(filenameLineEdit, 1, 1, 1, 1);

        dirnameLineEdit = new QLineEdit(groupBox_savefile);
        dirnameLineEdit->setObjectName(QString::fromUtf8("dirnameLineEdit"));
        dirnameLineEdit->setEnabled(false);
        dirnameLineEdit->setMinimumSize(QSize(400, 35));
        dirnameLineEdit->setMaximumSize(QSize(500, 22));

        gridLayout_22->addWidget(dirnameLineEdit, 0, 1, 1, 1);

        savevideoPushButton = new QPushButton(groupBox_savefile);
        savevideoPushButton->setObjectName(QString::fromUtf8("savevideoPushButton"));
        savevideoPushButton->setEnabled(false);
        sizePolicy5.setHeightForWidth(savevideoPushButton->sizePolicy().hasHeightForWidth());
        savevideoPushButton->setSizePolicy(sizePolicy5);
        savevideoPushButton->setMinimumSize(QSize(130, 28));
        savevideoPushButton->setMaximumSize(QSize(130, 28));

        gridLayout_22->addWidget(savevideoPushButton, 3, 1, 1, 1);

        savefilePushButton = new QPushButton(groupBox_savefile);
        savefilePushButton->setObjectName(QString::fromUtf8("savefilePushButton"));
        sizePolicy5.setHeightForWidth(savefilePushButton->sizePolicy().hasHeightForWidth());
        savefilePushButton->setSizePolicy(sizePolicy5);
        savefilePushButton->setMinimumSize(QSize(125, 28));
        savefilePushButton->setMaximumSize(QSize(200, 28));

        gridLayout_22->addWidget(savefilePushButton, 3, 0, 1, 1);

        verticalSpacer_5 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_22->addItem(verticalSpacer_5, 4, 0, 1, 1);


        verticalLayout_2->addWidget(groupBox_savefile);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_4);

        tabWidget->addTab(saveTab, QString());
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        sizePolicy3.setHeightForWidth(tab->sizePolicy().hasHeightForWidth());
        tab->setSizePolicy(sizePolicy3);
        groupBox_rsigweb = new QGroupBox(tab);
        groupBox_rsigweb->setObjectName(QString::fromUtf8("groupBox_rsigweb"));
        groupBox_rsigweb->setGeometry(QRect(0, 4, 800, 700));
        sizePolicy.setHeightForWidth(groupBox_rsigweb->sizePolicy().hasHeightForWidth());
        groupBox_rsigweb->setSizePolicy(sizePolicy);
        groupBox_rsigweb->setMinimumSize(QSize(800, 0));
        gridLayout_24 = new QGridLayout(groupBox_rsigweb);
        gridLayout_24->setObjectName(QString::fromUtf8("gridLayout_24"));
        verticalSpacer_6 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_24->addItem(verticalSpacer_6, 9, 0, 1, 1);

        tipsPushButton = new QPushButton(groupBox_rsigweb);
        tipsPushButton->setObjectName(QString::fromUtf8("tipsPushButton"));
        sizePolicy5.setHeightForWidth(tipsPushButton->sizePolicy().hasHeightForWidth());
        tipsPushButton->setSizePolicy(sizePolicy5);

        gridLayout_24->addWidget(tipsPushButton, 3, 0, 1, 1);

        datausePushButton = new QPushButton(groupBox_rsigweb);
        datausePushButton->setObjectName(QString::fromUtf8("datausePushButton"));
        sizePolicy5.setHeightForWidth(datausePushButton->sizePolicy().hasHeightForWidth());
        datausePushButton->setSizePolicy(sizePolicy5);

        gridLayout_24->addWidget(datausePushButton, 4, 0, 1, 1);

        openwebPushButton = new QPushButton(groupBox_rsigweb);
        openwebPushButton->setObjectName(QString::fromUtf8("openwebPushButton"));
        sizePolicy5.setHeightForWidth(openwebPushButton->sizePolicy().hasHeightForWidth());
        openwebPushButton->setSizePolicy(sizePolicy5);
        openwebPushButton->setMinimumSize(QSize(0, 0));
        openwebPushButton->setMaximumSize(QSize(16777215, 0));

        gridLayout_24->addWidget(openwebPushButton, 6, 0, 1, 1);

        emailPushButton = new QPushButton(groupBox_rsigweb);
        emailPushButton->setObjectName(QString::fromUtf8("emailPushButton"));
        sizePolicy5.setHeightForWidth(emailPushButton->sizePolicy().hasHeightForWidth());
        emailPushButton->setSizePolicy(sizePolicy5);
        emailPushButton->setMinimumSize(QSize(0, 0));
        emailPushButton->setMaximumSize(QSize(16777215, 100));

        gridLayout_24->addWidget(emailPushButton, 7, 0, 1, 1);

        downloaderPushButton = new QPushButton(groupBox_rsigweb);
        downloaderPushButton->setObjectName(QString::fromUtf8("downloaderPushButton"));
        sizePolicy5.setHeightForWidth(downloaderPushButton->sizePolicy().hasHeightForWidth());
        downloaderPushButton->setSizePolicy(sizePolicy5);

        gridLayout_24->addWidget(downloaderPushButton, 5, 0, 1, 1);

        exitPushButton = new QPushButton(groupBox_rsigweb);
        exitPushButton->setObjectName(QString::fromUtf8("exitPushButton"));
        sizePolicy5.setHeightForWidth(exitPushButton->sizePolicy().hasHeightForWidth());
        exitPushButton->setSizePolicy(sizePolicy5);
        exitPushButton->setMinimumSize(QSize(0, 0));
        exitPushButton->setMaximumSize(QSize(16777215, 100));

        gridLayout_24->addWidget(exitPushButton, 8, 0, 1, 1);

        label_3 = new QLabel(groupBox_rsigweb);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout_24->addWidget(label_3, 0, 0, 1, 1);

        rsigTextBrowser = new QTextBrowser(groupBox_rsigweb);
        rsigTextBrowser->setObjectName(QString::fromUtf8("rsigTextBrowser"));
        rsigTextBrowser->setEnabled(true);
        sizePolicy5.setHeightForWidth(rsigTextBrowser->sizePolicy().hasHeightForWidth());
        rsigTextBrowser->setSizePolicy(sizePolicy5);
        rsigTextBrowser->setMinimumSize(QSize(0, 325));
        rsigTextBrowser->setAutoFillBackground(false);
        rsigTextBrowser->setFrameShadow(QFrame::Raised);

        gridLayout_24->addWidget(rsigTextBrowser, 2, 0, 1, 1);

        horizontalSpacer_50 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_24->addItem(horizontalSpacer_50, 2, 1, 1, 1);

        groupBox_2 = new QGroupBox(groupBox_rsigweb);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        sizePolicy.setHeightForWidth(groupBox_2->sizePolicy().hasHeightForWidth());
        groupBox_2->setSizePolicy(sizePolicy);
        groupBox_2->setMinimumSize(QSize(0, 32));
        groupBox_2->setMaximumSize(QSize(16777215, 50));
        checknowPushButton = new QPushButton(groupBox_2);
        checknowPushButton->setObjectName(QString::fromUtf8("checknowPushButton"));
        checknowPushButton->setGeometry(QRect(12, 16, 200, 30));
        sizePolicy5.setHeightForWidth(checknowPushButton->sizePolicy().hasHeightForWidth());
        checknowPushButton->setSizePolicy(sizePolicy5);
        checknowPushButton->setMinimumSize(QSize(0, 30));
        checknowPushButton->setMaximumSize(QSize(16777215, 24));

        gridLayout_24->addWidget(groupBox_2, 1, 0, 1, 1);

        tabWidget->addTab(tab, QString());

        mainVerticalLayout->addWidget(tabWidget);

        groupBox_lowerControl = new QGroupBox(layoutWidget);
        groupBox_lowerControl->setObjectName(QString::fromUtf8("groupBox_lowerControl"));
        QSizePolicy sizePolicy14(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy14.setHorizontalStretch(0);
        sizePolicy14.setVerticalStretch(0);
        sizePolicy14.setHeightForWidth(groupBox_lowerControl->sizePolicy().hasHeightForWidth());
        groupBox_lowerControl->setSizePolicy(sizePolicy14);
        verticalLayout = new QVBoxLayout(groupBox_lowerControl);
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        groupBox_timestep = new QGroupBox(groupBox_lowerControl);
        groupBox_timestep->setObjectName(QString::fromUtf8("groupBox_timestep"));
        groupBox_timestep->setMaximumSize(QSize(700, 16777215));
        gridLayout_8 = new QGridLayout(groupBox_timestep);
        gridLayout_8->setObjectName(QString::fromUtf8("gridLayout_8"));
        gridLayout_8->setContentsMargins(-1, 0, -1, 0);
        backButton24 = new QToolButton(groupBox_timestep);
        backButton24->setObjectName(QString::fromUtf8("backButton24"));
        sizePolicy.setHeightForWidth(backButton24->sizePolicy().hasHeightForWidth());
        backButton24->setSizePolicy(sizePolicy);
        backButton24->setArrowType(Qt::NoArrow);

        gridLayout_8->addWidget(backButton24, 2, 0, 1, 1);

        backButton = new QToolButton(groupBox_timestep);
        backButton->setObjectName(QString::fromUtf8("backButton"));
        sizePolicy.setHeightForWidth(backButton->sizePolicy().hasHeightForWidth());
        backButton->setSizePolicy(sizePolicy);
        backButton->setToolButtonStyle(Qt::ToolButtonFollowStyle);
        backButton->setArrowType(Qt::LeftArrow);

        gridLayout_8->addWidget(backButton, 2, 1, 1, 1);

        label_9 = new QLabel(groupBox_timestep);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        sizePolicy4.setHeightForWidth(label_9->sizePolicy().hasHeightForWidth());
        label_9->setSizePolicy(sizePolicy4);

        gridLayout_8->addWidget(label_9, 0, 4, 1, 1);

        playButton = new QPushButton(groupBox_timestep);
        playButton->setObjectName(QString::fromUtf8("playButton"));
        sizePolicy.setHeightForWidth(playButton->sizePolicy().hasHeightForWidth());
        playButton->setSizePolicy(sizePolicy);
        playButton->setCheckable(false);

        gridLayout_8->addWidget(playButton, 2, 2, 1, 1);

        forwardButton = new QToolButton(groupBox_timestep);
        forwardButton->setObjectName(QString::fromUtf8("forwardButton"));
        sizePolicy.setHeightForWidth(forwardButton->sizePolicy().hasHeightForWidth());
        forwardButton->setSizePolicy(sizePolicy);
        forwardButton->setToolButtonStyle(Qt::ToolButtonFollowStyle);
        forwardButton->setArrowType(Qt::RightArrow);

        gridLayout_8->addWidget(forwardButton, 2, 3, 1, 1);

        forwardButton24 = new QToolButton(groupBox_timestep);
        forwardButton24->setObjectName(QString::fromUtf8("forwardButton24"));
        sizePolicy.setHeightForWidth(forwardButton24->sizePolicy().hasHeightForWidth());
        forwardButton24->setSizePolicy(sizePolicy);
        forwardButton24->setArrowType(Qt::NoArrow);

        gridLayout_8->addWidget(forwardButton24, 2, 4, 1, 1);

        timestepSlider = new QSlider(groupBox_timestep);
        timestepSlider->setObjectName(QString::fromUtf8("timestepSlider"));
        timestepSlider->setOrientation(Qt::Horizontal);

        gridLayout_8->addWidget(timestepSlider, 1, 0, 1, 4);

        label_timestep = new QLabel(groupBox_timestep);
        label_timestep->setObjectName(QString::fromUtf8("label_timestep"));
        sizePolicy4.setHeightForWidth(label_timestep->sizePolicy().hasHeightForWidth());
        label_timestep->setSizePolicy(sizePolicy4);
        QFont font5;
        font5.setPointSize(13);
        font5.setBold(true);
        font5.setWeight(75);
        label_timestep->setFont(font5);
        label_timestep->setTextFormat(Qt::AutoText);
        label_timestep->setAlignment(Qt::AlignCenter);

        gridLayout_8->addWidget(label_timestep, 0, 1, 1, 2);

        animationSpeed = new QSpinBox(groupBox_timestep);
        animationSpeed->setObjectName(QString::fromUtf8("animationSpeed"));
        animationSpeed->setMinimumSize(QSize(0, 30));
        animationSpeed->setFocusPolicy(Qt::ClickFocus);
        animationSpeed->setMinimum(1);
        animationSpeed->setMaximum(20);
        animationSpeed->setValue(20);

        gridLayout_8->addWidget(animationSpeed, 1, 4, 1, 1);


        verticalLayout->addWidget(groupBox_timestep);

        textBrowser = new QTextBrowser(groupBox_lowerControl);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));
        textBrowser->setEnabled(true);
        sizePolicy14.setHeightForWidth(textBrowser->sizePolicy().hasHeightForWidth());
        textBrowser->setSizePolicy(sizePolicy14);
        textBrowser->setMinimumSize(QSize(300, 40));
        textBrowser->setMaximumSize(QSize(2048, 100));
        textBrowser->setBaseSize(QSize(0, 0));
        textBrowser->viewport()->setProperty("cursor", QVariant(QCursor(Qt::IBeamCursor)));
        textBrowser->setAcceptDrops(false);
        textBrowser->setAutoFillBackground(false);
        textBrowser->setStyleSheet(QString::fromUtf8(""));
        textBrowser->setFrameShape(QFrame::NoFrame);
        textBrowser->setFrameShadow(QFrame::Plain);
        textBrowser->setLineWidth(1);
        textBrowser->setMidLineWidth(0);
        textBrowser->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        textBrowser->setLineWrapMode(QTextEdit::NoWrap);
        textBrowser->setReadOnly(true);
        textBrowser->setAcceptRichText(false);
        textBrowser->setTextInteractionFlags(Qt::TextBrowserInteraction);
        textBrowser->setOpenLinks(false);

        verticalLayout->addWidget(textBrowser);

        clearlogToolButton = new QToolButton(groupBox_lowerControl);
        clearlogToolButton->setObjectName(QString::fromUtf8("clearlogToolButton"));
        QSizePolicy sizePolicy15(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
        sizePolicy15.setHorizontalStretch(0);
        sizePolicy15.setVerticalStretch(0);
        sizePolicy15.setHeightForWidth(clearlogToolButton->sizePolicy().hasHeightForWidth());
        clearlogToolButton->setSizePolicy(sizePolicy15);
        clearlogToolButton->setMinimumSize(QSize(0, 30));
        clearlogToolButton->setMaximumSize(QSize(16777215, 20));
        clearlogToolButton->setArrowType(Qt::NoArrow);

        verticalLayout->addWidget(clearlogToolButton);

        verticalSpacer = new QSpacerItem(20, 5, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);

        verticalLayout->addItem(verticalSpacer);


        mainVerticalLayout->addWidget(groupBox_lowerControl);


        retranslateUi(ControlUIForm);

        tabWidget->setCurrentIndex(1);
        specialControlsStackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(ControlUIForm);
    } // setupUi

    void retranslateUi(QWidget *ControlUIForm)
    {
        ControlUIForm->setWindowTitle(QApplication::translate("ControlUIForm", "RSIG Control UI", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        tabWidget->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        groupBox_mapprojbound->setTitle(QString());
        boundariesGroupBox->setTitle(QApplication::translate("ControlUIForm", "Show Boundaries", 0, QApplication::UnicodeUTF8));
        maplineNone->setText(QApplication::translate("ControlUIForm", "None", 0, QApplication::UnicodeUTF8));
        maplineStates_hires->setText(QApplication::translate("ControlUIForm", "US States (High Res.)", 0, QApplication::UnicodeUTF8));
        label_19->setText(QApplication::translate("ControlUIForm", "Color:", 0, QApplication::UnicodeUTF8));
        maplineNations->setText(QApplication::translate("ControlUIForm", "Nations", 0, QApplication::UnicodeUTF8));
        maplineCounties->setText(QApplication::translate("ControlUIForm", "US Counties", 0, QApplication::UnicodeUTF8));
        maplineStates->setText(QApplication::translate("ControlUIForm", "US States", 0, QApplication::UnicodeUTF8));
        projectionsGroupBox->setTitle(QApplication::translate("ControlUIForm", "Map Projection (2D)", 0, QApplication::UnicodeUTF8));
        sphericalRadioButton->setText(QApplication::translate("ControlUIForm", "Spherical", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        lonlatRadioButton->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        lonlatRadioButton->setText(QApplication::translate("ControlUIForm", "Longitude/Latitude", 0, QApplication::UnicodeUTF8));
        mercatorRadioButton->setText(QApplication::translate("ControlUIForm", "Mercator", 0, QApplication::UnicodeUTF8));
        stereographicRadioButton->setText(QApplication::translate("ControlUIForm", "Stereographic", 0, QApplication::UnicodeUTF8));
        lambertconformalRadioButton->setText(QApplication::translate("ControlUIForm", "Lambert Conformal", 0, QApplication::UnicodeUTF8));
        groupBox_mapserver->setTitle(QString());
        mapserverLabel->setText(QApplication::translate("ControlUIForm", "<b>2D Map Server</b>", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        mapserverComboBox->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        groupBox_mapoptions->setTitle(QString());
        miscGroupBox->setTitle(QApplication::translate("ControlUIForm", "Other Map Options", 0, QApplication::UnicodeUTF8));
        showScaleCheckBox->setText(QApplication::translate("ControlUIForm", "Show Length Scale", 0, QApplication::UnicodeUTF8));
        showLonlatCheckBox->setText(QApplication::translate("ControlUIForm", "Show Longitude/Latitude Lines", 0, QApplication::UnicodeUTF8));
        showDataProbe->setText(QApplication::translate("ControlUIForm", "Show Data Probe (HUD)", 0, QApplication::UnicodeUTF8));
        showNavigatorCheckBox->setText(QApplication::translate("ControlUIForm", "Show Navigator", 0, QApplication::UnicodeUTF8));
        groupBox_mapother->setTitle(QString());
        groupBox_20->setTitle(QString());
        chk_tooltips->setText(QApplication::translate("ControlUIForm", "Show widget tooltips", 0, QApplication::UnicodeUTF8));
        chkUseDataPrefs->setText(QApplication::translate("ControlUIForm", "Apply data preferences", 0, QApplication::UnicodeUTF8));
        label_48->setText(QApplication::translate("ControlUIForm", "GUI font size", 0, QApplication::UnicodeUTF8));
        groupBox_21->setTitle(QString());
        label_47->setText(QApplication::translate("ControlUIForm", "On double click:", 0, QApplication::UnicodeUTF8));
        chk_mapDoubleClickOption1->setText(QApplication::translate("ControlUIForm", "Print Google Map URL", 0, QApplication::UnicodeUTF8));
        chk_mapDoubleClickOption2->setText(QApplication::translate("ControlUIForm", "Open Google Map URL", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(mapTab), QApplication::translate("ControlUIForm", "Map", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabToolTip(tabWidget->indexOf(mapTab), QApplication::translate("ControlUIForm", "Set map preferences", 0, QApplication::UnicodeUTF8));
        groupBox_datatab->setTitle(QString());
        groupBox_scenario->setTitle(QString());
        scenarioLabel->setText(QApplication::translate("ControlUIForm", "<b>Scenario", 0, QApplication::UnicodeUTF8));
        timebaseLabel->setText(QApplication::translate("ControlUIForm", "<b>Timebase", 0, QApplication::UnicodeUTF8));
        groupBox_vartree->setTitle(QString());
        dataClearButton->setText(QApplication::translate("ControlUIForm", "Clear", 0, QApplication::UnicodeUTF8));
        groupBox_dataselections->setTitle(QString());
        regridCheckbox2->setText(QString());
        regridCheckbox3->setText(QString());
        regridCheckbox4->setText(QString());
        regridCheckbox5->setText(QString());
        regridCheckbox1->setText(QString());
        label_8->setText(QApplication::translate("ControlUIForm", "True color imagery:", 0, QApplication::UnicodeUTF8));
        label_selectedData->setText(QApplication::translate("ControlUIForm", "Selected data ", 0, QApplication::UnicodeUTF8));
        label_aggregate->setText(QApplication::translate("ControlUIForm", "Aggregate", 0, QApplication::UnicodeUTF8));
        label_regrid->setText(QApplication::translate("ControlUIForm", "Regrid", 0, QApplication::UnicodeUTF8));
        label_layer->setText(QApplication::translate("ControlUIForm", "Layer", 0, QApplication::UnicodeUTF8));
        groupBox_datetime->setTitle(QString());
        dateGroupBox->setTitle(QString());
        dateLabel->setText(QApplication::translate("ControlUIForm", "Date", 0, QApplication::UnicodeUTF8));
        daysLabel->setText(QApplication::translate("ControlUIForm", "# of days ", 0, QApplication::UnicodeUTF8));
        dateDateEdit->setDisplayFormat(QApplication::translate("ControlUIForm", "yyyy/MM/dd", 0, QApplication::UnicodeUTF8));
        boundingGroupBox->setTitle(QString());
        northDSpinBox->setSpecialValueText(QString());
        label_n->setText(QApplication::translate("ControlUIForm", "N", 0, QApplication::UnicodeUTF8));
        label_s->setText(QApplication::translate("ControlUIForm", "S", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("ControlUIForm", "BBox", 0, QApplication::UnicodeUTF8));
        label_e->setText(QApplication::translate("ControlUIForm", "E", 0, QApplication::UnicodeUTF8));
        groupBox_west->setTitle(QString());
        label_w->setText(QApplication::translate("ControlUIForm", "W", 0, QApplication::UnicodeUTF8));
        filterToolButton->setText(QApplication::translate("ControlUIForm", "Filter params", 0, QApplication::UnicodeUTF8));
        regridToolButton->setText(QApplication::translate("ControlUIForm", "Re-grid params", 0, QApplication::UnicodeUTF8));
        checkServersPushButton->setText(QApplication::translate("ControlUIForm", "Check Servers", 0, QApplication::UnicodeUTF8));
        getdataPushButton->setText(QApplication::translate("ControlUIForm", "Get Data", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("ControlUIForm", "Data", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabToolTip(tabWidget->indexOf(tab_2), QApplication::translate("ControlUIForm", "Select data to load<br><br>1) Choose up to 5 data sources from the menu tree<br>2) Select a starting date and number of days to process<br>3) Define the bounding box of interest<br>4) Click \"Get Data\"", 0, QApplication::UnicodeUTF8));
        label_mode->setText(QApplication::translate("ControlUIForm", "Mode:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        groupBox_mode->setAccessibleDescription(QString());
#endif // QT_NO_ACCESSIBILITY
        groupBox_mode->setTitle(QString());
        radioButton2D->setText(QApplication::translate("ControlUIForm", "2D", 0, QApplication::UnicodeUTF8));
        radioButton3D->setText(QApplication::translate("ControlUIForm", "3D", 0, QApplication::UnicodeUTF8));
        radioButtonPanelDisplay->setText(QApplication::translate("ControlUIForm", "Panel", 0, QApplication::UnicodeUTF8));
        autozoomView->setText(QApplication::translate("ControlUIForm", "Autozoom", 0, QApplication::UnicodeUTF8));
        resetView->setText(QApplication::translate("ControlUIForm", "Reset", 0, QApplication::UnicodeUTF8));
        groupBox_visibility->setTitle(QString());
        vizCheckBox->setText(QApplication::translate("ControlUIForm", "Visible", 0, QApplication::UnicodeUTF8));
        cellBoundaryCheckbox->setText(QApplication::translate("ControlUIForm", "Cell Boundary", 0, QApplication::UnicodeUTF8));
        label_opacity->setText(QApplication::translate("ControlUIForm", "Opacity     ", 0, QApplication::UnicodeUTF8));
        groupBox_opacity->setTitle(QString());
        opacity_display->setText(QApplication::translate("ControlUIForm", "100", 0, QApplication::UnicodeUTF8));
        label_minmax->setText(QApplication::translate("ControlUIForm", "Min / max  ", 0, QApplication::UnicodeUTF8));
        groupBox_minmax->setTitle(QString());
        vizDataDefault->setText(QApplication::translate("ControlUIForm", "data default", 0, QApplication::UnicodeUTF8));
        label_threshold->setText(QApplication::translate("ControlUIForm", "Threshold  ", 0, QApplication::UnicodeUTF8));
        groupBox_threshold->setTitle(QString());
        thresholdCheckBox->setText(QString());
        ThresholdLabel->setText(QApplication::translate("ControlUIForm", "1", 0, QApplication::UnicodeUTF8));
        label_colormap->setText(QApplication::translate("ControlUIForm", "Colormap  ", 0, QApplication::UnicodeUTF8));
        groupBox_colormap->setTitle(QString());
        label_scalefactor->setText(QApplication::translate("ControlUIForm", "Vertical\n"
"scalefactor", 0, QApplication::UnicodeUTF8));
        groupBox_scalefactor->setTitle(QString());
        label_ruler->setText(QApplication::translate("ControlUIForm", "Vertical\n"
"ruler          ", 0, QApplication::UnicodeUTF8));
        groupBox_ruler->setTitle(QString());
        rulerCheckBox->setText(QString());
        label_24->setText(QApplication::translate("ControlUIForm", "Lon:", 0, QApplication::UnicodeUTF8));
        label_25->setText(QApplication::translate("ControlUIForm", "Lat:", 0, QApplication::UnicodeUTF8));
        WindLabel->setText(QApplication::translate("ControlUIForm", "Wind\n"
"Controls\n"
"         ", 0, QApplication::UnicodeUTF8));
        groupBox_windcontrols->setTitle(QString());
        windVectorArrowOption->setText(QApplication::translate("ControlUIForm", "Arrows", 0, QApplication::UnicodeUTF8));
        windVectorBarbOption->setText(QApplication::translate("ControlUIForm", "Barbs", 0, QApplication::UnicodeUTF8));
        windVectorScaleFactorLabel->setText(QApplication::translate("ControlUIForm", "Scaling", 0, QApplication::UnicodeUTF8));
        windVectorHeadSizeLabel->setText(QApplication::translate("ControlUIForm", "Tip size", 0, QApplication::UnicodeUTF8));
        autoSparseCheckBox->setText(QApplication::translate("ControlUIForm", "Auto sparse", 0, QApplication::UnicodeUTF8));
        windVectorSparsenessLabel->setText(QApplication::translate("ControlUIForm", "Sparse by", 0, QApplication::UnicodeUTF8));
        SpecialControlLabel->setText(QApplication::translate("ControlUIForm", "Special\n"
"Controls:   ", 0, QApplication::UnicodeUTF8));
        groupBox_specialcontrols->setTitle(QString());
        groupBox_cmaq->setTitle(QString());
        CMAQColLabel->setText(QApplication::translate("ControlUIForm", "1", 0, QApplication::UnicodeUTF8));
        CMAQLayerLabel->setText(QApplication::translate("ControlUIForm", "1", 0, QApplication::UnicodeUTF8));
        ShowCMAQLayer->setText(QApplication::translate("ControlUIForm", "Layer:", 0, QApplication::UnicodeUTF8));
        CMAQRowLabel->setText(QApplication::translate("ControlUIForm", "1", 0, QApplication::UnicodeUTF8));
        flattenCMAQ->setText(QApplication::translate("ControlUIForm", "Flatten", 0, QApplication::UnicodeUTF8));
        ShowCMAQCol->setText(QApplication::translate("ControlUIForm", "Col:", 0, QApplication::UnicodeUTF8));
        ShowCMAQRow->setText(QApplication::translate("ControlUIForm", "Row:", 0, QApplication::UnicodeUTF8));
        groupBox_ceilometer->setTitle(QString());
        label_33->setText(QApplication::translate("ControlUIForm", "Size", 0, QApplication::UnicodeUTF8));
        ceilometerShowMixingLayer->setText(QApplication::translate("ControlUIForm", "Show Mixing Layer", 0, QApplication::UnicodeUTF8));
        ceilometerShowLocations->setText(QApplication::translate("ControlUIForm", "Show Locations", 0, QApplication::UnicodeUTF8));
        label_27->setText(QApplication::translate("ControlUIForm", "Max alt. (km)", 0, QApplication::UnicodeUTF8));
        label_28->setText(QApplication::translate("ControlUIForm", "Min alt. (km)", 0, QApplication::UnicodeUTF8));
        label_18->setText(QApplication::translate("ControlUIForm", "Plot\n"
"Scroller", 0, QApplication::UnicodeUTF8));
        groupBox_neubrew->setTitle(QString());
        label_32->setText(QApplication::translate("ControlUIForm", "Size", 0, QApplication::UnicodeUTF8));
        neubrewShowLocations->setText(QApplication::translate("ControlUIForm", "Show Locations", 0, QApplication::UnicodeUTF8));
        label_30->setText(QApplication::translate("ControlUIForm", "Plot\n"
"Scroller", 0, QApplication::UnicodeUTF8));
        label_31->setText(QApplication::translate("ControlUIForm", "Max alt. (km)", 0, QApplication::UnicodeUTF8));
        label_29->setText(QApplication::translate("ControlUIForm", "Min alt. (km)", 0, QApplication::UnicodeUTF8));
        groupBox_behr->setTitle(QString());
        persistBEHR->setText(QApplication::translate("ControlUIForm", "Swath Persist", 0, QApplication::UnicodeUTF8));
        groupBox_modis->setTitle(QString());
        persistMODIS->setText(QApplication::translate("ControlUIForm", "Swath Persist", 0, QApplication::UnicodeUTF8));
        groupBox_viirs->setTitle(QString());
        persistVIIRS->setText(QApplication::translate("ControlUIForm", "Swath Persist", 0, QApplication::UnicodeUTF8));
        groupBox_tropomi->setTitle(QString());
        persistTROPOMI->setText(QApplication::translate("ControlUIForm", "Swath Persist", 0, QApplication::UnicodeUTF8));
        groupBox_hms->setTitle(QString());
        persistHMS->setText(QApplication::translate("ControlUIForm", "Satellite Persist", 0, QApplication::UnicodeUTF8));
        groupBox_panel->setTitle(QString());
        label_34->setText(QApplication::translate("ControlUIForm", "Sampled data:", 0, QApplication::UnicodeUTF8));
        label_21->setText(QApplication::translate("ControlUIForm", "Probing data:", 0, QApplication::UnicodeUTF8));
        label_20->setText(QApplication::translate("ControlUIForm", "Sampling sites:", 0, QApplication::UnicodeUTF8));
        groupBox_pandora->setTitle(QString());
        label_38->setText(QApplication::translate("ControlUIForm", "Size", 0, QApplication::UnicodeUTF8));
        label_37->setText(QApplication::translate("ControlUIForm", "Max alt. (km)", 0, QApplication::UnicodeUTF8));
        label_35->setText(QApplication::translate("ControlUIForm", "Min alt. (km)", 0, QApplication::UnicodeUTF8));
        label_36->setText(QApplication::translate("ControlUIForm", "Plot\n"
"Scroller", 0, QApplication::UnicodeUTF8));
        pandoraShowLocations->setText(QApplication::translate("ControlUIForm", "Show Locations", 0, QApplication::UnicodeUTF8));
        groupBox_tempo->setTitle(QString());
        persistTEMPO->setText(QApplication::translate("ControlUIForm", "Swath Persist", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(propTab), QApplication::translate("ControlUIForm", "View", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabToolTip(tabWidget->indexOf(propTab), QApplication::translate("ControlUIForm", "Visualization controls", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(viewTab), QApplication::translate("ControlUIForm", "Info", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabToolTip(tabWidget->indexOf(viewTab), QApplication::translate("ControlUIForm", "Information about loaded data", 0, QApplication::UnicodeUTF8));
        groupBox_savetab->setTitle(QString());
        groupBox_saveformats->setTitle(QString());
        label_5->setText(QApplication::translate("ControlUIForm", "<html><head/><body><p><span style=\" font-weight:600;\">Data file formats:</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        netcdfioapiRadioButton->setText(QApplication::translate("ControlUIForm", "NetCDF - IOAPI (CMAQ or regridded data only)", 0, QApplication::UnicodeUTF8));
        asciisheetRadioButton->setText(QApplication::translate("ControlUIForm", "ASCII spreadsheet", 0, QApplication::UnicodeUTF8));
        scenarioRadioButton->setText(QApplication::translate("ControlUIForm", "Scenario", 0, QApplication::UnicodeUTF8));
        originalFileRadioButton->setText(QApplication::translate("ControlUIForm", "Original files (slow)", 0, QApplication::UnicodeUTF8));
        mpegRadioButton->setText(QApplication::translate("ControlUIForm", "MPEG", 0, QApplication::UnicodeUTF8));
        shapeRadioButton->setText(QApplication::translate("ControlUIForm", "Shape file", 0, QApplication::UnicodeUTF8));
        geotiffRadioButton->setText(QApplication::translate("ControlUIForm", "Geotiff (floating point rasterization)", 0, QApplication::UnicodeUTF8));
        allpngsRadioButton->setText(QApplication::translate("ControlUIForm", "All images: sequence of PNGs", 0, QApplication::UnicodeUTF8));
        highrespngRadioButton->setText(QApplication::translate("ControlUIForm", "High resolution image - PNG", 0, QApplication::UnicodeUTF8));
        binaryxdrRadioButton->setText(QApplication::translate("ControlUIForm", "Binary XDR", 0, QApplication::UnicodeUTF8));
        currentpngRadioButton->setText(QApplication::translate("ControlUIForm", "Current image - PNG", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("ControlUIForm", "<html><head/><body><p><span style=\" font-weight:600;\">Visualization file formats:</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        netcdfcoardsRadioButton->setText(QApplication::translate("ControlUIForm", "NetCDF - COARDS", 0, QApplication::UnicodeUTF8));
        kmlRadioButton->setText(QApplication::translate("ControlUIForm", "KML file", 0, QApplication::UnicodeUTF8));
        screenRecordRadioButton->setText(QApplication::translate("ControlUIForm", "Screen Recording", 0, QApplication::UnicodeUTF8));
        geotiffrgbRadioButton->setText(QApplication::translate("ControlUIForm", "Geotiff (RGB)", 0, QApplication::UnicodeUTF8));
        gzipCheckBox->setText(QApplication::translate("ControlUIForm", "Use GZip file compression", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("ControlUIForm", "<html><head/><body><p><span style=\" font-weight:600;\">Options:</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        saveMetadataCheckBox->setText(QApplication::translate("ControlUIForm", "Save metadata", 0, QApplication::UnicodeUTF8));
        groupBox_savefile->setTitle(QString());
        writetofileToolButton->setText(QApplication::translate("ControlUIForm", "Directory:", 0, QApplication::UnicodeUTF8));
        label_26->setText(QApplication::translate("ControlUIForm", "Filename:", 0, QApplication::UnicodeUTF8));
        filename_generated->setText(QApplication::translate("ControlUIForm", "(generated)", 0, QApplication::UnicodeUTF8));
        filenameLineEdit->setText(QApplication::translate("ControlUIForm", "myRsig3DScenario.xml", 0, QApplication::UnicodeUTF8));
        savevideoPushButton->setText(QApplication::translate("ControlUIForm", "Record Screen", 0, QApplication::UnicodeUTF8));
        savefilePushButton->setText(QApplication::translate("ControlUIForm", "Save", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(saveTab), QApplication::translate("ControlUIForm", "Save", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabToolTip(tabWidget->indexOf(saveTab), QApplication::translate("ControlUIForm", "Options for saving data to your disk", 0, QApplication::UnicodeUTF8));
        groupBox_rsigweb->setTitle(QString());
        tipsPushButton->setText(QApplication::translate("ControlUIForm", "Show Usage Tips", 0, QApplication::UnicodeUTF8));
        datausePushButton->setText(QApplication::translate("ControlUIForm", "Show Data Use Policy", 0, QApplication::UnicodeUTF8));
        openwebPushButton->setText(QApplication::translate("ControlUIForm", "Open RSIG project webpage", 0, QApplication::UnicodeUTF8));
        emailPushButton->setText(QApplication::translate("ControlUIForm", "Email to RSIG project", 0, QApplication::UnicodeUTF8));
        downloaderPushButton->setText(QApplication::translate("ControlUIForm", "File Oriented Downloader", 0, QApplication::UnicodeUTF8));
        exitPushButton->setText(QApplication::translate("ControlUIForm", "Exit RSIG3D", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("ControlUIForm", "<b>Check for updated RSIG3D version</b>", 0, QApplication::UnicodeUTF8));
        rsigTextBrowser->setHtml(QApplication::translate("ControlUIForm", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Lucida Grande'; font-size:10pt; font-weight:600;\">To learn more, please visit our website: </span></p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Lucida Grande'; font-size:10pt; color:#1213ff;\">https://www.epa.gov/rsig</span></p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Lucida Grande'; font-size:10pt;"
                        " font-weight:600;\">Or contact </span><span style=\" font-family:'Lucida Grande'; font-size:10pt; font-weight:600; text-decoration: underline; color:#1b1eff;\">rsig@epa.gov</span></p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Lucida Grande'; font-size:10pt; font-weight:600; color:#000000;\">Tech support: 919-541-5500</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QString());
        checknowPushButton->setText(QApplication::translate("ControlUIForm", "Check for update now", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("ControlUIForm", "RSIG Web", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabToolTip(tabWidget->indexOf(tab), QApplication::translate("ControlUIForm", "Misc web enabled functions:<br><br>- Updates<br>- Data use policy<br>- File oriented downloader<br>- RSIG contact info", 0, QApplication::UnicodeUTF8));
        groupBox_lowerControl->setTitle(QString());
        groupBox_timestep->setTitle(QString());
        backButton24->setText(QApplication::translate("ControlUIForm", "-24", 0, QApplication::UnicodeUTF8));
        backButton->setText(QApplication::translate("ControlUIForm", ".", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("ControlUIForm", "Speed:", 0, QApplication::UnicodeUTF8));
        playButton->setText(QApplication::translate("ControlUIForm", "Play", 0, QApplication::UnicodeUTF8));
        forwardButton->setText(QApplication::translate("ControlUIForm", ".", 0, QApplication::UnicodeUTF8));
        forwardButton24->setText(QApplication::translate("ControlUIForm", "+24", 0, QApplication::UnicodeUTF8));
        label_timestep->setText(QApplication::translate("ControlUIForm", "Timestep", 0, QApplication::UnicodeUTF8));
        animationSpeed->setSpecialValueText(QString());
        textBrowser->setDocumentTitle(QString());
        clearlogToolButton->setText(QApplication::translate("ControlUIForm", "clear", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ControlUIForm: public Ui_ControlUIForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONTROLDOCKWIDGET_H
