/******************************************************************************
PURPOSE: mainwin.C - The main window for RSIG3D application

NOTES:   derived from QMainWindow class

HISTORY: 2012-11-11 zhang.nan@epa.gov
         Evolved by freeman.matt@epa.gov
STATUS: unreviewed untested
******************************************************************************/

#if _MSC_VER
#define NOMINMAX
#define snprintf _snprintf
#include <windows.h>
#endif

#include <stdio.h>
#include <iostream>

#include <QtGui>
#include <QDockWidget>
#include <QTextStream>
#include <QMouseEvent>
#include <QMessageBox>
#include <QComboBox>
#include <QPixmap>
#include <QFont>

#include "RSIGGlobal.h" // For executableVersion().
#include "GeoDataPropertyEdit.h"    //class GeoDataPropertyEdit
#include "VisDataPropertyEdit.h"    //class VisDataPropertyEdit
#include "DomUtils.h"
#include "RSIGScene.h"
#include "GraphicsView.h"
#include "ControlDockWidget.h"
#include "GeoDataObject.h"
#include "GeoDataList.h"
#include "VisDataObject.h"
#include "VisDataList.h"
#include "VisOptions.h"
#include "GetData.h"

#include <images/filenew.xpm>
#include <images/fileopen.xpm>
#include <images/f_open.xpm>
#include <images/filesave.xpm>
#include <images/fileprint.xpm>

#include "MainWin.h"

#ifdef _WIN32
void zsleep(unsigned milliseconds) {
  Sleep(milliseconds);
}
#define SNPRINTF _snprintf_s
#else
#include <unistd.h>
void zsleep(unsigned milliseconds) {
  usleep(milliseconds * 1000); // takes microseconds
}
#define SNPRINTF snprintf
#endif


static QDir gOpenDir = QDir::homePath();
static QDir gSaveDir = QDir::homePath();
static QStringList gPropFields;
static QLineEdit* staticMessageLabel = NULL;


QString formatLonLat(double lon, double lat) {

  if (staticMessageLabel==NULL) {
    return QString("");
  }

  long bufsize = 1024;
  char buffer[bufsize];
  snprintf(buffer, bufsize, "Lon:%f, Lat:%f", lon, lat);
  const QString msg(buffer);
  return msg;
}



void displayProbe(QString probeMsg) {
  staticMessageLabel->setText(probeMsg);
}



void displayProbe2(QString) {
}



//==============================================================

class BugFixDockWidget: public QDockWidget {

private:
  QWidget* mProxy;
  QWidget* mRealWidget;

public:

  BugFixDockWidget(QWidget *parent = 0) : QDockWidget(parent) {
    mProxy = new QWidget;
    this->setWidget(mProxy);
  }
  
  QWidget *getMainWidget() {
    return mProxy;
  }
  
  void setRealWidget(QWidget* w) {
    mRealWidget = w;
  }
  
protected:

  void resizeEvent ( QResizeEvent * event ) {
    const QSize s = event->size();
    QDockWidget::resizeEvent(event);

    if (mProxy) {
      mProxy->resize(s);
    }

    if (mRealWidget) {
      mRealWidget->resize(s.width()-2, s.height());
    }
  }
};


//==============================================================


static ControlDockWidget* createControlDockWidget(MainWin *mainwin) {
  ControlDockWidget* dock = new ControlDockWidget(mainwin);
  dock->setFloating(false);
  mainwin->setDockOptions(QMainWindow::VerticalTabs);
  mainwin->addDockWidget(Qt::LeftDockWidgetArea, dock);
  return dock;
}



MainWin* MainWin::singleton; // Singleton Pattern static attribute.



/*!
 \class MainWin
 \brief The MainWin class is the main window class.

 \ingroup

 \reentrant

 A QT application usually contains a QApplication and a QMainWindow.
 The QApplication class is responsible for many application related stuff and the event loop.
 The QMainWindow is responsible for creating menu, toolbar, and main rendering window etc.
 */

