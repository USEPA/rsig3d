
#include <QObject>
#include <QDebug>
#include <QDir>

#include "RSIGGlobal.h"
#include "VisOptions.h"



VisOptions::VisOptions():QObject() {
  //mode2D = true;
  displayMode = 0; // 2D
  viewResetFlag = 0;
  showRuler = true;
  calipsoScaleFactor = 1.0;
  windVectorScaleFactor = 50.0;
  windVectorSparseness = 10.0;
  windVectorAutosparse = true;
  ceilometerMinAltitude = 0.0;
  ceilometerMaxAltitude = 15.0;
  ceilometerPlotScrollerValue = 0;
  ceilometerShowLocations = true;
  ceilometerShowMixingLayer = true;
  ceilometerSize = 200;
  pandoraMinAltitude = 0.0;
  pandoraMaxAltitude = 5.0;
  pandoraPlotScrollerValue = 0;
  pandoraShowLocations = true;
  pandoraSize = 200;
  neubrewMinAltitude = 0.0;
  neubrewMaxAltitude = 50.0;
  neubrewPlotScrollerValue = 0;
  neubrewShowLocations = true;
  neubrewSize = 200;
  flattenCMAQ = false;
  persistBEHR = false;
  persistMODIS = false;
  persistVIIRS = false;
  persistTROPOMI = false;
  persistTEMPO = false;
  persistHMS = false;
  drawModisTrueColor = false;
  opacityModisTrueColor = 100.0;
  trueColorType = "None";
  persistBEHRHours = 24;
  persistMODISHours = 24;
  persistVIIRSHours = 24;
  persistTROPOMIHours = 24;
  persistTEMPOHours = 1;
  persistHMSHours = 24;
  curProjection = RSIGState::PROJECTION_LONGLAT;
  lonlatlabelOrientation = 0;
  rulerLon = 0.0;
  rulerLat = 0.0;
  bboxMinLon = 0; // initial value set in ControlDockWidget_init
  bboxMinLat = 0; // initial value set in ControlDockWidget_init
  bboxMaxLon = 0; // initial value set in ControlDockWidget_init
  bboxMaxLat = 0; // initial value set in ControlDockWidget_init
  currentDayNum = -1;
  filenameScenario = QString("myScenario.xml");
  filenamePng = QString("");
  filenamePngDefault = QString("");
  saveDir = QDir::homePath();
  mapOverlayFile = "none";
  maplineColor = "Black";
  maplineColorIndex = 0;
  windVectorLabel = "";
  windVectorLength = 16;
  windVectorTipSize = 10;
  saveMetadataFlag = false;
  saveFileFormatIndex = -1;
  dataSourceList << "";
  uniqueID = "";
  downloadsOutstanding = 0;
}

// destructor
VisOptions::~VisOptions() {
  //delete mode2D;
}

// getters ////////////////////////////////////////////////////
QStringList VisOptions::getDataSourceList() const {
  return dataSourceList;
}

QString VisOptions::getFilenameScenario() const {
  return filenameScenario;
}

QString VisOptions::getFilenamePng() const {
  return filenamePng;
}

QString VisOptions::getFilenamePngDefault() const {
  return filenamePngDefault;
}

QString VisOptions::getSaveDir() const {
  return saveDir;
}

//bool VisOptions::getMode2D() const {
//  return mode2D;
//}
int VisOptions::getDisplayMode() const {
  return displayMode;
}

bool VisOptions::getCeilometerShowLocations() const {
  return ceilometerShowLocations;
}

bool VisOptions::getCeilometerShowMixingLayer() const {
  return ceilometerShowMixingLayer;
}

bool VisOptions::getPandoraShowLocations() const {
  return pandoraShowLocations;
}

bool VisOptions::getNeubrewShowLocations() const {
  return neubrewShowLocations;
}

bool VisOptions::getDrawModisTrueColor() const {
  return drawModisTrueColor;
}

float VisOptions::getOpacityModisTrueColor() const {
  return opacityModisTrueColor;
}

QString VisOptions::getTrueColorType() const {
  return trueColorType;
}

int VisOptions::getViewResetFlag() const {
  return viewResetFlag;
}

int VisOptions::getCurProjection() const {
  return curProjection;
}

int VisOptions::getLonlatLabelOrientation() const {
  return lonlatlabelOrientation;
}

bool VisOptions::getFlattenCMAQ() const {
  return flattenCMAQ;
}

bool VisOptions::getPersistBEHR() const {
  return persistBEHR;
}

bool VisOptions::getPersistMODIS() const {
  return persistMODIS;
}

bool VisOptions::getPersistVIIRS() const {
  return persistVIIRS;
}

bool VisOptions::getPersistTROPOMI() const {
  return persistTROPOMI;
}

bool VisOptions::getPersistTEMPO() const {
  return persistTEMPO;
}

bool VisOptions::getPersistHMS() const {
  return persistHMS;
}

