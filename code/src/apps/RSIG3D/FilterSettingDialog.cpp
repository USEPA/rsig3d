/******************************************************************************
PURPOSE: FilterSettingData.cpp - Implements the filter setting dialog.

NOTES:

HISTORY: 2015-12-29 freeman.matt@epa.gov
******************************************************************************/
#include <iostream>
#include <QDebug>
#include <QSslError>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "FilterSettingData.h"
#include "FilterSettingDialog.h"
#include "ui_filtersetting.h"
#include <QString>
#include <QTextLine>
#include <QTextStream>
#include <QFile>
#include <QDir>
#include <DatasetManager.h>
#include "RSIGGlobal.h"
#include "MainWin.h"
#include "ControlDockWidget.h"

static FilterSettingData staticData;

struct SpinBoxWithValue {
  QDoubleSpinBox *widget;
  double *value;
};

// for validating purpleair API key. All characters must be able to survive
// rsigserver untainting. 
static QRegExp PAvalidator(FilterSettingData::validatorRegEx);
static QRegExp TEMPOvalidator(FilterSettingData::validatorRegExCaseInsen);
static QRegExp OAQPSvalidator(FilterSettingData::validatorRegExCaseInsen);

// for validating CALIPSO data version. All characters must be able to survive
// rsigserver untainting.

static void setUiValues(Ui_FilterSettingDialog *ui, const FilterSettingData& data) {
    FilterSettingData& g = const_cast<FilterSettingData&>(data);
  struct SpinBoxWithValue array[]={
    {ui->aodDSpinBox, &g.AODmin}
    ,{ui->aodDSpinBox2, &g.AODmax}
    ,{ui->aodstdDSpinBox, &g.AODSTDmin}
    ,{ui->aodstdDSpinBox2, &g.AODSTDmax}
    ,{ui->clsDSpinBox, &g.CLSmin}
    ,{ui->clsDSpinBox2, &g.CLSmax}
    ,{ui->ch1DSpinBox, &g.CH1min}
    ,{ui->ch1DSpinBox2, &g.CH1max}
    ,{ui->sigDSpinBox, &g.SIGmin}
    ,{ui->sigDSpinBox2, &g.SIGmax}
    ,{ui->sfcDSpinBox, &g.SFCmin}
    ,{ui->sfcDSpinBox2, &g.SFCmax}
    ,{ui->mosDSpinBox, &g.MOSmin}
    ,{ui->mosDSpinBox2, &g.MOSmax}
    ,{ui->scaDSpinBox, &g.SCAmin}
    ,{ui->scaDSpinBox2, &g.SCAmax}
    ,{ui->maxuncetaintyDSpinBox, &g.UncertaintyMax}
  };

  for (unsigned int i=0; i<sizeof(array)/sizeof(array[0]); i++) {
    array[i].widget->setValue(*(array[i].value));
  }

  ui->mincadSpinBox->setValue(g.CADmin);

  ui->minTropomiQualitySpinBox->setValue(g.TropomiQuality);
  ui->maxTropomiCloudFractionSpinBox->setValue(g.TropomiMaxCloudFraction);
  ui->TropomiGroundPixelMinimumSpinBox->setValue(g.TropomiGroundPixelMinimum);
  ui->TropomiGroundPixelMaximumSpinBox->setValue(g.TropomiGroundPixelMaximum);
  ui->TropomiAllowNegativeCounts->setChecked(g.TropomiAllowNegativeCounts);

  if (g.viirsNoaaQAFlag == "high") {
    ui->viirsNoaaQAflagMenu->setCurrentIndex(0);
  } else if (g.viirsNoaaQAFlag == "medium") {
    ui->viirsNoaaQAflagMenu->setCurrentIndex(1);
  }

  if (g.pandoraQAFlag == "high") {
    ui->pandoraQAflagMenu->setCurrentIndex(0);
  } else if (g.pandoraQAFlag == "medium") {
    ui->pandoraQAflagMenu->setCurrentIndex(1);
  } else if (g.pandoraQAFlag == "low") {
    ui->pandoraQAflagMenu->setCurrentIndex(2);
  }

  if (g.TEMPOQAFlag == "normal") {
    ui->TEMPOQAflagMenu->setCurrentIndex(0);
  } else if (g.TEMPOQAFlag == "suspect") {
    ui->TEMPOQAflagMenu->setCurrentIndex(1);
  } else if (g.TEMPOQAFlag == "bad") {
    ui->TEMPOQAflagMenu->setCurrentIndex(2);
  }

  ui->maxTEMPOCloudFractionSpinBox->setValue(g.TEMPOMaxCloudFraction);
  ui->maxTEMPOSolarZenithAngleSpinBox->setValue(g.TEMPOMaxSolarZenithAngle);
  ui->TEMPOAllowNegativeCounts->setChecked(g.TEMPOAllowNegativeCounts);
  ui->TEMPOKey->setText( g.TEMPOKey );

  ui->maxOMICloudFractionSpinBox->setValue(g.OMIMaxCloudFraction);
  ui->maxOMISolarZenithAngleSpinBox->setValue(g.OMIMaxSolarZenithAngle);
  ui->OMIAllowNegativeCounts->setChecked(g.OMIAllowNegativeCounts);

  ui->OAQPSKey->setText( g.OAQPSKey );

  ui->acceptablePurpleairChannelDifference->setValue(g.PurpleAirAcceptableChannelDifference);
  ui->acceptablePurpleairChannelAgreement->setValue(g.PurpleAirAcceptableChannelAgreement);
  ui->minPurpleairCountPercentage->setValue(g.PurpleAirMinAggregationCountPercentage);
  ui->usePurpleairHumidityReplacement->setChecked(g.usePurpleAirHumidityReplacement);
  ui->purpleairHumidityReplacementValue->setValue(g.PurpleAirReplacementHumidity);
  ui->apikeyPurpleAir->setText( g.PurpleAirAPIKey );

  if (g.PurpleairLocationFlag == 0) {
    ui->purpleairLocationOutdoor->setChecked(true);
  } else if (g.PurpleairLocationFlag == 1) {
    ui->purpleairLocationIndoor->setChecked(true);
  } else {
    ui->purpleairLocationIndoorOutdoor->setChecked(true);
  }

  int indexL1 = ui->calipsoVersionL1Combobox->findText(g.CALIPSOL1DataVersion);
  int indexL2 = ui->calipsoVersionL2Combobox->findText(g.CALIPSOL2DataVersion);
  ui->calipsoVersionL1Combobox->setCurrentIndex( indexL1 >= 0 ? indexL1 : 0 );
  ui->calipsoVersionL2Combobox->setCurrentIndex( indexL2 >= 0 ? indexL2 : 0 );
}



