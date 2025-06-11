/******************************************************************************
PURPOSE: ControlDockWidget.C - Implements the control widget.

NOTES: Implemented using QT's QDockWidget class.

HISTORY: 2012/11/12, zhang.nan@epa.gov, Created.
******************************************************************************/

#include <assert.h>  // For macro assert().
#include <stdio.h>   // For snprintf().
#include <string.h>  // For memset().

#include <vector>    // For std::vector.
#include <string>    // For std::string.
#include <iostream>  // For std::endl.

#include <QApplication>      //qApp, slot closeAllWindows()
#include <QDesktopServices>  //QDesktopServices::openUrl(QUrl);
#include <QString>
#include <QResizeEvent>
#include <QDebug>
#include <QVector>
#include <QStringList>
#include <QDir>               //QDir::homePath()
#include <QComboBox>
#include <QSizePolicy>
#include <QList>
#include <QObjectList>
#include <QFileDialog>
#include <QUuid>
#include <QScrollBar>

#include <DatasetManager.h> // For class emvl::DatasetManager.

#include "RSIGGlobal.h"            //global vars
#include "HttpDownloadDialog.h"    //HttpGet class, multi-threaded downloading web data
#include "GeoDataObject.h"
#include "GeoDataList.h"
#include "GeoDataPropertyEdit.h"
#include "VisDataPropertyEdit.h"
#include "DomUtils.h"               //XML utils
#include "ControlDockWidget.h"
#include "ui_controldockwidget.h"   //UI form
#include "ui_regridparms.h"
#include "LookupTableWidget.h"
#include "SlantLookupTableWidget.h"
#include "MainWin.h"
#include "VisDataList.h"
#include "VisOptions.h"
#include "GetData.h"     // For fileNamesOfLoadedDatasets(), reloadDatasets().
#include "RegridParmsDialog.h"
#include "RegridParmsData.h"
#include "FilterSettingDialog.h"
#include "FilterSettingData.h"
#include "RGBColormap.h" // For RGBColormap and related functions.

static QString scenarioStrings[] = {
  "User Defined",
  "Restore From File",
  "Restore Last"
};

static QString timebaseStrings[] = {
  "Hourly",
  "Daily",
  "Daily_LST",
  "Monthly"
};

// max range for durationSpinBox
static int timebaseMaxHourly   = 7;  // days
//static int timebaseMaxDaily    = 31; //days
//static int timebaseMaxDailyLst = 31; //days
static int timebaseMaxDaily    = 365; //days
static int timebaseMaxDailyLst = 365; //days
static int timebaseMaxMonthly  = 24; //months
const int TABWIDGET_HEIGHT_ROW5 = 560;



/*!
 \class ControlDockWidget
 
 \brief This class provides the window for various control widgets. 
 It is the \e view of the Model-View-Control (MVC) model.
 
 \sa QDocWidget
 */

ControlDockWidget::ControlDockWidget(QWidget *parent): 
  QDockWidget(parent)
  ,ui(0)
  ,m_viewSplitter(0)
  ,m_objectPropEdit(0)
  ,m_visPropEdit(0)
  ,m_snapshotTimer()
  ,m_stateTimer()
  ,m_ffmpeg()
  ,m_visible(1) {

  try {
    this->setObjectName("controls");
    ui = new Ui_ControlUIForm;
    ui->setupUi(this);
    ui->mainVerticalLayout->setContentsMargins(2, 2, 2, 2);
    ui->tabWidget->setCurrentIndex(0);
    ui->tabWidget->setMinimumHeight(TABWIDGET_HEIGHT_ROW5);
    ui->tabWidget->setMaximumHeight(TABWIDGET_HEIGHT_ROW5);

#ifdef __APPLE__
    // On Macs default tabs must be size-adjusted to fit:
    ui->tabWidget->setStyleSheet("QTabBar::tab:selected { color:#0000FF; background-color:#CCCCCC }\nQTabBar::tab { padding-top:2px; padding-bottom:2px; padding-left:10px; padding-right:10px; margin-top:0px; margin-bottom:0px; }");
#endif

    this->setWidget(ui->mainVerticalLayout->parentWidget());
    this->setWindowTitle("RSIG3D Controls");

    const int dockWidth = 450;
    this->setFeatures(QDockWidget::NoDockWidgetFeatures);
    this->setAllowedAreas(Qt::LeftDockWidgetArea);
    this->setMinimumWidth(dockWidth-60);
    this->resize(dockWidth, 600);
    this->setFloating(false);

    this->initTab0ClientData(); //Map Tab
    this->initTab1ClientData(); //Data Tab
    this->initTab2ClientData(); //View Tab
    this->initTab3ClientData(); //Save Tab
    this->initTab4ClientData(); //RSIG Web Tab

    this->setWidgetTooltips();
    this->initRSIG3DHelp(0);
                
    connect(&m_stateTimer, SIGNAL(timeout()), this, SLOT(onStateTimeOut()));
    m_stateTimer.setSingleShot(false);
    m_stateTimer.start(333);

    ui->textBrowser->setContextMenuPolicy(Qt::DefaultContextMenu);
    ui->textBrowser->setReadOnly(true);
    ui->textBrowser->setStyleSheet("background-color: rgb(240,240,240);");
    ui->textBrowser->verticalScrollBar()->setStyleSheet(
                                                        "background-color: transparent;"
                                                        "alternate-background-color: transparent;"
                                                        );
     ui->textBrowser->horizontalScrollBar()->setStyleSheet(
                                                        "background-color: transparent;"
                                                        "alternate-background-color: transparent;"
                                                        );
 
    deleteModisTrueColorImages();
  } catch(...) {
    throw "Error: ControlDockWidget failed in construction!";
  }
}



ControlDockWidget::~ControlDockWidget() {

  try {

    if (m_stateTimer.isActive()) {
      m_stateTimer.stop();
    }

    if (m_snapshotTimer.isActive()) {
      m_snapshotTimer.stop();
    }

    if (m_objectPropEdit) {
      if (m_objectPropEdit->parent() == NULL) {
        delete m_objectPropEdit;
      }
    }

    if (m_visPropEdit) {
      if (m_visPropEdit->parent() == NULL) {
        delete m_visPropEdit;
      }
    }

    delete ui;
  } catch (...) {
    qDebug()<<"Error: ~ControlDockWidget destructor.";
  }
}



void ControlDockWidget::deleteModisTrueColorImages() {
  // delete old MODIS True Color images
  QDir dir(QDir::homePath() + "/.rsig/");
  dir.setNameFilters(QStringList() << "trueColor_*.png");
  dir.setFilter(QDir::Files);

  foreach(QString dirFile, dir.entryList()) {
    dir.remove(dirFile);
  }

  RSIGScene& rsigScene = RSIGScene::getInstance();
  rsigScene.reloadModisTrueColor(-1);
}



void ControlDockWidget::setVariablesToComboBox(QComboBox *w,
                                               const QString *array,
                                               const QString *tips,
                                               const QString *tipscat,
                                               int len) {

  if (array && w && tips) {
    int count = 0;
    w->clear();

    for (int i=0; i<len; i++) {
      const QString& str = (array[i]);
      QString tip = (tips[i]);

      if (tipscat) {
        tip = tipscat[i] + ": "+ tip;
      }

      if (str.length()==0) {
        continue;
      }

      if (str[0]==' ') {
        continue;
      }

      if (str[0]=='-' && str[1]=='-') {
        w->insertSeparator(i);
      } else {
        w->addItem(str);
        w->setItemData(count, tip, Qt::ToolTipRole);
        w->setItemData(count, tip, Qt::StatusTipRole);
        w->setItemData(count, tip, Qt::WhatsThisRole);
      }

      count++;
    }

    //set default index, conditionally
    int index = 0;

    if (array[0][0]=='-' && array[0][1]=='-') {
      index=1;
    }

    w->setCurrentIndex(index);
  }
}



void ControlDockWidget::addColormapHelper() {
  const size_t colormapCount = emvl::getRGBColormapCount();

  for ( size_t index = 0; index < colormapCount; ++index ) {
    const char* const name = emvl::getRGBColormapName( index );
    ui->vizColormapCombobox->addItem( QString( name ) );
  }
}



void ControlDockWidget::addMaplineColorHelper() {
  const int colorCount = sizeof(maplineColorStrings) / sizeof(maplineColorStrings[0]);

  for (int i=0; i<colorCount; ++i) {
    ui->maplineColorComboBox->addItem(maplineColorStrings[i]);
  }
}



void ControlDockWidget::addScenariosHelper() {
  const int scenarioCount = sizeof(scenarioStrings) / sizeof(scenarioStrings[0]);

  for (int i=0; i<scenarioCount; ++i) {
    ui->scenarioComboBox->addItem(scenarioStrings[i]);
  }
}



void ControlDockWidget::addTimebaseHelper() {
  const int timebaseCount = sizeof(timebaseStrings) / sizeof(timebaseStrings[0]);

  for (int i=0; i<timebaseCount; ++i) {
    ui->timebaseComboBox->addItem(timebaseStrings[i]);
  }
}



/*!
 \fn void ControlDockWidget::onScenarioIndexChanged(int index)

 Change the scenario index.
 */
void ControlDockWidget::onScenarioIndexChanged(int index) {
  const int scenarioCount = sizeof(scenarioStrings)/sizeof(scenarioStrings[0]);

  if (index>=0 && index<scenarioCount) {
    MainWin& mainWin = MainWin::getInstance();

    switch (index) {
    case 1: //Restore From File
      {
        onDataClearClicked();
        //clear the object list to zero
        m_objectPropEdit->clear();

        // remove old items from the visualization combo box
        for (int ind=0; ind<=ui->vizVarCombobox->count(); ind++) {
          //always remove item 0, because the combobox shrinks every time removeItem() runs
          ui->vizVarCombobox->removeItem(0);
        }

        QString filter("XML files (*.xml)");
        QString fname = QFileDialog::getOpenFileName(mainWin.controlWidget(),
                                             "Select RSIG3D state file",
                                             QDir::currentPath(),
                                             filter);
        qDebug("restoring from file");
        mainWin.restoreStateFromFile(fname);
      }
      break;
    case 2: //Restore Last
      onDataClearClicked();
      //clear the object list to zero
      m_objectPropEdit->clear();

      // remove old items from the visualization combo box
      for (int ind=0; ind<=ui->vizVarCombobox->count(); ind++) {
        //always remove item 0, because the combobox shrinks every time removeItem() runs
        ui->vizVarCombobox->removeItem(0);
      }

      mainWin.restoreStateFromFile(mainWin.stateFileName());
      break;
    case 3: //Fire Event, 5 sources are open
      break;
    case 4: //Ozone Event
      break;
    default:
      break;
    }
  }
}



void ControlDockWidget::onTimebaseIndexChanged(int index) {
  const int timebaseCount = sizeof(timebaseStrings)/sizeof(timebaseStrings[0]);
  QString fname;
  QString rsigVariablesFilename;
  RSIGState* gState = RSIGStateSingleton::getInstance();

  //clear the object list to zero
  m_objectPropEdit->clear();
        
  // remove old items from the visualization combo box
  int numItems = ui->vizVarCombobox->count();

  for (int ind=0; ind<=numItems; ind++) {
    //always remove item 0, because the combobox shrinks every time removeItem() runs
    ui->vizVarCombobox->removeItem(0);
  }
           
  if (index>=0 && index<timebaseCount) {

    switch (index) {
    case RSIGState::TIMEBASE_HOURLY: // Hourly
      rsigVariablesFilename = ":/rsig_variables.txt";
      this->addVariablesHelper2(rsigVariablesFilename);
      ui->daysLabel->setText("# of days\n(max " + QString::number(timebaseMaxHourly) + ")");
      ui->durationSpinBox->setRange(1, timebaseMaxHourly);
      gState->setTimebaseType(RSIGState::TIMEBASE_HOURLY);
      break;
    case RSIGState::TIMEBASE_DAILY: // Daily
      rsigVariablesFilename = ":/rsig_variables_daily.txt";
      this->addVariablesHelper2(rsigVariablesFilename);
      ui->daysLabel->setText("# of days\n(max " + QString::number(timebaseMaxDaily) + ")");
      ui->durationSpinBox->setRange(1, timebaseMaxDaily);
      gState->setTimebaseType(RSIGState::TIMEBASE_DAILY);
      break;
    case RSIGState::TIMEBASE_DAILY_LST: // Daily LST
      rsigVariablesFilename = ":/rsig_variables_lstdaily.txt";
      this->addVariablesHelper2(rsigVariablesFilename);
      ui->daysLabel->setText("# of days\n(max " + QString::number(timebaseMaxDailyLst) + ")");
      ui->durationSpinBox->setRange(1, timebaseMaxDaily);
      gState->setTimebaseType(RSIGState::TIMEBASE_DAILY_LST);
      break;
    case RSIGState::TIMEBASE_MONTHLY: // Monthly
      rsigVariablesFilename = ":/rsig_variables_monthly.txt";
      this->addVariablesHelper2(rsigVariablesFilename);
      ui->daysLabel->setText("# of months\n(max " + QString::number(timebaseMaxMonthly) + ")");
      ui->durationSpinBox->setRange(1, timebaseMaxMonthly);
      gState->setTimebaseType(RSIGState::TIMEBASE_MONTHLY);
      break;
    default:
      break;
    }
  }

  onDurationChanged();
}



