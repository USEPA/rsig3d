/******************************************************************************
PURPOSE: RSIG3DHelp.cpp - Implements the help dialog.

NOTES:

HISTORY: 2022/01/27, freeman.matt@epa.gov, Created.
******************************************************************************/

#include <QDebug>
#include <QDir>
#include <QFile>
#include "RSIG3DHelp.h"
#include "ui_rsig3dhelp.h"
#include "DatasetManager.h" // For remoteVersion().

int btnPress;

void RSIG3DHelp::setText(const QString text) {
  ui->rsig3dHelpTextBrowser->setText(text);
}


//======================================================================

/*!
 \class RSIG3DHelp
 
 \brief A dialog widget class for displaying help.
 */

RSIG3DHelp::RSIG3DHelp(QWidget *parent) : QDialog(parent), ui(0) {
  btnPress = 0;     //default
  dontShowAgain = 0; //default
  ui = new Ui_RSIG3DHelp;
  ui->setupUi(this);
  ui->groupBox->setStyleSheet("QGroupBox {border:0;}");
  
  //setUiValues(ui, staticData);
  
  connect(ui->rsig3dHelpOkButton, SIGNAL(clicked()), this, SLOT(onOk()));
  connect(ui->dontShowAgain,      SIGNAL(stateChanged(int)), this, SLOT(onDontShowAgainClicked(int)));

  const int version = emvl::DatasetManager::remoteVersion();
  this->setPreferenceFilename(QDir::homePath() + "/.rsig/nohelp_" + QString::number(version) + ".txt");

  QFile prefFile(this->getPreferenceFilename());

  if (prefFile.exists()) {
    ui->dontShowAgain->setCheckState(Qt::Checked);
  }
}




RSIG3DHelp::~RSIG3DHelp() {
  delete ui;
}



void RSIG3DHelp::cleanUp() {
  // clean up old preference files
  QDir directory(QDir::homePath() + "/.rsig");
  QStringList files = directory.entryList(QStringList() << "nohelp_*.txt", QDir::Files);
  QFile fileToRemove;

  foreach(QString filename, files) {

    if (! preferenceFilename.contains(filename)) {
      fileToRemove.setFileName(QDir::homePath() + "/.rsig/" + filename);
      fileToRemove.remove();
    }
  }
}



void RSIG3DHelp::setButtonPress(int thisButtonPress) {
  btnPress = thisButtonPress;
}



int RSIG3DHelp::getButtonPress() {
  return btnPress;
}



void RSIG3DHelp::setPreferenceFilename(QString filename) {
  preferenceFilename = filename;
}



QString RSIG3DHelp::getPreferenceFilename() {
  return preferenceFilename;
}



void RSIG3DHelp::onDontShowAgainClicked(int val) {
  dontShowAgain = val;
}



void RSIG3DHelp::onOk() {

  QFile file(this->getPreferenceFilename());

  if (dontShowAgain == Qt::Checked) {
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    file.close();
  } else {
    file.remove();
  }
  
  setButtonPress(0);
  this->close();
}