static void getUiValues(const Ui_FilterSettingDialog *ui, FilterSettingData& g) {
  struct SpinBoxWithValue array[] = {
                                   {ui->aodDSpinBox, &g.AODmin}
                                   ,{ui->aodDSpinBox2, &g.AODmax}
                                   ,{ui->aodstdDSpinBox, &g.AODSTDmin}
                                   ,{ui->aodstdDSpinBox2, &g.AODSTDmax}
                                   ,{ui->clsDSpinBox, &g.CLSmin}
                                   ,{ui->clsDSpinBox2, &g.CLSmax}
                                   ,{ui->ch1DSpinBox, &g.CH1min}
                                   ,{ui->ch1DSpinBox2, &g.CH1max}
                                   ,{ui->sigDSpinBox, &g.SIGmin}
                                   ,{ui->sigDSpinBox2, &g.SIGmax}
                                   ,{ui->sfcDSpinBox, &g.SFCmin}
                                   ,{ui->sfcDSpinBox2, &g.SFCmax}
                                   ,{ui->mosDSpinBox, &g.MOSmin}
                                   ,{ui->mosDSpinBox2, &g.MOSmax}
                                   ,{ui->scaDSpinBox, &g.SCAmin}
                                   ,{ui->scaDSpinBox2, &g.SCAmax}
                                   ,{ui->maxuncetaintyDSpinBox, &g.UncertaintyMax}
  };
  
  for (unsigned int i=0; i<sizeof(array)/sizeof(array[0]); i++)
    *(array[i].value) = array[i].widget->value();
  
  g.CADmin = ui->mincadSpinBox->value();

  g.TropomiQuality = ui->minTropomiQualitySpinBox->value();
  g.TropomiMaxCloudFraction = ui->maxTropomiCloudFractionSpinBox->value();
  g.TropomiGroundPixelMinimum = ui->TropomiGroundPixelMinimumSpinBox->value();
  g.TropomiGroundPixelMaximum = ui->TropomiGroundPixelMaximumSpinBox->value();
  g.TropomiAllowNegativeCounts = ui->TropomiAllowNegativeCounts->isChecked();

  g.viirsNoaaQAFlag = ui->viirsNoaaQAflagMenu->currentText();
  g.pandoraQAFlag = ui->pandoraQAflagMenu->currentText();

  g.TEMPOQAFlag = ui->TEMPOQAflagMenu->currentText();
  g.TEMPOMaxCloudFraction = ui->maxTEMPOCloudFractionSpinBox->value();
  g.TEMPOMaxSolarZenithAngle = ui->maxTEMPOSolarZenithAngleSpinBox->value();
  g.TEMPOAllowNegativeCounts = ui->TEMPOAllowNegativeCounts->isChecked();
  g.TEMPOKey = ui->TEMPOKey->text();

  g.OMIMaxCloudFraction = ui->maxOMICloudFractionSpinBox->value();
  g.OMIMaxSolarZenithAngle = ui->maxOMISolarZenithAngleSpinBox->value();
  g.OMIAllowNegativeCounts = ui->OMIAllowNegativeCounts->isChecked();

  g.OAQPSKey = ui->OAQPSKey->text();

  g.PurpleAirAcceptableChannelDifference =  ui->acceptablePurpleairChannelDifference->value();
  g.PurpleAirAcceptableChannelAgreement  =  ui->acceptablePurpleairChannelAgreement->value();

  g.PurpleAirMinAggregationCountPercentage = ui->minPurpleairCountPercentage->value();
  g.usePurpleAirHumidityReplacement = ui->usePurpleairHumidityReplacement->isChecked();
  g.PurpleAirReplacementHumidity = ui->purpleairHumidityReplacementValue->value();

  if (ui->purpleairLocationOutdoor->isChecked()) {
    g.PurpleairLocationFlag = 0;
  } else if (ui->purpleairLocationIndoor->isChecked()) {
    g.PurpleairLocationFlag = 1;
  } else {
    g.PurpleairLocationFlag = 2;
  }

  QString currentPurpleairKey = ui->apikeyPurpleAir->text();

  g.PurpleAirAPIKey = currentPurpleairKey;

  g.CALIPSOL1DataVersion = ui->calipsoVersionL1Combobox->currentText();
  g.CALIPSOL2DataVersion = ui->calipsoVersionL2Combobox->currentText();
}



