
#ifndef DATAFILTERPARAMETERS_H
#define DATAFILTERPARAMETERS_H

/******************************************************************************
PURPOSE: DataFilterParameters.h - Remote web service query parameters used to
         filter data when constructing Datasets.
HISTORY: 2013-02-27 plessel.todd@epa.gov
STATUS: unreviewed tested
******************************************************************************/

#include <string> // For std::string.

//================================== TYPES ====================================

namespace emvl {

class DataFilterParameters {

private:

  // Attributes:

  double CALIPSOMinimumAcceptedCADScore_; // [20, 100]. Default 20.0.
  double CALIPSOMaximumAcceptedUncertainty_; // [0, 100]. Default 99.0.
  double GASPAODMinimum_; // -0.5
  double GASPAODMaximum_; // 2.05
  double GASPSTDMinimum_; // 0
  double GASPSTDMaximum_; // 2.55
  double GASPCLSMinimum_; // 0
  double GASPCLSMaximum_; // 25
  double GASPCH1Minimum_; // 0
  double GASPCH1Maximum_; // 0.425
  double GASPSIGMinimum_; // -0.5
  double GASPSIGMaximum_; // 0.52
  double GASPSFCMinimum_; // -0.1
  double GASPSFCMaximum_; // 0.41
  double GASPMOSMinimum_; // 0
  double GASPMOSMaximum_; // 0.425
  double GASPSCAMinimum_; // 0
  double GASPSCAMaximum_; // 255.0
  int TROPOMIMinimumAcceptedQuality_; // [50, 100]. Default 100.
  double TROPOMIMaximumCloudFraction_; // [0, 1]. Default 1.0.
  int TROPOMIGroundPixelMinimum_; // [0, 449]. Default IMISSING.
  int TROPOMIGroundPixelMaximum_; // [0, 449]. Default IMISSING.
  bool TROPOMIAllowNegativeCounts_; // Of TROPOMI molecules/cm2. Default false.
  const char* VIIRSNOAAMinimumAcceptedQuality_; // "high", "medium". Def "high".
  const char* PandoraMinimumAcceptedQuality_; // "high","medium","low".Def"high"
  const char* TEMPOMinimumAcceptedQuality_; // "normal", "suspect", "bad".
  double TEMPOMaximumCloudFraction_; // [0, 1]. Default 1.0.
  double TEMPOMaximumSolarZenithAngle_; // [0, 90]. Default 90.0.
  bool TEMPOAllowNegativeCounts_; // Of molecules/cm2. Default false.
  double OMIMaximumCloudFraction_; // [0, 1]. Default 1.0.
  double OMIMaximumSolarZenithAngle_; // [0, 90]. Default 90.0.
  bool OMIAllowNegativeCounts_; // Of molecules/cm2. Default false.
  std::string TEMPOKey_; // Required to access TEMPO non-proxy data.
  std::string OAQPSKey_; // Required to access OAQPS data.
  std::string PurpleAirAPIKey_; // Required to access PurpleAir data.
  int PurpleAirOutInFlag_; // [0 = outdoor, 1 = indoor, 2 = either]. Default 2.
  double PurpleAirMaximumDifference_; // [0, 100]. Default 5.
  double PurpleAirMaximumRatio_; // [0, 1]. Default 0.7.
  double PurpleAirMinimumAggregationCountPercentage_; // [0, 100]. Default 75.
  double PurpleAirDefaultHumidity_; // [0, 100]. Default MISSING.
  std::string dataVersion_; // E.g., '4.00' for CALIPSO. Default '' unspecified

public:

