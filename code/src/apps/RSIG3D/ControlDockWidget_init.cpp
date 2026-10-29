/******************************************************************************
PURPOSE: ControlDockWidget_init.cpp - Implements openglscene.

NOTES: Implemented some slot functions of ControlDockWidget class.

HISTORY: 2012/11/12, zhang.nan@epa.gov, Created.
******************************************************************************/

#include <iostream>
#include <string>

#include <QApplication>        //qApp, slot closeAllWindows()
#include <QDesktopWidget>
#include <QMainWindow>
#include <QDesktopServices>      //QDesktopServices::openUrl(QUrl);
#include <QString>          //QString
#include <QStringList>     
#include <QDebug>          //qDebug()
#include <QResizeEvent>             //QResizeEvent class
#include <QDir>                     //QDir::homePath()
#include <QFileInfo>
#include <QFileDialog>
#include <QMessageBox>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QStandardItem>
#include <QSignalMapper>
#include <QScrollBar>
#include <QSettings>
#include <QVariant>
#include <QMap>
#include <QUuid>

#include <Timestamp.h> // For emvl::Timestamp.

#include "RSIGGlobal.h"
#include "GeoDataObject.h"
#include "VisDataObject.h"
#include "GeoDataList.h"
#include "VisDataList.h"
#include "GeoDataPropertyEdit.h"
#include "VisDataPropertyEdit.h"
#include "LookupTableWidget.h"      //LookupTableWidget class
#include "SlantLookupTableWidget.h" //SlantLookupTableWidget class
#include "PointRenderer.h"          //PointRenderer class
#include "HttpDownloadDialog.h"    //HttpGet class, multi-threaded downloading web data
#include "RegridParmsDialog.h"      //RegridParms dialog
#include "FilterSettingDialog.h"    //FilterPamrs dialog
#include "DomUtils.h"               //XML utils
#include "ControlDockWidget.h"      //self header
#include "ui_controldockwidget.h"   //UI form
#include "VisOptions.h"
#include "DataPreferences.h"
#include "VisDataPropertyDelegate.h"

static QStringList gSourceList;
static QStringList gAttribeList; 
static QStringList gUnitList;
static QStringList gCommentList; 
static QVector<int> gIndexList;

static QStringList gSourceTree;
static QTreeWidget *tw;
static QTreeWidgetItem *newItem1;
static QTreeWidgetItem *newItem2;
static QTreeWidgetItem *newItem3;
static QTreeWidgetItem *newItem4;
static QTreeWidgetItem *newItem5;
static QTreeWidgetItem *newItem6;
static QTreeWidgetItem *newItem7;


QTreeWidgetItem * addChild (QTreeWidgetItem *myparent, QTreeWidgetItem *mychild, QString myItemName) {   

  if (myparent) {
    int child_count = myparent->childCount();
    // see if we already have this item in the list of children
    bool match_exists = false;

    for (int childInd=0; childInd<child_count; childInd++) {
      QTreeWidgetItem *existing_child = myparent->child(childInd);

      if (existing_child->text(0) == myItemName) {
        match_exists = true; // don't add the child because it already exists in the tree
      }
    }

    if ( match_exists == false ) {
      // add the child
      mychild = new QTreeWidgetItem(myparent);
      mychild->setText(0, myItemName);
    }
  }

  return mychild;
}



void ControlDockWidget::readDataAvailability() {
  const QString filename(":/rsig_availability.txt"); 
  QFile f(filename);

  if (!f.isOpen()) {
    const bool r = f.open(QIODevice::ReadOnly);

    if (r == false) {
      qDebug()<<"Error in open file: " << filename;
      return;
    }
  }

  QTextStream s( &f );

  for ( int index = 0; ! s.atEnd(); ++index ) {
    const QString line(s.readLine());
    const QStringList parse(line.split(' '));
    bool ok = false;
    availNames[index] = parse[0];
    minAvailDate[index] = parse[1].toLong(&ok, 10);
    maxAvailDate[index] = parse[2].toLong(&ok, 10);
  }
}



