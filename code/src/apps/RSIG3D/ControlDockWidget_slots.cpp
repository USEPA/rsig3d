/******************************************************************************
PURPOSE: ControlDockWidget_slots.C - Implements some slot functions of
         ControlDockWidget class.

HISTORY: 2012-11-15, zhang.nan@epa.gov
         Evolved by freeman.matt@epa.gov
STATUS:  unreviewed untested
******************************************************************************/

#include <assert.h> // For macro assert().
#include <unistd.h>

#include <iostream>
#include <vector>    // For std::vector.
#include <string>    // For std::string.
#include <algorithm> // For std:stable_sort(), std::unique().

#include <QApplication>        //qApp, slot closeAllWindows()
#include <QDesktopServices>      //QDesktopServices::openUrl(QUrl);
#include <QString>
#include <QDebug>
#include <QResizeEvent>             //QResizeEvent class
#include <QDir>                     //QDir::homePath()
#include <QFileInfo>
#include <QFileDialog>
#include <QMap>
#include <QMessageBox> 
#include <QVarLengthArray>
#include <QStandardItem>
#include <QTreeWidgetItemIterator>
#include <QFile>
#include <QTextStream>
#include <QDesktopWidget>
#include <QMutex>

//#include <QUrl>
//#include <QNetworkRequest>
//#include <QNetworkReply>
//#include <QNetworkAccessManager>
//#include <QByteArray>
//#include <QEventLoop>



#include <Bounds.h>          // For class emvl::Bounds.
#include <Dataset.h>         // For class emvl::Dataset.
#include <DatasetManager.h>  // For class emvl:DatasetManager.

#include "CeilometerCurtainArray.h" // For cca::*.
#include "RSIGGlobal.h"
#include "GeoDataObject.h"
#include "VisDataObject.h"
#include "GeoDataList.h"
#include "VisDataList.h"
#include "GeoDataPropertyEdit.h"
#include "VisDataPropertyEdit.h"
#include "LookupTableWidget.h"      //LookupTableWidget class
#include "SlantLookupTableWidget.h" //SlantLookupTableWidget class
#include "GetData.h"
#include "HttpDownloadDialog.h"     //HttpGet class, multi-threaded downloading web data
#include "RegridParmsDialog.h"      //RegridParms dialog
#include "FilterSettingDialog.h"    //FilterPamrs dialog
#include "CMAQFileReaderDialog.h"
#include "ControlDockWidget.h"      //self header
#include "ui_controldockwidget.h"   //UI form
#include "FileOrientedDownloader.h"
#include "MainWin.h"
#include "VisOptions.h"
#include "DataUsePolicy.h"
#include "RSIGScene.h"
#include "ModisTrueColor.h"
#include "VisDataObjectProperties.h"
#include "RGBColormap.h"
#include "RSIG3DHelp.h"
#include "DataPreferences.h"

#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif

static int num_selected_sources = 0;
//static VisDataObjectProperties oldVisProperties[5];
static VisDataObjectProperties oldVisProperties[10]; // 5 variables plus 5 possible extra variables (e.g. hourly pandora)
static QString oldSelectedDatasource;

static QMutex mutex(QMutex::Recursive);



struct MapServerInfo{
  const char* mMapSever;         //server name string
  int mUseMarbleWidget;           //server belongs to marble widget?
  int mSupportedProjectionFlag;   //bitmap flag for the 3 projection types
  const char* mTheme;            //two usages:
};



static struct MapServerInfo mapServers[]={
  {"Marble-plain (offline)", 1, 31, "earth/plain/plain.dgml"}
  ,{"Marble-srtm (offline)", 1, 31, "earth/srtm/srtm.dgml"}
  ,{"Marble-bluemarble (offline)", 1, 31, "earth/bluemarble/bluemarble.dgml"}
  ,{"-", 1, 0, ""}   //Separator
  ,{"Google map (online)", 0, 4, "GM"}
  ,{"Google terrain map (online)", 0, 4, "GT"}
  ,{"Google satellite hybrid map (online)", 0, 4, "GH"}
  ,{"Google satellite map (online)", 0, 4, "GS"}
};



void ControlDockWidget::addMapServersHelper() {
  const int serverCount = sizeof(mapServers)/sizeof(mapServers[0]);

  for (int i=0; i<serverCount; i++) {

    if (mapServers[i].mMapSever[0] == '-') {
      ui->mapserverComboBox->insertSeparator(i);
    } else {
      ui->mapserverComboBox->addItem(mapServers[i].mMapSever);
    }
  }
}



/*!
 \fn QRectF ControlDockWidget::regionOfInterest() const

 Return the Region-Of-Interest rectangle in QRectF.
 */
QRectF ControlDockWidget::regionOfInterest() const {
  const double x0 = ui->westDSpinBox->value();
  const double x1 = ui->eastDSpinBox->value();
  const double y0 = ui->southDSpinBox->value();
  const double y1 = ui->northDSpinBox->value();
  const QRectF r(QPointF(x0, y0), QPointF(x1, y1));
  return r;
}



/*!
 \fn void ControlDockWidget::setRegionOfInterest(const QRectF & r)

 Set the Region-Of-Interest rectangle in this class.
 */
void ControlDockWidget::setRegionOfInterest(const QRectF & r) {
  const double x0 = r.left();
  const double y0 = r.bottom();
  const double x1 = r.right();
  const double y1 = r.top();
  const bool f = blockSignals(true);
  ui->westDSpinBox->setValue(x0);
  ui->eastDSpinBox->setValue(x1);
  ui->southDSpinBox->setValue(y0);
  ui->northDSpinBox->setValue(y1);
  set_currentGuiBbox_minLat(y0);
  set_currentGuiBbox_maxLat(y1);
  set_currentGuiBbox_minLon(x0);
  set_currentGuiBbox_maxLon(x1);
  blockSignals(f);
  Q_EMIT boundingGeoBoxChanged(r);
}



/*!
 \fn QString ControlDockWidget::currentMapServer() const

 Return the current map server name.
 */
QString ControlDockWidget::currentMapServer() const {
  return ui->mapserverComboBox->currentText();
}



/*!
 \fn QString ControlDockWidget::currentMapProjection() const

 Return the current projection name.
 */
QString ControlDockWidget::currentMapProjection() const {
  const QRadioButton* widgets[] = {
    ui->lonlatRadioButton,
    ui->sphericalRadioButton,
    ui->mercatorRadioButton,
    ui->lambertconformalRadioButton,
    ui->stereographicRadioButton
  };
  QString projection = widgets[0]->text();

  for (unsigned int i=1; i<sizeof(widgets)/sizeof(widgets[0]); i++) {
    const QRadioButton *w = widgets[i];

    if (w->isChecked()) {
      projection = w->text();
      break;
    }
  }

  return projection;
}



/*!
 \fn void ControlDockWidget::setCurrentMapServer(const QString& s)

 Set the current map server name.
 */
void ControlDockWidget::setCurrentMapServer(const QString& s) {

  for (unsigned int i=0; i<sizeof(mapServers)/sizeof(mapServers[0]); i++) {
    const MapServerInfo& info = mapServers[i];

    if (s == info.mMapSever) {
      ui->mapserverComboBox->setCurrentIndex(i);
      break;
    }
  }
}



/*!
 \fn void ControlDockWidget::setCurrentMapServer(const QString& s)
 
 Set the current map server name.
 */
void ControlDockWidget::setCurrentMapProjection(const QString& s) {
  QRadioButton* widgets[] = {ui->lonlatRadioButton, ui->sphericalRadioButton, ui->mercatorRadioButton, ui->lambertconformalRadioButton, ui->stereographicRadioButton};

  for (unsigned int i=0; i<sizeof(widgets)/sizeof(widgets[0]); i++) {
    QRadioButton *w = widgets[i];

    if (w->text() == s) {
      w->setChecked(true);
      onMapProjectionChanged();
      break;
    }
  }
}



void ControlDockWidget::onGuiFontsizeChanged(int fontsize) {

  QFont font = qApp->font();

  //QFont font("Helvetica", fontsize, QFont::Bold);
   
  //qDebug() << fontsize << font.pointSize() << font << qApp->font();

  font.setPointSizeF(float(fontsize));
  qApp->setFont(font);
  QApplication::setFont(font);

  foreach (QWidget *widget, QApplication::allWidgets()) {
    widget->setFont(QApplication::font());
    widget->update();
  }

  
  Q_EMIT requestRedraw();
}

void ControlDockWidget::onMaplinesChanged() {
  VisOptions* vOptions = VisOptionsSingleton::getInstance();

  if (ui->maplineNone->isChecked()) {
    vOptions->setMapOverlayFile("none");
  } else if (ui->maplineNations->isChecked()) {
    vOptions->setMapOverlayFile("/image/map_world.bin");
  } else if (ui->maplineStates->isChecked()) {
    vOptions->setMapOverlayFile("/image/map_states.bin");
  } else if (ui->maplineStates_hires->isChecked()) {
    vOptions->setMapOverlayFile("/image/states_hires.bin");
  } else if (ui->maplineCounties->isChecked()) {
    vOptions->setMapOverlayFile("/image/map_counties.bin");
  }

  Q_EMIT requestRedraw();
}



void ControlDockWidget::onMaplineColorIndexChanged(int index) {

  if (index<0) {
    return;
  }

  VisOptions* vOptions = VisOptionsSingleton::getInstance();
  //clear focus on the widget, prevent some mis-operations
  ui->maplineColorComboBox->clearFocus();
  vOptions->setMaplineColor( maplineColorStrings[index] );
  vOptions->setMaplineColorIndex( index );
  Q_EMIT requestRedraw();
}



/*!
 \fn void ControlDockWidget::setCurrentMapServer(const QString& s)
 
 Set the current map server name.
 */
void ControlDockWidget::closeEvent(QCloseEvent *) {
  Q_EMIT widgetClosed();
}



void ControlDockWidget::doResize(int w, int h) {
  QWidget *wgt = ui->mainVerticalLayout->parentWidget();
  wgt->resize(QSize(w-14, h));
  h = ui->viewTab->height();
  ui->viewTab->resize(w, h);
  m_viewSplitter->resize( w-9, h-2);
}



/*!
 \fn void ControlDockWidget::setCurrentMapServer(const QString& s)
 
 Set the current map server name.
 */
void ControlDockWidget::resizeEvent(QResizeEvent *event) {
  const QSize winsize = event->size();
  QWidget* w = ui->mainVerticalLayout->parentWidget();
  this->doResize(winsize.width(), w->size().height());
}



/*!
 \fn void ControlDockWidget::onMapProjectionChanged()
 
 Change the map projection type.
 */
void ControlDockWidget::onMapProjectionChanged() {
  const QRadioButton *buttons[] = {
    ui->lonlatRadioButton,
    ui->sphericalRadioButton,
    ui->mercatorRadioButton,
    ui->lambertconformalRadioButton,
    ui->stereographicRadioButton
  };
  const int nButtons = sizeof(buttons)/sizeof(buttons[0]);
  int index = 0;

  for (int i=0; i<nButtons; i++) {

    if (buttons[i]->isChecked()) {
      index = i;
      break;
    }
  }

  const RSIGState::MapProjectionType projections[]={
    RSIGState::PROJECTION_LONGLAT
    ,RSIGState::PROJECTION_SPHERICAL
    ,RSIGState::PROJECTION_MERCATOR
    ,RSIGState::PROJECTION_LAMBERTCONFORMALCONIC
    ,RSIGState::PROJECTION_STEREOGRAPHIC
  };

  if (index>=0 && index<nButtons) {
    Q_EMIT mapProjectionChanged(projections[index]);
    Q_EMIT requestRedraw();
    VisOptions* vOptions = VisOptionsSingleton::getInstance();
    vOptions->setCurProjection(projections[index]);
    vOptions->setViewResetFlag(0);
    QString curProjection = this->currentMapProjection();

    if ( (projections[index] != RSIGState::PROJECTION_LONGLAT && projections[index] != RSIGState::PROJECTION_MERCATOR) && vOptions->getDrawModisTrueColor()) {
      this->postMessage("TrueColor imagery cannot be displayed in the " + curProjection + " projection.", RSIGState::WARNING_TEXT);
    }
  }
}



/*!
 \fn void ControlDockWidget::setCurrentMapServer(const QString& s)
 
 Set the current map server name.
 */
void ControlDockWidget::onMapServerIndexChanged(int index) {
  if (index<0) {
    return;
  }

  //clear focus on the widget, prevent some mis-operations
  ui->mapserverComboBox->clearFocus();
  QRadioButton* buttons[] = {
    ui->lonlatRadioButton,
    ui->sphericalRadioButton,
    ui->mercatorRadioButton,
    ui->lambertconformalRadioButton,
    ui->stereographicRadioButton
  };
  const int nButton = sizeof(buttons)/sizeof(buttons[0]);
  const struct MapServerInfo *p = &mapServers[index];
  const int supportedProjFlag = p->mSupportedProjectionFlag;

  for (int i=0; i<nButton; i++) {
    QRadioButton *b = buttons[i];
    const bool supportflag = (supportedProjFlag & (1<<i)) != 0;
    b->setDisabled(!supportflag);
  }

  // if only support mecator projection, eg. all the online maps

  if (supportedProjFlag == 4) {

    if (!ui->mercatorRadioButton->isChecked()) {
      ui->mercatorRadioButton->setChecked(true);
      Q_EMIT mapProjectionChanged(RSIGState::PROJECTION_MERCATOR);
    }
  }

  Q_EMIT mapServerChanged(bool(p->mUseMarbleWidget), p->mTheme);
}



void ControlDockWidget::onDateChanged() {
  RSIGState *gState = RSIGStateSingleton::getInstance();
  gState->setDate( ui->dateDateEdit->dateTime().toString(Qt::ISODate) );
  generateDefaultPngFilename();
  Q_EMIT requestRedraw();
}

void ControlDockWidget::onDurationChanged() {
  const int duration = ui->durationSpinBox->value();
  RSIGState *gState = RSIGStateSingleton::getInstance();
  gState->setDuration(duration);

  /* update the timeslider limits */
  int ind1 = ui->aggregate1->findText("None");
  int ind2 = ui->aggregate2->findText("None");
  int ind3 = ui->aggregate3->findText("None");
  int ind4 = ui->aggregate4->findText("None");
  int ind5 = ui->aggregate5->findText("None");
  int nTimesteps = 24; // default

  switch (gState->mTimebaseType) {
  case RSIGState::TIMEBASE_HOURLY:
    nTimesteps = duration * 24 - 1;
    if (ind1 == -1) { ui->aggregate1->insertItem(0, "None"); ui->aggregate1->setCurrentIndex(0); }
    if (ind2 == -1) { ui->aggregate2->insertItem(0, "None"); ui->aggregate2->setCurrentIndex(0); }
    if (ind3 == -1) { ui->aggregate3->insertItem(0, "None"); ui->aggregate3->setCurrentIndex(0); }
    if (ind4 == -1) { ui->aggregate4->insertItem(0, "None"); ui->aggregate4->setCurrentIndex(0); }
    if (ind5 == -1) { ui->aggregate5->insertItem(0, "None"); ui->aggregate5->setCurrentIndex(0); }
    ui->forwardButton24->setEnabled(true);
    ui->backButton24->setEnabled(true);
    break;
  case RSIGState::TIMEBASE_DAILY:
    nTimesteps = duration - 1;
    if (ind1 >= 0) { ui->aggregate1->removeItem(ind1); }
    if (ind2 >= 0) { ui->aggregate2->removeItem(ind2); }
    if (ind3 >= 0) { ui->aggregate3->removeItem(ind3); }
    if (ind4 >= 0) { ui->aggregate4->removeItem(ind4); }
    if (ind5 >= 0) { ui->aggregate5->removeItem(ind5); }
    ui->forwardButton24->setEnabled(false);
    ui->backButton24->setEnabled(false);
    break;
  case RSIGState::TIMEBASE_DAILY_LST:
    nTimesteps = duration - 1;
    if (ind1 >= 0) { ui->aggregate1->removeItem(ind1); }
    if (ind2 >= 0) { ui->aggregate2->removeItem(ind2); }
    if (ind3 >= 0) { ui->aggregate3->removeItem(ind3); }
    if (ind4 >= 0) { ui->aggregate4->removeItem(ind4); }
    if (ind5 >= 0) { ui->aggregate5->removeItem(ind5); }
    ui->forwardButton24->setEnabled(false);
    ui->backButton24->setEnabled(false);
    break;
  case RSIGState::TIMEBASE_MONTHLY:
    nTimesteps = duration - 1;
    if (ind1 >= 0) { ui->aggregate1->removeItem(ind1); }
    if (ind2 >= 0) { ui->aggregate2->removeItem(ind2); }
    if (ind3 >= 0) { ui->aggregate3->removeItem(ind3); }
    if (ind4 >= 0) { ui->aggregate4->removeItem(ind4); }
    if (ind5 >= 0) { ui->aggregate5->removeItem(ind5); }
    ui->forwardButton24->setEnabled(false);
    ui->backButton24->setEnabled(false);
    break;
  default:
    break;
  }

  generateDefaultPngFilename();
  VisOptions* vOptions = VisOptionsSingleton::getInstance();
  ui->filenameLineEdit->setText(vOptions->getFilenamePngDefault());
  ui->timestepSlider->setMaximum(nTimesteps);
}



/*!
 \fn void ControlDockWidget::onOpenWebClicked()

 Launch a desktop service to open a default web browser.
 */
void ControlDockWidget::onOpenWebClicked() {
  const QUrl url("https://www.epa.gov/rsig/");
  QDesktopServices::openUrl(url);
}



void ControlDockWidget::onFileOrientedDownloaderClicked() {

  /*
   * Here we want this pop-up dialog to NOT BLOCK user interactions with the
   * other application windows.
   * To achieve this:
   * 1. Declare a dynamically allocated object (with new).
   * 2. Mark that object to be deleted upon close.
   * 3. Call show() (instead of exec() which blocks).
   */

  FileOrientedDownloader* fileOrientedDownloader = new FileOrientedDownloader(0);
  fileOrientedDownloader->setAttribute(Qt::WA_DeleteOnClose);
  fileOrientedDownloader->show();
}



/*!
 \fn void ControlDockWidget::onEmailClicked()

 Launch a desktop service to open a default email client.
 */
void ControlDockWidget::onEmailClicked() {
  const QUrl url("mailto:rsig@epa.gov");
  QDesktopServices::openUrl(url);
}



/*!
 \fn void ControlDockWidget::onExitClicked()

 Exit the application. It will call qApp->closeAllWindows();
 */
void ControlDockWidget::onExitClicked() {
  qApp->closeAllWindows();
}



/*!
 \fn void ControlDockWidget::onFilterParametersClicked()

 Set CMAQ filter parameters.
 */
void ControlDockWidget::onFilterParametersClicked() {

  /*
   * Here we want this pop-up dialog to BLOCK user interactions with other
   * application windows until the user closes the dialog.
   * To achieve this:
   * 1. Declare object on the stack (not new).
   * 2. Call exec() which blocks.
   */

  FilterSettingDialog filterSettingDialog;
  filterSettingDialog.exec();
}



/*!
 \fn void ControlDockWidget::onRegridParametersClicked()

 Set CMAQ regrid parameters.
 */
void ControlDockWidget::onRegridParametersClicked() {

  /*
   * Here we want this pop-up dialog to BLOCK user interactions with other
   * application windows until the user closes the dialog.
   * To achieve this:
   * 1. Declare object on the stack (not new).
   * 2. Call exec() which blocks.
   */

  RegridParmsDialog regridParmsDialog;
  regridParmsDialog.exec(); // Synchronus: blocks until close.
}



/*!
 \fn void ControlDockWidget::postMessage(const QString &msg, int mode)

 Display text information in the text output region.
 The parameter \e msg is the text message. 
 The parameter \e mode is the text output style.
 The \e mode values can be found in RSIGState::RsigMessageType.
 */
void ControlDockWidget::postMessage(const QString &msg, int mode) {
  QString text;
  switch (mode) {

  // NOTE: Be sure to leave a space after the closing font tag.
  // Otherwise Linux will show boxes instead of proper text.

  case RSIGState::NORMAL_TEXT:
    text = "<font color=green>" + msg + "</font> ";
    break;
  case RSIGState::WARNING_TEXT:
    text = "<font color=blue>" + msg + "</font> ";
    break;
  case RSIGState::DEBUG_TEXT:
    text = "<font color=black>" + msg + "</font> ";
    break;
  case RSIGState::ERROR_TEXT:
    text = "<font color=red>" + msg + "</font> ";
    break;
  case RSIGState::FATAL_ERROR_TEXT:
    text = "<font color=red size=10>" + msg + "</font> ";
    break;
  default:
    text = msg;
    break;
  };

  QMetaObject::invokeMethod(ui->textBrowser, "append", Qt::QueuedConnection, Q_ARG(QString, text));
}



/*!
 \fn void ControlDockWidget::onBoundingGeoBoxChanged(const QRectF &rectf)

 Set the bounding box size using into value \e rectf.
 */

void ControlDockWidget::set_currentGuiBbox_minLat(double minLat) {
  currentGuiBbox_minLat = minLat;
}



void ControlDockWidget::set_currentGuiBbox_maxLat(double maxLat) {
  currentGuiBbox_maxLat = maxLat;
}



void ControlDockWidget::set_currentGuiBbox_minLon(double minLon) {
  currentGuiBbox_minLon = minLon;
}



void ControlDockWidget::set_currentGuiBbox_maxLon(double maxLon) {
  currentGuiBbox_maxLon = maxLon;
}



double ControlDockWidget::get_currentGuiBbox_minLat() {
  return currentGuiBbox_minLat;
}



