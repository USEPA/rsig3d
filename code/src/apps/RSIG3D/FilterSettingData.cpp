/******************************************************************************
PURPOSE: FilterSettingData.cpp - Implements the filter setting dialog.

NOTES:

HISTORY: 2015-12-29 freeman.matt@epa.gov
******************************************************************************/

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QDebug>

#include "FilterSettingData.h"

const char* const FilterSettingData::validatorRegEx          = "[A-Z0-9-]*";
const char* const FilterSettingData::validatorRegExCaseInsen = "[A-Za-z0-9-]*";



FilterSettingData::FilterSettingData() {
  initFilterSettingData();
}



FilterSettingData::~FilterSettingData() {
}



void FilterSettingData::initFilterSettingData() {
  AODmin = -0.5;
  AODmax = 2.05;
  AODSTDmin = 0.0;
  AODSTDmax = 2.55;
  CLSmin = 0.0;
  CLSmax = 25.0;
  CH1min = 0.0;
  CH1max = 0.425;
  SIGmin = -0.5;
  SIGmax = 0.52;
  SFCmin = -0.1;
  SFCmax = 0.41;
  MOSmin = 0.0;
  MOSmax = 0.425;
  SCAmin = 0.0;
  SCAmax = 255;

  HourFraction = 2;

  CADmin = 20;
  UncertaintyMax = 99.0;

  TropomiQuality = 75;
  TropomiMaxCloudFraction = 1.0;
  TropomiGroundPixelMinimum = 0;
  TropomiGroundPixelMaximum = 0;
  TropomiAllowNegativeCounts = false;
  TEMPOMaxCloudFraction = 0.15; // Set default to 0.15 per Jim email 2024-05-14.
  TEMPOMaxSolarZenithAngle = 70.0; // Set default to 70 per email 2024-04-28.
  TEMPOAllowNegativeCounts = false;
  OMIMaxCloudFraction = 0.15; // Set default to 0.15 per Jim email 2024-05-14.
  OMIMaxSolarZenithAngle = 70.0; // Set default to 70 per email 2024-04-28.
  OMIAllowNegativeCounts = false;

  viirsNoaaQAFlag = "high";
  pandoraQAFlag = "high";
  PurpleAirAcceptableChannelDifference = 5.0;
  PurpleAirAcceptableChannelAgreement = 70.0;  
  PurpleAirMinAggregationCountPercentage = 75.0;
  PurpleAirReplacementHumidity = 50.0;
  usePurpleAirHumidityReplacement = true;
  PurpleairLocationFlag = 0; // 0=outdoor, 1=indoor, 2=both
  
  TEMPOQAFlag = "normal";
  TEMPOKey.clear();
  OAQPSKey.clear();
  PurpleAirAPIKey.clear();
  CALIPSOL1DataVersion = "Latest";
  CALIPSOL2DataVersion = "Latest";

  // Read purpleair key from file.
  // For validating purpleair API key,
  // all characters must be able to survive rsigserver untainting.
  QRegExp validator(validatorRegEx);
  QString filename_apikey = QDir::homePath() + "/.rsig/pakey.txt";
  QFile file_apikey(filename_apikey);
  QTextStream stream(&file_apikey);

  if (file_apikey.open(QIODevice::ReadOnly | QFile::Text )) {
    const int maxChars = 40;
    const QString mykey( stream.readLine( maxChars ) );

    if (validator.exactMatch(mykey)) {
      PurpleAirAPIKey = mykey;
    }
  }


  {
    QRegExp validatorTempo(validatorRegExCaseInsen);
    QString filename = QDir::homePath() + "/.rsig/tempokey.txt";
    QFile file(filename);
    QTextStream stream2(&file);

    if (file.open(QIODevice::ReadOnly | QFile::Text )) {
      const int maxChars = 40;
      const QString mykey( stream2.readLine( maxChars ) );

      if (validatorTempo.exactMatch(mykey)) {
        TEMPOKey = mykey;
      }
    }
  }

  {
    QRegExp validatorTempo(validatorRegExCaseInsen);
    QString filename = QDir::homePath() + "/.rsig/oaqpskey.txt";
    QFile file(filename);
    QTextStream stream2(&file);

    if (file.open(QIODevice::ReadOnly | QFile::Text )) {
      const int maxChars = 40;
      const QString mykey( stream2.readLine( maxChars ) );

      if (validatorTempo.exactMatch(mykey)) {
        OAQPSKey = mykey;
      }
    }
  }

}



