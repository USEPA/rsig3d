/******************************************************************************
PURPOSE: ControlDockWidget.h - An extended QDockwidget class for the role of
         CONTROL's UI.

NOTES:

HISTORY: 2012-11-15, zhang.nan@epa.gov
         Evolved by freeman.matt@epa.gov
STATUS:  unreviewed untested
******************************************************************************/

#ifndef CONTROLDOCKWIDGET_H_
#define CONTROLDOCKWIDGET_H_

#include <string>

#include <QString>
#include <QStringList>
#include <QDockWidget>
#include <QDomElement>
#include <QSplitter>
#include <QTimer>
#include <QProcess>
#include <QTreeWidget>
#include <QStandardItem>

#include "GeoDataObject.h"

#define CMAQ_EAST "CMAQ-East (OAQPS)"
#define CMAQ_WEST "CMAQ-West (OAQPS)"
#define CMAQ_CDC  "CMAQ-CDC 12km"

static const QString maplineColorStrings[] = {
  "Black",
  "White",
  "Gray10",
  "Gray20",
  "Gray30",
  "Gray40",
  "Gray50",
  "Gray60",
  "Gray70",
  "Gray80",
  "Gray90"
};

struct DataSourceAndVariable {
  const char* const mSourceName;   //source name &
  QString* mSourceTip;             //source tip
  QString* mVariablesArray;        //a list of variables
  QString* mVariableTipsArray;
  QString* mVariableTipsCatArray;
  QString*mVariableTrueNamesArray;
  int mArrayLength;
};

extern const struct DataSourceAndVariable dataSourceVar[]; // UGLY.

class Ui_ControlUIForm;
class GeoDataPropertyEdit;
class VisDataPropertyEdit;
class QResizeEvent;
class QCloseEvent;
class QComboBox;

class ControlDockWidget : public QDockWidget {
private:

  Q_OBJECT
  Q_DISABLE_COPY(ControlDockWidget);

  // Attributes:

  Ui_ControlUIForm* ui;
  QSplitter* m_viewSplitter;
  GeoDataPropertyEdit* m_objectPropEdit;
  VisDataPropertyEdit* m_visPropEdit;
  QTimer m_snapshotTimer;    //snapshot timer
  QTimer m_stateTimer;       //timer for fix some QT bugs
  QProcess m_ffmpeg;         //video making process
  QProcess m_updator;        //updating process
  int m_visible;             //visible flag, to fix QT bugs in window closing event
  double currentGuiBbox_minLat;
  double currentGuiBbox_maxLat;
  double currentGuiBbox_minLon;
  double currentGuiBbox_maxLon;

public:
  static const int availNamesSize = 100;
  static const int selectedDataLongWidth  = 293;
  static const int selectedDataMedWidth   = 265;
  static const int selectedDataShortWidth = 181;

  QString availNames[100]; // UGLY public attributes!
  long minAvailDate[100];
  long maxAvailDate[100];

  ControlDockWidget(QWidget *parent = 0);
  ~ControlDockWidget();

  QDomElement domElement(const QString& name, QDomDocument& document) const;
  void initFromDOMElement(const QDomElement& element);
  void set_currentGuiBbox_minLat(double minLat);
  void set_currentGuiBbox_maxLat(double minLat);
  void set_currentGuiBbox_minLon(double minLon);
  void set_currentGuiBbox_maxLon(double maxLon);
  double get_currentGuiBbox_minLat();
  double get_currentGuiBbox_maxLat();
  double get_currentGuiBbox_minLon();
  double get_currentGuiBbox_maxLon();

  QString panelDisplaySource1Selection;
  void setPanelDisplaySource1Selection(QString selection);
  QString getPanelDisplaySource1Selection();

  QString panelDisplaySource2Selection;
  void setPanelDisplaySource2Selection(QString selection);
  QString getPanelDisplaySource2Selection();
  
Q_SIGNALS:
  void mapServerChanged(bool useMarbleWidget, const char *theme);
  void mapProjectionChanged(int mode);
  void boundingGeoBoxChanged(const QRectF &);
  void requestRedraw();
  void requestInitSnapshot(const QString &dirName);
  void requestInitSnapshotForVideo(const QString &dirName);
  void requestSnapshot();
  void requestSingleSnapshot();
  void widgetClosed();
  
public Q_SLOTS:
  //modes can be NORMAL_TEXT, WARNING_TEXT, ERROR_TEXT, etc
  void postMessage(const QString &msg, int mode);
  void onBoundingGeoBoxChanged(const QRectF &rectf);
  //need from main.cpp for checking before or after GUI showup
  void onCheckUpdateClicked();
  void handleUpdate();
  void update_timestepSlider(int counter_value);
  int get_animationSpeed();
  void update_playButtonText(QString buttontext);

