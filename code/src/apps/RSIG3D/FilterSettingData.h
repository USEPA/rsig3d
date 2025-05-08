/******************************************************************************
PURPOSE: filtersetting.h - A class for running http protocol get command.

NOTES:   .

HISTORY: 2012/11/28, zhang.nan@epa.gov, Created.
STATUS:  unreviewed, untested.
******************************************************************************/

#ifndef FILTERSETTINGDATA_H_
#define FILTERSETTINGDATA_H_
#include <QString>

struct FilterSettingData {
public:
  static const char* const validatorRegEx;
  static const char* const validatorRegExCaseInsen;

  double AODmin;
  double AODmax;

  double AODSTDmin;
  double AODSTDmax;

  double CLSmin;
  double CLSmax;

  double CH1min;
  double CH1max;

  double SIGmin;
  double SIGmax;

  double SFCmin;
  double SFCmax;

  double MOSmin;
  double MOSmax;

  double SCAmin;
  double SCAmax;

  int HourFraction;

  int CADmin;

  int TropomiQuality;
  double TropomiMaxCloudFraction;
  int TropomiGroundPixelMinimum;
  int TropomiGroundPixelMaximum;
  bool TropomiAllowNegativeCounts;
  double TEMPOMaxCloudFraction;
  double TEMPOMaxSolarZenithAngle;
  bool TEMPOAllowNegativeCounts;
  double OMIMaxCloudFraction;
  double OMIMaxSolarZenithAngle;
  bool OMIAllowNegativeCounts;

  double UncertaintyMax;

  double PurpleAirAcceptableChannelDifference;
  double PurpleAirAcceptableChannelAgreement;
  double PurpleAirMinAggregationCountPercentage;
  double PurpleAirReplacementHumidity;
  bool   usePurpleAirHumidityReplacement;
  int    PurpleairLocationFlag;
  
  QString viirsNoaaQAFlag;
  QString pandoraQAFlag;
  QString TEMPOQAFlag;
  QString TEMPOKey;
  QString OAQPSKey;
  QString PurpleAirAPIKey;
  QString CALIPSOL1DataVersion;
  QString CALIPSOL2DataVersion;

  FilterSettingData();

  ~FilterSettingData();

  void initFilterSettingData();
};

#endif //FILTERSETTINGDATA_H_