double ControlDockWidget::get_currentGuiBbox_maxLat() {
  return currentGuiBbox_maxLat;
}



double ControlDockWidget::get_currentGuiBbox_minLon() {
  return currentGuiBbox_minLon;
}



double ControlDockWidget::get_currentGuiBbox_maxLon() {
  return currentGuiBbox_maxLon;
}



void ControlDockWidget::onBoundingGeoBoxChanged(const QRectF &rectf) {
  double x0 = rectf.x();
  double x1 = x0+rectf.width();
  double y0 = rectf.y();
  double y1 = y0+rectf.height();

  if (x0>x1) {
    double t=x0;
    x0=x1; x1=t;
  }

  if (y0>y1) {
    double t=y0;
    y0=y1; y1=t;
  }

  const bool f = blockSignals(true);
  QDoubleSpinBox* widgets[4]={
    ui->northDSpinBox,
    ui->southDSpinBox,
    ui->eastDSpinBox,
    ui->westDSpinBox
  };
  const double values[4]={y1, y0, x1, x0};

  for (int i=0; i<4; i++) {
    QDoubleSpinBox *w = widgets[i];
    const double v = values[i];

    if (w->value()!=v) {
      w->setValue(v);
    }
  }

  set_currentGuiBbox_minLat(y0);
  set_currentGuiBbox_maxLat(y1);
  set_currentGuiBbox_minLon(x0);
  set_currentGuiBbox_maxLon(x1);
  blockSignals(f);
}



void ControlDockWidget::onSelectedLayer1Changed(int) {
}



void ControlDockWidget::onSelectedLayer2Changed(int) {
}



void ControlDockWidget::onSelectedLayer3Changed(int) {
}



void ControlDockWidget::onSelectedLayer4Changed(int) {
}



void ControlDockWidget::onSelectedLayer5Changed(int) {
}



void ControlDockWidget::onTest(int value) {
  qDebug() << "test value is " << value;
}



/*!
 \fn void void ControlDockWidget::onBoundingGeoBoxChanged()

 Internal use. Collect the bounding box size. Emit the signal boundingGeoBoxChanged().
 */
void ControlDockWidget::onBoundingGeoBoxChanged() {
  double x0 = ui->westDSpinBox->value();
  double x1 = ui->eastDSpinBox->value();
  double y0 = ui->southDSpinBox->value();
  double y1 = ui->northDSpinBox->value();
  set_currentGuiBbox_minLat(y0);
  set_currentGuiBbox_maxLat(y1);
  set_currentGuiBbox_minLon(x0);
  set_currentGuiBbox_maxLon(x1);

  if (x0>x1) {
    const double t=x0;
    x0=x1; x1=t;
  }

  if (y0>y1) {
    const double t=y0;
    y0=y1; y1=t;
  }

  const QRectF r(x0, y0, x1-x0, y1-y0);
  Q_EMIT boundingGeoBoxChanged(r);
}



void ControlDockWidget::onShowLonlatLinesChanged(int val) {
  RSIGState *gState = RSIGStateSingleton::getInstance();
  gState->mShowLonlatLines = bool(val>0);
  Q_EMIT requestRedraw();
}

void ControlDockWidget::onShowScaleChanged(int val) {
  RSIGState *gState = RSIGStateSingleton::getInstance();
  gState->mShowScale = bool(val>0);
  Q_EMIT requestRedraw();
}



void ControlDockWidget::onSetMapDoubleClickOption1(int val) {
  RSIGState *gState = RSIGStateSingleton::getInstance();
  gState->mShowGooglemapUrl = bool(val>0);
  Q_EMIT requestRedraw();
}

void ControlDockWidget::onSetMapDoubleClickOption2(int val) {
  RSIGState *gState = RSIGStateSingleton::getInstance();
  gState->mOpenGooglemapUrl = bool(val>0);
  Q_EMIT requestRedraw();
}



void ControlDockWidget::onLonlatLabelOrientationChanged(int val) {
  VisOptions* vOptions = VisOptionsSingleton::getInstance();
  vOptions->setLonlatLabelOrientation(val);
  Q_EMIT requestRedraw();
}



void ControlDockWidget::onRulerLonSpinBoxChanged(double value) {
  VisOptions* vOptions = VisOptionsSingleton::getInstance();
  vOptions->setRulerLon(value);
  Q_EMIT requestRedraw();
}



void ControlDockWidget::onRulerLatSpinBoxChanged(double value) {
  VisOptions* vOptions = VisOptionsSingleton::getInstance();
  vOptions->setRulerLat(value);
  Q_EMIT requestRedraw();
}



void ControlDockWidget::onCalipsoScaleFactorChanged(double value) {
  VisOptions* vOptions = VisOptionsSingleton::getInstance();
  vOptions->setCalipsoScaleFactor(value);
  Q_EMIT requestRedraw();
}



void ControlDockWidget::onWindVectorScaleFactorChanged(double value) {
  VisOptions* vOptions = VisOptionsSingleton::getInstance();
  vOptions->setWindVectorScaleFactor(value);
  Q_EMIT requestRedraw();
}



void ControlDockWidget::onWindVectorTipSizeChanged(double value) {
  VisOptions* vOptions = VisOptionsSingleton::getInstance();
  vOptions->setWindVectorTipSize(value);
  Q_EMIT requestRedraw();
}



void ControlDockWidget::onWindVectorSparsenessChanged(double value) {
  VisOptions* vOptions = VisOptionsSingleton::getInstance();
  vOptions->setWindVectorSparseness(value);
  ui->windVectorSparseness->setValue(vOptions->getWindVectorSparseness());
  Q_EMIT requestRedraw();
}



void ControlDockWidget::onWindVectorAutosparseChanged(bool val) {
  VisOptions* vOptions = VisOptionsSingleton::getInstance();
  vOptions->setWindVectorAutosparse(val);

  if (val) {
    ui->windVectorSparseness->setEnabled(false);
    ui->windVectorSparsenessLabel->setEnabled(false);
  } else {
    ui->windVectorSparseness->setEnabled(true);
    ui->windVectorSparsenessLabel->setEnabled(true);
    ui->windVectorSparseness->setValue(vOptions->getWindVectorSparseness());

  }
  
  Q_EMIT requestRedraw();
}



void ControlDockWidget::onCeilometerMinimumAltitudeChanged(double value) {
  VisOptions* vOptions = VisOptionsSingleton::getInstance();
  vOptions->setCeilometerMinAltitude(value);
  Q_EMIT requestRedraw();
}

void ControlDockWidget::onCeilometerMaximumAltitudeChanged(double value) {
  VisOptions* vOptions = VisOptionsSingleton::getInstance();
  vOptions->setCeilometerMaxAltitude(value);
  Q_EMIT requestRedraw();
}

void ControlDockWidget::onCeilometerPlotScrollerChanged(int value) {
  VisOptions* vOptions = VisOptionsSingleton::getInstance();
  vOptions->setCeilometerPlotScrollerValue(value);
  Q_EMIT requestRedraw();
}

void ControlDockWidget::onCeilometerSizeChanged(int value) {
  VisOptions* vOptions = VisOptionsSingleton::getInstance();
  vOptions->setCeilometerSize(value);
  Q_EMIT requestRedraw();
}

void ControlDockWidget::onCeilometerShowLocationsToggled(bool value) {
  VisOptions* vOptions = VisOptionsSingleton::getInstance();
  vOptions->setCeilometerShowLocations(value);
  Q_EMIT requestRedraw();
}

void ControlDockWidget::onCeilometerShowMixingLayerToggled(bool value) {
  VisOptions* vOptions = VisOptionsSingleton::getInstance();
  vOptions->setCeilometerShowMixingLayer(value);
  Q_EMIT requestRedraw();
}


void ControlDockWidget::onPandoraMinimumAltitudeChanged(double value) {
  VisOptions* vOptions = VisOptionsSingleton::getInstance();
  vOptions->setPandoraMinAltitude(value);
  Q_EMIT requestRedraw();
}

void ControlDockWidget::onPandoraMaximumAltitudeChanged(double value) {
  VisOptions* vOptions = VisOptionsSingleton::getInstance();
  vOptions->setPandoraMaxAltitude(value);
  Q_EMIT requestRedraw();
}

void ControlDockWidget::onPandoraPlotScrollerChanged(int value) {
  VisOptions* vOptions = VisOptionsSingleton::getInstance();
  vOptions->setPandoraPlotScrollerValue(value);
  Q_EMIT requestRedraw();
}

void ControlDockWidget::onPandoraSizeChanged(int value) {
  VisOptions* vOptions = VisOptionsSingleton::getInstance();
  vOptions->setPandoraSize(value);
  Q_EMIT requestRedraw();
}

void ControlDockWidget::onPandoraShowLocationsToggled(bool value) {
  VisOptions* vOptions = VisOptionsSingleton::getInstance();
  vOptions->setPandoraShowLocations(value);
  Q_EMIT requestRedraw();
}


void ControlDockWidget::onNeubrewMinimumAltitudeChanged(double value) {
  VisOptions* vOptions = VisOptionsSingleton::getInstance();
  vOptions->setNeubrewMinAltitude(value);
  Q_EMIT requestRedraw();
}



void ControlDockWidget::onNeubrewMaximumAltitudeChanged(double value) {
  VisOptions* vOptions = VisOptionsSingleton::getInstance();
  vOptions->setNeubrewMaxAltitude(value);
  Q_EMIT requestRedraw();
}



void ControlDockWidget::onNeubrewPlotScrollerChanged(int value) {
  VisOptions* vOptions = VisOptionsSingleton::getInstance();
  vOptions->setNeubrewPlotScrollerValue(value);
  Q_EMIT requestRedraw();
}



void ControlDockWidget::onNeubrewSizeChanged(int value) {
  VisOptions* vOptions = VisOptionsSingleton::getInstance();
  vOptions->setNeubrewSize(value);
  Q_EMIT requestRedraw();
}



void ControlDockWidget::onNeubrewShowLocationsToggled(bool value) {
  VisOptions* vOptions = VisOptionsSingleton::getInstance();
  vOptions->setNeubrewShowLocations(value);
  Q_EMIT requestRedraw();
}



void ControlDockWidget::onRadioButton2DToggled(bool value) {
  VisOptions* vOptions = VisOptionsSingleton::getInstance();
  
  // turn marble navigator off in 3D mode
  RSIGState *gState = RSIGStateSingleton::getInstance();

  
  if (value == true) {
    vOptions->setDisplayMode(0);

    const QString curName = ui->vizVarCombobox->currentText();
    VisDataList* vislist = VisDataListSingleton::getInstance();
    VisDataObject* visObj = vislist->getObjectByName(curName);
    if (visObj) {
      const GeoDataObject* const geoDataObject = qobject_cast<GeoDataObject*>(visObj->parent());
      const emvl::Dataset* const dataset       = geoDataObject ? geoDataObject->model() : 0;
      const bool isGrid                        = dataset ? dataset->isStructuredGrid() : 0;
      switchSpecialControlsWidget( isGrid, visObj->objectName() );
    }
    //qDebug() << "2D display";

    // 2d mode - navigator on if user wants it

    if (ui->showNavigatorCheckBox->checkState() == Qt::Checked) {
      gState->mShowNavigator = 1;
    }

    // turn off 3D ruler
    ui->rulerCheckBox->setEnabled(false);
    ui->rulerLonSpinBox->setEnabled(false);
    ui->rulerLatSpinBox->setEnabled(false);

    // lock layer option
    //ui->ShowCMAQLayer->setEnabled(false);

    // turn off CMAQ row/column
    ui->ShowCMAQRow->setEnabled(false);
    ui->ShowCMAQCol->setEnabled(false);

    // enable map options
    ui->mapserverComboBox->setEnabled(true);
    ui->lonlatRadioButton->setEnabled(true);
    ui->sphericalRadioButton->setEnabled(true);
    ui->mercatorRadioButton->setEnabled(true);
    ui->lambertconformalRadioButton->setEnabled(true);
    ui->stereographicRadioButton->setEnabled(true);
    //ui->showLonlatCheckBox->setEnabled(true);
    ui->showNavigatorCheckBox->setEnabled(true);
  }
  
  Q_EMIT requestRedraw();
}

void ControlDockWidget::onRadioButton3DToggled(bool value) {

  VisOptions* vOptions = VisOptionsSingleton::getInstance();
  
  // turn marble navigator off in 3D mode
  RSIGState *gState = RSIGStateSingleton::getInstance();

  if (value == true) {
    //qDebug() << "3D display";

    //vOptions->setMode2D(false);
    vOptions->setDisplayMode(1);
    const QString curName = ui->vizVarCombobox->currentText();
    VisDataList* vislist = VisDataListSingleton::getInstance();
    VisDataObject* visObj = vislist->getObjectByName(curName);
    bool isGrid = false;
    if (visObj) {
      const GeoDataObject* const geoDataObject = qobject_cast<GeoDataObject*>(visObj->parent());
      const emvl::Dataset* const dataset       = geoDataObject ? geoDataObject->model() : 0;
      isGrid = dataset ? dataset->isStructuredGrid() : 0;
      switchSpecialControlsWidget( isGrid, visObj->objectName() );
    }
    
    // 3d mode - navigator always off
    gState->mShowNavigator = 0;
    // turn thresholding on
    ui->ThresholdSlider->setEnabled(true);
    ui->ThresholdLabel->setEnabled(true);
    ui->thresholdCheckBox->setEnabled(true);
    // turn on 3D ruler
    ui->rulerCheckBox->setEnabled(true);
    ui->rulerLonSpinBox->setEnabled(true);
    ui->rulerLatSpinBox->setEnabled(true);
    // unlock layer option for gridded data (e.g. cmaq)

    //if ( curName.contains( "cmaq" ) ||
    //     curName.contains( "omibehrioapi" ) ||
    //     curName.contains( "geoschem" ) ||
    //     curName.contains( "hrrr" ) ||
    //     curName.contains( "compare" ) ||
    if ( isGrid ) {
      ui->ShowCMAQLayer->setEnabled(true);
      // turn on CMAQ row/column if CMAQ is selected
      ui->ShowCMAQRow->setEnabled(true);
      ui->ShowCMAQCol->setEnabled(true);
    }

    // disable map options
    ui->mapserverComboBox->setEnabled(false);
    ui->lonlatRadioButton->setEnabled(false);
    ui->sphericalRadioButton->setEnabled(false);
    ui->mercatorRadioButton->setEnabled(false);
    ui->lambertconformalRadioButton->setEnabled(false);
    ui->stereographicRadioButton->setEnabled(false);
    ui->showNavigatorCheckBox->setEnabled(false);
  } 
  
  Q_EMIT requestRedraw();

}

void ControlDockWidget::onRadioButtonPanelDisplayToggled(bool value) {
  VisOptions* vOptions = VisOptionsSingleton::getInstance();

  // turn marble navigator off in Panel mode
  RSIGState *gState = RSIGStateSingleton::getInstance();

  if (value == true) {
    //qDebug() << "panel display";
    vOptions->setDisplayMode(2);

     // panel mode - navigator always off
    gState->mShowNavigator = 0;

    // turn off 3D ruler
    ui->rulerCheckBox->setEnabled(false);
    ui->rulerLonSpinBox->setEnabled(false);
    ui->rulerLatSpinBox->setEnabled(false);

    // enable panel display controls
    switchSpecialControlsWidget( false, "whatever" ); // not worried about source name when togglingpanel controls
  } else {
    // disable panel display controls
    switchSpecialControlsWidget( true, "whatever" ); // not worried about source name when togglingpanel controls
  }

  

  Q_EMIT requestRedraw();

}



void ControlDockWidget::onPanelDisplaySource1Changed(int index) {

  //qDebug() << "index=" << index;
  
  if (index>=0) {

    // clear any previous checkboxes for selecting probe locations
    ui->panelDisplayListWidget->clear();
    
    // get the name from the combo box
    const QString curSelection = ui->panelDisplaySource1->currentText().remove(".point");
    setPanelDisplaySource1Selection(curSelection);

    //qDebug() << "here" << curSelection;
    
    GeoDataList* const lst = GeoDataListSingleton::getInstance();
    const int list_size = lst->size();

    for ( int ind = 0; ind < list_size; ++ind ) {
      GeoDataObject* const obj = lst->getObject(ind);
      const QString thisBaseName = obj->objectName();
      
      if (thisBaseName.startsWith(curSelection)) {
        GeometryObjectType* const myData = obj->model();

        if (thisBaseName.contains("ceilometer") || thisBaseName.contains("pandora") ) {
          std::vector<size_t>      profileDatasetIndices;
          std::vector<std::string> profileDatasetVariables;
          std::vector<std::string> profileDatasetUnits;
          std::vector<long long>   profileDatasetIds;
          std::vector<double>      profileDatasetLongitudes;
          std::vector<double>      profileDatasetLatitudes;
          std::vector<std::string> profileDatasetNames;

          cca::getCeilometerInfo( profileDatasetIndices,
                                  profileDatasetVariables,
                                  profileDatasetUnits,
                                  profileDatasetIds,
                                  profileDatasetLongitudes,
                                  profileDatasetLatitudes,
                                  profileDatasetNames );

          //qDebug() << "profile size" << profileDatasetIds.size() << profileDatasetNames.size();

          for (size_t myInd = 0; myInd < profileDatasetNames.size(); ++myInd) {
            const QString thisName( profileDatasetNames[ myInd ].c_str() );
            //qDebug() << "profile name" << thisName;
            QListWidgetItem* item = new QListWidgetItem(thisName, ui->panelDisplayListWidget);
            item->setFlags(item->flags() | Qt::ItemIsUserCheckable); // set checkable flag
            item->setCheckState(Qt::Unchecked); // initialize check state
          }

        } else if ( myData->cellType() == emvl::Dataset::POINT &&
                    ! myData->isVector() ) {
          const std::vector<std::string>& notes = myData->allDistinctNotes();

          if ( notes.size() ) {

            // Reduce distinct notes down to unique set:

            std::vector<std::string> uniqueNotes( notes ); //x
            std::stable_sort( uniqueNotes.begin(), uniqueNotes.end() );
            uniqueNotes.erase( std::unique( uniqueNotes.begin(),
                                            uniqueNotes.end() ),
                              uniqueNotes.end() );
            const size_t count = uniqueNotes.size();

            for ( size_t index = 0; index < count; ++index ) {
              const std::string& uniqueNote = uniqueNotes[ index ];
              QString name = QString::fromStdString( uniqueNote );
              QListWidgetItem* item =
                new QListWidgetItem( name, ui->panelDisplayListWidget );
              item->setFlags( item->flags() | Qt::ItemIsUserCheckable );
              item->setCheckState( Qt::Unchecked );
            }
          }
        }
      }
    }

    Q_EMIT requestRedraw();
  }
}



void ControlDockWidget::onPanelDisplaySource2Changed(int index) {

  if (index>=0) {

    // get the name from the combo box
    const QString curSelection = ui->panelDisplaySource2->currentText().remove(".polygon");
    setPanelDisplaySource2Selection(curSelection);

    Q_EMIT requestRedraw(); 
  } 
}


QStringList ControlDockWidget::getDisplayPanelProbelist() {

  QStringList melist;
  
  for (int row = 0; row < ui->panelDisplayListWidget->count(); row++) {
    QListWidgetItem *item = ui->panelDisplayListWidget->item(row);
    // process item
    
    if (item->checkState() == Qt::Checked) {
      //qDebug() << item->text();
      melist.append(item->text());
    }
  }
  
  return melist;
}


void ControlDockWidget::onPanelDisplayListWidgetClicked() {
  Q_EMIT requestRedraw();
}


void ControlDockWidget::setPanelDisplaySource1Selection(QString selection) {
  panelDisplaySource1Selection = selection;
}

QString ControlDockWidget::getPanelDisplaySource1Selection() {
  return panelDisplaySource1Selection;
}

void ControlDockWidget::setPanelDisplaySource2Selection(QString selection) {
  panelDisplaySource2Selection = selection;
}

QString ControlDockWidget::getPanelDisplaySource2Selection() {
  return panelDisplaySource2Selection;
}




void ControlDockWidget::onViewResetPressed() {
  //printf("in onViewResetPressed %d\n", value);
  VisOptions* vOptions = VisOptionsSingleton::getInstance();
  vOptions->setViewResetFlag(1);
  // also reset the bbox to the data
  const QPointF topLeft     = QPointF(vOptions->getBboxMinLon(), vOptions->getBboxMaxLat());
  const QPointF bottomRight = QPointF(vOptions->getBboxMaxLon(), vOptions->getBboxMinLat());
  const QRectF origGeoRect  = QRectF(topLeft, bottomRight);
  this->setRegionOfInterest(origGeoRect);
  Q_EMIT requestRedraw();
}



void ControlDockWidget::onViewAutozoomPressed() {
  //printf("in onViewResetPressed %d\n", value);
  VisOptions* vOptions = VisOptionsSingleton::getInstance();
  vOptions->setViewResetFlag(4);
  Q_EMIT requestRedraw();
}



void ControlDockWidget::onCheckboxFlattenCMAQChanged(int check_flag) {
  VisOptions* vOptions = VisOptionsSingleton::getInstance();

  if (check_flag == Qt::Checked) {
    vOptions->setFlattenCMAQ(true);
  } else {
    vOptions->setFlattenCMAQ(false);
  }

  Q_EMIT requestRedraw();
}



void ControlDockWidget::onCheckboxPersistBEHRChanged(int check_flag) {
  VisOptions* vOptions = VisOptionsSingleton::getInstance();

  if (check_flag == Qt::Checked) {
    vOptions->setPersistBEHR(true);
    ui->persistBEHRHours->setEnabled(true);
  } else {
    vOptions->setPersistBEHR(false);
    ui->persistBEHRHours->setEnabled(false);
  }

  Q_EMIT requestRedraw();
}