void ControlDockWidget::readSourceTree(QString rsigVariablesFile,
                                       QStringList &sourceTree,
                                       QTreeWidget *tw) {
  QStringList parse;

  // disable all layers by default
  ui->selectedLayer1->hide();
  ui->selectedLayer2->hide();
  ui->selectedLayer3->hide();
  ui->selectedLayer4->hide();
  ui->selectedLayer5->hide();

  sourceTree.clear();
  // file name has to be listed in "RSIG3D.qrc"
  const QString filename = rsigVariablesFile;
  QFile f(filename);

  if (!f.isOpen()) {
    const bool r = f.open(QIODevice::ReadOnly);

    if (r == false) {
      qDebug()<<"Error in open file: " << filename;
      return;
    }
  }

  QString prev_source("");
  QTextStream s( &f );

  while (!s.atEnd()) {
    QString line = s.readLine();

    if (line.size()>2 && !line.startsWith('#')) {
      QTextStream linestream (&line, QIODevice::ReadOnly);
      QString s0;
      linestream >> s0;
      QString s1;
      QChar inchar = 0;

      do {
        linestream >> inchar;
      } while ( inchar == ' ');

      s1.append(inchar);

      do {
        linestream >> inchar;
        s1.append(inchar);
      } while (inchar != ')' && !linestream.atEnd());
  
      //skip the begining blanks in s2
      QString s2 = linestream.readAll();

      if (s2.size() != 0 && s2[0] == QChar(' ')) {
        int start=0;

        while ((s2[start]==' ') && (start < s2.size())) {
          start ++;
        }

        QString tmp;

        for (int i=start; i<s2.size(); i++) {
          tmp.append(s2[i]);
        }

        s2 = tmp;
      }
  
      QString this_tooltip;
      this_tooltip = get_tooltip(s0, s1, s2);
      parse = s0.split('.'); // make sure to replace commas with dots after parsing
                             // this is done to accomodate COMPARE variables

      for (int tokID=0; tokID<parse.length(); tokID++) {
        // add top level menu if needed
        QList<QTreeWidgetItem *> match_list = tw->findItems(parse[0], Qt::MatchExactly, 0);

        if ( (tokID == 0) && (match_list.length() == 0 ) ) {
          newItem1 = new QTreeWidgetItem(tw);
          newItem1->setText(0,parse[tokID].replace(",", "."));
        }

        // add second level menu if needed

        if (tokID == 1) {
          newItem2 = addChild(newItem1, newItem2, parse[tokID].replace(",", "."));

          if (this_tooltip != "" && parse[tokID] != "cmaq"  && parse[tokID] != "satellitepm") {
            newItem2->setToolTip(0,this_tooltip);
          }

          if (tokID == parse.length()-1) {
            newItem2->setCheckState(0, Qt::Unchecked);
            newItem2->setFlags(newItem2->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
          }
        }

        // third level

        if (tokID == 2) {
          newItem3 = addChild(newItem2, newItem3, parse[tokID].replace(",", "."));

          if (this_tooltip != "") {
            newItem3->setToolTip(0,this_tooltip);
          }

          if (tokID == parse.length()-1) {
            newItem3->setCheckState(0, Qt::Unchecked);
            newItem3->setFlags(newItem3->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
          }
        }

        // fourth level
        if (tokID == 3) {
          newItem4 = addChild(newItem3, newItem4, parse[tokID].replace(",", "."));

          if (this_tooltip != "") {
            newItem4->setToolTip(0,this_tooltip);
          }

          if (tokID == parse.length()-1) {
            newItem4->setCheckState(0, Qt::Unchecked);
            newItem4->setFlags(newItem4->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
          }
        }

        // fifth level
        if (tokID == 4) {
          newItem5 = addChild(newItem4, newItem5, parse[tokID].replace(",", "."));

          if (this_tooltip != "") {
            newItem5->setToolTip(0,this_tooltip);
          }

          if (tokID == parse.length()-1) {
            newItem5->setCheckState(0, Qt::Unchecked);
            newItem5->setFlags(newItem5->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
          }
        }

        // sixth level
        if (tokID == 5) {
          newItem6 = addChild(newItem5, newItem6, parse[tokID].replace(",", "."));

          if (this_tooltip != "") {
            newItem6->setToolTip(0,this_tooltip);
          }

          if (tokID == parse.length()-1) {
            newItem6->setCheckState(0, Qt::Unchecked);
            newItem6->setFlags(newItem6->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
          }
        }

        // seventh level
        if (tokID == 6) {
          newItem7 = addChild(newItem6, newItem7, parse[tokID].replace(",", "."));

          if (this_tooltip != "") {
            newItem7->setToolTip(0,this_tooltip);
          }

          if (tokID == parse.length()-1) {
            newItem7->setCheckState(0, Qt::Unchecked);
            newItem7->setFlags(newItem7->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
          }
        }
      }
    }
  }
}



QString ControlDockWidget::get_variableDescription(QString item) {
  QString return_string = ""; // default
  QString this_var = "";

  for (int sourceInd=0; sourceInd<gSourceList.length(); sourceInd++) {

    for (int attribeInd=0; attribeInd<1; attribeInd++) {
      this_var = gSourceList[sourceInd] + "." + gAttribeList[attribeInd];

      if (this_var.contains(item)) {
        return_string = gCommentList[sourceInd+attribeInd];
        break;
      }
    }
  }

  return return_string;
}



void readResourceHelper(QString rsigVariablesFilename,
                        QStringList & sourceList,
                        QStringList & attribeList,
                        QStringList & unitList,
                        QStringList & commentList,
                        QVector<int> & indexList) {

  sourceList.clear(); 
  attribeList.clear();
  unitList.clear();
  commentList.clear();
  indexList.clear();
  const QString filename = rsigVariablesFilename;
  QFile f(filename);

  if (!f.isOpen()) {
    const bool r = f.open(QIODevice::ReadOnly);

    if (r == false) {
      qDebug()<<"Error in open file: " << filename;
      return;
    }
  }
  
  QString prev_source("");
  QTextStream s( &f );
  
  while (!s.atEnd()) {
    QString line = s.readLine();

    if (line.size()>2) {
      QTextStream linestream (&line, QIODevice::ReadOnly);
      QString s0;      
      linestream >> s0;
      
      //read unit description, the format is "(**)"
      QString s1;
      QChar inchar = 0;

      do {
        linestream >> inchar;      
      } while ( inchar == ' ');

      s1.append(inchar);

      do {
        linestream >> inchar;
        s1.append(inchar);
      } while (inchar != ')' && !linestream.atEnd());
      
      //skip the begining blanks in s2
      QString s2 = linestream.readAll();

      if (s2.size() != 0 && s2[0] == QChar(' ')) {
        int start=0;

        while ((s2[start]==' ') && (start < s2.size())) {
          start ++;
        }

        QString tmp;

        for (int i=start; i<s2.size(); i++) {
          tmp.append(s2[i]);
        }

        s2 = tmp;
      }
      
      const int dotpos = s0.lastIndexOf(QChar('.'));

      if (dotpos >= 0) {
        QString source, attribe;

        for (int i=0; i<dotpos; i++) {
          source.append(s0[i]);
        }

        for (int i=dotpos+1; i<s0.size(); i++) {
          attribe.append(s0[i]);
        }
        
        if (prev_source != source) {
          const int n = attribeList.size();
          indexList.push_back( n );
          sourceList.push_back( source );
          prev_source = source;
        }

        attribeList.push_back( attribe );
        unitList.push_back( s1 );
        commentList.push_back( s2 );  

        for (int tokID=0; tokID<source.length(); tokID++) {

          if (tokID == 0) {
            //category = category_lookup(source);
          }
        }
      }
    }  
  }
  
  indexList.push_back(attribeList.size());
}



void ControlDockWidget::addVariablesHelper2(const QString rsigVariablesFilename) {
  // change to hourglass cursor
  QApplication::setOverrideCursor(Qt::WaitCursor);
  
  // clear any existing data selections
  onDataClearClicked();
  
  // load source data list
  gSourceList.clear();
  gAttribeList.clear();
  gUnitList.clear();
  gCommentList.clear();
  gIndexList.clear();

  if (gSourceList.size() == 0) {
    readResourceHelper(rsigVariablesFilename, gSourceList, gAttribeList, gUnitList, gCommentList, gIndexList);
  }
  
  //init the data source input widgets
  const char * stylestr = "QToolTip { border: 2px solid red; background: yellow }";
  const QLatin1String tipstyle(stylestr);
  
  tw = ui->varTree;
  tw->clear();
  tw->verticalScrollBar()->setStyleSheet(
                                         "background-color: transparent;"
                                         "alternate-background-color: transparent;"
                                         );
  tw->horizontalScrollBar()->setStyleSheet(
                                           "background-color: transparent;"
                                           "alternate-background-color: transparent;"
                                           );
  
  // set font pixel size
  //QFont twFont = tw->font();
  //twFont.setPixelSize(12);
  //tw->setFont(twFont);
  
  // Ensure that a horizontal scrollbar is used rather than an ellipsis
  (*tw).header()->setResizeMode(0, QHeaderView::ResizeToContents);
  (*tw).header()->setStretchLastSection(false);
  
  readDataAvailability();
  readSourceTree(rsigVariablesFilename, gSourceTree, tw);

  // back to normal cursor
  QApplication::restoreOverrideCursor();
}



void ControlDockWidget::setVariablesToComboBox2(QComboBox *w, int index) {

  if (w == NULL || index<0 || index>=gSourceList.size()) {
    qDebug() << "setVariablesToComboBox2(): Error in input values!";
    return;
  }

  const QStringList & varList = gAttribeList;
  const QStringList & tipList = gCommentList;
  const int startpos = gIndexList[index];
  const int endpos = gIndexList[index+1];  
  int count = 0;
  w->clear();

  for (int i= startpos; i<endpos; i++) {
    const QString& str = varList[i]; 
    const QString& tip = tipList[i];

    if (str.length()==0) {
      continue;
    }

    if (str[0]==' ') {
      continue;
    }

    if (str[0]=='-' && str[1]=='-') {
      w->insertSeparator(i);
    } else {
      w->addItem(str);
      w->setItemData(count, tip, Qt::ToolTipRole);
      w->setItemData(count, tip, Qt::StatusTipRole);
      w->setItemData(count, tip, Qt::WhatsThisRole);
    }

    count++;
  }

  //set default index, conditionally
  int cindex = 0;

  if (varList[0][0]=='-' && varList[0][1]=='-') {
    cindex=1;
  }

  w->setCurrentIndex(cindex);
}



void ControlDockWidget::initTab0ClientData() {
  // for the Map tab
  
  // actual screen size
  QDesktopWidget desktop;
  int screenWidth  = desktop.screenGeometry().width();
  int screenHeight = desktop.screenGeometry().height();
  qDebug() << "screen dimensions:" << screenWidth << screenHeight;

  int extraHeight = 85;
  int extraWidth  = 1000;
  if (screenHeight > 2000) {
    // add height where needed
    ui->tabWidget->setMinimumHeight(ui->tabWidget->height()     + extraHeight*4);
    ui->mapTab->setMinimumHeight(ui->tabWidget->height()     + extraHeight*4);
    ui->groupBox_mapprojbound->setMinimumHeight(ui->groupBox_mapprojbound->height() + extraHeight);
    ui->groupBox_mapoptions->setMinimumHeight(ui->groupBox_mapoptions->height() + extraHeight);
    ui->groupBox_mapother->setMinimumHeight(ui->groupBox_mapother->height() + extraHeight);

    ui->groupBox_mapother->setMinimumWidth(ui->groupBox_mapother->width() + extraWidth);
    ui->groupBox_mapoptions->setMinimumWidth(ui->groupBox_mapoptions->width() + extraWidth);
    ui->groupBox_mapprojbound->setMinimumWidth(ui->groupBox_mapprojbound->width() + extraWidth);

    // reposition widgets as needed
    ui->groupBox_mapoptions->setGeometry(ui->groupBox_mapoptions->x(),
                                 ui->groupBox_mapprojbound->y() + ui->groupBox_mapprojbound->height() + 2,
                                 ui->groupBox_mapoptions->width(),
                                 ui->groupBox_mapoptions->height());
    ui->groupBox_mapother->setGeometry(ui->groupBox_mapother->x(),
                                 ui->groupBox_mapoptions->y() + ui->groupBox_mapoptions->height() + 2,
                                 ui->groupBox_mapother->width(),
                                 ui->groupBox_mapother->height());
    
  }
  
  this->onViewAutozoomPressed();
  
  VisOptions* vOptions = VisOptionsSingleton::getInstance();
  
  // uniqueID for this session
  QString uniqueID = QUuid::createUuid().toString().remove("{").remove("}").remove("-");
  vOptions->setUniqueId(uniqueID);

  // detect when tab is canged
  connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(onTabWidgetIndexChanged(int)));

  // For the MAP tab

  addMapServersHelper();
  addMaplineColorHelper();

  const RSIGState* const gState = RSIGStateSingleton::getInstance();
  ui->mapserverComboBox->setCurrentIndex(0);
  ui->lonlatRadioButton->setChecked(true);
  ui->maplineNone->setChecked(true);
  ui->showLonlatCheckBox->setChecked(true);
  ui->showScaleCheckBox->setChecked(true);
  ui->showNavigatorCheckBox->setChecked(gState->mShowNavigator);
  
  connect(ui->showLonlatCheckBox, SIGNAL(stateChanged(int)), this, SLOT(onShowLonlatLinesChanged(int)));
  connect(ui->showScaleCheckBox, SIGNAL(stateChanged(int)), this, SLOT(onShowScaleChanged(int)));
  connect(ui->showNavigatorCheckBox, SIGNAL(stateChanged(int)), this, SLOT(onShowNavigatorChanged(int)));
  connect(ui->clearlogToolButton, SIGNAL(clicked(bool)), this, SLOT(onClearLog()));
  
  connect(ui->mapserverComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onMapServerIndexChanged(int)));
  connect(ui->lonlatRadioButton, SIGNAL(clicked(bool)), this, SLOT(onMapProjectionChanged()));
  connect(ui->sphericalRadioButton, SIGNAL(clicked(bool)), this, SLOT(onMapProjectionChanged()));
  connect(ui->mercatorRadioButton, SIGNAL(clicked(bool)), this, SLOT(onMapProjectionChanged()));  
  connect(ui->lambertconformalRadioButton, SIGNAL(clicked(bool)), this, SLOT(onMapProjectionChanged()));
  connect(ui->stereographicRadioButton, SIGNAL(clicked(bool)), this, SLOT(onMapProjectionChanged()));  
  connect(ui->chk_tooltips, SIGNAL(clicked(bool)), this, SLOT(setWidgetTooltips()));  
  connect(ui->guiFontsizeSpinbox, SIGNAL(valueChanged(int)), this, SLOT(onGuiFontsizeChanged(int)) );

  connect(ui->chk_mapDoubleClickOption1, SIGNAL(stateChanged(int)), this, SLOT(onSetMapDoubleClickOption1(int)));  
  connect(ui->chk_mapDoubleClickOption2, SIGNAL(stateChanged(int)), this, SLOT(onSetMapDoubleClickOption2(int)));  
  
  connect(ui->maplineNone, SIGNAL(clicked(bool)), this, SLOT(onMaplinesChanged()));
  connect(ui->maplineNations, SIGNAL(clicked(bool)), this, SLOT(onMaplinesChanged()));
  connect(ui->maplineStates, SIGNAL(clicked(bool)), this, SLOT(onMaplinesChanged()));
  connect(ui->maplineStates_hires, SIGNAL(clicked(bool)), this, SLOT(onMaplinesChanged()));
  connect(ui->maplineCounties, SIGNAL(clicked(bool)), this, SLOT(onMaplinesChanged()));
  connect(ui->maplineColorComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onMaplineColorIndexChanged(int)));

  connect(ui->showDataProbe, SIGNAL(stateChanged(int)), this, SLOT(onShowDataProbeChanged(int)));
        
  qRegisterMetaType<QTextCursor>("QTextCursor");
}



//initialize page 1: Data
int srcDefaults[] = {1, 2, 63, 95, 97};
int varDefaults[] = {1, 9,  0,  6,  0};
void ControlDockWidget::initTab1ClientData() {
  // for the Data tab

  // add height where needed
  // actual screen size
  QDesktopWidget desktop;
  //int screenWidth  = desktop.screenGeometry().width();
  int screenHeight = desktop.screenGeometry().height();
  int extraHeight = 50;
  int extraWidth  = 1000;
  if (screenHeight > 2000) {
    ui->groupBox_datatab->setMinimumHeight(ui->groupBox_datatab->height() + extraHeight*5);
    ui->groupBox_vartree->setMinimumHeight(ui->groupBox_vartree->height() + extraHeight*4);
    ui->groupBox_dataselections->setMinimumHeight(ui->groupBox_dataselections->height() + extraHeight*2);
    ui->groupBox_datetime->setMinimumHeight(ui->groupBox_datetime->height()             + extraHeight);
    //ui->groupBox_getdata->setMinimumHeight(ui->groupBox_getdata->height()               + extraHeight);

    ui->groupBox_datatab->setMinimumWidth(ui->groupBox_datatab->width()               + extraWidth);
    ui->groupBox_scenario->setMinimumWidth(ui->groupBox_scenario->width()             + extraWidth);
    ui->groupBox_vartree->setMinimumWidth(ui->groupBox_vartree->width()               + extraWidth);
    ui->groupBox_dataselections->setMinimumWidth(ui->groupBox_dataselections->width() + extraWidth);
    ui->groupBox_datetime->setMinimumWidth(ui->groupBox_datetime->width()             + extraWidth*1.25);
    ui->groupBox_getdata->setMinimumWidth(ui->groupBox_getdata->width()               + extraWidth);

    // reposition widgets as needed
    int vertSpacing = 5;
    ui->groupBox_vartree->setGeometry(ui->groupBox_vartree->x(),
                                      ui->groupBox_scenario->y() + ui->groupBox_scenario->height() + vertSpacing,
                                      ui->groupBox_vartree->width(),
                                      ui->groupBox_vartree->height());
    ui->groupBox_dataselections->setGeometry(ui->groupBox_dataselections->x(),
                                      ui->groupBox_vartree->y() + ui->groupBox_vartree->height() + vertSpacing,
                                      ui->groupBox_dataselections->width(),
                                      ui->groupBox_dataselections->height());
    ui->groupBox_datetime->setGeometry(ui->groupBox_datetime->x(),
                                       ui->groupBox_dataselections->y() + ui->groupBox_dataselections->height() + vertSpacing,
                                       ui->groupBox_datetime->width(),
                                       ui->groupBox_datetime->height());
    ui->groupBox_getdata->setGeometry(ui->groupBox_getdata->x(),
                                      ui->groupBox_datetime->y() + ui->groupBox_datetime->height() + vertSpacing,
                                      ui->groupBox_getdata->width(),
                                      ui->groupBox_getdata->height());

    
  }

  
  // add styling to certain groupboxes
  ui->groupBox_vartree->setStyleSheet("QGroupBox {border:1px solid gray;}");
  ui->groupBox_dataselections->setStyleSheet("QGroupBox {border:1px solid gray;}");
  ui->dateGroupBox->setStyleSheet("QGroupBox {border:1px solid gray;}");
  ui->boundingGroupBox->setStyleSheet("QGroupBox {border:1px solid gray;}");
  ui->groupBox_getdata->setStyleSheet("QGroupBox {border:1px solid gray;}");

  
  ui->groupBox_scenario->setStyleSheet("QGroupBox {border:0;}");
  ui->groupBox_vartree->setStyleSheet("QGroupBox {border:0;}");
  ui->groupBox_dataselections->setStyleSheet("QGroupBox {border:0;}");
  ui->groupBox_datetime->setStyleSheet("QGroupBox {border:0;}");
  ui->groupBox_getdata->setStyleSheet("QGroupBox {border:0;}");
  ui->groupBox_west->setStyleSheet("QGroupBox {border:0;}");


  
  // adjust font for aggregate menu
#ifdef _WIN32
  QFont font = ui->aggregate1->font();
  font.setPointSize(7);
  ui->aggregate1->setFont(font);
  ui->aggregate2->setFont(font);
  ui->aggregate3->setFont(font);
  ui->aggregate4->setFont(font);
  ui->aggregate5->setFont(font);
#endif
  
  addScenariosHelper();
  addTimebaseHelper();

  const QString rsigVariablesFilename = ":/rsig_variables.txt";
  addVariablesHelper2(rsigVariablesFilename);

  // set default date to yesterday
  emvl::Timestamp timestamp; // Current date/time UTC.
  timestamp.offsetHours( -24 ); // Backup one day.
  const int yyyymmdd = timestamp.yyyymmdd();
  const int yyyy     = yyyymmdd / 10000;
  const int mm       = yyyymmdd / 100 % 100;
  const int dd       = yyyymmdd % 100;
  ui->dateDateEdit->setDate(QDate(yyyy, mm, dd));
        
  QString menuA = "None";
  ui->aggregate1->addItem(menuA);
  ui->aggregate2->addItem(menuA);
  ui->aggregate3->addItem(menuA);
  ui->aggregate4->addItem(menuA);
  ui->aggregate5->addItem(menuA);
  QString menuB = "UTC daily mean";
  ui->aggregate1->addItem(menuB);
  ui->aggregate2->addItem(menuB);
  ui->aggregate3->addItem(menuB);
  ui->aggregate4->addItem(menuB);
  ui->aggregate5->addItem(menuB);
  QString menuC = "UTC daily max";
  ui->aggregate1->addItem(menuC);
  ui->aggregate2->addItem(menuC);
  ui->aggregate3->addItem(menuC);
  ui->aggregate4->addItem(menuC);
  ui->aggregate5->addItem(menuC);
  //ui->selectedData1->setFixedWidth(selectedDataLongWidth);
  //ui->selectedData2->setFixedWidth(selectedDataLongWidth);
  //ui->selectedData3->setFixedWidth(selectedDataLongWidth);
  //ui->selectedData4->setFixedWidth(selectedDataLongWidth);
  //ui->selectedData5->setFixedWidth(selectedDataLongWidth);
  ui->aggregate1->hide();
  ui->aggregate2->hide();
  ui->aggregate3->hide();
  ui->aggregate4->hide();
  ui->aggregate5->hide();

  ui->scenarioComboBox->setCurrentIndex(0);

  // true color imagery
  ui->trueColorComboBox->addItem("None");
  ui->trueColorComboBox->addItem("MODIS-AQUA");
  ui->trueColorComboBox->addItem("MODIS-TERRA");
  ui->trueColorComboBox->addItem("VIIRS-SNPP");
  ui->trueColorComboBox->addItem("VIIRS-NOAA20");
  ui->trueColorComboBox->addItem("VIIRS-NOAA21");

  // set default bbox
  VisOptions *vOptions = VisOptionsSingleton::getInstance();
  double minLon = -126.0;
  double minLat = 24.0;
  double maxLon = -66.0;
  double maxLat = 50.0;
  ui->westDSpinBox->setValue(minLon);
  ui->eastDSpinBox->setValue(maxLon);
  ui->northDSpinBox->setValue(maxLat);
  ui->southDSpinBox->setValue(minLat);  
  vOptions->setBboxMinLon(minLon);
  vOptions->setBboxMinLat(minLat);
  vOptions->setBboxMaxLon(maxLon);
  vOptions->setBboxMaxLat(maxLat);
  set_currentGuiBbox_minLat(minLat);
  set_currentGuiBbox_maxLat(maxLat);
  set_currentGuiBbox_minLon(minLon);
  set_currentGuiBbox_maxLon(maxLon);

  // set font pixel size for timestep label
  //QFont labFont = ui->label_timestep->font();
  //labFont.setPixelSize(14);
  //ui->label_timestep->setFont(labFont);

  connect(tw, SIGNAL(itemChanged(QTreeWidgetItem *, int)), this, SLOT(onTreeWidgetItemChanged(QTreeWidgetItem *)));

  connect(ui->scenarioComboBox, SIGNAL(activated(int)), this, SLOT(onScenarioIndexChanged(int)));
  connect(ui->timebaseComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onTimebaseIndexChanged(int)));
  connect(ui->getdataPushButton, SIGNAL(clicked()), this, SLOT(onGetdataClicked()));
  connect(ui->checkServersPushButton, SIGNAL(clicked()), this, SLOT(onCheckServersClicked()));

  connect(ui->dataClearButton, SIGNAL(clicked()), this, SLOT(onDataClearClicked()));  
  
  connect(ui->filterToolButton, SIGNAL(clicked()), this, SLOT(onFilterParametersClicked()));  
  connect(ui->regridToolButton, SIGNAL(clicked()), this, SLOT(onRegridParametersClicked()));  
  
  connect(ui->dateDateEdit, SIGNAL(dateChanged(QDate)), this, SLOT(onDateChanged()));  
  connect(ui->durationSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onDurationChanged()));  
  
  connect(ui->northDSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onBoundingGeoBoxChanged()));  
  connect(ui->southDSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onBoundingGeoBoxChanged()));  
  connect(ui->eastDSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onBoundingGeoBoxChanged()));  
  connect(ui->westDSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onBoundingGeoBoxChanged()));  

  connect(ui->selectedLayer1, SIGNAL(valueChanged(int)), this, SLOT(onSelectedLayer1Changed(int)));  
  connect(ui->selectedLayer2, SIGNAL(valueChanged(int)), this, SLOT(onSelectedLayer2Changed(int)));  
  connect(ui->selectedLayer3, SIGNAL(valueChanged(int)), this, SLOT(onSelectedLayer3Changed(int)));  
  connect(ui->selectedLayer4, SIGNAL(valueChanged(int)), this, SLOT(onSelectedLayer4Changed(int)));  
  connect(ui->selectedLayer5, SIGNAL(valueChanged(int)), this, SLOT(onSelectedLayer5Changed(int)));

  // initialize date and number of days
  onDateChanged();
  onDurationChanged();
  generateDefaultPngFilename();
}