FilterSettingData* FilterSettingDialog::getFilterSetting() {
  return &staticData;
}


//======================================================================

/*!
 \class FilterSettingDialog
 
 \brief A dialog widget class for filter settting.
 */

FilterSettingDialog::FilterSettingDialog(QWidget *parent)
  : QDialog(parent), ui(0) {

  ui = new Ui_FilterSettingDialog;
  ui->setupUi(this);
  ui->tabWidget->setCurrentIndex(0);

#ifdef __APPLE__
    // On Macs default tabs must be size-adjusted to fit:
  ui->tabWidget->setStyleSheet("QTabBar::tab:selected { color:#0000FF; background-color:#CCCCCC}\nQTabBar::tab { padding-top:5px; padding-bottom:5px; padding-left:10px; padding-right:10px; font-size:10pt; }");
#endif

  setUiValues(ui, staticData);

  ui->TEMPOKeyLabel->setToolTip("Access to pre-V03 TEMPO data is restricted to TEMPO Science Team members only.");

  ui->OAQPSKeyLabel->setToolTip("Access to CMAQ-OAQPS data is restricted to OAQPS Science Team members only.");

  ui->apikeyPurpleAirLabel->setToolTip("API key needed to get PurpleAir data.\nTo get a key, please contact the RSIG team at rsig@epa.gov");
  ui->apikeyPurpleAir->setToolTip("API key needed to get PurpleAir data.\nTo get a key, please contact the RSIG team at rsig@epa.gov");
  ui->apikeyPurpleAir->setValidator(new QRegExpValidator(PAvalidator));

  ui->acceptablePurpleairChannelDifference->setToolTip("The PurpleAir data will be deemed valid if the A an B channels agree to within the specified amount in ug/m3.");
  ui->acceptablePurpleairChannelAgreement->setToolTip("The PurpleAir data will be deemed valid if the A an B channels agree to within the specified percentage.");
  ui->minPurpleairCountPercentage->setToolTip("Minimum percentage of samples needed when aggregating data by time.");
  ui->usePurpleairHumidityReplacement->setToolTip("If checked, invalid humidity levels will be replaced with the specified value in EPA's PM2.5 correction equation.");
  ui->purpleairHumidityReplacementValue->setToolTip("Replacement humidity level to use in EPA's PM2.5 correction equation in case the measured humidity value is invalid.");

  ui->purpleairLocationOutdoor->setToolTip("Only retrieve data tagged as being outdoors.");
  ui->purpleairLocationIndoor->setToolTip("Only retrieve data tagged as being indoors.");
  ui->purpleairLocationIndoorOutdoor->setToolTip("Retrieve both indoor and outdoor data.");

  ui->calipsoVersionL1Combobox->setToolTip("Desired version of all L1 data products. If 'Latest' is selected, the most recent version will be used.");
  ui->calipsoVersionL2Combobox->setToolTip("Desired version of all L2 data products. If 'Latest' is selected, the most recent version will be used.");

  ui->minTropomiQualitySpinBox->setToolTip("Higher values indicate better data quality, but may also reduce or entirely eliminate the amount of data retrieved. Default value is 75.");
  ui->maxTropomiCloudFractionSpinBox->setToolTip("Lower values will filter out the estimated influence of clouds. Default value is 1.0 (no filtering)");
  ui->TropomiGroundPixelMinimumSpinBox->setToolTip("The ground pixel minimum and maximum are used to limit the extent of the swath. If the maximum ground pixel set to zero the filter will not be applied.\n\n  - OFFL NO2 and HCHO range: 0-449\n  - OFFL CH4 and CO range: 0-214");
  ui->TropomiGroundPixelMaximumSpinBox->setToolTip("The ground pixel minimum and maximum are used to limit the extent of the swath. If the maximum ground pixel set to zero the filter will not be applied.\n\n  - OFFL NO2 and HCHO range: 0-449\n  - OFFL CH4 and CO range: 0-214");
  ui->TropomiAllowNegativeCounts->setToolTip("Allow retrieval of negative molecules/cm2?");

  ui->TEMPOAllowNegativeCounts->setToolTip("Allow retrieval of negative molecules/cm2?");
  ui->TEMPOKey->setValidator(new QRegExpValidator(TEMPOvalidator));

  ui->OMIAllowNegativeCounts->setToolTip("Allow retrieval of negative molecules/cm2?");

  ui->OAQPSKey->setValidator(new QRegExpValidator(OAQPSvalidator));

  connect(ui->acceptPushButton, SIGNAL(clicked()), this, SLOT(onOK()));
  connect(ui->cancelPushButton, SIGNAL(clicked()), this, SLOT(onCancel()));
  connect(ui->resetPushButton,  SIGNAL(clicked()), this, SLOT(onReset()));
  connect(ui->apikeyPurpleAir, SIGNAL(editingFinished()), this, SLOT(validatePurpleairKey()));
  connect(ui->TEMPOKey, SIGNAL(editingFinished()), this, SLOT(validateTEMPOKey()));
  connect(ui->OAQPSKey, SIGNAL(editingFinished()), this, SLOT(validateOAQPSKey()));
}