  void loadGeoDataObject(GeoDataObject* geoDataObject) {
    onDownloadWorkFinished(geoDataObject);
  }

  bool onShowDataUsePolicy(int activeFlag);
  void onCheckDataUsePolicyAccepted();
  void initRSIG3DHelp(int forceShowFlag);
  void onDateChanged();
  void onDurationChanged();
  void switchSpecialControlsWidget( const bool isGrid,
                                    const QString& curName );
  void setWidgetTooltips();
  void onWindVectorScaleFactorChanged(double);
  void onWindVectorTipSizeChanged(double);
  QString currentMapProjection() const;
  int getAggregateIndex(QString text);
  QStringList getDisplayPanelProbelist();
  void onPanelDisplayListWidgetClicked();
                                        
                                   
protected Q_SLOTS:
  void onScenarioIndexChanged(int index);
  void onTimebaseIndexChanged(int index);
  void onMapServerIndexChanged(int index);
  void onMapProjectionChanged();
  void onShowLonlatLinesChanged(int);
  void onShowScaleChanged(int);
  void onShowLUTsChanged(int);
  void onShowNavigatorChanged(int);
  void onShowDataProbeChanged(int);
  void onClearLog();
  void onOpenWebClicked();
  void onEmailClicked();
  void onDownloadWorkFinished(QObject *points);
  void onSaveWorkFinished(QObject *points);
  void onFileOrientedDownloaderClicked();
  void onFilterParametersClicked();
  void onRegridParametersClicked();
  void onSaveVideoToggled( bool );
  void onWriteToFileClicked();
  void onFFMPEGFinished(int, QProcess::ExitStatus);
  void onFFMPEGError(QProcess::ProcessError);
  void onUpdateRsigFinished(int, QProcess::ExitStatus);
  void onUpdateRsigError(QProcess::ProcessError);
  void onExitClicked();
  void onStateTimeOut();
  void onTabWidgetIndexChanged(int);
  void generateDefaultPngFilename();
  void onSetMapDoubleClickOption1(int);
  void onSetMapDoubleClickOption2(int);

  // time controls
  void onAnimationSpeedChanged(int speed);
  void ontimestepSliderChanged(int timestep);
  void onPlayButtonClicked();
  void onForwardButtonClicked();
  void onForwardButton24Clicked();
  void onBackButtonClicked();
  void onBackButton24Clicked();

  // viz controls
  void onOpacitySliderChanged(int);
  void onThresholdSliderChanged(int);
  void onShowCMAQRowChanged(int);
  void onShowCMAQColChanged(int);
  void onShowCMAQLayerChanged(int);
  void onCMAQRowSliderChanged(int);
  void onCMAQColSliderChanged(int);
  void onCMAQLayerSliderChanged(int);
  void onVisibilityChanged(int);
  void onDataDefaultChanged(int);
  void onShowCellBoundaryChanged(int);
  void onCheckboxFlattenCMAQChanged(int);
  void onCheckboxPersistBEHRChanged(int);
  void onCheckboxPersistMODISChanged(int);
  void onCheckboxPersistVIIRSChanged(int);
  void onCheckboxPersistTROPOMIChanged(int);
  void onCheckboxPersistTEMPOChanged(int);
  void onCheckboxPersistHMSChanged(int);
  void onThresholdCheckboxChanged(int);
  void onRulerCheckboxChanged(int);
  void onCeilometerPlotScrollerChanged(int);
  void onPandoraPlotScrollerChanged(int);
  void onNeubrewPlotScrollerChanged(int);
  void onNeubrewSizeChanged(int);
  void onCeilometerSizeChanged(int);
  void onPandoraSizeChanged(int);
  void onMinSpinboxChanged(double);
  void onMaxSpinboxChanged(double);
  void onCalipsoScaleFactorChanged(double);
  void onWindVectorSparsenessChanged(double);
  void onCeilometerMinimumAltitudeChanged(double);
  void onCeilometerMaximumAltitudeChanged(double);
  void onPandoraMinimumAltitudeChanged(double);
  void onPandoraMaximumAltitudeChanged(double);
  void onNeubrewMinimumAltitudeChanged(double);
  void onNeubrewMaximumAltitudeChanged(double);
  void onRulerLonSpinBoxChanged(double);
  void onRulerLatSpinBoxChanged(double);
  void onPersistBEHRHoursChanged(int);
  void onPersistMODISHoursChanged(int);
  void onPersistVIIRSHoursChanged(int);
  void onPersistTROPOMIHoursChanged(int);
  void onPersistTEMPOHoursChanged(int);
  void onPersistHMSHoursChanged(int);
  void onColormapIndexChanged(int index);
  void onVizVarIndexChanged(int index);
  void onDataClearClicked();
  void onRadioButton2DToggled(bool value);
  void onRadioButton3DToggled(bool value);
  void onRadioButtonPanelDisplayToggled(bool value);
  void onCeilometerShowLocationsToggled(bool value);
  void onCeilometerShowMixingLayerToggled(bool value);
  void onPandoraShowLocationsToggled(bool value);
  void onNeubrewShowLocationsToggled(bool value);
  void onWindVectorAutosparseChanged(bool value);
  void onViewResetPressed();
  void onViewAutozoomPressed();
  void onLonlatLabelOrientationChanged(int val);
  void onTreeWidgetItemChanged(QTreeWidgetItem *thisitem);
  void onSaveTypeClicked();
  void downloadImage();
  void downloadImageError(QString errorString);
  void downloadImageMessage(QString messageString);
  void deleteModisTrueColorImages();
  void onSaveFilenameChanged();
  void onMaplinesChanged();
  void onMaplineColorIndexChanged(int index);
  void onPanelDisplaySource1Changed(int index);
  void onPanelDisplaySource2Changed(int index);
  void onGuiFontsizeChanged(int fontsize);


  