MainWin::MainWin(QWidget *parent, QGraphicsScene * scene):
  QMainWindow(parent, 0)
  ,m_pToolBar(NULL)
  ,m_objectBrowser(NULL)
  ,m_objectPropEdit(NULL)
  ,m_visBrowser(NULL)
  ,m_visPropEdit(NULL)
  ,mStopPlay(false)
  ,mTimer()
{

  // set main font size for GUI
  QFont font = this->font();
  font.setPixelSize(12);
  this->setFont(font);
  
  this->setStateFileName(QDir::homePath()+"/.rsig3d.xml");

  this->initializeSnapshotVariables();

  //create the main rendering window
  // TEMP HACK try {
    m_pGLUIWin = new GraphicsView( this, scene ); //x
    setCentralWidget(m_pGLUIWin);
  // TEMP HACK } catch (...) {
  // TEMP HACK qDebug()<<"Error in creating rendering window, abort...";
  // TEMP HACK exit(1);
  // TEMP HACK }
  
  //create main control widget
  // TEMP HACK try {
    m_controlWidget = createControlDockWidget(this); //x
  // TEMP HACK } catch (...) {
  // TEMP HACK qDebug()<<"Error in creating rendering window, abort...";
  // TEMP HACK exit(1);
  // TEMP HACK }

  connect(m_controlWidget, SIGNAL(requestSnapshot()), this, SLOT(onRequestSnapshot()));
  connect(m_controlWidget, SIGNAL(requestSingleSnapshot()), this, SLOT(onRequestSingleSnapshot()));
  connect(m_controlWidget, SIGNAL(requestInitSnapshot(const QString&)), this, SLOT(onRequestInitSnapshot(const QString&)));
  connect(m_controlWidget, SIGNAL(requestInitSnapshotForVideo(const QString&)), this, SLOT(onRequestInitSnapshotForVideo(const QString&)));
  connect(m_controlWidget, SIGNAL(requestRedraw()), this, SLOT(refreshGLWindow()));
  connect(m_controlWidget, SIGNAL(mapServerChanged(bool, const char*)), m_pGLUIWin, SLOT(onMapServerChanged(bool, const char *)));
  connect(m_controlWidget, SIGNAL(mapProjectionChanged(int)), m_pGLUIWin, SLOT(onMapProjectionChanged(int)));
  connect(m_controlWidget, SIGNAL(boundingGeoBoxChanged(const QRectF &)), m_pGLUIWin, SLOT(onBoundingGeoBoxChanged(const QRectF&)));
  connect(m_pGLUIWin, SIGNAL(boundingGeoBoxChanged(const QRectF &)), m_controlWidget, SLOT(onBoundingGeoBoxChanged(const QRectF&)));

  //set status bar widgets

  QStatusBar *sbar = statusBar();
  sbar->setMinimumHeight(28);
  sbar->setMaximumHeight(28);
  sbar->addWidget(new QLabel, 150);

  QLineEdit *msglabel = new QLineEdit;
  staticMessageLabel = msglabel;
  staticMessageLabel->setDisabled(true);
  msglabel->setAlignment(Qt::AlignLeft);
  msglabel->setText("No message");
  msglabel->setMinimumHeight(20);
  msglabel->setMaximumHeight(20);
  sbar->addWidget(msglabel, 256);
  sbar->showMessage( APP_NAME" ready.", 5000 );

  QObject::connect(&mTimer, SIGNAL(timeout()), this, SLOT(onTimeOut()));
  const int version = executableVersion();
  const QString msg = "RSIG3D version " + QString::number( version );
  m_controlWidget->postMessage(msg, RSIGState::NORMAL_TEXT);
  const QString server = RSIGServerHost();
  m_controlWidget->postMessage(QString("Data server: ")+server, RSIGState::NORMAL_TEXT);
  singleton = this;
}



MainWin::~MainWin() {

  try {

    if (m_pGLUIWin && m_pGLUIWin->parent() == NULL) {
      delete m_pGLUIWin;
    }

    if (m_controlWidget && m_controlWidget->parent() == NULL) {
      delete m_controlWidget;
    }

  } catch (...) {
    qDebug() << "Error: MainWin destructor!";
  }
}



