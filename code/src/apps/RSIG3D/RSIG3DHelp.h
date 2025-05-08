/******************************************************************************
PURPOSE: RSIG3DHelp.h - A class for RSIG3D help.

NOTES:   .

HISTORY: 2022/01/27, freeman.matt@epa.gov, Created.
STATUS:  unreviewed, untested.
******************************************************************************/

#ifndef RSIG3DHELP_H_
#define RSIG3DHELP_H_

#include <QDialog>
#include <QString>

class Ui_RSIG3DHelp;

class RSIG3DHelp: public QDialog {
  Q_OBJECT 

  private:
  int btnPress; 
  int dontShowAgain;
  QString preferenceFilename;
  
  public:
  
  RSIG3DHelp(QWidget *parent=0);
  virtual ~RSIG3DHelp();
  
  void setText(const QString text);
  
  //void toggleAcceptButton(int toggle);
  //void toggleCancelButton(int toggle);
  //void toggleOkButton(int toggle);
  
  void setButtonPress(int thisButtonPress);
  int getButtonPress();
  
  void setPreferenceFilename(QString filename);
  QString getPreferenceFilename();

  void cleanUp();
                                 
  protected Q_SLOTS:
  void onOk();
  void onDontShowAgainClicked(int val);
  
 private:
  Q_DISABLE_COPY(RSIG3DHelp);
  
  Ui_RSIG3DHelp* ui;
  
};


#endif //RSIG3DHELP_H_
