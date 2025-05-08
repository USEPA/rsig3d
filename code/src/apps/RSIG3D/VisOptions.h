#ifndef INC_VISOPTIONS_H_
#define INC_VISOPTIONS_H_

#include <QObject>
#include "Singleton.h"
#include <QStringList>

class VisOptions: public QObject {

  Q_OBJECT

protected:
  VisOptions();
  ~VisOptions();

private:
  //bool mode2D;
  int displayMode;
  bool flattenCMAQ;
  bool persistBEHR;
  bool persistMODIS;
  bool persistVIIRS;
  bool persistTROPOMI;
  bool persistTEMPO;
  bool persistHMS;
  bool drawModisTrueColor;
  float opacityModisTrueColor;
  QString trueColorType;
  int persistBEHRHours;
  int persistMODISHours;
  int persistVIIRSHours;
  int persistTROPOMIHours;
  int persistTEMPOHours;
  int persistHMSHours;
  bool showRuler;
  int viewResetFlag; // 0=do_nothing, 1=reset, 2=lambert, 3=stereographic, 4=autozoom
  int curProjection;
  int lonlatlabelOrientation;
  double calipsoScaleFactor;
  double windVectorScaleFactor;
  double windVectorSparseness;
  bool   windVectorAutosparse;
  int windVectorTipSize;
  double ceilometerMinAltitude;
  double ceilometerMaxAltitude;
  double pandoraMinAltitude;
  double pandoraMaxAltitude;
  double neubrewMinAltitude;
  double neubrewMaxAltitude;
  int ceilometerPlotScrollerValue;
  int pandoraPlotScrollerValue;
  int neubrewPlotScrollerValue;
  int ceilometerSize;
  int pandoraSize;
  int neubrewSize;
  bool ceilometerShowLocations;
  bool pandoraShowLocations;
  bool ceilometerShowMixingLayer;
  bool neubrewShowLocations;
  double rulerLon;
  double rulerLat;
  double bboxMinLon; // bbox coord of last data retrieval
  double bboxMinLat; // bbox coord of last data retrieval
  double bboxMaxLon; // bbox coord of last data retrieval
  double bboxMaxLat; // bbox coord of last data retrieval
  int currentDayNum;
  QString filenameScenario;
  QString filenamePng;
  QString filenamePngDefault;
  QString saveDir;
  QString mapOverlayFile;
  QString maplineColor;
  int maplineColorIndex;
  bool saveMetadataFlag;
  int saveFileFormatIndex;
  QStringList dataSourceList;
  QString uniqueID;

  QString windVectorLabel;
  double windVectorLength; // in pixels

  int downloadsOutstanding; // keeps track of number of downloads in progress
  
  friend class Singleton<VisOptions>;

public:
  
  QStringList getDataSourceList() const;
  void setDataSourceList(QStringList x);
  
  QString getFilenameScenario() const;
  void setFilenameScenario(QString x);

  QString getFilenamePng() const;
  void setFilenamePng(QString x);

  QString getFilenamePngDefault() const;
  void setFilenamePngDefault(QString x);

  QString getSaveDir() const;
  void setSaveDir(QString x);
  
  //bool getMode2D() const;
  //void setMode2D(bool x);
  int  getDisplayMode() const;
  void setDisplayMode(int x);

  int getViewResetFlag() const;
  void setViewResetFlag(int x);

  int getCurProjection() const;
  void setCurProjection(int x);

  int getLonlatLabelOrientation() const;
  void setLonlatLabelOrientation(int val);

  bool getFlattenCMAQ() const;
  void setFlattenCMAQ(bool x);

  bool getPersistBEHR() const;
  void setPersistBEHR(bool x);

  bool getPersistMODIS() const;
  void setPersistMODIS(bool x);

  bool getPersistVIIRS() const;
  void setPersistVIIRS(bool x);

  bool getPersistTROPOMI() const;
  void setPersistTROPOMI(bool x);

  bool getPersistTEMPO() const;
  void setPersistTEMPO(bool x);

  bool getPersistHMS() const;
  void setPersistHMS(bool x);
  