void ControlDockWidget::setWidgetTooltips() {
  bool tooltipsEnabled = false; // default

  if (ui->chk_tooltips->checkState() == Qt::Checked) {
    tooltipsEnabled = true;
  }
  
  if (!tooltipsEnabled) {

    // MAP tab
    ui->mapserverComboBox->setToolTip("");
    ui->projectionsGroupBox->setToolTip("");
    ui->boundariesGroupBox->setToolTip("");
    ui->maplineColorComboBox->setToolTip("");
    ui->showDataProbe->setToolTip("");
    ui->showNavigatorCheckBox->setToolTip("");
    ui->chkUseDataPrefs->setToolTip("");

    // DATA tab
    ui->scenarioLabel->setToolTip("");
    ui->scenarioComboBox->setToolTip("");
    ui->dataClearButton->setToolTip("");
    ui->label_aggregate->setToolTip("");
    ui->aggregate1->setToolTip("");
    ui->aggregate2->setToolTip("");
    ui->aggregate3->setToolTip("");
    ui->aggregate4->setToolTip("");
    ui->aggregate5->setToolTip("");
    ui->label_layer->setToolTip("");
    ui->selectedLayer1->setToolTip("");
    ui->selectedLayer2->setToolTip("");
    ui->selectedLayer3->setToolTip("");
    ui->selectedLayer4->setToolTip("");
    ui->selectedLayer5->setToolTip("");
    ui->label_regrid->setToolTip("");
    ui->regridCheckbox1->setToolTip("");
    ui->regridCheckbox2->setToolTip("");
    ui->regridCheckbox3->setToolTip("");
    ui->regridCheckbox4->setToolTip("");
    ui->regridCheckbox5->setToolTip("");
    ui->trueColorComboBox->setToolTip("");
    ui->dateGroupBox->setToolTip("");
    ui->boundingGroupBox->setToolTip("");
    ui->regridToolButton->setToolTip("");
    ui->filterToolButton->setToolTip("");
    ui->getdataPushButton->setToolTip("");
    ui->checkServersPushButton->setToolTip("");
    ui->frame_2->setToolTip("");
    ui->label_selectedData->setToolTip("");

    // VIEW tab
    ui->label_10->setToolTip("");
    ui->radioButton2D->setToolTip("");
    ui->radioButton3D->setToolTip("");
    ui->radioButtonPanelDisplay->setToolTip("");
    ui->autozoomView->setToolTip("");
    ui->resetView->setToolTip("");
    ui->vizVarCombobox->setToolTip("");
    ui->vizCheckBox->setToolTip("");
    ui->cellBoundaryCheckbox->setToolTip("");
    ui->vizOpacitySlider->setToolTip("");
    ui->label_13->setToolTip("");
    ui->vizMinSpinbox->setToolTip("");
    ui->vizMaxSpinbox->setToolTip("");
    ui->vizDataDefault->setToolTip("");
    ui->ThresholdSlider->setToolTip("");
    ui->vizColormapCombobox->setToolTip("");
    ui->calipsoScaleFactor->setToolTip("");
    ui->windVectorScaleFactor->setToolTip("");
    ui->windVectorSparseness->setToolTip("");
    ui->windVectorTipSize->setToolTip("");
    ui->autoSparseCheckBox->setToolTip("");
    ui->groupBox_13->setToolTip("");
    ui->persistMODIS->setToolTip("");
    ui->persistMODISHours->setToolTip("");
    ui->CMAQRowSlider->setToolTip("");
    ui->CMAQColSlider->setToolTip("");
    ui->flattenCMAQ->setToolTip("");
    ui->ceilometerMinimumAltitude->setToolTip("");
    ui->ceilometerMaximumAltitude->setToolTip("");
    ui->ceilometerShowMixingLayer->setToolTip("");
    ui->ceilometerShowLocations->setToolTip("");
    ui->ceilometerSize->setToolTip("");
    ui->ceilometerPlotScroller->setToolTip("");
    ui->pandoraMinimumAltitude->setToolTip("");
    ui->pandoraMaximumAltitude->setToolTip("");
    ui->pandoraShowLocations->setToolTip("");
    ui->pandoraSize->setToolTip("");
    ui->pandoraPlotScroller->setToolTip("");
    ui->neubrewMinimumAltitude->setToolTip("");
    ui->neubrewMaximumAltitude->setToolTip("");
    ui->neubrewMinimumAltitude->setToolTip("");
    ui->neubrewShowLocations->setToolTip("");
    ui->neubrewSize->setToolTip("");
    ui->neubrewPlotScroller->setToolTip("");

    // SAVE tab
    ui->scenarioRadioButton->setToolTip("");
    ui->binaryxdrRadioButton->setToolTip("");
    ui->asciisheetRadioButton->setToolTip("");
    ui->netcdfcoardsRadioButton->setToolTip("");
    ui->netcdfioapiRadioButton->setToolTip("");
    ui->mpegRadioButton->setToolTip("");
    ui->screenRecordRadioButton->setToolTip("");
    ui->currentpngRadioButton->setToolTip("");
    ui->allpngsRadioButton->setToolTip("");
    ui->gzipCheckBox->setToolTip("");
    ui->dirnameLineEdit->setToolTip("");
    ui->filenameLineEdit->setToolTip("");
    ui->savefilePushButton->setToolTip("");
    ui->savevideoPushButton->setToolTip("");

    // RSIG WEB tab
    ui->checknowPushButton->setToolTip("");
    ui->tipsPushButton->setToolTip("");
    ui->datausePushButton->setToolTip("");
    ui->downloaderPushButton->setToolTip("");
    ui->openwebPushButton->setToolTip("");
    ui->emailPushButton->setToolTip("");
    ui->exitPushButton->setToolTip("");

    } else {

    // MAP tab
    ui->mapserverComboBox->setToolTip("Map server to use for the 2D view.<br><br>NOTE: because Marble and Google use different control mechanisms, switching between servers may introduce slight zoom and position shifts.");
    ui->projectionsGroupBox->setToolTip("Set map projections for 2D view<br><br>-Google maps only use the web-Mercator projection.<br><br>-TrueColor images can only be displayed in Longitude/Latitude and Mercator.");
    ui->boundariesGroupBox->setToolTip("Display map boundary overlays.\n\nNote: map line sets have different spatial resolutions.");
    ui->maplineColorComboBox->setToolTip("Color to use for mapline overlay. ");
    ui->showDataProbe->setToolTip("Show data values at the mouse location in a heads up display (HUD). ");
    ui->showNavigatorCheckBox->setToolTip("Display the 2D map navigation controls. ");
    ui->chkUseDataPrefs->setToolTip("Automatically apply preferences from .rsig/rsig_data_prefs.ini when data is loaded.");

    // DATA tab
    ui->scenarioLabel->setToolTip("Load a saved scenario from an earlier session");
    ui->scenarioComboBox->setToolTip("Load a saved scenario from an earlier session, or define a new scenario below");
    ui->timebaseLabel->setToolTip("<b>Change the timestep size.</b><br><br>  -Some variables are not available for certain timebases.<br><br>  -Changing this menu will remove any loaded data, clear all selected variables, and reset the variable selection tree.<br><br>-Variables in the DAILY_LST menu are aggregated based on the local standard time (LST), consistent with U.S. air quality standards. All others (HOURLY, DAILY, MONTHLY) are aggregated based on UTC time.");
    ui->timebaseComboBox->setToolTip("<b>Change the timestep size.</b><br><br>  -Some variables are not available for certain timebases.<br><br>  -Changing this menu will remove any loaded data, clear all selected variables, and reset the variable selection tree<br><br>-Variables in the DAILY_LST menu are aggregated based on the local standard time (LST), consistent with U.S. air quality standards. All others (HOURLY, DAILY, MONTHLY) are aggregated based on UTC time.");
    ui->dataClearButton->setToolTip("Unselect all data variable selections");
    QString aggregateText = "<b>Aggregation options for certain supported variables</b><br><br>- Time aggregation is by UTC day. For aggregation by local standard time (LST), please select the Daily_LST timebase above.<br><br>- UTC daily max: Each data point is the maximum that occurred at that location during the UTC day.<br><br>- UTC daily mean: Each data point is the mean value at that location during the UTC day.";
    ui->label_aggregate->setToolTip(aggregateText);
    ui->aggregate1->setToolTip(aggregateText);
    ui->aggregate2->setToolTip(aggregateText);
    ui->aggregate3->setToolTip(aggregateText);
    ui->aggregate4->setToolTip(aggregateText);
    ui->aggregate5->setToolTip(aggregateText);
    ui->label_layer->setToolTip("Desired layer for multi-layer CMAQ variables<br><br>- Specify -1 for column integration of concentration variables<br>- Specify 0 to get all layers<br>- Specify 1 through LAYER_MAX for a specific layer");
    ui->selectedLayer1->setToolTip("Desired layer for multi-layer CMAQ variables<br><br>- Specify -1 for column integration of concentration variables<br>- Specify 0 to get all layers<br>- Specify 1 through LAYER_MAX for a specific layer <br><br>Note: For CMAQ wind a specific layer must be specified.");
    ui->selectedLayer2->setToolTip("Desired layer for multi-layer CMAQ variables<br><br>- Specify -1 for column integration of concentration variables<br>- Specify 0 to get all layers<br>- Specify 1 through LAYER_MAX for a specific layer <br><br>Note: For CMAQ wind a specific layer must be specified.");
    ui->selectedLayer3->setToolTip("Desired layer for multi-layer CMAQ variables<br><br>- Specify -1 for column integration of concentration variables<br>- Specify 0 to get all layers<br>- Specify 1 through LAYER_MAX for a specific layer <br><br>Note: For CMAQ wind a specific layer must be specified.");
    ui->selectedLayer4->setToolTip("Desired layer for multi-layer CMAQ variables<br><br>- Specify -1 for column integration of concentration variables<br>- Specify 0 to get all layers<br>- Specify 1 through LAYER_MAX for a specific layer <br><br>Note: For CMAQ wind a specific layer must be specified.");
    ui->selectedLayer5->setToolTip("Desired layer for multi-layer CMAQ variables<br><br>- Specify -1 for column integration of concentration variables<br>- Specify 0 to get all layers<br>- Specify 1 through LAYER_MAX for a specific layer <br><br>Note: For CMAQ wind a specific layer must be specified.");
    ui->label_regrid->setToolTip("Interpolate data to a specific CMAQ grid.<br><br>Use the \"Re-grid parameters\" button below to specify the grid.<br><br>Regridding happens on-the-fly when the data is requested.");
    ui->regridCheckbox1->setToolTip("Interpolate data to a specific CMAQ grid.<br><br>Use the \"Re-grid parameters\" button below to specify the grid.<br><br>Regridding happens on-the-fly when the data is requested.");
    ui->regridCheckbox2->setToolTip("Interpolate data to a specific CMAQ grid.<br><br>Use the \"Re-grid parameters\" button below to specify the grid.<br><br>Regridding happens on-the-fly when the data is requested.");
    ui->regridCheckbox3->setToolTip("Interpolate data to a specific CMAQ grid.<br><br>Use the \"Re-grid parameters\" button below to specify the grid.<br><br>Regridding happens on-the-fly when the data is requested.");
    ui->regridCheckbox4->setToolTip("Interpolate data to a specific CMAQ grid.<br><br>Use the \"Re-grid parameters\" button below to specify the grid.<br><br>Regridding happens on-the-fly when the data is requested.");
    ui->regridCheckbox5->setToolTip("Interpolate data to a specific CMAQ grid.<br><br>Use the \"Re-grid parameters\" button below to specify the grid.<br><br>Regridding happens on-the-fly when the data is requested.");
    ui->trueColorComboBox->setToolTip("Load True Color imagery (daily aggregation) if available");
    ui->dateGroupBox->setToolTip("Select time range of data to be retrieved");
    ui->boundingGroupBox->setToolTip("Latitude/Longitude bounds of desired data.<br><br>This can also be chosen by right clicking and dragging in the 2D view mode");
    ui->regridToolButton->setToolTip("Specify exact grid parameters to use for interpolation to a CMAQ grid");
    ui->filterToolButton->setToolTip("Specify data filtering parameters for GASP and CALIPSO data. ");
    ui->getdataPushButton->setToolTip("Once all selections have been set, click here to begin the data retrieval");
    ui->checkServersPushButton->setToolTip("Display all servers in the RSIG system that were offline in the last 24 hours");
    ui->frame_2->setToolTip("Animation controls");
    ui->label_selectedData->setToolTip("Select up to 5 variables from the menu tree above");
    ui->forwardButton->setToolTip("Increment time");
    ui->backButton->setToolTip("Decrement time");
    ui->playButton->setToolTip("Animate over time");
    ui->timestepSlider->setToolTip("Manual time selection");
    ui->animationSpeed->setToolTip("Animation speed");
    ui->clearlogToolButton->setToolTip("Clear the log messages below");

    ui->forwardButton24->setToolTip("Increment time by 24 hours");
    ui->backButton24->setToolTip("Decrement time by 24 hours");

    
    // VIEW tab
    ui->label_10->setToolTip("Select 2D or 3D viewing mode");
    ui->radioButton2D->setToolTip("Select 2D viewing mode");
    ui->radioButton3D->setToolTip("Select 3D viewing mode");
    ui->radioButtonPanelDisplay->setToolTip("Select Panel Display mode");
    ui->autozoomView->setToolTip("Automatically zoom to the currently defined bounding box");
    ui->resetView->setToolTip("Reset the view to the default setting");
    ui->vizVarCombobox->setToolTip("Variable selector.<br><br>You can modify the viewing properties of the selected variables using the controls below");
    ui->vizCheckBox->setToolTip("Turn selected variable on or off");
    ui->cellBoundaryCheckbox->setToolTip("View cell boundaries (polygon data only)");
    ui->vizOpacitySlider->setToolTip("Make data partially transparent");
    ui->label_13->setToolTip("Adjust data range of color map");
    ui->vizMinSpinbox->setToolTip("Adjust data range of color map");
    ui->vizMaxSpinbox->setToolTip("Adjust data range of color map");
    ui->vizDataDefault->setToolTip("Lock the color map range to the min/max present in the data");
    ui->ThresholdSlider->setToolTip("Only show data that is greater than the threshold");
    ui->vizColormapCombobox->setToolTip("Change the color map palette");
    ui->calipsoScaleFactor->setToolTip("Adjust the vertical exaggeration (3D viewing only)");
    ui->windVectorScaleFactor->setToolTip("Adjust the scale factor for wind vectors or barbs");
    ui->windVectorTipSize->setToolTip("Adjust the size of the wind vector tips");
    ui->autoSparseCheckBox->setToolTip("Automatically adjust the density of displayed wind vectors or barbs (Gridded data only)");
    ui->windVectorSparseness->setToolTip("Manually adjust the density of displayed wind vectors or barbs (Gridded data only)");
    ui->groupBox_13->setToolTip("Show a vertical scale at the specified location");
    ui->persistMODIS->setToolTip("Allow swath-based satellite data to persist for specified number of hours");
    ui->persistMODISHours->setToolTip("Number of hours for swath-based satellite data to remain visible");
    ui->persistVIIRS->setToolTip("Allow swath-based satellite data to persist for specified number of hours");
    ui->persistVIIRSHours->setToolTip("Number of hours for swath-based satellite data to remain visible");
    ui->persistBEHR->setToolTip("Allow swath-based satellite data to persist for specified number of hours");
    ui->persistBEHRHours->setToolTip("Number of hours for swath-based satellite data to remain visible");
    ui->persistTROPOMI->setToolTip("Allow swath-based satellite data to persist for specified number of hours");
    ui->persistTROPOMIHours->setToolTip("Number of hours for swath-based satellite data to remain visible");
    ui->persistTEMPO->setToolTip("Allow swath-based satellite data to persist for specified number of hours");
    ui->persistTEMPOHours->setToolTip("Number of hours for swath-based satellite data to remain visible");
    ui->persistHMS->setToolTip("Allow swath-based satellite data to persist for specified number of hours");
    ui->persistHMSHours->setToolTip("Number of hours for swath-based satellite data to remain visible");
    ui->CMAQRowSlider->setToolTip("Principal axis slice for 3D CMAQ data");
    ui->CMAQColSlider->setToolTip("Principal axis slice for 3D CMAQ data");
    ui->flattenCMAQ->setToolTip("Display CMAQ projected onto a flat Earth (3D mode)");
    ui->ceilometerMinimumAltitude->setToolTip("Altitude range for ceilometer data");
    ui->ceilometerMaximumAltitude->setToolTip("Altitude range for ceilometer data");
    ui->ceilometerShowMixingLayer->setToolTip("Display the estimated mixing layer height");
    ui->ceilometerShowLocations->setToolTip("Display a line connecting the ceilometer plot to its location on the map");
    ui->ceilometerSize->setToolTip("Size of the ceilometer plot");
    ui->ceilometerPlotScroller->setToolTip("Vertical scroller for multiple ceilometer plots");
    ui->pandoraMinimumAltitude->setToolTip("Altitude range for pandora data");
    ui->pandoraMaximumAltitude->setToolTip("Altitude range for pandora data");
    ui->pandoraShowLocations->setToolTip("Display a line connecting the pandora plot to its location on the map");
    ui->pandoraSize->setToolTip("Size of the pandora plot");
    ui->pandoraPlotScroller->setToolTip("Vertical scroller for multiple pandora plots");
    ui->neubrewMinimumAltitude->setToolTip("Altitude range for neubrew data");
    ui->neubrewMaximumAltitude->setToolTip("Altitude range for neubrew data");
    ui->neubrewShowLocations->setToolTip("Display a line connecting the neubrew plot to its location on the map");
    ui->neubrewSize->setToolTip("Size of the neubrew plot");
    ui->neubrewPlotScroller->setToolTip("Vertical scroller for multiple neubrew plots");

    // SAVE tab
    ui->scenarioRadioButton->setToolTip("Save your current scenario (data selections, time, bounding box, etc.) so that they can be restored in a later session ");
    ui->binaryxdrRadioButton->setToolTip("Save data in a IEEE binary format with a plain-text header");
    ui->asciisheetRadioButton->setToolTip("Save data in a plain-text (ASCII) format<br><br>This can lead to large files, so use caution");
    ui->netcdfcoardsRadioButton->setToolTip("Save data in NetCDF format using the COARDS convention");
    ui->netcdfioapiRadioButton->setToolTip("Save data in NetCDF format using the IOAPI convention. This is the same format that CMAQ files use");
    ui->mpegRadioButton->setToolTip("Save visualization animation in MPEG format");
    ui->screenRecordRadioButton->setToolTip("Record all screen activity (animation, 3D zoom/pan/rotate, etc.) in MPEG format. Click 'Stop Recording' when done.");
    ui->currentpngRadioButton->setToolTip("Save the current image frame in PNG format");
    ui->allpngsRadioButton->setToolTip("Save every image frame as a separate PNG");
    ui->gzipCheckBox->setToolTip("Use gzip file compression when saving files to disk.");
    ui->dirnameLineEdit->setToolTip("Directory where files will be saved");
    ui->filenameLineEdit->setToolTip("File name of file to be saved. Note: some file names will be auto-generated and cannot be specified by the user");
    ui->savefilePushButton->setToolTip("Click to save a file in the selected format");
    ui->savevideoPushButton->setToolTip("Click to begin screen recording. Click again to stop.");
    ui->originalFileRadioButton->setToolTip("Retrieve a compressed tar file (.tgz) containing the raw data files that have not been subsetted, filtered, or processed by RSIG.\n\nWarning this can be very slow and yield ery large files (multiple GB), or possibly even fail with a timeout.");
    ui->saveMetadataCheckBox->setToolTip("When data is saved, also save additional metadata files that describe the data provenance and processing chain.");

    // RSIG WEB tab
    ui->checknowPushButton->setToolTip("Check to see if an update is available. You can also check at https://www.epa.gov/rsig");
    ui->tipsPushButton->setToolTip("Show helpful useage tips");
    ui->datausePushButton->setToolTip("Read the RSIG Data Use Policy agreement");
    ui->downloaderPushButton->setToolTip("Open the RSIG file downloader tool, which allows you to download CMAQ sector emissions and FAQSD input files");
    ui->openwebPushButton->setToolTip("Open the RSIG homepage");
    ui->emailPushButton->setToolTip("Send email to RSIG development team");
    ui->exitPushButton->setToolTip("Exit the RSIG3D application");
  }
}



