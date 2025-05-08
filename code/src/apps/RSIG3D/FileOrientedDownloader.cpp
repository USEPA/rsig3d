
/******************************************************************************
PURPOSE: FileOrientedDownloader.cpp - Implements a dialog for downloading sets
        of files from rsigserver.

NOTES:

HISTORY: 2015-12-29 freeman.matt@epa.gov
******************************************************************************/

#include <iostream>
#include <QDebug>
#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>

#include <Dataset.h>              // For emvl::Dataset.
#include <DatasetManager.h>       // For emvl::DatasetManager.

#include "FileOrientedDownloader.h"
#include "FileOrientedDownloaderWorker.h"
#include "ui_filedownloader.h"
#include "GetData.h"

#ifdef _WIN32
#include "windows.h"
#elif defined( __linux__ )
#include <sys/statfs.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <sys/mount.h>
#else
#include <sys/stat.h>
#include <sys/param.h>
#include <sys/mount.h>
#endif

static const QString message(
  "This file downloader is offered as a convenience to RSIG3D users. "
  "Due to the size of the datasets offered, "
  "the downloads may take a long time to complete. "
  "Some products require a significant amount of disk space."
  "<br><br>"
  "For UNIX users, a csh script with the same functionality "
  "is available from:"
  "<br><br>"
  "<a href=\""
  "https://www.epa.gov/rsig/"
  "downloadabledatafiles.html\">"
  "https://www.epa.gov/rsig/"
  "downloadabledatafiles.html</a>" );

static double freeSpaceGB, requiredSpaceGB, totalSpaceGB;

static FileOrientedDownloaderWorker fileOrientedDownloaderWorker;



bool GetFreeTotalSpace(const QString& sDirPath, double& fTotal, double& fFree) {
  double fGB = 1024 * 1024 * 1024;

#ifdef _WIN32

  QString sCurDir = QDir::current().absolutePath();
  QDir::setCurrent(sDirPath);

  ULARGE_INTEGER free,total;

  bool bRes = ::GetDiskFreeSpaceExA( 0 , &free , &total , NULL );

  if ( !bRes )
      return false;

  QDir::setCurrent( sCurDir );

  fFree = static_cast<__int64>(free.QuadPart)  / fGB;
  fTotal = static_cast<__int64>(total.QuadPart)  / fGB;

#else

  struct stat stst;
  struct statfs stfs;

  if ( ::stat(sDirPath.toLocal8Bit(),&stst) == -1 )
      return false;

  if ( ::statfs(sDirPath.toLocal8Bit(),&stfs) == -1 )
      return false;

  fFree = stfs.f_bavail * ( stst.st_blksize / fGB );
  fTotal = stfs.f_blocks * ( stst.st_blksize / fGB );

#endif

  return true;
}



FileOrientedDownloader::FileOrientedDownloader(QWidget *parent): QDialog(parent) {
  QMessageBox* msgBox   = new QMessageBox();
  msgBox->setWindowTitle("RSIG3D File Downloader");
  
  msgBox->setText( message );
  msgBox->setWindowFlags(Qt::WindowStaysOnTopHint);
  msgBox->show();

  ui = new Ui_FileOrientedDownloader;
  ui->setupUi(this);

  // initialization
  QString homepath = QDir::homePath();
  ui->fileDownloaderDirLineEdit->setText(homepath);

  ui->fileDownloaderAvailableSpace->setSegmentStyle(QLCDNumber::Flat);
  ui->fileDownloaderRequiredSpace->setSegmentStyle(QLCDNumber::Flat);

  QPalette palette = QPalette();
  palette.setColor(QPalette::WindowText, Qt::black);
  ui->fileDownloaderAvailableSpace->setPalette(palette);

  palette.setColor(QPalette::WindowText, Qt::darkGreen);
  palette.setColor(QPalette::Light, Qt::darkGreen);
  palette.setColor(QPalette::Dark, Qt::darkGreen);
  ui->fileDownloaderRequiredSpace->setPalette(palette);

  GetFreeTotalSpace(homepath, totalSpaceGB, freeSpaceGB);
  ui->fileDownloaderAvailableSpace->display(freeSpaceGB);

  QFont font("Courier", 11, QFont::Normal);
  QFile productsFile(":products.txt");
  productsFile.open(QIODevice::ReadOnly | QIODevice::Text);
  QTextStream inFile(&productsFile);

  while (!inFile.atEnd()) {
    QString thisLine = inFile.readLine();
    // parse bytes and product name
    QString thisBytes   = thisLine.mid(0,4);
    QString thisProduct = thisLine.mid(4);
    QString thisItem = "[" + thisBytes +"GB] " + thisProduct;
    QListWidgetItem *listItem = new QListWidgetItem(thisItem, ui->fileDownloaderListWidget);
    Qt::ItemFlags flags = listItem->flags();
    flags &= ~Qt::ItemIsSelectable;
    listItem->setFlags(flags);
    listItem->setCheckState(Qt::Unchecked);
    listItem->setFont(font);
    ui->fileDownloaderListWidget->addItem(listItem);
  }

  // slots
  connect(ui->fileDownloaderDirButton, SIGNAL(clicked()), this, SLOT(onDirButtonClicked()));
  connect(ui->fileDownloaderDownloadButton, SIGNAL(clicked()), this, SLOT(onDownloadClicked()));
  connect(ui->fileDownloaderCancelButton, SIGNAL(clicked()), this, SLOT(onCancelClicked()));
  connect(ui->fileDownloaderListWidget, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(onListWidgetClicked()));

}



