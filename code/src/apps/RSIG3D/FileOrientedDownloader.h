

#ifndef FILEORIENTEDDOWNLOADER_H_
#define FILEORIENTEDDOWNLOADER_H_

#include <QDialog>
#include <QUrl>
#include <QFile>
#include <QFileInfo>
#include <QDir>

#include "ui_filedownloader.h"

class FileOrientedDownloader: public QDialog {
  Q_OBJECT
    
public:
  FileOrientedDownloader(QWidget *parent=0);
  
  virtual ~FileOrientedDownloader();
  

private slots:
  
public:
  void updateRequestedGB();
  void beginDownload();
  void setNumFilesToDownload(long num);
  void setPercentComplete(float percent);
  void setEnableProductList(bool flag);

protected Q_SLOTS:
  void onDirButtonClicked();
  
  void onDownloadClicked();
  
  void onCancelClicked();
  
  void onListWidgetClicked();

  void onDownloadFinished(bool);

  void onFileDownloaded();

  void onNumFilesChanged(long);


private:
  Q_DISABLE_COPY(FileOrientedDownloader);
  
  Ui_FileOrientedDownloader* ui;


  QUrl url;
  bool httpRequestAborted;
  qint64 fileSize;
  long numFilesToDownload;
  long numFilesRetrieved;

};


#endif //FILEORIENTEDDOWNLOADER_H_