void MainWin::resizeEvent ( QResizeEvent * event ) {
  QMainWindow::resizeEvent(event);
  const QSize sz = event->size();
  const QSize szgl = m_pGLUIWin->size();
  QString text = "Main window: " + QString().number(sz.width()) + "X"+QString().number(sz.height());
  text += ", GL window: " + QString().number(szgl.width()) + "X" + QString().number(szgl.height());
  statusBar()->showMessage(text, 3000);
}



QString MainWin::stateFileName() {
  return mStateFileName;
}



QString fromQByteArrayToQString(const QByteArray& array) {
  QString s;
  char buffer[4]={0, 0, 0, 0};

  for (int i=0; i<array.size(); i++) {
    const char x = array[i];
    char y = x & 0x0f;

    if (y<10) {
      buffer[1] = y +'0';
    } else {
      buffer[1] = y + 'A' - 10;
    }

    y = (x>>4)&0x0f;

    if (y<10) {
      buffer[0] = y +'0';
    } else {
      buffer[0] = y + 'A' - 10;
    }

    s+=buffer;
  }

  return s;
}



void MainWin::saveStateToFile(QString dirName, QString fileName, QString id) {

  if (dirName.isEmpty()) {
    return;
  }

  if (fileName.isEmpty()) {
    return;
  }
  
  QFileInfo dirInfo(dirName);

  if (!dirInfo.isDir()) {
    QMessageBox::warning(this, tr("Directory error", "Message box window title"), tr("Directory not valid.").arg(dirName));
    return;
  }
  
  const QString dirNameAbs = dirInfo.absolutePath();

  if (!QFileInfo(dirNameAbs).exists()) {
    QDir dir;

    if (!(dir.mkdir(dirNameAbs))) {
      QMessageBox::warning(this, tr("Directory error", "Message box window title"), tr("Unable to create directory %1").arg(dirName));
      return;
    }
  }

  QFile f(dirNameAbs + "/" + fileName);
  
  if (id.isEmpty()) {
    id = QString("RSIG3D");
  }

  if (f.open(QIODevice::WriteOnly)) {
    QTextStream out(&f);
    QDomDocument doc(id);
    doc.appendChild(domElement(id, doc));
    doc.save(out, 2);
    f.flush();
    f.close();
    m_controlWidget->postMessage(QString("State file ")+dirName+fileName+" saved.", RSIGState::NORMAL_TEXT);
  } else {
    QMessageBox::warning(this, tr("Save to file error", "Message box window title"), tr("Unable to save to file %1").arg(fileName) + ":\n" + f.errorString());
  }
}



bool MainWin::restoreStateFromFile(QString name) {

  if (name.isEmpty()) {
    return false;
  }
  
  QFileInfo fileInfo(name);

  if (!fileInfo.isFile()) { // No warning since it would be displayed at first start.
    return false;
  }
  
  if (!fileInfo.isReadable()) {
    QMessageBox::warning(this, tr("Problem in state restoration", "Message box window title"), tr("File %1 is not readable.").arg(name));
    return false;
  }
  
  // Read the DOM tree form file

  QFile f(name);

  if (f.open(QIODevice::ReadOnly) == true) {
    QDomDocument doc;
    doc.setContent(&f);
    f.close();
    QDomElement main = doc.documentElement();


    // reset data source list

    VisOptions *vOptions = VisOptionsSingleton::getInstance();
    QStringList melist = vOptions->getDataSourceList();
    melist.clear();
    vOptions->setDataSourceList(melist);
    initFromDOMElement(main);
    m_controlWidget->postMessage(QString("State file ")+name+" restored.", RSIGState::NORMAL_TEXT);
  } else {
    QMessageBox::warning(this, tr("Open file error", "Message box window title"), tr("Unable to open file %1").arg(name) + ":\n" + f.errorString());
    return false;
  }

  return true;
}



QByteArray fromQStringToQByteArray(const QString& s) {
  QByteArray array;

  if ( ! s.isEmpty() ) {
    const QByteArray copy( s.toAscii() );
    const char *p = copy.data();

    if ( p ) {

      for (int i=0; i<s.size(); i+=2) {
        char ch = p[i];
        int a = 0, b = 0;

        if (ch>='A') {
          a = ch - 'A' + 10;
        } else {
          a = ch - '0';
        }

        if ( i + 1 < s.size() ) {
          ch = p[i+1];

          if (ch>='A') {
            b = ch - 'A' + 10;
          } else {
            b = ch - '0';
          }

          char x = (a<<4) + b;
          array.push_back(x);
        }
      }
    }
  }

  return array;
}