FilterSettingDialog::~FilterSettingDialog() {
  delete ui;
}



void FilterSettingDialog::onOK() {
  getUiValues(ui, staticData);
  bool isTEMPOKeyValid = this->validateTEMPOKey();

  if (isTEMPOKeyValid) {
    this->writeTEMPOKeyFile();
  } else {
    FilterSettingData& g = const_cast<FilterSettingData&>(staticData);
    g.TEMPOKey = "";
  }

  bool isOAQPSKeyValid = this->validateOAQPSKey();

  if (isOAQPSKeyValid) {
    this->writeOAQPSKeyFile();
  } else {
    FilterSettingData& g = const_cast<FilterSettingData&>(staticData);
    g.OAQPSKey = "";
  }

  bool isPurpleairKeyValid = this->validatePurpleairKey();

  if (isPurpleairKeyValid) {
    this->writePurpleairApiKeyFile();
  } else {
    FilterSettingData& g = const_cast<FilterSettingData&>(staticData);
    g.PurpleAirAPIKey = "";
  }

  this->close();
}



void FilterSettingDialog::onCancel() {
  this->close();
}



void FilterSettingDialog::onReset() {
  setUiValues(ui, FilterSettingData());
  update();
}



void FilterSettingDialog::writeTEMPOKeyFile() {
  QString filename = QDir::homePath() + "/.rsig/tempokey.txt";
  QFile file(filename);

  if (file.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text)) {
    QTextStream stream(&file);
    stream << ui->TEMPOKey->text() << endl;
  }
}