//helper class
GeoDataPropertyEdit * createObjectBrowser(QWidget *parent) {
  const bool useClassName = false;
  const int useClassDepth = 1;   //need only one level class
  GeoDataPropertyEdit *pe = new GeoDataPropertyEdit(parent, useClassName, useClassDepth);
  pe->setHeader0Name("Downloaded Data");
  pe->setColumnWidth(0, 120);  
  pe->setColumnWidth(1, 80);    
  pe->resetProperties();
  pe->expandToDepth(0);
  pe->collapseAll();
  const int dockWidth = 1374;
  pe->setMinimumWidth(150);  
  pe->setMaximumWidth(dockWidth-100);
  pe->resize(dockWidth, 200);
  return pe;
}



//helper class
VisDataPropertyEdit* createVisObjectBrowser(QWidget *parent) {

  
  const bool useClassName = false;
  const int useClassDepth = 20;   //a large number, sufficient enough include all derived classes
  const int Col0Size = 250;
  VisDataPropertyEdit *pe = new VisDataPropertyEdit(parent, useClassName, useClassDepth);

  // do this to increase the vertical spacing via the sizeHint.
  pe->setItemDelegate(new VisDataPropertyDelegate());
    
  pe->setHeader0Name("Rendering Object");

  pe->setIndentation(20);
  pe->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);
  pe->setToolTip("Double click to open and modify visualization object properties.");
  pe->setUniformRowHeights(true);

  
  pe->setColumnWidth(0, Col0Size);  
  pe->setColumnWidth(1, pe->width()-Col0Size-10);  
  pe->resetProperties();
  pe->expandToDepth(0);
  //pe->collapseAll();
  pe->expandAll();
  const int dockWidth = 1374;
  pe->setMinimumWidth(150);
  pe->setMaximumWidth(dockWidth-100);
  pe->resize(dockWidth, 200);

  // actual screen size
  QDesktopWidget desktop;
  //int screenWidth  = desktop.screenGeometry().width();
  int screenHeight = desktop.screenGeometry().height();
  if (screenHeight < 2000) {
    pe->setMinimumHeight(600);
  } else {
    pe->setMinimumHeight(1800);
  }

  return pe;
}