void ControlDockWidget::onPersistBEHRHoursChanged(int value) {
  VisOptions* vOptions = VisOptionsSingleton::getInstance();
  vOptions->setPersistBEHRHours(value);
  Q_EMIT requestRedraw();
}



void ControlDockWidget::onCheckboxPersistMODISChanged(int check_flag) {
  VisOptions* vOptions = VisOptionsSingleton::getInstance();

  if (check_flag == Qt::Checked) {
    vOptions->setPersistMODIS(true);
    ui->persistMODISHours->setEnabled(true);
  } else {
    vOptions->setPersistMODIS(false);
    ui->persistMODISHours->setEnabled(false);
  }

  Q_EMIT requestRedraw();
}



void ControlDockWidget::onPersistMODISHoursChanged(int value) {
  VisOptions* vOptions = VisOptionsSingleton::getInstance();
  vOptions->setPersistMODISHours(value);
  Q_EMIT requestRedraw();
}



void ControlDockWidget::onCheckboxPersistVIIRSChanged(int check_flag) {
  VisOptions* vOptions = VisOptionsSingleton::getInstance();

  if (check_flag == Qt::Checked) {
    vOptions->setPersistVIIRS(true);
    ui->persistVIIRSHours->setEnabled(true);
  } else {
    vOptions->setPersistVIIRS(false);
    ui->persistVIIRSHours->setEnabled(false);
  }

  Q_EMIT requestRedraw();
}



void ControlDockWidget::onPersistVIIRSHoursChanged(int value) {
  VisOptions* vOptions = VisOptionsSingleton::getInstance();
  vOptions->setPersistVIIRSHours(value);
  Q_EMIT requestRedraw();
}



void ControlDockWidget::onCheckboxPersistTROPOMIChanged(int check_flag) {
  VisOptions* vOptions = VisOptionsSingleton::getInstance();

  if (check_flag == Qt::Checked) {
    vOptions->setPersistTROPOMI(true);
    ui->persistTROPOMIHours->setEnabled(true);
  } else {
    vOptions->setPersistTROPOMI(false);
    ui->persistTROPOMIHours->setEnabled(false);
  }

  Q_EMIT requestRedraw();
}

void ControlDockWidget::onPersistTROPOMIHoursChanged(int value) {
  VisOptions* vOptions = VisOptionsSingleton::getInstance();
  vOptions->setPersistTROPOMIHours(value);
  Q_EMIT requestRedraw();
}


void ControlDockWidget::onCheckboxPersistTEMPOChanged(int check_flag) {
  VisOptions* vOptions = VisOptionsSingleton::getInstance();

  if (check_flag == Qt::Checked) {
    vOptions->setPersistTEMPO(true);
    ui->persistTEMPOHours->setEnabled(true);
  } else {
    vOptions->setPersistTEMPO(false);
    ui->persistTEMPOHours->setEnabled(false);
  }

  Q_EMIT requestRedraw();
}

void ControlDockWidget::onPersistTEMPOHoursChanged(int value) {
  VisOptions* vOptions = VisOptionsSingleton::getInstance();
  vOptions->setPersistTEMPOHours(value);
  Q_EMIT requestRedraw();
}


void ControlDockWidget::onCheckboxPersistHMSChanged(int check_flag) {
  VisOptions* vOptions = VisOptionsSingleton::getInstance();

  if (check_flag == Qt::Checked) {
    vOptions->setPersistHMS(true);
    ui->persistHMSHours->setEnabled(true);
  } else {
    vOptions->setPersistHMS(false);
    ui->persistHMSHours->setEnabled(false);
  }

  Q_EMIT requestRedraw();
}



void ControlDockWidget::onPersistHMSHoursChanged(int value) {
  VisOptions* vOptions = VisOptionsSingleton::getInstance();
  vOptions->setPersistHMSHours(value);
  Q_EMIT requestRedraw();
}



void ControlDockWidget::onThresholdCheckboxChanged(int check_flag) {
  // get the name from the combo box
  const QString curName = ui->vizVarCombobox->currentText();
  // get an instance of the vislist (which contains all vis objects)
  VisDataList* vislist = VisDataListSingleton::getInstance();
  // extract the vis object corresponding to the selected variable from the combo box
  VisDataObject* visObj = vislist->getObjectByName(curName);

  if (visObj) {

    if (check_flag == Qt::Checked || check_flag == true) {
      visObj->setShowThreshold(true);
      ui->ThresholdSlider->setEnabled(true);
    } else {
      visObj->setShowThreshold(false);
      ui->ThresholdSlider->setEnabled(false);
    }
  }

  Q_EMIT requestRedraw();
}



void ControlDockWidget::onRulerCheckboxChanged(int check_flag) {
  VisOptions* vOptions = VisOptionsSingleton::getInstance();

  if (check_flag == Qt::Checked) {
    vOptions->setShowRuler(true);
  } else {
    vOptions->setShowRuler(false);
  }

  Q_EMIT requestRedraw();
}



void ControlDockWidget::run_downloader(QString coverage, int regrid, int layer, int aggregate, int dx, int dy, bool saveOriginalFlag, bool saveMetadataFlag) {
  // saveOriginalFLag = true if this is being called to save "original" data
  // saveMetadataFlag = true if this is being called to save metadata

  dx=dx+width();
  dy=dy+40;
  const double x0 = ui->westDSpinBox->value();
  const double y0 = ui->southDSpinBox->value();
  const double x1 = ui->eastDSpinBox->value();
  const double y1 = ui->northDSpinBox->value();
  const QDate xdate = ui->dateDateEdit->date();
  QRectF roi(QPointF(x0, y0), QPointF(x1, y1));  

  const int secPerDay = 86400;
  RSIGState* gState = RSIGStateSingleton::getInstance();
  int ndays = (gState->endingTimestamp.seconds() - gState->startingTimestamp.seconds() + 1) / secPerDay;
    
  VisOptions* vOptions = VisOptionsSingleton::getInstance();
  vOptions->setBboxMinLon(x0);
  vOptions->setBboxMinLat(y0);
  vOptions->setBboxMaxLon(x1);
  vOptions->setBboxMaxLat(y1);

  QStringList parse = coverage.split(".");
  QString sourceName = "";

  for (int sInd=0; sInd<parse.length()-1; sInd++) {
    sourceName.append(parse[sInd] + ".");
  }

  // remove trailing dot
  sourceName = sourceName.left(sourceName.length()-1);

  /*
   * Here we want this pop-up dialog to NOT BLOCK user interactions with the
   * other application windows.
   * To achieve this:
   * 1. Declare a dynamically allocated object (with new).
   * 2. Mark that object to be deleted upon close.
   * 3. Call show() (instead of exec() which blocks).
   */

  HttpDownloadDialog* downloader = new HttpDownloadDialog(this);
  downloader->setAttribute(Qt::WA_DeleteOnClose);

  downloader->setAggregate(aggregate);
  downloader->setRegrid(regrid);
  downloader->setRegionOfInterest(roi);
  downloader->setDateAndDays(xdate, ndays);
  downloader->setSource(sourceName);
  downloader->setCoverage(coverage);
  downloader->move(x()+dx, y()+dy);
  downloader->setLayer(layer);
  downloader->setSaveMetadataFlag(saveMetadataFlag);

  if (saveOriginalFlag || saveMetadataFlag) {
    connect(downloader, SIGNAL(workFinished(QObject*)), this, SLOT(onSaveWorkFinished(QObject *)));
  } else {
    connect(downloader, SIGNAL(workFinished(QObject*)), this, SLOT(onDownloadWorkFinished(QObject *)));
  }

  downloader->setAttribute(Qt::WA_DeleteOnClose);
  downloader->show(); // Non-blocking spawns thread and returns immediately.
  downloader->raise();
}



void ControlDockWidget::onDataClearClicked() {
  // clear all data selections in one easy step
  static QTreeWidget *tw = ui->varTree;
  QTreeWidgetItemIterator it(tw);

  while (*it) {

    if ( (*it)->checkState(0) == Qt::Checked ) {
      (*it)->setCheckState(0, Qt::Unchecked);
    }

    ++it;
  }

  ui->selectedData1->setToolTip("");
  ui->selectedData2->setToolTip("");
  ui->selectedData3->setToolTip("");
  ui->selectedData4->setToolTip("");
  ui->selectedData5->setToolTip("");
}



QString swap_compare_operator(QString source_string) {
  // Ensure that data source string is properly formatted.
  // In particular, compare variables need to have operator moved to end of string
  QString return_string = source_string; // default

  if (return_string.contains("abs_diff.")) {
    return_string.remove("abs_diff.");
    return_string.append("-abs_diff");

  } else if (return_string.contains("percent_diff.")) {
    return_string.remove("percent_diff.");
    return_string.append("-percent_diff");

  } else if (return_string.contains("diff.")) {
    return_string.remove("diff.");
    return_string.append("-diff");
  
  } else if (return_string.contains("ratio.")) {
    return_string.remove("ratio.");
    return_string.append("-ratio");
  }

  return return_string;
}



void ControlDockWidget::check_sources(QString s1, QString s2, QString s3, QString s4, QString s5, QDate date) {
  QString msg = "";
  int match = 0;
  bool ok;
  long selectedDate = date.toString(Qt::ISODate).remove(QString('-')).toLong(&ok, 10);

  for (int i=0; i<availNamesSize; ++i) {
    bool isInRange = selectedDate>=minAvailDate[i] && selectedDate<=maxAvailDate[i];

    if (availNames[i] != "") {

      if (s1.contains(availNames[i]+'.') && !isInRange) {
        msg.append("- ");
        msg.append(s1);
        msg.append("\n");
        msg.append("    have: ");
        msg.append(QString::number(minAvailDate[i]));
        msg.append(" to ");
        msg.append(QString::number(maxAvailDate[i]));
        msg.append("\n\n");
        match += 1;
      }

      if (s2.contains(availNames[i]+'.') && !isInRange) {
        msg.append("- ");
        msg.append(s2);
        msg.append("\n");
        msg.append("    have: ");
        msg.append(QString::number(minAvailDate[i]));
        msg.append(" to ");
        msg.append(QString::number(maxAvailDate[i]));
        msg.append("\n\n");
        match += 1;
      }

      if (s3.contains(availNames[i]+'.') && !isInRange) {
        msg.append("- ");
        msg.append(s3);
        msg.append("\n");
        msg.append("    have: ");
        msg.append(QString::number(minAvailDate[i]));
        msg.append(" to ");
        msg.append(QString::number(maxAvailDate[i]));
        msg.append("\n\n");
        match += 1;
      }

      if (s4.contains(availNames[i]+'.') && !isInRange) {
        msg.append("- ");
        msg.append(s4);
        msg.append("\n");
        msg.append("    have: ");
        msg.append(QString::number(minAvailDate[i]));
        msg.append(" to ");
        msg.append(QString::number(maxAvailDate[i]));
        msg.append("\n\n");
        match += 1;
      }

      if (s5.contains(availNames[i]+'.') && !isInRange) {
        msg.append("- ");
        msg.append(s5);
        msg.append("\n");
        msg.append("    have: ");
        msg.append(QString::number(minAvailDate[i]));
        msg.append(" to ");
        msg.append(QString::number(maxAvailDate[i]));
        msg.append("\n\n");
        match += 1;
      }
    }
  }

  if (match > 0) {
    QMessageBox msgBox;
    QString warningMsg = "The following variable(s) are not available\nfor the requested starting date.\n\nThe nominal availability is:\n\n";
    msgBox.setText(warningMsg.append(msg));
    msgBox.QDialog::setWindowTitle("RSIG3D warning");
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.exec();
  }
}



void ControlDockWidget::downloadImage() {
  const QString curTrueColor = ui->trueColorComboBox->currentText();
  QString myLayer = "";

  if (curTrueColor.compare("MODIS-AQUA") == 0) {
    myLayer = "MODIS_Aqua_CorrectedReflectance_TrueColor";
  } else if (curTrueColor.compare("MODIS-TERRA") == 0) {
    myLayer = "MODIS_Terra_CorrectedReflectance_TrueColor";
  } else if (curTrueColor.compare("VIIRS") == 0) {
    myLayer = "VIIRS_SNPP_CorrectedReflectance_TrueColor";
  }
  
  if (curTrueColor.compare("None") != 0) {
    const double x0 = ui->westDSpinBox->value();
    const double y0 = ui->southDSpinBox->value();
    const double x1 = ui->eastDSpinBox->value();
    const double y1 = ui->northDSpinBox->value();
    VisOptions* vOptions = VisOptionsSingleton::getInstance();
    vOptions->setBboxMinLon(x0);
    vOptions->setBboxMinLat(y0);
    vOptions->setBboxMaxLon(x1);
    vOptions->setBboxMaxLat(y1);
    const double x0_3857 = lon_to_EPSG3857(x0);
    const double x1_3857 = lon_to_EPSG3857(x1);
    const double y0_3857 = lat_to_EPSG3857(y0);
    const double y1_3857 = lat_to_EPSG3857(y1);
    const int ndays = ui->durationSpinBox->value();
    const QString extent_4326 = QString::number(x0) + ',' + QString::number(y0) + ',' + QString::number(x1) + ',' + QString::number(y1);
    const QString extent_3857 = QString::number(x0_3857, 'f', 6) + ',' + QString::number(y0_3857, 'f', 6) + ',' + QString::number(x1_3857, 'f', 6) + ',' + QString::number(y1_3857, 'f', 6);
    QDir homeDir = QDir(QDir::homePath());
    QThread* thread_4326[ndays];
    QThread* thread_3857[ndays];
    ModisTrueColor* myMTC_4326[ndays];
    ModisTrueColor* myMTC_3857[ndays];
   
    for (int day=0; day<ndays; ++day) {
      const QDate xdate = ui->dateDateEdit->date().addDays(day);
      QString time = QString::number(xdate.year()) + "-" + QString::number(xdate.month()).rightJustified(2, '0') + "-" +  QString::number(xdate.day()).rightJustified(2, '0');
      QString yyyy = QString("%1").arg(xdate.year(),  4, 10, QChar('0'));
      QString mm   = QString("%1").arg(xdate.month(), 2, 10, QChar('0'));
      QString dd   = QString("%1").arg(xdate.day(),   2, 10, QChar('0'));
      QString yyyymmdd = yyyy + mm + dd;
      QString url_4326 = "https://gibs.earthdata.nasa.gov/wms/epsg4326/best/wms.cgi?SERVICE=WMS&REQUEST=GetMap&VERSION=1.1.1&LAYERS=" + myLayer + "&STYLES=&FORMAT=image%2Fpng&TRANSPARENT=true&HEIGHT=1500&WIDTH=3000&SRS=EPSG:4326&TIME=" + time + "&bbox=" + extent_4326;
      QString url_3857 = "https://gibs.earthdata.nasa.gov/wms/epsg3857/best/wms.cgi?SERVICE=WMS&REQUEST=GetMap&VERSION=1.1.1&LAYERS=" + myLayer + "&STYLES=&FORMAT=image%2Fpng&TRANSPARENT=true&HEIGHT=1500&WIDTH=3000&SRS=EPSG:3857&TIME=" + time + "&bbox=" + extent_3857;
      QString filename_4326 = QDir::homePath() + "/.rsig/trueColor_" + curTrueColor + "_" + yyyymmdd +"_4326.png";
      QString filename_3857 = QDir::homePath() + "/.rsig/trueColor_" + curTrueColor + "_" + yyyymmdd +"_3857.png";

      thread_4326[day] = new QThread;
      myMTC_4326[day] = new ModisTrueColor();
      myMTC_4326[day]->setUrlString(url_4326);
      myMTC_4326[day]->setPathedFilename(filename_4326);
      myMTC_4326[day]->moveToThread(thread_4326[day]);
      connect(myMTC_4326[day], SIGNAL (error(QString)), this, SLOT (downloadImageError(QString)));
      connect(myMTC_4326[day], SIGNAL (message(QString)), this, SLOT (downloadImageMessage(QString)));
      connect(myMTC_4326[day], SIGNAL (requestRedraw()), this, SIGNAL (requestRedraw()));
      connect(thread_4326[day], SIGNAL(started()), myMTC_4326[day], SLOT(downloadImage())); // this refers to ModisTrueColor::downloadImage(), not ControlDockWidget::downloadImage() 
      connect(myMTC_4326[day], SIGNAL (finished()), thread_4326[day], SLOT (quit()));
      connect(myMTC_4326[day], SIGNAL (finished()), myMTC_4326[day], SLOT (done()));
      connect(thread_4326[day], SIGNAL (finished()), thread_4326[day], SLOT (deleteLater()));
      postMessage(url_4326, RSIGState::NORMAL_TEXT);
      thread_4326[day]->start();

      thread_3857[day] = new QThread;
      myMTC_3857[day] = new ModisTrueColor();
      myMTC_3857[day]->setUrlString(url_3857);
      myMTC_3857[day]->setPathedFilename(filename_3857);
      myMTC_3857[day]->moveToThread(thread_3857[day]);
      connect(myMTC_3857[day], SIGNAL (error(QString)), this, SLOT (downloadImageError(QString)));
      connect(myMTC_3857[day], SIGNAL (message(QString)), this, SLOT (downloadImageMessage(QString)));
      connect(myMTC_3857[day], SIGNAL (requestRedraw()), this, SIGNAL (requestRedraw()));
      connect(thread_3857[day], SIGNAL(started()), myMTC_3857[day], SLOT(downloadImage())); // ModisTrueColor::downloadImage(), not this one
      connect(myMTC_3857[day], SIGNAL (finished()), thread_3857[day], SLOT (quit()));
      connect(myMTC_3857[day], SIGNAL (finished()), myMTC_3857[day], SLOT (done()));
      connect(thread_3857[day], SIGNAL (finished()), thread_3857[day], SLOT (deleteLater()));
      postMessage(url_3857, RSIGState::NORMAL_TEXT);
      thread_3857[day]->start();
    }

    ui->vizVarCombobox->addItem("True Color imagery");
    ui->vizVarCombobox->setCurrentIndex( ui->vizVarCombobox->count()-1);
    ui->vizCheckBox->setChecked(true);
  }
}



void ControlDockWidget::downloadImageError(QString errorString) {
  postMessage(errorString, RSIGState::ERROR_TEXT);
}



void ControlDockWidget::downloadImageMessage(QString messageString) {
  postMessage(messageString, RSIGState::NORMAL_TEXT);
}



int ControlDockWidget::getAggregateIndex(QString text) {
  int return_val = -1;

  if      (text == "None")           { return_val = emvl::QueryParameters::AGGREGATE_NONE; }
  else if (text == "UTC daily mean") { return_val = emvl::QueryParameters::AGGREGATE_DAILY_MEAN; }
  else if (text == "UTC daily max")  { return_val = emvl::QueryParameters::AGGREGATE_DAILY_MAX; }

  if (return_val == -1) {
    qDebug() << "aggregate option is invalid: " << text;
  }

  return return_val;
}



QStringList ControlDockWidget::getOutage() {

  QStringList melist;
  
  int nHours = 24;
  QString command = "curl -k --silent --retry 0 -L --tcp-nodelay --max-time 120 "; // Timeout last
  QString myurl = "https://ofmpub.epa.gov/rsig/rsigserver?SERVICE=OUTAGE&NHOURS=" + QString::number(nHours);

  
  
#ifdef _WIN32
  const char quote = '"';
  const char* const mode = "rb";
#else
  const char quote = '\'';
  const char* const mode = "r";
#endif
  
  command.append(quote).append(myurl).append(quote);
  qDebug() << command;
  const std::string popenCommand( command.toStdString() );
  FILE* stream = popen( popenCommand.c_str(), mode );

  // buffered read/write
  enum { BUFSIZE = 1024 * 1024 };
  std::vector<char> buffer( BUFSIZE ); //x
  char* const buf = &buffer[ 0 ];
  //size_t totBytes = 0;

  QString outages = "";
  while ( ! feof( stream ) ) {
    const size_t thisBytes = fread( buf, 1, BUFSIZE, stream );

    if ( thisBytes ) {
      outages.append(buf);
    }
  }

  melist = outages.split("\n");

  return melist;
  
}




//=============================================================================
//=========================== IMPORTANT Functions ============================
//=============================================================================

//============================================================================
// Callback to begin the data INPUT process.
//============================================================================


void ControlDockWidget::onCheckServersClicked() {

  QStringList outageList = this->getOutage();

  QStringList sourcesToExclude;
  sourcesToExclude << "aquarius" << "buoy" << "dv" << "erddap" << "landuse" << "nerrs" << "nldas" << "ofs" << "smap" << "tidal" << "wqp";
  
  QString myMessage = "Data servers that have been offline during the last 24 hours.<br><br>";
  for (int listInd=0; listInd<outageList.size(); listInd++) {
    QString thisOutageString = outageList.at(listInd);
    QStringList thisOutageStringSplit = thisOutageString.split(',');
    if (thisOutageStringSplit.size() > 1) {

      // get the timestamp
      myMessage.append(thisOutageStringSplit[0] + ": ");

      // exclude data sources not available in RSIG3D
      int nMatches = 0;
      for (int sInd=1; sInd<thisOutageStringSplit.size(); sInd++) {
        // get just the source from the outage string
        QString thisSource = thisOutageStringSplit[sInd].split(" ")[0];
        qDebug() << "source found in outage string:" << thisSource;
        
        if (sourcesToExclude.filter(thisSource).size() == 0) {
          nMatches += 1;
          myMessage.append("<b>" + thisSource + "</b>, ");
        }
      }

      // finished processing all sources in the outage string
      if (nMatches == 0) {
        myMessage.append("ok<br>");
      } else {
        
        if (myMessage.endsWith(", ")) {
          // non-trivial myMessage with timestamp and sources
          myMessage.chop(2); // remove trailing comma
          myMessage.append("<br>");
        } else {
          // myMessage only contains a timestamp and no sources, so remove it
          //myMessage.chop(22);
        }
      }
    }
  }

    //myMessage.replace(",", ", ");
    
  QMessageBox msgBox;
  msgBox.setText(myMessage);
  msgBox.QDialog::setWindowTitle("RSIG3D information");
  msgBox.setTextFormat(Qt::RichText);
  msgBox.setIcon(QMessageBox::Information);
  msgBox.setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::WindowTitleHint);
  msgBox.exec();
}