QDomElement MainWin::domElement( const QString& name,
                                 QDomDocument& document ) const {
  QDomElement de = document.createElement(name);
  const int version = executableVersion();
  de.setAttribute( "version", QString::number( version ) );
  QDomElement geometryNode = document.createElement("Geometry");
  geometryNode.setAttribute("fullScreen", (isFullScreen()?"true":"false"));

  if (isFullScreen()) {
    geometryNode.setAttribute("prevPosX", QString::number(mPreScreenPosX));
    geometryNode.setAttribute("prevPosY", QString::number(mPreScreenPosY));
  } else {
    QWidget* tlw = topLevelWidget();
    geometryNode.setAttribute("width",  QString::number(tlw->width()));
    geometryNode.setAttribute("height", QString::number(tlw->height()));
    geometryNode.setAttribute("posX",   QString::number(tlw->pos().x()));
    geometryNode.setAttribute("posY",   QString::number(tlw->pos().y()));
  }

  const QByteArray stateinfo = this->saveState();
  const QString dockinfo = fromQByteArrayToQString(stateinfo);
  QDomElement dockNode = document.createElement("dock");
  dockNode.setAttribute("value", dockinfo);
  geometryNode.appendChild(dockNode);

  de.appendChild(geometryNode);

  if (m_controlWidget) {
    QDomElement node;

    if (name.compare("RSIG3D") == 0) {
      node = m_controlWidget->domElement("Control", document);
    } else {
      node = m_controlWidget->domElement(name, document);
    }

    de.appendChild(node);
  }

  const RSIGScene* scene = qobject_cast<const RSIGScene*>(m_pGLUIWin->scene());

  if (scene) {
    const QDomElement node = scene->domElement("Viewer", document);
    de.appendChild(node);    
  }

  return de;
}



void MainWin::initFromDOMElement(const QDomElement& element) {
  RSIGScene* scene = qobject_cast<RSIGScene*>(m_pGLUIWin->scene());
  
  const QString version = element.attribute("version");
  qDebug() << "RSIG3D version: "<<version;
  
  QDomElement child = element.firstChild().toElement();

  while (!child.isNull()) {

    if (child.tagName() == "Control") {

      if (m_controlWidget) {
        m_controlWidget->initFromDOMElement(child);
      }
    }
    
    if (child.tagName() == "Viewer") {

      if (scene) {
        scene->initFromDOMElement(child);
      }
    }

    if (child.tagName() == "Geometry") {
      const bool needFullScreen = DomUtils::boolFromDom(child, "fullScreen", false);

      if (needFullScreen) {
        mPreScreenPosX = (DomUtils::intFromDom(child, "prevPosX", 0));
        mPreScreenPosY = (DomUtils::intFromDom(child, "prevPosY", 0));
      } else {
        int width  = DomUtils::intFromDom(child, "width",  600);
        int height = DomUtils::intFromDom(child, "height", 400);
        topLevelWidget()->resize(width, height);        
        QPoint pos;
        pos.setX(DomUtils::intFromDom(child, "posX", 0));
        pos.setY(DomUtils::intFromDom(child, "posY", 0));
        topLevelWidget()->move(pos);
      }
      
      QDomElement ch = child.firstChild().toElement();

      while (!ch.isNull()) {

        if (ch.tagName()=="dock") {
          const QString dockinfo = ch.attribute("value");
          const QByteArray arrayinfo( fromQStringToQByteArray(dockinfo) );
          this->restoreState(arrayinfo);
        }
        ch = ch.nextSibling().toElement();
      }      
    }
            
    child = child.nextSibling().toElement();
  }  
}