void ControlDockWidget::initTab2ClientData() {
  // For the View and Info tabs

  // actual screen size
  QDesktopWidget desktop;
  //int screenWidth  = desktop.screenGeometry().width();
  int screenHeight = desktop.screenGeometry().height();

  
  m_viewSplitter = new QSplitter(Qt::Vertical, this->ui->viewTab);
  QSplitter* m_viewSplitterJunk = new QSplitter(); //unseen... this effectively gets rid of the splitter in the info tab

  m_viewSplitter->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);
  //m_viewSplitter->setMinimumHeight(600);
  //int h;
  //if (screenHeight > 2000) {
  //  h = 950;
  //  m_viewSplitter->setMinimumHeight(h*2 + 10);
  //} else {
  //  h = 300;
  //  m_viewSplitter->setMinimumHeight(h*2 + 10);
  //}
  m_viewSplitter->resize( m_viewSplitter->width(), ui->mainVerticalLayout->parentWidget()->size().height()-2);
  //m_viewSplitter->resize( m_viewSplitter->width(), ui->tabWidget->height()-2);

  
  GeoDataPropertyEdit* objPE = createObjectBrowser(m_viewSplitterJunk);
  VisDataPropertyEdit* visobjPE = createVisObjectBrowser(m_viewSplitter);

  objPE->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);
  visobjPE->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);
  
  //visobjPE->move(0, h+4);
  m_objectPropEdit = objPE;
  m_visPropEdit = visobjPE;
  connect(objPE, SIGNAL(propertyChanged()), this, SIGNAL(requestRedraw()));
  connect(visobjPE, SIGNAL(propertyChanged()), this, SIGNAL(requestRedraw()));
  
  //create object nodes
  const int N = 1;
  GeoDataObject* geoObjs[N] = {NULL};
  GeoDataList* datalist = GeoDataListSingleton::getInstance();

  if (m_objectPropEdit) {
    PropertyEditor::PropertyEdit *w = m_objectPropEdit;
    connect(datalist, SIGNAL(objectAdded(QObject*)), w, SLOT(addObject(QObject*)));
    connect(w, SIGNAL(objectDeleted(QObject *)), datalist, SLOT(onObjectRemoved(QObject*)));
    connect(w, SIGNAL(objectDeleted(QObject *)), m_visPropEdit, SLOT(onParentObjectRemoved(QObject*)));
  }

  //create visualization nodes, such as LUT & point renderer
  VisDataList* vislist = VisDataListSingleton::getInstance();

  
  if (m_visPropEdit) {
    PropertyEditor::PropertyEdit *w = m_visPropEdit;
    m_visPropEdit->resize( m_visPropEdit->width(), ui->mainVerticalLayout->parentWidget()->size().height()-5);

    //connect(vislist, SIGNAL(objectAdded(QObject*)), w, SLOT(addObject(QObject*)));
    connect(vislist, SIGNAL(objectAdded(QObject*)), w, SLOT(addObjectEMVL(QObject*)));
    connect(w, SIGNAL(objectDeleted(QObject *)), datalist, SLOT(onChildObjectRemoved(QObject*)));
    connect(w, SIGNAL(objectDeleted(QObject *)), vislist, SLOT(onObjectRemoved(QObject*)));

    if (geoObjs[0]!=NULL) {
      VisDataObject *renderer = geoObjs[0]->createDefaultVisualizationObject();

      if (renderer!=NULL) {
        connect(renderer, SIGNAL(valueChanged()), this, SIGNAL(requestRedraw()));
      }
    }
  }
  
  m_viewSplitterJunk->addWidget(m_objectPropEdit);
  m_viewSplitter->addWidget(m_visPropEdit);


  int extraHeight = 45;
  int extraWidth  = 1000;
  if (screenHeight > 2000) {
    // add height where needed
    //m_objectPropEdit->setMinimumHeight(m_objectPropEdit->height() + extraHeight);
    //m_visPropEdit->setMinimumHeight(m_visPropEdit->height()       + extraHeight);
    //m_viewSplitter->setMinimumHeight(m_viewSplitter->height()       + extraHeight);

    
    ui->groupBox_mode->setMinimumHeight(ui->groupBox_mode->height()                       + extraHeight);
    ui->groupBox_visibility->setMinimumHeight(ui->groupBox_visibility->height()           + extraHeight);
    ui->groupBox_opacity->setMinimumHeight(ui->groupBox_opacity->height()                 + extraHeight);
    ui->groupBox_minmax->setMinimumHeight(ui->groupBox_minmax->height()                   + extraHeight);
    ui->groupBox_threshold->setMinimumHeight(ui->groupBox_threshold->height()             + extraHeight);
    ui->groupBox_colormap->setMinimumHeight(ui->groupBox_colormap->height()               + extraHeight);
    ui->groupBox_scalefactor->setMinimumHeight(ui->groupBox_scalefactor->height()         + extraHeight);
    ui->groupBox_ruler->setMinimumHeight(ui->groupBox_ruler->height()                     + extraHeight);
    ui->groupBox_windcontrols->setMinimumHeight(ui->groupBox_windcontrols->height()       + extraHeight);
    ui->groupBox_specialcontrols->setMinimumHeight(ui->groupBox_specialcontrols->height() + extraHeight*10);
    ui->specialControlsStackedWidget->setMinimumHeight(ui->specialControlsStackedWidget->height() + extraHeight*10);

    ui->pageBLANK->setMinimumHeight(ui->pageBLANK->height()                     + extraHeight*10);
    ui->pageCMAQ->setMinimumHeight(ui->pageCMAQ->height()                       + extraHeight*10);
    ui->pageCEILOMETER->setMinimumHeight(ui->pageCEILOMETER->height()           + extraHeight*10);
    ui->pageNEUBREW->setMinimumHeight(ui->pageNEUBREW->height()                 + extraHeight*10);
    ui->pageBEHR->setMinimumHeight(ui->pageBEHR->height()                       + extraHeight*10);
    ui->pageMODIS->setMinimumHeight(ui->pageMODIS->height()                     + extraHeight*10);
    ui->pageVIIRS->setMinimumHeight(ui->pageVIIRS->height()                     + extraHeight*10);
    ui->pageTROPOMI->setMinimumHeight(ui->pageTROPOMI->height()                 + extraHeight*10);
    ui->pageHMS->setMinimumHeight(ui->pageHMS->height()                         + extraHeight*10);
    ui->pagePanel->setMinimumHeight(ui->pagePanel->height()                     + extraHeight*10);
    ui->pagePandora->setMinimumHeight(ui->pagePandora->height()                 + extraHeight*10);
    ui->pageTEMPO->setMinimumHeight(ui->pageTEMPO->height()                     + extraHeight*10);
    
    ui->groupBox_cmaq->setMinimumHeight(ui->groupBox_cmaq->height()                       + extraHeight*10);
    ui->groupBox_ceilometer->setMinimumHeight(ui->groupBox_ceilometer->height()           + extraHeight*10);
    ui->groupBox_neubrew->setMinimumHeight(ui->groupBox_neubrew->height()                 + extraHeight*10);
    ui->groupBox_behr->setMinimumHeight(ui->groupBox_behr->height()                       + extraHeight*10);
    ui->groupBox_modis->setMinimumHeight(ui->groupBox_modis->height()                     + extraHeight*10);
    ui->groupBox_viirs->setMinimumHeight(ui->groupBox_viirs->height()                     + extraHeight*10);
    ui->groupBox_tropomi->setMinimumHeight(ui->groupBox_tropomi->height()                 + extraHeight*10);
    ui->groupBox_hms->setMinimumHeight(ui->groupBox_hms->height()                         + extraHeight*10);
    ui->groupBox_panel->setMinimumHeight(ui->groupBox_panel->height()                     + extraHeight*10);
    ui->groupBox_pandora->setMinimumHeight(ui->groupBox_pandora->height()                 + extraHeight*10);
    ui->groupBox_tempo->setMinimumHeight(ui->groupBox_tempo->height()                     + extraHeight*10);

    
    ui->groupBox_minmax->setMinimumWidth(ui->groupBox_minmax->width() + extraWidth);

    
    //reposish as needed
    m_visPropEdit->setGeometry(m_visPropEdit->x(),
                               m_objectPropEdit->y() + m_objectPropEdit->height() + 2,
                               m_visPropEdit->width(),
                               m_visPropEdit->height());
    
    ui->groupBox_visibility->setGeometry(ui->groupBox_visibility->x(),
                                         ui->groupBox_mode->y() + ui->groupBox_mode->height() + 2,
                                         ui->groupBox_visibility->width(),
                                         ui->groupBox_visibility->height());
    ui->groupBox_opacity->setGeometry(ui->groupBox_opacity->x(),
                                      ui->groupBox_visibility->y() + ui->groupBox_visibility->height() + 2,
                                      ui->groupBox_opacity->width(),
                                      ui->groupBox_opacity->height());
    ui->groupBox_minmax->setGeometry(ui->groupBox_minmax->x(),
                                     ui->groupBox_opacity->y() + ui->groupBox_opacity->height() + 2,
                                     ui->groupBox_minmax->width(),
                                     ui->groupBox_minmax->height());
    ui->groupBox_threshold->setGeometry(ui->groupBox_threshold->x(),
                                        ui->groupBox_minmax->y() + ui->groupBox_minmax->height() + 2,
                                        ui->groupBox_threshold->width(),
                                        ui->groupBox_threshold->height());
    ui->groupBox_colormap->setGeometry(ui->groupBox_colormap->x(),
                                       ui->groupBox_threshold->y() + ui->groupBox_threshold->height() + 2,
                                       ui->groupBox_colormap->width(),
                                       ui->groupBox_colormap->height());
    ui->groupBox_scalefactor->setGeometry(ui->groupBox_scalefactor->x(),
                                          ui->groupBox_colormap->y() + ui->groupBox_colormap->height() + 2,
                                          ui->groupBox_scalefactor->width(),
                                          ui->groupBox_scalefactor->height());
    ui->groupBox_ruler->setGeometry(ui->groupBox_ruler->x(),
                                    ui->groupBox_scalefactor->y() + ui->groupBox_scalefactor->height() + 2,
                                    ui->groupBox_ruler->width(),
                                    ui->groupBox_ruler->height());
    ui->groupBox_windcontrols->setGeometry(ui->groupBox_windcontrols->x(),
                                           ui->groupBox_ruler->y() + ui->groupBox_ruler->height() + 2,
                                           ui->groupBox_windcontrols->width(),
                                           ui->groupBox_windcontrols->height());
    ui->groupBox_specialcontrols->setGeometry(ui->groupBox_specialcontrols->x(),
                                           ui->groupBox_windcontrols->y() + ui->groupBox_windcontrols->height() + 2,
                                           ui->groupBox_specialcontrols->width(),
                                           ui->groupBox_specialcontrols->height());

    ui->label_mode->setGeometry(0, ui->groupBox_mode->y(), ui->label_mode->width(), ui->label_mode->height());
    //ui->label_visibility->setGeometry(0, ui->groupBox_visibility->y(), ui->label_visibility->width(), ui->label_visibility->height());
    ui->label_opacity->setGeometry(0, ui->groupBox_opacity->y(), ui->label_opacity->width(), ui->label_opacity->height());
    ui->label_minmax->setGeometry(0, ui->groupBox_minmax->y(), ui->label_minmax->width(), ui->label_minmax->height());
    ui->label_threshold->setGeometry(0, ui->groupBox_threshold->y(), ui->label_threshold->width(), ui->label_threshold->height());
    ui->label_colormap->setGeometry(0, ui->groupBox_colormap->y(), ui->label_colormap->width(), ui->label_colormap->height());
    ui->label_scalefactor->setGeometry(0, ui->groupBox_scalefactor->y(), ui->label_scalefactor->width(), ui->label_scalefactor->height());
    ui->label_ruler->setGeometry(0, ui->groupBox_ruler->y(), ui->label_ruler->width(), ui->label_ruler->height());
    ui->WindLabel->setGeometry(0, ui->groupBox_windcontrols->y(), ui->WindLabel->width(), ui->WindLabel->height());
    ui->SpecialControlLabel->setGeometry(0, ui->groupBox_specialcontrols->y(), ui->SpecialControlLabel->width(), ui->SpecialControlLabel->height());

  }
    
  connect(ui->vizOpacitySlider, SIGNAL(valueChanged(int)), this, SLOT(onOpacitySliderChanged(int)) );
  connect(ui->ThresholdSlider, SIGNAL(valueChanged(int)), this, SLOT(onThresholdSliderChanged(int)) );
  connect(ui->ShowCMAQRow, SIGNAL(stateChanged(int)), this, SLOT(onShowCMAQRowChanged(int)) );
  connect(ui->ShowCMAQCol, SIGNAL(stateChanged(int)), this, SLOT(onShowCMAQColChanged(int)) );
  connect(ui->ShowCMAQLayer, SIGNAL(stateChanged(int)), this, SLOT(onShowCMAQLayerChanged(int)) );
  connect(ui->CMAQRowSlider, SIGNAL(valueChanged(int)), this, SLOT(onCMAQRowSliderChanged(int)) );
  connect(ui->CMAQColSlider, SIGNAL(valueChanged(int)), this, SLOT(onCMAQColSliderChanged(int)) );
  connect(ui->CMAQLayerSlider, SIGNAL(valueChanged(int)), this, SLOT(onCMAQLayerSliderChanged(int)) );
  connect(ui->vizCheckBox, SIGNAL(stateChanged(int)), this, SLOT(onVisibilityChanged(int)) );  
  connect(ui->cellBoundaryCheckbox, SIGNAL(stateChanged(int)), this, SLOT(onShowCellBoundaryChanged(int)) );  
  connect(ui->vizDataDefault, SIGNAL(stateChanged(int)), this, SLOT(onDataDefaultChanged(int)) );  
  connect(ui->vizMinSpinbox, SIGNAL(valueChanged(double)), this, SLOT(onMinSpinboxChanged(double)));
  connect(ui->vizMaxSpinbox, SIGNAL(valueChanged(double)), this, SLOT(onMaxSpinboxChanged(double)));
  connect(ui->thresholdCheckBox, SIGNAL(stateChanged(int)), this, SLOT(onThresholdCheckboxChanged(int)) );  
  connect(ui->rulerCheckBox, SIGNAL(stateChanged(int)), this, SLOT(onRulerCheckboxChanged(int)) );  
  
  // turn thresholding off since we always start in 2D mode
  ui->ThresholdSlider->setEnabled(false);
  ui->ThresholdLabel->setEnabled(false);
  ui->thresholdCheckBox->setChecked(false);;
  
  // turn off 3D ruler since we start in 2D
  ui->rulerCheckBox->setEnabled(false);
  ui->rulerLonSpinBox->setEnabled(false);
  ui->rulerLatSpinBox->setEnabled(false);
  
  // turn on layer option since we are starting in 2D mode
  ui->ShowCMAQLayer->setEnabled(false);
  
  // turn off CMAQ row/colum since we are starting in 2D mode
  ui->ShowCMAQRow->setEnabled(false);
  ui->ShowCMAQCol->setEnabled(false);
  ui->groupBox_minmax->setStyleSheet("QGroupBox {border:0;}");
  //ui->groupBox_minmax->setStyleSheet("QGroupBox {border:1px solid gray;}");
  ui->groupBox_mode->setStyleSheet("QGroupBox {border:0;}");
  ui->groupBox_visibility->setStyleSheet("QGroupBox {border:0;}");
  ui->groupBox_scalefactor->setStyleSheet("QGroupBox {border:0;}");
  ui->groupBox_opacity->setStyleSheet("QGroupBox {border:0;}");
  ui->groupBox_colormap->setStyleSheet("QGroupBox {border:0;}");
  ui->groupBox_specialcontrols->setStyleSheet("QGroupBox {border:0;}");
  ui->groupBox_threshold->setStyleSheet("QGroupBox {border:0;}");
  //ui->groupBox_threshold->setStyleSheet("QGroupBox {border:1px solid gray;}");
  ui->groupBox_ruler->setStyleSheet("QGroupBox {border:0;}");
  ui->groupBox_windcontrols->setStyleSheet("QGroupBox {border:0;}");
  ui->groupBox_mapprojbound->setStyleSheet("QGroupBox {border:0;}");
  ui->groupBox_mapserver->setStyleSheet("QGroupBox {border:0;}");
  ui->groupBox_mapoptions->setStyleSheet("QGroupBox {border:0;}");
  ui->groupBox_mapother->setStyleSheet("QGroupBox {border:0;}");
  ui->groupBox_timestep->setStyleSheet("QGroupBox {border:0;}");
  // and add a border around others
  ui->projectionsGroupBox->setStyleSheet("QGroupBox {border:1px solid gray;}");
  ui->boundariesGroupBox->setStyleSheet("QGroupBox {border:1px solid gray;}");
  ui->miscGroupBox->setStyleSheet("QGroupBox {border:1px solid gray;}");
  ui->groupBox_20->setStyleSheet("QGroupBox {border:1px solid gray;}");
  ui->groupBox_21->setStyleSheet("QGroupBox {border:1px solid gray;}");


  //ui->groupBox_cmaq->setStyleSheet("QGroupBox {border:1px solid gray;}");
  //ui->groupBox_specialcontrols->setStyleSheet("QGroupBox {border:1px solid red;}");
  //ui->pageCMAQ->setStyleSheet("QGroupBox {border:1px solid green;}");
  ui->groupBox_cmaq->setStyleSheet("QGroupBox {border:0;}");
  ui->groupBox_specialcontrols->setStyleSheet("QGroupBox {border:0;}");
  ui->pageCMAQ->setStyleSheet("QGroupBox {border:0;}");


  
  ui->mainVerticalLayout->setAlignment(Qt::AlignTop);
  
  addColormapHelper();
  ui->scenarioComboBox->setCurrentIndex(0);
  connect(ui->vizColormapCombobox, SIGNAL(activated(int)), this, SLOT(onColormapIndexChanged(int)));
  connect(ui->vizVarCombobox, SIGNAL(activated(int)), this, SLOT(onVizVarIndexChanged(int)));
  
  connect(ui->radioButton2D, SIGNAL(toggled(bool)), this, SLOT(onRadioButton2DToggled(bool)));
  connect(ui->radioButton3D, SIGNAL(toggled(bool)), this, SLOT(onRadioButton3DToggled(bool)));
  connect(ui->radioButtonPanelDisplay, SIGNAL(toggled(bool)), this, SLOT(onRadioButtonPanelDisplayToggled(bool)));

  connect(ui->flattenCMAQ, SIGNAL(stateChanged(int)), this, SLOT(onCheckboxFlattenCMAQChanged(int)));
  connect(ui->persistBEHR, SIGNAL(stateChanged(int)), this, SLOT(onCheckboxPersistBEHRChanged(int)));
  connect(ui->persistMODIS, SIGNAL(stateChanged(int)), this, SLOT(onCheckboxPersistMODISChanged(int)));
  connect(ui->persistVIIRS, SIGNAL(stateChanged(int)), this, SLOT(onCheckboxPersistVIIRSChanged(int)));
  connect(ui->persistTROPOMI, SIGNAL(stateChanged(int)), this, SLOT(onCheckboxPersistTROPOMIChanged(int)));
  connect(ui->persistTEMPO, SIGNAL(stateChanged(int)), this, SLOT(onCheckboxPersistTEMPOChanged(int)));
  connect(ui->persistHMS, SIGNAL(stateChanged(int)), this, SLOT(onCheckboxPersistHMSChanged(int)));
  
  connect(ui->persistBEHRHours, SIGNAL(valueChanged(int)), this, SLOT(onPersistBEHRHoursChanged(int)));
  connect(ui->persistMODISHours, SIGNAL(valueChanged(int)), this, SLOT(onPersistMODISHoursChanged(int)));
  connect(ui->persistVIIRSHours, SIGNAL(valueChanged(int)), this, SLOT(onPersistVIIRSHoursChanged(int)));
  connect(ui->persistTROPOMIHours, SIGNAL(valueChanged(int)), this, SLOT(onPersistTROPOMIHoursChanged(int)));
  connect(ui->persistTEMPOHours, SIGNAL(valueChanged(int)), this, SLOT(onPersistTEMPOHoursChanged(int)));
  connect(ui->persistHMSHours, SIGNAL(valueChanged(int)), this, SLOT(onPersistHMSHoursChanged(int)));
  
  connect(ui->resetView, SIGNAL(clicked()), this, SLOT(onViewResetPressed()));
  connect(ui->autozoomView, SIGNAL(clicked()), this, SLOT(onViewAutozoomPressed()));
  
  connect(ui->calipsoScaleFactor, SIGNAL(valueChanged(double)), this, SLOT(onCalipsoScaleFactorChanged(double)));
  connect(ui->windVectorScaleFactor, SIGNAL(valueChanged(double)), this, SLOT(onWindVectorScaleFactorChanged(double)));
  connect(ui->windVectorTipSize, SIGNAL(valueChanged(double)), this, SLOT(onWindVectorTipSizeChanged(double)));
  connect(ui->windVectorSparseness, SIGNAL(valueChanged(double)), this, SLOT(onWindVectorSparsenessChanged(double)));
  connect(ui->autoSparseCheckBox, SIGNAL(toggled(bool)), this, SLOT(onWindVectorAutosparseChanged(bool)));
  
  ui->windVectorBarbOption->setEnabled(false);
  
  connect(ui->ceilometerMinimumAltitude, SIGNAL(valueChanged(double)), this, SLOT(onCeilometerMinimumAltitudeChanged(double)));
  connect(ui->ceilometerMaximumAltitude, SIGNAL(valueChanged(double)), this, SLOT(onCeilometerMaximumAltitudeChanged(double)));
  connect(ui->ceilometerPlotScroller, SIGNAL(valueChanged(int)), this, SLOT(onCeilometerPlotScrollerChanged(int)));
  connect(ui->ceilometerSize, SIGNAL(valueChanged(int)), this, SLOT(onCeilometerSizeChanged(int)));
  
  connect(ui->pandoraMinimumAltitude, SIGNAL(valueChanged(double)), this, SLOT(onPandoraMinimumAltitudeChanged(double)));
  connect(ui->pandoraMaximumAltitude, SIGNAL(valueChanged(double)), this, SLOT(onPandoraMaximumAltitudeChanged(double)));
  connect(ui->pandoraPlotScroller, SIGNAL(valueChanged(int)), this, SLOT(onPandoraPlotScrollerChanged(int)));
  connect(ui->pandoraSize, SIGNAL(valueChanged(int)), this, SLOT(onPandoraSizeChanged(int)));
  
  connect(ui->neubrewMinimumAltitude, SIGNAL(valueChanged(double)), this, SLOT(onNeubrewMinimumAltitudeChanged(double)));
  connect(ui->neubrewMaximumAltitude, SIGNAL(valueChanged(double)), this, SLOT(onNeubrewMaximumAltitudeChanged(double)));
  connect(ui->neubrewPlotScroller, SIGNAL(valueChanged(int)), this, SLOT(onNeubrewPlotScrollerChanged(int)));
  connect(ui->neubrewSize, SIGNAL(valueChanged(int)), this, SLOT(onNeubrewSizeChanged(int)));
  
  connect(ui->rulerLonSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onRulerLonSpinBoxChanged(double)));
  connect(ui->rulerLatSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onRulerLatSpinBoxChanged(double)));
  connect(ui->ceilometerShowLocations, SIGNAL(toggled(bool)), this, SLOT(onCeilometerShowLocationsToggled(bool)));
  connect(ui->ceilometerShowMixingLayer, SIGNAL(toggled(bool)), this, SLOT(onCeilometerShowMixingLayerToggled(bool)));
  connect(ui->neubrewShowLocations, SIGNAL(toggled(bool)), this, SLOT(onNeubrewShowLocationsToggled(bool)));
  connect(ui->pandoraShowLocations, SIGNAL(toggled(bool)), this, SLOT(onPandoraShowLocationsToggled(bool)));
  
  // set certain default vis options
  VisOptions *vOptions = VisOptionsSingleton::getInstance();
  vOptions->setRulerLon(ui->rulerLonSpinBox->value());
  vOptions->setRulerLat(ui->rulerLatSpinBox->value());
}



