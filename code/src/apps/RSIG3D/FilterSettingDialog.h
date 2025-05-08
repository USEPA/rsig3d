/******************************************************************************
PURPOSE: filtersetting.h - A class for running http protocol get command.

NOTES:   .

HISTORY: 2012/11/28, zhang.nan@epa.gov, Created.
STATUS:  unreviewed, untested.
******************************************************************************/

#ifndef FILTERSETTING_H_
#define FILTERSETTING_H_

#include <QDialog>
#include <Dataset.h>

struct FilterSettingData;
class Ui_FilterSettingDialog;
  
class FilterSettingDialog: public QDialog {
  Q_OBJECT

public:
  FilterSettingDialog(QWidget *parent=0);

  virtual ~FilterSettingDialog();

  static FilterSettingData* getFilterSetting();

                                                        
public Q_SLOTS:
  void onOK();
  void onCancel();
  void onReset();
  void writePurpleairApiKeyFile();
  void writeTEMPOKeyFile();
  void writeOAQPSKeyFile();
  bool validatePurpleairKey();
  bool validateTEMPOKey();
  bool validateOAQPSKey();

private:
  Q_DISABLE_COPY(FilterSettingDialog);
  Ui_FilterSettingDialog* ui;
};


#endif //FILTERSETTING_H_