void MainWin::keyPressEvent(QKeyEvent* e) {
  RSIGState *gState = RSIGStateSingleton::getInstance();
  int newCounterVal;

  switch(e->key()) {
  case Qt::Key_Escape:
  case Qt::Key_P:
    mTimer.setInterval(m_controlWidget->get_animationSpeed());

    if (mTimer.isActive()) {
      m_controlWidget->update_playButtonText(QString("Play"));
      mTimer.stop();
      statusBar()->showMessage("Timer stopped.", 2000);
    } else {
      m_controlWidget->update_playButtonText(QString("Stop"));
      statusBar()->showMessage("Timer started.", 2000);
      mTimer.start();
    }

    break;
  case Qt::Key_Backspace:
    newCounterVal = gState->mCounter - 1;

    if (newCounterVal < 0) {

      if (gState->mTimebaseType == RSIGState::TIMEBASE_HOURLY) {
        newCounterVal = gState->mDuration * 24 - 1;
      } else {
        newCounterVal = gState->mDuration - 1;
      }
    }

    gState->mCounter = newCounterVal;
    statusBar()->showMessage("Counter ID: " + QString().number(newCounterVal), 2000);
    m_controlWidget->update_timestepSlider(gState->mCounter);
    this->refreshGLWindow();
    break;
  case Qt::Key_Space:
  case Qt::Key_Enter:
  case Qt::Key_Return:
    break;
  default:
    QMainWindow::keyPressEvent(e);
    break;
  }
}



void MainWin::closeEvent( QCloseEvent* ce ) {
  QFileInfo fileInfo(stateFileName());
  this->saveStateToFile(fileInfo.absolutePath() + "/", fileInfo.fileName(), "");
  QMainWindow::closeEvent(ce);
}



void MainWin::refreshGLWindow() {
  //directly call QGraphicsView::update() won't work, nor the repaint() function

  if (m_pGLUIWin) {
    QWidget* viewport = m_pGLUIWin->viewport();
    viewport->update();
  }
}




void MainWin::centerAndResize() {
  // get the dimension available on this screen
  QSize availableSize = qApp->desktop()->availableGeometry().size();
  int width  = availableSize.width();
  int height = availableSize.height();
  width  *= 0.8;   // 90% of the screen size

  if (width>1024) {
    width = 1024;
  }

  height *= 0.8; // Fraction of the screen size

  if (height>768) {
    height = 768;
  }

  QSize newSize( width, height );

  setGeometry(QStyle::alignedRect(Qt::LeftToRight,
                                  Qt::AlignCenter,
                                  newSize,
                                  qApp->desktop()->availableGeometry() ) );
}



void MainWin::saveImage(bool automatic, bool overwrite) {
  const QString backupName = snapshotFileName();  

  if (!automatic) {
    const int count = snapshotCounter();
    QFileInfo info(snapshotFileName());
    const QString filename= info.absolutePath() + "/" + info.baseName() + QString().number(count) + ".png";
    this->setSnapshotFileName(filename);
  }

  this->setSnapshotFormat("PNG");
  this->saveSnapshot(automatic, overwrite);
  this->setSnapshotFileName(backupName);
}