QString ControlDockWidget::get_tooltip(QString var, QString units, QString description) {
  QString return_tooltip = "";
  QString coverage = get_variableCoverage(var);
  QStringList varParse = var.split(".");

  QString thisSource = varParse[1].toUpper();

  if (thisSource.compare("MODIS") == 0) {
    thisSource = thisSource + " AQUA (MYD files) and TERRA (MOD files)";
  } else if (thisSource.compare("VIIRSNOAA") == 0) {
    thisSource = thisSource + " S-NPP (npp files) and NOAA-20 (j01 files)";
  }

  return_tooltip = thisSource + ": " + description + " " + units + "\n" + "COVERAGE: " + coverage;

  // special notes
  if (thisSource.compare("PURPLEAIR") == 0) {
    return_tooltip = return_tooltip + "\n" + "NOTE: An API-key is required to access PurpleAir data. To get a key, please request one via contact@purpleair.com\nOnce you have a key, you can enter it in the Data->Filter_parameters->PurpleAir_Filter dialog.";
  }
  
  return return_tooltip;
}



QString ControlDockWidget::get_variableCoverage(QString item) {
  // Return a tooltip string if the specified item matches the list. 
  // If not, return empty string.
  QString return_string = "";
  QString prefix = "";
  QStringList months; months << "Jan" << "Feb" << "Mar" << "Apr" << "May" << "Jun" << "Jul" << "Aug" << "Sep" << "Oct" << "Nov" << "Dec";

  for (int i=0; i<availNamesSize; ++i) {

    if (availNames[i] != "") {

      if (item.contains(availNames[i] + '.')) {

        // process special cases first

        if (availNames[i].contains("acam")) {
          return_string = "July 2011: 1, 2, 5, 10, 11, 14, 20, 21, 22, 26, 27, 29";
        } else if (availNames[i].contains("hsrl")) {
          return_string = "July 2011: 1, 2, 5, 10, 11, 14, 20, 21, 22, 26, 27, 29";
        } else {

          if (availNames[i].contains("airnow2")) {
            prefix = "www.airnowapi.org server: ";
          } else if (availNames[i].contains("airnow")) {
            prefix = "csv.airnowgateway.org server: ";
          }

          // use availability that came from rsig_availability.txt
          QString minYear  = QString::number(minAvailDate[i]).mid(0,4);
          QString minMonth = QString::number(minAvailDate[i]).mid(4,2);
          QString minDay   = QString::number(minAvailDate[i]).mid(6,2);
          QString minMonthName = months[minMonth.toInt()-1];


          
          if (availNames[i].contains("tempo") &&
              ! availNames[i].contains("tempo2") &&
              ! item.contains("proxy")) { // HACK for TEMPO:
            return_string =
              "\n  Public access: Version 3 data (May 13, 2024 - present)"
              "\n  Non-key public access: Unvalidated data for these dates: "
              "Aug 2, 9, 16, 21, 22, 23, 25, 26, 2023 and Dec 17-30, 2023; "
              "\n  TEMPO Science Team access: " +
              minMonthName + " " + minDay + ", " + minYear + "  to  " ;
          } else {
            return_string = prefix + minMonthName + " " + minDay + ", " + minYear + "  to  " ;
          }

          if (maxAvailDate[i] == 99999999) {
            return_string = return_string + "present";
          } else {
            QString maxYear  = QString::number(maxAvailDate[i]).mid(0,4);
            QString maxMonth = QString::number(maxAvailDate[i]).mid(4,2);
            QString maxDay   = QString::number(maxAvailDate[i]).mid(6,2);
            QString maxMonthName = months[maxMonth.toInt()-1];
            return_string = return_string + maxMonthName + " " + maxDay + ", " + maxYear;
          }
        }
      }
    }
  }
    
  return return_string;
}



bool isCompareVar(QString item) {
  return item.contains("abs_diff.") ||
         item.contains("percent_diff.") ||
         item.contains("diff.") ||
         item.contains("ratio.");
}