bool VisOptions::getShowRuler() const {
  return showRuler;
}

int VisOptions::getPersistBEHRHours() const {
  return persistBEHRHours;
}

int VisOptions::getPersistMODISHours() const {
  return persistMODISHours;
}

int VisOptions::getPersistVIIRSHours() const {
  return persistVIIRSHours;
}

int VisOptions::getPersistTROPOMIHours() const {
  return persistTROPOMIHours;
}

int VisOptions::getPersistTEMPOHours() const {
  return persistTEMPOHours;
}

int VisOptions::getPersistHMSHours() const {
  return persistHMSHours;
}

double VisOptions::getCalipsoScaleFactor() const {
  return calipsoScaleFactor;
}

double VisOptions::getWindVectorScaleFactor() const {
  return windVectorScaleFactor;
}

double VisOptions::getWindVectorTipSize() const {
  return windVectorTipSize;
}

double VisOptions::getWindVectorSparseness() const {
  return windVectorSparseness;
}

bool VisOptions::getWindVectorAutosparse() const {
  return windVectorAutosparse;
}

QString VisOptions::getWindVectorLabel() const {
  return windVectorLabel;
}

int VisOptions::getWindVectorLength() const {
  return windVectorLength;
}

double VisOptions::getCeilometerMinAltitude() const {
  return ceilometerMinAltitude;
}

double VisOptions::getCeilometerMaxAltitude() const {
  return ceilometerMaxAltitude;
}

int VisOptions::getCeilometerPlotScrollerValue() const {
  return ceilometerPlotScrollerValue;
}

int VisOptions::getCeilometerSize() const {
  return ceilometerSize;
}

double VisOptions::getPandoraMinAltitude() const {
  return pandoraMinAltitude;
}

double VisOptions::getPandoraMaxAltitude() const {
  return pandoraMaxAltitude;
}

int VisOptions::getPandoraPlotScrollerValue() const {
  return pandoraPlotScrollerValue;
}

int VisOptions::getPandoraSize() const {
  return pandoraSize;
}

double VisOptions::getNeubrewMinAltitude() const {
  return neubrewMinAltitude;
}

double VisOptions::getNeubrewMaxAltitude() const {
  return neubrewMaxAltitude;
}

int VisOptions::getNeubrewPlotScrollerValue() const {
  return neubrewPlotScrollerValue;
}

int VisOptions::getNeubrewSize() const {
  return neubrewSize;
}

double VisOptions::getRulerLon() const {
  return rulerLon;
}

double VisOptions::getRulerLat() const {
  return rulerLat;
}

double VisOptions::getBboxMinLon() const {
  return bboxMinLon;
}

double VisOptions::getBboxMinLat() const {
  return bboxMinLat;
}

double VisOptions::getBboxMaxLon() const {
  return bboxMaxLon;
}

double VisOptions::getBboxMaxLat() const {
  return bboxMaxLat;
}

int VisOptions::getCurrentDayNum() const {
  return currentDayNum;
}

const QString& VisOptions::getMapOverlayFile() const {
  return mapOverlayFile;
}

QString VisOptions::getMaplineColor() const {
  return maplineColor;
}

int VisOptions::getMaplineColorIndex() const {
  return maplineColorIndex;
}

bool VisOptions::getSaveMetadataFlag() const {
  return saveMetadataFlag;
}

int VisOptions::getSaveFileFormatIndex() const {
  return saveFileFormatIndex;
}

QString VisOptions::getUniqueId() const {
  return uniqueID;
}

int VisOptions::getDownloadsOutstanding() const {
  return downloadsOutstanding;
}

// setters ////////////////////////////////////////////////////
void VisOptions::setDataSourceList(QStringList x) {
  dataSourceList = x;
}

void VisOptions::setFilenameScenario(QString x) {
  filenameScenario = x;
}

void VisOptions::setFilenamePng(QString x) {
  filenamePng = x;
}

void VisOptions::setFilenamePngDefault(QString x) {
  filenamePngDefault = x;
}

void VisOptions::setSaveDir(QString x) {
  saveDir = x;
}

//void VisOptions::setMode2D(bool x) {
//  mode2D = x;
//}
void VisOptions::setDisplayMode(int x) {
  displayMode = x;
}

void VisOptions::setCeilometerShowLocations(bool x) {
  ceilometerShowLocations = x;
}

void VisOptions::setCeilometerShowMixingLayer(bool x) {
  ceilometerShowMixingLayer = x;
}

void VisOptions::setPandoraShowLocations(bool x) {
  pandoraShowLocations = x;
}

void VisOptions::setNeubrewShowLocations(bool x) {
  neubrewShowLocations = x;
}

void VisOptions::setDrawModisTrueColor(bool x) {
  drawModisTrueColor = x;
}

void VisOptions::setOpacityModisTrueColor(float x) {
  opacityModisTrueColor = x;
}

