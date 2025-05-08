/******************************************************************************
PURPOSE: RegridParmsData.h - A class for RegridParmsData.

NOTES:

HISTORY: 2012/11/12, zhang.nan@epa.gov, Created.
STATUS:  unreviewed, untested.
******************************************************************************/

#ifndef REGRIDPARMSDIALOG_H_
#define REGRIDPARMSDIALOG_H_

#include <QDialog>
#include "ui_regridparms.h"

struct RegridParmsData;

class RegridParmsDialog: public QDialog {
  Q_OBJECT

public:
  RegridParmsDialog(QWidget *parent=0);

  virtual ~RegridParmsDialog();

  static RegridParmsData* getRegridParmsData();
  void showRegridElements();


protected Q_SLOTS:
  void onOK();

  void onCancel();

  void onReset();

  void onCmaqGridIndexChanged(int);

  void onCmaqGridTypeChanged(int);

  void onUserOverrideChanged();

private:
  Q_DISABLE_COPY(RegridParmsDialog);
  Ui_RegridParmsDialog* ui;
};


#endif //REGRIDPARMSDIALOG_H_