  bool getDrawModisTrueColor() const;
  void setDrawModisTrueColor(bool x);

  float getOpacityModisTrueColor() const;
  void setOpacityModisTrueColor(float x);

  QString getTrueColorType() const;
  void setTrueColorType(QString x);

  int getPersistBEHRHours() const;
  void setPersistBEHRHours(int value);

  int getPersistMODISHours() const;
  void setPersistMODISHours(int value);

  int getPersistVIIRSHours() const;
  void setPersistVIIRSHours(int value);

  int getPersistTROPOMIHours() const;
  void setPersistTROPOMIHours(int value);

  int getPersistTEMPOHours() const;
  void setPersistTEMPOHours(int value);

  int getPersistHMSHours() const;
  void setPersistHMSHours(int value);
  
  bool getShowRuler() const;
  void setShowRuler(bool x);

  bool getCeilometerShowLocations() const;
  void setCeilometerShowLocations(bool x);

  bool getCeilometerShowMixingLayer() const;
  void setCeilometerShowMixingLayer(bool x);

  bool getPandoraShowLocations() const;
  void setPandoraShowLocations(bool x);
  
  bool getNeubrewShowLocations() const;
  void setNeubrewShowLocations(bool x);

  double getCalipsoScaleFactor() const;
  void setCalipsoScaleFactor(double value);

  double getWindVectorScaleFactor() const;
  void setWindVectorScaleFactor(double value);

  double getWindVectorTipSize() const;
  void setWindVectorTipSize(double value);

  double getWindVectorSparseness() const;
  void setWindVectorSparseness(double value);

  bool getWindVectorAutosparse() const;
  void setWindVectorAutosparse(bool x);
  
  double getCeilometerMinAltitude() const;
  void setCeilometerMinAltitude(double value);

  double getCeilometerMaxAltitude() const;
  void setCeilometerMaxAltitude(double value);

  int getCeilometerPlotScrollerValue() const;
  void setCeilometerPlotScrollerValue(int value);
  
  int getCeilometerSize() const;
  void setCeilometerSize(int value);

  double getPandoraMinAltitude() const;
  void setPandoraMinAltitude(double value);

  double getPandoraMaxAltitude() const;
  void setPandoraMaxAltitude(double value);

  int getPandoraPlotScrollerValue() const;
  void setPandoraPlotScrollerValue(int value);
  
  int getPandoraSize() const;
  void setPandoraSize(int value);

  double getNeubrewMinAltitude() const;
  void setNeubrewMinAltitude(double value);

  double getNeubrewMaxAltitude() const;
  void setNeubrewMaxAltitude(double value);

  int getNeubrewPlotScrollerValue() const;
  void setNeubrewPlotScrollerValue(int value);
  
  int getNeubrewSize() const;
  void setNeubrewSize(int value);
  
  double getRulerLon() const;
  void setRulerLon(double value);

  double getRulerLat() const;
  void setRulerLat(double value);

  double getBboxMinLon() const;
  void setBboxMinLon(double value);

  double getBboxMinLat() const;
  void setBboxMinLat(double value);

  double getBboxMaxLon() const;
  void setBboxMaxLon(double value);

  double getBboxMaxLat() const;
  void setBboxMaxLat(double value);

  int getCurrentDayNum() const;
  void setCurrentDayNum(int value);

  const QString& getMapOverlayFile() const;
  void setMapOverlayFile(QString x);

  QString getMaplineColor() const;
  void setMaplineColor(QString x);

  int getMaplineColorIndex() const;
  void setMaplineColorIndex(int x);


  QString getWindVectorLabel() const;
  void setWindVectorLabel(QString x);
  
  int getWindVectorLength() const;
  void setWindVectorLength(int value);

  bool getSaveMetadataFlag() const;
  void setSaveMetadataFlag(bool value);

  int getSaveFileFormatIndex() const;
  void setSaveFileFormatIndex(int value);

  QString getUniqueId() const;
  void setUniqueId(QString x);

  int getDownloadsOutstanding() const;
  void setDownloadsOutstanding(int value);
};

typedef Singleton<VisOptions> VisOptionsSingleton;

#endif  //INC_VISOPTIONS_H_