FileOrientedDownloader::~FileOrientedDownloader() {
  delete ui;
  ui = 0;
}



void FileOrientedDownloader::onDirButtonClicked() {
  const QString fname =
    QFileDialog::getExistingDirectory(this, "Select directory",
                                      QDir::currentPath(),
                                      QFileDialog::ShowDirsOnly);

  if (fname != "") {
    ui->fileDownloaderDirLineEdit->setText(fname);
  }
}



void FileOrientedDownloader::onDownloadClicked() {
  ui->fileDownloaderDownloadButton->setEnabled(false);
  beginDownload();
}



void FileOrientedDownloader::onCancelClicked() {
  httpRequestAborted = true;
  ui->fileDownloaderDownloadButton->setEnabled(true);
  setEnableProductList(true);    
  fileOrientedDownloaderWorker.setAbortFlag(true);
  fileOrientedDownloaderWorker.wait();
  this->close();
}



void FileOrientedDownloader::onListWidgetClicked() {
  updateRequestedGB();
}



void FileOrientedDownloader::updateRequestedGB() {

  // Determine which products are checked and update the indicator
  
  double totGB = 0.0;

  for (int row = 0; row < ui->fileDownloaderListWidget->count(); row++) {
    QListWidgetItem *item = ui->fileDownloaderListWidget->item(row);
    // process item

    if (item->checkState() == Qt::Checked) {
      QString thisGBString = item->text().mid(1,3);
      totGB += thisGBString.toDouble();
    }
  }

  requiredSpaceGB = totGB;
  ui->fileDownloaderRequiredSpace->display(requiredSpaceGB);

  QPalette palette = QPalette();

  if (requiredSpaceGB < freeSpaceGB) {
    palette.setColor(QPalette::WindowText, Qt::darkGreen);
    palette.setColor(QPalette::Light, Qt::darkGreen);
    palette.setColor(QPalette::Dark, Qt::darkGreen);

  } else {
    palette.setColor(QPalette::WindowText, Qt::red);
    palette.setColor(QPalette::Light, Qt::red);
    palette.setColor(QPalette::Dark, Qt::red);
  }

  ui->fileDownloaderRequiredSpace->setPalette(palette);
}



void FileOrientedDownloader::setEnableProductList(bool flag) {
  ui->fileDownloaderListWidget->setEnabled(flag);
}



void FileOrientedDownloader::beginDownload() {

  httpRequestAborted = false;

  if (requiredSpaceGB < freeSpaceGB) {
    numFilesRetrieved = 0; 
    setEnableProductList(false);
    ui->fileDownloaderProgressBar->setMinimum(0);   //   0 percent min
    ui->fileDownloaderProgressBar->setMaximum(100); // 100 percent max
    fileOrientedDownloaderWorker.setQListWidget(ui->fileDownloaderListWidget);
    fileOrientedDownloaderWorker.setBaseDir(ui->fileDownloaderDirLineEdit->text());
    fileOrientedDownloaderWorker.setAbortFlag(false);
    connect(&fileOrientedDownloaderWorker, SIGNAL(finished(bool)), this, SLOT(onDownloadFinished(bool))); 
    connect(&fileOrientedDownloaderWorker, SIGNAL(fileDownloaded()), this, SLOT(onFileDownloaded())); 
    connect(&fileOrientedDownloaderWorker, SIGNAL(numFilesSignal(long)), this, SLOT(onNumFilesChanged(long))); 
    fileOrientedDownloaderWorker.start();

    if (numFilesRetrieved == 0) {
      ui->fileDownloaderProgressBar->setValue(5); // so there will be some progress shown initially
    }

  } else {
    QString msgString = QString("Download aborted: Not enough disk space.\n\nRequired: %1 GB\n\nAvailable: %2 GB").arg(QString::number(requiredSpaceGB), QString::number(freeSpaceGB));
    QMessageBox* msgBox   = new QMessageBox();
    msgBox->setWindowTitle("RSIG3D File Downloader Warning");
    msgBox->setText(msgString);
    msgBox->setWindowFlags(Qt::WindowStaysOnTopHint);
    msgBox->show();
  }
}



void FileOrientedDownloader::onFileDownloaded() {
  numFilesRetrieved += 1;

  if (numFilesToDownload > 0) {
    float percent_complete = float(numFilesRetrieved) / float(numFilesToDownload) * 100;

    if (percent_complete > 1) {
      ui->fileDownloaderProgressBar->setValue(percent_complete);
    } else {
      ui->fileDownloaderProgressBar->setValue(percent_complete); // so we can see something on the bar
    }
  }
}



void FileOrientedDownloader::onDownloadFinished(bool finishFlag) {
  ui->fileDownloaderDownloadButton->setEnabled(true);
  setEnableProductList(true);

  if (finishFlag == false) { // not aborted
    QMessageBox* msgBox   = new QMessageBox();
    msgBox->setWindowTitle("RSIG3D File Downloads Complete");
    msgBox->setText("The file downloads are complete!!");
    msgBox->setWindowFlags(Qt::WindowStaysOnTopHint);
    msgBox->show();
  }

  onCancelClicked();
}



void FileOrientedDownloader::onNumFilesChanged(long numFiles) {
  numFilesToDownload = numFiles;
}



