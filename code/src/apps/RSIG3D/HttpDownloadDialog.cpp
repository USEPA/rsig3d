/******************************************************************************
PURPOSE: HttpDownloadDialog.C - Implements HttpDownloadDialog class.

NOTES: Implemented using QT's QNetworkAccessManager class.

HISTORY: 2012/11/12, zhang.nan@epa.gov, Created.
******************************************************************************/

#include <stdlib.h> // For strtod().
#include <string.h> // For strncpy().

#if _MSC_VER
#define NOMINMAX
#define snprintf _snprintf
#include <windows.h>
#endif

#include <iostream>

#include <QThread>
#include <QDebug>
#include <QSslError>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QString>

#include <Utilities.h>       // For parseValues().
#include <QueryParameters.h> // For class emvl::QueryParameters.
#include <Dataset.h>         // For class emvl::Dataset.

#include "HttpDownloadDialog.h"
#include "GetData.h"
#include "GeoDataObject.h"
#include "ui_downloaddialog.h"
#include "VisOptions.h"
#include "RegridParmsDialog.h"
#include "RegridParmsData.h"
#include "FilterSettingDialog.h"
#include "FilterSettingData.h"
#include "MainWin.h"
#include "ControlDockWidget.h"



#ifdef _MSC_VER
inline void zsleep2(unsigned milliseconds) {
  Sleep(milliseconds);
}
#define SNPRINTF _snprintf_s
#else
#include <unistd.h>
inline void zsleep2(unsigned milliseconds) {
  usleep(milliseconds * 1000); // takes microseconds
}
#define SNPRINTF snprintf
#endif


//===================================================================



void DataReader::setInput(QNetworkReply * reply) {
  mNetReply = reply;
}



DataReader::DataReader(): QObject(), mNetReply(0) {
}



DataReader::~DataReader() {
}



void DataReader::start() {
  QObject* result = 0;

  if (mNetReply) {
    mNetReply = 0;
  }

  Q_EMIT finished(result);
  this->thread()->quit();
}



//===================================================================

LibCurlDataGet::LibCurlDataGet() : QObject(0) {
  mDataName[0] = 0;
  mStartTime[0] = 0;
  mEndTime[0] = 0;
  mROI = QRectF(0, 0, 0, 0);
  mRegrid = 0;
  mAggregate = 0;
  mLayer = 1;
  mSaveMetadataFlag = false;
}



LibCurlDataGet::~LibCurlDataGet() {
}


// Retrieve dataset to a file for this thread:

void LibCurlDataGet::start() {

  MainWin& mainWin = MainWin::getInstance();
  ControlDockWidget* const controlDockWidget = mainWin.controlWidget();

  // Get regrid parameters from GUI and parse and validate them:

  const RegridParmsData* const regridData = RegridParmsDialog::getRegridParmsData();
  const int selected_regridMethod = regridData->regridMethod;
  const int regridMethod =
    ! mRegrid? emvl::QueryParameters::REGRID_NONE
    : selected_regridMethod == 0 ? emvl::QueryParameters::REGRID_WEIGHTED
    : selected_regridMethod == 1 ? emvl::QueryParameters::REGRID_MEAN
    : selected_regridMethod == 2 ? emvl::QueryParameters::REGRID_NEAREST
    : emvl::QueryParameters::REGRID_NONE;
  const int regridAggregateMethod =
    regridMethod != emvl::QueryParameters::REGRID_NONE ?
      regridData->regridAggregation
    : emvl::QueryParameters::REGRID_AGGREGATE_NONE;

  // Parse VGLVLS string into an array of doubles:

  enum { MAXIMUM_LEVELS = emvl::CMAQGridParameters::MXLAYS3 + 1 };
  double vglvls[ MAXIMUM_LEVELS ];
  const int levels =
    parseValues( regridData->VGLVLS.c_str(), MAXIMUM_LEVELS, vglvls );
  bool ok =
    emvl::CMAQGridParameters::isValidParameters((int)regridData->NCOLS,
                                                (int)regridData->NROWS,
                                                levels - 1,
                                                (int)regridData->GDTYP,
                                                (double)regridData->RADIUS,
                                                (double)regridData->PALP,
                                                (double)regridData->PBET,
                                                (double)regridData->PGAM,
                                                (double)regridData->XCENT,
                                                (double)regridData->YCENT,
                                                (double)regridData->XORIG,
                                                (double)regridData->YORIG,
                                                (double)regridData->XCELL,
                                                (double)regridData->YCELL,
                                                (int)regridData->VGTYP,
                                                (double)regridData->VGTOP,
                                                vglvls,
                                                (double)regridData->G,
                                                (double)regridData->R,
                                                (double)regridData->A,
                                                (double)regridData->T0,
                                                (double)regridData->P0);

  if ( ! ok ) {
    controlDockWidget->postMessage( "Invalid CMAQ Grid Parameters.",
                                    RSIGState::NORMAL_TEXT );
    Q_EMIT finished(0); // kills the download popup
  } else {

    // Get filter parameters from GUI and parse and validate them:

    const FilterSettingData* const filterData =
      FilterSettingDialog::getFilterSetting();

    const std::string dataVersion =
      strstr( this->mDataName, "calipso.l1." ) ?
        filterData->CALIPSOL1DataVersion == "Latest" ?
          ""
          : filterData->CALIPSOL1DataVersion.toStdString()
      : strstr( this->mDataName, "calipso.l2" ) ?
        filterData->CALIPSOL2DataVersion == "Latest" ?
        ""
        : filterData->CALIPSOL2DataVersion.toStdString()
      : "";

    const double replacementHumidity =
      filterData->usePurpleAirHumidityReplacement ?
        filterData->PurpleAirReplacementHumidity
      : emvl::MISSING;

    const std::string key( filterData->PurpleAirAPIKey.toStdString() );
    const std::string TEMPOkey( filterData->TEMPOKey.toStdString() );
    const std::string OAQPSkey( filterData->OAQPSKey.toStdString() );
    const std::string qualityLevel( filterData->viirsNoaaQAFlag.toStdString());
    const std::string qualityLevel2( filterData->pandoraQAFlag.toStdString());
    const std::string TEMPOQualityLevel( filterData->TEMPOQAFlag.toStdString());
    const bool useGroundPixelRange =
      filterData->TropomiGroundPixelMinimum > 0 &&
      filterData->TropomiGroundPixelMinimum <
      filterData->TropomiGroundPixelMaximum;
    const int groundPixelMinimum =
      useGroundPixelRange ? filterData->TropomiGroundPixelMinimum
      : emvl::IMISSING;
    const int groundPixelMaximum =
      useGroundPixelRange ? filterData->TropomiGroundPixelMaximum
      : emvl::IMISSING;

    ok = emvl::DataFilterParameters::isValidParameters(
           filterData->CADmin,
           filterData->UncertaintyMax,
           filterData->AODmin,
           filterData->AODmax,
           filterData->AODSTDmin,
           filterData->AODSTDmax,
           filterData->CLSmin,
           filterData->CLSmax,
           filterData->CH1min,
           filterData->CH1max,
           filterData->SIGmin,
           filterData->SIGmax,
           filterData->SFCmin,
           filterData->SFCmax,
           filterData->MOSmin,
           filterData->MOSmax,
           filterData->SCAmin,
           filterData->SCAmax,
           filterData->TropomiQuality,
           filterData->TropomiMaxCloudFraction,
           groundPixelMinimum,
           groundPixelMaximum,
           filterData->TropomiAllowNegativeCounts,
           qualityLevel.c_str(),
           qualityLevel2.c_str(),
           TEMPOQualityLevel.c_str(),
           filterData->TEMPOMaxCloudFraction,
           filterData->TEMPOMaxSolarZenithAngle,
           filterData->TEMPOAllowNegativeCounts,
           filterData->OMIMaxCloudFraction,
           filterData->OMIMaxSolarZenithAngle,
           filterData->OMIAllowNegativeCounts,
           TEMPOkey,
           OAQPSkey,
           key,
           filterData->PurpleairLocationFlag,
           filterData->PurpleAirAcceptableChannelDifference,
           filterData->PurpleAirAcceptableChannelAgreement / 100.0,
           filterData->PurpleAirMinAggregationCountPercentage,
           replacementHumidity,
           dataVersion );

    if ( ! ok ) {
      controlDockWidget->postMessage( "Invalid Data Filter Parameters.",
                                      RSIGState::WARNING_TEXT );
      Q_EMIT finished(0); // kills the download popup
      
    } else if ( QString(this->mDataName).contains("purpleair") &&
                (filterData->PurpleAirAPIKey.length() == 0 ||
                 ! emvl::DatasetManager::isValidPurpleAirKey( key.c_str() )) ) {
      controlDockWidget->postMessage( "PurpleAir data requires a valid API READ key.\nTo obtain one, email contact@purpleair.com.\nOnce you have a key, you can enter it in the Data->Filter_parameters->PurpleAir_Filter dialog.",
                                      RSIGState::WARNING_TEXT );

      VisOptions* const vOptions = VisOptionsSingleton::getInstance();
      vOptions->setDownloadsOutstanding(vOptions->getDownloadsOutstanding() - 1);

      if (vOptions->getDownloadsOutstanding() == 0) {
        controlDockWidget->enableGetDataButton();
      }
      
      Q_EMIT finished(0); // kills the download popup

    } else {

      // Parameters are valid so create objects needed for download:
      
      const emvl::CMAQGridParameters
        gridParameters((int)regridData->NCOLS,
                      (int)regridData->NROWS,
                      levels - 1,
                      (int)regridData->GDTYP,
                      (double)regridData->RADIUS,
                      (double)regridData->PALP,
                      (double)regridData->PBET,
                      (double)regridData->PGAM,
                      (double)regridData->XCENT,
                      (double)regridData->YCENT,
                      (double)regridData->XORIG,
                      (double)regridData->YORIG,
                      (double)regridData->XCELL,
                      (double)regridData->YCELL,
                      (int)regridData->VGTYP,
                      (double)regridData->VGTOP,
                      vglvls,
                      (double)regridData->G,
                      (double)regridData->R,
                      (double)regridData->A,
                      (double)regridData->T0,
                      (double)regridData->P0);

      const emvl::DataFilterParameters
        filterParameters(filterData->CADmin,
                         filterData->UncertaintyMax,
                         filterData->AODmin,
                         filterData->AODmax,
                         filterData->AODSTDmin,
                         filterData->AODSTDmax,
                         filterData->CLSmin,
                         filterData->CLSmax,
                         filterData->CH1min,
                         filterData->CH1max,
                         filterData->SIGmin,
                         filterData->SIGmax,
                         filterData->SFCmin,
                         filterData->SFCmax,
                         filterData->MOSmin,
                         filterData->MOSmax,
                         filterData->SCAmin,
                         filterData->SCAmax,
                         filterData->TropomiQuality,
                         filterData->TropomiMaxCloudFraction,
                         groundPixelMinimum,
                         groundPixelMaximum,
                         filterData->TropomiAllowNegativeCounts,
                         qualityLevel.c_str(),
                         qualityLevel2.c_str(),
                         TEMPOQualityLevel.c_str(),
                         filterData->TEMPOMaxCloudFraction,
                         filterData->TEMPOMaxSolarZenithAngle,
                         filterData->TEMPOAllowNegativeCounts,
                         filterData->OMIMaxCloudFraction,
                         filterData->OMIMaxSolarZenithAngle,
                         filterData->OMIAllowNegativeCounts,
                         TEMPOkey,
                         OAQPSkey,
                         key,
                         filterData->PurpleairLocationFlag,
                         filterData->PurpleAirAcceptableChannelDifference,
                         filterData->PurpleAirAcceptableChannelAgreement / 100.0,
                         filterData->PurpleAirMinAggregationCountPercentage,
                         replacementHumidity,
                         dataVersion );

      try {
        VisOptions* const vOptions = VisOptionsSingleton::getInstance();
        const bool getMetadata = mSaveMetadataFlag;
        const int format = vOptions->getSaveFileFormatIndex();

        const std::string saveDir( vOptions->getSaveDir().toStdString() );
        const char* const saveDirectory =
          getMetadata || format == emvl::QueryParameters::ORIGINAL ?
            saveDir.c_str()
          : 0;

        const double west = mROI.x();
        const double south = mROI.y();
        const double east = west + mROI.width();
        const double north = south + mROI.height();

        emvl::Dataset* const data =
          downloadData( getMetadata,
                        true,
                        format,
                        saveDirectory,
                        this->mDataName,
                        mStartTime, mEndTime,
                        west, east, south, north,
                        mLayer,
                        mAggregate,
                        regridMethod,
                        regridAggregateMethod,
                        gridParameters,
                        filterParameters );

        if ( getMetadata || format == emvl::QueryParameters::ORIGINAL ) {
          Q_EMIT finished( 0 );
          QString message( "Saved " );

          if ( getMetadata ) {
            message.append( "metadata file for " );
          } else {
            message.append( "original file for " );
          }

          message.append( this->mDataName );
          controlDockWidget->postMessage( message, RSIGState::NORMAL_TEXT );

        } else {
          QRectF dataRequestBoundingBox;
          dataRequestBoundingBox.setLeft( west );
          dataRequestBoundingBox.setRight( east );
          dataRequestBoundingBox.setBottom( south );
          dataRequestBoundingBox.setTop( north );
          GeoDataObject* dataObj = new GeoDataObject(0, data, &dataRequestBoundingBox );
          Q_EMIT finished(dataObj);
        }
      } catch (...) {
        QString message( "Failed to download data " );
        message.append( this->mDataName );
        controlDockWidget->postMessage( message, RSIGState::WARNING_TEXT );
      }
    }
  }

  this->thread()->quit();
}