void MainWin::saveSnapshot(bool automatic, bool overwrite) {
  VisOptions *vOptions = VisOptionsSingleton::getInstance();
  QFileInfo fileInfo(vOptions->getFilenamePng());
  QString saveDir = vOptions->getSaveDir() + '/';
  QString savePathPlusFile = saveDir + fileInfo.filePath();

  if ( automatic && snapshotCounter() >= 0 ) {
    // In automatic mode, names have a number appended
    const QString baseName = fileInfo.baseName();
    QString count;
    count.sprintf("%.04d", snapshotCounter_++);
    QString suffix;

    suffix = fileInfo.suffix();

    if (suffix.isEmpty()) {
      suffix = "png";
    }

    fileInfo.setFile(saveDir + baseName + '-' + count + '.' + suffix);

    if (!overwrite) {

      while (fileInfo.exists()) {
        count.sprintf("%.04d", snapshotCounter_++);
        fileInfo.setFile(saveDir + baseName + '-' + count + '.' + fileInfo.suffix());
      }
    }
  }

  //grab OpenGL framebuffer for the snapshot, note that the back buffer needs to be read
  QImage glscreen;

  if (m_pGLUIWin != NULL) {

    if (m_pGLUIWin->getQGLlWidget()->doubleBuffer()) {
      m_pGLUIWin->getQGLlWidget()->swapBuffers(); // swapBuffers() leads to blank images on some systems (e.g. Windows)
    }

    m_pGLUIWin->update();
    m_pGLUIWin->getQGLlWidget()->updateGL();

    glscreen = m_pGLUIWin->getQGLlWidget()->grabFrameBuffer(true);
    int mywidth  = glscreen.width();
    int myheight = glscreen.height();
    mywidth  -= (mywidth  % 2);  // ensure width  is even to keep ffmpeg happy
    myheight -= (myheight % 2);  // ensure height is even to keep ffmpeg happy
    QImage glscreen_trim = glscreen.copy(0,0,mywidth,myheight);
    glscreen = glscreen_trim;
  }

  QString msg;

  if (automatic) {
    QString msg = QString("Saving image ") + fileInfo.filePath();
  } else {
    QString msg = QString("Saving image ") + savePathPlusFile;
  }

  statusBar()->showMessage(msg, 5000);

  bool saveOK;

  if (automatic) {
    saveOK = glscreen.save(fileInfo.filePath(), snapshotFormat().toLatin1().constData(), 100);
  } else {
    saveOK = glscreen.save(savePathPlusFile, snapshotFormat().toLatin1().constData(), 100);
  }

  if (!saveOK) {
    QMessageBox::warning(this, "Snapshot problem", "Unable to save snapshot in\n"+savePathPlusFile);
  } else {
    m_pGLUIWin->viewport()->update();
    snapshotSaveDir_ = fileInfo.filePath();
  }
}



void MainWin::initializeSnapshotVariables(const QString &dirName) {
  snapshotCounter_= 0;
  snapshotFormat_="PNG";
  snapshotSaveDir_ = QDir::homePath(); // default
  snapshotFileName_="snapshot.png";

  if (dirName != "") {
    QFileInfo info(dirName);

    if (info.isDir()) {
      snapshotSaveDir_ = dirName;
      snapshotFileName_ = info.absolutePath() + "/snapshot.png";
    } else {
      const QString fname = info.baseName();
      const QString fpath = info.absolutePath();
      snapshotSaveDir_ = fpath;
      snapshotFileName_ = fpath + "/" + fname;
    }
  }
}



void MainWin::initializeSnapshotVariablesForVideo(const QString &dirName) {
  snapshotCounter_= 0;
  snapshotFormat_="PNG";
  snapshotSaveDir_ = QDir::homePath(); // default
  snapshotFileName_="snapshot.png";

  if (dirName != "") {
    QFileInfo info(dirName);

    if (info.isDir()) {
      snapshotSaveDir_ = dirName;
      snapshotFileName_ = info.absolutePath() + "/RSIG-Snapshot.png";
    } else {
      const QString fname = info.baseName();
      const QString fpath = info.absolutePath();
      snapshotSaveDir_ = fpath;
      snapshotFileName_ = fpath + "/" + fname;
    }
  }
}



void MainWin::onRequestSnapshot() {
  const QString backupName = snapshotFileName();
  saveImage(true, true);
  this->setSnapshotFileName(backupName);
}



void MainWin::onRequestSingleSnapshot() {
  const QString backupName = snapshotFileName();
  saveImage(false, true);
  this->setSnapshotFileName(backupName);
}



void MainWin::onRequestInitSnapshot(const QString&dirName) {
  // initializeSnapshotVariables() is in MainWin_snapshop.cpp
  this->initializeSnapshotVariables(dirName);
}



void MainWin::onRequestInitSnapshotForVideo(const QString&dirName) {
  // initializeSnapshotVariablesForVideo() is in MainWin_snapshop.cpp
  this->initializeSnapshotVariablesForVideo(dirName);
}



void MainWin::onTimeOut() {
  RSIGState *gState = RSIGStateSingleton::getInstance();

  if (gState->mTimebaseType == RSIGState::TIMEBASE_HOURLY) {
    gState->mCounter = (gState->mCounter + 1) % (gState->mDuration * 24);
  } else {
    gState->mCounter = (gState->mCounter + 1) % gState->mDuration;
  }
 
  mTimer.setInterval(m_controlWidget->get_animationSpeed());
  m_controlWidget->update_timestepSlider(gState->mCounter);
  this->refreshGLWindow();
}