  // These are the important callbacks that retrieve data and save data:

  void onGetdataClicked(); // Routine starts data retrieval per source menu.
  void onSaveFileClicked(); // Saving datasets or starts downloading originals.
  void onCheckServersClicked(); // Check status of servers in the last 24 hours.

  // Helpers for above data retrieval/save routines:

public:

  void deleteAllDataObjects();
  void enableGetDataButton();

private:

  QStringList getOutage();
  
  void checkAndStartLoadingDataset( const QString& sd,
                                    const int layer,
                                    const int aggregate,
                                    const int regrid,
                                    const int dx,
                                    const int dy );

  // Check for and start saving dataset as original format and/or metadata:

  void checkAndStartSavingOriginalDataset( const QString& sd,
                                           const int dx,
                                           const int dy,
                                           const std::string& directory,
                                           const bool asOriginal,
                                           const bool andMetadata );

  // Save non-webservice CMAQDataset as CMAQ format to directory:

  void saveAsOriginalCMAQFile( const QString& source,
                               const std::string& directory ) const;

  // Save datasets (not original files) to user-selected format and directory:

  void saveDatasets( const int datasetOutputFormat,
                     const std::string& directory );

  // Starts an asynchronus thread to webservice.
  // Used for data input and output original files and metadata:

  void run_downloader( QString coverage, int regrid, int layer, int aggregate,
                       int dx, int dy,
                       bool saveOriginalFlag, bool saveMetadataFlag );

  // Displays a blocking/synchronus dialog:

  void useCMAQFileReaderDialog( const QString& source,
                                const int layer, const int aggregate );

  static int getIndexOfVisDataObjectForSource( const QString& source );
  static int getIndexOfGeoDataObjectForSource( const QString& source );


protected:

  virtual void resizeEvent(QResizeEvent *event);
  virtual void closeEvent(QCloseEvent *event);
  QString currentMapServer() const;
  QRectF regionOfInterest() const;
  void setRegrid(const int & regrid);
  void setRegionOfInterest(const QRectF & r);
  void setCurrentMapServer(const QString& s);
  void setCurrentMapProjection(const QString& s);  
  
private Q_SLOTS:

  void onBoundingGeoBoxChanged();
  void onSelectedLayer1Changed(int);  
  void onSelectedLayer2Changed(int);  
  void onSelectedLayer3Changed(int);  
  void onSelectedLayer4Changed(int);  
  void onSelectedLayer5Changed(int);
  void onTest(int);  

private:

  void addMapServersHelper();
  void addScenariosHelper();
  void addTimebaseHelper();
  void addVariablesHelper();
  void addVariablesHelper2(const QString rsigVariablesFilename);
  void addMaplineColorHelper();
  void setVariablesToComboBox(QComboBox *w, const QString *array, const QString *tips, const QString *tipscat, int len);
  void setVariablesToComboBox2(QComboBox *w, int index);
  void initTab0ClientData();
  void initTab1ClientData();
  void initTab2ClientData();
  void initTab3ClientData();
  void initTab4ClientData();
  void readDataPrefs();
  void doResize(int w, int h);
  void addColormapHelper();
  void initTreeWidget(QTreeWidget *tw);
  void setVariablesToTree(QTreeWidget *tw, int index);
  void readSourceTree(QString rsigVariablesFilename, QStringList &sourceTree, QTreeWidget *tw);
  void pack_selectedData();
  void readDataAvailability();
  QString get_tooltip(QString var, QString units, QString description);
  QString get_variableCoverage(QString item);
  QString get_variableDescription(QString item);
  void setVisWidgetState(VisDataObject * visObj);
  bool saveScenario(const QString &dirname);
  void check_sources(QString s1,  QString s2, QString s3, QString s4, QString s5, QDate date);
  void enableModisTrueColorOptions(bool flag);
};


#endif //CONTROLDOCKWIDGET_H_