//===================================================================



static const int UI_MINHEIGHT = 210;
static const int UI_MAXHEIGHT = 420;

HttpDownloadDialog::HttpDownloadDialog(QWidget *parent): QDialog(parent)
    ,mManager(0)
    ,mReply(0)
    ,mLogVisible(false)
    ,mJobStarted(false)
    ,mThread() 
    ,mReader()
    ,mCurlGet()
    ,mRegrid(0)
{
  ui = new Ui_DownloadDialog;
  ui->setupUi(this);
  ui->closeonfinishCheckBox->setChecked(true);
  ui->logTextBrowser->hide();
  ui->okPushButton->setDisabled(true);
  ui->cancelPushButton->setDisabled(true);
  
  this->setMinimumHeight(UI_MINHEIGHT);
  this->setMaximumHeight(UI_MINHEIGHT);

  mManager = new QNetworkAccessManager(0);
  connect(mManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onHttpDownloadDialogFinished(QNetworkReply*)));
  
  connect(ui->showlogToolButton, SIGNAL(clicked()), this, SLOT(onShowlogClicked()));
  connect(ui->okPushButton, SIGNAL(clicked()), this, SLOT(onOK()));
  connect(ui->cancelPushButton, SIGNAL(clicked()), this, SLOT(onCancel()));
}