void ControlDockWidget::initTab3ClientData() {
  // for the Save tab

  // actual screen size
  QDesktopWidget desktop;
  //int screenWidth  = desktop.screenGeometry().width();
  int screenHeight = desktop.screenGeometry().height();

  int extraHeight = 85;
  //int extraWidth  = 1000;
  if (screenHeight > 2000) {
    // add height where needed
    ui->groupBox_savetab->setMinimumHeight(ui->groupBox_savetab->height()     + extraHeight*4);

  }
  
  QFont viewTabFont = ui->viewTab->font();
  viewTabFont.setPixelSize(11);
  ui->viewTab->setFont(viewTabFont);
  ui->currentpngRadioButton->setChecked(true); // default selection
  //connect signals for movie making process
  connect(&m_ffmpeg, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(onFFMPEGFinished(int, QProcess::ExitStatus)));
  connect(&m_ffmpeg, SIGNAL(error(QProcess::ProcessError)), this, SLOT(onFFMPEGError(QProcess::ProcessError)));
  
  //connect signals for RSIG updator process  
  connect(&m_updator, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(onUpdateRsigFinished(int, QProcess::ExitStatus)));
  connect(&m_updator, SIGNAL(error(QProcess::ProcessError)), this, SLOT(onUpdateRsigError(QProcess::ProcessError)));
  
  connect(ui->writetofileToolButton, SIGNAL(clicked()), this, SLOT(onWriteToFileClicked()));  
  connect(ui->savefilePushButton, SIGNAL(clicked()), this, SLOT(onSaveFileClicked()));  
  
  ui->dirnameLineEdit->setText(QDir::homePath()+"/");
  ui->savevideoPushButton->setCheckable(true);
  ui->savevideoPushButton->setChecked(false);
  ui->savevideoPushButton->setVisible(false);     // not using at the moment
  ui->screenRecordRadioButton->setVisible(false); // not using at the moment

  connect(ui->savevideoPushButton, SIGNAL(toggled(bool)), this, SLOT(onSaveVideoToggled(bool)));
  
  m_snapshotTimer.setInterval(0);
  m_snapshotTimer.setSingleShot(false);
  connect(&m_snapshotTimer, SIGNAL(timeout()), this, SIGNAL(requestSnapshot()));

  connect(ui->scenarioRadioButton, SIGNAL(toggled(bool)), this, SLOT(onSaveTypeClicked()));
  connect(ui->binaryxdrRadioButton, SIGNAL(toggled(bool)), this, SLOT(onSaveTypeClicked()));
  connect(ui->asciisheetRadioButton,  SIGNAL(toggled(bool)), this, SLOT(onSaveTypeClicked()));
  connect(ui->netcdfcoardsRadioButton, SIGNAL(toggled(bool)), this, SLOT(onSaveTypeClicked()));
  connect(ui->netcdfioapiRadioButton,  SIGNAL(toggled(bool)), this, SLOT(onSaveTypeClicked()));
  connect(ui->originalFileRadioButton,  SIGNAL(toggled(bool)), this, SLOT(onSaveTypeClicked()));
  connect(ui->geotiffRadioButton,  SIGNAL(toggled(bool)), this, SLOT(onSaveTypeClicked()));
  connect(ui->shapeRadioButton,  SIGNAL(toggled(bool)), this, SLOT(onSaveTypeClicked()));
  connect(ui->originalFileRadioButton, SIGNAL(toggled(bool)), this, SLOT(onSaveTypeClicked()));
  connect(ui->kmlRadioButton, SIGNAL(toggled(bool)), this, SLOT(onSaveTypeClicked()));
  connect(ui->mpegRadioButton, SIGNAL(toggled(bool)), this, SLOT(onSaveTypeClicked()));
  connect(ui->screenRecordRadioButton, SIGNAL(toggled(bool)), this, SLOT(onSaveTypeClicked()));
  connect(ui->geotiffrgbRadioButton, SIGNAL(toggled(bool)), this, SLOT(onSaveTypeClicked()));
  connect(ui->currentpngRadioButton, SIGNAL(toggled(bool)), this, SLOT(onSaveTypeClicked()));
  connect(ui->highrespngRadioButton, SIGNAL(toggled(bool)), this, SLOT(onSaveTypeClicked()));
  connect(ui->originalFileRadioButton, SIGNAL(toggled(bool)), this, SLOT(onSaveTypeClicked()));
  connect(ui->allpngsRadioButton, SIGNAL(toggled(bool)), this, SLOT(onSaveTypeClicked()));
  connect(ui->filenameLineEdit, SIGNAL(textChanged(QString)), this, SLOT(onSaveFilenameChanged()));

  connect(ui->panelDisplaySource1, SIGNAL(activated(int)), this, SLOT(onPanelDisplaySource1Changed(int)));
  connect(ui->panelDisplaySource2, SIGNAL(activated(int)), this, SLOT(onPanelDisplaySource2Changed(int)));
  connect(ui->panelDisplayListWidget, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(onPanelDisplayListWidgetClicked()));
  
  ui->filenameLineEdit->show();
  ui->filename_generated->hide();
  ui->filenameLineEdit->setText( VisOptionsSingleton::getInstance()->getFilenamePngDefault() );
  ui->filename_generated->setText( VisOptionsSingleton::getInstance()->getFilenamePngDefault() );
}



