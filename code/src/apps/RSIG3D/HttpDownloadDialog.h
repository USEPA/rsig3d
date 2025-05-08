/******************************************************************************
PURPOSE: HttpDownloadDialog.h - A class for running http protocol get command.

NOTES:   Implementation is based QT's QNetworkAccessManager.

HISTORY: 2012/11/12, zhang.nan@epa.gov, Created.
STATUS:  unreviewed, untested.
******************************************************************************/

#ifndef HTTPDOWNLOADDIALOG_H_
#define HTTPDOWNLOADDIALOG_H_

#include <QtGlobal> //qint64
#include <QList>
#include <QDialog>
#include <QThread>
#include <QDate>
#include <QSslError>
#include <QNetworkReply>

class QNetworkReply;
class QNetworkAccessManager;
class Ui_DownloadDialog;


class DataReader: public QObject {
  Q_OBJECT

public:
  DataReader();
  virtual ~DataReader();
  void setInput(QNetworkReply * reply);

Q_SIGNALS:
  void finished(QObject*);

public Q_SLOTS:
  void start();

private:
  QNetworkReply* mNetReply;
};


class LibCurlDataGet: public QObject {
  Q_OBJECT

public:
  LibCurlDataGet();
  virtual ~LibCurlDataGet();
  const static int DataNameLength = 511;
  const static int TimeStringLength = 31;

Q_SIGNALS:
  void finished(QObject * data);

public Q_SLOTS:
  void start();

public:
  char mDataName[DataNameLength+1];
  char mStartTime[TimeStringLength+1];
  char mEndTime[TimeStringLength+1];
  QRectF mROI;
  int mRegrid;
  int mLayer;
  int mAggregate;
 bool mSaveMetadataFlag;
};



class HttpDownloadDialog: public QDialog {
  Q_OBJECT

public:

  HttpDownloadDialog(QWidget *parent=0);

  virtual ~HttpDownloadDialog();

  void setLayer(const int layer);
  void setAggregate(const int aggregate);
  void setSaveMetadataFlag(const bool flag);
  void setSource(const QString& sourcename);
  void setCoverage(const QString& coveragename);
  void setRegionOfInterest(const QRectF& rectf);
  void setDateAndDays(const QDate & date, int ndays);
  void setRegrid(const int regrid);
  int runLibCurlHttp();

Q_SIGNALS:
  void workFinished(QObject* data);

  void workAborted();

public Q_SLOTS:
  virtual void show();  //overloaded show() function of the original QWidget::show()
  virtual void keyPressEvent(QKeyEvent * e);
  void onDataReady(QObject * data);

protected Q_SLOTS:
  void onHttpDownloadDialogFinished(QNetworkReply* reply);
  void onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);
  void onError(QNetworkReply::NetworkError errorcode);
  void onSslErrors(const QList<QSslError> &errors);
  void onShowlogClicked();
  void onOK();
  void onCancel();

protected:

private:
  Q_DISABLE_COPY(HttpDownloadDialog);

  QRectF mROI;
  QDate mDate;
  int mDays;
  Ui_DownloadDialog* ui;
  QNetworkAccessManager* mManager;
  QNetworkReply* mReply;
  bool mLogVisible;
  bool mJobStarted;
  QThread mThread;
  DataReader mReader;
  LibCurlDataGet mCurlGet;
  int mRegrid;
  int mLayer;
  int mAggregate;
  bool mSaveMetadataFlag;

 public:
  QString mCoverage;
};

#endif //HTTPDOWNLOADDIALOG_H_
