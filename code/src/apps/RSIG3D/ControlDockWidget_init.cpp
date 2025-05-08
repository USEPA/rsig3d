/******************************************************************************
PURPOSE: ControlDockWidget_init.cpp - Implements openglscene.

NOTES: Implemented some slot functions of ControlDockWidget class.

HISTORY: 2012/11/12, zhang.nan@epa.gov, Created.
******************************************************************************/

#include <iostream>
#include <string>

#include <QApplication>        //qApp, slot closeAllWindows() 
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

  QStringList parse;
  bool ok;
  QTextStream s( &f );
  int ind=0;

  while (!s.atEnd()) {
    QString line = s.readLine();
    parse = line.split(' ');
    availNames[ind] = parse[0];
    minAvailDate[ind] = parse[1].toLong(&ok, 10);
    maxAvailDate[ind] = parse[2].toLong(&ok, 10);
    ind += 1;
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
  QFont twFont = tw->font();
  twFont.setPixelSize(12);
  tw->setFont(twFont);
  
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

  QFont font = qApp->font();
  ui->guiFontsizeSpinbox->setValue(font.pointSize());
   
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
  ui->selectedData1->setFixedWidth(selectedDataLongWidth);
  ui->selectedData2->setFixedWidth(selectedDataLongWidth);
  ui->selectedData3->setFixedWidth(selectedDataLongWidth);
  ui->selectedData4->setFixedWidth(selectedDataLongWidth);
  ui->selectedData5->setFixedWidth(selectedDataLongWidth);
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
  ui->trueColorComboBox->addItem("VIIRS");

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
  QFont labFont = ui->label_timestep->font();
  labFont.setPixelSize(14);
  ui->label_timestep->setFont(labFont);

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
  VisDataPropertyEdit *pe = new VisDataPropertyEdit(parent, useClassName, useClassDepth);
  pe->setHeader0Name("Rendering Object");  
  pe->setColumnWidth(0, 120);  
  pe->setColumnWidth(1, pe->width()-120-10);  
  pe->resetProperties();
  pe->expandToDepth(0);
  pe->collapseAll();
  const int dockWidth = 1374;
  pe->setMinimumWidth(150);
  pe->setMaximumWidth(dockWidth-100);
  pe->resize(dockWidth, 200);
  return pe;
}



void ControlDockWidget::initTab2ClientData() {
  // For the Info tab

  m_viewSplitter = new QSplitter(Qt::Vertical, this->ui->viewTab);
  GeoDataPropertyEdit* objPE = createObjectBrowser(m_viewSplitter);
  VisDataPropertyEdit* visobjPE = createVisObjectBrowser(m_viewSplitter);
  const int h = objPE->height();
  visobjPE->move(0, h+4);
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
    connect(vislist, SIGNAL(objectAdded(QObject*)), w, SLOT(addObject(QObject*)));
    connect(w, SIGNAL(objectDeleted(QObject *)), datalist, SLOT(onChildObjectRemoved(QObject*)));
    connect(w, SIGNAL(objectDeleted(QObject *)), vislist, SLOT(onObjectRemoved(QObject*)));

    if (geoObjs[0]!=NULL) {
      VisDataObject *renderer = geoObjs[0]->createDefaultVisualizationObject();

      if (renderer!=NULL) {
        connect(renderer, SIGNAL(valueChanged()), this, SIGNAL(requestRedraw()));
      }
    }
  }
  
  m_viewSplitter->addWidget(m_objectPropEdit);
  m_viewSplitter->addWidget(m_visPropEdit);

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
  
  // make groupboxes invisible
  ui->groupBox_4->setStyleSheet("QGroupBox {border:0;}");
  ui->groupBox_6->setStyleSheet("QGroupBox {border:0;}");
  ui->groupBox_7->setStyleSheet("QGroupBox {border:0;}");
  ui->groupBox_8->setStyleSheet("QGroupBox {border:0;}");
  ui->groupBox_9->setStyleSheet("QGroupBox {border:0;}");
  ui->groupBox_10->setStyleSheet("QGroupBox {border:0;}");
  ui->groupBox_11->setStyleSheet("QGroupBox {border:0;}");
  ui->groupBox_12->setStyleSheet("QGroupBox {border:0;}");
  ui->groupBox_13->setStyleSheet("QGroupBox {border:0;}");
  ui->groupBox_14->setStyleSheet("QGroupBox {border:0;}");
  
  ui->boundingGroupBox->setStyleSheet("QGroupBox {border:0;}");
  ui->dateGroupBox->setStyleSheet("QGroupBox {border:0;}");
  
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
}


void ControlDockWidget::readDataPrefs() {

  DataPreferences* const dataPreferences = DataPreferencesSingleton::getInstance();
  dataPreferences->readDataPrefs();
  QMap<QString, DataPreferences::DataProperties> dplt = dataPreferences->getDataPrefsLookupTable();
 
}