void ControlDockWidget::initTab4ClientData() {


  QDesktopWidget desktop;
  //int screenWidth  = desktop.screenGeometry().width();
  int screenHeight = desktop.screenGeometry().height();
  
  int extraHeight = 85;
  //int extraWidth  = 1000;
  if (screenHeight > 2000) {
    // add height where needed
    ui->groupBox_rsigweb->setMinimumHeight(ui->groupBox_rsigweb->height()     + extraHeight*4);
  }


  ui->timestepSlider->setMaximum(23); //default, until data is read
  ui->timestepSlider->setMinimum(0); 
  connect(ui->timestepSlider, SIGNAL(valueChanged(int)), this, SLOT(ontimestepSliderChanged(int)) );
  connect(ui->playButton, SIGNAL(pressed()), this, SLOT(onPlayButtonClicked()) );
  connect(ui->forwardButton, SIGNAL(pressed()), this, SLOT(onForwardButtonClicked()) );
  connect(ui->forwardButton24, SIGNAL(pressed()), this, SLOT(onForwardButton24Clicked()) );
  connect(ui->backButton, SIGNAL(pressed()), this, SLOT(onBackButtonClicked()) );
  connect(ui->backButton24, SIGNAL(pressed()), this, SLOT(onBackButton24Clicked()) );
  connect(ui->animationSpeed, SIGNAL(valueChanged(int)), this, SLOT(onAnimationSpeedChanged(int)) );

  QSignalMapper* signalMapper = new QSignalMapper(this) ;
  connect(ui->datausePushButton, SIGNAL(clicked()), signalMapper, SLOT(map()));
  signalMapper -> setMapping (ui->datausePushButton, 0) ;
  connect (signalMapper, SIGNAL(mapped(int)), this, SLOT(onShowDataUsePolicy(int))) ;

  QSignalMapper* signalMapper2 = new QSignalMapper(this) ;
  connect(ui->tipsPushButton, SIGNAL(clicked()), signalMapper2, SLOT(map()));
  signalMapper2 -> setMapping (ui->tipsPushButton, 1) ;
  connect (signalMapper2, SIGNAL(mapped(int)), this, SLOT(initRSIG3DHelp(int))) ;
  connect(ui->downloaderPushButton, SIGNAL(clicked()), this, SLOT(onFileOrientedDownloaderClicked()));  
  connect(ui->openwebPushButton, SIGNAL(clicked()), this, SLOT(onOpenWebClicked()));
  connect(ui->emailPushButton, SIGNAL(clicked()), this, SLOT(onEmailClicked()));
  connect(ui->exitPushButton, SIGNAL(clicked()), this, SLOT(onExitClicked()));
  connect(ui->checknowPushButton, SIGNAL(clicked()), this, SLOT(onCheckUpdateClicked()));


  QFont font = qApp->font();
  ui->guiFontsizeSpinbox->setValue(font.pointSize());
  #ifdef __APPLE__
    const int guiFontSize = 11;
  #else
    const int guiFontSize = 8;
  #endif
  ui->guiFontsizeSpinbox->setValue(guiFontSize);
  this->onGuiFontsizeChanged(guiFontSize);
  
}


void ControlDockWidget::readDataPrefs() {

  DataPreferences* const dataPreferences = DataPreferencesSingleton::getInstance();
  dataPreferences->readDataPrefs();
  QMap<QString, DataPreferences::DataProperties> dplt = dataPreferences->getDataPrefsLookupTable();
 
}