HttpDownloadDialog::~HttpDownloadDialog() {

  while (mThread.isRunning()) {
    zsleep2(100);
  }

  if (mReply) {

    if (mReply->isRunning()) {
      mReply->abort();
    }

    mReply->deleteLater();
    mReply = 0;
  }

  if (mManager) {
    mManager->deleteLater();
    mManager = 0;
  }

  delete ui;
}



void HttpDownloadDialog::keyPressEvent(QKeyEvent*) {
}



void HttpDownloadDialog::onHttpDownloadDialogFinished(QNetworkReply* reply) {

  if (reply->error() == QNetworkReply::NoError) {
    //Get the http status code
    int v = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    if (v >= 200 && v < 300) { // Success
      //Here we got the final reply
      ui->okPushButton->setDisabled(false);
      ui->cancelPushButton->setDisabled(true);

      const qint64 nbytes= reply->bytesAvailable();

      if (nbytes != 0) {
        QThread *thread0 = &mThread;
        DataReader *worker = &mReader;
        worker->setInput(reply);
        worker->moveToThread(thread0);
        connect(worker, SIGNAL(finished(QObject*)), this, SLOT(onDataReady(QObject*)));
        connect(thread0, SIGNAL(started()), worker, SLOT(start()));
        thread0->start();
      } else {
        std::cerr << "Insufficient bytes to read: "<< nbytes <<" bytes.\n";
        onDataReady(0);
      }
    } else {
      mReply = NULL;
      reply->deleteLater();

      if (v >= 300 && v < 400) { // Redirection
        // Get the redirection url
        QUrl newUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
        // Because the redirection url can be relative, we use the previous one to resolve it
        newUrl = reply->url().resolved(newUrl);

        QNetworkAccessManager* manager = reply->manager();
        QNetworkRequest redirection(newUrl);
        mReply = manager->get(redirection);
        mReply->setReadBufferSize(0);  //unlimited read buffer size

        connect(mReply, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(onDownloadProgress(qint64, qint64)));
      }
    }
  } else {// Error
    ui->logTextBrowser->append(reply->errorString());
  }
}