void check_item(QString sd, QTreeWidget *tw) {
  QStringList parse = sd.split('.');
  bool isCompare = isCompareVar(sd);

  // get first level (e.g. "cmaq"

  QList<QTreeWidgetItem *> match_list = tw->findItems(parse[0], Qt::MatchExactly | Qt::MatchRecursive, 0);

  for (int matchInd=0; matchInd < match_list.length(); matchInd++) {
    QTreeWidgetItem * thisItem0 = match_list.value(matchInd);
    thisItem0->setExpanded(true);

    // get second level (e.g. "cdc")

    for (int childInda=0; childInda<thisItem0->childCount(); childInda++) {
      QTreeWidgetItem * thisItem1 = thisItem0->child(childInda);

      if (isCompare && sd.contains(thisItem1->text(0))) {
        thisItem1->setCheckState(0, Qt::Checked);
      } else if (thisItem1->text(0) == parse[1]) {
        thisItem1->setExpanded(true);

        if (parse.length() == 2) {
          thisItem1->setCheckState(0, Qt::Checked);
        }

        // get third level (e.g."conus")

        for (int childIndb=0; childIndb<thisItem1->childCount(); childIndb++) {
          QTreeWidgetItem * thisItem2 = thisItem1->child(childIndb);

          if (thisItem2->text(0) == parse[2]) {
            thisItem2->setExpanded(true);

            if ( (parse.length() == 3) && (thisItem2) ) {
              thisItem2->setCheckState(0, Qt::Checked);
            }

            // get fourth level (e.g. "conc")

            for (int childIndc=0; childIndc<thisItem2->childCount(); childIndc++) {
              QTreeWidgetItem * thisItem3 = thisItem2->child(childIndc);

              if (thisItem3->text(0) == parse[3]) {
                thisItem3->setExpanded(true);

                if ( (parse.length() == 4) && (thisItem3) ) {
                  thisItem3->setCheckState(0, Qt::Checked);
                }

                // get fifth level (e.g. "a25j")

                for (int childIndd=0; childIndd<thisItem3->childCount(); childIndd++) {
                  QTreeWidgetItem * thisItem4 = thisItem3->child(childIndd);

                  if (thisItem4->text(0) == parse[4]) {
                    thisItem4->setExpanded(true);

                    if ( (parse.length() == 5) && (thisItem4) ) {
                      thisItem4->setCheckState(0, Qt::Checked);
                    }

                    // get sixth level (e.g. "")

                    for (int childInde=0; childInde<thisItem4->childCount(); childInde++) {
                      QTreeWidgetItem * thisItem5 = thisItem4->child(childInde);

                      if (thisItem5->text(0) == parse[5]) {
                          thisItem5->setExpanded(true);

                        if ( (parse.length() == 6) && (thisItem5) ) {
                          thisItem5->setCheckState(0, Qt::Checked);
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
}



QString swap_compare_operator1(QString source_string) {
  
  // Ensure that data source string is properly formatted.
  // In particular, compare variables need to have operator moved to end of string

  QString return_string = source_string; // default

  if (return_string.contains("abs_diff_")) {
    return_string.replace("abs_diff_", "compare_");
    return_string.append("-abs_diff");

  } else if (return_string.contains("percent_diff_")) {
    return_string.replace("percent_diff_", "compare_");
    return_string.append("-percent_diff");

  } else if (return_string.contains("diff_")) {
    return_string.replace("diff_", "compare_");
    return_string.append("-diff");
  
  } else if (return_string.contains("ratio_")) {
    return_string.replace("ratio_", "compare_");
    return_string.append("-ratio");
  }

  return return_string;
}



/*!
 \fn QDomElement ControlDockWidget::domElement(const QString& name, QDomDocument& document) const
 
 Returns a QDomElement representation of the state of the class. 
 It is used for save the state information for a new startup.
 
 \e {name} is the name of the QDomElement, and
 \e {document} is the QDomDocument to contain this element.
 */
QDomElement ControlDockWidget::domElement(const QString& id, QDomDocument& document) const {

  QDomElement stateNode;
  VisOptions *vOptions = VisOptionsSingleton::getInstance();
  const RSIGState * gState = RSIGStateSingleton::getInstance();
  QString uniqueID = QUuid::createUuid().toString().remove("{").remove("}").remove("-");
  vOptions->setUniqueId(uniqueID);

  if (id.compare("Control") == 0) {
    stateNode = document.createElement(id);
  } else {
    stateNode = document.createElement(QString("Control"));
  }

  const QString checkStrings[3] = {
    QString("CHECK_NEVER"),
    QString("CHECK_START"),
    QString("CHECK_QUIT")
  };

  const bool isvisible = bool(m_visible>=1);
  stateNode.setAttribute("isVisible", QString::number(int(isvisible)));
  stateNode.setAttribute("activeTab", QString::number(ui->tabWidget->currentIndex()));
  stateNode.setAttribute("showLonlatLines", QString::number(ui->showLonlatCheckBox->isChecked()));
  stateNode.setAttribute("showScale", QString::number(ui->showScaleCheckBox->isChecked()));
  stateNode.setAttribute("showNavigator", QString::number(ui->showNavigatorCheckBox->isChecked()));
  stateNode.setAttribute("showDataProbe", QString::number(ui->showDataProbe->isChecked()));
  
  stateNode.setAttribute("showWidgetTooltips", QString::number(ui->chk_tooltips->isChecked()));
  stateNode.setAttribute("useDataPreferences", QString::number(ui->chkUseDataPrefs->isChecked()));
  stateNode.setAttribute("guiFontSize", QString::number(ui->guiFontsizeSpinbox->value()));

  stateNode.setAttribute("saveFile", ui->dirnameLineEdit->text());
  stateNode.setAttribute("checkUpdate", checkStrings[gState->mCheckUpdate]);
  stateNode.setAttribute("maplineFile", vOptions->getMapOverlayFile());
  stateNode.setAttribute("maplineColorIndex", vOptions->getMaplineColorIndex());
  const QPoint pos0 = this->pos();
  const QRectF rtf(pos0.x(), pos0.y(), this->width(), this->height());
  stateNode.appendChild(DomUtils::QRectFDomElement(rtf, "rect", document));
  const QString mapServer = this->currentMapServer();
  const QString mapProjection = this->currentMapProjection();
  stateNode.appendChild(DomUtils::QMapDomElement(mapServer, mapProjection, "map", document));
  const QPointF topLeft     = QPointF(vOptions->getBboxMinLon(), vOptions->getBboxMaxLat());
  const QPointF bottomRight = QPointF(vOptions->getBboxMaxLon(), vOptions->getBboxMinLat());
  const QRectF origGeoRect  = QRectF(topLeft, bottomRight);
  stateNode.appendChild(DomUtils::QRectFDomElement(origGeoRect, "roi", document));
  QDomElement nodeTime = document.createElement("time_controls");
  nodeTime.setAttribute("animation_speed", ui->animationSpeed->value());
  nodeTime.setAttribute("sliderIndex", ui->timestepSlider->value());
  nodeTime.setAttribute("timebaseIndex", gState->mTimebaseType);
  stateNode.appendChild(nodeTime);
  const QDate dat = ui->dateDateEdit->date();
  const int days = ui->durationSpinBox->value();
  stateNode.appendChild(DomUtils::DateDaysElement(dat, days, document));

  // begin SaveState

  QDomElement datanode1 = document.createElement("data1");
  QDomElement datanode2 = document.createElement("data2");
  QDomElement datanode3 = document.createElement("data3");
  QDomElement datanode4 = document.createElement("data4");
  QDomElement datanode5 = document.createElement("data5");
  QDomElement* datanodePtr = 0;

  // Dataset (variable) names:

  datanode1.setAttribute("src", ui->selectedData1->toPlainText());
  datanode2.setAttribute("src", ui->selectedData2->toPlainText());
  datanode3.setAttribute("src", ui->selectedData3->toPlainText());
  datanode4.setAttribute("src", ui->selectedData4->toPlainText());
  datanode5.setAttribute("src", ui->selectedData5->toPlainText());

  // dataName defaults
  datanode1.setAttribute("dataName", "");
  datanode2.setAttribute("dataName", "");
  datanode3.setAttribute("dataName", "");
  datanode4.setAttribute("dataName", "");
  datanode5.setAttribute("dataName", "");

  // aggregation defaults

  datanode1.setAttribute("aggregateIndex", "-1");
  datanode2.setAttribute("aggregateIndex", "-1");
  datanode3.setAttribute("aggregateIndex", "-1");
  datanode4.setAttribute("aggregateIndex", "-1");
  datanode5.setAttribute("aggregateIndex", "-1");

  // specific aggregation

  if (!ui->aggregate1->isHidden()) { datanode1.setAttribute("aggregateIndex", QString::number(ui->aggregate1->currentIndex())); }
  if (!ui->aggregate2->isHidden()) { datanode2.setAttribute("aggregateIndex", QString::number(ui->aggregate2->currentIndex())); }
  if (!ui->aggregate3->isHidden()) { datanode3.setAttribute("aggregateIndex", QString::number(ui->aggregate3->currentIndex())); }
  if (!ui->aggregate4->isHidden()) { datanode4.setAttribute("aggregateIndex", QString::number(ui->aggregate4->currentIndex())); }
  if (!ui->aggregate5->isHidden()) { datanode5.setAttribute("aggregateIndex", QString::number(ui->aggregate5->currentIndex())); }
           
  /*
   * Save pathed file names of currently-loaded datasets:
   *   src1="cmaq.cdc.conus.conc.o3" src2="" src3="" ...
   *   file1="/Users/plessel/.rsig/cmaq_cdc_conus_conc_o3_2008-07-03.xdr"
   */

  const size_t count = 5;
  const QString sourceNames[ count ] = {
    swap_compare_operator1(ui->selectedData1->toPlainText().replace( '.', '_' )),
    swap_compare_operator1(ui->selectedData2->toPlainText().replace( '.', '_' )),
    swap_compare_operator1(ui->selectedData3->toPlainText().replace( '.', '_' )),
    swap_compare_operator1(ui->selectedData4->toPlainText().replace( '.', '_' )),
    swap_compare_operator1(ui->selectedData5->toPlainText().replace( '.', '_' ))
  };

  // loop over 5 data "slots"

  for ( size_t index = 0; index < count; ++index ) {

    switch (index) {
    case 0:
      datanodePtr = &datanode1;
      break;
    case 1:
      datanodePtr = &datanode2;
      break;
    case 2:
      datanodePtr = &datanode3;
      break;
    case 3:
      datanodePtr = &datanode4;
      break;
    case 4:
      datanodePtr = &datanode5;
      break;
    default:
      break;
    }

    const QString& sourceName = sourceNames[ index ];
    bool regridFlag = false;

    //qDebug() << sourceName;

    // Save attributes (minval, maxval, colormapindex, isDefaultRangeChecked):

    double thisMinVal              = -9999.0; // Default values if no data object
    double thisMaxVal              = -9999.0;
    qreal thisAlpha               = 1.0;
    int   thisColormapIndex       = 0;
    int   thisDefaultRangeChecked = 1;
    int   thisShowCellBoundary    = 0;
    int   thisShowThreshold       = 0;
    int   thisThresholdIndex      = 0;

    const char* fileName = ""; // This will point into one of the objects below.
    std::string datasetFileName; // Must ensure destructor not called
    // until after last use of fileName pointer in setAttribute() call below!
    QByteArray inBytes; // Ditto for this object!

    const int dataIndex = // Index of data object for sourceName.
      sourceName.isEmpty() ? -1 : getIndexOfGeoDataObjectForSource(sourceName);

    if ( dataIndex != -1 ) {
      const int visIndex = getIndexOfVisDataObjectForSource( sourceName );

      if ( visIndex != -1 ) {
        const VisDataList* const visDataList = VisDataListSingleton::getInstance();
        assert( visDataList );
        const VisDataObject* const visDataObject = visDataList->getObjectByIndex( visIndex );
        assert( visDataObject );
        const GeoDataList* const geoDataList = GeoDataListSingleton::getInstance();
        assert( geoDataList );
        const GeoDataObject* const geoDataObject = geoDataList->getObject( dataIndex );
        assert( geoDataObject );
        const emvl::Dataset* const dataset = geoDataObject->model();
        assert( dataset );
        QString visName( visDataObject->objectName() );
        const int dot = visName.indexOf( '.' );

        if ( dot != -1 ) {
          visName.truncate( dot );
        }

        datanodePtr->setAttribute( "dataName", visName );
        datasetFileName = dataset->fileName();
        fileName = datasetFileName.c_str();

        if ( id.compare( "Control" ) != 0 ) {

          // Make a copy of file to use with user-saved scenario:

          QFileInfo finfo = QFileInfo( fileName );
          QString custom_fname =
          finfo.absolutePath() + emvl::slash + id + '_' + finfo.fileName();
          QFile::copy( QString( fileName ), custom_fname );

          // Override fileName:

          inBytes = custom_fname.toUtf8();
          fileName = inBytes.constData();
        }

        // Get object attribute values:

        thisMinVal              = visDataObject->minval();
        thisMaxVal              = visDataObject->maxval();
        thisColormapIndex       = visDataObject->curColormapIndex();
        thisDefaultRangeChecked = visDataObject->isDefaultRangeChecked();
        thisAlpha               = visDataObject->alpha();
        thisShowCellBoundary    = visDataObject->showCellBoundary();
        thisShowThreshold       = visDataObject->ShowThreshold();
        thisThresholdIndex      = visDataObject->thresholdSliderValue();
      }
    }

    datanodePtr->setAttribute("minval", QString::number(thisMinVal, 'g'));
    datanodePtr->setAttribute("maxval", QString::number(thisMaxVal, 'g'));
    datanodePtr->setAttribute("colormapIndex", thisColormapIndex);
    datanodePtr->setAttribute("defaultRangeChecked", thisDefaultRangeChecked);
    datanodePtr->setAttribute("alpha", thisAlpha);
    datanodePtr->setAttribute("showCellBoundary", thisShowCellBoundary);
    datanodePtr->setAttribute("showThreshold", thisShowThreshold);
    datanodePtr->setAttribute("thresholdIndex", thisThresholdIndex);
    datanodePtr->setAttribute( "file", fileName );
    datanodePtr->setAttribute( "regrid", ((regridFlag==true) ? 1 : 0) );
  } // End for loop on data menu selections.


  // additional variables that are loaded automatically
  //QDomElement datanodeExtra1 = document.createElement("dataExtra1");
  //QDomElement datanodeExtra2 = document.createElement("dataExtra2");
  //QDomElement datanodeExtra3 = document.createElement("dataExtra3");
  //QDomElement datanodeExtra4 = document.createElement("dataExtra4");
  //QDomElement datanodeExtra5 = document.createElement("dataExtra5");
  //// dataName defaults
  //datanodeExtra1.setAttribute("dataName", "");
  //datanodeExtra2.setAttribute("dataName", "");
  //datanodeExtra3.setAttribute("dataName", "");
  //datanodeExtra4.setAttribute("dataName", "");
  //datanodeExtra5.setAttribute("dataName", "");

  int counter = 1;
  // hourly pandora
  const VisDataList* const visDataList = VisDataListSingleton::getInstance();
  for (int listInd=0; listInd<visDataList->size(); listInd++) {
    const VisDataObject* const visObj = visDataList->getObjectByIndex( listInd );
    if (visObj) {
      GeoDataObject* geoObj = qobject_cast<GeoDataObject*>(visObj->parent());
      const emvl::Dataset* const dataset = geoObj->model();
      QString visName( visObj->objectName() );
      QString visNamePrefix( visObj->objectName() );
      const int dot = visName.indexOf( '.' );
      if ( dot != -1 ) {
        visNamePrefix.truncate( dot );
      }

      if (visName.startsWith("hourly_pandora") && !visName.endsWith(".lut")) {
        QString nodeName = "dataExtra" + QString::number(counter);
        QDomElement myDatanodeExtra = document.createElement(nodeName);
        datanodePtr = &myDatanodeExtra;
        
        datanodePtr->setAttribute( "dataName", visNamePrefix );
        std::string datasetFileName = dataset->fileName();
        const char* fileName = datasetFileName.c_str();

        // Make a copy of file to use with user-saved scenario:
        QFileInfo finfo = QFileInfo( fileName );
        QString custom_fname = finfo.absolutePath() + emvl::slash + id + '_' + finfo.fileName();
        QFile::copy( QString( fileName ), custom_fname );
        // Override fileName:
        QByteArray inBytes = custom_fname.toUtf8();
        fileName = inBytes.constData();

        // Get object attribute values:
        double thisMinVal             = visObj->minval();
        double thisMaxVal             = visObj->maxval();
        int   thisColormapIndex       = visObj->curColormapIndex();
        int   thisDefaultRangeChecked = visObj->isDefaultRangeChecked();
        qreal thisAlpha               = visObj->alpha();
        int   thisShowCellBoundary    = visObj->showCellBoundary();
        int   thisShowThreshold       = visObj->ShowThreshold();
        int   thisThresholdIndex      = visObj->thresholdSliderValue();
        
        datanodePtr->setAttribute("minval", QString::number(thisMinVal, 'g'));
        datanodePtr->setAttribute("maxval", QString::number(thisMaxVal, 'g'));
        datanodePtr->setAttribute("colormapIndex", thisColormapIndex);
        datanodePtr->setAttribute("defaultRangeChecked", thisDefaultRangeChecked);
        datanodePtr->setAttribute("alpha", thisAlpha);
        datanodePtr->setAttribute("showCellBoundary", thisShowCellBoundary);
        datanodePtr->setAttribute("showThreshold", thisShowThreshold);
        datanodePtr->setAttribute("thresholdIndex", thisThresholdIndex);
        datanodePtr->setAttribute( "file", fileName );

        stateNode.appendChild(myDatanodeExtra);
        
        counter += 1;
      }
    }
  }
  
  // Write attributes that are not part of data objects:

  // Save layer settings:
  datanode1.setAttribute("layer", ui->selectedLayer1->value());
  datanode2.setAttribute("layer", ui->selectedLayer2->value());
  datanode3.setAttribute("layer", ui->selectedLayer3->value());
  datanode4.setAttribute("layer", ui->selectedLayer4->value());
  datanode5.setAttribute("layer", ui->selectedLayer5->value());

  stateNode.appendChild(datanode1);
  stateNode.appendChild(datanode2);
  stateNode.appendChild(datanode3);
  stateNode.appendChild(datanode4);
  stateNode.appendChild(datanode5);

  //stateNode.appendChild(datanodeExtra1);
  //stateNode.appendChild(datanodeExtra2);
  //stateNode.appendChild(datanodeExtra3);
  //stateNode.appendChild(datanodeExtra4);
  //stateNode.appendChild(datanodeExtra5);

  QDomElement node2 = document.createElement("vis_controls");
  node2.setAttribute("vertical_scalefactor", ui->calipsoScaleFactor->value());
  node2.setAttribute("vertical_ruler_show", vOptions->getShowRuler());
  node2.setAttribute("vertical_ruler_lat", vOptions->getRulerLat());
  node2.setAttribute("vertical_ruler_lon", vOptions->getRulerLon());
  node2.setAttribute("windvector_scalefactor", ui->windVectorScaleFactor->value());
  node2.setAttribute("windvector_tipsize", ui->windVectorTipSize->value());
  node2.setAttribute("windvector_sparseness", ui->windVectorSparseness->value());
  node2.setAttribute("windvector_autosparse", vOptions->getWindVectorAutosparse());
  stateNode.appendChild(node2);

  // MODIS TrueColor
  QDomElement node4 = document.createElement("truecolor");
  node4.setAttribute("draw_truecolor", vOptions->getDrawModisTrueColor());

  QString curTrueColor = vOptions->getTrueColorType();
  node4.setAttribute("truecolor_type", curTrueColor);
  node4.setAttribute("truecolor_uniqueID", uniqueID);
  node4.setAttribute("truecolor_ndays",ui->durationSpinBox->value());
  stateNode.appendChild(node4);

  const int ndays = ui->durationSpinBox->value();

  for (int daynum=0; daynum<ndays; daynum++) {
    const QDate xdate = ui->dateDateEdit->date().addDays(daynum);
    QString yyyy = QString("%1").arg(xdate.year(),  4, 10, QChar('0'));
    QString mm   = QString("%1").arg(xdate.month(), 2, 10, QChar('0'));
    QString dd   = QString("%1").arg(xdate.day(),   2, 10, QChar('0'));
    QString yyyymmdd = yyyy + mm + dd;
    QString filename_4326 = QDir::homePath() + "/.rsig/trueColor_" + curTrueColor + "_" + yyyymmdd +"_4326.png";
    QString filename_3857 = QDir::homePath() + "/.rsig/trueColor_" + curTrueColor + "_" + yyyymmdd +"_3857.png";
    QFile::rename(filename_4326, filename_4326+"_"+uniqueID);
    QFile::rename(filename_3857, filename_3857+"_"+uniqueID);
  }

  // Regrid settings
  QDomElement node5 = document.createElement("regrid_parameters");
  RegridParmsData* regridData = RegridParmsDialog::getRegridParmsData();
  node5.setAttribute("cmaqGrid",     regridData->cmaqGrid);
  node5.setAttribute("regridMethod", regridData->regridMethod);
  node5.setAttribute("aggregation",  regridData->regridAggregation);
  node5.setAttribute("NCOLS",        regridData->NCOLS);
  node5.setAttribute("NROWS",        regridData->NROWS);
  node5.setAttribute("NLAYS",        regridData->NLAYS);
  node5.setAttribute("GDTYP",        regridData->GDTYP);
  node5.setAttribute("RADIUS",       regridData->RADIUS);
  node5.setAttribute("PALP",         regridData->PALP);
  node5.setAttribute("PBET",         regridData->PBET);
  node5.setAttribute("PGAM",         regridData->PGAM);
  node5.setAttribute("XCENT",        regridData->XCENT);
  node5.setAttribute("YCENT",        regridData->YCENT);
  node5.setAttribute("XORIG",        regridData->XORIG);
  node5.setAttribute("YORIG",        regridData->YORIG);
  node5.setAttribute("XCELL",        regridData->XCELL);
  node5.setAttribute("YCELL",        regridData->YCELL);
  node5.setAttribute("VGTYP",        regridData->VGTYP);
  node5.setAttribute("VGTOP",        regridData->VGTOP);
  node5.setAttribute("G",            regridData->G);
  node5.setAttribute("R",            regridData->R);
  node5.setAttribute("A",            regridData->A);
  node5.setAttribute("T0",           regridData->T0);
  node5.setAttribute("P0",           regridData->P0);
  node5.setAttribute("VGLVLS",       regridData->VGLVLS.c_str());
  stateNode.appendChild(node5);

  // Filter settings
  QDomElement node6 = document.createElement("filter_parameters");
  FilterSettingData* filterData = FilterSettingDialog::getFilterSetting();
  node6.setAttribute("CADmin",         filterData->CADmin);
  node6.setAttribute("UncertaintyMax", filterData->UncertaintyMax);
  node6.setAttribute("AODmin",         filterData->AODmin);
  node6.setAttribute("AODmax",         filterData->AODmax);
  node6.setAttribute("AODSTDmin",      filterData->AODSTDmin);
  node6.setAttribute("AODSTDmax",      filterData->AODSTDmax);
  node6.setAttribute("CLSmin",         filterData->CLSmin);
  node6.setAttribute("CLSmax",         filterData->CLSmax);
  node6.setAttribute("CH1min",         filterData->CH1min);
  node6.setAttribute("CH1max",         filterData->CH1max);
  node6.setAttribute("SIGmin",         filterData->SIGmin);
  node6.setAttribute("SIGmax",         filterData->SIGmax);
  node6.setAttribute("SFCmin",         filterData->SFCmin);
  node6.setAttribute("SFCmax",         filterData->SFCmax);
  node6.setAttribute("MOSmin",         filterData->MOSmin);
  node6.setAttribute("MOSmax",         filterData->MOSmax);
  node6.setAttribute("SCAmin",         filterData->SCAmin);
  node6.setAttribute("SCAmax",         filterData->SCAmax);
  node6.setAttribute("TropomiQuality", filterData->TropomiQuality);
  node6.setAttribute("CALIPSOL1DataVersion", filterData->CALIPSOL1DataVersion);
  node6.setAttribute("CALIPSOL2DataVersion", filterData->CALIPSOL2DataVersion);
  stateNode.appendChild(node6);

  // Persist settings
  QDomElement node7 = document.createElement("swath_persist");
  node7.setAttribute("persistBEHR",         vOptions->getPersistBEHR());
  node7.setAttribute("persistBEHRHours",    vOptions->getPersistBEHRHours());
  node7.setAttribute("persistMODIS",        vOptions->getPersistMODIS());
  node7.setAttribute("persistMODISHours",   vOptions->getPersistMODISHours());
  node7.setAttribute("persistVIIRS",        vOptions->getPersistVIIRS());
  node7.setAttribute("persistVIIRSHours",   vOptions->getPersistVIIRSHours());
  node7.setAttribute("persistTROPOMI",      vOptions->getPersistTROPOMI());
  node7.setAttribute("persistTROPOMIHours", vOptions->getPersistTROPOMIHours());
  node7.setAttribute("persistTEMPO",        vOptions->getPersistTEMPO());
  node7.setAttribute("persistTEMPOHours",   vOptions->getPersistTEMPOHours());
  node7.setAttribute("persistHMS",          vOptions->getPersistHMS());
  node7.setAttribute("persistHMSHours",     vOptions->getPersistHMSHours());
  stateNode.appendChild(node7);

  return stateNode;
}



/*
 * Get index of data VisDataObject associated with data source menu item.
 * Search the VisDataList for a VisDataOject with name starting with sourceName
 * and then return the matching index or -1 if not found.
 * UGLY: This should be more straightforward - i.e., not requiring a search!
 */

int ControlDockWidget::getIndexOfVisDataObjectForSource(const QString& source) {
  int result = -1;
  VisDataList* const visDataList = VisDataListSingleton::getInstance();
  const int count = visDataList->size();

  for ( int index = 0; index < count; ++index ) {
    const VisDataObject* const visDataObject =
      visDataList->getObjectByIndex( index );

    if ( visDataObject ) {
      const QString visDataObjectName( visDataObject->objectName() );

      // Consider only data objects not lut:

      if ( ! visDataObjectName.endsWith( ".lut" ) ) {

        if ( visDataObjectName.startsWith( source ) ) {
          result = index;
          index = count; // Stop looping.
        }
      }
    }
  }

  return result;
}



/*
 * Get index of GeoDataObject associated with data source menu item.
 * Search the GeoDataList for a GeoDataOject with name starting with sourceName
 * and then return the matching index or -1 if not found.
 * UGLY: This should be more straightforward - i.e., not requiring a search!
 */

int ControlDockWidget::getIndexOfGeoDataObjectForSource(const QString& source) {
  int result = -1;
  GeoDataList* const geoDataList = GeoDataListSingleton::getInstance();
  const int count = geoDataList->size();

  for ( int index = 0; index < count; ++index ) {
    const GeoDataObject* const geoDataObject = geoDataList->getObject( index );

    if ( geoDataObject ) {
      const QString geoDataObjectName( geoDataObject->varName() );

      if ( geoDataObjectName.startsWith( source ) ) {
        result = index;
        index = count; // Stop looping.
      }
    }
  }

  return result;
}




/*!
 \fn void ControlDockWidget::initFromDOMElement(const QDomElement& element)
 
 Initialize the class itself from a QDomElement which contains the state information.
 It is used for recovering from a new startup.
 
 \e {element} is the instance of the QDomElement.
 */
void ControlDockWidget::initFromDOMElement(const QDomElement& element) {
  RSIGState* gState = RSIGStateSingleton::getInstance();

  QRectF boundingBox;
  RSIGScene& rsigScene = RSIGScene::getInstance();
  VisOptions* vOptions = VisOptionsSingleton::getInstance();
  std::vector<std::string> fileNames;
  std::vector<std::string> datasetNames;
  rsigScene.numCeilometerVars = 0;
  rsigScene.ceilometerCCACreated = false;
  rsigScene.ceilometerShowRegridWarning = true;
  rsigScene.numPandoraVars = 0;
  rsigScene.pandoraCCACreated = false;
  rsigScene.pandoraShowRegridWarning = true;
  rsigScene.neubrewShowRegridWarning = true;

  //----------------------- Delete all existing data -------------------------

  // remove old items from the visualization combo box
  int numItems = ui->vizVarCombobox->count();

  for (int ind=0; ind<=numItems; ind++) {
    //always remove item 0, because the combobox shrinks every time removeItem() runs
    ui->vizVarCombobox->removeItem(0);
  }

  /*
   * UGLY. Must call geoDataList->clear() before datasetManager->removeAll()
   * because geoDataList->clear() zeros each GeoDataObject's mObject/Dataset
   * pointer before DatasetManqer deletes those Dataset objects
   * so that when GeoDataObject's destructor is called (later via event) it
   * won't have dangling references to deleted objects!
   */

  GeoDataList* const geoDataList = GeoDataListSingleton::getInstance();
  geoDataList->clear(); // Delete any GeoDataObjects.
  emvl::DatasetManager* const datasetManager = getDatasetManager();
  datasetManager->removeAll(); // Delete any loaded datasets.

  //-------------------------- Read new state/data ---------------------------

  for ( QDomElement child = element.firstChild().toElement();
        ! child.isNull();
        child = child.nextSibling().toElement() ) {
    const QString tagName( child.tagName() );

    if (tagName == "rect" ) {
      const QRectF r = DomUtils::QRectFFromDom(child);
      this->move(r.x(), r.y());
      this->doResize(r.width(), r.height());
    } else if ( tagName == "roi" ) {
      boundingBox = DomUtils::QRectFFromDom(child);
      const float left   = boundingBox.left();
      const float right  = boundingBox.right();
      const float bottom = boundingBox.bottom();
      const float top    = boundingBox.top();
      const float minlon = std::min<float>( left, right );
      const float maxlon = std::max<float>( left, right );
      const float minlat = std::min<float>( bottom, top );
      const float maxlat = std::max<float>( bottom, top );
      vOptions->setBboxMinLon(minlon);
      vOptions->setBboxMinLat(minlat);
      vOptions->setBboxMaxLon(maxlon);
      vOptions->setBboxMaxLat(maxlat);
      const QPointF topLeft     = QPointF(vOptions->getBboxMinLon(), vOptions->getBboxMaxLat());
      const QPointF bottomRight = QPointF(vOptions->getBboxMaxLon(), vOptions->getBboxMinLat());
      const QRectF origGeoRect  = QRectF(topLeft, bottomRight);
      this->setRegionOfInterest(origGeoRect);
    } else if ( tagName == "map" ) {
      QString server, projection;
      DomUtils::QMapFromDom(child, server, projection);
      this->setCurrentMapServer(server);
      this->setCurrentMapProjection(projection);
    } else if ( tagName == "datedays" ) {
      QDate dat;
      int days;
      DomUtils::DateDaysFromDom(child, dat, days);
      ui->dateDateEdit->setDate(dat);
      ui->durationSpinBox->setValue(days);
    } else if ( tagName == "data" ) { // This is for backward campatibility with older scenario files
      rsigScene.numCeilometerVars = 0;
      rsigScene.numPandoraVars = 0;
      // begin RestoreState
      const QString sd1( swap_compare_operator1(DomUtils::stringFromDom(child, "src1", "")));
      const QString sd2( swap_compare_operator1(DomUtils::stringFromDom(child, "src2", "")));
      const QString sd3( swap_compare_operator1(DomUtils::stringFromDom(child, "src3", "")));
      const QString sd4( swap_compare_operator1(DomUtils::stringFromDom(child, "src4", "")));
      const QString sd5( swap_compare_operator1(DomUtils::stringFromDom(child, "src5", "")));

      // set proper tree checkboxes
      QTreeWidget* tw = ui->varTree;
      check_item(sd1, tw);
      check_item(sd2, tw);
      check_item(sd3, tw);
      check_item(sd4, tw);
      check_item(sd5, tw);

      // Get matched pathed file names of datasets to reload:

      const int count = 5;
      const QString sourceNames[ count ] = { sd1, sd2, sd3, sd4, sd5 };
      std::vector<std::string> fileNames;
      std::vector<std::string> datasetNames;

      for ( int index = 0; index < count; ++index ) {
        const QString& sourceName = sourceNames[ index ];

        if ( sourceName.size() > 0 ) {
          char label[ 32 ] = "";
          memset( label, 0, sizeof label );
          snprintf( label, sizeof label / sizeof *label, "file%d", index + 1 );
          const QString qString( DomUtils::stringFromDom( child, label, "" ) );
          fileNames.push_back( qString.toStdString() ); //x
          datasetNames.push_back( sourceName.toStdString()); //x

          if (sourceName.toLower().contains("ceilometer")) {
            rsigScene.numCeilometerVars += 1;
          }
          if (sourceName.toLower().contains("pandora")) {
            rsigScene.numPandoraVars += 1;
          }
        }
      }

      rsigScene.ceilometerCCACreated = false;
      rsigScene.ceilometerShowRegridWarning = true;
      rsigScene.pandoraCCACreated = false;
      rsigScene.pandoraShowRegridWarning = true;
      rsigScene.neubrewShowRegridWarning = true;

      rsigScene.ceilometerCurtainComputed.resize(4);
      for (size_t ceilInd=0; ceilInd<rsigScene.ceilometerCurtainComputed.size(); ceilInd++) {
        rsigScene.ceilometerCurtainComputed[ceilInd] = false;
      }
      rsigScene.pandoraCurtainComputed.resize(4);
      for (size_t pandoraInd=0; pandoraInd<rsigScene.pandoraCurtainComputed.size(); pandoraInd++) {
        rsigScene.pandoraCurtainComputed[pandoraInd] = false;
      }

      reloadDatasets( boundingBox, datasetNames, fileNames );

      int sLayer1, sLayer2, sLayer3, sLayer4, sLayer5;
      sLayer1 = DomUtils::stringFromDom(child, "layer1", "").toInt();
      sLayer2 = DomUtils::stringFromDom(child, "layer2", "").toInt();
      sLayer3 = DomUtils::stringFromDom(child, "layer3", "").toInt();
      sLayer4 = DomUtils::stringFromDom(child, "layer4", "").toInt();
      sLayer5 = DomUtils::stringFromDom(child, "layer5", "").toInt();
      ui->selectedLayer1->setValue(sLayer1);
      ui->selectedLayer2->setValue(sLayer2);
      ui->selectedLayer3->setValue(sLayer3);
      ui->selectedLayer4->setValue(sLayer4);
      ui->selectedLayer5->setValue(sLayer5);

      int sRegrid1, sRegrid2, sRegrid3, sRegrid4, sRegrid5;
      sRegrid1 = DomUtils::stringFromDom(child, "regrid1", "").toInt();
      sRegrid2 = DomUtils::stringFromDom(child, "regrid2", "").toInt();
      sRegrid3 = DomUtils::stringFromDom(child, "regrid3", "").toInt();
      sRegrid4 = DomUtils::stringFromDom(child, "regrid4", "").toInt();
      sRegrid5 = DomUtils::stringFromDom(child, "regrid5", "").toInt();
      ui->regridCheckbox1->setCheckState(sRegrid1 == 0 ? Qt::Unchecked : Qt::Checked);
      ui->regridCheckbox2->setCheckState(sRegrid2 == 0 ? Qt::Unchecked : Qt::Checked);
      ui->regridCheckbox3->setCheckState(sRegrid3 == 0 ? Qt::Unchecked : Qt::Checked);
      ui->regridCheckbox4->setCheckState(sRegrid4 == 0 ? Qt::Unchecked : Qt::Checked);
      ui->regridCheckbox5->setCheckState(sRegrid5 == 0 ? Qt::Unchecked : Qt::Checked);

    } else if ( tagName == "data1"      ||
                tagName == "data2"      ||
                tagName == "data3"      ||
                tagName == "data4"      ||
                tagName == "data5"      ||
                tagName == "dataExtra1" ||
                tagName == "dataExtra2" ||
                tagName == "dataExtra3" ||
                tagName == "dataExtra4" ||
                tagName == "dataExtra5" ) {

      // begin RestoreState
      bool isExtra = tagName.contains("Extra");

      QString dataName = DomUtils::stringFromDom(child, "dataName", "");
            
      QString sd;
      if (isExtra) {
        sd = dataName;
      } else {
        sd = swap_compare_operator1(DomUtils::stringFromDom(child, "src", ""));
        // set proper tree checkboxes
        QTreeWidget* tw = ui->varTree;
        check_item(sd, tw);
      }
      

      // set mapping between source names and data object names
      QStringList dataSourceList = vOptions->getDataSourceList();

      if ( ! sd.isEmpty() && ! dataName.isEmpty() ) {
        dataSourceList << sd + "#" + dataName;
      }

      vOptions->setDataSourceList(dataSourceList);

      // Get matched pathed file names of datasets to reload:

      const QString& sourceName = sd;

      if ( sourceName.size() > 0 ) {
        datasetNames.clear();
        fileNames.clear();
        const QString qString( DomUtils::stringFromDom( child, "file", "" ) );
        fileNames.push_back( qString.toStdString() ); //x
        datasetNames.push_back( sourceName.toStdString()); //x
        reloadDatasets( boundingBox, datasetNames, fileNames );

        if (sourceName.toLower().contains("ceilometer")) {
          rsigScene.numCeilometerVars += 1;
        }

        if (sourceName.toLower().contains("pandora") && !sourceName.toLower().contains("hourly")) {
          rsigScene.numPandoraVars += 1;
        }
      }
      
      rsigScene.ceilometerCurtainComputed.resize(4);
      for (size_t ceilInd=0; ceilInd<rsigScene.ceilometerCurtainComputed.size(); ceilInd++) {
        rsigScene.ceilometerCurtainComputed[ceilInd] = false;
      }

      rsigScene.pandoraCurtainComputed.resize(4);
      for (size_t pandoraInd=0; pandoraInd<rsigScene.pandoraCurtainComputed.size(); pandoraInd++) {
        rsigScene.pandoraCurtainComputed[pandoraInd] = false;
      }

      int sLayer = 0;
      int sRegrid = 0;
      Qt::CheckState checkedState = Qt::Unchecked;
      int sAggregateIndex = 0;
      if (!isExtra) {
        sLayer                  = DomUtils::stringFromDom(child, "layer", "").toInt();
        sRegrid                 = DomUtils::stringFromDom(child, "regrid", "").toInt();
        checkedState = sRegrid ? Qt::Checked : Qt::Unchecked;
        sAggregateIndex         = DomUtils::stringFromDom( child, "aggregateIndex", "" ).toInt();
      }
      
     if ( tagName == "data1" ) {
        ui->selectedLayer1->setValue( sLayer );
        ui->regridCheckbox1->setCheckState( checkedState );
        ui->aggregate1->setCurrentIndex( sAggregateIndex );

         if ( sAggregateIndex == -1 ) {
           ui->aggregate1->hide();
         } else {
           ui->aggregate1->show();
         }

      } else if ( tagName == "data2" ) {
        ui->selectedLayer2->setValue( sLayer );
        ui->regridCheckbox2->setCheckState( checkedState );
        ui->aggregate2->setCurrentIndex( sAggregateIndex );

         if ( sAggregateIndex == -1 ) {
           ui->aggregate2->hide();
         } else {
           ui->aggregate2->show();
         }

      } else if ( tagName == "data3" ) {
        ui->selectedLayer3->setValue( sLayer );
        ui->regridCheckbox3->setCheckState( checkedState );
        ui->aggregate3->setCurrentIndex( sAggregateIndex );

         if ( sAggregateIndex == -1 ) {
           ui->aggregate3->hide();
         } else {
           ui->aggregate3->show();
         }

      } else if ( tagName == "data4" ) {
        ui->selectedLayer4->setValue( sLayer );
        ui->regridCheckbox4->setCheckState( checkedState );
        ui->aggregate4->setCurrentIndex( sAggregateIndex );

         if ( sAggregateIndex == -1 ) {
           ui->aggregate4->hide();
         } else {
           ui->aggregate4->show();
         }

      } else if ( tagName == "data5" ) {
        ui->selectedLayer5->setValue( sLayer );
        ui->regridCheckbox5->setCheckState( checkedState );
        ui->aggregate5->setCurrentIndex( sAggregateIndex );

         if ( sAggregateIndex == -1 ) {
           ui->aggregate5->hide();
         } else {
           ui->aggregate5->show();
         }
      }

      // set object attributes
      VisDataList* vislist = VisDataListSingleton::getInstance();
      QString visNamePolygon          = sd.replace(".", "_") + ".polygon";
      QString visNamePoint            = sd.replace(".", "_") + ".point";
      QString visNameVector           = sd.replace(".", "_") + ".vector";
      QString minvalString            = "minval";
      QString maxvalString            = "maxval";
      QString ciString                = "colormapIndex";
      QString drcString               = "defaultRangeChecked";
      QString alphaString             = "alpha";
      QString showCellBoundaryString  = "showCellBoundary";
      QString showThresholdString     = "showThreshold";
      QString thresholdIndexString    = "thresholdIndex";
      double  thisMinval              = (double)DomUtils::stringFromDom(child, minvalString, "").toDouble();
      double  thisMaxval              = (double)DomUtils::stringFromDom(child, maxvalString, "").toDouble();
      int     thisColormapIndex       =        DomUtils::stringFromDom(child, ciString, "").toInt();
      bool    thisDefaultRangeChecked =  (bool)DomUtils::stringFromDom(child, drcString, "").toInt();
      qreal   thisAlpha               = (qreal)DomUtils::stringFromDom(child, alphaString, "").toFloat();
      bool    thisShowCellBoundary    =  (bool)DomUtils::stringFromDom(child, showCellBoundaryString, "").toInt();
      bool    thisShowThreshold       =  (bool)DomUtils::stringFromDom(child, showThresholdString, "").toInt();
      int     thisThresholdIndex      =        DomUtils::stringFromDom(child, thresholdIndexString, "").toInt();

      VisDataObject* thisVisObjPolygon = vislist->getObjectByName(visNamePolygon);
      VisDataObject* thisVisObjPoint   = vislist->getObjectByName(visNamePoint);
      VisDataObject* thisVisObjVector  = vislist->getObjectByName(visNameVector);

      if ( thisVisObjPolygon ) {

        if ( thisMinval != -9999 ) {
          thisVisObjPolygon->setMinval(thisMinval);
          thisVisObjPolygon->setLastMinval(thisMinval);
        }

        if ( thisMaxval != -9999 ) {
          thisVisObjPolygon->setMaxval(thisMaxval);
          thisVisObjPolygon->setLastMaxval(thisMaxval);
        }

        thisVisObjPolygon->setCurColormapIndex(thisColormapIndex);
        onColormapIndexChanged(thisColormapIndex);
        thisVisObjPolygon->setDefaultRangeChecked(thisDefaultRangeChecked);
        thisVisObjPolygon->setAlpha(thisAlpha);
        thisVisObjPolygon->setShowCellBoundary(thisShowCellBoundary);
        thisVisObjPolygon->setShowThreshold(thisShowThreshold);
        thisVisObjPolygon->setThresholdSliderValue(thisThresholdIndex);
        onThresholdSliderChanged(thisThresholdIndex);
        setVisWidgetState(thisVisObjPolygon);
      }

      if ( thisVisObjPoint ) {

        if ( thisMinval != -9999 ) {
          thisVisObjPoint->setMinval(thisMinval);
          thisVisObjPoint->setLastMinval(thisMinval);
        }

        if ( thisMaxval != -9999 ) {
          thisVisObjPoint->setMaxval(thisMaxval);
          thisVisObjPoint->setLastMaxval(thisMaxval);
        }

        thisVisObjPoint->setCurColormapIndex(thisColormapIndex);
        onColormapIndexChanged(thisColormapIndex);
        thisVisObjPoint->setDefaultRangeChecked(thisDefaultRangeChecked);
        thisVisObjPoint->setAlpha(thisAlpha);
        thisVisObjPoint->setShowCellBoundary(thisShowCellBoundary);
        thisVisObjPoint->setShowThreshold(thisShowThreshold);
        thisVisObjPoint->setThresholdSliderValue(thisThresholdIndex);
        onThresholdSliderChanged(thisThresholdIndex);
        setVisWidgetState(thisVisObjPoint);
      }

      if ( thisVisObjVector ) {

        if ( thisMinval != -9999 ) {
          thisVisObjVector->setMinval(thisMinval);
          thisVisObjVector->setLastMinval(thisMinval);
        }

        if ( thisMaxval != -9999 ) {
          thisVisObjVector->setMaxval(thisMaxval);
          thisVisObjVector->setLastMaxval(thisMaxval);
        }

        thisVisObjVector->setCurColormapIndex(thisColormapIndex);
        onColormapIndexChanged(thisColormapIndex);
        thisVisObjVector->setDefaultRangeChecked(thisDefaultRangeChecked);
        thisVisObjVector->setAlpha(thisAlpha);
        thisVisObjVector->setShowCellBoundary(thisShowCellBoundary);
        thisVisObjVector->setShowThreshold(thisShowThreshold);
        thisVisObjVector->setThresholdSliderValue(thisThresholdIndex);
        onThresholdSliderChanged(thisThresholdIndex);
        setVisWidgetState(thisVisObjVector);
      }

    } else if ( tagName == "vis_controls" ) {
      float vertical_scalefactor   = DomUtils::stringFromDom(child, "vertical_scalefactor", "").toFloat();
      float windvector_scalefactor = DomUtils::stringFromDom(child, "windvector_scalefactor", "").toFloat();
      float windvector_sparseness  = DomUtils::stringFromDom(child, "windvector_sparseness", "").toFloat();
      int windvector_tipsize       = DomUtils::stringFromDom(child, "windvector_tipsize", "").toInt();
      bool windvector_autosparse   = DomUtils::stringFromDom(child, "windvector_autosparse", "").toInt();
      int vertical_ruler_show      = DomUtils::stringFromDom(child, "vertical_ruler_show", "").toInt();
      double vertical_ruler_lat    = DomUtils::stringFromDom(child, "vertical_ruler_lat", "").toDouble();
      double vertical_ruler_lon    = DomUtils::stringFromDom(child, "vertical_ruler_lon", "").toDouble();
      ui->calipsoScaleFactor->setValue(vertical_scalefactor);
      ui->windVectorScaleFactor->setValue(windvector_scalefactor);
      ui->windVectorTipSize->setValue(windvector_tipsize);
      ui->windVectorSparseness->setValue(windvector_sparseness);
      ui->autoSparseCheckBox->setChecked(windvector_autosparse);
      vOptions->setWindVectorTipSize(windvector_tipsize);
      vOptions->setWindVectorAutosparse(windvector_autosparse);
      vOptions->setWindVectorSparseness(windvector_sparseness);
      vOptions->setShowRuler((bool)vertical_ruler_show);
      vOptions->setRulerLat(vertical_ruler_lat);
      vOptions->setRulerLon(vertical_ruler_lon);
      ui->rulerCheckBox->setChecked(vOptions->getShowRuler());
      ui->rulerLonSpinBox->setValue(vOptions->getRulerLon());
      ui->rulerLatSpinBox->setValue(vOptions->getRulerLat());
    } else if ( tagName == "time_controls" ) {
      float animation_speed = DomUtils::stringFromDom(child, "animation_speed", "").toFloat();
      ui->animationSpeed->setValue(animation_speed);
      int sliderIndex = DomUtils::stringFromDom(child, "sliderIndex", "").toInt();
      update_timestepSlider(sliderIndex);
      int timebaseIndex =  DomUtils::stringFromDom(child, "timebaseIndex", "").toInt();
      gState->setTimebaseType(timebaseIndex);
      ui->timebaseComboBox->setCurrentIndex(timebaseIndex);
      RSIGState *gState = RSIGStateSingleton::getInstance();
      gState->mCounter = sliderIndex;
      generateDefaultPngFilename();
      ui->filenameLineEdit->setText(vOptions->getFilenamePngDefault());
      ui->filenameLineEdit->show();
      ui->filename_generated->hide();

    } else if ( tagName == "truecolor" ) {
      DomUtils::stringFromDom(child, "draw_truecolor", "").toInt();
      QString curTrueColor = DomUtils::stringFromDom(child, "truecolor_type", "");
      vOptions->setTrueColorType(curTrueColor);

      QString truecolor_uniqueID = DomUtils::stringFromDom(child, "truecolor_uniqueID", "");
      vOptions->setUniqueId(truecolor_uniqueID);

      int truecolor_ndays = DomUtils::stringFromDom(child, "truecolor_ndays", "").toInt();

      //vOptions->setDrawModisTrueColor((bool)flag);
      for (int ind=0; ind<ui->trueColorComboBox->maxCount(); ind++) {

        if (ui->trueColorComboBox->itemText(ind) == curTrueColor) {
          ui->trueColorComboBox->setCurrentIndex(ind);
          break;
        }
      }

      if ( curTrueColor != "None" ) {
        vOptions->setDrawModisTrueColor(true);
        enableModisTrueColorOptions(false);
        ui->vizVarCombobox->addItem("True Color imagery");
        ui->vizVarCombobox->setCurrentIndex( ui->vizVarCombobox->count()-1);
        ui->vizCheckBox->setChecked(true);

        for (int daynum=0; daynum<truecolor_ndays; daynum++) {
          const QDate xdate = ui->dateDateEdit->date().addDays(daynum);
          QString yyyy = QString("%1").arg(xdate.year(),  4, 10, QChar('0'));
          QString mm   = QString("%1").arg(xdate.month(), 2, 10, QChar('0'));
          QString dd   = QString("%1").arg(xdate.day(),   2, 10, QChar('0'));
          QString yyyymmdd = yyyy + mm + dd;
          QString filename_4326 = QDir::homePath() + "/.rsig/trueColor_" + curTrueColor + "_" + yyyymmdd +"_4326.png";
          QString filename_3857 = QDir::homePath() + "/.rsig/trueColor_" + curTrueColor + "_" + yyyymmdd +"_3857.png";
          QFile::rename(filename_4326+"_"+truecolor_uniqueID, filename_4326);
          QFile::rename(filename_3857+"_"+truecolor_uniqueID, filename_3857);
        }
      }
    } else if ( tagName == "regrid_parameters" ) {
      RegridParmsData* regridData = RegridParmsDialog::getRegridParmsData();
      regridData->cmaqGrid           = DomUtils::stringFromDom(child, "cmaqGrid", "").toInt();
      regridData->regridMethod       = DomUtils::stringFromDom(child, "regridMethod", "").toInt();
      regridData->regridAggregation  = DomUtils::stringFromDom(child, "aggregation", "").toInt();
      regridData->VGTYP              = DomUtils::stringFromDom(child, "VGTYP", "").toInt();
      regridData->GDTYP              = DomUtils::stringFromDom(child, "GDTYP", "").toInt();
      regridData->NCOLS              = DomUtils::stringFromDom(child, "NCOLS", "").toInt();
      regridData->NROWS              = DomUtils::stringFromDom(child, "NROWS", "").toInt();
      regridData->NLAYS              = DomUtils::stringFromDom(child, "NLAYS", "").toInt();
      regridData->XORIG              = DomUtils::stringFromDom(child, "XORIG", "").toDouble();
      regridData->YORIG              = DomUtils::stringFromDom(child, "YORIG", "").toDouble();
      regridData->XCELL              = DomUtils::stringFromDom(child, "XCELL", "").toDouble();
      regridData->YCELL              = DomUtils::stringFromDom(child, "YCELL", "").toDouble();
      regridData->VGTOP              = DomUtils::stringFromDom(child, "VGTOP", "").toDouble();
      regridData->PALP               = DomUtils::stringFromDom(child, "PALP", "").toDouble();
      regridData->PBET               = DomUtils::stringFromDom(child, "PBET", "").toDouble();
      regridData->PGAM               = DomUtils::stringFromDom(child, "PGAM", "").toDouble();
      regridData->XCENT              = DomUtils::stringFromDom(child, "XCENT", "").toDouble();
      regridData->YCENT              = DomUtils::stringFromDom(child, "YCENT", "").toDouble();
      regridData->RADIUS             = DomUtils::stringFromDom(child, "RADIUS", "").toDouble();
      regridData->G                  = DomUtils::stringFromDom(child, "G", "").toDouble();
      regridData->R                  = DomUtils::stringFromDom(child, "R", "").toDouble();
      regridData->A                  = DomUtils::stringFromDom(child, "A", "").toDouble();
      regridData->T0                 = DomUtils::stringFromDom(child, "T0", "").toDouble();
      regridData->P0                 = DomUtils::stringFromDom(child, "P0", "").toDouble();
      regridData->VGLVLS             = DomUtils::stringFromDom(child, "VGLVLS", "").toStdString();

    } else if ( tagName == "filter_parameters" ) {
      FilterSettingData * filterData = FilterSettingDialog::getFilterSetting();
      filterData->CADmin             = DomUtils::stringFromDom(child, "CADmin", "").toInt();
      filterData->UncertaintyMax     = DomUtils::stringFromDom(child, "UncertaintyMax", "").toDouble();
      filterData->AODmin             = DomUtils::stringFromDom(child, "AODmin", "").toDouble();
      filterData->AODmax             = DomUtils::stringFromDom(child, "AODmax", "").toDouble();
      filterData->AODSTDmin          = DomUtils::stringFromDom(child, "AODSTDmin", "").toDouble();
      filterData->AODSTDmax          = DomUtils::stringFromDom(child, "AODSTDmax", "").toDouble();
      filterData->CLSmin             = DomUtils::stringFromDom(child, "CLSmin", "").toDouble();
      filterData->CLSmax             = DomUtils::stringFromDom(child, "CLSmax", "").toDouble();
      filterData->CH1min             = DomUtils::stringFromDom(child, "CH1min", "").toDouble();
      filterData->CH1max             = DomUtils::stringFromDom(child, "CH1max", "").toDouble();
      filterData->SIGmin             = DomUtils::stringFromDom(child, "SIGmin", "").toDouble();
      filterData->SIGmax             = DomUtils::stringFromDom(child, "SIGmax", "").toDouble();
      filterData->SFCmin             = DomUtils::stringFromDom(child, "SFCmin", "").toDouble();
      filterData->SFCmax             = DomUtils::stringFromDom(child, "SFCmax", "").toDouble();
      filterData->MOSmin             = DomUtils::stringFromDom(child, "MOSmin", "").toDouble();
      filterData->MOSmax             = DomUtils::stringFromDom(child, "MOSmax", "").toDouble();
      filterData->SCAmin             = DomUtils::stringFromDom(child, "SCAmin", "").toDouble();
      filterData->SCAmax             = DomUtils::stringFromDom(child, "SCAmax", "").toDouble();
      filterData->TropomiQuality     = DomUtils::stringFromDom(child, "TropomiQuality", "").toInt();
      filterData->CALIPSOL1DataVersion = DomUtils::stringFromDom(child, "CALIPSOL1DataVersion", "");
      filterData->CALIPSOL2DataVersion = DomUtils::stringFromDom(child, "CALIPSOL2DataVersion", "");

    } else if ( tagName == "swath_persist" ) {
      bool persistBEHR      = DomUtils::stringFromDom(child, "persistBEHR", "").toInt();
      int  persistBEHRHours = DomUtils::stringFromDom(child, "persistBEHRHours", "").toInt();
      vOptions->setPersistBEHR(persistBEHR);
      vOptions->setPersistBEHRHours(persistBEHRHours);
      ui->persistBEHR->setChecked(persistBEHR);
      ui->persistBEHRHours->setValue(persistBEHRHours);

      bool persistMODIS      = DomUtils::stringFromDom(child, "persistMODIS", "").toInt();
      int  persistMODISHours = DomUtils::stringFromDom(child, "persistMODISHours", "").toInt();
      vOptions->setPersistMODIS(persistMODIS);
      vOptions->setPersistMODISHours(persistMODISHours);
      ui->persistMODIS->setChecked(persistMODIS);
      ui->persistMODISHours->setValue(persistMODISHours);
      
      bool persistVIIRS      = DomUtils::stringFromDom(child, "persistVIIRS", "").toInt();
      int  persistVIIRSHours = DomUtils::stringFromDom(child, "persistVIIRSHours", "").toInt();
      vOptions->setPersistVIIRS(persistVIIRS);
      vOptions->setPersistVIIRSHours(persistVIIRSHours);
      ui->persistVIIRS->setChecked(persistVIIRS);
      ui->persistVIIRSHours->setValue(persistVIIRSHours);
      
      bool persistTROPOMI      = DomUtils::stringFromDom(child, "persistTROPOMI", "").toInt();
      int  persistTROPOMIHours = DomUtils::stringFromDom(child, "persistTROPOMIHours", "").toInt();
      vOptions->setPersistTROPOMI(persistTROPOMI);
      vOptions->setPersistTROPOMIHours(persistTROPOMIHours);
      ui->persistTROPOMI->setChecked(persistTROPOMI);
      ui->persistTROPOMIHours->setValue(persistTROPOMIHours);
      
      bool persistTEMPO      = DomUtils::stringFromDom(child, "persistTEMPO", "").toInt();
      int  persistTEMPOHours = DomUtils::stringFromDom(child, "persistTEMPOHours", "").toInt();
      vOptions->setPersistTEMPO(persistTEMPO);
      vOptions->setPersistTEMPOHours(persistTEMPOHours);
      ui->persistTEMPO->setChecked(persistTEMPO);
      ui->persistTEMPOHours->setValue(persistTEMPOHours);
      
      bool persistHMS      = DomUtils::stringFromDom(child, "persistHMS", "").toInt();
      int  persistHMSHours = DomUtils::stringFromDom(child, "persistHMSHours", "").toInt();
      vOptions->setPersistHMS(persistHMS);
      vOptions->setPersistHMSHours(persistHMSHours);
      ui->persistHMS->setChecked(persistHMS);
      ui->persistHMSHours->setValue(persistHMSHours);
    }
  }


  const QString strIsFloating = element.attribute("isFloating", "0");
  const bool isfloating = strIsFloating.toInt();
  this->setFloating(isfloating);
  const QString strIsVisible = element.attribute("isVisible", "1");
  const bool isvisible = strIsVisible.toInt();
  this->setVisible(isvisible);
  const QString strActiveTab = element.attribute("activeTab", "0");
  const int activetab = strActiveTab.toInt();
  ui->tabWidget->setCurrentIndex(activetab);

  if (activetab == 4) { // save tab
    generateDefaultPngFilename();
    ui->filenameLineEdit->setText(vOptions->getFilenamePngDefault());
  }

  const QString strShowLonlatLines = element.attribute("showLonlatLines", "1");
  const int showLonlatLines = strShowLonlatLines.toInt();
  ui->showLonlatCheckBox->setChecked(showLonlatLines);
  gState->mShowLonlatLines = showLonlatLines;

  const QString strShowScale = element.attribute("showScale", "1");
  const int showScale = strShowScale.toInt();
  ui->showScaleCheckBox->setChecked(showScale);
  gState->mShowScale = showScale;

  QString maplineFile( element.attribute( "maplineFile", "" ) );
  vOptions->setMapOverlayFile( maplineFile );

  if ( maplineFile.contains( "none" ) ) {
    ui->maplineNone->setChecked(true);
  } else if ( maplineFile.contains( "/image/map_world.bin" ) ) {
    ui->maplineNations->setChecked(true);
  } else if ( maplineFile.contains( "/image/map_states.bin" ) ) {
    ui->maplineStates->setChecked(true);
  } else if ( maplineFile.contains("/image/states_hires.bin" ) ) {
    ui->maplineStates_hires->setChecked(true);
  } else if ( maplineFile.contains( "/image/map_counties.bin" ) ) {
    ui->maplineCounties->setChecked(true);
  }

  const int maplineColorIndex = element.attribute("maplineColorIndex", "0").toInt();
  vOptions->setMaplineColorIndex( maplineColorIndex );
  onMaplineColorIndexChanged(maplineColorIndex);
  ui->maplineColorComboBox->setCurrentIndex(maplineColorIndex);

  const QString strShowLUTs = element.attribute("showLUTs", "1");
  const int showLUTs = strShowLUTs.toInt();
  gState->mShowLUTs = showLUTs;

  const QString strShowNavigator = element.attribute("showNavigator", "1");
  const int showNavigator = strShowNavigator.toInt();
  ui->showNavigatorCheckBox->setChecked(showNavigator);
  gState->mShowNavigator = showNavigator;

  const QString strShowDataProbe = element.attribute("showDataProbe", "1");
  const int showDataProbe = strShowDataProbe.toInt();
  ui->showDataProbe->setChecked(showDataProbe);
  gState->mShowDataProbe = showDataProbe;



  const QString strShowWidgetTooltips = element.attribute("showWidgetTooltips", "1");
  const int showWidgetTooltips = strShowWidgetTooltips.toInt();
  ui->chk_tooltips->setChecked(showWidgetTooltips);

  const QString strUseDataPreferences = element.attribute("useDataPreferences", "1");
  const int useDataPreferences = strUseDataPreferences.toInt();
  ui->chkUseDataPrefs->setChecked(useDataPreferences);

  const QString strGuiFontSize = element.attribute("guiFontSize", "8");
  const int guiFontSize = strGuiFontSize.toInt();
  ui->guiFontsizeSpinbox->setValue(guiFontSize);



  
  const QString savefile = element.attribute("saveFile", QDir::homePath());
  ui->dirnameLineEdit->setText(savefile);

  const QString strCheckUpdate = element.attribute("checkUpdate", "CHECK_QUIT");

  if (strCheckUpdate == "CHECK_NEVER") {
    gState->mCheckUpdate = RSIGState::CHECK_NEVER;
  } else if (strCheckUpdate == "CHECK_START") {
    gState->mCheckUpdate = RSIGState::CHECK_START;
  } else if (strCheckUpdate == "CHECK_QUIT") {
    gState->mCheckUpdate = RSIGState::CHECK_QUIT;
  }


  checkAndLoadCeilometerMixingLayerHeightDataset();

  Q_EMIT requestRedraw();
}


