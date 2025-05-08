/******************************************************************************
 PURPOSE: datausepolicy.h - A class for user acceptance of the data use policy.
 
 NOTES:   .
 
 HISTORY: 2015/11/05, freeman.matt@epa.gov, Created.
 STATUS:  unreviewed, untested.
 ******************************************************************************/

#ifndef DATAUSEPOLICY_H_
#define DATAUSEPOLICY_H_

#include <QDialog>


class Ui_DataUsePolicy;

class DataUsePolicy: public QDialog {
  Q_OBJECT 

 private:
  static int buttonPress; 
  
 public:
  
  DataUsePolicy(QWidget *parent=0);
  virtual ~DataUsePolicy();

  void setText(const QString text);
  
  void toggleAcceptButton(int toggle);
  void toggleCancelButton(int toggle);
  void toggleOkButton(int toggle);
  
  static void setButtonPress(int thisButtonPress);
  static int getButtonPress();
  
  protected Q_SLOTS:
  void onOk();
  
  void onAccept();
  
  void onCancel();
  
 private:
  Q_DISABLE_COPY(DataUsePolicy);
  
  Ui_DataUsePolicy* ui;
  
};


#endif //DATAUSEPOLICY_H_
