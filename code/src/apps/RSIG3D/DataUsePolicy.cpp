/******************************************************************************
PURPOSE: DataUsePolicy.cpp - Implements the data use policy dialog.

NOTES:

HISTORY: 2015/11/05, freeman.matt@epa.gov, Created.
******************************************************************************/

#include <iostream>
#include <QDebug>
#include <QSslError>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "DataUsePolicy.h"
#include "ui_datausepolicy.h"


int DataUsePolicy::buttonPress = 0; // static class attribute.



void DataUsePolicy::setText(const QString text) {
  ui->dataPolicyTextBrowser->setText(text);
}



void  DataUsePolicy::toggleAcceptButton(int toggle) {

  if (toggle) {
    ui->dataPolicyAcceptButton->show();
  } else {
    ui->dataPolicyAcceptButton->hide();
  }
}



void  DataUsePolicy::toggleCancelButton(int toggle) {

  if (toggle) {
    ui->dataPolicyCancelButton->show();
  } else {
    ui->dataPolicyCancelButton->hide();
  }
}



void  DataUsePolicy::toggleOkButton(int toggle) {

  if (toggle) {
    ui->dataPolicyOkButton->show();
  } else {
    ui->dataPolicyOkButton->hide();
  }
}



/*!
 \class DataUsePolicy
 
 \brief A dialog widget class for displaying data use policy.
 */

DataUsePolicy::DataUsePolicy(QWidget *parent) : QDialog(parent), ui(0) {
  buttonPress = 0; //default
  ui = new Ui_DataUsePolicy;
  ui->setupUi(this);
  ui->groupBox->setStyleSheet("QGroupBox {border:0;}");
  connect(ui->dataPolicyAcceptButton, SIGNAL(clicked()), this, SLOT(onAccept()));
  connect(ui->dataPolicyCancelButton, SIGNAL(clicked()), this, SLOT(onCancel()));
  connect(ui->dataPolicyOkButton, SIGNAL(clicked()), this, SLOT(onOk()));
}



DataUsePolicy::~DataUsePolicy() {
  delete ui;
  ui = 0;
}



void DataUsePolicy::setButtonPress(int thisButtonPress) {
  buttonPress = thisButtonPress;
}



int DataUsePolicy::getButtonPress() {
  return buttonPress;
}


void DataUsePolicy::onOk() {
  setButtonPress(0);
  this->close();
}



void DataUsePolicy::onAccept() {
  setButtonPress(1);
  this->accept();
}



void DataUsePolicy::onCancel() {
  setButtonPress(2);
  this->close();
}