/******************************************************************************
PURPOSE: onGetdataClicked - Callback to begin the data input process of
         retrieving datasets from either the webservice (asynchronus) or
         CMAQFileReaderDialog (synchronus/blocking).
******************************************************************************/

void ControlDockWidget::onGetdataClicked() {

  // get the list of recent data outages
  QStringList outageList = this->getOutage();

  
  this->readDataPrefs(); // data preferences 
      
  // sources

  QString sd1 = ui->selectedData1->toPlainText();
  QString sd2 = ui->selectedData2->toPlainText();
  QString sd3 = ui->selectedData3->toPlainText();
  QString sd4 = ui->selectedData4->toPlainText();
  QString sd5 = ui->selectedData5->toPlainText();

  if ( sd1.isEmpty() && sd2.isEmpty() && sd3.isEmpty() && sd4.isEmpty() &&
       sd5.isEmpty() ) {
    return; // No data sources are selected so just return.
  }

  // check to see if any of the sources have recently been offline
  if (!sd1.isEmpty() && outageList.filter(sd1.split('.')[0]).size() > 0) {
    QString msg = "The " + sd1.split('.')[0] + " server has been offline " + QString::number(outageList.filter(sd1.split('.')[0]).size()) + " of the last 24 hours.";
    postMessage(msg, RSIGState::WARNING_TEXT);
  }
  if (!sd2.isEmpty() && outageList.filter(sd2.split('.')[0]).size() > 0) {
    QString msg = "The " + sd2.split('.')[0] + " server has been offline " + QString::number(outageList.filter(sd2.split('.')[0]).size()) + " of the last 24 hours.";
    postMessage(msg, RSIGState::WARNING_TEXT);
  }

 if (!sd3.isEmpty() && outageList.filter(sd3.split('.')[0]).size() > 0) {
   QString msg = "The " + sd3.split('.')[0] + " server has been offline " + QString::number(outageList.filter(sd3.split('.')[0]).size()) + " of the last 24 hours.";
    postMessage(msg, RSIGState::WARNING_TEXT);
  }

 if (!sd4.isEmpty() && outageList.filter(sd4.split('.')[0]).size() > 0) {
   QString msg = "The " + sd4.split('.')[0] + " server has been offline " + QString::number(outageList.filter(sd4.split('.')[0]).size()) + " of the last 24 hours.";
    postMessage(msg, RSIGState::WARNING_TEXT);
  }

 if (!sd5.isEmpty() && outageList.filter(sd5.split('.')[0]).size() > 0) {
   QString msg = "The " + sd5.split('.')[0] + " server has been offline " + QString::number(outageList.filter(sd5.split('.')[0]).size()) + " of the last 24 hours.";
    postMessage(msg, RSIGState::WARNING_TEXT);
  }
  

  
  // remember the selected data in the view menu to preserve continuity
  oldSelectedDatasource = ui->vizVarCombobox->currentText();

  
  // Disable GetData button to avoid further processing until this request is done.
  // Reenable once all data has been loaded.
  ui->getdataPushButton->setEnabled(false);

  postMessage(QString("------"), RSIGState::NORMAL_TEXT);
  VisOptions* vOptions = VisOptionsSingleton::getInstance();

  // reset data source list
  QStringList melist = vOptions->getDataSourceList();
  melist.clear();
  vOptions->setDataSourceList(melist);

  //vOptions->setPersistBEHR(false);
  //vOptions->setPersistMODIS(false);
  //vOptions->setPersistVIIRS(false);
  //vOptions->setPersistTROPOMI(false);
  //vOptions->setPersistTEMPO(false);
  //vOptions->setPersistHMS(false);

  vOptions->setCurrentDayNum(-1); // will be reset in RSIGScene after MODIS True Color image is set
  //ui->persistBEHR->setCheckState(Qt::Unchecked);
  //ui->persistMODIS->setCheckState(Qt::Unchecked);
  //ui->persistVIIRS->setCheckState(Qt::Unchecked);
  //ui->persistTROPOMI->setCheckState(Qt::Unchecked);
  //ui->persistTEMPO->setCheckState(Qt::Unchecked);
  //ui->persistHMS->setCheckState(Qt::Unchecked);

  ui->panelDisplaySource1->clear();
  ui->panelDisplaySource2->clear();
  ui->panelDisplayListWidget->clear();
  setPanelDisplaySource1Selection("");
  setPanelDisplaySource2Selection("");
  
  
  VisDataList* vislist = VisDataListSingleton::getInstance();
  int arraySize = sizeof(oldVisProperties)/sizeof(oldVisProperties[0]);

  for (int ind=0; ind<arraySize; ind++) {
    // get properties of existing vis objects
    QString thisName = vislist->getName(ind);
    VisDataObject* thisVisObject = vislist->getObjectByName(thisName);

    // set properties in order to maintain apparent object coherence once new data is loaded
    if (thisVisObject) {

      if (thisName.contains(".point") || thisName.contains(".polygon") || thisName.contains(".vector")) {
        oldVisProperties[ind].setName(thisName);
        oldVisProperties[ind].setShowObject(thisVisObject->showObject());
        oldVisProperties[ind].setMinval(thisVisObject->minval());
        oldVisProperties[ind].setMaxval(thisVisObject->maxval());
        oldVisProperties[ind].setLastMinval(thisVisObject->lastMinval());
        oldVisProperties[ind].setLastMaxval(thisVisObject->lastMaxval());
        oldVisProperties[ind].setCurColormapIndex(thisVisObject->curColormapIndex());
        oldVisProperties[ind].setDefaultRangeChecked(thisVisObject->isDefaultRangeChecked());
        oldVisProperties[ind].setAlpha(thisVisObject->alpha());
        oldVisProperties[ind].setShowCellBoundary(thisVisObject->showCellBoundary());
        oldVisProperties[ind].setShowThreshold(thisVisObject->ShowThreshold());
        oldVisProperties[ind].setThresholdSliderValue(thisVisObject->thresholdSliderValue());
        oldVisProperties[ind].setMinValChangedThisSession(thisVisObject->minValChangedThisSession());
        oldVisProperties[ind].setMaxValChangedThisSession(thisVisObject->maxValChangedThisSession());
      }
    } else {
      oldVisProperties[ind].setName("null");
    }
  }

  // UGLY. Must remove datasets _after_ the above code to avoid stale
  // GeoDataObject->model() pointers to deleted datasets!

  deleteAllDataObjects();

  //------------------------ Begin loading data objects: ---------------------

  // MODIS True Color imagery
  const QString curTrueColor = ui->trueColorComboBox->currentText();
  // default is that truecolor imagery is off
  vOptions->setDrawModisTrueColor(false);
  vOptions->setCurrentDayNum(-1); // forces reload of png
  vOptions->setTrueColorType(curTrueColor);

  if (curTrueColor.compare("None") != 0) {
    downloadImage();
    vOptions->setDrawModisTrueColor(true);
    vOptions->setCurrentDayNum(-1); // forces reload of png
    enableModisTrueColorOptions(false);
  }

  int regrid1 = ui->regridCheckbox1->checkState();
  int regrid2 = ui->regridCheckbox2->checkState();
  int regrid3 = ui->regridCheckbox3->checkState();
  int regrid4 = ui->regridCheckbox4->checkState();
  int regrid5 = ui->regridCheckbox5->checkState();

  int layer1 = ui->selectedLayer1->value();
  int layer2 = ui->selectedLayer2->value();
  int layer3 = ui->selectedLayer3->value();
  int layer4 = ui->selectedLayer4->value();
  int layer5 = ui->selectedLayer5->value();

  int agg1 = 0; //default
  int agg2 = 0;
  int agg3 = 0;
  int agg4 = 0;
  int agg5 = 0;

  if (!ui->aggregate1->isHidden()) { agg1 = getAggregateIndex(ui->aggregate1->currentText()); } // NOTE: Use !isHidden() instead of isVisible()
  if (!ui->aggregate2->isHidden()) { agg2 = getAggregateIndex(ui->aggregate2->currentText()); } //       as widget is not visible if we are on a different tab.
  if (!ui->aggregate3->isHidden()) { agg3 = getAggregateIndex(ui->aggregate3->currentText()); }
  if (!ui->aggregate4->isHidden()) { agg4 = getAggregateIndex(ui->aggregate4->currentText()); }
  if (!ui->aggregate5->isHidden()) { agg5 = getAggregateIndex(ui->aggregate5->currentText()); }

  // modify source if it is a compare variable
  sd1 = swap_compare_operator(sd1);
  sd2 = swap_compare_operator(sd2);
  sd3 = swap_compare_operator(sd3);
  sd4 = swap_compare_operator(sd4);
  sd5 = swap_compare_operator(sd5);
  
  // keep track of how many ceilometer variables there are
  int nCeilVars = 4;
  RSIGScene& rsigScene = RSIGScene::getInstance();
  rsigScene.numCeilometerVars = 0;
  rsigScene.ceilometerCCACreated = false;
  rsigScene.ceilometerShowRegridWarning = true;
  rsigScene.neubrewShowRegridWarning = true;
  rsigScene.ceilometerCurtainComputed.clear();
  rsigScene.ceilometerCurtainComputed.resize(nCeilVars);

  for (int ceilInd=0; ceilInd<nCeilVars; ceilInd++) {
    //qDebug() << ceilInd;
    rsigScene.ceilometerCurtainComputed.push_back(false);
  }

  const QDate xdate = ui->dateDateEdit->date();
  check_sources(sd1, sd2, sd3, sd4, sd5, xdate);
  vOptions->setSaveMetadataFlag(false);
  vOptions->setSaveFileFormatIndex(emvl::QueryParameters::XDR);
  const int selectedSourceCount =
    ! sd1.isEmpty() + ! sd2.isEmpty() + ! sd3.isEmpty() + ! sd4.isEmpty() +
    ! sd5.isEmpty();
  vOptions->setDownloadsOutstanding( selectedSourceCount );

  const QString sd[] = { sd1, sd2, sd3, sd4, sd5 };
  const size_t count = sizeof sd / sizeof sd[ 0 ];
  const int layer[] = { layer1, layer2, layer3, layer4, layer5 };
  const int aggregate[] = { agg1, agg2, agg3, agg4, agg5 };
  const int regrid[] = { regrid1, regrid2, regrid3, regrid4, regrid5 };

  // Load datasets for each selected source:

  for ( size_t index = 0, dx = 20, dy = 20; index < count;
        ++index, dx += dx, dy += dy ) {

    // for pandora data, only get hourly averaged data
    // NOTE: Regular pandora and hourly pandora seem to have a race condition that causes intermittent crashes.
    //       This issue is unresolved. For now, only get hourly pandora.
    //       If we ever get both again, make changes in CeilometerCurtainArray.cpp too.
    if (sd[index].startsWith("pandora") && !sd[index].startsWith("pandora.id")) {
      const int myAggregate = emvl::QueryParameters::AGGREGATE_HOURLY;
      checkAndStartLoadingDataset( sd[ index ], layer[ index ], myAggregate, regrid[ index ], dx, dy );
    } else {
      checkAndStartLoadingDataset( sd[ index ], layer[ index ], aggregate[ index ], regrid[ index ], dx, dy );
    }
    
    
  }

  Q_EMIT requestRedraw();
}



//====================== Helper routines to INPUT data ========================



void ControlDockWidget::deleteAllDataObjects() {

  deleteModisTrueColorImages();

  m_objectPropEdit->clear(); // Clear the GeoDataPropertyEdit list.

  // Remove old items from the visualization combo box:

  int numItems = ui->vizVarCombobox->count();

  while ( numItems-- ) {
    // Always remove item 0, because the combobox shrinks on each call.
    ui->vizVarCombobox->removeItem(0);
  }

  GeoDataList* const geoDataList = GeoDataListSingleton::getInstance();
  geoDataList->clear();
    
  emvl::DatasetManager* const datasetManager = getDatasetManager();
  datasetManager->removeAll(); // Delete all loaded datasets.
}



// Start loading a dataset either via webservice or CMAQFileReaderDialog:

void ControlDockWidget::checkAndStartLoadingDataset( const QString& sd,
                                                     const int layer,
                                                     const int aggregate,
                                                     const int regrid,
                                                     const int dx,
                                                     const int dy ) {

  if ( ! sd.isEmpty() ) {

    if ( sd.startsWith( "ceilometer" ) ) {
      RSIGScene& rsigScene = RSIGScene::getInstance();
      rsigScene.numCeilometerVars += 1;
    }

    if ( sd.startsWith( "cmaq.read.file" ) ) {
      useCMAQFileReaderDialog( sd, layer, aggregate ); // Synchronus/blocking.
    } else { // run_downloader spawns an asynchronus thread:
      run_downloader( sd, regrid, layer, aggregate, dx, dy, false, false );
    }
  }
}



/*
 * If a selected source name starts with "cmaq.read.file" then read from a
 * local or remote CMAQ file.
 * This is handled by showing a CMAQFileReaderDialog which lets the user
 * browse the local or remote host for files and then click the Read Variable
 * button (or Cancel button).
 * The pop-up dialog is modal so it blocks until the user closes it.
 * If successful, the local subset file name is obtained from the
 * CMAQFileReaderModel.
 */

void ControlDockWidget::useCMAQFileReaderDialog( const QString& source,
                                                 const int layer,
                                                 const int aggregate ) {

  // UGLY. Should RSIGState and RSIGScene be merged into one Model?

  const RSIGState* const state = RSIGStateSingleton::getInstance();
  const emvl::Timestamp startingTimestamp( state->getStartingTimestamp() );
  const emvl::Timestamp endingTimestamp( state->getEndingTimestamp() );

  // UGLY: Get BBOX state from GUI.
  // All program state should be in the single composite Model!

  const double west  = ui->westDSpinBox->value();
  const double east  = ui->eastDSpinBox->value();
  const double south = ui->southDSpinBox->value();
  const double north = ui->northDSpinBox->value();
  const emvl::Bounds bounds( west, east, south, north );

  // Reset the model to current subset parameters before showing the dialog:

  CMAQFileReaderModel& cmaqFileReaderModel =
    RSIGScene::getInstance().cmaqFileReaderModel;

  cmaqFileReaderModel.setSubset( bounds, startingTimestamp, endingTimestamp,
                                 layer, aggregate );

  cmaqFileReaderModel.clearOutputFile();

  // Display dialog window which is modal so it blocks until read or cancel:

  /*
   * Note there is only one instance of CMAQFileReaderModel and each time a
   * single instance of CMAQFileReaderDialog is created here,
   * it is given that instance to show/update.
   */

  {
    CMAQFileReaderDialog cmaqFileReaderDialog(
      RSIGScene::getInstance().cmaqFileReaderModel );
    cmaqFileReaderDialog.exec();
  } // cmaqFileReaderDialog destructor called here.
  
  // At this point, the user has either cancelled or attempted to read.
  // Check model outputFileName for successful read result.

  const std::string& outputFileName = cmaqFileReaderModel.getOutputFile();
  GeoDataObject* geoDataObject = 0;

  if ( ! outputFileName.empty() ) { // Non-empty implies successful read.
    emvl::DatasetManager* const datasetManager = getDatasetManager();
    emvl::Dataset* const dataset =
      datasetManager->readDataset( outputFileName ); //x

    /*
     * UGLY HACK must edit name to show source from menu, e.g., cmaq.read.file1_NO2
     * so save/restore state file works in ControlDockWidget::domElement().
     * TODO: Replace that messy dom/xml approach with a simple text file!
     */

    std::string name( dataset->name() );

    if ( name.length() > 5 && name.find( "CMAQ_" ) == 0 ) { // name = "CMAQ_NO2"
      QString source_( source );     // source is "cmaq.read.file1".
      source_.replace( '.', '_' );   // source_ is "cmaq_read_file1".
      name.replace( 0, 4, source_.toStdString() ); // name is "cmaq_read_file1_NO2".
      dataset->name( name );
    }

    datasetManager->append( dataset ); // Transfer ownership to datasetManager.

    // Wrap the Dataset inside a GeoDataObject:

    QRectF boundingBox;
    boundingBox.setLeft( west );
    boundingBox.setRight( east );
    boundingBox.setBottom( south );
    boundingBox.setTop( north );
    geoDataObject = new GeoDataObject( 0, dataset, &boundingBox );
  }

  onDownloadWorkFinished( geoDataObject ); // Re-enables Get Data button.
}



//============================================================================
// Callback to begin the data OUTPUT process.
//============================================================================

/******************************************************************************
PURPOSE: onSaveFileClicked - Write each dataset (or images) to the save
         directory in the given file format.
NOTES:  This requires more webservice downloading if the selected output format
        is ORIGINAL (SLOW) and/or save metadata is selected.
******************************************************************************/

