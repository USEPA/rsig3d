
/******************************************************************************
PURPOSE: FileOrientedDownloader.cpp - Implements a dialog for downloading sets
        of files from rsigserver.

NOTES:

HISTORY: 2015-12-29 freeman.matt@epa.gov
******************************************************************************/

#include <QDebug>
#include <QListWidget>
#include <QUrl>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QCryptographicHash>

#include <Dataset.h>              // For emvl::Dataset.
#include <DatasetManager.h>       // For emvl::DatasetManager.
#include "GetData.h"
#include "FileOrientedDownloaderWorker.h"

void FileOrientedDownloaderWorker::run() {
  emvl::DatasetManager * datasetManager = getDatasetManager();
  const QString rsigserver( emvl::DatasetManager::webservice().c_str() );

  // first pass: figure out how many files there are (for the progress bar)
  long numFiles = 0;

  for (int row = 0; row < listWidget->count(); row++) {
    QListWidgetItem *item = listWidget->item(row);

    if (item->checkState() == Qt::Checked) {
      QString thisProduct = item->text().mid(9);
      QFile filesFile(":files.txt");
      filesFile.open(QIODevice::ReadOnly | QIODevice::Text);
      QTextStream filesinFile(&filesFile);

      // loop through list of files

      while (!filesinFile.atEnd()) {
        QString thisFilesLine = filesinFile.readLine();

        if (thisFilesLine.contains(thisProduct)) {
          numFiles += 1;
        }
      }
    }
  }

  emit numFilesSignal(numFiles);

  // second pass: process files
  // loop through products

  for (int row = 0; row < listWidget->count(); row++) {
    QListWidgetItem *item = listWidget->item(row);
    // process item

    if (item->checkState() == Qt::Checked) {
      QString thisProduct = item->text().mid(9);
      // read files.txt
      QFile filesFile(":files.txt");
      filesFile.open(QIODevice::ReadOnly | QIODevice::Text);
      QTextStream filesinFile(&filesFile);
   
      // loop through list of files
      while (!filesinFile.atEnd()) {

        if (abortFlag) {
          qDebug() << "download aborted";
          break;
        }

        QString thisFilesLine = filesinFile.readLine();
     
        if (thisFilesLine.contains(thisProduct)) {
         QStringList parse = thisFilesLine.split(" ");
         QString officialChecksum = parse[0];
         QString remotePathPlusFile = parse[1];
         QUrl url = QUrl(rsigserver + remotePathPlusFile);
         QFileInfo localQFileInfo = QFileInfo(baseDir + "/" + remotePathPlusFile);
         QDir dir(localQFileInfo.absolutePath());

          if (!dir.exists()) {
            dir.mkpath(localQFileInfo.absolutePath());
          }

          QFile* file = new QFile(localQFileInfo.absoluteFilePath());
       
          // check if local file exists and if it has the correct checksum. if not, download it.
          bool downloadFlag = true; // default

          if (file->exists()) {
            // get checksum
            file->open(QIODevice::ReadOnly);
            QCryptographicHash hash(QCryptographicHash::Md5);

            while ( ! file->atEnd() ) {
              hash.addData(file->read(8192));
            }

            file->close();

            if (officialChecksum == hash.result().toHex()) {
              downloadFlag = false;
            }
          }


          if (downloadFlag) {
            datasetManager->retrieveFile( remotePathPlusFile.toStdString().c_str(), localQFileInfo.absoluteFilePath().toStdString().c_str() );
            emit fileDownloaded();
          } else {
            emit fileDownloaded();
          }
        }
      }

       filesFile.close();
    }
  }

  emit finished(abortFlag);
}



void FileOrientedDownloaderWorker::setQListWidget(const QListWidget * widget) {
  listWidget = widget;
}



void FileOrientedDownloaderWorker::setBaseDir(const QString dir) {
  baseDir = dir;
}



void FileOrientedDownloaderWorker::setAbortFlag(bool flag) {
  abortFlag = flag;
}