int HttpDownloadDialog::runLibCurlHttp() {
  //prepare input parameters
  LibCurlDataGet *worker = &mCurlGet;
  {
    QByteArray byteArray = mCoverage.toUtf8();
    const char* cStringDataName = byteArray.constData();
    const int N = LibCurlDataGet::DataNameLength;
    strncpy(worker->mDataName, cStringDataName, N);
    worker->mDataName[N] = 0;
  }

  QDateTime startDateTime;
  startDateTime.setDate(mDate);
  startDateTime.setTimeSpec(Qt::UTC);
  QDateTime endDateTime = startDateTime.addDays(mDays).addSecs(-1);
  endDateTime.setTimeSpec(Qt::UTC);

  QString strStartDT = startDateTime.toString(Qt::ISODate);
  QString strEndDT = endDateTime.toString(Qt::ISODate);

  {
    QByteArray byteArray = strStartDT.toUtf8();
    const char* cStringDataName = byteArray.constData();
    const int N = LibCurlDataGet::TimeStringLength;
    strncpy(worker->mStartTime, cStringDataName, N);
    worker->mStartTime[19] = '-';
    worker->mStartTime[20] = '0';
    worker->mStartTime[21] = '0';
    worker->mStartTime[22] = '0';
    worker->mStartTime[23] = '0';
    worker->mStartTime[24] = 0;
  }

  {
    QByteArray byteArray = strEndDT.toUtf8();
    const char* cStringDataName = byteArray.constData();
    const int N = LibCurlDataGet::TimeStringLength;
    strncpy(worker->mEndTime, cStringDataName, N);
    worker->mEndTime[19] = '-';
    worker->mEndTime[20] = '0';
    worker->mEndTime[21] = '0';
    worker->mEndTime[22] = '0';
    worker->mEndTime[23] = '0';
    worker->mEndTime[24] = 0;
  }

  worker->mROI = mROI;
  worker->mRegrid = mRegrid;
  worker->mLayer = mLayer;
  worker->mAggregate = mAggregate;
  worker->mSaveMetadataFlag = mSaveMetadataFlag;

  // In a different thread, run the download function

  QThread* thread0 = &mThread;
  worker->moveToThread(thread0);
  connect(thread0, SIGNAL(started()), worker, SLOT(start()));
  connect(worker, SIGNAL(finished(QObject*)), this, SLOT(onDataReady(QObject*)));
  thread0->start();

  return 1;
}



void HttpDownloadDialog::setAggregate(const int aggregate) {
  mAggregate = aggregate;
}



void HttpDownloadDialog::setRegrid(const int regrid) {
  mRegrid = regrid;
}



void HttpDownloadDialog::setRegionOfInterest(const QRectF& r) {
  mROI = r;
}



void HttpDownloadDialog::setDateAndDays(const QDate & date, int ndays) {
  mDate = date;
  mDays = ndays;
}



void HttpDownloadDialog::setCoverage(const QString& coveragename) {
  mCoverage = coveragename;
  ui->dataLabel->setText(mCoverage);
}



void HttpDownloadDialog::setSource(const QString& sourcename) {
  ui->sourceLabel->setText(sourcename);
}



void HttpDownloadDialog::setLayer(const int layer) {
  mLayer = layer;
}



void HttpDownloadDialog::setSaveMetadataFlag(const bool flag) {
  mSaveMetadataFlag = flag;
}



void HttpDownloadDialog::onError(QNetworkReply::NetworkError errorcode) {
  QString errstr("NetworkError code: ");
  errstr+=QString::number(errorcode);
  errstr+="\n";
  ui->logTextBrowser->append(errstr);
}



void HttpDownloadDialog::onSslErrors(const QList<QSslError>& errors) {
  QString errstr("QSslError: ");

  for (int i=0; i<errors.length(); i++) {
    //errstr += errors[i].errorString() + "\n";
  }

  ui->logTextBrowser->append(errstr);
}



void HttpDownloadDialog::onShowlogClicked() {
  mLogVisible = !mLogVisible;
  ui->logTextBrowser->setVisible(mLogVisible);

  if (mLogVisible) {
    this->setMinimumHeight(UI_MAXHEIGHT);
    this->setMaximumHeight(UI_MAXHEIGHT);
  } else {
    this->setMinimumHeight(UI_MINHEIGHT);
    this->setMaximumHeight(UI_MINHEIGHT);
  }
}



void HttpDownloadDialog::onDataReady(QObject *data) {

  if (data != NULL) {
    VisOptions* const vOptions = VisOptionsSingleton::getInstance();
    QStringList melist = vOptions->getDataSourceList();
    melist << mCoverage + "#" +  data->objectName(); // coverage (aka menu item name), hash delimiter, dataObject name
    vOptions->setDataSourceList(melist);
    
    Q_EMIT workFinished(data);
  }
  
  if (ui->closeonfinishCheckBox->isChecked()) {
    close();
  }
}



void HttpDownloadDialog::onOK() {
  this->close();
}



void HttpDownloadDialog::onCancel() {

  if (mReply && mReply->isRunning()) {
    mReply->abort();
  }

  Q_EMIT workAborted();
  this->close();
}

//overloaded show() function of the original QWidget::show() 
void HttpDownloadDialog::show() {
  QDialog::show();

  if (!mJobStarted) {
    mJobStarted=true;
    runLibCurlHttp();
    mJobStarted = false;
  }
}



void HttpDownloadDialog::onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal) {
  const int percent = int(100.0*bytesReceived / (bytesTotal+0.1) + 0.5);
  ui->progressBar->setValue(percent);
}