void ControlDockWidget::onSaveFileClicked() {

  // Create a type and table to hold info about each radio button:

  typedef struct {
    QRadioButton* const radioButton; // Radio button that may be selected.
    const int datasetOutputFormat;   // emvl::QueryParameters::XDR, etc. or -1.
  } Entry;

  const Entry table[] = {

    // Radio buttons under 'Data file formats':

    { ui->scenarioRadioButton,     -1 },
    { ui->binaryxdrRadioButton,    emvl::QueryParameters::XDR },
    { ui->asciisheetRadioButton,   emvl::QueryParameters::ASCII },
    { ui->netcdfcoardsRadioButton, emvl::QueryParameters::NETCDF_COARDS },
    { ui->netcdfioapiRadioButton,  emvl::QueryParameters::NETCDF_IOAPI },
    { ui->geotiffRadioButton,      -1 },
    { ui->shapeRadioButton,        emvl::QueryParameters::SHAPEFILE },
    { ui->originalFileRadioButton, emvl::QueryParameters::ORIGINAL },

    // Radio buttons under 'Visualization file formats':

    { ui->kmlRadioButton,          emvl::QueryParameters::KML },
    { ui->mpegRadioButton,         -1 },
    { ui->screenRecordRadioButton, -1 },
    { ui->geotiffrgbRadioButton,   -1 },
    { ui->currentpngRadioButton,   -1 },
    { ui->highrespngRadioButton,   -1 },
    { ui->allpngsRadioButton,      -1 },

    { 0, -1 } // End of table.
  };
  const size_t count = sizeof table / sizeof *table - 1;

  // Find the single selected radio button:

  QRadioButton* button = 0;
  int datasetOutputFormat = -1;

  for ( size_t index = 0; index < count; ++index ) {
    const Entry* const entry = table + index;

    if ( entry->radioButton->isChecked() ) {
      button = entry->radioButton;
      datasetOutputFormat = entry->datasetOutputFormat;
      index = count;
    }
  }

  if ( button ) {

    // Set saving path:

    QString defaultPath = ui->dirnameLineEdit->text();

    if ( defaultPath.isEmpty() || defaultPath.isNull() ) {
      defaultPath = QDir::homePath();
    }

    if ( defaultPath[defaultPath.size() - 1 ] != '/' ) {
      defaultPath.append( '/' );
    }

    VisOptions* const vOptions = VisOptionsSingleton::getInstance();

    // Save either data or visualization/image format:

    const bool saveMetadata = ui->saveMetadataCheckBox->isChecked();
    vOptions->setSaveMetadataFlag(saveMetadata);
    vOptions->setSaveFileFormatIndex(datasetOutputFormat);
    vOptions->setSaveDir( defaultPath );
    QString saveDirQstr( defaultPath );
    std::string saveDir( saveDirQstr.toStdString() );

    if ( (datasetOutputFormat == emvl::QueryParameters::ORIGINAL ||
          saveMetadata) && button != ui->scenarioRadioButton) {

      // sources

      QString sd1 = ui->selectedData1->toPlainText();
      QString sd2 = ui->selectedData2->toPlainText();
      QString sd3 = ui->selectedData3->toPlainText();
      QString sd4 = ui->selectedData4->toPlainText();
      QString sd5 = ui->selectedData5->toPlainText();

      // modify source if it is a compare variable

      sd1 = swap_compare_operator(sd1);
      sd2 = swap_compare_operator(sd2);
      sd3 = swap_compare_operator(sd3);
      sd4 = swap_compare_operator(sd4);
      sd5 = swap_compare_operator(sd5);

      const QDate xdate = ui->dateDateEdit->date();
      check_sources(sd1, sd2, sd3, sd4, sd5, xdate);

      // Retrieve and save original files and/or metadata:

      const bool saveOriginal =
        datasetOutputFormat == emvl::QueryParameters::ORIGINAL;
      const QString sd[] = { sd1, sd2, sd3, sd4, sd5 };
      const size_t count = sizeof sd / sizeof sd[ 0 ];

      for ( size_t index = 0, dx = 20, dy = 20; index < count;
        ++index, dx += dx, dy += dy ) {

        // Check for and save data as original files and/or metadata:

        checkAndStartSavingOriginalDataset( sd[ index ], dx, dy, saveDir,
                                            saveOriginal, saveMetadata );
      }
    }

    // Save datasets (not original files) in formats (NETCDF_COARDS, KML,...):

    saveDatasets( datasetOutputFormat, saveDir );

    // Save vis formats:

    QString origSaveDir     = vOptions->getSaveDir();
    QString origFilenamePng = vOptions->getFilenamePng();
    QString basenameTemp    = "RSIG-Snapshot";
    const QString directoryNameTemp = QDir::homePath() + "/.rsig/";

    if (button == ui->mpegRadioButton) {

      /*
       * hack the save directory and filename to redirect temporary files
       * corresponding to image frames.
       */

      vOptions->setSaveDir(directoryNameTemp);
      vOptions->setFilenamePng(basenameTemp);
      saveDirQstr = directoryNameTemp;
      saveDir = saveDirQstr.toStdString();
    } else {
      vOptions->setSaveDir( defaultPath );
      saveDirQstr = defaultPath;
      saveDir = saveDirQstr.toStdString();
    }

    if ( button == ui->scenarioRadioButton ) {
      saveDirQstr = defaultPath;
      MainWin& mainWin = MainWin::getInstance();
      QString filename = ui->filenameLineEdit->text();
      QString id = QFileInfo( filename ).baseName();
      mainWin.saveStateToFile( saveDirQstr, filename, id );
    } else if ( button == ui->currentpngRadioButton ) {
      QDir dir( defaultPath );

      if ( ! dir.exists() ) {
        const QString message = "Directory " + dir.absolutePath() + " created.";
        this->postMessage( message, RSIGState::WARNING_TEXT );
        dir.mkdir( dir.absolutePath() );
      }

      QString filename = ui->filenameLineEdit->text();
      QString pathedFilename = defaultPath + filename;
      QImage glscreen;
      MainWin& mainWin = MainWin::getInstance();
      
      
      GraphicsView* m_pGLUIWin = mainWin.graphicsView();

      if ( m_pGLUIWin != NULL ) {
        m_pGLUIWin->update();

        if ( m_pGLUIWin->getQGLlWidget()->doubleBuffer() ) {
          //m_pGLUIWin->getQGLlWidget()->swapBuffers();
        }

        m_pGLUIWin->getQGLlWidget()->updateGL();
        glscreen = m_pGLUIWin->getQGLlWidget()->grabFrameBuffer( false );
        // hi res attempt
        //glscreen= m_pGLUIWin->getQGLlWidget()->renderPixmap( 1400, 1400 ).toImage();
      }

      
      const std::string copy( pathedFilename.toStdString() );
      printf( "Saving %s\n", copy.c_str() );
      this->postMessage( "Saving " + pathedFilename, RSIGState::NORMAL_TEXT );
      const bool ok = glscreen.save( pathedFilename, "PNG", 99 );

      if ( ok ) {
        Q_EMIT requestRedraw();
      }
    } else if ( button == ui->mpegRadioButton ||
                button == ui->allpngsRadioButton ) {
      QString infoMsg;
      if (button == ui->mpegRadioButton) {
        infoMsg = "Creating image frames and saving MPEG.\n\n For best results, do not alter the view. This message will automatically close when the MPEG has been created.";
      } else if (button == ui->allpngsRadioButton) {
        infoMsg = "Saving images.\n\n For best results, do not alter the view. This message will automatically close when all images have been saved.";
      }

      QMessageBox msgBox;
      msgBox.setText(infoMsg);
      msgBox.QDialog::setWindowTitle("RSIG3D information");
      msgBox.setIcon(QMessageBox::Information);
      msgBox.setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::WindowTitleHint);
      msgBox.setEnabled( false );
      msgBox.setModal( false );
      msgBox.open();

      QString imagesPath;

      if (button == ui->mpegRadioButton) {
        imagesPath = QDir::homePath() + "/.rsig/";
        // delete existing temporary files
        QDir tempdir(imagesPath, basenameTemp + "*.png");

        for (int ind=0; ind<tempdir.entryList().length(); ind++) {
          tempdir.remove(tempdir.entryList()[ind]);
        }
      } else {
        imagesPath = defaultPath;
      }

      Q_EMIT requestInitSnapshot(imagesPath);
      int timesteps = 1; // default
      const GeoDataList* const geoDataList = GeoDataListSingleton::getInstance();
      const size_t datasetCount = geoDataList->size();

      if ( datasetCount > 0 ) {
        timesteps = geoDataList->getObject(0)->model()->timesteps();
      }

      const int origTimestep = RSIGStateSingleton::getInstance()->mCounter;
      RSIGStateSingleton::getInstance()->mCounter = 0;
      Q_EMIT requestRedraw();
      qApp->processEvents();

      for ( int timestep = 0; timestep < timesteps; timestep++ ) {
        RSIGStateSingleton::getInstance()->mCounter = timestep;
        Q_EMIT requestRedraw();
        qApp->processEvents();
        glFlush();
        usleep(500000);

        // Qt BUG HACK workaround:
        // redraw because sometimes grabFrameBuffer() get a previous image!
        Q_EMIT requestRedraw();
        qApp->processEvents();
        glFlush();
        usleep(500000);
        Q_EMIT requestSnapshot();
      }

      RSIGStateSingleton::getInstance()->mCounter = origTimestep;
      Q_EMIT requestRedraw();
      qApp->processEvents();
      this->postMessage( "Saving images done.", RSIGState::WARNING_TEXT );

      if ( button == ui->mpegRadioButton ) {
        this->onSaveVideoToggled( false );
        // unhack the save directory and filename to redirect temporary files corresponding to image frames
        vOptions->setSaveDir(origSaveDir);
        vOptions->setFilenamePng(origFilenamePng);
      }

      msgBox.close();
    } else if (button == ui->screenRecordRadioButton) {

      if (ui->savefilePushButton->text().compare("Save") == 0) {
        onSaveVideoToggled(true); // save
      } else {
        onSaveVideoToggled(false); // stop recording
        ui->savefilePushButton->setText("Save");
      }
    }
  }
}



//====================== Helper routines to OUTPUT data =======================



// Check for and start saving dataset as original format and/or metadata:

void ControlDockWidget::checkAndStartSavingOriginalDataset(
  const QString& sd,
  const int dx,
  const int dy,
  const std::string& directory,
  const bool asOriginal,
  const bool andMetadata ) {

  if ( ! sd.isEmpty() ) {

    if ( sd.startsWith( "cmaq.read.file" ) ) { // Not from webservice (no metadata):

      if ( asOriginal ) {
        saveAsOriginalCMAQFile( sd, directory ); // Synchronus/blocking/fast.
      }
    } else { // Download from webservice:

      if ( asOriginal ) { // run_downloader spawns an asynchronus thread:
        run_downloader( sd, 0, 0, 0, dx, dy, true, false ); // Original file SLOW
      }

      if ( andMetadata ) { // Spawn a thread to retrieve metadata file.
        run_downloader( sd, 0, 0, 0, dx + dx, dy + dy, asOriginal, true);
      }
    }
  }
}



// Save non-webservice CMAQDataset as CMAQ format to directory:

void ControlDockWidget::saveAsOriginalCMAQFile( const QString& source,
                                        const std::string& directory ) const {
  QString source_( source );
  source_.replace( '.', '_' );
  const int index = getIndexOfGeoDataObjectForSource( source_ );

  if ( index > -1 ) {
    const GeoDataList* const geoDataList = GeoDataListSingleton::getInstance();
    assert( geoDataList );
    const GeoDataObject* const geoDataObject = geoDataList->getObject( index );
    assert( geoDataObject );
    const emvl::Dataset* const dataset = geoDataObject->model();
    assert( dataset );
    dataset->write( directory, emvl::QueryParameters::NETCDF_IOAPI );
  }
}



// Save datasets (non-original files) in formats (NETCDF_COARDS, KML,...).
// Note: cannot simply use emvl::DatasetManager.writeAll() since KML format
// requires bounds and colormaps.

void ControlDockWidget::saveDatasets( const int datasetOutputFormat,
                                      const std::string& directory ) {
  const GeoDataList* const geoDataList = GeoDataListSingleton::getInstance();
  const size_t datasetCount = geoDataList->size();

  if ( datasetCount && datasetOutputFormat != -1 &&
       datasetOutputFormat != emvl::QueryParameters::ORIGINAL ) {
    std::vector<emvl::Dataset*> datasets( datasetCount ); //x
    std::vector<int> colormaps( datasetCount ); //x index of colormap for KML.

    for ( size_t dataset = 0; dataset < datasetCount; ++dataset ) {
      GeoDataObject* const geoDataObject =
        geoDataList->getObject( dataset );
      datasets[ dataset ] = geoDataObject->model();
      colormaps[ dataset ] = geoDataObject->getColormapIndex();
    }

    // Get bounds for KML format:
    // UGLY. Should get state from the Model instead of the GUI!

    const double west  = ui->westDSpinBox->value();
    const double east  = ui->eastDSpinBox->value();
    const double south = ui->southDSpinBox->value();
    const double north = ui->northDSpinBox->value();
    const Bounds bounds = { { west, east }, { south, north } };
    const bool ok = saveData( &datasets[0], datasets.size(),
                              &colormaps[0], bounds,
                              directory.c_str(), datasetOutputFormat );

    if ( ok ) {
      this->postMessage( "Output finished.", RSIGState::WARNING_TEXT );
    } else {
      this->postMessage( "Output Failed!", RSIGState::ERROR_TEXT );
    }
  }
}


//=============================================================================
//======================== End of IMPORTANT Functions =========================
//=============================================================================



void::ControlDockWidget::onTabWidgetIndexChanged(int index) {
  // Save tab

  if (index == 4 && ui->currentpngRadioButton->isChecked() ) {
    ui->filenameLineEdit->setEnabled(false);
    VisOptions* vOptions = VisOptionsSingleton::getInstance();
    QString curFilename = ui->filenameLineEdit->text();

    if (curFilename == "" && vOptions->getFilenamePngDefault() == "") {
      generateDefaultPngFilename();
      ui->filenameLineEdit->setText(vOptions->getFilenamePngDefault());
    }
  }
}



void ControlDockWidget::pack_selectedData() {
  // shuffle selected data boxes so that all blank boxes are at the bottom
  //qDebug() << "in pack_selectedData";
  // find first empty position, starting from top
  int first_empty_position = -1;
  if (ui->selectedData1->toPlainText() == "") { first_empty_position = 1; } else
  if (ui->selectedData2->toPlainText() == "") { first_empty_position = 2; } else
  if (ui->selectedData3->toPlainText() == "") { first_empty_position = 3; } else
  if (ui->selectedData4->toPlainText() == "") { first_empty_position = 4; } else
  if (ui->selectedData5->toPlainText() == "") { first_empty_position = 5; }

  if (first_empty_position > num_selected_sources) {
    // do nothing, don't need to shuffle
  } else {
    // pop the next one up and then recurse
    if (first_empty_position == 1) {ui->selectedData1->setPlainText(ui->selectedData2->toPlainText()); ui->selectedData2->setPlainText(""); }
    if (first_empty_position == 2) {ui->selectedData2->setPlainText(ui->selectedData3->toPlainText()); ui->selectedData3->setPlainText(""); }
    if (first_empty_position == 3) {ui->selectedData3->setPlainText(ui->selectedData4->toPlainText()); ui->selectedData4->setPlainText(""); }
    if (first_empty_position == 4) {ui->selectedData4->setPlainText(ui->selectedData5->toPlainText()); ui->selectedData5->setPlainText(""); }
    pack_selectedData();
  }
}


// Get coverage string from leaf variable name.
// E.g., input item = no2
// result = regridded.conus.monthly.tropomi.offl.no2

static QString getCoverage( const QTreeWidgetItem* item ) {
  assert( item );
  QString result( item->text( 0 ) );

  for ( item = item->parent(); item && item->parent(); item = item->parent() ) {
    result.prepend( item->text( 0 ) + '.' );
  }

  if ( item && item->text( 0 ) == "Compare" ) {
    result.prepend( "compare." );
  }

  return result;
}



// Determine some properties of the coverage to enable/disable options:
// Aggregate Menu, Layer typein and Regrid checkbox.

static void getCoverageProperties( const QString coverage,
                                   bool& canAggregate,
                                   bool& isMultiLayered,
                                   bool& forceSingleLayer,
                                   bool& noRegrid ) {

  // Check if coverage is one of the coverages that support optional
  // aggregation (e.g., daily_max8) and if so, enable Aggregate menu:

  canAggregate =
    OR3( coverage.startsWith( "aqs." ),
         coverage.startsWith( "cmaq." ),
         coverage.startsWith( "omibehrioapi." ) );

  // Check if coverage is a multi-layer grid and if so, enable Layer typein:

  const bool isCompare = coverage.startsWith( "compare." );

  isMultiLayered =
    OR2( AND2( coverage.startsWith( "cmaq." ),
               ! OR11( coverage.contains( "2d." ),
                       AND2( coverage.contains( ".emis." ),
                             ! coverage.contains( "hemi.emis." ) ),
                       coverage.contains( ".wdt." ),
                       coverage.contains( ".aconc." ),
                       coverage.contains( ".conus.aerovis." ),
                       coverage.contains( ".aod." ),
                       coverage.contains( ".integrated." ),
                       coverage.contains( ".dep." ),
                       coverage.contains( ".depv." ),
                       coverage.contains( ".drydep." ),
                       coverage.contains( ".wetdep." ) ) ),
         AND2( isCompare,
               OR2( coverage.contains( "acam." ),
                    coverage.contains( "calipso." ) ) ) );

  forceSingleLayer = false; // Must initialize output parameter!

  // Check if coverage is already CMAQ-format (so cannot be regridded again)
  // and if so, disable Regrid button:

  noRegrid =
    OR8( coverage.startsWith( "cmaq." ),
         coverage.startsWith( "omibehrioapi." ),
         coverage.startsWith( "tempo.l3." ),
         coverage.startsWith( "tempo2.l3." ),
         coverage.startsWith( "hrrr." ),
         coverage.startsWith( "regridded." ),
         coverage.startsWith( "compare." ),
         coverage.startsWith( "ceilometer." ) ); // UNIMPLEMENTED: regridded ceilometer curtain vis.
}



// Set show/hide/enable/disable aggregate menu, layer typein and regrid button
// based on selected data variable coverage.

void ControlDockWidget::onTreeWidgetItemChanged( QTreeWidgetItem* item ) {

  // Use a temporary table of widgets to adjust based on selection:

  typedef struct {
    QPlainTextEdit* const selectedData;
    QComboBox* const aggregateMenu;
    QSpinBox* const layer;
    QCheckBox* const regridCheckbox;
  } Options;

  Options table[ 5 ] = {
    { ui->selectedData1, ui->aggregate1, ui->selectedLayer1, ui->regridCheckbox1 },
    { ui->selectedData2, ui->aggregate2, ui->selectedLayer2, ui->regridCheckbox2 },
    { ui->selectedData3, ui->aggregate3, ui->selectedLayer3, ui->regridCheckbox3 },
    { ui->selectedData4, ui->aggregate4, ui->selectedLayer4, ui->regridCheckbox4 },
    { ui->selectedData5, ui->aggregate5, ui->selectedLayer5, ui->regridCheckbox5 }
  };

  const QString coverage( getCoverage( item ) ); // Fully dotted e.g., aqs.no2.
  
  if ( item->checkState( 0 ) == Qt::Checked ) {

    if ( num_selected_sources < 5 ) {
      ++num_selected_sources;

      // Set size, visibility, etc. of optional widgets based on coverage:

      bool canAggregate = false;
      bool isMultiLayered = false;
      bool forceSingleLayer = false;
      bool noRegrid = false;

      getCoverageProperties( coverage,
                             canAggregate,
                             isMultiLayered,
                             forceSingleLayer,
                             noRegrid );

      // Find first empty position, starting from top:

      size_t index = 0;

      while ( AND2( index < sizeof table / sizeof *table,
                    table[ index ].selectedData->toPlainText() != "" ) ) {
        ++index;
      }

      assert( index < sizeof table / sizeof *table );
      Options& options = table[ index ];

      // Copy fully-dotted coverage to selected data line and set tooltip:

      options.selectedData->setPlainText( coverage );
      options.selectedData->setToolTip( item->toolTip( 0 ) );

      // Set state/visibility of Aggregate Menu and
      // also adjust length of selectedData to fit and scroll to show variable:

      options.aggregateMenu->setCurrentIndex( 0 ); // Default state.
      const RSIGState* gState = RSIGStateSingleton::getInstance();

      if ( canAggregate && ( gState->mTimebaseType == RSIGState::TIMEBASE_HOURLY || gState->mTimebaseType == RSIGState::TIMEBASE_DAILY )) {
        options.selectedData->setFixedWidth( selectedDataShortWidth );
        options.selectedData->moveCursor( QTextCursor::End ); // Show var.
        options.aggregateMenu->show();
      } else {
        options.selectedData->setFixedWidth( selectedDataLongWidth );
        options.aggregateMenu->hide();
      }

      // Set state of Layer typein:

      options.layer->setValue( 0 ); // Default to all layers.

      if ( isMultiLayered ) {
        options.layer->setEnabled( true );
        options.layer->show();

        if ( forceSingleLayer ) {
          options.layer->setValue( 1 ); // Change 0=all to layer 1 only.
        }
      } else {
        options.layer->setEnabled( false );
        options.layer->hide();
      }

      // Set state of Regrid checkbox:

      options.regridCheckbox->setCheckState( Qt::Unchecked ); // Default state.

      if ( noRegrid ) {
        options.regridCheckbox->hide();
      } else {
        options.regridCheckbox->show();
      }

    } else { // Already have 5 sources selected, so uncheck the variable:
      ui->textBrowser->append( "Maximum limit of 5 data sources." );
      ++num_selected_sources; // Compensate for the box briefly being checked.
      item->setCheckState( 0, Qt::Unchecked ); // Indirect tail recursive call.
    }

  } else {

    // Unchecked a previously selected variable so
    // remove item from selected sources and reset option widgets to defaults:

    size_t index = 0;

    while ( AND2( index < sizeof table / sizeof *table,
                  table[ index ].selectedData->toPlainText() != coverage ) ) {
      ++index;
    }

    if ( index < sizeof table / sizeof *table ) {
      Options& options = table[ index ];
      options.selectedData->setPlainText( "" );
      options.selectedData->setToolTip( "" );
      options.selectedData->setFixedWidth( selectedDataLongWidth );
      options.layer->setValue( 0 );
      options.layer->setEnabled( false );
      options.layer->hide();
      options.aggregateMenu->setCurrentIndex( 0 );
      options.aggregateMenu->hide();
      options.regridCheckbox->setCheckState( Qt::Unchecked );
      options.regridCheckbox->show();
      //--num_selected_sources;
      assert( num_selected_sources >= 0 );
    }

    --num_selected_sources;

  }

}



int ControlDockWidget::get_animationSpeed() {
  // get the value from the animation speed spinbox and turn it into
  // a delay in milliseconds for the animation timer.
  // speed=1, delay=1500
  // speed=20, delay=0
  int base_delay = 2; // needed to prevent timer from overrunning rendering
  int speed = ui->animationSpeed->value();
  int delay = speed*(-50) + 1000 + base_delay;
  return delay;
}



void ControlDockWidget::update_timestepSlider(int counter_value) {
  // given a counter value, set the slider position accordingly
  // used to keep the slider in sync if other buttons are pressed
  const RSIGState* gState = RSIGStateSingleton::getInstance();
  const int duration = ui->durationSpinBox->value();
  int totalTimesteps = 24; // default

  switch (gState->mTimebaseType) {
  case RSIGState::TIMEBASE_HOURLY:
    totalTimesteps = duration * 24;
    break;
  case RSIGState::TIMEBASE_DAILY:
    totalTimesteps = duration;
    break;
  case RSIGState::TIMEBASE_DAILY_LST:
    totalTimesteps = duration;
    break;
  case RSIGState::TIMEBASE_MONTHLY:
    totalTimesteps = duration;
    break;
  default:
    break;
  }
  
  int new_sliderValue = counter_value;

  if (new_sliderValue >= totalTimesteps) {
    new_sliderValue = new_sliderValue - totalTimesteps;
  }

  if (new_sliderValue < 0) {
    new_sliderValue = new_sliderValue + totalTimesteps;
  }

  ui->timestepSlider->setValue(new_sliderValue);
}



void ControlDockWidget::update_playButtonText(QString buttontext) {
  ui->playButton->setText(buttontext);  
}



void ControlDockWidget::onAnimationSpeedChanged(int) {
  Q_EMIT requestRedraw();
}



void ControlDockWidget::generateDefaultPngFilename() {
  const emvl::Timestamp currentTimestamp( RSIGStateSingleton::getInstance()->getCurrentTimestamp() );
  QString utcHour = QString::number(currentTimestamp.startOfHour().hhmmss() / 10000).rightJustified(2, '0');
  emvl::Timestamp::UTCString yyyy_mm_dd = "";
  currentTimestamp.toUTCString( yyyy_mm_dd );
  char* const t = strchr( yyyy_mm_dd, 'T' );

  if ( t ) {
    *t = '\0'; // Terminate string at 'T', yielding date portion: "YYYY-MM-DD"
  }

  QString yyyy_mm = QString(yyyy_mm_dd).mid(0,7);
  QString fileName( "EPA-RSIG-Snapshot_" );
  
  RSIGState *gState = RSIGStateSingleton::getInstance();
  if (gState->mTimebaseType == RSIGState::TIMEBASE_HOURLY) {
    fileName.append( yyyy_mm_dd ); // YYYY-MM-DD.
    fileName.append( "_" + utcHour ); // Time
    fileName.append( "Z.png" );
  } else if (gState->mTimebaseType == RSIGState::TIMEBASE_DAILY) {
    fileName.append(yyyy_mm_dd);
    fileName.append(".png");
  } else if (gState->mTimebaseType == RSIGState::TIMEBASE_DAILY_LST) {
    fileName.append(yyyy_mm_dd);
    fileName.append(".png");
  } else { //monthly
    fileName.append(yyyy_mm);
    fileName.append(".png");
  }

  VisOptions* vOptions = VisOptionsSingleton::getInstance();
  vOptions->setFilenamePngDefault( fileName );
  ui->filenameLineEdit->setText(vOptions->getFilenamePngDefault());
}



