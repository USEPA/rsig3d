/******************************************************************************
PURPOSE: DataFilterParameters.C - Data filter parameters used to filter data
         at remote web service.
HISTORY: 2013-02-27 plessel.todd@epa.gov
NOTES: See http://www.baronams.com/products/ioapi/GRIDS.html
******************************************************************************/

//================================ INCLUDES ===================================

#include <string.h> // For strcmp().

#include <string>   // For std::string.

#include <Assertions.h>           // For PRE0*(), POST0*(), AND?().
#include <DataUtilities.h>        // For isNan().
#include <DataFilterParameters.h> // For public interface.

//============================= PUBLIC FUNCTIONS ==============================



namespace emvl {


/******************************************************************************
PURPOSE: isValidParameters - Are the parameters valid?
INPUTS:  const double CALIPSOMinimumAcceptedCADScore  [20, 100].
         const double CALIPSOMaximumAcceptedUncertainty  [0, 100].
         const double GASPAODMinimum -0.5
         const double GASPAODMaximum 2.05
         const double GASPSTDMinimum 0
         const double GASPSTDMaximum 2.55
         const double GASPCLSMinimum 0
         const double GASPCLSMaximum 25
         const double GASPCH1Minimum 0
         const double GASPCH1Maximum 0.425
         const double GASPSIGMinimum -0.5
         const double GASPSIGMaximum 0.52
         const double GASPSFCMinimum -0.1
         const double GASPSFCMaximum 0.41
         const double GASPMOSMinimum 0
         const double GASPMOSMaximum 0.425
         const double GASPSCAMinimum 0
         const double GASPSCAMaximum 255
         const int TROPOMIMinimumAcceptedQuality [50, 100].
         const double TROPOMIMaximumCloudFraction [0, 1].
         const int TROPOMIGroundPixelMinimum [0, 449]. Default IMISSING.
         const int TROPOMIGroundPixelMaximum [0, 449]. Default IMISSING.
         const bool TROPOMIAllowNegativeCounts  Of TROPOMI molecules/cm2?
         const char* const VIIRSNOAAMinimumAcceptedQuality "high" or "medium".
         const char* const PandoraMinimumAcceptedQuality "high", "medium", "low"
         const int TEMPOMinimumAcceptedQuality 0 = normal, 1 = suspect, 2 = bad
         const double TEMPOMaximumCloudFraction [0, 1].
         const double TEMPOMaximumSolarZenithAngle [0, 90].
         const bool TEMPOAllowNegativeCounts  Of molecules/cm2?
         const double OMIMaximumCloudFraction [0, 1].
         const double OMIMaximumSolarZenithAngle [0, 90].
         const bool OMIAllowNegativeCounts  Of molecules/cm2?
         const std::string& TEMPOKey
         const std::string& OAQPSKey
         const std::string& PurpleAirAPIKey
         const int PurpleAirOutInFlag[0 = outdoor, 1 = indoor, 2 = either].
         const double PurpleAirMaximumDifference [0, 100].
         const double PurpleAirMaximumRatio [0, 1].
         const double PurpleAirMinimumAggregationCountPercentage [0, 100].
         const double PurpleAirDefaultHumidity [0, 100].
         const std::String& dataVersion
RETURNS: bool true if valid, else false.
******************************************************************************/

bool DataFilterParameters::isValidParameters(
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
  const double PurpleAirMinimumAggregationCountPercentage,
  const double PurpleAirDefaultHumidity,
  const std::string& dataVersion ) {

  const bool result =
    AND30( IN_RANGE( CALIPSOMinimumAcceptedCADScore, 20.0, 100.0 ),
           IN_RANGE( CALIPSOMaximumAcceptedUncertainty, 0.0, 100.0 ),
           IN_RANGE( GASPAODMinimum, -0.5, 2.05 ),
           IN_RANGE( GASPAODMaximum, GASPAODMinimum, 2.05 ),
           IN_RANGE( GASPSTDMinimum, 0.0, 2.55 ),
           IN_RANGE( GASPSTDMaximum, GASPSTDMinimum, 2.55 ),
           IN_RANGE( GASPCLSMinimum, 0.0, 25.0 ),
           IN_RANGE( GASPCLSMaximum, GASPCLSMinimum, 25.0 ),
           IN_RANGE( GASPCH1Minimum, 0.0, 0.425 ),
           IN_RANGE( GASPCH1Maximum, GASPCH1Minimum, 0.425 ),
           IN_RANGE( GASPSIGMinimum, -0.5, 0.52 ),
           IN_RANGE( GASPSIGMaximum, GASPSIGMinimum, 0.52 ),
           IN_RANGE( GASPSFCMinimum, -0.1, 0.41 ),
           IN_RANGE( GASPSFCMaximum, GASPSFCMinimum, 0.41 ),
           IN_RANGE( GASPMOSMinimum, 0.0, 0.425 ),
           IN_RANGE( GASPMOSMaximum, GASPMOSMinimum, 0.425 ),
           IN_RANGE( GASPSCAMinimum, 0.0, 255.0 ),
           IN_RANGE( GASPSCAMaximum, GASPSCAMinimum, 255.0 ),
           AND2( IN_RANGE( TROPOMIMinimumAcceptedQuality, 50, 100 ),
                 IN_RANGE( TROPOMIMaximumCloudFraction, 0.0, 1.0 ) ),
           OR2( AND2( TROPOMIGroundPixelMinimum == IMISSING,
                      TROPOMIGroundPixelMaximum == IMISSING ),
                AND2( TROPOMIGroundPixelMinimum >= 0,
                      TROPOMIGroundPixelMaximum >= TROPOMIGroundPixelMinimum )),
           AND2( IN_RANGE( TEMPOMaximumCloudFraction, 0.0, 1.0 ),
                 IN_RANGE( OMIMaximumCloudFraction, 0.0, 1.0 ) ),
           AND2( IN_RANGE( TEMPOMaximumSolarZenithAngle, 0.0, 90.0 ),
                 IN_RANGE( OMIMaximumSolarZenithAngle, 0.0, 90.0 ) ),
           OR3( ! strcmp( TEMPOMinimumAcceptedQuality, "normal" ),
                ! strcmp( TEMPOMinimumAcceptedQuality, "suspect" ),
                ! strcmp( TEMPOMinimumAcceptedQuality, "bad" ) ),
           OR2( ! strcmp( VIIRSNOAAMinimumAcceptedQuality, "high" ),
                ! strcmp( VIIRSNOAAMinimumAcceptedQuality, "medium" ) ),
           OR3( ! strcmp( PandoraMinimumAcceptedQuality, "high" ),
                ! strcmp( PandoraMinimumAcceptedQuality, "medium" ),
                ! strcmp( PandoraMinimumAcceptedQuality, "low" ) ),
           IN_RANGE( PurpleAirOutInFlag, 0, 2 ),
           IN_RANGE( PurpleAirMaximumDifference, 0.0, 100.0 ),
           IN_RANGE( PurpleAirMaximumRatio, 0.0, 1.0 ),
           IN_RANGE( PurpleAirMinimumAggregationCountPercentage, 0.0, 100.0 ),
           OR2( PurpleAirDefaultHumidity == MISSING,
                IN_RANGE( PurpleAirDefaultHumidity, 0.0, 100.0 ) ) );
  return result;
}



/******************************************************************************
PURPOSE: ~DataFilterParameters - Destructor.
******************************************************************************/

DataFilterParameters::~DataFilterParameters() {
  PRE( true );
  CALIPSOMaximumAcceptedUncertainty_ = 0.0;
  GASPAODMinimum_ = 0.0;
  GASPAODMaximum_ = 0.0;
  GASPSTDMinimum_ = 0.0;
  GASPSTDMaximum_ = 0.0;
  GASPCLSMinimum_ = 0.0;
  GASPCLSMaximum_ = 0.0;
  GASPCH1Minimum_ = 0.0;
  GASPCH1Maximum_ = 0.0;
  GASPSIGMinimum_ = 0.0;
  GASPSIGMaximum_ = 0.0;
  GASPSFCMinimum_ = 0.0;
  GASPSFCMaximum_ = 0.0;
  GASPMOSMinimum_ = 0.0;
  GASPMOSMaximum_ = 0.0;
  GASPSCAMinimum_ = 0.0;
  GASPSCAMaximum_ = 0.0;
  TROPOMIMinimumAcceptedQuality_ = 0;
  TROPOMIMaximumCloudFraction_ = 0.0;
  TROPOMIGroundPixelMinimum_ = IMISSING;
  TROPOMIGroundPixelMaximum_ = IMISSING;
  TROPOMIAllowNegativeCounts_ = false;
  VIIRSNOAAMinimumAcceptedQuality_ = 0;
  PandoraMinimumAcceptedQuality_ = 0;
  TEMPOMinimumAcceptedQuality_ = 0;
  TEMPOMaximumCloudFraction_ = 0.0;
  TEMPOMaximumSolarZenithAngle_ = 0.0;
  TEMPOAllowNegativeCounts_ = false;
  OMIMaximumCloudFraction_ = 0.0;
  OMIMaximumSolarZenithAngle_ = 0.0;
  OMIAllowNegativeCounts_ = false;
  PurpleAirOutInFlag_ = 0;
  PurpleAirMaximumDifference_ = 0.0;
  PurpleAirMaximumRatio_ = 0.0;
  PurpleAirMinimumAggregationCountPercentage_ = 0.0;
  PurpleAirDefaultHumidity_ = 0.0;
}



/******************************************************************************
PURPOSE: DataFilterParameters - Default contructor.
******************************************************************************/

DataFilterParameters::DataFilterParameters() {
  CALIPSOMinimumAcceptedCADScore_ = 20.0;
  CALIPSOMaximumAcceptedUncertainty_ = 99.0;
  GASPAODMinimum_ = -0.5;
  GASPAODMaximum_ = 2.05;
  GASPSTDMinimum_ = 0.0;
  GASPSTDMaximum_ = 2.55;
  GASPCLSMinimum_ = 0.0;
  GASPCLSMaximum_ = 25.0;
  GASPCH1Minimum_ = 0.0;
  GASPCH1Maximum_ = 0.425;
  GASPSIGMinimum_ = -0.5;
  GASPSIGMaximum_ = 0.52;
  GASPSFCMinimum_ = -0.1;
  GASPSFCMaximum_ = 0.41;
  GASPMOSMinimum_ = 0.0;
  GASPMOSMaximum_ = 0.425;
  GASPSCAMinimum_ = 0.0;
  GASPSCAMaximum_ = 255.0;
  TROPOMIMinimumAcceptedQuality_ = 100;
  TROPOMIMaximumCloudFraction_ = 1.0;
  TROPOMIGroundPixelMinimum_ = IMISSING;
  TROPOMIGroundPixelMaximum_ = IMISSING;
  TROPOMIAllowNegativeCounts_ = false;
  VIIRSNOAAMinimumAcceptedQuality_ = "high";
  PandoraMinimumAcceptedQuality_ = "high";
  TEMPOMinimumAcceptedQuality_ = "normal";
  TEMPOMaximumCloudFraction_ = 1.0;
  TEMPOMaximumSolarZenithAngle_ = 90.0;
  TEMPOAllowNegativeCounts_ = false;
  OMIMaximumCloudFraction_ = 1.0;
  OMIMaximumSolarZenithAngle_ = 90.0;
  OMIAllowNegativeCounts_ = false;
  PurpleAirOutInFlag_ = 2; // 2 = either outside or inside.
  PurpleAirMaximumDifference_ = 5.0;
  PurpleAirMaximumRatio_ = 0.7;
  PurpleAirMinimumAggregationCountPercentage_ = 75.0;
  PurpleAirDefaultHumidity_ = MISSING;

  POST29( CALIPSOMinimumAcceptedCADScore() == 20.0,
          CALIPSOMaximumAcceptedUncertainty() == 99.0,
          GASPAODMinimum() == -0.5,
          GASPAODMaximum() == 2.05,
          GASPSTDMinimum() == 0.0,
          GASPSTDMaximum() == 2.55,
          GASPCLSMinimum() == 0.0,
          GASPCLSMaximum() == 25.0,
          GASPCH1Minimum() == 0.0,
          GASPCH1Maximum() == 0.425,
          GASPSIGMinimum() == -0.5,
          GASPSIGMaximum() == 0.52,
          GASPSFCMinimum() == -0.1,
          GASPSFCMaximum() == 0.41,
          GASPMOSMinimum() == 0.0,
          GASPMOSMaximum() == 0.425,
          GASPSCAMinimum() == 0.0,
          GASPSCAMaximum() == 255.0,
          AND2( TROPOMIMinimumAcceptedQuality() == 100,
                TROPOMIMaximumCloudFraction() == 1.0 ),
          AND3( TROPOMIGroundPixelMinimum_ == IMISSING,
                TROPOMIGroundPixelMaximum_ == IMISSING,
                TROPOMIAllowNegativeCounts() == false ),
          ! strcmp( VIIRSNOAAMinimumAcceptedQuality_, "high" ),
          ! strcmp( PandoraMinimumAcceptedQuality_, "high" ),
          ! strcmp( TEMPOMinimumAcceptedQuality_, "normal" ),
          AND6( TEMPOMaximumCloudFraction() == 1.0,
                TEMPOMaximumSolarZenithAngle() == 90.0,
                TEMPOAllowNegativeCounts() == false,
                OMIMaximumCloudFraction() == 1.0,
                OMIMaximumSolarZenithAngle() == 90.0,
                OMIAllowNegativeCounts() == false ),
          PurpleAirOutInFlag() == 2,
          PurpleAirMaximumDifference() == 5.0,
          PurpleAirMaximumRatio() == 0.7,
          PurpleAirMinimumAggregationCountPercentage() == 75.0,
          PurpleAirDefaultHumidity() == MISSING );
}



/******************************************************************************
PURPOSE: DataFilterParameters - Contructor.
INPUTS:  const double CALIPSOMinimumAcceptedCADScore  [20, 100].
         const double CALIPSOMaximumAcceptedUncertainty  [0, 100].
         const double GASPAODMinimum -0.5
         const double GASPAODMaximum 2.05
         const double GASPSTDMinimum 0
         const double GASPSTDMaximum 2.55
         const double GASPCLSMinimum 0
         const double GASPCLSMaximum 25
         const double GASPCH1Minimum 0
         const double GASPCH1Maximum 0.425
         const double GASPSIGMinimum -0.5
         const double GASPSIGMaximum 0.52
         const double GASPSFCMinimum -0.1
         const double GASPSFCMaximum 0.41
         const double GASPMOSMinimum 0
         const double GASPMOSMaximum 0.425
         const double GASPSCAMinimum 0
         const double GASPSCAMaximum 255
         const int TROPOMIMinimumAcceptedQuality [50, 100].
         const double TROPOMIMaximumCloudFraction [0, 1].
         const int TROPOMIGroundPixelMinimum [0, 449]. Default IMISSING.
         const int TROPOMIGroundPixelMaximum [0, 449]. Default IMISSING.
         const bool TROPOMIAllowNegativeCounts  Of TROPOMI molecules/cm2?
         const char* const VIIRSNOAAMinimumAcceptedQuality "high" or "medium".
         const char* const PandoraMinimumAcceptedQuality "high","medium","low".
         const char* const TEMPOMinimumAcceptedQuality "normal","suspect","bad"
         const double TEMPOMaximumCloudFraction [0, 1].
         const double TEMPOMaximumSolarZenithAngle [0, 90].
         const bool TEMPOAllowNegativeCounts  Of molecules/cm2?
         const double OMIMaximumCloudFraction [0, 1].
         const double OMIMaximumSolarZenithAngle [0, 90].
         const bool OMIAllowNegativeCounts  Of molecules/cm2?
         const std::string& TEMPOKey
         const std::string& OAQPSKey
         const std::string& PurpleAirAPIKey
         const int PurpleAirOutInFlag
         const double PurpleAirMaximumDifference
         const double PurpleAirMaximumRatio
         const double PurpleAirMinimumAggregationCountPercentage
         const double PurpleAirDefaultHumidity
         const std::String& dataVersion
******************************************************************************/

DataFilterParameters::DataFilterParameters(
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
  const char* const VIIRSNOAAMinimumAcceptedQuality,
  const char* const PandoraMinimumAcceptedQuality,
  const char* const TEMPOMinimumAcceptedQuality,
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
  const std::string& dataVersion ) {

  PRE0( isValidParameters( CALIPSOMinimumAcceptedCADScore,
                           CALIPSOMaximumAcceptedUncertainty,
                           GASPAODMinimum,
                           GASPAODMaximum,
                           GASPSTDMinimum,
                           GASPSTDMaximum,
                           GASPCLSMinimum,
                           GASPCLSMaximum,
                           GASPCH1Minimum,
                           GASPCH1Maximum,
                           GASPSIGMinimum,
                           GASPSIGMaximum,
                           GASPSFCMinimum,
                           GASPSFCMaximum,
                           GASPMOSMinimum,
                           GASPMOSMaximum,
                           GASPSCAMinimum,
                           GASPSCAMaximum,
                           TROPOMIMinimumAcceptedQuality,
                           TROPOMIMaximumCloudFraction,
                           TROPOMIGroundPixelMinimum,
                           TROPOMIGroundPixelMaximum,
                           TROPOMIAllowNegativeCounts,
                           VIIRSNOAAMinimumAcceptedQuality,
                           PandoraMinimumAcceptedQuality,
                           TEMPOMinimumAcceptedQuality,
                           TEMPOMaximumCloudFraction,
                           TEMPOMaximumSolarZenithAngle,
                           TEMPOAllowNegativeCounts,
                           OMIMaximumCloudFraction,
                           OMIMaximumSolarZenithAngle,
                           OMIAllowNegativeCounts,
                           TEMPOKey,
                           OAQPSKey,
                           PurpleAirAPIKey,
                           PurpleAirOutInFlag,
                           PurpleAirMaximumDifference,
                           PurpleAirMaximumRatio,
                           PurpleAirMinimumAggregationCountPercentage,
                           PurpleAirDefaultHumidity,
                           dataVersion ) );

  CALIPSOMinimumAcceptedCADScore_    = CALIPSOMinimumAcceptedCADScore;
  CALIPSOMaximumAcceptedUncertainty_ = CALIPSOMaximumAcceptedUncertainty;
  GASPAODMinimum_ = GASPAODMinimum;
  GASPAODMaximum_ = GASPAODMaximum;
  GASPSTDMinimum_ = GASPSTDMinimum;
  GASPSTDMaximum_ = GASPSTDMaximum;
  GASPCLSMinimum_ = GASPCLSMinimum;
  GASPCLSMaximum_ = GASPCLSMaximum;
  GASPCH1Minimum_ = GASPCH1Minimum;
  GASPCH1Maximum_ = GASPCH1Maximum;
  GASPSIGMinimum_ = GASPSIGMinimum;
  GASPSIGMaximum_ = GASPSIGMaximum;
  GASPSFCMinimum_ = GASPSFCMinimum;
  GASPSFCMaximum_ = GASPSFCMaximum;
  GASPMOSMinimum_ = GASPMOSMinimum;
  GASPMOSMaximum_ = GASPMOSMaximum;
  GASPSCAMinimum_ = GASPSCAMinimum;
  GASPSCAMaximum_ = GASPSCAMaximum;
  TROPOMIMinimumAcceptedQuality_ = TROPOMIMinimumAcceptedQuality;
  TROPOMIMaximumCloudFraction_   = TROPOMIMaximumCloudFraction;
  TROPOMIGroundPixelMinimum_     = TROPOMIGroundPixelMinimum;
  TROPOMIGroundPixelMaximum_     = TROPOMIGroundPixelMaximum;
  TROPOMIAllowNegativeCounts_    = TROPOMIAllowNegativeCounts;
  TEMPOMaximumCloudFraction_     = TEMPOMaximumCloudFraction;
  TEMPOMaximumSolarZenithAngle_  = TEMPOMaximumSolarZenithAngle;
  TEMPOAllowNegativeCounts_      = TEMPOAllowNegativeCounts;
  OMIMaximumCloudFraction_       = OMIMaximumCloudFraction;
  OMIMaximumSolarZenithAngle_    = OMIMaximumSolarZenithAngle;
  OMIAllowNegativeCounts_        = OMIAllowNegativeCounts;

  // Assign to static const literal storage:

  if ( ! strcmp( VIIRSNOAAMinimumAcceptedQuality, "medium" ) ) {
    VIIRSNOAAMinimumAcceptedQuality_ = "medium";
  } else {
    VIIRSNOAAMinimumAcceptedQuality_ = "high";
  }

  if ( ! strcmp( PandoraMinimumAcceptedQuality, "low" ) ) {
    PandoraMinimumAcceptedQuality_ = "low";
  } else if ( ! strcmp( PandoraMinimumAcceptedQuality, "medium" ) ) {
    PandoraMinimumAcceptedQuality_ = "medium";
  } else {
    PandoraMinimumAcceptedQuality_ = "high";
  }

  if ( ! strcmp( TEMPOMinimumAcceptedQuality, "bad" ) ) {
    TEMPOMinimumAcceptedQuality_ = "bad";
  } else if ( ! strcmp( TEMPOMinimumAcceptedQuality, "suspect" ) ) {
    TEMPOMinimumAcceptedQuality_ = "suspect";
  } else {
    TEMPOMinimumAcceptedQuality_ = "normal";
  }

  TEMPOKey_ = TEMPOKey;
  OAQPSKey_ = OAQPSKey;
  PurpleAirAPIKey_ = PurpleAirAPIKey;
  PurpleAirOutInFlag_ = PurpleAirOutInFlag;
  PurpleAirMaximumDifference_ = PurpleAirMaximumDifference;
  PurpleAirMaximumRatio_ = PurpleAirMaximumRatio;
  PurpleAirMinimumAggregationCountPercentage_ =
    PurpleAirMinimumAggregationCountPercentage;
  PurpleAirDefaultHumidity_ = PurpleAirDefaultHumidity;
  dataVersion_ = dataVersion;

  POST30( this->CALIPSOMinimumAcceptedCADScore() ==
           CALIPSOMinimumAcceptedCADScore,
          this->CALIPSOMaximumAcceptedUncertainty() ==
             CALIPSOMaximumAcceptedUncertainty,
          this->GASPAODMinimum() == GASPAODMinimum,
          this->GASPAODMaximum() == GASPAODMaximum,
          this->GASPSTDMinimum() == GASPSTDMinimum,
          this->GASPSTDMaximum() == GASPSTDMaximum,
          this->GASPCLSMinimum() == GASPCLSMinimum,
          this->GASPCLSMaximum() == GASPCLSMaximum,
          this->GASPCH1Minimum() == GASPCH1Minimum,
          this->GASPCH1Maximum() == GASPCH1Maximum,
          this->GASPSIGMinimum() == GASPSIGMinimum,
          this->GASPSIGMaximum() == GASPSIGMaximum,
          this->GASPSFCMinimum() == GASPSFCMinimum,
          this->GASPSFCMaximum() == GASPSFCMaximum,
          this->GASPMOSMinimum() == GASPMOSMinimum,
          this->GASPMOSMaximum() == GASPMOSMaximum,
          this->GASPSCAMinimum() == GASPSCAMinimum,
          this->GASPSCAMaximum() == GASPSCAMaximum,
          AND5( this->TROPOMIMinimumAcceptedQuality() ==
                  TROPOMIMinimumAcceptedQuality,
                this->TROPOMIMaximumCloudFraction() ==
                  TROPOMIMaximumCloudFraction,
                this->TROPOMIGroundPixelMinimum_ ==
                  TROPOMIGroundPixelMinimum,
                this->TROPOMIGroundPixelMaximum_ ==
                  TROPOMIGroundPixelMaximum,
                this->TROPOMIAllowNegativeCounts() ==
                  TROPOMIAllowNegativeCounts ),
          AND6( this->TEMPOMaximumCloudFraction() ==
                  TEMPOMaximumCloudFraction,
                this->TEMPOMaximumSolarZenithAngle() ==
                  TEMPOMaximumSolarZenithAngle,
                this->TEMPOAllowNegativeCounts() == TEMPOAllowNegativeCounts,
                this->OMIMaximumCloudFraction() == OMIMaximumCloudFraction,
                this->OMIMaximumSolarZenithAngle() ==
                  OMIMaximumSolarZenithAngle,
                this->OMIAllowNegativeCounts() == OMIAllowNegativeCounts ),
          ! strcmp( VIIRSNOAAMinimumAcceptedQuality_,
                    VIIRSNOAAMinimumAcceptedQuality ),
          ! strcmp( PandoraMinimumAcceptedQuality_,
                    PandoraMinimumAcceptedQuality ),
          ! strcmp( TEMPOMinimumAcceptedQuality_,
                    TEMPOMinimumAcceptedQuality ),
          AND2( this->TEMPOKey() == TEMPOKey,
                this->OAQPSKey() == OAQPSKey ),
          this->PurpleAirAPIKey() == PurpleAirAPIKey,
          IN_RANGE( this->PurpleAirOutInFlag(), 0, 2 ),
          IN_RANGE( this->PurpleAirMaximumDifference(), 0.0, 100.0 ),
          IN_RANGE( this->PurpleAirMaximumRatio(), 0.0, 1.0 ),
          IN_RANGE( this->PurpleAirMinimumAggregationCountPercentage(),
                    0.0, 100.0 ),
          OR2( this->PurpleAirDefaultHumidity() == MISSING,
               IN_RANGE( this->PurpleAirDefaultHumidity(), 0.0, 100.0 ) ) );
}



/******************************************************************************
PURPOSE: DataFilterParameters - Copy constructor.
INPUTS:  const DataFilterParameters& other  Object to copy.
******************************************************************************/

DataFilterParameters::DataFilterParameters(const DataFilterParameters& other) {
  PRE0( other.invariant() );
  CALIPSOMinimumAcceptedCADScore_ = other.CALIPSOMinimumAcceptedCADScore_;
  CALIPSOMaximumAcceptedUncertainty_ =
    other.CALIPSOMaximumAcceptedUncertainty_;
  GASPAODMinimum_ = other.GASPAODMinimum_;
  GASPAODMaximum_ = other.GASPAODMaximum_;
  GASPSTDMinimum_ = other.GASPSTDMinimum_;
  GASPSTDMaximum_ = other.GASPSTDMaximum_;
  GASPCLSMinimum_ = other.GASPCLSMinimum_;
  GASPCLSMaximum_ = other.GASPCLSMaximum_;
  GASPCH1Minimum_ = other.GASPCH1Minimum_;
  GASPCH1Maximum_ = other.GASPCH1Maximum_;
  GASPSIGMinimum_ = other.GASPSIGMinimum_;
  GASPSIGMaximum_ = other.GASPSIGMaximum_;
  GASPSFCMinimum_ = other.GASPSFCMinimum_;
  GASPSFCMaximum_ = other.GASPSFCMaximum_;
  GASPMOSMinimum_ = other.GASPMOSMinimum_;
  GASPMOSMaximum_ = other.GASPMOSMaximum_;
  GASPSCAMinimum_ = other.GASPSCAMinimum_;
  GASPSCAMaximum_ = other.GASPSCAMaximum_;
  TROPOMIMinimumAcceptedQuality_ = other.TROPOMIMinimumAcceptedQuality_;
  TROPOMIMaximumCloudFraction_ = other.TROPOMIMaximumCloudFraction_;
  TROPOMIGroundPixelMinimum_ = other.TROPOMIGroundPixelMinimum_;
  TROPOMIGroundPixelMaximum_ = other.TROPOMIGroundPixelMaximum_;
  TROPOMIAllowNegativeCounts_ = other.TROPOMIAllowNegativeCounts_;
  VIIRSNOAAMinimumAcceptedQuality_ = other.VIIRSNOAAMinimumAcceptedQuality_;
  PandoraMinimumAcceptedQuality_ = other.PandoraMinimumAcceptedQuality_;
  TEMPOMinimumAcceptedQuality_ = other.TEMPOMinimumAcceptedQuality_;
  TEMPOMaximumCloudFraction_ = other.TEMPOMaximumCloudFraction_;
  TEMPOMaximumSolarZenithAngle_ = other.TEMPOMaximumSolarZenithAngle_;
  TEMPOAllowNegativeCounts_ = other.TEMPOAllowNegativeCounts_;
  OMIMaximumCloudFraction_ = other.OMIMaximumCloudFraction_;
  OMIMaximumSolarZenithAngle_ = other.OMIMaximumSolarZenithAngle_;
  OMIAllowNegativeCounts_ = other.OMIAllowNegativeCounts_;
  TEMPOKey_ = other.TEMPOKey_;
  OAQPSKey_ = other.OAQPSKey_;
  PurpleAirAPIKey_ = other.PurpleAirAPIKey_;
  PurpleAirOutInFlag_ = other.PurpleAirOutInFlag_;
  PurpleAirMaximumDifference_ = other.PurpleAirMaximumDifference_;
  PurpleAirMaximumRatio_      = other.PurpleAirMaximumRatio_;
  PurpleAirMinimumAggregationCountPercentage_ =
    other.PurpleAirMinimumAggregationCountPercentage_;
  PurpleAirDefaultHumidity_ = other.PurpleAirDefaultHumidity_;

  dataVersion_ = other.dataVersion_;

  POST( *this == other );
}



/******************************************************************************
PURPOSE: operator= - Assign object.
INPUTS:  const DataFilterParameters& other  Object to copy.
******************************************************************************/

DataFilterParameters& DataFilterParameters::operator=(
  const DataFilterParameters& other ) {
  PRE( other.invariant() );

  if ( this != &other ) {
    CALIPSOMinimumAcceptedCADScore_ = other.CALIPSOMinimumAcceptedCADScore_;
    CALIPSOMaximumAcceptedUncertainty_ =
      other.CALIPSOMaximumAcceptedUncertainty_;
    GASPAODMinimum_ = other.GASPAODMinimum_;
    GASPAODMaximum_ = other.GASPAODMaximum_;
    GASPSTDMinimum_ = other.GASPSTDMinimum_;
    GASPSTDMaximum_ = other.GASPSTDMaximum_;
    GASPCLSMinimum_ = other.GASPCLSMinimum_;
    GASPCLSMaximum_ = other.GASPCLSMaximum_;
    GASPCH1Minimum_ = other.GASPCH1Minimum_;
    GASPCH1Maximum_ = other.GASPCH1Maximum_;
    GASPSIGMinimum_ = other.GASPSIGMinimum_;
    GASPSIGMaximum_ = other.GASPSIGMaximum_;
    GASPSFCMinimum_ = other.GASPSFCMinimum_;
    GASPSFCMaximum_ = other.GASPSFCMaximum_;
    GASPMOSMinimum_ = other.GASPMOSMinimum_;
    GASPMOSMaximum_ = other.GASPMOSMaximum_;
    GASPSCAMinimum_ = other.GASPSCAMinimum_;
    GASPSCAMaximum_ = other.GASPSCAMaximum_;
    TROPOMIMinimumAcceptedQuality_ = other.TROPOMIMinimumAcceptedQuality_;
    TROPOMIMaximumCloudFraction_ = other.TROPOMIMaximumCloudFraction_;
    TROPOMIGroundPixelMinimum_ = other.TROPOMIGroundPixelMinimum_;
    TROPOMIGroundPixelMaximum_ = other.TROPOMIGroundPixelMaximum_;
    TROPOMIAllowNegativeCounts_ = other.TROPOMIAllowNegativeCounts_;
    VIIRSNOAAMinimumAcceptedQuality_ = other.VIIRSNOAAMinimumAcceptedQuality_;
    PandoraMinimumAcceptedQuality_ = other.PandoraMinimumAcceptedQuality_;
    TEMPOMinimumAcceptedQuality_ = other.TEMPOMinimumAcceptedQuality_;
    TEMPOMaximumCloudFraction_ = other.TEMPOMaximumCloudFraction_;
    TEMPOMaximumSolarZenithAngle_ = other.TEMPOMaximumSolarZenithAngle_;
    TEMPOAllowNegativeCounts_ = other.TEMPOAllowNegativeCounts_;
    OMIMaximumCloudFraction_ = other.OMIMaximumCloudFraction_;
    OMIMaximumSolarZenithAngle_ = other.OMIMaximumSolarZenithAngle_;
    OMIAllowNegativeCounts_ = other.OMIAllowNegativeCounts_;
    TEMPOKey_ = other.TEMPOKey_;
    OAQPSKey_ = other.OAQPSKey_;
    PurpleAirAPIKey_ = other.PurpleAirAPIKey_;
    PurpleAirOutInFlag_ = other.PurpleAirOutInFlag_;
    PurpleAirMaximumDifference_ = other.PurpleAirMaximumDifference_;
    PurpleAirMaximumRatio_      = other.PurpleAirMaximumRatio_;
    PurpleAirMinimumAggregationCountPercentage_ =
      other.PurpleAirMinimumAggregationCountPercentage_;
    PurpleAirDefaultHumidity_ = other.PurpleAirDefaultHumidity_;

    dataVersion_ = other.dataVersion_;
  }

  POST( *this == other );
  return *this;
}



/******************************************************************************
PURPOSE: CALIPSOMinimumAcceptedCADScore - Set named attribute to value.
INPUTS:  const double value  Value to set named attribute to.
******************************************************************************/

void DataFilterParameters::CALIPSOMinimumAcceptedCADScore(const double value) {
  PRE( IN_RANGE( value, 20.0, 100.0 ) );
  CALIPSOMinimumAcceptedCADScore_ = value;
  POST( CALIPSOMinimumAcceptedCADScore() == value );
}



/******************************************************************************
PURPOSE: CALIPSOMaximumAcceptedUncertainty - Set named attribute to value.
INPUTS:  const double value  Value to set named attribute to.
******************************************************************************/

void DataFilterParameters::CALIPSOMaximumAcceptedUncertainty(
  const double value ) {
  PRE( IN_RANGE( value, 0.0, 100.0 ) );
  CALIPSOMaximumAcceptedUncertainty_ = value;
  POST( CALIPSOMaximumAcceptedUncertainty() == value );
}



/******************************************************************************
PURPOSE: GASPAODRange - Set minimum and maximum AOD values.
INPUTS:  const double minimum  Minimum value.
INPUTS:  const double maximum  Maximum value.
******************************************************************************/

void DataFilterParameters::GASPAODRange( const double minimum,
                                         const double maximum ) {
  PRE2( IN_RANGE( minimum, -0.5, 2.05 ),
        IN_RANGE( maximum, minimum, 2.05 ) );
  GASPAODMinimum_ = minimum;
  GASPAODMaximum_ = maximum;
  POST2( GASPAODMinimum() == minimum, GASPAODMaximum() == maximum );
}



/******************************************************************************
PURPOSE: GASPSTDRange - Set minimum and maximum STD values.
INPUTS:  const double minimum  Minimum value.
INPUTS:  const double maximum  Maximum value.
******************************************************************************/

void DataFilterParameters::GASPSTDRange( const double minimum,
                                         const double maximum ) {
  PRE2( IN_RANGE( minimum, 0.0, 2.55 ),
        IN_RANGE( maximum, minimum, 2.55 ) );
  GASPSTDMinimum_ = minimum;
  GASPSTDMaximum_ = maximum;
  POST2( GASPSTDMinimum() == minimum, GASPSTDMaximum() == maximum );
}



/******************************************************************************
PURPOSE: GASPCLSRange - Set minimum and maximum CLS values.
INPUTS:  const double minimum  Minimum value.
INPUTS:  const double maximum  Maximum value.
******************************************************************************/

void DataFilterParameters::GASPCLSRange( const double minimum,
                                         const double maximum ) {
  PRE2( IN_RANGE( minimum, 0.0, 25.0 ),
        IN_RANGE( maximum, minimum, 25.0 ) );
  GASPCLSMinimum_ = minimum;
  GASPCLSMaximum_ = maximum;
  POST2( GASPCLSMinimum() == minimum, GASPCLSMaximum() == maximum );
}



/******************************************************************************
PURPOSE: GASPCH1Range - Set minimum and maximum CH1 values.
INPUTS:  const double minimum  Minimum value.
INPUTS:  const double maximum  Maximum value.
******************************************************************************/

void DataFilterParameters::GASPCH1Range( const double minimum,
                                         const double maximum ) {
  PRE2( IN_RANGE( minimum, 0.0, 0.425 ),
        IN_RANGE( maximum, minimum, 0.425 ) );
  GASPCH1Minimum_ = minimum;
  GASPCH1Maximum_ = maximum;
  POST2( GASPCH1Minimum() == minimum, GASPCH1Maximum() == maximum );
}



/******************************************************************************
PURPOSE: GASPSIGRange - Set minimum and maximum SIG values.
INPUTS:  const double minimum  Minimum value.
INPUTS:  const double maximum  Maximum value.
******************************************************************************/

void DataFilterParameters::GASPSIGRange( const double minimum,
                                         const double maximum ) {
  PRE2( IN_RANGE( minimum, -0.5, 0.52 ),
        IN_RANGE( maximum, minimum, 0.52 ) );
  GASPSIGMinimum_ = minimum;
  GASPSIGMaximum_ = maximum;
  POST2( GASPSIGMinimum() == minimum, GASPSIGMaximum() == maximum );
}



/******************************************************************************
PURPOSE: GASPSFCRange - Set minimum and maximum SFC values.
INPUTS:  const double minimum  Minimum value.
INPUTS:  const double maximum  Maximum value.
******************************************************************************/

void DataFilterParameters::GASPSFCRange( const double minimum,
                                         const double maximum ) {
  PRE2( IN_RANGE( minimum, -0.1, 0.41 ),
        IN_RANGE( maximum, minimum, 0.41 ) );
  GASPSFCMinimum_ = minimum;
  GASPSFCMaximum_ = maximum;
  POST2( GASPSFCMinimum() == minimum, GASPSFCMaximum() == maximum );
}



/******************************************************************************
PURPOSE: GASPMOSRange - Set minimum and maximum MOS values.
INPUTS:  const double minimum  Minimum value.
INPUTS:  const double maximum  Maximum value.
******************************************************************************/

void DataFilterParameters::GASPMOSRange( const double minimum,
                                         const double maximum ) {
  PRE2( IN_RANGE( minimum, 0.0, 0.425 ),
        IN_RANGE( maximum, minimum, 0.425 ) );
  GASPMOSMinimum_ = minimum;
  GASPMOSMaximum_ = maximum;
  POST2( GASPMOSMinimum() == minimum, GASPMOSMaximum() == maximum );
}



/******************************************************************************
PURPOSE: GASPSCARange - Set minimum and maximum SCA values.
INPUTS:  const double minimum  Minimum value.
INPUTS:  const double maximum  Maximum value.
******************************************************************************/

void DataFilterParameters::GASPSCARange( const double minimum,
                                         const double maximum ) {
  PRE2( IN_RANGE( minimum, 0.0, 255.0 ),
        IN_RANGE( maximum, minimum, 255.0 ) );
  GASPSCAMinimum_ = minimum;
  GASPSCAMaximum_ = maximum;
  POST2( GASPSCAMinimum() == minimum, GASPSCAMaximum() == maximum );
}



/******************************************************************************
PURPOSE: TROPOMIMinimumAcceptedQuality - Set named attribute to value.
INPUTS:  const int value  Value to set named attribute to.
******************************************************************************/

void DataFilterParameters::TROPOMIMinimumAcceptedQuality( const int value ) {
  PRE( IN_RANGE( value, 50, 100 ) );
  TROPOMIMinimumAcceptedQuality_ = value;
  POST( TROPOMIMinimumAcceptedQuality() == value );
}



/******************************************************************************
PURPOSE: TROPOMIMaximumCloudFraction - Set named attribute to value.
INPUTS:  const double value  Value to set named attribute to.
******************************************************************************/

void DataFilterParameters::TROPOMIMaximumCloudFraction( const double value ) {
  PRE( IN_RANGE( value, 0.0, 1.0 ) );
  TROPOMIMaximumCloudFraction_ = value;
  POST( TROPOMIMaximumCloudFraction() == value );
}



/******************************************************************************
PURPOSE: TROPOMIGroundPixelRange - Set TROPOMI ground pixel range to values.
INPUTS:  const int minimum  Minimum value of range.
         const int maximum  Maximum value of range.
******************************************************************************/

void DataFilterParameters::TROPOMIGroundPixelRange( const int minimum,
                                                    const int maximum ) {
  PRE( OR2( AND2( minimum == IMISSING, maximum == IMISSING ),
            AND2( minimum >= 0, maximum >= minimum ) ) );
  TROPOMIGroundPixelMinimum_ = minimum;
  TROPOMIGroundPixelMaximum_ = maximum;
  POST2( TROPOMIGroundPixelMinimum_ == minimum,
         TROPOMIGroundPixelMaximum_ == maximum );
}



/******************************************************************************
PURPOSE: TROPOMIAllowNegativeCounts - Set named attribute to value.
INPUTS:  const bool value  Value to set named attribute to.
******************************************************************************/

void DataFilterParameters::TROPOMIAllowNegativeCounts( const bool value ) {
  PRE( true );
  TROPOMIAllowNegativeCounts_ = value;
  POST( TROPOMIAllowNegativeCounts() == value );
}



/******************************************************************************
PURPOSE: VIIRSNOAAMinimumAcceptedQuality - Set named attribute to value.
INPUTS:  const int value  Value to set named attribute to.
******************************************************************************/

void DataFilterParameters::VIIRSNOAAMinimumAcceptedQuality(
  const char* const value ) {
  PRE( OR2( ! strcmp( value, "high" ), ! strcmp( value, "medium" ) ) );

  if ( ! strcmp( value, "medium" ) ) {
    VIIRSNOAAMinimumAcceptedQuality_ = "medium";
  } else {
    VIIRSNOAAMinimumAcceptedQuality_ = "high";
  }

  POST( ! strcmp( VIIRSNOAAMinimumAcceptedQuality(), value ) );
}



/******************************************************************************
PURPOSE: PandoraMinimumAcceptedQuality - Set named attribute to value.
INPUTS:  const int value  Value to set named attribute to.
******************************************************************************/

void DataFilterParameters::PandoraMinimumAcceptedQuality(
  const char* const value ) {
  PRE( OR3( ! strcmp( value, "high" ),
            ! strcmp( value, "medium" ),
            ! strcmp( value, "low" ) ) );

  if ( ! strcmp( value, "low" ) ) {
    PandoraMinimumAcceptedQuality_ = "low";
  } else if ( ! strcmp( value, "medium" ) ) {
    PandoraMinimumAcceptedQuality_ = "medium";
  } else {
    PandoraMinimumAcceptedQuality_ = "high";
  }

  POST( ! strcmp( PandoraMinimumAcceptedQuality(), value ) );
}



/******************************************************************************
PURPOSE: TEMPOMinimumAcceptedQuality - Set named attribute to value.
INPUTS:  const int value  Value to set named attribute to.
******************************************************************************/

void DataFilterParameters::TEMPOMinimumAcceptedQuality(
  const char* const value ) {
  PRE( OR3( ! strcmp( value, "normal" ),
            ! strcmp( value, "suspect" ),
            ! strcmp( value, "bad" ) ) );

  if ( ! strcmp( value, "bad" ) ) {
    TEMPOMinimumAcceptedQuality_ = "bad";
  } else if ( ! strcmp( value, "suspect" ) ) {
    TEMPOMinimumAcceptedQuality_ = "suspect";
  } else {
    TEMPOMinimumAcceptedQuality_ = "normal";
  }

  POST( ! strcmp( TEMPOMinimumAcceptedQuality(), value ) );
}



/******************************************************************************
PURPOSE: TEMPOMaximumCloudFraction - Set named attribute to value.
INPUTS:  const double value  Value to set named attribute to.
******************************************************************************/

void DataFilterParameters::TEMPOMaximumCloudFraction( const double value ) {
  PRE( IN_RANGE( value, 0.0, 1.0 ) );
  TEMPOMaximumCloudFraction_ = value;
  POST( TEMPOMaximumCloudFraction() == value );
}



/******************************************************************************
PURPOSE: TEMPOMaximumSolarZenithAngle - Set named attribute to value.
INPUTS:  const double value  Value to set named attribute to.
******************************************************************************/

void DataFilterParameters::TEMPOMaximumSolarZenithAngle( const double value ) {
  PRE( IN_RANGE( value, 0.0, 90.0 ) );
  TEMPOMaximumSolarZenithAngle_ = value;
  POST( TEMPOMaximumSolarZenithAngle() == value );
}



/******************************************************************************
PURPOSE: TEMPOAllowNegativeCounts - Set named attribute to value.
INPUTS:  const bool value  Value to set named attribute to.
******************************************************************************/

void DataFilterParameters::TEMPOAllowNegativeCounts( const bool value ) {
  PRE( true );
  TEMPOAllowNegativeCounts_ = value;
  POST( TEMPOAllowNegativeCounts() == value );
}



/******************************************************************************
PURPOSE: OMIMaximumCloudFraction - Set named attribute to value.
INPUTS:  const double value  Value to set named attribute to.
******************************************************************************/

void DataFilterParameters::OMIMaximumCloudFraction( const double value ) {
  PRE( IN_RANGE( value, 0.0, 1.0 ) );
  OMIMaximumCloudFraction_ = value;
  POST( OMIMaximumCloudFraction() == value );
}



/******************************************************************************
PURPOSE: OMIMaximumSolarZenithAngle - Set named attribute to value.
INPUTS:  const double value  Value to set named attribute to.
******************************************************************************/

void DataFilterParameters::OMIMaximumSolarZenithAngle( const double value ) {
  PRE( IN_RANGE( value, 0.0, 90.0 ) );
  OMIMaximumSolarZenithAngle_ = value;
  POST( OMIMaximumSolarZenithAngle() == value );
}



/******************************************************************************
PURPOSE: OMIAllowNegativeCounts - Set named attribute to value.
INPUTS:  const bool value  Value to set named attribute to.
******************************************************************************/

void DataFilterParameters::OMIAllowNegativeCounts( const bool value ) {
  PRE( true );
  OMIAllowNegativeCounts_ = value;
  POST( OMIAllowNegativeCounts() == value );
}



/******************************************************************************
PURPOSE: TEMPOKey - Set named attribute to value.
INPUTS:  const std::string value  Value to set named attribute to.
******************************************************************************/

void DataFilterParameters::TEMPOKey( const std::string& value ) {
  PRE( true );
  TEMPOKey_ = value;
  POST( TEMPOKey() == value );
}



/******************************************************************************
PURPOSE: OAQPSKey - Set named attribute to value.
INPUTS:  const std::string value  Value to set named attribute to.
******************************************************************************/

void DataFilterParameters::OAQPSKey( const std::string& value ) {
  PRE( true );
  OAQPSKey_ = value;
  POST( OAQPSKey() == value );
}



/******************************************************************************
PURPOSE: PurpleAirAPIKey - Set named attribute to value.
INPUTS:  const std::string value  Value to set named attribute to.
******************************************************************************/

void DataFilterParameters::PurpleAirAPIKey( const std::string& value ) {
  PRE( true );
  PurpleAirAPIKey_ = value;
  POST( PurpleAirAPIKey() == value );
}



/******************************************************************************
PURPOSE: PurpleAirOutInFlag - Set attribute to value.
INPUTS:  const int value  Value to set named attribute to.
******************************************************************************/

void DataFilterParameters::PurpleAirOutInFlag( const int value ) {
  PRE( IN_RANGE( value, 0, 2 ) );
  PurpleAirOutInFlag_ = value;
  POST( PurpleAirOutInFlag() == value );
}



/******************************************************************************
PURPOSE: PurpleAirMaximumDifference - Set attribute to value.
INPUTS:  const double value  Value to set named attribute to.
******************************************************************************/

void DataFilterParameters::PurpleAirMaximumDifference( const double value ) {
  PRE( IN_RANGE( value, 0.0, 100.0 ) );
  PurpleAirMaximumDifference_ = value;
  POST( PurpleAirMaximumDifference() == value );
}



/******************************************************************************
PURPOSE: PurpleAirMaximumRatio - Set attribute to value.
INPUTS:  const double value  Value to set named attribute to.
******************************************************************************/

void DataFilterParameters::PurpleAirMaximumRatio( const double value ) {
  PRE( IN_RANGE( value, 0.0, 1.0 ) );
  PurpleAirMaximumRatio_ = value;
  POST( PurpleAirMaximumRatio() == value );
}



/******************************************************************************
PURPOSE: PurpleAirMinimumAggregationCountPercentage - Set attribute to value.
INPUTS:  const double value  Value to set named attribute to.
******************************************************************************/

void DataFilterParameters::PurpleAirMinimumAggregationCountPercentage(
                                                        const double value ) {
  PRE( IN_RANGE( value, 0.0, 100.0 ) );
  PurpleAirMinimumAggregationCountPercentage_ = value;
  POST( PurpleAirMinimumAggregationCountPercentage() == value );
}



/******************************************************************************
PURPOSE: PurpleAirDefaultHumidity - Set named attribute to value.
INPUTS:  const double value  Value to set named attribute to.
******************************************************************************/

void DataFilterParameters::PurpleAirDefaultHumidity( const double value ) {
  PRE( OR2( value == MISSING, IN_RANGE( value, 0.0, 100.0 ) ) );
  PurpleAirDefaultHumidity_ = value;
  POST( PurpleAirDefaultHumidity() == value );
}



/******************************************************************************
PURPOSE: dataVersion - Set named attribute to value.
INPUTS:  const std::string value  Value to set named attribute to.
******************************************************************************/

void DataFilterParameters::dataVersion( const std::string& value ) {
  PRE( true );
  dataVersion_ = value;
  POST( dataVersion() == value );
}



/******************************************************************************
PURPOSE: operator== - Are objects equal?
INPUTS:  const DataFilterParameters& other  Object to compare to.
RETURNS: bool true if equal, else false.
******************************************************************************/

bool DataFilterParameters::operator==( const DataFilterParameters& other )
  const {
  PRE( other.invariant() );
  const bool result =
    AND30( CALIPSOMinimumAcceptedCADScore_ ==
             other.CALIPSOMinimumAcceptedCADScore_,
           CALIPSOMaximumAcceptedUncertainty_ ==
             other.CALIPSOMaximumAcceptedUncertainty_,
           GASPAODMinimum_ == other.GASPAODMinimum_,
           GASPAODMaximum_ == other.GASPAODMaximum_,
           GASPSTDMinimum_ == other.GASPSTDMinimum_,
           GASPSTDMaximum_ == other.GASPSTDMaximum_,
           GASPCLSMinimum_ == other.GASPCLSMinimum_,
           GASPCLSMaximum_ == other.GASPCLSMaximum_,
           GASPCH1Minimum_ == other.GASPCH1Minimum_,
           GASPCH1Maximum_ == other.GASPCH1Maximum_,
           GASPSIGMinimum_ == other.GASPSIGMinimum_,
           GASPSIGMaximum_ == other.GASPSIGMaximum_,
           GASPSFCMinimum_ == other.GASPSFCMinimum_,
           GASPSFCMaximum_ == other.GASPSFCMaximum_,
           GASPMOSMinimum_ == other.GASPMOSMinimum_,
           GASPMOSMaximum_ == other.GASPMOSMaximum_,
           GASPSCAMinimum_ == other.GASPSCAMinimum_,
           GASPSCAMaximum_ == other.GASPSCAMaximum_,
           AND4( TROPOMIMinimumAcceptedQuality_ ==
                   other.TROPOMIMinimumAcceptedQuality_,
                 TROPOMIGroundPixelMinimum_ == other.TROPOMIGroundPixelMinimum_,
                 TROPOMIGroundPixelMaximum_ == other.TROPOMIGroundPixelMaximum_,
                 TROPOMIAllowNegativeCounts_ ==
                  other.TROPOMIAllowNegativeCounts_ ),
           AND6( TEMPOMaximumCloudFraction_ ==
                   other.TEMPOMaximumCloudFraction_,
                 TEMPOMaximumSolarZenithAngle_ ==
                   other.TEMPOMaximumSolarZenithAngle_,
                 TEMPOAllowNegativeCounts_ ==
                   other.TEMPOAllowNegativeCounts_,
                 OMIMaximumCloudFraction_ == other.OMIMaximumCloudFraction_,
                 OMIMaximumSolarZenithAngle_ ==
                   other.TEMPOMaximumSolarZenithAngle_,
                 OMIAllowNegativeCounts_ == other.OMIAllowNegativeCounts_ ),
           ! strcmp( VIIRSNOAAMinimumAcceptedQuality_,
                     other.VIIRSNOAAMinimumAcceptedQuality_ ),
           ! strcmp( PandoraMinimumAcceptedQuality_,
                     other.PandoraMinimumAcceptedQuality_ ),
           AND3( ! strcmp( TEMPOMinimumAcceptedQuality_,
                     other.TEMPOMinimumAcceptedQuality_ ),
                 TEMPOKey_ == other.TEMPOKey_,
                 OAQPSKey_ == other.OAQPSKey_ ),
           PurpleAirAPIKey_ == other.PurpleAirAPIKey_,
           PurpleAirOutInFlag_ == other.PurpleAirOutInFlag_,
           PurpleAirMaximumDifference_ == other.PurpleAirMaximumDifference_,
           PurpleAirMaximumRatio_      == other.PurpleAirMaximumRatio_,
           PurpleAirMinimumAggregationCountPercentage_ ==
             other.PurpleAirMinimumAggregationCountPercentage_,
           PurpleAirDefaultHumidity_ == other.PurpleAirDefaultHumidity_,
           dataVersion_ == other.dataVersion_ );
  return result;
}



/******************************************************************************
PURPOSE: operator!= - Are objects unequal?
INPUTS:  const DataFilterParameters& other  Object to compare to.
RETURNS: bool true if unequal, else false.
******************************************************************************/

bool DataFilterParameters::operator!=( const DataFilterParameters& other )
  const {
  PRE( other.invariant() );
  const bool result = ! ( *this == other );
  return result;
}



/******************************************************************************
PURPOSE: invariant - Class invariant.
RETURNS: bool true otherwise the code is defective.
******************************************************************************/

bool DataFilterParameters::invariant() const {
  const bool result =
    isValidParameters( CALIPSOMinimumAcceptedCADScore_,
                       CALIPSOMaximumAcceptedUncertainty_,
                       GASPAODMinimum_,
                       GASPAODMaximum_,
                       GASPSTDMinimum_,
                       GASPSTDMaximum_,
                       GASPCLSMinimum_,
                       GASPCLSMaximum_,
                       GASPCH1Minimum_,
                       GASPCH1Maximum_,
                       GASPSIGMinimum_,
                       GASPSIGMaximum_,
                       GASPSFCMinimum_,
                       GASPSFCMaximum_,
                       GASPMOSMinimum_,
                       GASPMOSMaximum_,
                       GASPSCAMinimum_,
                       GASPSCAMaximum_,
                       TROPOMIMinimumAcceptedQuality_,
                       TROPOMIMaximumCloudFraction_,
                       TROPOMIGroundPixelMinimum_,
                       TROPOMIGroundPixelMaximum_,
                       TROPOMIAllowNegativeCounts_,
                       VIIRSNOAAMinimumAcceptedQuality_,
                       PandoraMinimumAcceptedQuality_,
                       TEMPOMinimumAcceptedQuality_,
                       TEMPOMaximumCloudFraction_,
                       TEMPOMaximumSolarZenithAngle_,
                       TEMPOAllowNegativeCounts_,
                       OMIMaximumCloudFraction_,
                       OMIMaximumSolarZenithAngle_,
                       OMIAllowNegativeCounts_,
                       TEMPOKey_,
                       OAQPSKey_,
                       PurpleAirAPIKey_,
                       PurpleAirOutInFlag_,
                       PurpleAirMaximumDifference_,
                       PurpleAirMaximumRatio_,
                       PurpleAirMinimumAggregationCountPercentage_,
                       PurpleAirDefaultHumidity_,
                       dataVersion_ );
  return result;
}



/******************************************************************************
PURPOSE: CALIPSOMinimumAcceptedCADScore - Returns value of named attribute.
RETURNS: double CALIPSOMinimumAcceptedCADScore.
******************************************************************************/

double DataFilterParameters::CALIPSOMinimumAcceptedCADScore() const {
  PRE( true );
  const double result = CALIPSOMinimumAcceptedCADScore_;
  POST( IN_RANGE( result, 20.0, 100.0 ) );
  return result;
}



/******************************************************************************
PURPOSE: CALIPSOMaximumAcceptedUncertainty - Returns value of named attribute.
RETURNS: double CALIPSOMaximumAcceptedUncertainty.
******************************************************************************/

double DataFilterParameters::CALIPSOMaximumAcceptedUncertainty() const {
  PRE( true );
  const double result = CALIPSOMaximumAcceptedUncertainty_;
  POST( IN_RANGE( result, 0.0, 100.0 ) );
  return result;
}



/******************************************************************************
PURPOSE: GASPAODMinimum - Returns value of named attribute.
RETURNS: double GASPAODMinimum.
******************************************************************************/

double DataFilterParameters::GASPAODMinimum() const {
  PRE( true );
  const double result = GASPAODMinimum_;
  POST( IN_RANGE( result, -0.5, 2.05 ) );
  return result;
}



/******************************************************************************
PURPOSE: GASPAODMaximum - Returns value of named attribute.
RETURNS: double GASPAODMaximum.
******************************************************************************/

double DataFilterParameters::GASPAODMaximum() const {
  PRE( true );
  const double result = GASPAODMaximum_;
  POST( IN_RANGE( result, -0.5, 2.05 ) );
  return result;
}



/******************************************************************************
PURPOSE: GASPSTDMinimum - Returns value of named attribute.
RETURNS: double GASPSTDMinimum.
******************************************************************************/

double DataFilterParameters::GASPSTDMinimum() const {
  PRE( true );
  const double result = GASPSTDMinimum_;
  POST( IN_RANGE( result, 0.0, 2.55 ) );
  return result;
}



/******************************************************************************
PURPOSE: GASPSTDMaximum - Returns value of named attribute.
RETURNS: double GASPSTDMaximum.
******************************************************************************/

double DataFilterParameters::GASPSTDMaximum() const {
  PRE( true );
  const double result = GASPSTDMaximum_;
  POST( IN_RANGE( result, 0.0, 2.55 ) );
  return result;
}



/******************************************************************************
PURPOSE: GASPCLSMinimum - Returns value of named attribute.
RETURNS: double GASPCLSMinimum.
******************************************************************************/

double DataFilterParameters::GASPCLSMinimum() const {
  PRE( true );
  const double result = GASPCLSMinimum_;
  POST( IN_RANGE( result, 0.0, 25.0 ) );
  return result;
}



/******************************************************************************
PURPOSE: GASPCLSMaximum - Returns value of named attribute.
RETURNS: double GASPCLSMaximum.
******************************************************************************/

double DataFilterParameters::GASPCLSMaximum() const {
  PRE( true );
  const double result = GASPCLSMaximum_;
  POST( IN_RANGE( result, 0.0, 25.0 ) );
  return result;
}



/******************************************************************************
PURPOSE: GASPCH1Minimum - Returns value of named attribute.
RETURNS: double GASPCH1Minimum.
******************************************************************************/

double DataFilterParameters::GASPCH1Minimum() const {
  PRE( true );
  const double result = GASPCH1Minimum_;
  POST( IN_RANGE( result, 0.0, 0.425 ) );
  return result;
}



/******************************************************************************
PURPOSE: GASPCH1Maximum - Returns value of named attribute.
RETURNS: double GASPCH1Maximum.
******************************************************************************/

double DataFilterParameters::GASPCH1Maximum() const {
  PRE( true );
  const double result = GASPCH1Maximum_;
  POST( IN_RANGE( result, 0.0, 0.425 ) );
  return result;
}



/******************************************************************************
PURPOSE: GASPSIGMinimum - Returns value of named attribute.
RETURNS: double GASPSIGMinimum.
******************************************************************************/

double DataFilterParameters::GASPSIGMinimum() const {
  PRE( true );
  const double result = GASPSIGMinimum_;
  POST( IN_RANGE( result, -0.5, 0.52 ) );
  return result;
}



/******************************************************************************
PURPOSE: GASPSIGMaximum - Returns value of named attribute.
RETURNS: double GASPSIGMaximum.
******************************************************************************/

double DataFilterParameters::GASPSIGMaximum() const {
  PRE( true );
  const double result = GASPSIGMaximum_;
  POST( IN_RANGE( result, -0.5, 0.52 ) );
  return result;
}



/******************************************************************************
PURPOSE: GASPSFCMinimum - Returns value of named attribute.
RETURNS: double GASPSFCMinimum.
******************************************************************************/

double DataFilterParameters::GASPSFCMinimum() const {
  PRE( true );
  const double result = GASPSFCMinimum_;
  POST( IN_RANGE( result, -0.1, 0.41 ) );
  return result;
}



/******************************************************************************
PURPOSE: GASPSFCMaximum - Returns value of named attribute.
RETURNS: double GASPSFCMaximum.
******************************************************************************/

double DataFilterParameters::GASPSFCMaximum() const {
  PRE( true );
  const double result = GASPSFCMaximum_;
  POST( IN_RANGE( result, -0.1, 0.41 ) );
  return result;
}



/******************************************************************************
PURPOSE: GASPMOSMinimum - Returns value of named attribute.
RETURNS: double GASPMOSMinimum.
******************************************************************************/

double DataFilterParameters::GASPMOSMinimum() const {
  PRE( true );
  const double result = GASPMOSMinimum_;
  POST( IN_RANGE( result, 0.0, 0.425 ) );
  return result;
}



/******************************************************************************
PURPOSE: GASPMOSMaximum - Returns value of named attribute.
RETURNS: double GASPMOSMaximum.
******************************************************************************/

double DataFilterParameters::GASPMOSMaximum() const {
  PRE( true );
  const double result = GASPMOSMaximum_;
  POST( IN_RANGE( result, 0.0, 0.425 ) );
  return result;
}



/******************************************************************************
PURPOSE: GASPSCAMinimum - Returns value of named attribute.
RETURNS: double GASPSCAMinimum.
******************************************************************************/

double DataFilterParameters::GASPSCAMinimum() const {
  PRE( true );
  const double result = GASPSCAMinimum_;
  POST( IN_RANGE( result, 0.0, 255.0 ) );
  return result;
}



/******************************************************************************
PURPOSE: GASPSCAMaximum - Returns value of named attribute.
RETURNS: double GASPSCAMaximum.
******************************************************************************/

double DataFilterParameters::GASPSCAMaximum() const {
  PRE( true );
  const double result = GASPSCAMaximum_;
  POST( IN_RANGE( result, 0.0, 255.0 ) );
  return result;
}



/******************************************************************************
PURPOSE: TROPOMIMinimumAcceptedQuality - Returns value of named attribute.
RETURNS: int TROPOMIMinimumAcceptedQuality.
******************************************************************************/

int DataFilterParameters::TROPOMIMinimumAcceptedQuality() const {
  PRE( true );
  const int result = TROPOMIMinimumAcceptedQuality_;
  POST( IN_RANGE( result, 50, 100 ) );
  return result;
}



/******************************************************************************
PURPOSE: TROPOMIMaximumCloudFraction - Returns value of named attribute.
RETURNS: double TROPOMIMaximumCloudFraction.
******************************************************************************/

double DataFilterParameters::TROPOMIMaximumCloudFraction() const {
  PRE( true );
  const double result = TROPOMIMaximumCloudFraction_;
  POST( IN_RANGE( result, 0.0, 1.0 ) );
  return result;
}



/******************************************************************************
PURPOSE: TROPOMIGroundPIxelMinimum - Returns value of named attribute.
RETURNS: int TROPOMIGroundPixelMinimum.
******************************************************************************/

int DataFilterParameters::TROPOMIGroundPixelMinimum() const {
  PRE( true );
  const int result = TROPOMIGroundPixelMinimum_;
  POST( OR2( result == IMISSING, result >= 0 ) );
  return result;
}



/******************************************************************************
PURPOSE: TROPOMIGroundPIxelMaximum - Returns value of named attribute.
RETURNS: int TROPOMIGroundPixelMaximum.
******************************************************************************/

int DataFilterParameters::TROPOMIGroundPixelMaximum() const {
  PRE( true );
  const int result = TROPOMIGroundPixelMaximum_;
  POST( OR2( result == IMISSING, result >= 0 ) );
  return result;
}



/******************************************************************************
PURPOSE: TROPOMIAllowNegativeCounts - Returns value of named attribute.
RETURNS: bool TROPOMIAllowNegativeCounts.
******************************************************************************/

bool DataFilterParameters::TROPOMIAllowNegativeCounts() const {
  PRE( true );
  const bool result = TROPOMIAllowNegativeCounts_;
  return result;
}



/******************************************************************************
PURPOSE: VIIRSNOAAMinimumAcceptedQuality - Returns value of named attribute.
RETURNS: const char* VIIRSNOAAMinimumAcceptedQuality.
******************************************************************************/

const char* DataFilterParameters::VIIRSNOAAMinimumAcceptedQuality() const {
  PRE( true );
  const char* result =
    ! strcmp( VIIRSNOAAMinimumAcceptedQuality_, "medium" ) ? "medium" : "high";

  POST( OR2( ! strcmp( result, "high" ), ! strcmp( result, "medium" ) ) );
  return result;
}



/******************************************************************************
PURPOSE: PandoraMinimumAcceptedQuality - Returns value of named attribute.
RETURNS: const char* PandoraMinimumAcceptedQuality.
******************************************************************************/

const char* DataFilterParameters::PandoraMinimumAcceptedQuality() const {
  PRE( true );
  const char* result =
    ! strcmp( PandoraMinimumAcceptedQuality_, "low" ) ? "low"
    : ! strcmp( PandoraMinimumAcceptedQuality_, "medium" ) ? "medium"
    : "high";

  POST( OR3( ! strcmp( result, "high" ),
             ! strcmp( result, "medium" ),
             ! strcmp( result, "low" ) ) );
  return result;
}



/******************************************************************************
PURPOSE: TEMPOMinimumAcceptedQuality - Returns value of named attribute.
RETURNS: const char* TEMPOMinimumAcceptedQuality.
******************************************************************************/

const char* DataFilterParameters::TEMPOMinimumAcceptedQuality() const {
  PRE( true );
  const char* result =
    ! strcmp( TEMPOMinimumAcceptedQuality_, "bad" ) ? "bad"
    : ! strcmp( TEMPOMinimumAcceptedQuality_, "suspect" ) ? "suspect"
    : "normal";

  POST( OR3( ! strcmp( result, "normal" ),
             ! strcmp( result, "suspect" ),
             ! strcmp( result, "bad" ) ) );
  return result;
}



/******************************************************************************
PURPOSE: TEMPOMaximumCloudFraction - Returns value of named attribute.
RETURNS: double TEMPOMaximumCloudFraction.
******************************************************************************/

double DataFilterParameters::TEMPOMaximumCloudFraction() const {
  PRE( true );
  const double result = TEMPOMaximumCloudFraction_;
  POST( IN_RANGE( result, 0.0, 1.0 ) );
  return result;
}



/******************************************************************************
PURPOSE: TEMPOMaximumSolarZenithAngle - Returns value of named attribute.
RETURNS: double TEMPOMaximumSolarZenithAngle.
******************************************************************************/

double DataFilterParameters::TEMPOMaximumSolarZenithAngle() const {
  PRE( true );
  const double result = TEMPOMaximumSolarZenithAngle_;
  POST( IN_RANGE( result, 0.0, 90.0 ) );
  return result;
}



/******************************************************************************
PURPOSE: TEMPOAllowNegativeCounts - Returns value of named attribute.
RETURNS: bool TEMPOAllowNegativeCounts.
******************************************************************************/

bool DataFilterParameters::TEMPOAllowNegativeCounts() const {
  PRE( true );
  const bool result = TEMPOAllowNegativeCounts_;
  return result;
}



/******************************************************************************
PURPOSE: OMIMaximumCloudFraction - Returns value of named attribute.
RETURNS: double OMIMaximumCloudFraction.
******************************************************************************/

double DataFilterParameters::OMIMaximumCloudFraction() const {
  PRE( true );
  const double result = OMIMaximumCloudFraction_;
  POST( IN_RANGE( result, 0.0, 1.0 ) );
  return result;
}



/******************************************************************************
PURPOSE: OMIMaximumSolarZenithAngle - Returns value of named attribute.
RETURNS: double OMIMaximumSolarZenithAngle.
******************************************************************************/

double DataFilterParameters::OMIMaximumSolarZenithAngle() const {
  PRE( true );
  const double result = OMIMaximumSolarZenithAngle_;
  POST( IN_RANGE( result, 0.0, 90.0 ) );
  return result;
}



/******************************************************************************
PURPOSE: OMIAllowNegativeCounts - Returns value of named attribute.
RETURNS: bool OMIAllowNegativeCounts.
******************************************************************************/

bool DataFilterParameters::OMIAllowNegativeCounts() const {
  PRE( true );
  const bool result = OMIAllowNegativeCounts_;
  return result;
}



/******************************************************************************
PURPOSE: TEMPOKey - Returns value of named attribute.
RETURNS: const char* TEMPOKey.
******************************************************************************/

const std::string& DataFilterParameters::TEMPOKey() const {
  PRE( true );
  return TEMPOKey_;
}



/******************************************************************************
PURPOSE: OAQPSKey - Returns value of named attribute.
RETURNS: const char* OAQPSKey.
******************************************************************************/

const std::string& DataFilterParameters::OAQPSKey() const {
  PRE( true );
  return OAQPSKey_;
}



/******************************************************************************
PURPOSE: PurpleAirAPIKey - Returns value of named attribute.
RETURNS: const char* PurpleAirAPIKey.
******************************************************************************/

const std::string& DataFilterParameters::PurpleAirAPIKey() const {
  PRE( true );
  return PurpleAirAPIKey_;
}



/******************************************************************************
PURPOSE: PurpleAirOutInFlag - Returns value of named attribute.
RETURNS: int PurpleAirOutInFlag.
******************************************************************************/

int DataFilterParameters::PurpleAirOutInFlag() const {
  PRE( true );
  const double result = PurpleAirOutInFlag_;
  POST( IN_RANGE( result, 0, 2 ) );
  return result;
}



/******************************************************************************
PURPOSE: PurpleAirMaximumDifference - Returns value of named attribute.
RETURNS: double PurpleAirMaximumDifference.
******************************************************************************/

double DataFilterParameters::PurpleAirMaximumDifference() const {
  PRE( true );
  const double result = PurpleAirMaximumDifference_;
  POST( IN_RANGE( result, 0.0, 100.0 ) );
  return result;
}



/******************************************************************************
PURPOSE: PurpleAirMaximumRatio - Returns value of named attribute.
RETURNS: double PurpleAirMaximumRatio.
******************************************************************************/

double DataFilterParameters::PurpleAirMaximumRatio() const {
  PRE( true );
  const double result = PurpleAirMaximumRatio_;
  POST( IN_RANGE( result, 0.0, 1.0 ) );
  return result;
}



/******************************************************************************
PURPOSE: PurpleAirMinimumAggregationCountPercentage - Returns value of named
         attribute.
RETURNS: double PurpleAirMinimumAggregationCountPercentage.
******************************************************************************/

double DataFilterParameters::PurpleAirMinimumAggregationCountPercentage() const {
  PRE( true );
  const double result = PurpleAirMinimumAggregationCountPercentage_;
  POST( IN_RANGE( result, 0.0, 100.0 ) );
  return result;
}



/******************************************************************************
PURPOSE: PurpleAirDefaultHumidity - Returns value of named attribute.
RETURNS: double PurpleAirDefaultHumidity.
******************************************************************************/

double DataFilterParameters::PurpleAirDefaultHumidity() const {
  PRE( true );
  const double result = PurpleAirDefaultHumidity_;
  POST( OR2( result == MISSING, IN_RANGE( result, 0.0, 100.0 ) ) );
  return result;
}



/******************************************************************************
PURPOSE: dataVersion - Returns value of named attribute.
RETURNS: const char* dataVersion.
******************************************************************************/

const std::string& DataFilterParameters::dataVersion() const {
  PRE( true );
  return dataVersion_;
}



} // End namespace emvl.



