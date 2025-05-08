
#ifndef INC_DATAPREFERENCES_H_
#define INC_DATAPREFERENCES_H_

#include <QMap>
#include <QString>
#include <QObject>
#include "Singleton.h"
#include "DataPreferences.h"

class DataPreferences: public QObject {
  Q_OBJECT

 
 public:

  struct DataProperties {
    double min;
    double max;
    QString colormap;
  };
  
  QMap<QString, DataProperties> dataPrefsLookupTable;
  void readDataPrefs();
  QMap<QString, DataProperties> getDataPrefsLookupTable();

  
 protected:
  DataPreferences();
  ~DataPreferences();
  
 private:
  Q_DISABLE_COPY(DataPreferences);
  friend class Singleton<DataPreferences>;

};
  
  typedef Singleton<DataPreferences> DataPreferencesSingleton;
  
#endif  //INC_DATAPREFERENCES_H_