void ControlDockWidget::ontimestepSliderChanged(int timestep) {
  RSIGStateSingleton::getInstance()->mCounter = timestep;
  generateDefaultPngFilename();
  
  if (ui->currentpngRadioButton->isChecked()) {
    const QString curFilename( ui->filenameLineEdit->text() );
    const VisOptions* const vOptions = VisOptionsSingleton::getInstance();

    if (curFilename.contains("EPA-RSIG-Snapshot_") && curFilename.length() == 36) {
      ui->filenameLineEdit->setText(vOptions->getFilenamePngDefault());
      ui->filename_generated->setText(vOptions->getFilenamePngDefault());
    } else {
      ui->filenameLineEdit->setText(vOptions->getFilenamePng());
      ui->filename_generated->setText(vOptions->getFilenamePng());
    }
  }

  Q_EMIT requestRedraw();
}



void ControlDockWidget::onPlayButtonClicked() {
  QKeyEvent *event = new QKeyEvent ( QEvent::KeyPress, Qt::Key_P, Qt::NoModifier);
  QCoreApplication::postEvent (this, event);
  Q_EMIT requestRedraw();
}



void ControlDockWidget::onForwardButtonClicked() {
  // update the mCounter
  RSIGState *gState = RSIGStateSingleton::getInstance();

  if (gState->mTimebaseType == RSIGState::TIMEBASE_HOURLY) {
    gState->mCounter = (gState->mCounter + 1) % (gState->mDuration * 24);
  } else {
    gState->mCounter = (gState->mCounter + 1) % gState->mDuration;
  }

  update_timestepSlider(gState->mCounter);
  Q_EMIT requestRedraw();
}

void ControlDockWidget::onForwardButton24Clicked() {
  // update the mCounter by 24 hours (for Luke)
  RSIGState *gState = RSIGStateSingleton::getInstance();

  if (gState->mTimebaseType == RSIGState::TIMEBASE_HOURLY) {
    gState->mCounter = (gState->mCounter + 24) % (gState->mDuration * 24);
  } else {
    qDebug() << "No logic implemented";
  }

  update_timestepSlider(gState->mCounter);
  Q_EMIT requestRedraw();
}



void ControlDockWidget::onBackButtonClicked() {
  // update the mCounter
  RSIGState* gState = RSIGStateSingleton::getInstance();
  int newCounterVal = gState->mCounter - 1;

  if (newCounterVal < 0) {

    if (gState->mTimebaseType == RSIGState::TIMEBASE_HOURLY) {
      newCounterVal = gState->mDuration * 24 - 1;
    } else {
      newCounterVal = gState->mDuration - 1;
    }
  }

  gState->mCounter = newCounterVal;
  update_timestepSlider(gState->mCounter);
  Q_EMIT requestRedraw();
}

void ControlDockWidget::onBackButton24Clicked() {
  // update the mCounter by 24 hours (for Luke)
  RSIGState* gState = RSIGStateSingleton::getInstance();

  int newCounterVal = gState->mCounter - 24;

  if (newCounterVal < 0) {

    if (gState->mTimebaseType == RSIGState::TIMEBASE_HOURLY) {
      newCounterVal = gState->mDuration * 24 + newCounterVal;
    } else {
      qDebug() << "No logic implemented";
    }
  }

  gState->mCounter = newCounterVal;
  update_timestepSlider(gState->mCounter);
  Q_EMIT requestRedraw();
}



void ControlDockWidget::onOpacitySliderChanged(int opacityValue) {
  // get the name from the combo box
  const QString curName = ui->vizVarCombobox->currentText();

  if (curName.contains("True Color imagery")) {
    VisOptions* vOptions = VisOptionsSingleton::getInstance();
    vOptions->setOpacityModisTrueColor((float)opacityValue);
    // update the opacity label
    ui->opacity_display->setText( QString::number(opacityValue+1) );
    // redraw
    Q_EMIT requestRedraw();
  } else {
    // get an instance of the vislist (which contains all vis objects)
    VisDataList* vislist = VisDataListSingleton::getInstance();
    // extract the vis object corresponding to the selected variable from the combo box
    VisDataObject* visObj = vislist->getObjectByName(curName);

    if (visObj != NULL) {
      // set the opacity of the object
      visObj->setAlpha((float)(opacityValue+1)/100);
      // update the opacity label
      ui->opacity_display->setText( QString::number(opacityValue+1) );
    }
  }
}



void ControlDockWidget::onThresholdSliderChanged(int sliderValue) {
  // get the name from the combo box
  const QString curName = ui->vizVarCombobox->currentText();
  // get an instance of the vislist (which contains all vis objects)
  VisDataList* vislist = VisDataListSingleton::getInstance();
  // extract the vis object corresponding to the selected variable from the combo box
  VisDataObject* visObj = vislist->getObjectByName(curName);
  // ditto for the geo object
  GeoDataObject* geoObj = 0;

  if (visObj != NULL) {
    geoObj = qobject_cast<GeoDataObject*>(visObj->parent());
  }

  if (visObj != NULL && geoObj != NULL) {
    float min = visObj->minmaxVal().x;
    float max = visObj->minmaxVal().y;
    float threshold_value = min + (sliderValue/100.0 * (max - min));
    visObj->setThreshold(threshold_value);
    visObj->setThresholdSliderValue(sliderValue);
    // update the label
    ui->ThresholdLabel->setText( QString::number(threshold_value) );
  }
}

void ControlDockWidget::onShowCMAQRowChanged(int flag) {
  
  // get the name from the combo box
  const QString curName = ui->vizVarCombobox->currentText();

  // get an instance of the vislist (which contains all vis objects)
  VisDataList* vislist = VisDataListSingleton::getInstance();

  // extract the vis object corresponding to the selected variable from the combo box
  VisDataObject* visObj = vislist->getObjectByName(curName);

  if (visObj != NULL) {
    if (flag == Qt::Checked) {
      visObj->setShowCMAQRow(true);
      visObj->setCMAQRow(ui->CMAQRowSlider->value());
      ui->CMAQRowSlider->setEnabled(true);
      ui->CMAQRowLabel->setEnabled(true);
    } else {
      visObj->setShowCMAQRow(false);
      visObj->setCMAQRow(0);
      ui->CMAQRowSlider->setEnabled(false);
      ui->CMAQRowLabel->setEnabled(false);
    }
  }
}

void ControlDockWidget::onShowCMAQColChanged(int flag) {
  // get the name from the combo box
  const QString curName = ui->vizVarCombobox->currentText();
  // get an instance of the vislist (which contains all vis objects)
  VisDataList* vislist = VisDataListSingleton::getInstance();
  // extract the vis object corresponding to the selected variable from the combo box
  VisDataObject* visObj = vislist->getObjectByName(curName);

  if (visObj != NULL) {
    if (flag == Qt::Checked) {
      visObj->setShowCMAQCol(true);
      visObj->setCMAQCol(ui->CMAQColSlider->value());
      ui->CMAQColSlider->setEnabled(true);
      ui->CMAQColLabel->setEnabled(true);
    } else {
      visObj->setShowCMAQCol(false);
      visObj->setCMAQCol(0);
      ui->CMAQColSlider->setEnabled(false);
      ui->CMAQColLabel->setEnabled(true);
    }
  }
}



void ControlDockWidget::onShowCMAQLayerChanged(int flag) {
  // get the name from the combo box
  const QString curName = ui->vizVarCombobox->currentText();
  // get an instance of the vislist (which contains all vis objects)
  VisDataList* vislist = VisDataListSingleton::getInstance();
  // extract the vis object corresponding to the selected variable from the combo box
  VisDataObject* visObj = vislist->getObjectByName(curName);

  if (visObj != NULL) {
    if (flag == Qt::Checked) {
      visObj->setShowCMAQLayer(true);
      visObj->setCMAQLayer(ui->CMAQLayerSlider->value());
      ui->CMAQLayerSlider->setEnabled(true);
      ui->CMAQLayerLabel->setEnabled(true);
    } else {
      visObj->setShowCMAQLayer(false);
      visObj->setCMAQLayer(0);
      ui->CMAQLayerSlider->setEnabled(false);
      ui->CMAQLayerLabel->setEnabled(true);
    }
  }
}



void ControlDockWidget::onCMAQRowSliderChanged(int rowValue) {
  // get the name from the combo box
  const QString curName = ui->vizVarCombobox->currentText();
  // get an instance of the vislist (which contains all vis objects)
  VisDataList* vislist = VisDataListSingleton::getInstance();
  // extract the vis object corresponding to the selected variable from the combo box
  VisDataObject* visObj = vislist->getObjectByName(curName);

  if (visObj != NULL) {
    // set the row of the object
    visObj->setCMAQRow(rowValue);
    // update the label
    ui->CMAQRowLabel->setText( QString::number(rowValue) );
  }
}



void ControlDockWidget::onCMAQColSliderChanged(int colValue) {
  // get the name from the combo box
  const QString curName = ui->vizVarCombobox->currentText();
  // get an instance of the vislist (which contains all vis objects)
  VisDataList* vislist = VisDataListSingleton::getInstance();
  // extract the vis object corresponding to the selected variable from the combo box
  VisDataObject* visObj = vislist->getObjectByName(curName);

  if (visObj != NULL) {
    // set the row of the object
    visObj->setCMAQCol(colValue);
    // update the label
    ui->CMAQColLabel->setText( QString::number(colValue) );
  }
}



void ControlDockWidget::onCMAQLayerSliderChanged(int layerValue) {
  // get the name from the combo box
  const QString curName = ui->vizVarCombobox->currentText();
  // get an instance of the vislist (which contains all vis objects)
  VisDataList* vislist = VisDataListSingleton::getInstance();
  // extract the vis object corresponding to the selected variable from the combo box
  VisDataObject* visObj = vislist->getObjectByName(curName);

  if (visObj != NULL) {
    // set the row of the object
    visObj->setCMAQLayer(layerValue);
    // update the label
    ui->CMAQLayerLabel->setText( QString::number(layerValue) );
  }
}



void ControlDockWidget::onVisibilityChanged(int visibility_flag) {
  // get the name from the combo box
  const QString curName = ui->vizVarCombobox->currentText();

  if (curName.contains("True Color imagery")) {
    VisOptions* vOptions = VisOptionsSingleton::getInstance();
    vOptions->setDrawModisTrueColor(visibility_flag);
    Q_EMIT requestRedraw();
  } else {
    // get an instance of the vislist (which contains all vis objects)
    VisDataList* vislist = VisDataListSingleton::getInstance();
    // extract the vis object corresponding to the selected variable from the combo box
    VisDataObject* visObj = vislist->getObjectByName(curName);

    if (visObj != NULL) {
      // set the visibility of the object
      visObj->setShowObject(bool(visibility_flag>0));
    }
  }
}



void ControlDockWidget::onShowCellBoundaryChanged(int show_flag) {
  // get the name from the combo box
  const QString curName = ui->vizVarCombobox->currentText();
  // get an instance of the vislist (which contains all vis objects)
  VisDataList* vislist = VisDataListSingleton::getInstance();
  // extract the vis object corresponding to the selected variable from the combo box
  VisDataObject* visObj = vislist->getObjectByName(curName);

  if (visObj != NULL) {
    visObj->setShowCellBoundary(bool(show_flag>0));
  }
}



void ControlDockWidget::onDataDefaultChanged(int dataMinMax_flag) {
  // get the name from the combo box
  const QString curName = ui->vizVarCombobox->currentText();
  // get an instance of the vislist (which contains all vis objects)
  VisDataList* vislist = VisDataListSingleton::getInstance();
  // extract the vis object corresponding to the selected variable from the combo box
  VisDataObject* visObj = vislist->getObjectByName(curName);
  if (visObj != NULL) {
    // remember that this visobj is using the defaults
    visObj->setDefaultRangeChecked((bool)dataMinMax_flag);

    // set the min/max of the vis object

    if (dataMinMax_flag != 0) {
      // deactivate min/max spinboxes
       ui->vizMinSpinbox->setEnabled(FALSE);
       ui->vizMaxSpinbox->setEnabled(FALSE);

      // set limits to min/max of data
      Vector2f minmax = visObj->minmaxData();
      ui->vizMinSpinbox->setRange(minmax.x, minmax.y);
      ui->vizMaxSpinbox->setRange(minmax.x, minmax.y);
      ui->vizMinSpinbox->setSingleStep((minmax.y-minmax.x)/25);
      ui->vizMaxSpinbox->setSingleStep((minmax.y-minmax.x)/25);
      ui->vizMinSpinbox->setValue(minmax.x);
      ui->vizMaxSpinbox->setValue(minmax.y);
    } else {
      // activate min/max spinboxes
      ui->vizMinSpinbox->setEnabled(TRUE);
      ui->vizMaxSpinbox->setEnabled(TRUE);

      // set limits per user's spec
      ui->vizMinSpinbox->setRange(-1e23, 1e23);
      ui->vizMaxSpinbox->setRange(-1e23, 1e23);
      ui->vizMinSpinbox->setValue(visObj->lastMinval());
      ui->vizMaxSpinbox->setValue(visObj->lastMaxval());
    }
  }
}



void ControlDockWidget::onMinSpinboxChanged(double value) {
  // get the name from the combo box
  const QString curName = ui->vizVarCombobox->currentText();
  // get an instance of the vislist (which contains all vis objects)
  VisDataList* vislist = VisDataListSingleton::getInstance();
  // extract the vis object corresponding to the selected variable from the combo box
  VisDataObject* visObj = vislist->getObjectByName(curName);

  if (visObj != NULL) {
    visObj->setMinval(value);

    if (!ui->vizDataDefault->isChecked()) {
      visObj->setLastMinval(value);
      visObj->setMinValChangedThisSession(true);
    }

    // change threshold minimum
    onThresholdSliderChanged(ui->ThresholdSlider->value());

    // extract the lookup table corresponding to the selected variable from the combo box
    QString curLut = curName;
    // convert name to lut (we either have polygon, point, or vector - just convert all)
    curLut.replace(QString(".polygon"), QString(".lut"));
    curLut.replace(QString(".point"), QString(".lut"));
    curLut.replace(QString(".vector"), QString(".lut"));

    LookupTableWidget *lut = vislist->getLutByName(curLut);

    if (lut != NULL) {
      Vector2f rng = visObj->minmaxVal();
      lut->adjustRange(rng.x, rng.y);
    }
  }
}



void ControlDockWidget::onMaxSpinboxChanged(double value) {
  // get the name from the combo box
  const QString curName = ui->vizVarCombobox->currentText();
  // get an instance of the vislist (which contains all vis objects)
  VisDataList* vislist = VisDataListSingleton::getInstance();
  // extract the vis object corresponding to the selected variable from the combo box
  VisDataObject* visObj = vislist->getObjectByName(curName);

  if (visObj != NULL) {
    visObj->setMaxval(value);

    if (!ui->vizDataDefault->isChecked()) {
      visObj->setLastMaxval(value);
      visObj->setMaxValChangedThisSession(true);
    }

    // threshold value must change since limits were changed
    onThresholdSliderChanged(ui->ThresholdSlider->value());

    // extract the lookup table corresponding to the selected variable from the combo box
    QString curLut = curName;
    // convert name to lut (we either have polygon, point, or vector - just convert all)
    curLut.replace(QString(".polygon"), QString(".lut"));
    curLut.replace(QString(".point"), QString(".lut"));
    curLut.replace(QString(".vector"), QString(".lut"));

    LookupTableWidget *lut = vislist->getLutByName(curLut);

    if (lut != NULL) {
      Vector2f rng = visObj->minmaxVal();
      lut->adjustRange(rng.x, rng.y);
    }
  }
}



void ControlDockWidget::setVisWidgetState(VisDataObject* visObj) {
  // get an instance of the vis options
  VisOptions* vOptions = VisOptionsSingleton::getInstance();

  // set the controls to match the selected variable 
  if (visObj != NULL) {
    // visibility
    ui->vizCheckBox->setChecked(visObj->isVisible());
    
    // opacity
    ui->vizOpacitySlider->setSliderPosition(visObj->alpha()*100-1);
    
    // threshold
    ui->ThresholdSlider->setSliderPosition(visObj->thresholdSliderValue());
    ui->ThresholdLabel->setText( QString::number(visObj->threshold()) );
    ui->thresholdCheckBox->setChecked(visObj->ShowThreshold());

    // cell boundary
    ui->cellBoundaryCheckbox->setChecked(visObj->showCellBoundary());

    const GeoDataObject* const geoDataObject = qobject_cast<GeoDataObject*>(visObj->parent());
    const emvl::Dataset* const dataset       = geoDataObject ? geoDataObject->model() : 0;
    const bool isGrid                        = dataset ? dataset->isStructuredGrid() : 0;

    // special controls
    const QString& name = visObj->objectName();
    switchSpecialControlsWidget( isGrid, name );

    if ( isGrid ) {
      //bool mode2D = vOptions->getMode2D();
      int displayMode = vOptions->getDisplayMode();
      // show CMAQ widgets
      ui->ShowCMAQRow->show();
      ui->ShowCMAQCol->show();
      ui->ShowCMAQLayer->show();
      ui->CMAQRowSlider->show();
      ui->CMAQColSlider->show();
      ui->CMAQLayerSlider->show();
      ui->CMAQRowLabel->show();
      ui->CMAQColLabel->show();
      ui->CMAQLayerLabel->show();
      ui->flattenCMAQ->show();

      // set max of sliders
      ui->CMAQRowSlider->setMaximum(visObj->MaxCMAQRow());
      ui->CMAQColSlider->setMaximum(visObj->MaxCMAQCol());
      ui->CMAQLayerSlider->setMaximum(visObj->MaxCMAQLayer());

      // enable widgets
      //if (!mode2D) {
      if (displayMode == 1) { // 3D
        ui->ShowCMAQRow->setEnabled(true);
        ui->ShowCMAQCol->setEnabled(true);
      }

      ui->ShowCMAQLayer->setEnabled(true);
      // set widget values to this visObj

      //if (!mode2D && visObj->ShowCMAQRow()) {
      if (displayMode == 1 && visObj->ShowCMAQRow()) { // displaymode 3d
        ui->ShowCMAQRow->setCheckState(Qt::Checked);
        ui->CMAQRowSlider->setEnabled(true);
        ui->CMAQRowLabel->setEnabled(true);
        ui->CMAQRowSlider->setSliderPosition(visObj->CMAQRow());
      } else {
        ui->ShowCMAQRow->setCheckState(Qt::Unchecked);
        ui->CMAQRowSlider->setEnabled(false);
        ui->CMAQRowLabel->setEnabled(false);
      }

      //if (!mode2D && visObj->ShowCMAQCol()) {
      if (displayMode == 1 && visObj->ShowCMAQCol()) { // displaymode 3D
        ui->ShowCMAQCol->setCheckState(Qt::Checked);
        ui->CMAQColSlider->setEnabled(true);
        ui->CMAQColLabel->setEnabled(true);
        ui->CMAQColSlider->setSliderPosition(visObj->CMAQCol());
      } else {
        ui->ShowCMAQCol->setCheckState(Qt::Unchecked);
        ui->CMAQColSlider->setEnabled(false);
        ui->CMAQColLabel->setEnabled(false);
      }

      if (visObj->ShowCMAQLayer()) {
        ui->ShowCMAQLayer->setCheckState(Qt::Checked);
        ui->CMAQLayerSlider->setEnabled(true);
        ui->CMAQLayerLabel->setEnabled(true);
        ui->CMAQLayerSlider->setSliderPosition(visObj->CMAQLayer());
      } else {
        ui->ShowCMAQLayer->setCheckState(Qt::Unchecked);
        ui->CMAQLayerSlider->setEnabled(false);
        ui->CMAQLayerLabel->setEnabled(false);
      }
    } else {
      // hide CMAQ widgets
      ui->ShowCMAQRow->hide();
      ui->ShowCMAQCol->hide();
      ui->ShowCMAQLayer->hide();
      ui->CMAQRowSlider->hide();
      ui->CMAQColSlider->hide();
      ui->CMAQLayerSlider->hide();
      ui->CMAQRowLabel->hide();
      ui->CMAQColLabel->hide();
      ui->CMAQLayerLabel->hide();
      ui->flattenCMAQ->hide();

      // disable widgets
      ui->ShowCMAQRow->setEnabled(false);
      ui->ShowCMAQCol->setEnabled(false);
      ui->ShowCMAQLayer->setEnabled(false);
      ui->CMAQRowSlider->setEnabled(false);
      ui->CMAQColSlider->setEnabled(false);
      ui->CMAQLayerSlider->setEnabled(false);
      ui->CMAQRowLabel->setEnabled(false);
      ui->CMAQColLabel->setEnabled(false);
      ui->CMAQLayerLabel->setEnabled(false);
    }

    const bool isVector = name.endsWith( ".vector" );

    if ( isVector ) {
      ui->windVectorScaleFactor->show();
      ui->windVectorTipSize->show();
      ui->windVectorSparseness->show();
      ui->windVectorSparsenessLabel->show();
      ui->autoSparseCheckBox->show();
      ui->windVectorHeadSizeLabel->show();
      ui->windVectorScaleFactorLabel->show();
      ui->windVectorBarbOption->show();
      ui->windVectorArrowOption->show();
      ui->WindLabel->show();
    } else {
      ui->windVectorScaleFactor->hide();
      ui->windVectorTipSize->hide();
      ui->windVectorSparseness->hide();
      ui->windVectorSparsenessLabel->hide();
      ui->autoSparseCheckBox->hide();
      ui->windVectorHeadSizeLabel->hide();
      ui->windVectorScaleFactorLabel->hide();
      ui->windVectorBarbOption->hide();
      ui->windVectorArrowOption->hide();
      ui->WindLabel->hide();
    }



    // colormap
    ui->vizColormapCombobox->setCurrentIndex(visObj->curColormapIndex());

    // default range
    ui->vizDataDefault->setChecked(visObj->isDefaultRangeChecked());

    // temporarily block events for min and max spinboxen 
    ui->vizMinSpinbox->blockSignals(true);
    ui->vizMaxSpinbox->blockSignals(true);
    ui->vizDataDefault->blockSignals(true);
    
    // data limits
    if (visObj->objectName().contains("calipso") && visObj->objectName().contains("backscatter")) {
      // data limits are preset to match the backscatter colormap
      ui->vizMinSpinbox->setValue(0.001);
      ui->vizMaxSpinbox->setValue(0.1);
    } else {

      if (ui->vizDataDefault->checkState() == Qt::Checked) {
        // set limits to min/max of data
        Vector2f minmax = visObj->minmaxData();
        ui->vizMinSpinbox->setRange(minmax.x, minmax.y);
        ui->vizMaxSpinbox->setRange(minmax.x, minmax.y);
        ui->vizMinSpinbox->setSingleStep((minmax.y-minmax.x)/25);
        ui->vizMaxSpinbox->setSingleStep((minmax.y-minmax.x)/25);
        ui->vizMinSpinbox->setValue(minmax.x);
        ui->vizMaxSpinbox->setValue(minmax.y);
      } else { // set limits per user's spec
        ui->vizMinSpinbox->setRange(-1e23, 1e23);
        ui->vizMaxSpinbox->setRange(-1e23, 1e23);
        ui->vizMinSpinbox->setValue(visObj->minval());
        ui->vizMaxSpinbox->setValue(visObj->maxval());
      }
      
      // unblock events for min and max spinboxen 
      ui->vizMinSpinbox->blockSignals(false);
      ui->vizMaxSpinbox->blockSignals(false);
      ui->vizDataDefault->blockSignals(false);
    }
  
    // inactivate min/max spinboxes for certain variables, activate for all others
    if (visObj->objectName().contains("calipso") && visObj->objectName().contains("backscatter")) {
      ui->vizMinSpinbox->setEnabled(false);
      ui->vizMaxSpinbox->setEnabled(false);
      ui->vizDataDefault->setEnabled(false);
    } else if (!visObj->isDefaultRangeChecked()) {
      ui->vizMinSpinbox->setEnabled(true);
      ui->vizMaxSpinbox->setEnabled(true);
      ui->vizDataDefault->setEnabled(true);
    } else {
      ui->vizDataDefault->setEnabled(true);
    }
  } else {
    // visObj was NULL (happens if "MODIS TRUE Color imagery" was selected)
    // special controls
    switchSpecialControlsWidget( false, "none" );
  }
}        