  static bool isValidParameters(
    const double CALIPSOMinimumAcceptedCADScore,
    const double CALIPSOMaximumAcceptedUncertainty,
    const double GASPAODMinimum,
    const double GASPAODMaximum,
    const double GASPSTDMinimum,
    const double GASPSTDMaximum,
    const double GASPCLSMinimum,
    const double GASPCLSMaximum,
    const double GASPCH1Minimum,
    const double GASPCH1Maximum,
    const double GASPSIGMinimum,
    const double GASPSIGMaximum,
    const double GASPSFCMinimum,
    const double GASPSFCMaximum,
    const double GASPMOSMinimum,
    const double GASPMOSMaximum,
    const double GASPSCAMinimum,
    const double GASPSCAMaximum,
    const int TROPOMIMinimumAcceptedQuality,
    const double TROPOMIMaximumCloudFraction,
    const int TROPOMIGroundPixelMinimum,
    const int TROPOMIGroundPixelMaximum,
    const bool TROPOMIAllowNegativeCounts,
    const char* VIIRSNOAAMinimumAcceptedQuality,
    const char* PandoraMinimumAcceptedQuality,
    const char* TEMPOMinimumAcceptedQuality,
    const double TEMPOMaximumCloudFraction,
    const double TEMPOMaximumSolarZenithAngle,
    const bool TEMPOAllowNegativeCounts,
    const double OMIMaximumCloudFraction,
    const double OMIMaximumSolarZenithAngle,
    const bool OMIAllowNegativeCounts,
    const std::string& TEMPOKey,
    const std::string& OAQPSKey,
    const std::string& PurpleAirAPIKey,
    const int PurpleAirOutInFlag,
    const double PurpleAirMaximumDifference,
    const double PurpleAirMaximumRatio,
    const double PurpleAirMinimumAggregationCountPercentage_,
    const double PurpleAirDefaultHumidity,
    const std::string& dataVersion );

  // Commands:

  ~DataFilterParameters();
  DataFilterParameters();

  DataFilterParameters( const double CALIPSOMinimumAcceptedCADScore,
                        const double CALIPSOMaximumAcceptedUncertainty,
                        const double GASPAODMinimum,
                        const double GASPAODMaximum,
                        const double GASPSTDMinimum,
                        const double GASPSTDMaximum,
                        const double GASPCLSMinimum,
                        const double GASPCLSMaximum,
                        const double GASPCH1Minimum,
                        const double GASPCH1Maximum,
                        const double GASPSIGMinimum,
                        const double GASPSIGMaximum,
                        const double GASPSFCMinimum,
                        const double GASPSFCMaximum,
                        const double GASPMOSMinimum,
                        const double GASPMOSMaximum,
                        const double GASPSCAMinimum,
                        const double GASPSCAMaximum,
                        const int TROPOMIMinimumAcceptedQuality,
                        const double TROPOMIMaximumCloudFraction,
                        const int TROPOMIGroundPixelMinimum,
                        const int TROPOMIGroundPixelMaximum,
                        const bool TROPOMIAllowNegativeCounts,
                        const char* VIIRSNOAAMinimumAcceptedQuality,
                        const char* PandoraMinimumAcceptedQuality,
                        const char* TEMPOMinimumAcceptedQuality,
                        const double TEMPOMaximumCloudFraction,
                        const double TEMPOMaximumSolarZenithAngle,
                        const bool TEMPOAllowNegativeCounts,
                        const double OMIMaximumCloudFraction,
                        const double OMIMaximumSolarZenithAngle,
                        const bool OMIAllowNegativeCounts,
                        const std::string& TEMPOKey,
                        const std::string& OAQPSKey,
                        const std::string& PurpleAirAPIKey,
                        const int PurpleAirOutInFlag,
                        const double PurpleAirMaximumDifference,
                        const double PurpleAirMaximumRatio,
                        const double PurpleAirMinimumAggregationCountPercentage,
                        const double PurpleAirDefaultHumidity,
                        const std::string& dataVersion );