void FilterSettingDialog::writeOAQPSKeyFile() {
  QString filename = QDir::homePath() + "/.rsig/oaqpskey.txt";
  QFile file(filename);

  if (file.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text)) {
    QTextStream stream(&file);
    stream << ui->OAQPSKey->text() << endl;
  }
}



void FilterSettingDialog::writePurpleairApiKeyFile() {
  QString filename_apikey = QDir::homePath() + "/.rsig/pakey.txt";
  QFile file_apikey(filename_apikey);

  if (file_apikey.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text)) {
    QTextStream stream(&file_apikey);
    stream << ui->apikeyPurpleAir->text() << endl;
  }
}



bool FilterSettingDialog::validatePurpleairKey() {
  const QString text( ui->apikeyPurpleAir->text() );
  int keyLength = ui->apikeyPurpleAir->text().length();
  
  if (keyLength != 0) {
    const std::string copy( text.toStdString() );
    const char* const key = copy.c_str();
    const bool isValid = emvl::DatasetManager::isValidPurpleAirKey( key );
    
    if (!isValid) {
      MainWin& mainWin = MainWin::getInstance();
      ControlDockWidget* const controlDockWidget = mainWin.controlWidget();
      controlDockWidget->postMessage( "PurpleAir READ key could not be validated.",
                                      RSIGState::WARNING_TEXT );
    }

    return isValid;
  } else {
    return true; // Accept a zero length key here so the user does not get into "invalid key hell".
                 // Another check will also be done before data is requested (HttpDownloadDialog) that
                 // will alert the user of a zero length key.
  }
}



bool FilterSettingDialog::validateTEMPOKey() {
  const QString text( ui->TEMPOKey->text() );
  int keyLength = ui->TEMPOKey->text().length();

  if (keyLength != 0) {
    const std::string copy( text.toStdString() );
    const char* const key = copy.c_str();
    const bool isValid = emvl::DatasetManager::isValidTEMPOKey( key );

    if (!isValid) {
      MainWin& mainWin = MainWin::getInstance();
      ControlDockWidget* const controlDockWidget = mainWin.controlWidget();
      controlDockWidget->postMessage( "TEMPO READ key could not be validated.",
                                      RSIGState::WARNING_TEXT );
    }

    return isValid;
  } else {
    return true; // Accept a zero length key here so the user does not get into "invalid key hell".
                 // Another check will also be done before data is requested (HttpDownloadDialog) that
                 // will alert the user of a zero length key.
  }
}


bool FilterSettingDialog::validateOAQPSKey() {
  const QString text( ui->OAQPSKey->text() );
  int keyLength = ui->OAQPSKey->text().length();

  if (keyLength != 0) {
    const std::string copy( text.toStdString() );
    const char* const key = copy.c_str();
    const bool isValid = emvl::DatasetManager::isValidOAQPSKey( key );

    if (!isValid) {
      MainWin& mainWin = MainWin::getInstance();
      ControlDockWidget* const controlDockWidget = mainWin.controlWidget();
      controlDockWidget->postMessage( "OAQPS READ key could not be validated.",
                                      RSIGState::WARNING_TEXT );
    }

    return isValid;
  } else {
    return true; // Accept a zero length key here so the user does not get into "invalid key hell".
                 // Another check will also be done before data is requested (HttpDownloadDialog) that
                 // will alert the user of a zero length key.
  }
}