void ControlDockWidget::switchSpecialControlsWidget( const bool isGrid,
                                                     const QString& curName ) {

  // stacked widget index
  // 0 - nothing
  // 1 = grid controls
  // 2 = ceilometer
  // 3 = neubrew ... etc

  
  VisOptions* vOptions = VisOptionsSingleton::getInstance();

  if (vOptions->getDisplayMode() < 2) {
    if ( isGrid) {
      ui->specialControlsStackedWidget->setCurrentIndex(1);
      ui->SpecialControlLabel->setText("Grid\nControls    ");
    } else if (curName.startsWith("ceilometer")) {
      ui->specialControlsStackedWidget->setCurrentIndex(2);
      ui->SpecialControlLabel->setText("Ceilometer\nControls   ");
    } else if (curName.startsWith("neubrew")) {
      ui->specialControlsStackedWidget->setCurrentIndex(3);
      ui->SpecialControlLabel->setText("NEUBrew\nControls    ");
    } else if (curName.startsWith("behr")) { // behr and behrioapi
      ui->specialControlsStackedWidget->setCurrentIndex(4);
      ui->SpecialControlLabel->setText("BEHR\nControls    ");
    } else if (curName.startsWith("modis_")) {
      ui->specialControlsStackedWidget->setCurrentIndex(5);
      ui->SpecialControlLabel->setText("MODIS\nControls    ");
    } else if (curName.startsWith("viirs")) {
      ui->specialControlsStackedWidget->setCurrentIndex(6);
      ui->SpecialControlLabel->setText("VIIRS\nControls    ");
    } else if (curName.startsWith("tropomi_")) {
      ui->specialControlsStackedWidget->setCurrentIndex(7);
      ui->SpecialControlLabel->setText("TROPOMI\nControls    ");
    } else if (curName.startsWith("hms_")) {
      ui->specialControlsStackedWidget->setCurrentIndex(8);
      ui->SpecialControlLabel->setText("HMS\nControls    ");
    } else if (curName.startsWith("pandora")) {
      ui->specialControlsStackedWidget->setCurrentIndex(10);
      ui->SpecialControlLabel->setText("Pandora\nControls    ");
    } else if (curName.startsWith("tempo_")) {
      ui->specialControlsStackedWidget->setCurrentIndex(11);
      ui->SpecialControlLabel->setText("TEMPO\nControls    ");
    } else {
      ui->specialControlsStackedWidget->setCurrentIndex(0);
      ui->SpecialControlLabel->setText("");
    }


  } else {
    ui->specialControlsStackedWidget->setCurrentIndex(9);
    ui->SpecialControlLabel->setText("Panel Display\nControls    ");
  }
}



void ControlDockWidget::enableModisTrueColorOptions(bool flag) {
  ui->cellBoundaryCheckbox->setEnabled(flag);
  ui->vizMinSpinbox->setEnabled(flag);
  ui->vizMaxSpinbox->setEnabled(flag);
  ui->vizDataDefault->setEnabled(flag);
  ui->thresholdCheckBox->setEnabled(flag);
  ui->ThresholdSlider->setEnabled(flag);
  ui->vizColormapCombobox->setEnabled(flag);
}



void ControlDockWidget::onVizVarIndexChanged(int index) {

  if (index>=0 ) {
    // get the name from the combo box
    const QString curName = ui->vizVarCombobox->currentText();

    if (curName.contains("True Color imagery")) {
      setVisWidgetState(NULL);
      VisOptions* vOptions = VisOptionsSingleton::getInstance();
      ui->vizCheckBox->setChecked(vOptions->getDrawModisTrueColor());
      // deavtivate controls that are not applicable to MODIS True Color
      enableModisTrueColorOptions(false);
      // opacity
      ui->vizOpacitySlider->setSliderPosition(vOptions->getOpacityModisTrueColor());
    } else {
      // get an instance of the vislist (which contains all vis objects)
      VisDataList* vislist = VisDataListSingleton::getInstance();
      enableModisTrueColorOptions(true);
      // extract the vis object corresponding to the selected variable from the combo box
      VisDataObject* visObj = vislist->getObjectByName(curName);
      setVisWidgetState(visObj);
    }
  }
}



void ControlDockWidget::onColormapIndexChanged(int index) {

  if (index>=0) {
    // get the name from the combo box
    const QString curName = ui->vizVarCombobox->currentText();

    // get an instance of the vislist (which contains all vis objects)
    VisDataList* vislist = VisDataListSingleton::getInstance();

    // extract the vis object corresponding to the selected variable from the combo box
    VisDataObject* const visObj = vislist->getObjectByName(curName);

    if (visObj) {
      QString aggregate = visObj->getAggregate();
      QString variableName = curName.toLower();
      QString colormapName = QString(emvl::getRGBColormapName(index));

      // Check to see if special colormap is valid to use with the selected variable
      if ( (colormapName.contains("AQI-PM25-24hr") && !(variableName.contains("pm25_daily_average") || (variableName.contains("_pm25") && aggregate.contains("daily"))) )                                                 ||
           (colormapName.contains("AQI-Ozone-1hr") && !(variableName.endsWith("ozone.point") && aggregate.contains("none")))                                                                                              ||
           (colormapName.contains("AQI-Ozone-8hr") && !(variableName.contains("ozone_8hour_average") || variableName.contains("ozone_daily_8hour") || (variableName.contains("_ozone") && aggregate.contains("daily"))))  ||
           (colormapName.contains("AQI-CO-8hr")    && !(variableName.contains("aqs_co") && aggregate.contains("daily")))                                                                                                  ||
           (colormapName.contains("AQI-NO2-1hr")   && !(variableName.endsWith("no2.point") && aggregate.contains("none")))                                                                                                ||
           (colormapName.contains("AQI-SO2-1hr")   && !(variableName.endsWith("so2.point") && aggregate.contains("none")))                                                                                                ||
           (colormapName.contains("CALIPSO-Backscatter")    && !(variableName.contains("_backscatter")))                                                                                                                  ||
           (colormapName.contains("CALIPSO-Depolarization") && !(variableName.contains("_depolarization")))          ) {

        // Nope. Bail out
        QMessageBox* msgBox = new QMessageBox();
        msgBox->QDialog::setWindowTitle("RSIG3D warning");
        msgBox->setText(QString("The AQI and CALIPSO specialty color maps can only be used with matching source data."));
        msgBox->setIcon(QMessageBox::Information);
        msgBox->setStandardButtons(QMessageBox::Ok);
        msgBox->setDefaultButton(QMessageBox::Ok);
        msgBox->exec();

        // return the vis state to use the previous color map
        setVisWidgetState(visObj);
        return;
      }

      // extract the lookup table corresponding to the selected variable from the combo box
      QString curLut = curName;
      // convert name to lut (we either have polygon, point, or vector - just convert all)
      curLut.replace(QString(".polygon"), QString(".lut"));
      curLut.replace(QString(".point"), QString(".lut"));
      curLut.replace(QString(".vector"), QString(".lut"));

      LookupTableWidget *lut = vislist->getLutByName(curLut);

      if (lut != NULL) {
        lut->setMattLookupTable(index, (float)visObj->minval(), (float)visObj->maxval());

        QStringList mySlantText; // default is blank
        int myTickStep = 2; //default

        if ( ( (variableName.contains("aqs_pm25") && aggregate.contains("daily"))    || // aggregated or hourly
                variableName.contains("aqs_ozone")                                   || // aggregated or hourly
               (variableName.contains("aqs_co")   && aggregate.contains("daily"))    || // aggregated
                variableName.contains("aqs_no2")                                     || // hourly
                variableName.contains("aqs_so2")                                     || // hourly
                variableName.contains("airnow_ozone")                                || // hourly (because no aggregation for airnow)
                variableName.contains("airnow_no2")                                  || // hourly
                variableName.contains("airnow_so2")                                  || // hourly
                variableName.contains("_o3")                                           ) && colormapName.contains("AQI") ) {
          QStringList airnowCategories; airnowCategories << "Good" << "Moderate" <<"Sens Grp" << "Unhealthy" << "Very unhealthy" << "Hazardous";
          mySlantText = airnowCategories;
          myTickStep = 1;
        }

        lut->setSlantText(mySlantText);
        lut->setTickStep(myTickStep);
        lut->genTexture();
      }

      visObj->setCurColormapIndex(index);
      Q_EMIT requestRedraw();
    }
  }
}



/*!
 \fn void ControlDockWidget::onShowLUTsChanged(int val)

 Enable/disable dispaly of the lookup table area.
 */
void ControlDockWidget::onShowLUTsChanged(int val) {
  RSIGState *gState = RSIGStateSingleton::getInstance();
  gState->mShowLUTs = bool(val>0);
  Q_EMIT requestRedraw();
}



/*!
 \fn void ControlDockWidget::onShowNavigatorChanged(int val) 
 
 Enable/disable display of the MarbleNavigator widget.
 */
void ControlDockWidget::onShowNavigatorChanged(int val) {
  RSIGState *gState = RSIGStateSingleton::getInstance();
  gState->mShowNavigator = bool(val>0);
  Q_EMIT requestRedraw();
}



void ControlDockWidget::onShowDataProbeChanged(int val) {
  RSIGState *gState = RSIGStateSingleton::getInstance();
  gState->mShowDataProbe = bool(val>0);
  Q_EMIT requestRedraw();
}



/*!
 \fn void ControlDockWidget::onWriteToFileClicked()
 
 Set the name of the file to be saved.
 */
void ControlDockWidget::onWriteToFileClicked() {
  const QString fname =
    QFileDialog::getExistingDirectory( this, "Select directory",
                                       QDir::currentPath(),
                                       QFileDialog::ShowDirsOnly );

  if (fname != "") {
    ui->dirnameLineEdit->setText(fname + "/");
  }
}



/*!
 \fn void ControlDockWidget::onSaveFileClicked()
 
 Save a file using the output option, can be image or date formats.
 */

void ControlDockWidget::onSaveTypeClicked() {
  VisOptions* vOptions = VisOptionsSingleton::getInstance();

  if (ui->scenarioRadioButton->isChecked()) {
    ui->filenameLineEdit->setEnabled(true);
    ui->filenameLineEdit->setText(vOptions->getFilenameScenario());
    ui->filenameLineEdit->show();
    ui->filename_generated->hide();
  } else if (ui->currentpngRadioButton->isChecked()) {
    ui->filenameLineEdit->setEnabled(false);
    ui->filenameLineEdit->hide();
    ui->filename_generated->show();
    QString curFilename = ui->filenameLineEdit->text();

    if (curFilename == "" || curFilename.contains("EPA-RSIG-Snapshot_") || curFilename == "RSIG-snapshot") {
      generateDefaultPngFilename();
      ui->filenameLineEdit->setText(vOptions->getFilenamePngDefault());
      ui->filename_generated->setText(vOptions->getFilenamePngDefault());
    } else {
      generateDefaultPngFilename();
      ui->filenameLineEdit->setText(vOptions->getFilenamePngDefault());
      ui->filename_generated->setText(vOptions->getFilenamePngDefault());
    }

    ui->filenameLineEdit->hide();
    ui->filename_generated->show();
  } else if (ui->allpngsRadioButton->isChecked()) {
    vOptions->setFilenamePngDefault("RSIG-snapshot");
    ui->filenameLineEdit->setText(vOptions->getFilenamePngDefault());
    ui->filenameLineEdit->hide();
    ui->filename_generated->show();
    ui->filename_generated->setText("generated");
  } else if (ui->mpegRadioButton->isChecked()) {
    vOptions->setFilenamePngDefault("RSIG-snapshot");
    ui->filenameLineEdit->setText(vOptions->getFilenamePngDefault());
    ui->filenameLineEdit->hide();
    ui->filename_generated->show();
    ui->filename_generated->setText("generated");
  } else if (ui->screenRecordRadioButton->isChecked()) {
    vOptions->setFilenamePngDefault("RSIG-snapshot");
    ui->filenameLineEdit->setText(vOptions->getFilenamePngDefault());
    ui->filenameLineEdit->hide();
    ui->filename_generated->show();
    ui->filename_generated->setText("generated");
  } else {
    ui->filenameLineEdit->hide();
    ui->filename_generated->show();
    ui->filename_generated->setText("generated");    
  }
  Q_EMIT requestRedraw();
}



void ControlDockWidget::onSaveFilenameChanged() {
  VisOptions* vOptions = VisOptionsSingleton::getInstance();
  QString filename =  ui->filenameLineEdit->text();
  
  if (ui->scenarioRadioButton->isChecked()) {
    vOptions->setFilenameScenario(filename);
  } else if (ui->currentpngRadioButton->isChecked()) {
    vOptions->setFilenamePng(filename);
  } else if (ui->allpngsRadioButton->isChecked()) {
    vOptions->setFilenamePng(filename);
  } else if (ui->mpegRadioButton->isChecked()) {
    vOptions->setFilenamePng(filename);
  } else if (ui->screenRecordRadioButton->isChecked()) {
    vOptions->setFilenamePng(filename);
  }
}


  

/*!
 \fn void ControlDockWidget::onStateTimeOut()
 
 Timer signal handler for state saving timer. 
 The reason for using a timer in saving states is that QT has a bug when closing its widgets. 
 If the widget is visible, when we want to save it just before the exit signal, its state turns 
 into invisible, which is incorrect for the saving.
 */
void ControlDockWidget::onStateTimeOut() {
  int c = this->isVisible();

  if (c==0) {
    c=-1;
  }

  m_visible += c;

  if (m_visible>2) {
    m_visible=2;
  }

  if (m_visible<0) {
    m_visible=0;
  }
}



/*!
 \fn void ControlDockWidget::onClearLog()
 
 Clear output text in the text output region.
 */
void ControlDockWidget::onClearLog() {
  ui->textBrowser->setHtml(QString());
}



/*!
 \fn void ControlDockWidget::onSaveVideoToggled(bool flag)
 
 Handling signal for the saveVideo button. Performs the save video action.
 */
void ControlDockWidget::onSaveVideoToggled(bool toggleFlag) {
  VisOptions* vOptions = VisOptionsSingleton::getInstance();
  QString origSaveDir = vOptions->getSaveDir();
  QString origFilenamePng = vOptions->getFilenamePng();
  QString basenameTemp = "RSIG-Snapshot";
  //setup file name for temporary image frames
  const QString directoryNameTemp = QDir::homePath() + "/.rsig/";
  QFileInfo infoTemp(directoryNameTemp);
  const QString pathTemp = infoTemp.absolutePath();          

  if (toggleFlag) {
    // If toggled on, start the snapshot timer. This only happens when Record Screen is pressed
    const QString directoryName = ui->dirnameLineEdit->text();
    QFileInfo info(directoryName);
    QDir dir = info.dir();

    if (!dir.exists()) {
      const QString msg = "Directory "+ dir.absolutePath()+ " created.";
      this->postMessage(msg, RSIGState::WARNING_TEXT);
      dir.mkdir(dir.absolutePath());
    }

    // hack the save directory and filename to redirect temporary files corresponding to image frames
    vOptions->setSaveDir(directoryNameTemp);
    vOptions->setFilenamePng(basenameTemp);

    // delete existing temporary files
    QDir tempdir(pathTemp, basenameTemp + "*.png");

    for (int ind=0; ind<tempdir.entryList().length(); ind++) {
      tempdir.remove(tempdir.entryList()[ind]);
    }

    //ui->savevideoPushButton->setText("Stop Recording");
    ui->savefilePushButton->setText("Stop Recording");
    Q_EMIT requestInitSnapshotForVideo(directoryNameTemp);
    m_snapshotTimer.start(0);
  } else {
    // If toggled off, stop the timer as assemble the video. This happens when:
    // a) Record Screen is toggled off OR
    // b) Save File is pressed when MPEG Visualization format is selected

    ui->savevideoPushButton->setText("Record Screen");

    if (m_snapshotTimer.isActive()) {
      m_snapshotTimer.stop();
    }

    // sleep to let last file be written
    usleep(1000000); // microseconds

    // unhack the save directory and filename to redirect temporary files corresponding to image frames
    vOptions->setSaveDir(origSaveDir);
    vOptions->setFilenamePng(origFilenamePng);
    ui->savevideoPushButton->setEnabled(false);
    ui->savefilePushButton->setEnabled(false);
    //setup file name for output animation
    const QString directoryName = ui->dirnameLineEdit->text();
    QFileInfo info(directoryName);
    const QString path = info.absolutePath();
    QString basename;

    if (ui->savefilePushButton->text().compare("Save") == 0) {
      basename = "RSIG-animation"; // for MPEG
    } else {
      basename = "RSIG-screencapture"; // for screen capture
    }

    const QString outputname = path + "/" + basename + ".mp4";
    QFileInfo outfileInfo(outputname);

    if (outfileInfo.exists()) {
      QFile::remove(outputname);
    }

    QString filenameTemp =  basenameTemp + "-%04d.png";
    const QString templatename = pathTemp + "/" + filenameTemp;

    //run the process
    QProcess& _FFMPEG = m_ffmpeg;
    const QString pathedApplication( RSIGState::getProgramDirectory() );
    const QString _process(pathedApplication + "/ffmpeg");
    QStringList _paramList;
    _paramList.clear();
    _paramList << "-y"
               << "-f" << "image2"
               << "-r" << "3"
               << "-i" << templatename
               << "-vcodec" << "mpeg4"
               << "-pix_fmt" << "yuv420p"
               << "-qscale:v" << "1"
               << outputname;

    QString cmdline = QString("Run: ") +  _process;

    for (int i=0; i<_paramList.size(); i++) {
      cmdline += " ";
      cmdline += _paramList[i];
    }

    postMessage(cmdline, RSIGState::NORMAL_TEXT);
    postMessage(QString("Saving video file: ") + outputname, RSIGState::NORMAL_TEXT);
    update();
    _FFMPEG.start(_process, _paramList);
  }
}



/*!
 \fn void ControlDockWidget::onFFMPEGFinished(int exitCode, QProcess::ExitStatus exitStatus)
 
 Handling signal for FFMPEG process finishing.
 */
void ControlDockWidget::onFFMPEGFinished(int exitCode, QProcess::ExitStatus) {

  if (exitCode != 0) {
    const QString msg = "ffmpeg has finished with exit code " + QString().number(exitCode) + ".";
    postMessage(msg, RSIGState::WARNING_TEXT);
  }

  ui->savevideoPushButton->setEnabled(true);
  ui->savefilePushButton->setEnabled(true);
}



/*!
 \fn void ControlDockWidget::onFFMPEGError(QProcess::ProcessError errorid)
 
 Handling signal for FFMPEG process error. 
 */
