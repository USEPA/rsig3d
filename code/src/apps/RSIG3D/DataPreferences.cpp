
#include <QDir>
#include <QString>
#include <QMap>
#include <QSettings>
#include <QStringList>
#include <QDebug>
#include "DataPreferences.h"
#include "MainWin.h"
#include "ControlDockWidget.h"


DataPreferences::DataPreferences() : QObject() {
  // default settings
  const DataProperties dataProperties = { 0.0, 10.0, QString("Blue-red") };
  dataPrefsLookupTable.insert( QString("default"), dataProperties );
}


DataPreferences::~DataPreferences() {
}


void DataPreferences::readDataPrefs() {

  MainWin& mainWin = MainWin::getInstance();
  ControlDockWidget* const controlDockWidget = mainWin.controlWidget();
  
  QString homeDir = QDir::homePath();
  QString iniFilePath = homeDir + "/.rsig/rsig_data_prefs.ini";

  if (QFile::exists(iniFilePath)) {
    QString msg = "Reading " + iniFilePath;
    controlDockWidget->postMessage(msg, RSIGState::NORMAL_TEXT);
  } else {
    QString msg  = "Data preferences file not found: " + iniFilePath;
    QString msg2 = "Creating: " + iniFilePath;
    QString msg3 = "You can edit this file to customize your data display preferences";    
    controlDockWidget->postMessage(msg,  RSIGState::WARNING_TEXT);
    controlDockWidget->postMessage(msg2, RSIGState::WARNING_TEXT);
    controlDockWidget->postMessage(msg3, RSIGState::WARNING_TEXT);
    
    QFile file(iniFilePath);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    out << "; Add data preferences here like the commented example below to\n";
    out << "; specify the min, max, and colormap of any variables you want.\n";    
    out << "; [tempo.l2.no2.vertical_column_sum]\n";
    out << "; min=0.0\n";
    out << "; max=1.5e16\n";
    out << "; colormap=Blue-red\n";
    out << "\n";
    file.close(); 
  }


  // Create a QSettings object
  QSettings settings(iniFilePath, QSettings::IniFormat);

  // get keys
  QStringList groups = settings.childGroups(); // Get all groups. Each group is an rsig variable (e.g. tempo.l2.no2.vertical_column_sum)

  double mymin = 0.0;
  double mymax = 0.0;
  QString mycolormap;
  DataProperties myProperties;

  for (int groupind=0; groupind<groups.size(); groupind++) {
    //qDebug() << groupind << groups[groupind];
    
    QString myVar      = groups[groupind]; // e.g. tempo.l2.no2.vertical_column_sum
    QString myVarMunge = myVar.replace(".", "_");
    
    settings.beginGroup(groups[groupind]);
    QStringList keys = settings.childKeys();
    bool foundMin      = false;
    bool foundMax      = false;
    bool foundColormap = false;
    for (int keyind=0; keyind<keys.size(); keyind++) {

     
      QString myProperty = keys[keyind];     // e.g. min, max, colormap, etc
      QVariant myValue   = settings.value(keys[keyind]); // Get the data value

      if (myProperty == "min")      { mymin      = myValue.toDouble(); foundMin = true; }
      if (myProperty == "max")      { mymax      = myValue.toDouble(); foundMax = true; }
      if (myProperty == "colormap") { mycolormap = myValue.toString(); foundColormap = true; }

      // myProperties = {mymin, mymax, mycolormap};
      myProperties.min = mymin;
      myProperties.max = mymax;
      myProperties.colormap = mycolormap;
    }

    if (!foundMin) {
       QString msg = myVarMunge + " min not found, using default";
       controlDockWidget->postMessage(msg, RSIGState::WARNING_TEXT);
    }
    if (!foundMax) {
      QString msg = myVarMunge + " max not found, using default";
       controlDockWidget->postMessage(msg, RSIGState::WARNING_TEXT);
    }
    if (!foundColormap) {
      QString msg = myVarMunge + " colormap not found, using default";
       controlDockWidget->postMessage(msg, RSIGState::WARNING_TEXT);
    }
    
    settings.endGroup();
    
    dataPrefsLookupTable.insert(myVarMunge, myProperties);

  }
 
}

QMap<QString,  DataPreferences::DataProperties> DataPreferences::getDataPrefsLookupTable() {
  return dataPrefsLookupTable;
}