void VisOptions::setTrueColorType(QString x) {
  trueColorType = x;
}

void VisOptions::setViewResetFlag(int x) {
  viewResetFlag = x;
}

void VisOptions::setCurProjection(int x) {
  curProjection = x;
}

void VisOptions::setLonlatLabelOrientation(int x) {
  lonlatlabelOrientation = x;
}

void VisOptions::setFlattenCMAQ(bool x) {
  flattenCMAQ = x;
}

void VisOptions::setPersistBEHR(bool x) {
  persistBEHR = x;
}

void VisOptions::setPersistMODIS(bool x) {
  persistMODIS = x;
}

void VisOptions::setPersistVIIRS(bool x) {
  persistVIIRS = x;
}

void VisOptions::setPersistTROPOMI(bool x) {
  persistTROPOMI = x;
}

void VisOptions::setPersistTEMPO(bool x) {
  persistTEMPO = x;
}

void VisOptions::setPersistHMS(bool x) {
  persistHMS = x;
}

void VisOptions::setShowRuler(bool x) {
  showRuler = x;
}

void VisOptions::setPersistBEHRHours(int value) {
  persistBEHRHours = value;
}

void VisOptions::setPersistMODISHours(int value) {
  persistMODISHours = value;
}

void VisOptions::setPersistVIIRSHours(int value) {
  persistVIIRSHours = value;
}

void VisOptions::setPersistTROPOMIHours(int value) {
  persistTROPOMIHours = value;
}

void VisOptions::setPersistTEMPOHours(int value) {
  persistTEMPOHours = value;
}

void VisOptions::setPersistHMSHours(int value) {
  persistHMSHours = value;
}

void VisOptions::setCalipsoScaleFactor(double value) {
  calipsoScaleFactor = value;
}

void VisOptions::setWindVectorScaleFactor(double value) {
  windVectorScaleFactor = value;
}

void VisOptions::setWindVectorTipSize(double value) {
  windVectorTipSize = value;
}

void VisOptions::setWindVectorSparseness(double value) {
  windVectorSparseness = value;
}

void VisOptions::setWindVectorAutosparse(bool x) {
  windVectorAutosparse = x;
}

void VisOptions::setWindVectorLabel(QString value) {
  windVectorLabel = value;
}

void VisOptions::setWindVectorLength(int value) {
  windVectorLength = value;
}

void VisOptions::setCeilometerMinAltitude(double value) {
  ceilometerMinAltitude = value;
}

void VisOptions::setCeilometerMaxAltitude(double value) {
  ceilometerMaxAltitude = value;
}

void VisOptions::setCeilometerPlotScrollerValue(int value) {
  ceilometerPlotScrollerValue = value;
}

void VisOptions::setCeilometerSize(int value) {
  ceilometerSize = value;
}

void VisOptions::setPandoraMinAltitude(double value) {
  pandoraMinAltitude = value;
}

void VisOptions::setPandoraMaxAltitude(double value) {
  pandoraMaxAltitude = value;
}

void VisOptions::setPandoraPlotScrollerValue(int value) {
  pandoraPlotScrollerValue = value;
}

void VisOptions::setPandoraSize(int value) {
  pandoraSize = value;
}

void VisOptions::setNeubrewMinAltitude(double value) {
  neubrewMinAltitude = value;
}

void VisOptions::setNeubrewMaxAltitude(double value) {
  neubrewMaxAltitude = value;
}

void VisOptions::setNeubrewPlotScrollerValue(int value) {
  neubrewPlotScrollerValue = value;
}

void VisOptions::setNeubrewSize(int value) {
  neubrewSize = value;
}

void VisOptions::setRulerLon(double value) {
  rulerLon = value;
}

void VisOptions::setRulerLat(double value) {
  rulerLat = value;
}

void VisOptions::setBboxMinLon(double value) {
  bboxMinLon = value;
}

void VisOptions::setBboxMinLat(double value) {
  bboxMinLat = value;
}

void VisOptions::setBboxMaxLon(double value) {
  bboxMaxLon = value;
}

void VisOptions::setBboxMaxLat(double value) {
  bboxMaxLat = value;
}

void VisOptions::setCurrentDayNum(int value) {
  currentDayNum = value;
}

void VisOptions::setMapOverlayFile(QString x) {
  mapOverlayFile = x;
}

void VisOptions::setMaplineColor(QString x) {
  maplineColor = x;
}

void VisOptions::setMaplineColorIndex(int x) {
  maplineColorIndex = x;
}

void VisOptions::setSaveMetadataFlag(bool x) {
  saveMetadataFlag = x;
}

void VisOptions::setSaveFileFormatIndex(int x) {
  saveFileFormatIndex = x;
}

void VisOptions::setUniqueId(QString x) {
  uniqueID = x;
}

void VisOptions::setDownloadsOutstanding(int x) {
  downloadsOutstanding = x;
}