void ControlDockWidget::onFFMPEGError(QProcess::ProcessError errorid) {
  QString errormsg;

  switch (errorid) {
    case QProcess::FailedToStart:
      errormsg = "QProcess::FailedToStart";
      break;
    case QProcess::Crashed:
      errormsg = "QProcess::Crashed";
      break;
    case QProcess::Timedout:
      errormsg = "QProcess::Timedout";
      break;
    case QProcess::ReadError:
      errormsg = "QProcess::ReadError";
      break;
    case QProcess::WriteError:
      errormsg = "QProcess::WriteError";
      break;
    case QProcess::UnknownError:
    default:
      errormsg = "QProcess::UnknownError";
      break;
  };

  const QString msg = "ffmpeg error: " + errormsg;
  postMessage(msg, RSIGState::ERROR_TEXT);
  m_ffmpeg.kill();
  ui->savevideoPushButton->setEnabled(true);
  ui->savefilePushButton->setEnabled(true);
}



/*!
 \fn void ControlDockWidget::onUpdateRsigFinished(int exitCode, QProcess::ExitStatus exitStatus)

 Event processing for finishing updating the RSIG3D package. 
 It handles the signal sent from the updating process.
 */
void ControlDockWidget::onUpdateRsigFinished(int exitCode, QProcess::ExitStatus) {
  const QString msg = "RSIG has finished updating, exit code " + QString().number(exitCode) + ".";
  postMessage(msg, RSIGState::WARNING_TEXT);
  ui->checknowPushButton->setEnabled(true);
  QMessageBox msgBox;
  msgBox.setText(QString("RSIG3D has been updated"));
  msgBox.setIcon(QMessageBox::Information);
  msgBox.setInformativeText("Do you want to restart?");
  msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
  msgBox.setDefaultButton(QMessageBox::Ok);
  const int ret = msgBox.exec();

  if (ret == QMessageBox::Ok) {
    const QString pathedApplication( RSIGState::getProgramDirectory() );
    QString rsigUrl = QString("file://") + pathedApplication + "/RSIG3D";
    qDebug() << rsigUrl;
    QDesktopServices::openUrl(rsigUrl);
    qApp->exit();
  }
}



/*!
 \fn void ControlDockWidget::onUpdateRsigError(QProcess::ProcessError errorid)
 
 Error handling when RSIG3D meets errors when updating itself.
 */
void ControlDockWidget::onUpdateRsigError(QProcess::ProcessError errorid) {
  QString errormsg;

  switch (errorid) {
    case QProcess::FailedToStart:
      errormsg = "QProcess::FailedToStart";
      break;
    case QProcess::Crashed:
      errormsg = "QProcess::Crashed";
      break;
    case QProcess::Timedout:
      errormsg = "QProcess::Timedout";
      break;
    case QProcess::ReadError:
      errormsg = "QProcess::ReadError";
      break;
    case QProcess::WriteError:
      errormsg = "QProcess::WriteError";
      break;
    case QProcess::UnknownError:
    default:
      errormsg = "QProcess::UnknownError";
      break;
  };

  const QString msg = "RSIG3D updator error: " + errormsg;
  postMessage(msg, RSIGState::ERROR_TEXT);
  m_updator.kill();
  ui->checknowPushButton->setEnabled(true);
}



bool ControlDockWidget::onShowDataUsePolicy(int activeFlag) {
  QFile dataPolicyFile(":RSIG3D_DataUsePolicy.txt");
  dataPolicyFile.open(QIODevice::ReadOnly | QIODevice::Text);
  QTextStream inFile(&dataPolicyFile);
  QString dataPolicy = inFile.readAll();

  DataUsePolicy policyDialog;

  if (activeFlag) {
    policyDialog.toggleAcceptButton(1);
    policyDialog.toggleCancelButton(1);
    policyDialog.toggleOkButton(0);
  } else {
    policyDialog.toggleAcceptButton(0);
    policyDialog.toggleCancelButton(0);
    policyDialog.toggleOkButton(1);
  }

  policyDialog.setText(dataPolicy);
  const int selection = policyDialog.exec(); // Show & block.
  bool return_value = true; //default - agreement was accepted

  if (activeFlag == 1) {

    if (selection == 1) { //Accept
      // make sure directory is there
      QDir homeDir = QDir(QDir::homePath());
      homeDir.mkpath(".rsig");
      QString fileToWrite =  QDir::homePath() + "/.rsig/RSIG3D_DataUsePolicy.txt";      

      // copy official file to user's rsig directory

      if (QFile::exists(fileToWrite)) {
        QFile tempFile;
        tempFile.setFileName(fileToWrite);
        tempFile.setPermissions(QFile::WriteOwner);
        tempFile.remove();
      }

      QFile::copy(":RSIG3D_DataUsePolicy.txt", fileToWrite);
    } else {
      //const bool closed = msgBox.close();
      return_value = false; // agreement was not accepted
    }
  }

  return return_value;
}



void ControlDockWidget::onCheckDataUsePolicyAccepted() {
  // if activeFlag == false, pressing OK or Cancel is a no-op

  QFile acceptedDataPolicyFile(QDir::homePath() + "/.rsig/RSIG3D_DataUsePolicy.txt");
  QFile officialDataPolicyFile(":RSIG3D_DataUsePolicy.txt");

  bool accepted;

  //if (!acceptedDataPolicyFile.exists()) {
  if (QFileInfo(acceptedDataPolicyFile).size() != QFileInfo(officialDataPolicyFile).size()) {
    accepted = this->onShowDataUsePolicy(1);
  } else {
    qDebug() << "\nRSIG3D Data Policy Use Agreement was previously accepted.";
    qDebug() << "To view the policy, click RSIG Web -> Show Data Use Policy.\n";
    this->postMessage( "RSIG3D Data Policy Use Agreement was previously accepted.", RSIGState::NORMAL_TEXT );
    this->postMessage( "To view the policy, click RSIG Web -> Show Data Use Policy.", RSIGState::NORMAL_TEXT );
    accepted = true;
  }

  if (accepted == false) {
    qDebug() << "Data use agreement not accepted. RSIG3D exiting."; 
    exit(EXIT_FAILURE);
  }
}



void ControlDockWidget::initRSIG3DHelp(int forceShowFlag) {

  /*
   * Here we want this pop-up dialog to NOT BLOCK user interactions with the
   * other application windows.
   * To achieve this:
   * 1. Declare a dynamically allocated object (with new).
   * 2. Mark that object to be deleted upon close.
   * 3. Call show() (instead of exec() which blocks).
   */

  RSIG3DHelp* helpDialog = new RSIG3DHelp(0);
  QFile prefFile(helpDialog->getPreferenceFilename());
  helpDialog->cleanUp();
  
  if (! prefFile.exists() || forceShowFlag == 1) {
    QFile rsig3dHelpFile(":RSIG3D_Help.txt");
    rsig3dHelpFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream inFile(&rsig3dHelpFile);
    QString helpText = inFile.readAll();
    helpDialog->setText(helpText);
    helpDialog->setAttribute(Qt::WA_DeleteOnClose);
    helpDialog->show();
  }
}


void ControlDockWidget::handleUpdate() {
  setRSIGServerHost(); // Query webservice for RSIG3D version.
  QMessageBox msgBox;
  ui->checknowPushButton->setEnabled(false);
  const QString msg0("Checking RSIG3D version...");
  this->postMessage(msg0, RSIGState::WARNING_TEXT);
  qDebug() << msg0;
  int remoteVersion = 0;
  const bool newVersionAvailable = checkRsig3dUpdate( remoteVersion );

  if ( remoteVersion == 0 ) { // This indicates failure to query webservice!
    const char* const messageFormat =
      "Cannot connect to RSIG server.\n"
      "Ask your system administrator to\n"
      "check your computer Internet connection and\n"
      "network/VPN/security configuration\n"
      "to ensure you have permission to run programs that launch\n"
      "curl with HTTPS GET queries to\n"
      "'%s'.\n"
      "Without webservice access you cannot retrieve data.\n"
      "You may reload previously saved data using 'Data->Scenario->Restore'.";
    char messageText[ 512 ] = "";
    memset( messageText, 0, sizeof messageText );
    snprintf( messageText, sizeof messageText / sizeof (char),
              messageFormat, RSIGWebservice() );
    msgBox.setText( messageText );
    msgBox.QDialog::setWindowTitle("RSIG3D ");
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.exec();
    return;
  }

  if ( newVersionAvailable ) {
    const QString remoteVersionString = QString().number( remoteVersion );
    msgBox.setText(QString("There is a newer version of RSIG3D available: ") + remoteVersionString + " (YYYYMMDD)");
    msgBox.QDialog::setWindowTitle("RSIG3D updater");
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setInformativeText("Do you want to download the newest version?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);
    const int selection = msgBox.exec();

    if (selection == QMessageBox::Ok) {
      ui->exitPushButton->setEnabled(false);
      QString message("Downloading RSIG3D from...");
      this->postMessage(message, RSIGState::WARNING_TEXT);
      qDebug() << message;
      const QString downloadedZipFile = QDir::homePath() + "/RSIG3D.zip";
      const bool ok = downloadUpdate( downloadedZipFile );

      if ( ok ) {
        QMessageBox msgBox2;
        msgBox2.setText(QString("Once RSIG3D closes you will need to unzip and install the update, which was saved as:\n\n" + downloadedZipFile ));
        msgBox2.QDialog::setWindowTitle("RSIG3D updater");
        msgBox2.setIcon(QMessageBox::Information);
        msgBox2.setStandardButtons(QMessageBox::Ok);
        msgBox2.setDefaultButton(QMessageBox::Ok);
        message = "Downloaded new RSIG3D zip file to:";
        this->postMessage(message, RSIGState::WARNING_TEXT);
        message = downloadedZipFile;
        this->postMessage(message, RSIGState::WARNING_TEXT);
      } else {
        message = "Download RSIG3D failed.";
        this->postMessage(message, RSIGState::ERROR_TEXT);
        qDebug() << message;
      }

      ui->exitPushButton->setEnabled(true);
    }
  } else {
    const QString msg1("RSIG3D is up to date.");
    this->postMessage(msg1, RSIGState::WARNING_TEXT);
    qDebug() << msg1;
  }
  
  ui->checknowPushButton->setEnabled(true);
}



void ControlDockWidget::onCheckUpdateClicked() {
  handleUpdate();
}


void ControlDockWidget::enableGetDataButton() {
  // called from HttpDownloadDialog if purpleair or tempo key was not entered
  ui->getdataPushButton->setEnabled(true);
}


/*!
 \fn void ControlDockWidget::onDownloadWorkFinished(QObject *points) 
 
 Async download handler for data set download.
 */
void ControlDockWidget::onDownloadWorkFinished(QObject *data) {

  //qDebug() << "oldSelectedDatasource=" << oldSelectedDatasource;

  mutex.lock();
  VisOptions* vOptions = VisOptionsSingleton::getInstance();
  vOptions->setDownloadsOutstanding(vOptions->getDownloadsOutstanding() - 1);
  
  if ( vOptions->getDownloadsOutstanding() == 0 ) {

    /*
     * HACK for ceilometer data:
     * There are no more asynchronus downloads pending so
     * check if any downloaded data sets are ceilometer and
     * if so, check if any of them are mixing_layer_height and
     * if not then load/download mixing_layer_height now
     * so it is ready to overlay on ceilometer pop-up plots.
     */

    checkAndLoadCeilometerMixingLayerHeightDataset();
    ui->getdataPushButton->setEnabled(true);
  }

  mutex.unlock();

  if (data == NULL) {
    return;
  }

  GeoDataObject* dataObj = qobject_cast<GeoDataObject*>(data);

  if (dataObj != NULL) {
    
    if (dataObj->model() == NULL) {
      this->postMessage("Network data: "+dataObj->objectName()+" failed.", RSIGState::WARNING_TEXT);
      dataObj->deleteLater();      
      return;
    }

    /*
     * If the GeoDataObject was created in a different thread,
     * replace it with a new copy in this thread and transfer the
     * model/dataset pointer. Be sure to zero the model/dataset pointer in the
     * old object before marking it for deletion later!
     */

    if (dataObj->thread() != this->thread()) {
      QRectF dataRequestBoundingBox = dataObj->getDataRequestBoundingBox();
      GeoDataObject* newDataObj = new GeoDataObject( 0, dataObj->model(), &dataRequestBoundingBox );
      newDataObj->setObjectName(dataObj->objectName());
      dataObj->setModel(NULL);
      dataObj->deleteLater();
      dataObj = newDataObj;
    }

    //get min/max range.
    dataObj->computeMinmaxVal();
    
    // Append GeoDataObject onto the GeoDataList:

    GeoDataList* datalist = GeoDataListSingleton::getInstance();
    datalist->push_back(dataObj);

    connect(dataObj, SIGNAL(valueChanged()), this, SIGNAL(requestRedraw()));
    
    // Create a default LookupTableWidget and append it onto the VisDataList:

    VisDataList* vislist = VisDataListSingleton::getInstance();
    LookupTableWidget* lut = dataObj->createDefaultLookupTable();
    
    if (lut != NULL) {
      vislist->push_back(lut);
      connect(lut, SIGNAL(valueChanged()), this, SIGNAL(requestRedraw()));
    }
    
    // Create a default VisDataObject and append it onto the VisDataList:

    VisDataObject* visObj = dataObj->createDefaultVisualizationObject();

    if (visObj != NULL) {
      vislist->push_back(visObj);

      // populate the Visualization combo box
      ui->vizVarCombobox->addItem(visObj->objectName());
      ui->vizVarCombobox->setCurrentIndex( ui->vizVarCombobox->count()-1);

      if (visObj->objectName().contains("True Color") == false) {
        ui->vizColormapCombobox->setEnabled(true);
      }

      // populate panel display probe menus
      QStringList allowedProbeSources;
      allowedProbeSources << "airnow" << "aqs" << "purpleair" << "metar" << "hms" << "uvnet"; // UGLY: hourly_pandora handled seperately below

      if (visObj->objectName().contains(".point")) {
        QString topLevelSourceName    = visObj->objectName().split("_")[0];
        QString secondLevelSourceName = visObj->objectName().split("_")[1];

        if (allowedProbeSources.contains(topLevelSourceName)) {
          ui->panelDisplaySource1->addItem(visObj->objectName().remove(".point"));
          setPanelDisplaySource1Selection(visObj->objectName().remove(".point"));
          this->onPanelDisplaySource1Changed(0);
        } else if (topLevelSourceName == "hourly" && secondLevelSourceName == "pandora") {
          ui->panelDisplaySource1->addItem(visObj->objectName().remove(".point"));
          setPanelDisplaySource1Selection(visObj->objectName().remove(".point"));
          this->onPanelDisplaySource1Changed(0);
        }
      }

      // populate panel display target menu
      if (visObj->objectName().contains(".polygon")) {
        ui->panelDisplaySource2->addItem(visObj->objectName().remove(".polygon"));
        setPanelDisplaySource2Selection(visObj->objectName().remove(".polygon"));
      }

      Vector2f minmax = dataObj->minmaxVal();

      // set default lookuptable for variable
      if (lut != NULL) {
        // get currently selected colormap index
        int index = 0; // default

        // override index for certain variables
        if (visObj->objectName().contains("calipso") && visObj->objectName().toLower().contains("backscatter")) {
          //index = 7; // backscatter
          index = emvl::getRGBColormapIndex("CALIPSO-Backscatter");
          visObj->setMinval(0.0);
          visObj->setMaxval(0.1);
        } else if (visObj->objectName().contains("calipso") && visObj->objectName().toLower().contains("depolarization")) {
          //index = 8; //depolarization
          index = emvl::getRGBColormapIndex("CALIPSO-Depolarization");
          visObj->setMinval(-1.0);
          visObj->setMaxval(1.0);
        } else if (visObj->objectName().toLower().contains("aod") || visObj->objectName().toLower().contains("optical_depth")) {
          //index = 9; // aod
          index = emvl::getRGBColormapIndex("Blue-red");
        } else if (visObj->objectName().toLower().contains("hms_smoke")) {
          index = emvl::getRGBColormapIndex("Smoke Plume");
        }

        // update vis object with colormap index
        visObj->setCurColormapIndex(index);

        lut->setMattLookupTable(index, (float)minmax.x, (float)minmax.y);
        lut->genTexture();
      }
      
      const bool isGrid = dataObj->model()->isStructuredGrid();

      if ( isGrid ) {
      
        // set default threshold to middle value
        visObj->setThresholdSliderValue(49);

        // set showThreshold to false
        visObj->setShowThreshold(false);

        // set CMAQ slider maxes
        visObj->setMaxCMAQRow(dataObj->rows());
        visObj->setMaxCMAQCol(dataObj->columns());
        visObj->setMaxCMAQLayer(dataObj->layers());

        // set default CMAQ slider values
        visObj->setCMAQRow(0);  // 0 because the show state is false below
        visObj->setCMAQCol(0);  // 0 because the show state is false below
        visObj->setCMAQLayer(1);
        
        // CMAQ show state
        visObj->setShowCMAQRow(false);
        visObj->setShowCMAQCol(false);
        visObj->setShowCMAQLayer(true);
      } else { // NON CMAQ variables
        // set default threshold to middle value
        visObj->setThresholdSliderValue(0);
      }

      // reset the "data default" checkbox to true and disable the spinboxes
      ui->vizDataDefault->setChecked(1);    
      ui->vizMinSpinbox->setEnabled(false);
      ui->vizMaxSpinbox->setEnabled(false);
      

      // turn visibility on
      visObj->setShowObject(true);
      //ui->vizCheckBox->setChecked(1);
      
      // set min/max in vis object
      visObj->setMinval(minmax.x);
      visObj->setMaxval(minmax.y);
      visObj->setLastMinval(minmax.x);
      visObj->setLastMaxval(minmax.y);

      connect(visObj, SIGNAL(valueChanged()), this, SIGNAL(requestRedraw()));
      
      // restore visObj properties if possible to preserve coherence of the user experience
      QString thisVisObjName = visObj->objectName().toLower();

      // apply data preferences
      bool usedDataPreference = false;

      if (ui->chkUseDataPrefs->isChecked()) {
        qDebug() << "restoring properties";
        DataPreferences* const dataPreferences = DataPreferencesSingleton::getInstance();
        QMap<QString, DataPreferences::DataProperties> dplt = dataPreferences->getDataPrefsLookupTable();
        // Example of looking up a key
        //QString searchKey = "tempo.l2.no2.vertical_column_sum";
        QString searchKey = thisVisObjName.replace(".point", "").replace(".polygon", "").replace(".vector", "");

        if ( dplt.contains(searchKey) ) {
          DataPreferences::DataProperties foundItem =  dplt.value(searchKey);
          qDebug() << "Data preference found:" << searchKey;
          qDebug() << "  min:" << foundItem.min;
          qDebug() << "  max:" << foundItem.max;
          qDebug() << "  colormap:" << foundItem.colormap;
          
          visObj->setMinval(foundItem.min);
          visObj->setMaxval(foundItem.max);
          visObj->setLastMinval(foundItem.min);
          visObj->setLastMaxval(foundItem.max);
          int myindex = emvl::getRGBColormapIndex(foundItem.colormap);
          visObj->setCurColormapIndex(myindex);
          visObj->setDefaultRangeChecked(false);
          //onColormapIndexChanged(myindex);
          lut->setMattLookupTable(myindex, (float)foundItem.min, (float)foundItem.max);
          lut->genTexture();
          usedDataPreference = true;
          //setVisWidgetState(visObj);

        } else {
          qDebug() << searchKey << "data preference not found!";
        }
      }


      if (usedDataPreference == false) {
        int arraySize = sizeof(oldVisProperties)/sizeof(oldVisProperties[0]);

        for (int ind=0; ind<arraySize; ind++) {
          
          if (thisVisObjName.toLower().compare(oldVisProperties[ind].name().toLower()) == 0 &&
              (thisVisObjName.contains(".point") ||
               thisVisObjName.contains(".polygon") ||
               thisVisObjName.contains(".vector")) ) {
            
            //qDebug() << thisVisObjName << oldVisProperties[ind].name() << thisVisObjName.compare(oldVisProperties[ind].name()) << oldVisProperties[ind].minval() << oldVisProperties[ind].maxval();
            
            
            visObj->setShowObject(oldVisProperties[ind].showObject());
            visObj->setMinval(oldVisProperties[ind].minval());
            visObj->setMaxval(oldVisProperties[ind].maxval());
            
            if (oldVisProperties[ind].minValChangedThisSession()) {
              visObj->setLastMinval(oldVisProperties[ind].lastMinval());
            }
            
            if (oldVisProperties[ind].maxValChangedThisSession()) {
              visObj->setLastMaxval(oldVisProperties[ind].lastMaxval());
            }
            
            visObj->setCurColormapIndex(oldVisProperties[ind].curColormapIndex());
            visObj->setDefaultRangeChecked(oldVisProperties[ind].isDefaultRangeChecked());
            visObj->setAlpha(oldVisProperties[ind].alpha());
            visObj->setShowCellBoundary(oldVisProperties[ind].showCellBoundary());
            visObj->setShowThreshold(oldVisProperties[ind].ShowThreshold());
            visObj->setThresholdSliderValue(oldVisProperties[ind].thresholdSliderValue());
            
            onColormapIndexChanged(oldVisProperties[ind].curColormapIndex());
            onThresholdSliderChanged(oldVisProperties[ind].thresholdSliderValue());
          }
        }

        setVisWidgetState(visObj);
      }
    }
    
    this->postMessage("Network data: "+dataObj->objectName()+" loaded.", RSIGState::NORMAL_TEXT);
  }

  Q_EMIT requestRedraw();
}



void ControlDockWidget::onSaveWorkFinished(QObject*) {
}
