
#ifndef MINWIN_H
#define MINWIN_H

/******************************************************************************
PURPOSE: MainWin.h - defines MainWin, the main window class

NOTES:   based on QT's QMainWindow class
 
HISTORY: 2012-11-11 zhang.nan@epa.gov
         Evolved by freeman.matt@epa.gov
STATUS: unreviewed untested
*****************************************************************************/

#include <QRectF>
#include <QMainWindow>
#include <QDir>
#include <QTimer>
#include <QDomDocument>

class QResizeEvent;
class QGraphicsScene;
class QDockWidget;
class QObject;
class GraphicsView;
class QToolBar;
class QPrinter;
class ControlDockWidget;

#include "RSIGGlobal.h"
#include "GraphicsView.h"
#include "RSIGScene.h"


namespace PropertyEditor{
  class PropertyEdit;
}


class MainWin : public QMainWindow  {

private:

  Q_OBJECT
  Q_DISABLE_COPY(MainWin);

  static MainWin* singleton; // Singleton Design Pattern.

  // Attributes:

  GraphicsView* m_pGLUIWin;
  QToolBar* m_pToolBar;
  ControlDockWidget* m_controlWidget;
  QDockWidget* m_objectBrowser;
  PropertyEditor::PropertyEdit* m_objectPropEdit;
  QDockWidget* m_visBrowser;
  PropertyEditor::PropertyEdit* m_visPropEdit;
  QString mStateFileName;
  bool mStopPlay;
  int mPreScreenPosX;
  int mPreScreenPosY;
  QTimer mTimer;
  int snapshotCounter_;
  QString snapshotFormat_;
  QString snapshotFileName_;
  QDir snapshotSaveDir_;

public:

  static MainWin& getInstance() { return *singleton; } // Get Singleton.

  MainWin(QWidget *parent, QGraphicsScene * scene);
  ~MainWin();

  void centerAndResize();

  // Save/restore state to/from file:

  void setStateFileName(const QString& name) { mStateFileName = name; };
  QString stateFileName();
  QDomElement domElement(const QString& name, QDomDocument& document) const;
  virtual bool restoreStateFromFile(QString fileName);
  virtual void saveStateToFile(QString dirName, QString fileName, QString id);  
  
  ControlDockWidget * controlWidget() { return m_controlWidget; }
  GraphicsView * graphicsView() { return m_pGLUIWin; }

protected:

  virtual void closeEvent( QCloseEvent* );
  virtual void keyPressEvent( QKeyEvent* e );
  virtual void resizeEvent( QResizeEvent* event );
  
private:

  void initFromDOMElement(const QDomElement& element); // Restore state?
  void updateObjectList();

  // Saving screen images/movies:

  const QString& snapshotFormat() const { return snapshotFormat_; };
  int snapshotCounter() const { return snapshotCounter_; };
  const QString& snapshotFileName() const { return snapshotFileName_; };

  void saveSnapshot(bool automatic=false, bool overwrite=true);
  void initializeSnapshotVariables(const QString &dirName = "");
  void initializeSnapshotVariablesForVideo(const QString &dirName = "");
  void setSnapshotFormat(const QString& format) { snapshotFormat_ = format; };
  void setSnapshotCounter(int counter) { snapshotCounter_ = counter; };
  void setSnapshotFileName(const QString& name) { snapshotFileName_ = name; }
  void saveImage(bool automatic, bool overwrite);  
    
private Q_SLOTS:

  void refreshGLWindow();
  void onTimeOut();
  void onRequestSnapshot();
  void onRequestSingleSnapshot();
  void onRequestInitSnapshot(const QString& snapshotname);
  void onRequestInitSnapshotForVideo(const QString& snapshotname);
};


#endif /* MINWIN_H */