  DataFilterParameters( const DataFilterParameters& other );
  DataFilterParameters& operator=( const DataFilterParameters& other );
  void CALIPSOMinimumAcceptedCADScore( double value );
  void CALIPSOMaximumAcceptedUncertainty( const double value );
  void GASPAODRange( const double minimum, const double maximum );
  void GASPSTDRange( const double minimum, const double maximum );
  void GASPCLSRange( const double minimum, const double maximum );
  void GASPCH1Range( const double minimum, const double maximum );
  void GASPSIGRange( const double minimum, const double maximum );
  void GASPSFCRange( const double minimum, const double maximum );
  void GASPMOSRange( const double minimum, const double maximum );
  void GASPSCARange( const double minimum, const double maximum );
  void TROPOMIMinimumAcceptedQuality( const int value );
  void TROPOMIMaximumCloudFraction( const double value );
  void TROPOMIGroundPixelRange( const int minimum, const int maximum );
  void TROPOMIAllowNegativeCounts( const bool value );
  void VIIRSNOAAMinimumAcceptedQuality( const char* const value );
  void PandoraMinimumAcceptedQuality( const char* const value );
  void TEMPOMinimumAcceptedQuality( const char* const value );
  void TEMPOMaximumCloudFraction( const double value );
  void TEMPOMaximumSolarZenithAngle( const double value );
  void TEMPOAllowNegativeCounts( const bool value );
  void OMIMaximumCloudFraction( const double value );
  void OMIMaximumSolarZenithAngle( const double value );
  void OMIAllowNegativeCounts( const bool value );
  void TEMPOKey( const std::string& value );
  void OAQPSKey( const std::string& value );
  void PurpleAirAPIKey( const std::string& value );
  void PurpleAirOutInFlag( const int value );
  void PurpleAirMaximumDifference( const double value );
  void PurpleAirMaximumRatio( const double value );
  void PurpleAirMinimumAggregationCountPercentage( const double value );
  void PurpleAirDefaultHumidity( const double value );
  void dataVersion( const std::string& value );

  // Queries:

  bool operator==( const DataFilterParameters& other ) const;
  bool operator!=( const DataFilterParameters& other ) const;
  bool invariant() const;
  double CALIPSOMinimumAcceptedCADScore() const;
  double CALIPSOMaximumAcceptedUncertainty() const;
  double GASPAODMinimum() const;
  double GASPAODMaximum() const;
  double GASPSTDMinimum() const;
  double GASPSTDMaximum() const;
  double GASPCLSMinimum() const;
  double GASPCLSMaximum() const;
  double GASPCH1Minimum() const;
  double GASPCH1Maximum() const;
  double GASPSIGMinimum() const;
  double GASPSIGMaximum() const;
  double GASPSFCMinimum() const;
  double GASPSFCMaximum() const;
  double GASPMOSMinimum() const;
  double GASPMOSMaximum() const;
  double GASPSCAMinimum() const;
  double GASPSCAMaximum() const;
  int TROPOMIMinimumAcceptedQuality() const;
  double TROPOMIMaximumCloudFraction() const;
  int TROPOMIGroundPixelMinimum() const;
  int TROPOMIGroundPixelMaximum() const;
  bool TROPOMIAllowNegativeCounts() const;
  const char* VIIRSNOAAMinimumAcceptedQuality() const;
  const char* PandoraMinimumAcceptedQuality() const;
  const char* TEMPOMinimumAcceptedQuality() const;
  double TEMPOMaximumCloudFraction() const;
  double TEMPOMaximumSolarZenithAngle() const;
  bool TEMPOAllowNegativeCounts() const;
  double OMIMaximumCloudFraction() const;
  double OMIMaximumSolarZenithAngle() const;
  bool OMIAllowNegativeCounts() const;
  const std::string& TEMPOKey() const;
  const std::string& OAQPSKey() const;
  const std::string& PurpleAirAPIKey() const;
  int PurpleAirOutInFlag() const;
  double PurpleAirMaximumDifference() const;
  double PurpleAirMaximumRatio() const;
  double PurpleAirMinimumAggregationCountPercentage() const;
  double PurpleAirDefaultHumidity() const;
  const std::string& dataVersion() const;
};


} // namespace emvl.

#endif /* DATAFILTERPARAMETERS_H */


