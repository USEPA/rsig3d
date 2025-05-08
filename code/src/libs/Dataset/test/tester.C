/******************************************************************************
PURPOSE: tester.C - Test driver for Dataset Library.
HISTORY: 2012-11-20 plessel.todd@epa.gov
******************************************************************************/

//================================ INCLUDES ===================================

#include <assert.h> // For assert().
#include <limits.h> // For INT_MAX, etc.
#include <float.h>  // For DBL_MAX.
#include <math.h>   // For fabs().
#include <stdio.h>  // For FILE, stderr, fprintf(), snprintf(), fflush().
#include <string.h> // For memset(), memcpy(), memcmp().
#include <ctype.h>  // For isupper().
#include <time.h>   // For time().
#include <unistd.h> // For unlink().
#include <sys/stat.h> // For mkdir().

#include <algorithm> // For std::sort().
#include <iostream>  // For std::cerr, std::endl.
#include <exception> // For std::exception.

#include <Assertions.h> // For AND*(), OR*(), IMPLIES_ELSE().
#include <MapFile.h>    // For readMapFile*(), subsetMapFloat().
#include <KMLFile.h>    // For write*KML().

#include <ResourceHandler.h>          // For emvl::ResourceHandler.
#include <DataUtilities.h>            // For emvl:: utility routines.
#include <Bounds.h>                   // For emvl::Bounds.
#include <Timestamp.h>                // For emvl::Timestamp.
#include <MapProjection.h>            // For emvl::MapProjection.
#include <Lambert.h>                  // For emvl::Lambert.
#include <Mercator.h>                 // For emvl::Mercator.
#include <Albers.h>                   // For emvl::Albers.
#include <Stereographic.h>            // For emvl::Stereographic.
#include <HTTPConnection.h>           // For emvl::HTTPConnection.
#include <DataFilterParameters.h>     // For emvl::DataFilterParameters.
#include <CMAQGridParameters.h>       // For emvl::CMAQGridParameters.
#include <QueryParameters.h>          // For emvl::QueryParameters.
#include <Dataset.h>                  // For emvl::Dataset.
#include <PointDataset.h>             // For emvl::PointDataset.
#include <RegriddedPointDataset.h>    // For emvl::RegriddedPointDataset.
#include <SiteDataset.h>              // For emvl::SiteDataset.
#include <RegriddedSiteDataset.h>     // For emvl::RegriddedSiteDataset.
#include <CMAQDataset.h>              // For emvl::CMAQDataset.
#include <SwathDataset.h>             // For emvl::SwathDataset.
#include <RegriddedSwathDataset.h>    // For emvl::RegriddedSwathDataset.
#include <AircraftDataset.h>          // For emvl::AircraftDataset.
#include <RegriddedAircraftDataset.h> // For emvl::RegriddedAircraftDataset.
#include <ProfileDataset.h>           // For emvl::ProfileDataset.
#include <RegriddedProfileDataset.h>  // For emvl::RegriddedProfileDataset.
#include <CALIPSODataset.h>           // For emvl::CALIPSODataset.
#include <RegriddedCALIPSODataset.h>  // For emvl::RegriddedCALIPSODataset.
#include <GridDataset.h>              // For emvl::GridDataset.
#include <RegriddedGridDataset.h>     // For emvl::RegriddedGridDataset.
#include <PolygonDataset.h>           // For emvl::PolygonDataset.
#include <DatasetManager.h>           // For emvl::DatasetManager.

//============================ PRIVATE VARIABLES ==============================

static const char* const testFileName      = "test/test_file"; // Read-only.
static const char* const emptyFileName     = "test/empty_test_file";
static const char* const testDirectoryName = "test/output_dir"; // Writable.
static const char* const tempDirectoryName = "test/output_dir/junk_dir";
static FILE* outputKMLFile = 0;
static const char* const tempFileName      = "test/junk";      // Transient.
static const char* const nonExistentFileName = "/no/such/file";
#if defined( _WIN32 )
static const char* const unwritableDirectoryName = "/..";
#define MKDIR(name,unused) mkdir(name)
#else
static const char* const unwritableDirectoryName = "/dev";
#define MKDIR mkdir
#endif

static const char* remoteUser = 0; // login name on atmos1, if needed.

// Set path to curl: up to RSIG3D/bin/$platform

#if defined( __APPLE__ ) && defined( __x86_64__ )
static const char* const curlPath = "../../../../bin/Darwin.x86_64/RSIG3D.app/Contents/MacOS";
#elif defined( __APPLE__ ) && defined (__arm64__)
static const char* const curlPath = "../../../../bin/Darwin.arm64/RSIG3D.app/Contents/MacOS";
#elif defined( __linux__ ) && defined( __x86_64__ )
static const char* const curlPath = "../../../../bin/Linux.x86_64";
#elif defined( _WIN64 )
/* static const char* const curlPath = "../../../../bin/Win64"; */
static const char* const curlPath = "..\\..\\..\\..\\bin\\Win64";
#elif defined( _WIN32 )
/* static const char* const curlPath = "../../../../bin/Win32"; */
static const char* const curlPath = "..\\..\\..\\..\\bin\\Win32";
#else
#error "Unsupported platform"
#endif

static const char* const curlCommand =
  "curl -k --silent --retry 0 -L --tcp-nodelay --max-time";

// Point:

static const char* const invalidPointTestFileName =
  "./test/invalid_point_test_file.xdr";

static const char* const invalidRegriddedPointTestFileName =
  "./test/invalid_regridded_point_test_file.xdr";

static const char* const pointFiles[] = {
  "./test/master_goesbb_pm25_2018-11-09_2018-11-10.xdr",
 };

static const size_t pointFileCount = sizeof pointFiles / sizeof *pointFiles;

static const char* const regriddedPointFiles[] = {
  "./test/master_regridded_goesbb_pm25_2018-11-09_2018-11-10.xdr"
};

static const size_t regriddedPointFileCount =
  sizeof regriddedPointFiles / sizeof *regriddedPointFiles;


// Site:

static const char* const invalidSiteTestFileName =
  "./test/invalid_site_test_file.xdr";

static const char* const invalidRegriddedSiteTestFileName =
  "./test/invalid_regridded_site_test_file.xdr";

static const char* const siteFiles[] = {
  "./test/master_airnow2_pm25_2011-03-23_to_2011-03-24.xdr",
  //"./test/master_airnow_pm25_2005-08-26_to_2005-08-27.xdr",
  "./test/master_aqs_ozone_2001-08-26.xdr",
  "./test/master_faqsd_ozone_daily_8hour_maximum_2002-08-28.xdr",
  "./test/master_nesdis_pm25_2007-08-26_to_2007-08-27.xdr",
  "./test/master_uvnet_irradiance_1996-12-29_to_1997-01-04.xdr",
  "./test/master_metar_wind_2016-02-24.xdr"
};

static const size_t siteFileCount = sizeof siteFiles / sizeof *siteFiles;

static const char* const regriddedSiteFiles[] = {
  "./test/master_regridded_airnow2_pm25_2011-03-23_to_2011-03-24.xdr",
  "./test/master_regridded_airnow_pm25_2005-08-26_to_2005-08-27.xdr",
  "./test/master_regridded_aqs_ozone_2001-08-26.xdr",
  "./test/master_regridded_faqsd_ozone_daily_8hour_maximum_2002-08-28.xdr",
  "./test/master_regridded_nesdis_pm25_2007-08-26_to_2007-08-27.xdr",
  "./test/master_regridded_uvnet_irradiance_1996-12-01_to_1997-01-31.xdr",
  "./test/master_regridded_metar_wind_2016-02-24.xdr"
};

static const size_t regriddedSiteFileCount =
  sizeof regriddedSiteFiles / sizeof *regriddedSiteFiles;

// CMAQ:

static const char* const validCMAQFileName = "./test/CONC.ncf";
static const char* const invalidCMAQFileName = "./test/invalid_CONC.ncf";
static const char* const invalidCMAQTestFileName =
  "./test/invalid_cmaq_test_file.xdr";

static const char* const cmaqFiles[] = {
  "./test/master_cmaq_cdc_east_conc_o3_2006-08-28.xdr",
  //"./test/master_cmaq_aqmeii_conus_aod_2006-08-28_to_2006-08-31.xdr",
  "./test/master_cmaq_amad_hemi_ext_2006-08-28.xdr",
  "./test/master_cmaq_amad_conus_o3_daily_max8_2002-04-23_to_2002-04-24.xdr"
};

static const size_t cmaqFileCount = sizeof cmaqFiles / sizeof *cmaqFiles;

// GASP:

static const char* const invalidGASPTestFileName =
  "./test/invalid_gasp_test_file.xdr";

static const char* const invalidRegriddedGASPTestFileName =
  "./test/invalid_regridded_gasp_test_file.xdr";

static const char* const gaspFiles[] = {
  "./test/master_gasp_aod_2008-06-21.xdr"
};

static const size_t gaspFileCount = sizeof gaspFiles / sizeof *gaspFiles;

static const char* const regriddedGASPFiles[] = {
  "./test/master_regridded_gasp_aod_2008-06-21.xdr"
};

static const size_t regriddedGASPFileCount =
  sizeof regriddedGASPFiles / sizeof *regriddedGASPFiles;

// MODIS:

static const char* const invalidMODISTestFileName =
  "./test/invalid_modis_test_file.xdr";

static const char* const invalidRegriddedMODISTestFileName =
  "./test/invalid_regridded_modis_test_file.xdr";

static const char* const modisFiles[] = {
  "./test/master_modis_mod4_optical_depth_land_and_ocean_2005-08-28.xdr"
};

static const size_t modisFileCount = sizeof modisFiles / sizeof *modisFiles;

static const char* const regriddedMODISFiles[] = {
  "./test/master_regridded_modis_mod4_optical_depth_land_and_ocean_"
  "2005-08-28.xdr"
};

static const size_t regriddedMODISFileCount =
  sizeof regriddedMODISFiles / sizeof *regriddedMODISFiles;

// VIIRS:

static const char* const invalidVIIRSTestFileName =
  "./test/invalid_viirs_test_file.xdr";

static const char* const invalidRegriddedVIIRSTestFileName =
  "./test/invalid_regridded_viirs_test_file.xdr";

static const char* const viirsFiles[] = {
  "./test/master_viirs_ivaot_faot550_2013-06-15.xdr"
};

static const size_t viirsFileCount = sizeof viirsFiles / sizeof *viirsFiles;

static const char* const regriddedVIIRSFiles[] = {
  "./test/master_regridded_viirs_ivaot_faot550_2013-06-15.xdr"
};

static const size_t regriddedVIIRSFileCount =
  sizeof regriddedVIIRSFiles / sizeof *regriddedVIIRSFiles;

// Aircraft:

static const char* const invalidAircraftTestFileName =
  "./test/invalid_aircraft_test_file.xdr";

static const char* const invalidRegriddedAircraftTestFileName =
  "./test/invalid_regridded_aircraft_test_file.xdr";

static const char* const aircraftFiles[] = {
  "./test/master_mozaic_profile_ozone_2006-07-03.xdr",
  "./test/master_mozaic_wind_2007-07-03.xdr"
};

static const size_t aircraftFileCount =
  sizeof aircraftFiles / sizeof *aircraftFiles;

static const char* const regriddedAircraftFiles[] = {
  "./test/master_regridded_mozaic_profile_ozone_2006-07-03.xdr",
  "./test/master_regridded_mozaic_wind_2007-07-03.xdr"
};

static const size_t regriddedAircraftFileCount =
  sizeof regriddedAircraftFiles / sizeof *regriddedAircraftFiles;


// Profile:

static const char* const invalidProfileTestFileName =
  "./test/invalid_profile_test_file.xdr";

static const char* const invalidRegriddedProfileTestFileName =
  "./test/invalid_regridded_profile_test_file.xdr";

static const char* const profileFiles[] = {
  "./test/master_neubrew_ozone_2010-07-10.xdr"
};

static const size_t profileFileCount =
  sizeof profileFiles / sizeof *profileFiles;

static const char* const regriddedProfileFiles[] = {
  "./test/master_regridded_neubrew_ozone_2010-07-10.xdr"
};

static const size_t regriddedProfileFileCount =
  sizeof regriddedProfileFiles / sizeof *regriddedProfileFiles;


// CALIPSO:

static const char* const invalidCALIPSOTestFileName =
  "./test/invalid_calipso_test_file.xdr";

static const char* const invalidRegriddedCALIPSOTestFileName =
  "./test/invalid_regridded_calipso_test_file.xdr";

static const char* const calipsoFiles[] = {
  "./test/master_calipso_l2_05kmalay_feature_optical_depth_532_2006-07-05.xdr",
  "./test/master_calipso_l2_05kmapro_extinction_coefficient_532_2006-07-05.xdr"
};

static const size_t calipsoFileCount =
  sizeof calipsoFiles / sizeof *calipsoFiles;

static const char* const regriddedCALIPSOFiles[] = {
  "./test/master_regridded_calipso_l2_05kmalay.feature_optical_depth_532"
    "_2014-07-05.xdr",
  "./test/master_regridded_calipso_l2_05kmapro_extinction_coefficient_532"
    "_2014-07-05.xdr"
};

static const size_t regriddedCALIPSOFileCount =
  sizeof regriddedCALIPSOFiles / sizeof *regriddedCALIPSOFiles;


// Grid:

static const char* const invalidGridTestFileName =
  "./test/invalid_hrrr_wind_test_file.xdr";

static const char* const invalidRegriddedGridTestFileName =
  "./test/invalid_regridded_hrrr_wind_test_file.xdr";

static const char* const gridFiles[] = {
  "./test/master_hrrr_wind_2020-02-17.xdr",
  "./test/master_hrrr_wind_2020-02-17_to_2020-02-18.xdr"
};

static const size_t gridFileCount =
  sizeof gridFiles / sizeof *gridFiles;

static const char* const regriddedGridFiles[] = {
  "./test/master_regridded_hrrr_wind_2020-02-17.xdr"
};

static const size_t regriddedGridFileCount =
  sizeof regriddedGridFiles / sizeof *regriddedGridFiles;


// Polygon:

static const char* const invalidPolygonTestFileName =
  "./test/invalid_polygon_test_file.xdr";

static const char* const polygonFiles[] = {
  "./test/master_hms_smoke_2018-07-01_to_2018-07-02.xdr",
 };

static const size_t polygonFileCount =
  sizeof polygonFiles / sizeof *polygonFiles;


// Define some integer and real test values:

static const long long ivalues[] = {
  LLONG_MIN, LLONG_MIN + 1, LLONG_MIN + 2,
  LONG_MIN, LONG_MIN + 1, LONG_MIN + 2,
  INT_MIN, INT_MIN + 1, INT_MIN + 2,
  SHRT_MIN, SHRT_MIN + 1, SHRT_MIN + 2,
  CHAR_MIN, CHAR_MIN + 1, CHAR_MIN + 2,
  -100, -10, -1, 0, 1, 10, 100,
  CHAR_MAX - 1, CHAR_MAX,
  SHRT_MAX - 1, SHRT_MAX,
  INT_MAX - 1, INT_MAX,
  LONG_MAX - 1, LONG_MAX,
  LLONG_MAX - 2, LLONG_MAX - 1, LLONG_MAX,
  -UCHAR_MAX - 1, -UCHAR_MAX, -UCHAR_MAX + 1,
  -USHRT_MAX - 1, -USHRT_MAX, -USHRT_MAX + 1,
  -UINT_MAX - 1, -UINT_MAX, -UINT_MAX + 1,
  (const long long) ( -ULONG_MAX - 1 ),
  (const long long) -ULONG_MAX,
  (const long long) ( -ULONG_MAX + 1 ),
  (const long long) ( -ULLONG_MAX - 1 ),
  (const long long) -ULLONG_MAX,
  (const long long) ( -ULLONG_MAX + 1 ),
  UCHAR_MAX - 1, UCHAR_MAX, UCHAR_MAX + 1,
  USHRT_MAX - 1, USHRT_MAX, USHRT_MAX + 1,
  UINT_MAX - 1, UINT_MAX, UINT_MAX + 1,
  (const long long) ( ULONG_MAX - 1 ),
  (const long long) ULONG_MAX,
  (const long long) ( ULONG_MAX + 1 ),
  (const long long) ( ULLONG_MAX - 1 ),
  (const long long) ULLONG_MAX,
  (const long long) ( ULLONG_MAX + 1 ),
  -0xaa, 0xaa,
  -0xaaaa, 0xaaaa,
  -0xaaaaaaaa, 0xaaaaaaaa,
  -0xaaaaaaaaaaaaaaaaLL, (const long long) 0xaaaaaaaaaaaaaaaaLL,
  -0x55, 0x55,
  -0x5555, 0x5555,
  -0x55555555, 0x55555555,
  -0x5555555555555555LL, 0x5555555555555555LL,
  -0xfedcba9876543210LL, (const long long) 0xfedcba9876543210LL
};
static const size_t icount = sizeof ivalues / sizeof *ivalues;
static const double rvalues[] = {
  -2.0 * DBL_MAX, -DBL_MAX, -DBL_MAX * 0.5,
  -2.0 * FLT_MAX, -FLT_MAX, -FLT_MAX * 0.5,
  -1e308, -1e10,
  -180.0, -180.0 - 1e-20, -180.0 + 1e-20, -179.0,
  -90.0, -90.0 - 1e-20, -90.0 + 1e-20, -89.0,
  -45.0, -40.0, -30.0, -10.0, -2.0,
  -1.0,
  -0.1, -1e-2, -1e-6, -1e-10, -1e-16, -1e-30, -1e-36, -1e-300, -1e-309,
  -1.234567890123456789e-300,
  -1.234567890123456789e-36,
  -1.234567890123456789e-16,
  -1.234567890123456789e-10,
  -1.234567890123456789e-8,
  -1.234567890123456789e-6,
  -1.234567890123456789e-4,
  -1.234567890123456789e-2,
  -1.234567890123456789,
  -9.876543210987654321e-300,
  -9.876543210987654321e-36,
  -9.876543210987654321e-16,
  -9.876543210987654321e-10,
  -9.876543210987654321e-8,
  -9.876543210987654321e-6,
  -9.876543210987654321e-4,
  -9.876543210987654321e-2,
  -9.876543210987654321,
  -1.0 - DBL_MIN, -1.0 - FLT_MIN, -1.0 + DBL_MIN, -1.0 + FLT_MIN,
  -DBL_MIN * 2.0, -DBL_MIN - FLT_MIN, -DBL_MIN, -DBL_MIN + FLT_MIN,
  -DBL_MIN * 0.5, -DBL_MIN * 0.1,
  -FLT_MIN * 2.0, -FLT_MIN, -FLT_MIN * 0.5, -FLT_MIN * 0.1,
  0.0,
  2.0 * DBL_MAX, DBL_MAX, DBL_MAX * 0.5,
  2.0 * FLT_MAX, FLT_MAX, FLT_MAX * 0.5,
  1e308, 1e10,
  180.0, 180.0 + 1e-20, 180.0 - 1e-20,
  90.0, 90.0 + 1e-20, 90.0 - 1e-20, 89.0,
  75.0,
  45.0, 45.0 + 1e-20, 45.0 - 1e-20,
  40.0, 30.0, 10.0, 2.0, 1.0,
  0.1, 1e-2, 1e-6, 1e-10, 1e-16, 1e-30, 1e-36, 1e-300, 1e-309,
  1.234567890123456789e-300,
  1.234567890123456789e-36,
  1.234567890123456789e-16,
  1.234567890123456789e-10,
  1.234567890123456789e-8,
  1.234567890123456789e-6,
  1.234567890123456789e-4,
  1.234567890123456789e-2,
  1.234567890123456789,
  9.876543210987654321e-300,
  9.876543210987654321e-36,
  9.876543210987654321e-16,
  9.876543210987654321e-10,
  9.876543210987654321e-8,
  9.876543210987654321e-6,
  9.876543210987654321e-4,
  9.876543210987654321e-2,
  9.876543210987654321,
  6370000.0, -6370000.0,
  6378137.0, -6378137.0,
  6356752.3, -6356752.3,
  1.0 - DBL_MIN, 1.0 - FLT_MIN, 1.0 + DBL_MIN, 1.0 + FLT_MIN,
  DBL_MIN * 2.0, DBL_MIN - FLT_MIN, DBL_MIN, DBL_MIN + FLT_MIN,
  DBL_MIN * 0.5, DBL_MIN * 0.1,
  FLT_MIN * 2.0, FLT_MIN, FLT_MIN * 0.5, FLT_MIN * 0.1
};
static const size_t rcount = sizeof rvalues / sizeof *rvalues;

const double lons[] = {
  -180.0, -180.0 + 1e-20, -180.0 + 1e-8, -180.0 + 1e-6, -180.0 + 1e-4,
  -180.0 + 1e-3,
  -179.0,
  -90.0, -90.0 - 1e-20, -90.0 - 1e-8, -90.0 - 1e-6, -90.0 - 1e-4, -90.0 - 1e-3,
  -89.0, -90.0 + 1e-20, -90.0 + 1e-8, -90.0 + 1e-6, -90.0 + 1e-4, -90.0 + 1e-3,
  -45.0,
  -1.0, -1e-20, -1e-8, -1e-6, -1e-4, -1e-3,
  0.0, 0.0, 0.0, 0.0,
  1e-20, 1e-8, 1e-6, 1e-4, 1e-3,
  1.0,
  45.0,
  89.0, 90.0 + 1e-20, 90.0 + 1e-8, 90.0 + 1e-6, 90.0 + 1e-4, 90.0 + 1e-3,
  90.0, 90.0 - 1e-20, 90.0 - 1e-8, 90.0 - 1e-6, 90.0 - 1e-4, 90.0 - 1e-3,
  179.0,
  180.0 - 1e-3,
  180.0, 180.0 - 1e-20, 180.0 - 1e-8, 180.0 - 1e-6, 180.0 - 1e-4
};
const size_t lonsCount = sizeof lons / sizeof *lons;

const double lats[] = {
  -90.0, -90.0 + 1e-20, -90.0 + 1e-8, -90.0 + 1e-6, -90.0 + 1e-3,
  -89.0,
  -45.0, -45.0 + 1e-20, -45.0 + 1e-8, -45.0 + 1e-6, -45.0 + 1e-3,
  -45.0, -45.0 - 1e-20, -45.0 - 1e-8, -45.0 - 1e-6, -45.0 - 1e-3,
  -1.0, -1e-20, -1e-8, -1e-6, -1e-4, -1e-3,
  0.0, 0.0, 0.0, 0.0,
  1e-20, 1e-8, 1e-6, 1e-4, 1e-3,
  1.0,
  45.0, 45.0 + 1e-20, 45.0 + 1e-8, 45.0 + 1e-6, 45.0 + 1e-3,
  45.0, 45.0 - 1e-20, 45.0 - 1e-8, 45.0 - 1e-6, 45.0 - 1e-3,
  89.0,
  90.0, 90.0 - 1e-20, 90.0 - 1e-8, 90.0 - 1e-6, 90.0 - 1e-3
};
const size_t latsCount = sizeof lats / sizeof *lats;

static const char* const svalues0[] = {
  0, "", " ", " a", "a ", " word", "word ",
  "\t", "\n", "\r", "\ta", "a\t", "\na", "a\n", "\ra", "a\r"
};
static const size_t scount0 = sizeof svalues0 / sizeof *svalues0;
static const char* const svalues1[] = {
 "a", "word", "-01234567890=.+", "abcdefghijklmnopqrstuvwxyz", "ABC...XYZ"
};
static const size_t scount1 = sizeof svalues1 / sizeof *svalues1;

//=========================== FORWARD DECLARATIONS ============================

static void testResourceHandler();
static void testDataUtilities();
static void isNanTests();
static void isInfinityTests();
static void isMinusInfinityTests();
static void isFiniteTests();
static void aboutEqualTests();
static void isSignedCharTests();
static void isUnsignedCharTests();
static void isSignedShortTests();
static void isUnsignedShortTests();
static void isSignedIntTests();
static void isUnsignedIntTests();
static void isSignedLongTests();
static void isUnsignedLongTests();
static void isUnsignedLongLongTests();
static void isSizetTests();
static void toIntegerTests();
static void toRealTests();
static void isLongitudeTests();
static void isLatitudeTests();
static void isLongitudeLatitudeTests();
static void radiansTests();
static void degreesTests();
static void reverse2ByteWordsIfLittleEndianTests();
static void reverse4ByteWordsIfLittleEndianTests();
static void reverse8ByteWordsIfLittleEndianTests();
static void expand4ByteWordsTests();
static void isNanFreeTests();
static void allFiniteTests();
static void clampedToRangeTests();

static void allZeroTests();
template <typename T>
static void allZeroTestsT( const T values[], const size_t count );

static void allNonZeroTests();
template <typename T>
static void allNonZeroTestsT( const T values[], const size_t count );

static void isSortedTests();
template <typename T>
static void isSortedTestsT( const T values[], const size_t count );

static void hasTests();
template <typename T>
static void hasTestsT( const T values[], const size_t count );

static void minimumTests();
template <typename T>
static void minimumTestsT( const T values[], const size_t count );

static void maximumTests();
template <typename T>
static void maximumTestsT( const T values[], const size_t count );

static void rangeTests();
template <typename T>
static void rangeTestsT( const T values[], const size_t count );

static void sumTests();
template <typename T>
static void sumTestsT( const T values[], const size_t count );

static void accumulateTests();
template <typename T>
static void accumulateTestsT( const T values[], T values2[],
                              const size_t count );

static void scaledMaximumTests();
template <typename T>
static void scaledMaximumTestsT( const T values[], T values2[],
                                 const size_t count );

static void swapRowsTests();
template <typename T>
static void swapRowsTestsT( const T values[], T values2[],
                            const size_t rows, const size_t columns );

static void expandBytesTests();
static void isValidArgsTests();
static void indexOfStringTests();
static void wordCountTests();
static void lineCountTests();
static void lowercaseTests();
static void eraseDoubleQuotedCommasTests();
static void substituteWordTests();
static void fileSizeTests();
static void fileExistsTests();
static void copyFileTests();
static void copyFileBytesTests();
static void streamFileTests();
static void removeAllFilesTests();
static void removeDirectoryTests();
static void directoryListingTests();

static void testBounds();
static void testTimestamp();
static void testMapProjection();
static void testLambert();
static void testMercator();
static void testAlbers();
static void testStereographic();
static void testHTTPConnection();
static void testDataFilterParameters();
static void testCMAQGridParameters();
static void printCMAQGridParameters( const emvl::CMAQGridParameters& grid );
static void testQueryParameters();
static void testDataset();
static void testDataset( const emvl::Dataset& dataset, const bool writeKML );
static void testPointDataset();
static void testRegriddedPointDataset();
static void testSiteDataset();
static void testRegriddedSiteDataset();
static void testCMAQDataset();
static void testSwathDataset();
static void testRegriddedSwathDataset();
static void testAircraftDataset();
static void testRegriddedAircraftDataset();
static void testProfileDataset();
static void testRegriddedProfileDataset();
static void testCALIPSODataset();
static void testRegriddedCALIPSODataset();
static void testGridDataset();
static void testRegriddedGridDataset();
static void testPolygonDataset();
static void testDatasetManager();
static void testCMAQSubset();

static void printURL( const char* const url ) { printf( "%s\n", url ); }

static void finishKMLFile( FILE* file );
static void createKMLFileContent( emvl::ResourceHandler<FILE*>& fileCloser );

//============================= PUBLIC FUNCTIONS ==============================



/******************************************************************************
PURPOSE: main - Main routine.
INPUTS:  int argc            Number of arguments.
         char* argv[ argc ]  Command line arguments.
RETURNS: int 0.
******************************************************************************/

int main( int argc, char* argv[] ) {

  if ( AND4( argc > 1, argv, argv[ 1 ], argv[ 1 ][ 0 ] ) ) {
    remoteUser = argv[ 1 ];
  }

  if ( ! emvl::isDirectory( testDirectoryName ) ) {
    MKDIR( testDirectoryName, 0777 );
  } else {
    emvl::removeAllFiles( testDirectoryName );
  }

  printf( "calling emvl::DatasetManager::checkAndSetServerHost( curlPath = '%s')\n",
           curlPath );
  emvl::DatasetManager::checkAndSetServerHost( curlPath ); //x
  printf("emvl::DatasetManager::remoteVersion() = %d\n",
          emvl::DatasetManager::remoteVersion() );
  printf( "curl path = '%s'\n",
          emvl::DatasetManager::getCurlPath().c_str() );

  testResourceHandler();
  testDataUtilities();
  testBounds();
  testTimestamp();
  testMapProjection();
  testLambert();
  testMercator();
  testAlbers();
  testStereographic();
  testHTTPConnection();
  testDataFilterParameters();
  testCMAQGridParameters();
  testQueryParameters();
  testDataset();

  typedef struct {
    const char* const datasetType;
    void (*testRoutine)();
  } Entry;
  const Entry table[] = {
    { "Point",             testPointDataset             },
    { "Site",              testSiteDataset              },
    { "CMAQ",              testCMAQDataset              },
    { "Swath",             testSwathDataset             },
    { "Aircraft",          testAircraftDataset          },
    { "Profile",           testProfileDataset           },
    { "CALIPSO",           testCALIPSODataset           },
    { "Grid",              testGridDataset              },
    { "Polygon",           testPolygonDataset           },
    { "RegriddedPoint",    testRegriddedPointDataset    },
    { "RegriddedSite",     testRegriddedSiteDataset     },
    { "RegriddedSwath",    testRegriddedSwathDataset    },
    { "RegriddedAircraft", testRegriddedAircraftDataset },
    { "RegriddedProfile",  testRegriddedProfileDataset  },
    { "RegriddedCALIPSO",  testRegriddedCALIPSODataset  },
    { "RegriddedGrid",     testRegriddedGridDataset     },
    { 0, 0 } // End of table.
  };
  const size_t count = sizeof table / sizeof *table - 1;
  std::string outputKMLFileName;
  outputKMLFileName.reserve( 80 ); //x

  for ( size_t index = 0; index < count; ++index ) {
    const Entry* const entry = table + index;
    outputKMLFileName = testDirectoryName;
    outputKMLFileName += "/test_";
    outputKMLFileName += entry->datasetType;
    outputKMLFileName += ".kml";

    // Create outputKMLFile before appends by testDataset( dataset, true ):

    emvl::ResourceHandler<FILE*> fileCloser( fopen( outputKMLFileName.c_str(),
                                                    "wb" ),
                                             finishKMLFile );
    createKMLFileContent( fileCloser ); //x
    entry->testRoutine();
  } // outputKMLFile is flushed and closed.


  testDatasetManager(); // Tests retrievals from rsigserver.

  printf( "\nPASSED\n" );
  return 0;
}



//============================= PRIVATE FUNCTIONS =============================



/******************************************************************************
PURPOSE: finishKMLFile - Write final content of test output KML file and close
         the file.
INPUTS:  FILE* outputKMLFile  File to write final content to.
OUTPUTS: FILE* outputKMLFile  Closed.
******************************************************************************/

static void finishKMLFile( FILE* ) {

  if ( outputKMLFile ) {
    writeEndKML( outputKMLFile );
    fclose( outputKMLFile );
    outputKMLFile = 0;
  }
}



/******************************************************************************
PURPOSE: createKMLFileContent - Write initial content of test output KML file
         including colors, overall bounds, map images and subset map lines.
INPUTS:  emvl::ResourceHandler<FILE*>& fileCloser  KML file closer.
******************************************************************************/

static void createKMLFileContent( emvl::ResourceHandler<FILE*>& fileCloser ) {
  outputKMLFile = fileCloser.get();

  if ( ! outputKMLFile ) {
    throw std::runtime_error( "Failed to open KML file for writing." ); //x
  }

  const Bounds bounds = { { -126.0, -66.0 }, { 24.0, 50.0 } };
  writeStartKML( outputKMLFile, "Test", "Test Data", bounds ); //x
  writeMapImagesToKML( outputKMLFile, "test",
                       "modisTrueColor", ".png", testDirectoryName, bounds );

  typedef struct {
    const char* const name; // E.g., counties.
    const Color color;
  } Entry;
  const Entry table[] = {
    { "world",    { 0.85, 0.85, 0.85 } },
    { "na",       { 0.65, 0.65, 0.65 } },
    { "states",   { 0.45, 0.45, 0.45 } },
    { "counties", { 0.35, 0.35, 0.35 } }
  };
  const size_t count = sizeof table / sizeof *table;
  bool ok = true;

  for ( size_t index = 0; ok && index < count; ++index ) {
    const Entry* const entry = table + index;
    std::string mapFileName(std::string("test/map_") + entry->name + ".bin");//x
    emvl::ResourceHandler<FILE*> mapFileCloser(fopen(mapFileName.c_str(), "rb"),
                                               fclose );
    FILE* const mapFile = mapFileCloser.get();

    if ( ! mapFile ) {
      throw std::runtime_error( "Failed to open map file for reading." ); //x
    }

    int polylineCount = 0;
    int vertexCount = 0;
    ok = readMapFileHeader( mapFile, &polylineCount, &vertexCount ) == 1;

    if ( ok ) {
      std::vector<int> countsStorage( polylineCount ); //x
      std::vector<float> verticesStorage( vertexCount * 2 ); //x
      int* const counts = &countsStorage[ 0 ];
      float* const vertices = &verticesStorage[ 0 ];

      ok = readMapFileData( mapFile, polylineCount, vertexCount,
                            counts, vertices ) == 1;

      if ( ok ) {
        std::vector<int> subsetCountsStorage( polylineCount ); //x
        std::vector<float> subsetVerticesStorage( vertexCount * 2 ); //x
        int* const subsetCounts = &subsetCountsStorage[ 0 ];
        float* const subsetVertices = &subsetVerticesStorage[ 0 ];
        int subsetPolylineCount = 0;
        int subsetVertexCount = 0;

        subsetMapFloat( polylineCount, vertexCount, counts, vertices,
                        0.0, bounds,
                        &subsetPolylineCount, &subsetVertexCount,
                        subsetCounts, subsetVertices );

        ok = subsetPolylineCount > 0;

        if ( ok ) {
          writeMapPolylinesToKML( outputKMLFile,
                                  subsetPolylineCount,
                                  subsetCounts, subsetVertices,
                                  entry->name, entry->color );
        }
      }
    }
  }

  fflush( outputKMLFile ); // Ensure content is written before appends.

  if ( ! ok ) {
    throw std::runtime_error( "Failed to write initial KML file content." ); //x
  }

}



/******************************************************************************
PURPOSE: testResourceHandler - Test ResourceHandler class.
******************************************************************************/

static void testResourceHandler() {
  printf( "testResourceHandler()\n" );

  {
    const emvl::ResourceHandler<FILE*> fileCloser( 0, fclose );
    assert( fileCloser.get() == 0 );
  }

  {
    emvl::ResourceHandler<FILE*> fileCloser( fopen(tempFileName, "w"), fclose);
    FILE* file = fileCloser.get();
    assert ( file );
    fileCloser.handler2( tempFileName, unlink ); // Remove file in destructor.
    fprintf( file, "Testing ~ResourceHandler.\n" );
  }

  assert( ! emvl::fileExists( tempFileName ) ); // Temp file removed.

  {
    char command[ 256 ] = "";
    memset( command, 0, sizeof command );
    snprintf( command, sizeof command / sizeof *command,
             "%s%ccurl --version", curlPath, emvl::slash );
    printf( "command = %s\n", command );
    const emvl::ResourceHandler<FILE*>
      fileCloser( popen( command, "r" ), pclose );
    FILE* pipe = fileCloser.get();
    assert ( pipe );
    char line[ 1024 ] = "";
    memset( line, 0, sizeof line );
    assert( fgets( line, sizeof line / sizeof *line, pipe ) );
    printf( "%s\n", line );

    while ( fgets( line, sizeof line / sizeof *line, pipe ) ) {
      printf( "%s\n", line );
    }
  }
}



/******************************************************************************
PURPOSE: testDataUtilities - Test DataUtilities routines.
******************************************************************************/

static void testDataUtilities() {
  printf( "testDataUtilities()\n" );
  std::cout << "emvl::platform = " << emvl::platform << std::endl;
  std::cout << "emvl::slash = " << emvl::slash << std::endl;
  isNanTests();
  isInfinityTests();
  isMinusInfinityTests();
  isFiniteTests();
  aboutEqualTests();
  isSignedCharTests();
  isUnsignedCharTests();
  isSignedShortTests();
  isUnsignedShortTests();
  isSignedIntTests();
  isUnsignedIntTests();
  isSignedLongTests();
  isUnsignedLongTests();
  isUnsignedLongLongTests();
  isSizetTests();
  toIntegerTests();
  toRealTests();
  isLongitudeTests();
  isLatitudeTests();
  isLongitudeLatitudeTests();
  radiansTests();
  degreesTests();
  reverse2ByteWordsIfLittleEndianTests();
  reverse4ByteWordsIfLittleEndianTests();
  reverse8ByteWordsIfLittleEndianTests();
  expand4ByteWordsTests();
  isNanFreeTests();
  allFiniteTests();
  clampedToRangeTests();
  allZeroTests();
  allNonZeroTests();
  isSortedTests();
  hasTests();
  minimumTests();
  maximumTests();
  rangeTests();
  sumTests();
  accumulateTests();
  scaledMaximumTests();
  swapRowsTests();
  expandBytesTests();
  isValidArgsTests();
  indexOfStringTests();
  wordCountTests();
  lineCountTests();
  lowercaseTests();
  eraseDoubleQuotedCommasTests();
  substituteWordTests();
  fileExistsTests();
  fileSizeTests();
  copyFileTests();
  copyFileBytesTests();
  streamFileTests();
  removeAllFilesTests();
  removeDirectoryTests();
  directoryListingTests();
}



/******************************************************************************
PURPOSE: isNanTests - Tests of isNan routine.
******************************************************************************/

static void isNanTests() {
  assert( ! emvl::isNan( 0.0 ) );
  assert( ! emvl::isNan( 1.0 ) );
  assert( ! emvl::isNan( -1.0 ) );
  assert( ! emvl::isNan( -DBL_MAX ) );
  assert( ! emvl::isNan( DBL_MAX ) );
  const double infinity      =  2.0 * DBL_MAX;
  const double minusInfinity = -2.0 * DBL_MAX;
  assert( ! emvl::isNan( -2.0 * DBL_MAX ) );
  assert( ! emvl::isNan( 2.0 * DBL_MAX ) );
  assert( ! emvl::isNan( 1.0 / DBL_MAX ) );
  assert( ! emvl::isNan( -1.0 / DBL_MAX ) );
  assert( emvl::isNan( infinity + minusInfinity ) );
  assert( emvl::isNan( infinity / minusInfinity ) );
  assert( emvl::isNan( infinity * ( 1.0 / minusInfinity ) ) );
  assert( emvl::isNan( minusInfinity * ( 1.0 / infinity ) ) );

  for ( size_t index = 0; index < rcount; ++index ) {
    const double value = rvalues[ index ];
    const bool result = emvl::isNan( value );
    assert( result == ( value != rvalues[ index ] ) );
  }
}



/******************************************************************************
PURPOSE: isInfinityTests - Tests of isInfinity routine.
******************************************************************************/

static void isInfinityTests() {
  assert( ! emvl::isInfinity( 0.0 ) );
  assert( ! emvl::isInfinity( 1.0 ) );
  assert( ! emvl::isInfinity( -1.0 ) );
  assert( ! emvl::isInfinity( -DBL_MAX ) );
  assert( ! emvl::isInfinity( DBL_MAX ) );
  assert( ! emvl::isInfinity( -2.0 * DBL_MAX ) );
  assert(   emvl::isInfinity( 2.0 * DBL_MAX ) );
  assert( ! emvl::isInfinity( 1.0 / DBL_MAX ) );
  assert( ! emvl::isInfinity( -1.0 / DBL_MAX ) );
  assert( ! emvl::isInfinity( ( 2.0 * DBL_MAX ) + ( -2.0 * DBL_MAX ) ) );
  assert( ! emvl::isInfinity( ( 2.0 * DBL_MAX ) / ( -2.0 * DBL_MAX ) ) );
  assert( ! emvl::isInfinity( ( 2.0 * DBL_MAX ) * ( 1.0 / ( -2.0 * DBL_MAX))));
  assert( ! emvl::isInfinity( ( -2.0 * DBL_MAX ) * ( 1.0 / ( 2.0 * DBL_MAX))));

  for ( size_t index = 0; index < rcount; ++index ) {
    const double value = rvalues[ index ];
    const bool result = emvl::isInfinity( value );
    assert( result == ( value > DBL_MAX ) );
  }
}



/******************************************************************************
PURPOSE: isMinusInfinityTests - Tests of isMinusInfinity routine.
******************************************************************************/

static void isMinusInfinityTests() {
  assert( ! emvl::isMinusInfinity( 0.0 ) );
  assert( ! emvl::isMinusInfinity( 1.0 ) );
  assert( ! emvl::isMinusInfinity( -1.0 ) );
  assert( ! emvl::isMinusInfinity( -DBL_MAX ) );
  assert( ! emvl::isMinusInfinity( DBL_MAX ) );
  assert(   emvl::isMinusInfinity( -2.0 * DBL_MAX ) );
  assert( ! emvl::isMinusInfinity( 2.0 * DBL_MAX ) );
  assert( ! emvl::isMinusInfinity( 1.0 / DBL_MAX ) );
  assert( ! emvl::isMinusInfinity( -1.0 / DBL_MAX ) );
  assert( ! emvl::isMinusInfinity( ( 2.0 * DBL_MAX ) + ( -2.0 * DBL_MAX ) ) );
  assert( ! emvl::isMinusInfinity( ( 2.0 * DBL_MAX ) / ( -2.0 * DBL_MAX ) ) );
  assert( ! emvl::isMinusInfinity( ( 2.0 * DBL_MAX ) *
                                   ( 1.0 / ( -2.0 * DBL_MAX ) ) ) );
  assert( ! emvl::isMinusInfinity( ( -2.0 * DBL_MAX ) *
                                   ( 1.0 / ( 2.0 * DBL_MAX ) ) ) );

  for ( size_t index = 0; index < rcount; ++index ) {
    const double value = rvalues[ index ];
    const bool result = emvl::isMinusInfinity( value );
    assert( result == ( value < -DBL_MAX ) );
  }
}



/******************************************************************************
PURPOSE: isFiniteTests - Tests of isFinite routine.
******************************************************************************/

static void isFiniteTests() {
  assert( emvl::isFinite( 0.0 ) );
  assert( emvl::isFinite( 1.0 ) );
  assert( emvl::isFinite( -1.0 ) );
  assert( emvl::isFinite( -DBL_MAX ) );
  assert( emvl::isFinite( DBL_MAX ) );
  assert( ! emvl::isFinite( -2.0 * DBL_MAX ) );
  assert( ! emvl::isFinite( 2.0 * DBL_MAX ) );
  assert( emvl::isFinite( 1.0 / DBL_MAX ) );
  assert( emvl::isFinite( -1.0 / DBL_MAX ) );
  const double minusInfinity = -2.0 * DBL_MAX;
  const double infinity      =  2.0 * DBL_MAX;
  assert( ! emvl::isFinite( infinity + minusInfinity ) );
  assert( ! emvl::isFinite( infinity / minusInfinity ) );
  assert( ! emvl::isFinite( infinity * ( 1.0 / minusInfinity ) ) );
  assert( ! emvl::isFinite( minusInfinity * ( 1.0 / infinity ) ) ); 

  for ( size_t index = 0; index < rcount; ++index ) {
    const double value = rvalues[ index ];
    const bool result = emvl::isFinite( value );
    assert( result == IN_RANGE( value, -DBL_MAX, DBL_MAX ) );
  }
}



/******************************************************************************
PURPOSE: aboutEqualTests - Tests of aboutEqual routine.
******************************************************************************/

static void aboutEqualTests() {
  assert( emvl::aboutEqual( 0.0, 0.0 ) );
  assert( emvl::aboutEqual( 1.0, 1.0 ) );
  assert( emvl::aboutEqual( -1.0, -1.0 ) );
  assert( emvl::aboutEqual( -DBL_MAX, -DBL_MAX ) );
  assert( emvl::aboutEqual( DBL_MAX, DBL_MAX ) );
  assert( emvl::aboutEqual( -2.0 * DBL_MAX, -2.0 * DBL_MAX ) );
  assert( emvl::aboutEqual( 2.0 * DBL_MAX, 2.0 * DBL_MAX ) );
  assert( emvl::aboutEqual( 1.0 / DBL_MAX, 1.0 / DBL_MAX ) );
  assert( emvl::aboutEqual( -1.0 / DBL_MAX, -1.0 / DBL_MAX ) );

  for ( size_t index1 = 0; index1 < rcount; ++index1 ) {
    const double value1 = rvalues[ index1 ];

    for ( size_t index2 = 0; index2 < rcount; ++index2 ) {
      const double value2 = rvalues[ index2 ];

      for ( size_t index3 = 0; index3 < rcount; ++index3 ) {
        const double value3 = rvalues[ index3 ];

        if ( IN_RANGE( value3, 0.0, 0.1 ) ) {
          const bool result = emvl::aboutEqual( value1, value2, value3 );
          assert( IMPLIES( value1 == value2, result ) );
          assert( IMPLIES( fabs( value1 - value2 ) < value3, result ) );
          ////assert( IMPLIES( fabs( value1 - value2 ) > value3, ! result ) );
        } else {
          const bool result = emvl::aboutEqual( value1, value2 );
          assert( IMPLIES( value1 == value2, result ) );
          assert( IMPLIES( fabs( value1 - value2 ) < 1e-6, result ) );
          assert( IMPLIES( fabs( value1 - value2 ) > 1e-6, ! result ) );
        }
      }
    }
  }
}



/******************************************************************************
PURPOSE: isSignedCharTests - Tests of isSignedChar routine.
******************************************************************************/

static void isSignedCharTests() {
  assert( emvl::isSignedChar( LLONG_MIN )
          != ( sizeof (signed char) < sizeof (long long) ) );
  assert( emvl::isSignedChar( SCHAR_MIN - 1LL )
          != ( sizeof (signed char) < sizeof (long long) ) );
  assert( emvl::isSignedChar( SCHAR_MIN ) );
  assert( emvl::isSignedChar( SCHAR_MIN + 1LL ) );
  assert( emvl::isSignedChar( -1 ) );
  assert( emvl::isSignedChar( 0 ) );
  assert( emvl::isSignedChar( 1 ) );
  assert( emvl::isSignedChar( SCHAR_MAX - 1LL ) );
  assert( emvl::isSignedChar( SCHAR_MAX ) );
  assert( emvl::isSignedChar( SCHAR_MAX + 1LL )
          != ( sizeof (signed char) < sizeof (long long) ) );
  assert( emvl::isSignedChar( LLONG_MAX )
          != ( sizeof (signed char) < sizeof (long long) ) );
}



/******************************************************************************
PURPOSE: isUnsignedCharTests - Tests of isUnsignedChar routine.
******************************************************************************/

static void isUnsignedCharTests() {
  assert( ! emvl::isUnsignedChar( LLONG_MIN ) );
  assert( ! emvl::isUnsignedChar( -1 ) );
  assert( emvl::isUnsignedChar( 0 ) );
  assert( emvl::isUnsignedChar( 1 ) );
  assert( emvl::isUnsignedChar( UCHAR_MAX - 1LL )
          == ( sizeof (unsigned char) < sizeof (long long) ) );
  assert( emvl::isUnsignedChar( UCHAR_MAX )
          == ( sizeof (unsigned char) < sizeof (long long) ) );
  assert( emvl::isUnsignedChar( UCHAR_MAX + 1LL )
          != ( sizeof (unsigned char) < sizeof (long long) ) );
  assert( emvl::isUnsignedChar( LLONG_MAX )
          != ( sizeof (unsigned char) < sizeof (long long) ) );
}



/******************************************************************************
PURPOSE: isSignedShortTests - Tests of isSignedShort routine.
******************************************************************************/

static void isSignedShortTests() {
  assert( emvl::isSignedShort( LLONG_MIN )
          != ( sizeof (short) < sizeof (long long) ) );
  assert( emvl::isSignedShort( SHRT_MIN - 1 )
          != ( sizeof (short) < sizeof (long long) ) );
  assert( emvl::isSignedShort( SHRT_MIN ) );
  assert( emvl::isSignedShort( SHRT_MIN + 1LL ) );
  assert( emvl::isSignedShort( -1 ) );
  assert( emvl::isSignedShort( 0 ) );
  assert( emvl::isSignedShort( 1 ) );
  assert( emvl::isSignedShort( SHRT_MAX - 1LL ) );
  assert( emvl::isSignedShort( SHRT_MAX ) );
  assert( emvl::isSignedShort( SHRT_MAX + 1LL )
          != ( sizeof (short) < sizeof (long long) ) );
  assert( emvl::isSignedShort( LLONG_MAX )
          != ( sizeof (short) < sizeof (long long) ) );
}



/******************************************************************************
PURPOSE: isUnsignedShortTests - Tests of isUnsignedShort routine.
******************************************************************************/

static void isUnsignedShortTests() {
  assert( ! emvl::isUnsignedShort( LLONG_MIN ) );
  assert( ! emvl::isUnsignedShort( -1 ) );
  assert( emvl::isUnsignedShort( 0 ) );
  assert( emvl::isUnsignedShort( 1 ) );
  assert( emvl::isUnsignedShort( USHRT_MAX - 1LL )
          == ( sizeof (unsigned short) < sizeof (long long) ) );
  assert( emvl::isUnsignedShort( USHRT_MAX )
          == ( sizeof (unsigned short) < sizeof (long long) ) );
  assert( emvl::isUnsignedShort( USHRT_MAX + 1LL )
          != ( sizeof (unsigned short) < sizeof (long long) ) );
  assert( emvl::isUnsignedShort( LLONG_MAX )
          != ( sizeof (unsigned short) < sizeof (long long) ) );
}



/******************************************************************************
PURPOSE: isSignedIntTests - Tests of isSignedInt routine.
******************************************************************************/

static void isSignedIntTests() {
  assert( emvl::isSignedInt( LLONG_MIN )
          != ( sizeof (int) < sizeof (long long) ) );
  assert( emvl::isSignedInt( INT_MIN - 1LL )
          != ( sizeof (int) < sizeof (long long) ) );
  assert( emvl::isSignedInt( INT_MIN ) );
  assert( emvl::isSignedInt( INT_MIN + 1LL ) );
  assert( emvl::isSignedInt( -1 ) );
  assert( emvl::isSignedInt( 0 ) );
  assert( emvl::isSignedInt( 1 ) );
  assert( emvl::isSignedInt( INT_MAX - 1LL ) );
  assert( emvl::isSignedInt( INT_MAX ) );
  assert( emvl::isSignedInt( INT_MAX + 1LL )
          != ( sizeof (int) < sizeof (long long) ) );
  assert( emvl::isSignedInt( LLONG_MAX )
          != ( sizeof (int) < sizeof (long long) ) );
}



/******************************************************************************
PURPOSE: isUnsignedIntTests - Tests of isUnsignedInt routine.
******************************************************************************/

static void isUnsignedIntTests() {
  assert( ! emvl::isUnsignedInt( LLONG_MIN ) );
  assert( ! emvl::isUnsignedInt( -1 ) );
  assert( emvl::isUnsignedInt( 0 ) );
  assert( emvl::isUnsignedInt( 1 ) );
  assert( emvl::isUnsignedInt( UINT_MAX - 1LL )
          == ( sizeof (unsigned int) < sizeof (long long) ) );
  assert( emvl::isUnsignedInt( UINT_MAX )
          == ( sizeof (unsigned int) < sizeof (long long) ) );
  assert( emvl::isUnsignedInt( UINT_MAX + 1LL )
          != ( sizeof (unsigned int) < sizeof (long long) ) );
  assert( emvl::isUnsignedInt( LLONG_MAX )
          != ( sizeof (unsigned int) < sizeof (long long) ) );
}



/******************************************************************************
PURPOSE: isSignedLongTests - Tests of isSignedLong routine.
******************************************************************************/

static void isSignedLongTests() {
  assert( emvl::isSignedLong( LLONG_MIN )
          != ( sizeof (long) < sizeof (long long) ) );
  long long overflow = LONG_MIN;
  --overflow;
  assert( emvl::isSignedLong( overflow )
          != ( sizeof (long) < sizeof (long long) ) );
  assert( emvl::isSignedLong( LONG_MIN ) );
  assert( emvl::isSignedLong( LONG_MIN + 1LL ) );
  assert( emvl::isSignedLong( -1 ) );
  assert( emvl::isSignedLong( 0 ) );
  assert( emvl::isSignedLong( 1 ) );
  assert( emvl::isSignedLong( LONG_MAX - 1LL ) );
  assert( emvl::isSignedLong( LONG_MAX ) );
  overflow = LONG_MAX;
  ++overflow;
  assert( emvl::isSignedLong( overflow )
          != ( sizeof (long) < sizeof (long long) ) );
  assert( emvl::isSignedLong( LLONG_MAX )
          != ( sizeof (long) < sizeof (long long) ) );
}



/******************************************************************************
PURPOSE: isUnsignedLongTests - Tests of isUnsignedLong routine.
******************************************************************************/

static void isUnsignedLongTests() {
  assert( ! emvl::isUnsignedLong( LLONG_MIN ) );
  assert( ! emvl::isUnsignedLong( -1 ) );
  assert( emvl::isUnsignedLong( 0 ) );
  assert( emvl::isUnsignedLong( 1 ) );
  assert( emvl::isUnsignedLong( ULONG_MAX - 1LL )
          == ( sizeof (unsigned long) < sizeof (long long) ) );
  assert( emvl::isUnsignedLong( ULONG_MAX )
          == ( sizeof (unsigned long) < sizeof (long long) ) );
  assert( emvl::isUnsignedLong( ULONG_MAX + 1LL )
          != ( sizeof (unsigned long) < sizeof (long long) ) );
  assert( emvl::isUnsignedLong( LLONG_MAX )
          != ( sizeof (unsigned long) < sizeof (long long) ) );
}



/******************************************************************************
PURPOSE: isSizetTests - Tests of isSizet routine.
******************************************************************************/

static void isSizetTests() {
  assert( ! emvl::isSizet( LLONG_MIN ) );
  assert( ! emvl::isSizet( -1 ) );
  assert( emvl::isSizet( 0 ) );
  assert( emvl::isSizet( 1 ) );
  assert( emvl::isSizet( ULONG_MAX - 1LL )
          == ( sizeof (size_t) < sizeof (long long) ) );
  assert( emvl::isSizet( ULONG_MAX )
          == ( sizeof (size_t) < sizeof (long long) ) );
  assert( emvl::isSizet( ULONG_MAX + 1LL )
          != ( sizeof (size_t) < sizeof (long long) ) );
  assert( emvl::isSizet( LLONG_MAX )
          != ( sizeof (size_t) < sizeof (long long) ) );

  for ( size_t index = 0; index < icount; ++index ) {
    long long value = ivalues[ index ];
    const char cvalue = (char) value;
    const unsigned char ucvalue = (unsigned char) value;
    const short svalue = (short) value;
    const unsigned short usvalue = (unsigned short) value;
    const int ivalue = (int) value;
    const unsigned int uivalue = (unsigned int) value;
    const long lvalue = (long) value;
    const unsigned long ulvalue = (unsigned long) value;
    const size_t stvalue = (size_t) value;
    assert( OR10( emvl::isSignedChar( value ),
                  emvl::isUnsignedChar( value ),
                  emvl::isSignedShort( value ),
                  emvl::isUnsignedShort( value ),
                  emvl::isSignedInt( value ),
                  emvl::isUnsignedInt( value ),
                  emvl::isSignedLong( value ),
                  emvl::isUnsignedLong( value ),
                  emvl::isSizet( value ),
                  sizeof (size_t) < sizeof (long long) ) );
    value = (long long) cvalue;
    assert( emvl::isSignedChar( value ) );
    value = (long long) ucvalue;
    assert( emvl::isUnsignedChar( value ) == ( value >= 0 ) );
    value = (long long) svalue;
    assert( emvl::isSignedShort( value ) );
    value = (long long) usvalue;
    assert( emvl::isUnsignedShort( value ) == ( value >= 0 ) );
    value = (long long) ivalue;
    assert( emvl::isSignedInt( value ) );
    value = (long long) uivalue;
    assert( emvl::isUnsignedInt( value ) == ( value >= 0 ) );
    value = (long long) lvalue;
    assert( emvl::isSignedLong( value ) );
    value = (long long) ulvalue;
    assert( emvl::isUnsignedLong( value ) == ( value >= 0 ) );
    value = (long long) stvalue;
    assert( emvl::isSizet( value ) == ( value >= 0 ) );
  }
}



/******************************************************************************
PURPOSE: isUnsignedLongLongTests - Tests of isUnsignedLongLong routine.
******************************************************************************/

static void isUnsignedLongLongTests() {
  assert( ! emvl::isUnsignedLongLong( LLONG_MIN ) );
  assert( ! emvl::isUnsignedLongLong( -1 ) );
  assert( emvl::isUnsignedLongLong( 0 ) );
  assert( emvl::isUnsignedLongLong( 1 ) );
  assert( emvl::isUnsignedLongLong( LLONG_MAX - 1LL ) );
  assert( emvl::isUnsignedLongLong( LLONG_MAX ) );
  long long overflow = LLONG_MAX;
  ++overflow;
  assert( ! emvl::isUnsignedLongLong( overflow ) );
}



/******************************************************************************
PURPOSE: toIntegerTests - Tests of toInteger routine.
******************************************************************************/

static void toIntegerTests() {
  bool ok = false;
  long long result = 0;
  result = emvl::toInteger( "", 0, 0, ok );
  assert( ! ok ); assert( result == 0 );
  result = emvl::toInteger( "a", 0, 0, ok );
  assert( ! ok ); assert( result == 0 );
  result = emvl::toInteger( "-", 0, 0, ok );
  assert( ! ok ); assert( result == 0 );
  result = emvl::toInteger( "+", 0, 0, ok );
  assert( ! ok ); assert( result == 0 );
  result = emvl::toInteger( "-1", 0, 0, ok );
  assert( ! ok ); assert( result == 0 );
  result = emvl::toInteger( "1", 0, 0, ok );
  assert( ! ok ); assert( result == 0 );
  result = emvl::toInteger( "0", 0, 0, ok );
  assert( ok ); assert( result == 0 );
  result = emvl::toInteger( "-1", 0, 1, ok );
  assert( ! ok ); assert( result == 0 );
  result = emvl::toInteger( "0", 0, 1, ok );
  assert( ok ); assert( result == 0 );
  result = emvl::toInteger( "1", 0, 1, ok );
  assert( ok ); assert( result == 1 );
  result = emvl::toInteger( "2", 0, 1, ok );
  assert( ! ok ); assert( result == 0 );
  result = emvl::toInteger( "-2", -1, 1, ok );
  assert( ! ok ); assert( result == 0 );
  result = emvl::toInteger( "-1", -1, 1, ok );
  assert( ok ); assert( result == -1 );
  result = emvl::toInteger( "0", -1, 1, ok );
  assert( ok ); assert( result == 0 );
  result = emvl::toInteger( "1", -1, 1, ok );
  assert( ok ); assert( result == 1 );
  result = emvl::toInteger( "2", -1, 1, ok );
  assert( ! ok ); assert( result == 0 );
  result = emvl::toInteger( " -1 ", -1, 1, ok );
  assert( ok ); assert( result == -1 );
  result = emvl::toInteger( " 0 ", -1, 1, ok );
  assert( ok ); assert( result == 0 );
  result = emvl::toInteger( " 1 ", -1, 1, ok );
  assert( ok ); assert( result == 1 );
  result = emvl::toInteger( " +1 ", -1, 1, ok );
  assert( ok ); assert( result == 1 );
  result = emvl::toInteger( " 1 0", -1, 1, ok );
  assert( ! ok ); assert( result == 0 );
  result = emvl::toInteger( " - 1 ", -1, 1, ok );
  assert( ! ok ); assert( result == 0 );

  for ( size_t index = 0; index < icount; ++index ) {
    const long long value = ivalues[ index ];

    for ( size_t index1 = 0; index1 < icount; ++index1 ) {
      const long long minimum = ivalues[ index1 ];

      for ( size_t index2 = 0; index2 < icount; ++index2 ) {
        const long long maximum = ivalues[ index2 ];

        if ( minimum <= maximum ) {
          char s[ 32 ] = "";
          memset( s, 0, sizeof s );
          snprintf( s, sizeof s / sizeof *s, "%" LLD, value );
          bool ok = false;
          const long long result = emvl::toInteger( s, minimum, maximum, ok );
          assert( IMPLIES_ELSE( ok,
                                AND2( result == value,
                                      IN_RANGE( result, minimum, maximum ) ),
                                result == 0 ) );
        }
      }
    }
  }
}


/******************************************************************************
PURPOSE: toRealTests - Tests of toReal routine.
******************************************************************************/

static void toRealTests() {

  bool ok = false;
  double result = 0;
  result = emvl::toReal( "", 0.0, 0.0, ok );
  assert( ! ok ); assert( result == 0.0 );
  result = emvl::toReal( "a", 0.0, 0.0, ok );
  assert( ! ok ); assert( result == 0.0 );
  result = emvl::toReal( "-", 0.0, 0.0, ok );
  assert( ! ok ); assert( result == 0.0 );
  result = emvl::toReal( "+", 0.0, 0.0, ok );
  assert( ! ok ); assert( result == 0.0 );
  result = emvl::toReal( "-1", 0.0, 0.0, ok );
  assert( ! ok ); assert( result == 0.0 );
  result = emvl::toReal( "1", 0.0, 0.0, ok );
  assert( ! ok ); assert( result == 0.0 );
  result = emvl::toReal( "0", 0.0, 0.0, ok );
  assert( ok ); assert( result == 0.0 );
  result = emvl::toReal( "-1", 0.0, 1.0, ok );
  assert( ! ok ); assert( result == 0.0 );
  result = emvl::toReal( "0", 0.0, 1.0, ok );
  assert( ok ); assert( result == 0.0 );
  result = emvl::toReal( "1", 0.0, 1.0, ok );
  assert( ok ); assert( result == 1 );
  result = emvl::toReal( "2", 0.0, 1.0, ok );
  assert( ! ok ); assert( result == 0.0 );
  result = emvl::toReal( "-2", -1.0, 1.0, ok );
  assert( ! ok ); assert( result == 0.0 );
  result = emvl::toReal( "-1", -1.0, 1.0, ok );
  assert( ok ); assert( emvl::aboutEqual( result, -1.0 ) );
  result = emvl::toReal( "0", -1.0, 1.0, ok );
  assert( ok ); assert( result == 0.0 );
  result = emvl::toReal( "1", -1.0, 1.0, ok );
  assert( ok ); assert( emvl::aboutEqual( result, 1.0 ) );
  result = emvl::toReal( "2", -1.0, 1.0, ok );
  assert( ! ok ); assert( result == 0.0 );
  result = emvl::toReal( " -1 ", -1.0, 1.0, ok );
  assert( ok ); assert( emvl::aboutEqual( result, -1.0 ) );
  result = emvl::toReal( " 0 ", -1.0, 1.0, ok );
  assert( ok ); assert( result == 0.0 );
  result = emvl::toReal( " 1 ", -1.0, 1.0, ok );
  assert( ok ); assert( emvl::aboutEqual( result, 1.0 ) );
  result = emvl::toReal( " +1 ", -1.0, 1.0, ok );
  assert( ok ); assert( emvl::aboutEqual( result, 1.0 ) );
  result = emvl::toReal( " 1 0", -1.0, 1.0, ok );
  assert( ! ok ); assert( result == 0.0 );
  result = emvl::toReal( " - 1 ", -1.0, 1.0, ok );
  assert( ! ok ); assert( result == 0.0 );
  result = emvl::toReal( "-1.0", -1.0, 1.0, ok );
  assert( ok ); assert( emvl::aboutEqual( result, -1.0 ) );
  result = emvl::toReal( "-0.1", -1.0, 1.0, ok );
  assert( ok ); assert( emvl::aboutEqual( result, -0.1 ) );
  result = emvl::toReal( "0.1", -1.0, 1.0, ok );
  assert( ok ); assert( emvl::aboutEqual( result, 0.1 ) );
  result = emvl::toReal( "-1.1", -1.0, 1.0, ok );
  assert( ! ok ); assert( result == 0.0 );
  result = emvl::toReal( "1.1", -1.0, 1.0, ok );
  assert( ! ok ); assert( result == 0.0 );

  for ( size_t index = 0; index < rcount; ++index ) {
    const double value = rvalues[ index ];

    for ( size_t index1 = 0; index1 < rcount; ++index1 ) {
      const double minimum = rvalues[ index1 ];

      for ( size_t index2 = 0; index2 < icount; ++index2 ) {
        const double maximum = ivalues[ index2 ];

        if ( minimum <= maximum ) {
          char s[ 32 ] = "";
          memset( s, 0, sizeof s );
          snprintf( s, sizeof s / sizeof *s, "%g", value );
          bool ok = false;
          const double result = emvl::toReal( s, minimum, maximum, ok );

          assert( IMPLIES_ELSE( ok,
                                IN_RANGE( result, minimum, maximum ),
                                result == 0.0 ) );
        }
      }
    }
  }
}



/******************************************************************************
PURPOSE: isLongitudeTests - Test isLongitude routine.
******************************************************************************/

static void isLongitudeTests() {
  assert( ! emvl::isLongitude( -180.0 - 1e-6 ) );
  assert(   emvl::isLongitude( -180.0 ) );
  assert(   emvl::isLongitude( -180.0 + 1e-6 ) );
  assert(   emvl::isLongitude(  180.0 - 1e-6 ) );
  assert(   emvl::isLongitude(  180.0 ) );
  assert( ! emvl::isLongitude(  180.0 + 1e-6 ) );

  for ( size_t index = 0; index < rcount; ++index ) {
    const double value = rvalues[ index ];

    if ( ! emvl::isNan( value ) ) {
      const bool result = emvl::isLongitude( value );
      assert( result == IN_RANGE( value, -180.0, 180.0 ) );
    }
  }
}



/******************************************************************************
PURPOSE: isLatitudeTests - Test isLatitude routine.
******************************************************************************/

static void isLatitudeTests() {
  assert( ! emvl::isLatitude( -90.0 - 1e-6 ) );
  assert(   emvl::isLatitude( -90.0 ) );
  assert(   emvl::isLatitude( -90.0 + 1e-6 ) );
  assert(   emvl::isLatitude(  90.0 - 1e-6 ) );
  assert(   emvl::isLatitude(  90.0 ) );
  assert( ! emvl::isLatitude(  90.0 + 1e-6 ) );

  for ( size_t index = 0; index < rcount; ++index ) {
    const double value = rvalues[ index ];

    if ( ! emvl::isNan( value ) ) {
      const bool result = emvl::isLatitude( value );
      assert( result == IN_RANGE( value, -90.0, 90.0 ) );
    }
  }
}


/******************************************************************************
PURPOSE: isLongitudeLatitudeTests - Test isLongitudeLatitude routine.
******************************************************************************/

static void isLongitudeLatitudeTests() {
  assert( ! emvl::isLongitudeLatitude( -180.0 - 1e-6, 0.0 ) );
  assert( ! emvl::isLongitudeLatitude( 0.0, -90.0 - 1e-6 ) );
  assert( ! emvl::isLongitudeLatitude( -180.0 - 1e-6, -90.0 - 1e-6 ) );

  assert( ! emvl::isLongitudeLatitude( 180.0 + 1e-6, 0.0 ) );
  assert( ! emvl::isLongitudeLatitude( 0.0, 90.0 + 1e-6 ) );
  assert( ! emvl::isLongitudeLatitude( 180.0 + 1e-6, 90.0 + 1e-6 ) );

  const double longitudes[] = {
    -180.0, -180.0 + 1e-6, 0.0, 180.0 - 1e-6, 180.0
  };
  const size_t longitudeCount = sizeof longitudes / sizeof *longitudes;
  const double latitudes[] = { -90.0, -90.0 + 1e-6, 0.0, 90.0 - 1e-6, 90.0 };
  const size_t latitudeCount = sizeof latitudes / sizeof *latitudes;

  for ( size_t index = 0; index < longitudeCount; ++index ) {
    const double longitude = longitudes[ index ];

    for ( size_t index2 = 0; index2 < latitudeCount; ++index2 ) {
      const double latitude = latitudes[ index2 ];
      assert( emvl::isLongitudeLatitude( longitude, latitude ) );
    }
  }

  for ( size_t index1 = 0; index1 < rcount; ++index1 ) {
    const double value1 = rvalues[ index1 ];

    for ( size_t index2 = 0; index2 < rcount; ++index2 ) {
      const double value2 = rvalues[ index2 ];

      if ( ! OR2( emvl::isNan( value1 ), emvl::isNan( value2 ) ) ) {
        const bool result = emvl::isLongitudeLatitude( value1, value2 );
        assert( result == AND2( IN_RANGE( value1, -180.0, 180.0 ),
                                IN_RANGE( value2,  -90.0,  90.0 ) ) );
      }
    }
  }
}



/******************************************************************************
PURPOSE: radiansTests - Test radians routine.
******************************************************************************/

static void radiansTests() {
  assert( emvl::radians( 0.0 ) == 0.0 );
  assert( emvl::aboutEqual( emvl::radians( 45.0 ), 0.25 * M_PI ) );
  assert( emvl::aboutEqual( emvl::radians( 90.0 ), 0.5 * M_PI ) );
  assert( emvl::aboutEqual( emvl::radians( 180.0 ), M_PI ) );
  assert( emvl::aboutEqual( emvl::radians( 360.0 ), 2.0 * M_PI ) );
  assert( emvl::aboutEqual( emvl::radians( 720.0 ), 4.0 * M_PI ) );
  assert( emvl::aboutEqual( emvl::radians( -45.0 ), -0.25 * M_PI ) );
  assert( emvl::aboutEqual( emvl::radians( -90.0 ), -0.5 * M_PI ) );
  assert( emvl::aboutEqual( emvl::radians( -180.0 ), -M_PI ) );
  assert( emvl::aboutEqual( emvl::radians( -360.0 ), -2.0 * M_PI ) );
  assert( emvl::aboutEqual( emvl::radians( -720.0 ), -4.0 * M_PI ) );

  for ( size_t index = 0; index < rcount; ++index ) {
    const double value = rvalues[ index ];

    if ( ! emvl::isNan( value ) ) {
      const double result = emvl::radians( value );
      assert( ! emvl::isNan( result ) );
      assert( OR2( SIGN( result ) == SIGN( value ), result == 0.0 ) );
      assert( fabs( result ) <= fabs( value ) );
    }
  }
}



/******************************************************************************
PURPOSE: degreesTests - Test degrees routine.
******************************************************************************/

static void degreesTests() {
  assert( emvl::degrees( 0.0 ) == 0.0 );
  assert( emvl::aboutEqual( emvl::degrees( 0.25 * M_PI ), 45.0 ) );
  assert( emvl::aboutEqual( emvl::degrees( 0.5 * M_PI ), 90.0 ) );
  assert( emvl::aboutEqual( emvl::degrees( M_PI ), 180.0 ) );
  assert( emvl::aboutEqual( emvl::degrees( 2.0 * M_PI ), 360.0 ) );
  assert( emvl::aboutEqual( emvl::degrees( 4.0 * M_PI ), 720.0 ) );
  assert( emvl::aboutEqual( emvl::degrees( -0.25 * M_PI ), -45.0 ) );
  assert( emvl::aboutEqual( emvl::degrees( -0.5 * M_PI ), -90.0 ) );
  assert( emvl::aboutEqual( emvl::degrees( -M_PI ), -180.0 ) );
  assert( emvl::aboutEqual( emvl::degrees( -2.0 * M_PI ), -360.0 ) );
  assert( emvl::aboutEqual( emvl::degrees( -4.0 * M_PI ), -720.0 ) );

  for ( size_t index = 0; index < rcount; ++index ) {
    const double value = rvalues[ index ];

    if ( ! emvl::isNan( value ) ) {
      const double result = emvl::degrees( value );
      assert( ! emvl::isNan( result ) );
      assert( OR2( SIGN( result ) == SIGN( value ), result == 0.0 ) );
      assert( fabs( result ) >= fabs( value ) );
    }
  }

  for ( size_t index = 0; index < rcount; ++index ) {
    const double value = rvalues[ index ];

    if ( ! emvl::isNan( value ) ) {
      const double degrees = emvl::degrees( value );
      const double radians = emvl::radians( degrees );
      const bool result = emvl::aboutEqual( radians, value );
      assert( IMPLIES( AND2( emvl::isFinite( radians ),
                             emvl::isFinite( degrees ) ),
                       result ) );
    }
  }

  for ( size_t index = 0; index < rcount; ++index ) {
    const double value = rvalues[ index ];

    if ( ! emvl::isNan( value ) ) {
      const double radians = emvl::radians( value );
      const double degrees = emvl::degrees( radians );
      const bool result = emvl::aboutEqual( degrees, value );
      assert( IMPLIES( AND2( emvl::isFinite( radians ),
                             emvl::isFinite( degrees ) ),
                       result ) );
    }
  }
}



/******************************************************************************
PURPOSE: reverse2ByteWordsIfLittleEndianTests -
         Test reverse2ByteWordsIfLittleEndian routine.
******************************************************************************/

static void reverse2ByteWordsIfLittleEndianTests() {

  for ( size_t index = 0; index < icount; ++index ) {
    const short svalue = ivalues[ index ];
    const unsigned short uvalue = ivalues[ index ];
    short sresult = svalue;
    unsigned short uresult = uvalue;

    emvl::reverse2ByteWordsIfLittleEndian( (void*) &sresult );
    assert( IMPLIES( svalue == 0, sresult == 0 ) );
    assert( IMPLIES( svalue == -1, sresult == -1 ) );
    emvl::reverse2ByteWordsIfLittleEndian( &sresult );
    assert( sresult == svalue );

    emvl::reverse2ByteWordsIfLittleEndian( (void*) &uresult );
    assert( IMPLIES( uvalue == 0, uresult == 0 ) );
    assert( IMPLIES( uvalue == 65355, uresult == 65355 ) );
    emvl::reverse2ByteWordsIfLittleEndian( &uresult );
    assert( uresult == uvalue );
  }

  { // Test signed array argument usage:
    short values[ icount ];
    short valuesCopy[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      valuesCopy[ index ] = values[ index ] = ivalues[ index ];
    }

    emvl::reverse2ByteWordsIfLittleEndian( (void*) values,
                                           sizeof values / sizeof *values );
    emvl::reverse2ByteWordsIfLittleEndian( (void*) values,
                                           sizeof values / sizeof *values );
    for ( size_t index = 0; index < icount; ++index ) {
      assert( values[ index ] == valuesCopy[ index ] );
    }
  }

  { // Test unsigned array argument usage:
    unsigned short values[ icount ];
    unsigned short valuesCopy[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      valuesCopy[ index ] = values[ index ] = ivalues[ index ];
    }

    emvl::reverse2ByteWordsIfLittleEndian( (void*) values,
                                           sizeof values / sizeof *values );
    emvl::reverse2ByteWordsIfLittleEndian( (void*) values,
                                           sizeof values / sizeof *values );
    for ( size_t index = 0; index < icount; ++index ) {
      assert( values[ index ] == valuesCopy[ index ] );
    }
  }
}



/******************************************************************************
PURPOSE: reverse4ByteWordsIfLittleEndianTests -
         Test reverse4ByteWordsIfLittleEndian routine.
******************************************************************************/

static void reverse4ByteWordsIfLittleEndianTests() {

  for ( size_t index = 0; index < icount; ++index ) {
    const int svalue = ivalues[ index ];
    const unsigned int uvalue = ivalues[ index ];
    int sresult = svalue;
    unsigned int uresult = uvalue;

    emvl::reverse4ByteWordsIfLittleEndian( (void*) &sresult );
    assert( IMPLIES( svalue == 0, sresult == 0 ) );
    assert( IMPLIES( svalue == -1, sresult == -1 ) );
    emvl::reverse4ByteWordsIfLittleEndian( &sresult );
    assert( sresult == svalue );

    emvl::reverse4ByteWordsIfLittleEndian( (void*) &uresult );
    assert( IMPLIES( uvalue == 0, uresult == 0 ) );
    assert( IMPLIES( uvalue == 4294967295U, uresult == 4294967295U ) );
    emvl::reverse4ByteWordsIfLittleEndian( &uresult );
    assert( uresult == uvalue );
  }

  { // Test signed array argument usage:
    int values[ icount ];
    int valuesCopy[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      valuesCopy[ index ] = values[ index ] = ivalues[ index ];
    }

    emvl::reverse4ByteWordsIfLittleEndian( (void*) values,
                                           sizeof values / sizeof *values );
    emvl::reverse4ByteWordsIfLittleEndian( (void*) values,
                                           sizeof values / sizeof *values );
    for ( size_t index = 0; index < icount; ++index ) {
      assert( values[ index ] == valuesCopy[ index ] );
    }
  }

  { // Test unsigned array argument usage:
    unsigned int values[ icount ];
    unsigned int valuesCopy[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      valuesCopy[ index ] = values[ index ] = ivalues[ index ];
    }

    emvl::reverse4ByteWordsIfLittleEndian( (void*) values,
                                           sizeof values / sizeof *values );
    emvl::reverse4ByteWordsIfLittleEndian( (void*) values,
                                           sizeof values / sizeof *values );
    for ( size_t index = 0; index < icount; ++index ) {
      assert( values[ index ] == valuesCopy[ index ] );
    }
  }
}



/******************************************************************************
PURPOSE: reverse8ByteWordsIfLittleEndianTests -
         Test reverse8ByteWordsIfLittleEndian routine.
******************************************************************************/

static void reverse8ByteWordsIfLittleEndianTests() {

  for ( size_t index = 0; index < icount; ++index ) {
    const long long svalue = ivalues[ index ];
    const unsigned long long uvalue = ivalues[ index ];
    long long sresult = svalue;
    unsigned long long uresult = uvalue;

    emvl::reverse8ByteWordsIfLittleEndian( (void*) &sresult );
    assert( IMPLIES( svalue == 0, sresult == 0 ) );
    assert( IMPLIES( svalue == -1, sresult == -1 ) );
    emvl::reverse8ByteWordsIfLittleEndian( &sresult );
    assert( sresult == svalue );

    emvl::reverse8ByteWordsIfLittleEndian( (void*) &uresult );
    assert( IMPLIES( uvalue == 0, uresult == 0 ) );
    assert( IMPLIES( uvalue  == 18446744073709551615ULL,
                     uresult == 18446744073709551615ULL ) );
    emvl::reverse8ByteWordsIfLittleEndian( &uresult );
    assert( uresult == uvalue );
  }

  { // Test signed array argument usage:
    long long values[ icount ];
    long long valuesCopy[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      valuesCopy[ index ] = values[ index ] = ivalues[ index ];
    }

    emvl::reverse8ByteWordsIfLittleEndian( (void*) values,
                                           sizeof values / sizeof *values );
    emvl::reverse8ByteWordsIfLittleEndian( (void*) values,
                                           sizeof values / sizeof *values );
    for ( size_t index = 0; index < icount; ++index ) {
      assert( values[ index ] == valuesCopy[ index ] );
    }
  }

  { // Test unsigned array argument usage:
    unsigned long long values[ icount ];
    unsigned long long valuesCopy[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      valuesCopy[ index ] = values[ index ] = ivalues[ index ];
    }

    emvl::reverse8ByteWordsIfLittleEndian( (void*) values,
                                           sizeof values / sizeof *values );
    emvl::reverse8ByteWordsIfLittleEndian( (void*) values,
                                           sizeof values / sizeof *values );
    for ( size_t index = 0; index < icount; ++index ) {
      assert( values[ index ] == valuesCopy[ index ] );
    }
  }
}



/******************************************************************************
PURPOSE: expand4ByteWordsTests - Test expand4ByteWords routine.
******************************************************************************/

static void expand4ByteWordsTests() {

  // Test default argument form with count == 1:

  for ( size_t index = 0; index < icount; ++index ) {
    const long long value = ivalues[ index ];

    if ( emvl::isSignedInt( value ) ) {
      int value4 = (int) value;
      long long result = (long long) value4;
      emvl::expand4ByteWords( (long long*) &result );
      assert( result == value );
    } else if ( emvl::isUnsignedInt( value ) ) {
      unsigned int value4 = (int) value;
      long long result = (long long) value4;
      emvl::expand4ByteWords( (unsigned long long*) &result );
      assert( result == value );
    }
  }

  for ( size_t index = 0; index < rcount; ++index ) {
    const double value = rvalues[ index ];
    float value4[ 2 ] = { 0.0, 0.0 };
    value4[ 0 ] = (float)
      emvl::clampedToRange( value, (double) -FLT_MAX, (double) FLT_MAX );
    double* result = (double*) &value4[ 0 ];
    emvl::expand4ByteWords( result );
    assert( IMPLIES( IN_RANGE( value, -1e-38, 1e38 ),
                     emvl::aboutEqual( *result, value ) ) );
  }

  // Test array form:

  {
    long long result[ icount ];
    memset( result, 0, sizeof result );
    int* const array4 = (int*) &result[ 0 ];
    
    for ( size_t index = 0; index < icount; ++index ) {
      const long long value = ivalues[ index ];

      if ( emvl::isSignedInt( value ) ) {
        array4[ index ] = (int) value;
      }
    }

    emvl::expand4ByteWords( (long long*) result,
                            sizeof result / sizeof *result );

    for ( size_t index = 0; index < icount; ++index ) {
      assert( IMPLIES_ELSE( emvl::isSignedInt( ivalues[ index ] ),
                            result[ index ] == ivalues[ index ],
                            result[ index ] == 0 ) );
    }
  }

  {
    long long result[ icount ];
    memset( result, 0, sizeof result );
    unsigned int* const array4 = (unsigned int*) &result[ 0 ];
    
    for ( size_t index = 0; index < icount; ++index ) {
      const long long value = ivalues[ index ];

      if ( emvl::isUnsignedInt( value ) ) {
        array4[ index ] = (unsigned int) value;
      }
    }

    emvl::expand4ByteWords( (unsigned long long*) result,
                            sizeof result / sizeof *result );

    for ( size_t index = 0; index < icount; ++index ) {
      assert( IMPLIES_ELSE( emvl::isUnsignedInt( ivalues[ index ] ),
                            result[ index ] == ivalues[ index ],
                            result[ index ] == 0 ) );
    }
  }

  {
    double result[ rcount ];
    memset( result, 0, sizeof result );
    float* const array4 = (float*) &result[ 0 ];
    
    for ( size_t index = 0; index < rcount; ++index ) {
      const double value = rvalues[ index ];
      float value4 = (float)
        emvl::clampedToRange( value, (double) -FLT_MAX, (double) FLT_MAX );
      array4[ index ] = value4;
    }

    emvl::expand4ByteWords( result, sizeof result / sizeof *result );

    for ( size_t index = 0; index < rcount; ++index ) {
      const double value = rvalues[ index ];
      const double resultValue = result[ index ];
      assert( IMPLIES( IN_RANGE( value, -1e-38, 1e38 ),
                       emvl::aboutEqual( resultValue, value ) ) );
    }
  }
}



/******************************************************************************
PURPOSE: isNanFreeTests - Test isNanFree routine.
******************************************************************************/

static void isNanFreeTests() {

  assert( emvl::isNanFree( rvalues, rcount ) );

  { // Test double array usage:
    double values[ rcount ];

    for ( size_t index = 0; index < rcount; ++index ) {
      const double dvalue = rvalues[ index ];
      const double value = dvalue * dvalue / ( dvalue - 1.0 );
      values[ index ] = value;
    }

    assert( ! emvl::isNanFree( values, rcount ) );
  }

  { // Test float array usage:
    float values[ rcount ];

    for ( size_t index = 0; index < rcount; ++index ) {
      const double dvalue = rvalues[ index ];
      const float fvalue = dvalue;
      values[ index ] = fvalue;
    }

    assert( emvl::isNanFree( values, rcount ) );

    for ( size_t index = 0; index < rcount; ++index ) {
      const double dvalue = rvalues[ index ];
      const float fvalue = dvalue * dvalue / ( dvalue - 1.0 );
      values[ index ] = fvalue;
    }

    assert( ! emvl::isNanFree( values, rcount ) );
  }
}



/******************************************************************************
PURPOSE: allFiniteTests - Test allFinite routine.
******************************************************************************/

static void allFiniteTests() {

  assert( ! emvl::allFinite( rvalues, rcount ) );

  { // Test double array usage:
    double values[ rcount ];

    for ( size_t index = 0; index < rcount; ++index ) {
      const double dvalue = rvalues[ index ];
      const double value = emvl::isFinite( dvalue ) ? dvalue : 0.0;
      values[ index ] = value;
    }

    assert( emvl::allFinite( values, rcount ) );
  }

  { // Test float array usage:
    float values[ rcount ];

    for ( size_t index = 0; index < rcount; ++index ) {
      const float fvalue = rvalues[ index ];
      const float value = emvl::isFinite( fvalue ) ? fvalue : 0.0;
      values[ index ] = value;
    }

    assert( emvl::allFinite( values, rcount ) );

    for ( size_t index = 0; index < rcount; ++index ) {
      const float value = rvalues[ index ];
      values[ index ] = value;
    }

    assert( ! emvl::allFinite( values, rcount ) );
  }
}



/******************************************************************************
PURPOSE: clampedToRangeTests - Test clampedToRange routine.
******************************************************************************/

static void clampedToRangeTests() {

  for ( size_t index1 = 0; index1 < icount; ++index1 ) {
    const long long value1 = ivalues[ index1 ];
    const char cvalue1 = value1;
    const unsigned char ucvalue1 = value1;
    const short svalue1 = value1;
    const unsigned short usvalue1 = value1;
    const int ivalue1 = value1;
    const unsigned int uivalue1 = value1;
    const long lvalue1 = value1;
    const unsigned long ulvalue1 = value1;
    const long long llvalue1 = value1;
    const unsigned long long ullvalue1 = value1;

    for ( size_t index2 = 0; index2 < icount; ++index2 ) {
      const long long value2 = ivalues[ index2 ];
      const char cvalue2 = value2;
      const unsigned char ucvalue2 = value2;
      const short svalue2 = value2;
      const unsigned short usvalue2 = value2;
      const int ivalue2 = value2;
      const unsigned int uivalue2 = value2;
      const long lvalue2 = value2;
      const unsigned long ulvalue2 = value2;
      const long long llvalue2 = value2;
      const unsigned long long ullvalue2 = value2;

      for ( size_t index3 = 0; index3 < icount; ++index3 ) {
        const long long value3 = ivalues[ index3 ];
        const char cvalue3 = value3;
        const unsigned char ucvalue3 = value3;
        const short svalue3 = value3;
        const unsigned short usvalue3 = value3;
        const int ivalue3 = value3;
        const unsigned int uivalue3 = value3;
        const long lvalue3 = value3;
        const unsigned long ulvalue3 = value3;
        const long long llvalue3 = value3;
        const unsigned long long ullvalue3 = value3;

        if ( cvalue1 <= cvalue2 ) {
          const char cresult =
            emvl::clampedToRange( cvalue3, cvalue1, cvalue2 );
          assert( IN_RANGE( cresult, cvalue1, cvalue2 ) );
        }

        if ( ucvalue1 <= ucvalue2 ) {
          const unsigned char ucresult =
            emvl::clampedToRange( ucvalue3, ucvalue1, ucvalue2 );
          assert( IN_RANGE( ucresult, ucvalue1, ucvalue2 ) );
        }

        if ( svalue1 <= svalue2 ) {
          const short sresult =
            emvl::clampedToRange( svalue3, svalue1, svalue2 );
          assert( IN_RANGE( sresult, svalue1, svalue2 ) );
        }

        if ( usvalue1 <= usvalue2 ) {
          const unsigned short usresult =
            emvl::clampedToRange( usvalue3, usvalue1, usvalue2 );
          assert( IN_RANGE( usresult, usvalue1, usvalue2 ) );
        }

        if ( ivalue1 <= ivalue2 ) {
          const int iresult =
            emvl::clampedToRange( ivalue3, ivalue1, ivalue2 );
          assert( IN_RANGE( iresult, ivalue1, ivalue2 ) );
        }

        if ( uivalue1 <= uivalue2 ) {
          const unsigned int uiresult =
            emvl::clampedToRange( uivalue3, uivalue1, uivalue2);
          assert( IN_RANGE( uiresult, uivalue1, uivalue2 ) );
        }

        if ( lvalue1 <= lvalue2 ) {
          const long lresult =
            emvl::clampedToRange( lvalue3, lvalue1, lvalue2 );
          assert( IN_RANGE( lresult, lvalue1, lvalue2 ) );
        }

        if ( ulvalue1 <= ulvalue2 ) {
          const unsigned long ulresult =
            emvl::clampedToRange( ulvalue3, ulvalue1, ulvalue2 );
          assert( IN_RANGE( ulresult, ulvalue1, ulvalue2 ) );
        }

        if ( llvalue1 <= llvalue2 ) {
          const long long llresult =
            emvl::clampedToRange( llvalue3, llvalue1, llvalue2 );
          assert( IN_RANGE( llresult, llvalue1, llvalue2 ) );
        }

        if ( ullvalue1 <= ullvalue2 ) {
          const unsigned long long ullresult =
            emvl::clampedToRange( ullvalue3, ullvalue1, ullvalue2 );
          assert( IN_RANGE( ullresult, ullvalue1, ullvalue2 ) );
        }
      }
    }
  }

  for ( size_t index1 = 0; index1 < rcount; ++index1 ) {
    const double value1 = rvalues[ index1 ];
    const float fvalue1 = value1;

    for ( size_t index2 = 0; index2 < rcount; ++index2 ) {
      const double value2 = rvalues[ index2 ];
      const float fvalue2 = value2;

      for ( size_t index3 = 0; index3 < rcount; ++index3 ) {
        const double value3 = rvalues[ index3 ];
        const float fvalue3 = value3;

        if ( value1 <= value2 ) {
          const double result =
            emvl::clampedToRange( value3, value1, value2 );
          assert( IN_RANGE( result, value1, value2 ) );
        }

        if ( fvalue1 <= fvalue2 ) {
          const float result =
            emvl::clampedToRange( fvalue3, fvalue1, fvalue2 );
          assert( IN_RANGE( result, fvalue1, fvalue2 ) );
        }
      }
    }
  }
}



/******************************************************************************
PURPOSE: allZeroTests - Test allZero routine.
******************************************************************************/

static void allZeroTests( ) {

  {
    char values[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values[ index ] = ivalues[ index ];
    }

    allZeroTestsT( values, icount );
  }

  {
    unsigned char values[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values[ index ] = ivalues[ index ];
    }

    allZeroTestsT( values, icount );
  }

  {
    short values[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values[ index ] = ivalues[ index ];
    }

    allZeroTestsT( values, icount );
  }

  {
    unsigned short values[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values[ index ] = ivalues[ index ];
    }

    allZeroTestsT( values, icount );
  }

  {
    int values[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values[ index ] = ivalues[ index ];
    }

    allZeroTestsT( values, icount );
  }

  {
    unsigned int values[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values[ index ] = ivalues[ index ];
    }

    allZeroTestsT( values, icount );
  }

  {
    long values[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values[ index ] = ivalues[ index ];
    }

    allZeroTestsT( values, icount );
  }

  {
    unsigned long values[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values[ index ] = ivalues[ index ];
    }

    allZeroTestsT( values, icount );
  }

  {
    long long values[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values[ index ] = ivalues[ index ];
    }

    allZeroTestsT( values, icount );
  }

  {
    unsigned long long values[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values[ index ] = ivalues[ index ];
    }

    allZeroTestsT( values, icount );
  }

  {
    float values[ rcount ];

    for ( size_t index = 0; index < rcount; ++index ) {
      values[ index ] = rvalues[ index ];
    }

    allZeroTestsT( values, icount );
  }

  allZeroTestsT( rvalues, sizeof rvalues / sizeof *rvalues );
}



/******************************************************************************
PURPOSE: allZeroTestsT - Test allZero routine with type T.
******************************************************************************/

template <typename T>
static void allZeroTestsT( const T values[], const size_t count ) {
  assert( ! emvl::allZero( values, count ) );
  const T values1[ 1 ] = { 0 };
  assert( emvl::allZero( values1, sizeof values1 / sizeof *values1 ) );
  const T values2[ 2 ] = { 0, 0 };
  assert( emvl::allZero( values2, sizeof values2 / sizeof *values2 ) );
  const T values3[ 3 ] = { 0, 2, 3 };
  assert( ! emvl::allZero( values3, sizeof values3 / sizeof *values3 ) );
  const T* values4[ 4 ] = { values, values, values, 0 };
  assert( ! emvl::allZero( values4, sizeof values4 / sizeof *values4 ) );
}



/******************************************************************************
PURPOSE: allNonZeroTests - Test allNonZero routine.
******************************************************************************/

static void allNonZeroTests() {

  {
    char values[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values[ index ] = ivalues[ index ];
    }

    allNonZeroTestsT( values, icount );
  }

  {
    unsigned char values[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values[ index ] = ivalues[ index ];
    }

    allNonZeroTestsT( values, icount );
  }

  {
    short values[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values[ index ] = ivalues[ index ];
    }

    allNonZeroTestsT( values, icount );
  }

  {
    unsigned short values[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values[ index ] = ivalues[ index ];
    }

    allNonZeroTestsT( values, icount );
  }

  {
    int values[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values[ index ] = ivalues[ index ];
    }

    allNonZeroTestsT( values, icount );
  }

  {
    unsigned int values[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values[ index ] = ivalues[ index ];
    }

    allNonZeroTestsT( values, icount );
  }

  {
    long values[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values[ index ] = ivalues[ index ];
    }

    allNonZeroTestsT( values, icount );
  }

  {
    unsigned long values[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values[ index ] = ivalues[ index ];
    }

    allNonZeroTestsT( values, icount );
  }

  {
    long long values[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values[ index ] = ivalues[ index ];
    }

    allNonZeroTestsT( values, icount );
  }

  {
    unsigned long long values[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values[ index ] = ivalues[ index ];
    }

    allNonZeroTestsT( values, icount );
  }

  {
    float values[ rcount ];

    for ( size_t index = 0; index < rcount; ++index ) {
      values[ index ] = rvalues[ index ];
    }

    allNonZeroTestsT( values, rcount );
  }

  {
    double values[ rcount ];

    for ( size_t index = 0; index < rcount; ++index ) {
      values[ index ] = rvalues[ index ];
    }

    allNonZeroTestsT( values, rcount );
  }
}



/******************************************************************************
PURPOSE: allNonZeroTests - Test allNonZero routine with type T.
******************************************************************************/

template <typename T>
static void allNonZeroTestsT( const T values[], const size_t count ) {
  assert( ! emvl::allNonZero( values, count ) );
  const T values1[ 1 ] = { 1 };
  assert( emvl::allNonZero( values1, sizeof values1 / sizeof *values1 ) );
  const T values2[ 2 ] = { 1, 2 };
  assert( emvl::allNonZero( values2, sizeof values2 / sizeof *values2 ) );
  const T values3[ 3 ] = { 0, 2, 3 };
  assert( ! emvl::allNonZero( values3, sizeof values3 / sizeof *values3 ) );
  const T* values4[ 4 ] = { values, values, values, 0 };
  assert( ! emvl::allNonZero( values4, sizeof values4 / sizeof *values4 ) );
}



/******************************************************************************
PURPOSE: isSortedTests - Test isSorted routine.
******************************************************************************/

static void isSortedTests() {

  {
    char values[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values[ index ] = ivalues[ index ];
    }

    isSortedTestsT( values, icount );
  }

  {
    unsigned char values[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values[ index ] = ivalues[ index ];
    }

    isSortedTestsT( values, icount );
  }

  {
    short values[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values[ index ] = ivalues[ index ];
    }

    isSortedTestsT( values, icount );
  }

  {
    unsigned short values[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values[ index ] = ivalues[ index ];
    }

    isSortedTestsT( values, icount );
  }

  {
    int values[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values[ index ] = ivalues[ index ];
    }

    isSortedTestsT( values, icount );
  }

  {
    unsigned int values[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values[ index ] = ivalues[ index ];
    }

    isSortedTestsT( values, icount );
  }

  {
    long values[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values[ index ] = ivalues[ index ];
    }

    isSortedTestsT( values, icount );
  }

  {
    unsigned long values[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values[ index ] = ivalues[ index ];
    }

    isSortedTestsT( values, icount );
  }

  {
    long long values[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values[ index ] = ivalues[ index ];
    }

    isSortedTestsT( values, icount );
  }

  {
    unsigned long long values[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values[ index ] = ivalues[ index ];
    }

    isSortedTestsT( values, icount );
  }

  {
    float values[ rcount ];

    for ( size_t index = 0; index < rcount; ++index ) {
      values[ index ] = rvalues[ index ];
    }

    isSortedTestsT( values, rcount );
  }

  isSortedTestsT( rvalues, sizeof rvalues / sizeof *rvalues );
}



/******************************************************************************
PURPOSE: isSortedTestsT - Test isSorted routine with type T.
******************************************************************************/

template <typename T>
static void isSortedTestsT( const T values[], const size_t count ) {
  assert( IMPLIES( emvl::isNanFree( values, count ),
                   ! emvl::isSorted( values, count ) ) );
  const T values1[ 1 ] = { 1 };
  assert( emvl::isSorted( values1, sizeof values1 / sizeof *values1 ) );
  const T values2[ 2 ] = { 1, 2 };
  assert( emvl::isSorted( values2, sizeof values2 / sizeof *values2 ) );
  const T values3[ 3 ] = { 1, 1, 1 };
  assert( emvl::isSorted( values3, sizeof values3 / sizeof *values3 ) );
  const T* values4[ 4 ] = { values, values, values, 0 };
  assert( ! emvl::isSorted( values4, sizeof values4 / sizeof *values4 ) );
  static T* values5[ 5 ];
  assert( emvl::isSorted( values5, sizeof values5 / sizeof *values5 ) );
  T* values6[ count ];
  memcpy( values6, values, count );
  assert( IMPLIES( emvl::isNanFree( values6, count ),
                   ! emvl::isSorted( values6, count ) ) );
  std::sort( values6, values6 + count );
  assert( IMPLIES( emvl::isNanFree( values6, count ),
                   emvl::isSorted( values6, count ) ) );
}



/******************************************************************************
PURPOSE: hasTests - Test has routine.
******************************************************************************/

static void hasTests() {

  {
    char values[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values[ index ] = ivalues[ index ];
    }

    hasTestsT( values, icount );
  }

  {
    unsigned char values[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values[ index ] = ivalues[ index ];
    }

    hasTestsT( values, icount );
  }

  {
    short values[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values[ index ] = ivalues[ index ];
    }

    hasTestsT( values, icount );
  }

  {
    unsigned short values[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values[ index ] = ivalues[ index ];
    }

    hasTestsT( values, icount );
  }


  {
    int values[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values[ index ] = ivalues[ index ];
    }

    hasTestsT( values, icount );
  }

  {
    unsigned int values[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values[ index ] = ivalues[ index ];
    }

    hasTestsT( values, icount );
  }

  {
    long values[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values[ index ] = ivalues[ index ];
    }

    hasTestsT( values, icount );
  }

  {
    unsigned long values[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values[ index ] = ivalues[ index ];
    }

    hasTestsT( values, icount );
  }

  {
    long long values[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values[ index ] = ivalues[ index ];
    }

    hasTestsT( values, icount );
  }

  {
    unsigned long long values[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values[ index ] = ivalues[ index ];
    }

    hasTestsT( values, icount );
  }

  {
    float values[ rcount ];

    for ( size_t index = 0; index < rcount; ++index ) {
      values[ index ] = rvalues[ index ];
    }

    hasTestsT( values, rcount );
  }

  hasTestsT( rvalues, sizeof rcount );
}



/******************************************************************************
PURPOSE: hasTestsT - Test has routine with type T.
******************************************************************************/

template <typename T>
static void hasTestsT( const T values[], const size_t count ) {
  assert( IMPLIES( emvl::isNanFree( values, count ),
                   emvl::has( values, count, values[ 0 ] ) ) );
  assert( IMPLIES( emvl::isNanFree( values, count ),
                   emvl::has( values, count, values[ count / 2 ] ) ) );
  assert( IMPLIES( emvl::isNanFree( values, count ),
                   emvl::has( values, count, values[ count - 1 ] ) ) );
  const T values1[ 1 ] = { 1 };
  assert( emvl::has( values1, sizeof values1 / sizeof *values1, *values1 ) );
  assert( ! emvl::has( values1, sizeof values1 / sizeof *values1,
                       (T) 0 ) );
  const T values3[ 3 ] = { 1, 2, 3 };
  assert( emvl::has( values3, sizeof values3 / sizeof *values3, values3[0]));
  assert( emvl::has( values3, sizeof values3 / sizeof *values3, values3[1]));
  assert( emvl::has( values3, sizeof values3 / sizeof *values3, values3[2]));
  assert( ! emvl::has( values3, sizeof values3 / sizeof *values3,
                       (T) 0 ) );
  const T* values4[ 4 ] = { values, values, values, 0 };
  assert( emvl::has( values4, sizeof values4 / sizeof *values4,
                     (const T*) 0 ) );
  static T* values5[ 5 ];
  assert( emvl::has( (const T**) values5,
                      sizeof values5 / sizeof *values5,
                     (const T*) 0 ) );
  assert( ! emvl::has( (const T**) values5,
                       sizeof values5 / sizeof *values5, values ) );
}



/******************************************************************************
PURPOSE: minimumTests - Test minimum routine.
******************************************************************************/

static void minimumTests() {

  {
    char values[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values[ index ] = ivalues[ index ];
    }

    minimumTestsT( values, icount );
  }

  {
    unsigned char values[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values[ index ] = ivalues[ index ];
    }

    minimumTestsT( values, icount );
  }

  {
    short values[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values[ index ] = ivalues[ index ];
    }

    minimumTestsT( values, icount );
  }

  {
    unsigned short values[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values[ index ] = ivalues[ index ];
    }

    minimumTestsT( values, icount );
  }


  {
    int values[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values[ index ] = ivalues[ index ];
    }

    minimumTestsT( values, icount );
  }

  {
    unsigned int values[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values[ index ] = ivalues[ index ];
    }

    minimumTestsT( values, icount );
  }


  {
    long values[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values[ index ] = ivalues[ index ];
    }

    minimumTestsT( values, icount );
  }

  {
    unsigned long values[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values[ index ] = ivalues[ index ];
    }

    minimumTestsT( values, icount );
  }


  {
    long long values[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values[ index ] = ivalues[ index ];
    }

    minimumTestsT( values, icount );
  }

  {
    unsigned long long values[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values[ index ] = ivalues[ index ];
    }

    minimumTestsT( values, icount );
  }


  {
    float values[ rcount ];

    for ( size_t index = 0; index < rcount; ++index ) {
      values[ index ] = rvalues[ index ];
    }

    minimumTestsT( values, rcount );
  }

  minimumTestsT( rvalues, rcount );
}



/******************************************************************************
PURPOSE: minimumTestsT - Test minimum routine with type T.
******************************************************************************/

template <typename T>
static void minimumTestsT( const T values[], const size_t count ) {
  assert( IMPLIES( emvl::isNanFree( values, count ),
                   emvl::has( values, count, emvl::minimum(values, count))));
  const T values1[ 1 ] = { 1 };
  assert( emvl::minimum( values1, sizeof values1 / sizeof *values1 )
          == *values1 );
  const T values3[ 3 ] = { 1, 2, 3 };
  assert( emvl::minimum( values3, sizeof values3 / sizeof *values3 )
          == *values3 );
  const T* values4[ 4 ] = { values, values, values, 0 };
  assert( emvl::minimum( values4, sizeof values4 / sizeof *values4 )
          == values4[ 3 ] );
}



/******************************************************************************
PURPOSE: maximumTests - Test maximum routine.
******************************************************************************/

static void maximumTests() {

  {
    char values[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values[ index ] = ivalues[ index ];
    }

    maximumTestsT( values, icount );
  }

  {
    unsigned char values[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values[ index ] = ivalues[ index ];
    }

    maximumTestsT( values, icount );
  }

  {
    short values[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values[ index ] = ivalues[ index ];
    }

    maximumTestsT( values, icount );
  }

  {
    unsigned short values[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values[ index ] = ivalues[ index ];
    }

    maximumTestsT( values, icount );
  }


  {
    int values[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values[ index ] = ivalues[ index ];
    }

    maximumTestsT( values, icount );
  }

  {
    unsigned int values[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values[ index ] = ivalues[ index ];
    }

    maximumTestsT( values, icount );
  }


  {
    long values[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values[ index ] = ivalues[ index ];
    }

    maximumTestsT( values, icount );
  }

  {
    unsigned long values[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values[ index ] = ivalues[ index ];
    }

    maximumTestsT( values, icount );
  }


  {
    long long values[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values[ index ] = ivalues[ index ];
    }

    maximumTestsT( values, icount );
  }

  {
    unsigned long long values[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values[ index ] = ivalues[ index ];
    }

    maximumTestsT( values, icount );
  }


  {
    float values[ rcount ];

    for ( size_t index = 0; index < rcount; ++index ) {
      values[ index ] = rvalues[ index ];
    }

    maximumTestsT( values, rcount );
  }

  maximumTestsT( rvalues, rcount );
}



/******************************************************************************
PURPOSE: maximumTestsT - Test maximum routine with type T.
******************************************************************************/

template <typename T>
static void maximumTestsT( const T values[], const size_t count ) {
  assert( IMPLIES( emvl::isNanFree( values, count ),
                   emvl::has( values, count, emvl::maximum(values, count))));
  const T values1[ 1 ] = { 1 };
  assert( emvl::maximum( values1, sizeof values1 / sizeof *values1 )
          == *values1 );
  const T values3[ 3 ] = { 1, 2, 3 };
  assert( emvl::maximum( values3, sizeof values3 / sizeof *values3 )
          == values3[ 2 ] );
  const T* values4[ 4 ] = { values, values, values, 0 };
  assert( emvl::maximum( values4, sizeof values4 / sizeof *values4 )
          == values4[ 0 ] );
}



/******************************************************************************
PURPOSE: rangeTests - Test range routine.
******************************************************************************/

static void rangeTests() {

  {
    char values[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values[ index ] = ivalues[ index ];
    }

    rangeTestsT( values, icount );
  }

  {
    unsigned char values[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values[ index ] = ivalues[ index ];
    }

    rangeTestsT( values, icount );
  }

  {
    short values[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values[ index ] = ivalues[ index ];
    }

    rangeTestsT( values, icount );
  }

  {
    unsigned short values[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values[ index ] = ivalues[ index ];
    }

    rangeTestsT( values, icount );
  }


  {
    int values[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values[ index ] = ivalues[ index ];
    }

    rangeTestsT( values, icount );
  }

  {
    unsigned int values[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values[ index ] = ivalues[ index ];
    }

    rangeTestsT( values, icount );
  }


  {
    long values[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values[ index ] = ivalues[ index ];
    }

    rangeTestsT( values, icount );
  }

  {
    unsigned long values[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values[ index ] = ivalues[ index ];
    }

    rangeTestsT( values, icount );
  }


  {
    long long values[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values[ index ] = ivalues[ index ];
    }

    rangeTestsT( values, icount );
  }

  {
    unsigned long long values[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values[ index ] = ivalues[ index ];
    }

    rangeTestsT( values, icount );
  }


  {
    float values[ rcount ];

    for ( size_t index = 0; index < rcount; ++index ) {
      values[ index ] = rvalues[ index ];
    }

    rangeTestsT( values, rcount );
  }

  rangeTestsT( rvalues, rcount );
}



/******************************************************************************
PURPOSE: rangeTestsT - Test range routine with type T.
******************************************************************************/

template <typename T>
static void rangeTestsT( const T values[], const size_t count ) {
  assert( IMPLIES( emvl::isNanFree( values, count ),
                   emvl::has( values, count, emvl::maximum(values, count))));

  {
    const T values1[ 1 ] = { 1 };
    T minimum = 0;
    T maximum = 0;
    emvl::range( values1, sizeof values1 / sizeof *values1, minimum , maximum);
    assert( minimum == maximum );
    assert( maximum == *values1 );
    const T values3[ 3 ] = { 1, 2, 3 };
    emvl::range( values3, sizeof values3 / sizeof *values3, minimum , maximum);
    assert( minimum == values3[ 0 ] );
    assert( maximum == values3[ 2 ] );
  }
#if 0
  {
    const T* values4[ 4 ] = { values, values, values, 0 };
    T* minimum = 0;
    T* maximum = 0;
    emvl::range( values4, sizeof values4 / sizeof *values4, minimum , maximum);
    assert( minimum == values4[ 3 ] );
    assert( maximum == values4[ 0 ] );
  }
#endif
}



/******************************************************************************
PURPOSE: sumTests - Test sum routine.
******************************************************************************/

static void sumTests() {

  {
    char values[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values[ index ] = ivalues[ index ];
    }

    sumTestsT( values, icount );
  }

  {
    unsigned char values[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values[ index ] = ivalues[ index ];
    }

    sumTestsT( values, icount );
  }

  {
    short values[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values[ index ] = ivalues[ index ];
    }

    sumTestsT( values, icount );
  }

  {
    unsigned short values[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values[ index ] = ivalues[ index ];
    }

    sumTestsT( values, icount );
  }


  {
    int values[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values[ index ] = ivalues[ index ];
    }

    sumTestsT( values, icount );
  }

  {
    unsigned int values[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values[ index ] = ivalues[ index ];
    }

    sumTestsT( values, icount );
  }


  {
    long values[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values[ index ] = ivalues[ index ];
    }

    sumTestsT( values, icount );
  }

  {
    unsigned long values[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values[ index ] = ivalues[ index ];
    }

    sumTestsT( values, icount );
  }


  {
    long long values[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values[ index ] = ivalues[ index ];
    }

    sumTestsT( values, icount );
  }

  {
    unsigned long long values[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values[ index ] = ivalues[ index ];
    }

    sumTestsT( values, icount );
  }

  {
    float values[ rcount ];

    for ( size_t index = 0; index < rcount; ++index ) {
      values[ index ] = rvalues[ index ];
    }

    sumTestsT( values, rcount );
  }

  sumTestsT( rvalues, rcount );
}



/******************************************************************************
PURPOSE: sumTestsT - Test sum routine with type T.
******************************************************************************/

template <typename T>
static void sumTestsT( const T values[], const size_t count ) {
  assert( IMPLIES( emvl::allFinite( values, count ),
                   ! emvl::isNan( emvl::sum( values, count ) ) ) );
  const T values1[ 1 ] = { 1 };
  assert( emvl::sum( values1, sizeof values1 / sizeof *values1 )
          == *values1 );
  const T values3[ 3 ] = { 1, 2, 3 };
  assert( emvl::aboutEqual( emvl::sum( values3,
                                       sizeof values3 / sizeof *values3 ),
                            values3[ 0 ] + values3[ 1 ] +values3[ 2 ] ) );
}



/******************************************************************************
PURPOSE: accumulateTests - Test accumulate routine.
******************************************************************************/

static void accumulateTests() {
  {
    char values[ icount ];
    char values2[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values2[ index ] = values[ index ] = ivalues[ index ];
    }

    accumulateTestsT( values, values2, icount );
  }

  {
    unsigned char values[ icount ];
    unsigned char values2[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values2[ index ] = values[ index ] = ivalues[ index ];
    }

    accumulateTestsT( values, values2, icount );
  }

  {
    short values[ icount ];
    short values2[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values2[ index ] = values[ index ] = ivalues[ index ];
    }

    accumulateTestsT( values, values2, icount );
  }

  {
    unsigned short values[ icount ];
    unsigned short values2[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values2[ index ] = values[ index ] = ivalues[ index ];
    }

    accumulateTestsT( values, values2, icount );
  }

  {
    int values[ icount ];
    int values2[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values2[ index ] = values[ index ] = ivalues[ index ];
    }

    accumulateTestsT( values, values2, icount );
  }

  {
    unsigned int values[ icount ];
    unsigned int values2[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values2[ index ] = values[ index ] = ivalues[ index ];
    }

    accumulateTestsT( values, values2, icount );
  }

  {
    long values[ icount ];
    long values2[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values2[ index ] = values[ index ] = ivalues[ index ];
    }

    accumulateTestsT( values, values2, icount );
  }

  {
    unsigned long values[ icount ];
    unsigned long values2[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values2[ index ] = values[ index ] = ivalues[ index ];
    }

    accumulateTestsT( values, values2, icount );
  }

  {
    long long values[ icount ];
    long long values2[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values2[ index ] = values[ index ] = ivalues[ index ];
    }

    accumulateTestsT( values, values2, icount );
  }

  {
    unsigned long long values[ icount ];
    unsigned long long values2[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values2[ index ] = values[ index ] = ivalues[ index ];
    }

    accumulateTestsT( values, values2, icount );
  }

  {
    float values[ rcount ];
    float values2[ rcount ];

    for ( size_t index = 0; index < rcount; ++index ) {
      values2[ index ] = values[ index ] = rvalues[ index ];
    }

    accumulateTestsT( values, values2, rcount );
  }

  {
    double values[ rcount ];
    double values2[ rcount ];

    for ( size_t index = 0; index < rcount; ++index ) {
      values2[ index ] = values[ index ] = rvalues[ index ];
    }

    accumulateTestsT( values, values2, rcount );
  }
}



/******************************************************************************
PURPOSE: accumulateTestsT - Test accumulate routine with type T.
******************************************************************************/

template <typename T>
static void accumulateTestsT( const T values[], T values2[], const size_t count ) {

  if ( emvl::allFinite( values, count ) ) {
    memset( values2, 0, count * sizeof *values2 );
    emvl::accumulate( values, values2, count );

    for ( size_t index = 0; index < count; ++index ) {
      assert( values2[ index ] == values[ index ] );
    }

    memcpy( values2, values, count * sizeof *values2 );
    emvl::accumulate( values, values2, count );

    for ( size_t index = 0; index < count; ++index ) {
      const T value2 = values[ index ] + values[ index ];
      assert( values2[ index ] == value2 );
    }
  }

  T values1[ 1 ] = { 1 };
  emvl::accumulate( values1, values1, sizeof values1 / sizeof *values1 );
  assert( values1[ 0 ] == 2 );
  T values3[ 3 ] = { 0, 2, 3 };
  emvl::accumulate( values3, values3, sizeof values3 / sizeof *values3 );
  assert( values3[ 0 ] == 0 );
  assert( values3[ 1 ] == 4 );
  assert( values3[ 2 ] == 6 );
}



/******************************************************************************
PURPOSE: scaledMaximumTests - Test scaledMaximum routine.
******************************************************************************/

static void scaledMaximumTests() {
  {
    char values[ icount ];
    char values2[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values2[ index ] = values[ index ] = ivalues[ index ];
    }

    scaledMaximumTestsT( values, values2, icount );
  }

  {
    unsigned char values[ icount ];
    unsigned char values2[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values2[ index ] = values[ index ] = ivalues[ index ];
    }

    scaledMaximumTestsT( values, values2, icount );
  }

  {
    short values[ icount ];
    short values2[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values2[ index ] = values[ index ] = ivalues[ index ];
    }

    scaledMaximumTestsT( values, values2, icount );
  }

  {
    unsigned short values[ icount ];
    unsigned short values2[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values2[ index ] = values[ index ] = ivalues[ index ];
    }

    scaledMaximumTestsT( values, values2, icount );
  }

  {
    int values[ icount ];
    int values2[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values2[ index ] = values[ index ] = ivalues[ index ];
    }

    scaledMaximumTestsT( values, values2, icount );
  }

  {
    unsigned int values[ icount ];
    unsigned int values2[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values2[ index ] = values[ index ] = ivalues[ index ];
    }

    scaledMaximumTestsT( values, values2, icount );
  }

  {
    long values[ icount ];
    long values2[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values2[ index ] = values[ index ] = ivalues[ index ];
    }

    scaledMaximumTestsT( values, values2, icount );
  }

  {
    unsigned long values[ icount ];
    unsigned long values2[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values2[ index ] = values[ index ] = ivalues[ index ];
    }

    scaledMaximumTestsT( values, values2, icount );
  }

  {
    long long values[ icount ];
    long long values2[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values2[ index ] = values[ index ] = ivalues[ index ];
    }

    scaledMaximumTestsT( values, values2, icount );
  }

  {
    unsigned long long values[ icount ];
    unsigned long long values2[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values2[ index ] = values[ index ] = ivalues[ index ];
    }

    scaledMaximumTestsT( values, values2, icount );
  }

  {
    float values[ rcount ];
    float values2[ rcount ];

    for ( size_t index = 0; index < rcount; ++index ) {
      values2[ index ] = values[ index ] = rvalues[ index ];
    }

    scaledMaximumTestsT( values, values2, rcount );
  }

  {
    double values[ rcount ];
    double values2[ rcount ];

    for ( size_t index = 0; index < rcount; ++index ) {
      values2[ index ] = values[ index ] = rvalues[ index ];
    }

    scaledMaximumTestsT( values, values2, rcount );
  }
}



/******************************************************************************
PURPOSE: scaledMaximumTestsT - Test scaledMaximum routine with type T.
******************************************************************************/

template <typename T>
static void scaledMaximumTestsT( const T values[], T values2[], const size_t count) {
  const T zero = 0;
  const T one = 1;
  const T two = 2;

  if ( emvl::isNanFree( values, count ) ) {
    const T minimum = emvl::minimum( values, count );

    for ( size_t index1 = 0; index1 < count; ++index1 ) {
      const T scale = values[ index1 ];

      for ( size_t index2 = 0; index2 < count; ++index2 ) {
        const T threshold = values[ index2 ];
        memcpy( values2, values, count * sizeof *values2 );

        if ( AND4( scale, emvl::isFinite( scale ), ! emvl::isNan( scale ),
                   ! emvl::isNan( threshold ) ) ) {
          const T result =
            emvl::scaledMaximum( values2, count, scale, threshold );
          assert( ! emvl::isNan( result ) );
        }
      }
    }

    memcpy( values2, values, count * sizeof *values2 );
    assert( emvl::scaledMaximum( values2, count, one, minimum ) >= minimum );

    for ( size_t index = 0; index < count; ++index ) {
      assert( values2[ index ] == values[ index ] );
    }
  }

  T values1[ 1 ] = { 1 };
  assert( emvl::scaledMaximum( values1, 1, one, zero ) == one );
  assert( values1[ 0 ] == one );
  assert( emvl::scaledMaximum( values1, 1, one, one ) == one );
  assert( values1[ 0 ] == one );
  assert( emvl::scaledMaximum( values1, 1, two, one ) == one );
  assert( values1[ 0 ] == one );
  assert( emvl::scaledMaximum( values1, 1, two, zero ) == two );
  assert( values1[ 0 ] == two );
  T values3[ 3 ] = { 0, 1, 2 };
  assert( emvl::scaledMaximum( values3, 3, one, zero ) == 2 );
  assert( values3[ 0 ] == 0 );
  assert( values3[ 1 ] == 1 );
  assert( values3[ 2 ] == 2 );
  assert( emvl::scaledMaximum( values3, 3, one, two ) == 0 );
  assert( values3[ 0 ] == 0 );
  assert( values3[ 1 ] == 1 );
  assert( values3[ 2 ] == 2 );
  assert( emvl::scaledMaximum( values3, 3, two, two ) == 0 );
  assert( values3[ 0 ] == 0 );
  assert( values3[ 1 ] == 1 );
  assert( values3[ 2 ] == 2 );
  assert( emvl::scaledMaximum( values3, 3, two, one ) == 4 );
  assert( values3[ 0 ] == 0 );
  assert( values3[ 1 ] == 1 );
  assert( values3[ 2 ] == 4 );
}



/******************************************************************************
PURPOSE: swapRowsTests - Test swapRows routine.
******************************************************************************/

static void swapRowsTests() {
  {
    char values[ icount ];
    char values2[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values2[ index ] = values[ index ] = ivalues[ index ];
    }

    for ( size_t rows = 1; rows < icount; ++rows ) {

      if ( icount % rows == 0 ) {
        const size_t columns = icount / rows;
        assert( rows * columns == icount );
        swapRowsTestsT( values, values2, rows, columns );
      }
    }
  }

  {
    unsigned char values[ icount ];
    unsigned char values2[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values2[ index ] = values[ index ] = ivalues[ index ];
    }

    for ( size_t rows = 1; rows < icount; ++rows ) {

      if ( icount % rows == 0 ) {
        const size_t columns = icount / rows;
        assert( rows * columns == icount );
        swapRowsTestsT( values, values2, rows, columns );
      }
    }
  }

  {
    short values[ icount ];
    short values2[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values2[ index ] = values[ index ] = ivalues[ index ];
    }

    for ( size_t rows = 1; rows < icount; ++rows ) {

      if ( icount % rows == 0 ) {
        const size_t columns = icount / rows;
        assert( rows * columns == icount );
        swapRowsTestsT( values, values2, rows, columns );
      }
    }
  }

  {
    unsigned short values[ icount ];
    unsigned short values2[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values2[ index ] = values[ index ] = ivalues[ index ];
    }

    for ( size_t rows = 1; rows < icount; ++rows ) {

      if ( icount % rows == 0 ) {
        const size_t columns = icount / rows;
        assert( rows * columns == icount );
        swapRowsTestsT( values, values2, rows, columns );
      }
    }
  }

  {
    int values[ icount ];
    int values2[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values2[ index ] = values[ index ] = ivalues[ index ];
    }

    for ( size_t rows = 1; rows < icount; ++rows ) {

      if ( icount % rows == 0 ) {
        const size_t columns = icount / rows;
        assert( rows * columns == icount );
        swapRowsTestsT( values, values2, rows, columns );
      }
    }
  }

  {
    unsigned int values[ icount ];
    unsigned int values2[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values2[ index ] = values[ index ] = ivalues[ index ];
    }

    for ( size_t rows = 1; rows < icount; ++rows ) {

      if ( icount % rows == 0 ) {
        const size_t columns = icount / rows;
        assert( rows * columns == icount );
        swapRowsTestsT( values, values2, rows, columns );
      }
    }
  }

  {
    long values[ icount ];
    long values2[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values2[ index ] = values[ index ] = ivalues[ index ];
    }

    for ( size_t rows = 1; rows < icount; ++rows ) {

      if ( icount % rows == 0 ) {
        const size_t columns = icount / rows;
        assert( rows * columns == icount );
        swapRowsTestsT( values, values2, rows, columns );
      }
    }
  }

  {
    unsigned long values[ icount ];
    unsigned long values2[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values2[ index ] = values[ index ] = ivalues[ index ];
    }

    for ( size_t rows = 1; rows < icount; ++rows ) {

      if ( icount % rows == 0 ) {
        const size_t columns = icount / rows;
        assert( rows * columns == icount );
        swapRowsTestsT( values, values2, rows, columns );
      }
    }
  }

  {
    long long values[ icount ];
    long long values2[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values2[ index ] = values[ index ] = ivalues[ index ];
    }

    for ( size_t rows = 1; rows < icount; ++rows ) {

      if ( icount % rows == 0 ) {
        const size_t columns = icount / rows;
        assert( rows * columns == icount );
        swapRowsTestsT( values, values2, rows, columns );
      }
    }
  }

  {
    unsigned long values[ icount ];
    unsigned long values2[ icount ];

    for ( size_t index = 0; index < icount; ++index ) {
      values2[ index ] = values[ index ] = ivalues[ index ];
    }

    for ( size_t rows = 1; rows < icount; ++rows ) {

      if ( icount % rows == 0 ) {
        const size_t columns = icount / rows;
        assert( rows * columns == icount );
        swapRowsTestsT( values, values2, rows, columns );
      }
    }
  }

  {
    float values[ rcount ];
    float values2[ rcount ];

    for ( size_t index = 0; index < rcount; ++index ) {
      values2[ index ] = values[ index ] = rvalues[ index ];
    }

    for ( size_t rows = 1; rows < rcount; ++rows ) {

      if ( rcount % rows == 0 ) {
        const size_t columns = rcount / rows;
        assert( rows * columns == rcount );
        swapRowsTestsT( values, values2, rows, columns );
      }
    }
  }

  {
    double values[ rcount ];
    double values2[ rcount ];

    for ( size_t index = 0; index < rcount; ++index ) {
      values2[ index ] = values[ index ] = rvalues[ index ];
    }

    for ( size_t rows = 1; rows < rcount; ++rows ) {

      if ( rcount % rows == 0 ) {
        const size_t columns = rcount / rows;
        assert( rows * columns == rcount );
        swapRowsTestsT( values, values2, rows, columns );
      }
    }
  }
}



/******************************************************************************
PURPOSE: swapRowsTestsT - Test swapRows routine with type T.
******************************************************************************/

template <typename T>
static void swapRowsTestsT( const T values[], T values2[],
                            const size_t rows, const size_t columns ) {
  const size_t count = rows * columns;
  memcpy( values2, values, rows * columns * sizeof *values );
  emvl::swapRows( values2, rows, columns );

  for ( size_t row = 0; row < rows; ++row ) {
    const size_t row2 = rows - row - 1;
    assert( IN_RANGE( row2, 0, rows - 1 ) );


    for ( size_t column = 0; column < columns; ++column ) {
      const size_t index = row * columns + column;
      const size_t index2 = row2 * columns + column;
      assert( IN_RANGE( index, 0, count - 1 ) );
      assert( IN_RANGE( index2, 0, count - 1 ) );
      const T value  = values[ index ];
      const T value2 = values2[ index2 ];
      assert( IMPLIES( AND2( ! emvl::isNan( value ), ! emvl::isNan( value2 ) ),
                       value == value2 ) );
    }
  }

  emvl::swapRows( values2, rows, columns );

  for ( size_t index = 0; index < count; ++index ) {
    const T value  = values[  index ];
    const T value2 = values2[ index ];
    assert( IMPLIES( AND2( ! emvl::isNan( value ), ! emvl::isNan( value2 ) ),
                     value == value2 ) );
  }

  const T zero = 0;
  const T one = 1;
  const T two = 2;
  const T three = 3;
  T values1[ 1 ] = { one };
  emvl::swapRows( values1, 1, 1 );
  assert( values1[ 0 ] == one );
  T values4[ 4 ] = { zero, one, two, three };
  emvl::swapRows( values4, 2, 2 );
  assert( values4[ 0 ] == two );
  assert( values4[ 1 ] == three );
  assert( values4[ 2 ] == zero );
  assert( values4[ 3 ] == one );
  emvl::swapRows( values4, 2, 2 );
  assert( values4[ 0 ] == zero );
  assert( values4[ 1 ] == one );
  assert( values4[ 2 ] == two );
  assert( values4[ 3 ] == three );
}



/******************************************************************************
PURPOSE: expandBytesTests - Test expandBytes routine.
******************************************************************************/

static void expandBytesTests() {

  {
    const unsigned char* bytes = (const unsigned char*) ivalues;
    const short* values = (const short*) ivalues;
    const size_t count = sizeof ivalues / ( sizeof *values );
    short values2[ count ];
    memcpy( values2, values, sizeof values2 );
    emvl::expandBytes( values2, count );
    assert( emvl::isNanFree( values2, count ) );

    for ( size_t index = 0; index < count; ++index ) {
      const unsigned char byte = bytes[ index ];
      const short vbyte = byte;
      const short value = values2[ index ];
      assert( vbyte >= byte );
      assert( value == vbyte );
    }
  }

  {
    const unsigned char* bytes = (const unsigned char*) ivalues;
    const unsigned short* values = (const unsigned short*) ivalues;
    const size_t count = sizeof ivalues / ( sizeof *values );
    unsigned short values2[ count ];
    memcpy( values2, values, sizeof values2 );
    emvl::expandBytes( values2, count );
    assert( emvl::isNanFree( values2, count ) );

    for ( size_t index = 0; index < count; ++index ) {
      const unsigned char byte = bytes[ index ];
      const unsigned short vbyte = byte;
      const unsigned short value = values2[ index ];
      assert( vbyte >= byte );
      assert( value == vbyte );
    }
  }

  {
    const unsigned char* bytes = (const unsigned char*) ivalues;
    const int* values = (const int*) ivalues;
    const size_t count = sizeof ivalues / ( sizeof *values );
    int values2[ count ];
    memcpy( values2, values, sizeof values2 );
    emvl::expandBytes( values2, count );
    assert( emvl::isNanFree( values2, count ) );

    for ( size_t index = 0; index < count; ++index ) {
      const unsigned char byte = bytes[ index ];
      const int vbyte = byte;
      const int value = values2[ index ];
      assert( vbyte >= byte );
      assert( value == vbyte );
    }
  }

  {
    const unsigned char* bytes = (const unsigned char*) ivalues;
    const unsigned int* values = (const unsigned int*) ivalues;
    const size_t count = sizeof ivalues / ( sizeof *values );
    unsigned int values2[ count ];
    memcpy( values2, values, sizeof values2 );
    emvl::expandBytes( values2, count );
    assert( emvl::isNanFree( values2, count ) );

    for ( size_t index = 0; index < count; ++index ) {
      const unsigned char byte = bytes[ index ];
      const unsigned int vbyte = byte;
      const unsigned int value = values2[ index ];
      assert( vbyte >= byte );
      assert( value == vbyte );
    }
  }

  {
    const unsigned char* bytes = (const unsigned char*) ivalues;
    const long* values = (const long*) ivalues;
    const size_t count = sizeof ivalues / ( sizeof *values );
    long values2[ count ];
    memcpy( values2, values, sizeof values2 );
    emvl::expandBytes( values2, count );
    assert( emvl::isNanFree( values2, count ) );

    for ( size_t index = 0; index < count; ++index ) {
      const unsigned char byte = bytes[ index ];
      const long vbyte = byte;
      const long value = values2[ index ];
      assert( vbyte >= byte );
      assert( value == vbyte );
    }
  }

  {
    const unsigned char* bytes = (const unsigned char*) ivalues;
    const unsigned long* values = (const unsigned long*) ivalues;
    const size_t count = sizeof ivalues / ( sizeof *values );
    unsigned long values2[ count ];
    memcpy( values2, values, sizeof values2 );
    emvl::expandBytes( values2, count );
    assert( emvl::isNanFree( values2, count ) );

    for ( size_t index = 0; index < count; ++index ) {
      const unsigned char byte = bytes[ index ];
      const unsigned long vbyte = byte;
      const unsigned long value = values2[ index ];
      assert( vbyte >= byte );
      assert( value == vbyte );
    }
  }

  {
    const unsigned char* bytes = (const unsigned char*) ivalues;
    const long long* values = (const long long*) ivalues;
    const size_t count = sizeof ivalues / ( sizeof *values );
    long long values2[ count ];
    memcpy( values2, values, sizeof values2 );
    emvl::expandBytes( values2, count );
    assert( emvl::isNanFree( values2, count ) );

    for ( size_t index = 0; index < count; ++index ) {
      const unsigned char byte = bytes[ index ];
      const long long vbyte = byte;
      const long long value = values2[ index ];
      assert( vbyte >= byte );
      assert( value == vbyte );
    }
  }

  {
    const unsigned char* bytes = (const unsigned char*) ivalues;
    const unsigned long long* values = (const unsigned long long*) ivalues;
    const size_t count = sizeof ivalues / ( sizeof *values );
    unsigned long long values2[ count ];
    memcpy( values2, values, sizeof values2 );
    emvl::expandBytes( values2, count );
    assert( emvl::isNanFree( values2, count ) );

    for ( size_t index = 0; index < count; ++index ) {
      const unsigned char byte = bytes[ index ];
      const unsigned long long vbyte = byte;
      const unsigned long long value = values2[ index ];
      assert( vbyte >= byte );
      assert( value == vbyte );
    }
  }

  {
    const unsigned char* bytes = (const unsigned char*) rvalues;
    const float* values = (const float*) rvalues;
    const size_t count = sizeof rvalues / ( sizeof *values );
    float values2[ count ];
    memcpy( values2, values, sizeof values2 );
    emvl::expandBytes( values2, count );

    for ( size_t index = 0; index < count; ++index ) {
      const unsigned char byte = bytes[ index ];
      const float vbyte = byte;
      const float value = values2[ index ];
      assert( IMPLIES( ! emvl::isNan( value ), vbyte >= byte ) );
      assert( IMPLIES( ! emvl::isNan( value ), value == vbyte ) );
    }
  }

  {
    const unsigned char* bytes = (const unsigned char*) rvalues;
    const double* values = (const double*) rvalues;
    const size_t count = sizeof rvalues / ( sizeof *values );
    double values2[ count ];
    memcpy( values2, values, sizeof values2 );
    emvl::expandBytes( values2, count );
    assert( emvl::isNanFree( values2, count ) );

    for ( size_t index = 0; index < count; ++index ) {
      const unsigned char byte = bytes[ index ];
      const double vbyte = byte;
      const double value = values2[ index ];
      assert( IMPLIES( ! emvl::isNan( value ), vbyte >= byte ) );
      assert( IMPLIES( ! emvl::isNan( value ), value == vbyte ) );
    }
  }

}



/******************************************************************************
PURPOSE: isValidArgsTests - Test isValidArgs routine.
******************************************************************************/

static void isValidArgsTests() {

  assert( ! emvl::isValidArgs( 0, 0 ) );
  {
    const char* const args[ 1 ] = { 0 };
    assert( ! emvl::isValidArgs( 0, args ) );
    assert( ! emvl::isValidArgs( 1, args ) );
  }
  {
    const char* const args[ 1 ] = { "" };
    assert( ! emvl::isValidArgs( 0, args ) );
    assert( ! emvl::isValidArgs( 1, args ) );
  }
  {
    const char* const args[ 1 ] = { " " };
    assert( ! emvl::isValidArgs( 0, args ) );
    assert( ! emvl::isValidArgs( 1, args ) );
  }
  {
    const char* const args[ 1 ] = { " a" };
    assert( ! emvl::isValidArgs( 0, args ) );
    assert( ! emvl::isValidArgs( 1, args ) );
  }
  {
    const char* const args[ 1 ] = { "a " };
    assert( ! emvl::isValidArgs( 0, args ) );
    assert( ! emvl::isValidArgs( 1, args ) );
  }
  {
    const char* const args[ 1 ] = { "a" };
    assert( ! emvl::isValidArgs( 0, args ) );
    assert( emvl::isValidArgs( 1, args ) );
  }
  {
    const char* const args[ 1 ] = { "word" };
    assert( ! emvl::isValidArgs( 0, args ) );
    assert( emvl::isValidArgs( 1, args ) );
  }

  for ( size_t index = 0; index < scount0; ++index ) {
    assert( ! emvl::isValidArgs( index - 1, svalues0 ) );
    assert( ! emvl::isValidArgs( scount0 - index, svalues0 ) );
  }

  for ( size_t index = 0; index < scount1; ++index ) {
    assert( emvl::isValidArgs( index + 1, svalues1 ) );
    assert( emvl::isValidArgs( scount1 - index, svalues1 ) );
  }

}



/******************************************************************************
PURPOSE: indexOfStringTests - Test indexOfString routine.
******************************************************************************/

static void indexOfStringTests() {

  {
    const char* const strings[ 1 ] = { "a" };
    const size_t count = sizeof strings / sizeof *strings;
    assert( emvl::indexOfString( *strings, strings, count ) == 0 );
    assert( emvl::indexOfString( "a", strings, count ) == 0 );
    assert( emvl::indexOfString( "aa", strings, count ) == count );
    assert( emvl::indexOfString( "x", strings, count ) == count );
  }

  {
    const char* const strings[ 2 ] = { "a", "aa" };
    const size_t count = sizeof strings / sizeof *strings;
    assert( emvl::indexOfString( strings[ 0 ], strings, count ) == 0 );
    assert( emvl::indexOfString( strings[ 1 ], strings, count ) == 1 );
    assert( emvl::indexOfString( "a", strings, count ) == 0 );
    assert( emvl::indexOfString( "aa", strings, count ) == 1 );
    assert( emvl::indexOfString( "aaa", strings, count ) == count );
    assert( emvl::indexOfString( "x", strings, count ) == count );
  }

  {
    const char* const strings[ 2 ] = { "word", "or" };
    const size_t count = sizeof strings / sizeof *strings;
    assert( emvl::indexOfString( strings[ 0 ], strings, count ) == 0 );
    assert( emvl::indexOfString( strings[ 1 ], strings, count ) == 1 );
    assert( emvl::indexOfString( "word", strings, count ) == 0 );
    assert( emvl::indexOfString( "or", strings, count ) == 1 );
    assert( emvl::indexOfString( "w", strings, count ) == count );
    assert( emvl::indexOfString( "wo", strings, count ) == count );
    assert( emvl::indexOfString( "wor", strings, count ) == count );
    assert( emvl::indexOfString( "work", strings, count ) == count );
    assert( emvl::indexOfString( "o", strings, count ) == count );
    assert( emvl::indexOfString( "r", strings, count ) == count );
    assert( emvl::indexOfString( "oor", strings, count ) == count );
    assert( emvl::indexOfString( "orr", strings, count ) == count );
  }

  {
    const char* const strings[ 1 ] = { "two words" };
    const size_t count = sizeof strings / sizeof *strings;
    assert( emvl::indexOfString( strings[ 0 ], strings, count ) == 0 );
    assert( emvl::indexOfString( "two", strings, count ) == count );
    assert( emvl::indexOfString( "words", strings, count ) == count );
  }

  for ( size_t index = 0; index < scount1; ++index ) {
    assert(emvl::indexOfString(svalues1[ index ], svalues1, scount1) == index);
  }

}



/******************************************************************************
PURPOSE: wordCountTests - Test wordCount routine.
******************************************************************************/

static void wordCountTests() {

  assert( emvl::wordCount( "" ) == 0 );
  assert( emvl::wordCount( " " ) == 0 );
  assert( emvl::wordCount( "\t" ) == 0 );
  assert( emvl::wordCount( "\n" ) == 0 );
  assert( emvl::wordCount( "\r" ) == 0 );
  assert( emvl::wordCount( "  " ) == 0 );

  assert( emvl::wordCount( "a" ) == 1 );
  assert( emvl::wordCount( "one" ) == 1 );
  assert( emvl::wordCount( "two words" ) == 2 );
  assert( emvl::wordCount( " two   words   " ) == 2 );
  assert( emvl::wordCount( "just three words" ) == 3 );

  for ( size_t index = 0; index < scount1; ++index ) {
    assert( emvl::wordCount( svalues1[ index ] ) == 1 );
  }

}



/******************************************************************************
PURPOSE: lineCountTests - Test lineCount routine.
******************************************************************************/

static void lineCountTests() {

  assert( emvl::lineCount( "" ) == 0 );
  assert( emvl::lineCount( " " ) == 0 );
  assert( emvl::lineCount( "\t" ) == 0 );
  assert( emvl::lineCount( "\r" ) == 0 );
  assert( emvl::lineCount( "  " ) == 0 );
  assert( emvl::lineCount( "a" ) == 0 );
  assert( emvl::lineCount( "one" ) == 0 );
  assert( emvl::lineCount( "two words" ) == 0 );
  assert( emvl::lineCount( " two   words   " ) == 0 );
  assert( emvl::lineCount( "just three words" ) == 0 );

  for ( size_t index = 0; index < scount1; ++index ) {
    assert( emvl::lineCount( svalues1[ index ] ) == 0 );
  }

  assert( emvl::lineCount( "\n" ) == 1 );
  assert( emvl::lineCount( " \n" ) == 1 );
  assert( emvl::lineCount( "\n " ) == 1 );
  assert( emvl::lineCount( "\n\r" ) == 1 );
  assert( emvl::lineCount( "one\n" ) == 1 );
  assert( emvl::lineCount( "two words\n" ) == 1 );

  assert( emvl::lineCount( "\n\n" ) == 2 );
  assert( emvl::lineCount( "\n\r\n\r" ) == 2 );
  assert( emvl::lineCount( "\n \n" ) == 2 );
  assert( emvl::lineCount( " \t\n \n\t\t" ) == 2 );
  assert( emvl::lineCount( "two\nlines\n" ) == 2 );
  assert( emvl::lineCount( "two \n lines \n" ) == 2 );

  assert( emvl::lineCount( "has \n three \n lines \n" ) == 3 );
}



/******************************************************************************
PURPOSE: lowercaseTests - Test lowercase routine.
******************************************************************************/

static void lowercaseTests() {

  {
    char string[] = "";
    emvl::lowercase( string );
    assert( *string == '\0' );
  }

  {
    char string[] = " ";
    emvl::lowercase( string );
    assert( *string == ' ' );
  }

  {
    char string[] = " a";
    emvl::lowercase( string );
    assert( *string == ' ' );
    assert( string[ 1 ] == 'a' );
  }

  {
    char string[] = " This Is a Mixed-CASE StRiNg...";
    emvl::lowercase( string );

    for ( size_t index = 0, count = strlen( string ); index < count; ++index) {
      assert( ! isupper( string[ index ] ) );
    }
  }

  for ( size_t index = 0; index < scount1; ++index ) {
    char string[ 256 ] = "";
    memset( string, 0, sizeof string );
    strncpy( string, svalues1[ index ], sizeof string );
    emvl::lowercase( string );

    for ( size_t index = 0, count = strlen( string ); index < count; ++index) {
      assert( ! isupper( string[ index ] ) );
    }
  }

}



/******************************************************************************
PURPOSE: eraseDoubleQuotedCommasTests - Test eraseDoubleQuotedCommas routine.
******************************************************************************/

static void eraseDoubleQuotedCommasTests() {

  {
    char string[] = "";
    emvl::eraseDoubleQuotedCommas( string );
    assert( *string == '\0' );
  }

  {
    char string[] = " ";
    emvl::eraseDoubleQuotedCommas( string );
    assert( *string == ' ' );
  }

  {
    char string[] = " a";
    emvl::eraseDoubleQuotedCommas( string );
    assert( *string == ' ' );
    assert( string[ 1 ] == 'a' );
  }

  {
    char string[] = " this string has one \"comma,quoted\" part.";
    emvl::eraseDoubleQuotedCommas( string );
    assert( ! strchr( string, ',' ) );
  }

  {
    char string[] = " this has 'no, double-quoted commas' in it.";
    emvl::eraseDoubleQuotedCommas( string );
    assert( strchr( string, ',' ) );
  }

  {
    char string[] = " this has no, double-quoted commas in it.";
    emvl::eraseDoubleQuotedCommas( string );
    assert( strchr( string, ',' ) );
  }

  {
    char string[] = " this has \"one, quoted comma\" in it.";
    emvl::eraseDoubleQuotedCommas( string );
    assert( ! strchr( string, ',' ) );
  }

  {
    char string[] = "\"city, state\"";
    emvl::eraseDoubleQuotedCommas( string );
    assert( ! strcmp( string, "\"city  state\"" ) );
  }

  {
    char string[] = "street \"city, state\" zip";
    emvl::eraseDoubleQuotedCommas( string );
    assert( ! strcmp( string, "street \"city  state\" zip" ) );
  }

  for ( size_t index = 0; index < scount1; ++index ) {
    char string[ 256 ] = "";
    memset( string, 0, sizeof string );
    strncpy( string, svalues1[ index ], sizeof string );
    emvl::eraseDoubleQuotedCommas( string );
    assert( ! strchr( string, ',' ) );
  }

}



/******************************************************************************
PURPOSE: substituteWordTests - Test substituteWord routine.
******************************************************************************/

static void substituteWordTests() {

  {
    char output[] = "";
    emvl::substituteWord( "", "this", "that", output );
    assert( *output == '\0' );
  }

  {
    const char input[] = "other";
    char output[ 64 ] = "";
    emvl::substituteWord( input, "this", "that", output );
    assert( ! strcmp( output, input ) );
  }

  {
    const char* const input = "banana";
    char output[ 64 ] = "";
    emvl::substituteWord( input, input, input, output );
    assert( ! strcmp( output, input ) );
  }

  {
    const char* const input = "apple banana carrot";
    char output[ 64 ] = "";
    emvl::substituteWord( input, "apple", "pear", output );
    assert( ! strcmp( output, "pear banana carrot" ) );
  }

  {
    const char* const input = "apple banana carrot";
    char output[ 64 ] = "";
    emvl::substituteWord( input, "banana", "pear", output );
    assert( ! strcmp( output, "apple pear carrot" ) );
  }

  {
    const char* const input = "apple banana carrot";
    char output[ 64 ] = "";
    emvl::substituteWord( input, "carrot", "pear", output );
    assert( ! strcmp( output, "apple banana pear" ) );
  }

  for ( size_t index = 0; index < scount1; ++index ) {
    const char* const input = svalues1[ index ];
    const char* const from = "unmatched";
    const char* const to = "XYZ";
    char output[ 256 ] = "";
    memset( output, 0, sizeof output );
    emvl::substituteWord( input, from, to, output );
    assert( output[ sizeof output / sizeof *output - 1 ] == '\0' );
    assert( ! strcmp( output, input ) );
  }

  for ( size_t index = 0; index < scount1; ++index ) {
    const char* const input = svalues1[ index ];
    const char* const from = svalues1[ index ];
    const char* const to = "XYZ";
    char output[ 256 ] = "";
    memset( output, 0, sizeof output );
    emvl::substituteWord( input, from, to, output );
    assert( output[ sizeof output / sizeof *output - 1 ] == '\0' );
    assert( ! strcmp( output, to ) );
  }

}



/******************************************************************************
PURPOSE: fileExistsTests - Test fileExists routine.
******************************************************************************/

static void fileExistsTests() {
  assert( ! emvl::fileExists( " " ) );
  assert( ! emvl::fileExists( nonExistentFileName ) );
  assert( emvl::fileExists( "." ) );
  assert( emvl::fileExists( testFileName ) );
}



/******************************************************************************
PURPOSE: fileSizeTests - Test fileSize routine.
******************************************************************************/

static void fileSizeTests() {
  assert( emvl::fileSize( " " ) == 0 );
  assert( emvl::fileSize( nonExistentFileName ) == 0 );
  assert( emvl::fileSize( testFileName ) == 46 );
}



/******************************************************************************
PURPOSE: copyFileTests - Test copyFile routine.
******************************************************************************/

static void copyFileTests() {

  unlink( tempFileName );
  bool result = emvl::copyFile( " ", tempFileName );
  assert( ! result );
  assert( ! emvl::fileExists( tempFileName ) );

  result = emvl::copyFile( nonExistentFileName, tempFileName );
  assert( ! result );
  assert( ! emvl::fileExists( tempFileName ) );

  result = emvl::copyFile( testFileName, nonExistentFileName );
  assert( ! result );
  assert( ! emvl::fileExists( nonExistentFileName ) );

  result = emvl::copyFile( testFileName, tempFileName );
  assert( result );
  assert( emvl::fileSize( tempFileName ) == emvl::fileSize( testFileName ) );
  unlink( tempFileName );
}



/******************************************************************************
PURPOSE: copyFileBytesTests - Test copyFileBytes routine.
******************************************************************************/

static void copyFileBytesTests() {

  unlink( tempFileName );
  FILE* file = fopen( testFileName, "rb" ); assert( file );
  bool result = emvl::copyFileBytes( file, tempFileName, 1 );
  fclose( file ), file = 0;
  assert( result );
  assert( emvl::fileSize( tempFileName ) == 1 );
  unlink( tempFileName );

  file = fopen( testFileName, "rb" ); assert( file );
  result =
    emvl::copyFileBytes( file, tempFileName, emvl::fileSize( testFileName ) );
  fclose( file ), file = 0;
  assert( result );
  assert( emvl::fileSize( tempFileName ) > 1 );
  assert( emvl::fileSize( tempFileName ) == emvl::fileSize( testFileName ) );

  // Overwrite existing temp file:

  file = fopen( testFileName, "rb" ); assert( file );
  result = emvl::copyFileBytes( file, tempFileName );
  fclose( file ), file = 0;
  assert( result );
  assert( emvl::fileSize( tempFileName ) > 1 );
  assert( emvl::fileSize( tempFileName ) == emvl::fileSize( testFileName ) );
  unlink( tempFileName );

  file = fopen( testFileName, "rb" ); assert( file );
  result = emvl::copyFileBytes( file, nonExistentFileName, 1 );
  fclose( file ), file = 0;
  assert( ! result );
  assert( ! emvl::fileExists( nonExistentFileName ) );

  file = fopen( emptyFileName, "rb" ); assert( file );
  result = emvl::copyFileBytes( file, tempFileName, 1 );
  fclose( file ), file = 0;
  assert( ! result );
  unlink( tempFileName );
}



/******************************************************************************
PURPOSE: streamFileTests - Test streamFile routine.
******************************************************************************/

static void streamFileTests() {
  bool result = emvl::streamFile( " " );
  assert( ! result );
  result = emvl::streamFile( nonExistentFileName );
  assert( ! result );
  result = emvl::streamFile( testFileName );
  assert( result );
}



/******************************************************************************
PURPOSE: removeAllFilesTests - Test removeAllFiles routine.
******************************************************************************/

static void removeAllFilesTests() {
  emvl::removeAllFiles( testDirectoryName, "xdr nc" );
  emvl::removeAllFiles( testDirectoryName );
  char outputFileName[ 256 ] = "";
  memset( outputFileName, 0, sizeof outputFileName );
  snprintf( outputFileName, sizeof outputFileName / sizeof *outputFileName,
           "%s/junk", testDirectoryName );
  assert(outputFileName[sizeof outputFileName/sizeof *outputFileName-1]=='\0');
  const bool result = emvl::copyFile( testFileName, outputFileName );
  assert( result );
  emvl::removeAllFiles( testDirectoryName );
}




/******************************************************************************
PURPOSE: removeDirectoryTests - Test removeDirectory routine.
******************************************************************************/

static void removeDirectoryTests() {
  int status = MKDIR( tempDirectoryName, 0777 );

  if ( status ) {
    perror( "removeDirectoryTests mkdir problem:" );
    assert( status == 0 );
  }

  // Create several temporary sub-directories and files to be removed:

  for ( int subdir = 1; subdir <= 3; ++subdir ) {
    char outputDirectoryName[ 256 ] = "";
    memset( outputDirectoryName, 0, sizeof outputDirectoryName );
    snprintf( outputDirectoryName,
              sizeof outputDirectoryName / sizeof *outputDirectoryName,
             "%s/junk_sub_dir_%d", tempDirectoryName, subdir );
    assert( outputDirectoryName[ sizeof outputDirectoryName /
                                 sizeof *outputDirectoryName - 1 ] == '\0' );
    status = MKDIR( outputDirectoryName, 0777 );

    if ( status ) {
      perror( "removeDirectoryTests mkdir problem:" );
      assert( status == 0 ); // Stop tester.
    } else {
      char outputFileName[ 256 ] = "";
      memset( outputFileName, 0, sizeof outputFileName );
      snprintf( outputFileName,
                sizeof outputFileName / sizeof *outputFileName,
               "%s/junk_file_%d", outputDirectoryName, subdir );
      assert( outputFileName[ sizeof outputFileName /
                              sizeof *outputFileName - 1 ] == '\0' );
      const bool ok = emvl::copyFile( testFileName, outputFileName );
      assert( ok );
    }
  }

  assert( emvl::isDirectory( tempDirectoryName ) );
  emvl::removeDirectory( tempDirectoryName );
  assert( ! emvl::isDirectory( tempDirectoryName ) );
}



/******************************************************************************
PURPOSE: directoryListingTests - Test directoryListing routine.
******************************************************************************/

static void directoryListingTests() {
  emvl::removeAllFiles( testDirectoryName );
  assert( ! emvl::isDirectory( "/no/such/directory" ) );
  assert( ! emvl::isDirectory( testFileName ) );
  assert( emvl::isDirectory( testDirectoryName ) );
  assert( emvl::isDirectory( std::string( testDirectoryName ) ) );

  std::string name;
  name = "/no/such/file";
  std::cout << "emvl::isNetCDFFile( " << name << " ) = "
    << emvl::isNetCDFFile( name ) << std::endl;
  assert( ! emvl::isNetCDFFile( name ) );
  name = "test/testdata/MET/GRIDCRO2D";
  std::cout << "emvl::isNetCDFFile( " << name << " ) = "
    << emvl::isNetCDFFile( name ) << std::endl;
  assert( emvl::isNetCDFFile( name ) );

  std::cout << "emvl::getHomeDirectory() = "
    << emvl::getHomeDirectory() << std::endl;

  {
    char result[ 1 ] = "";
    const size_t size = sizeof result / sizeof *result;
    emvl::directoryListing( ".", 0, size, result );
    assert( *result == '\0' );
  }

  {
    char result[ 10 ] = "";
    const size_t size = sizeof result / sizeof *result;
    char outputFileName[ 256 ] = "";
    memset( outputFileName, 0, sizeof outputFileName );
    snprintf( outputFileName, sizeof outputFileName / sizeof *outputFileName,
             "%s/junk.txt", testDirectoryName );
    emvl::copyFile( testFileName, tempFileName );
    assert( emvl::fileExists( tempFileName ) );
    emvl::directoryListing( testDirectoryName, 0, size, result );
    assert( emvl::lineCount( result ) == 0 );
  }

  {
    char result[ 1024 ] = "";
    const size_t size = sizeof result / sizeof *result;
    char outputFileName[ 256 ] = "";
    memset( outputFileName, 0, sizeof outputFileName );
    snprintf( outputFileName, sizeof outputFileName / sizeof *outputFileName,
             "%s/junk.txt", testDirectoryName );
    assert(!outputFileName[ sizeof outputFileName / sizeof *outputFileName-1]);
    emvl::copyFile( testFileName, outputFileName );
    assert( emvl::fileExists( outputFileName ) );
    emvl::directoryListing( testDirectoryName, 0, size, result );
    assert( emvl::lineCount( result ) == 1 );
    emvl::removeAllFiles( testDirectoryName );
  }

  {
    char result[ 1024 ] = "";
    const size_t size = sizeof result / sizeof *result;
    char outputFileName[ 256 ] = "";
    memset( outputFileName, 0, sizeof outputFileName );
    const size_t files = 5000; // Number of temp test files to create.

    for ( size_t file = 0; file < files; ++file ) {
      snprintf( outputFileName, sizeof outputFileName / sizeof *outputFileName,
                "%s/junk_%05" LU ".txt", testDirectoryName, file );
      assert(!outputFileName[ sizeof outputFileName/sizeof *outputFileName-1]);
      emvl::copyFile( testFileName, outputFileName );
      assert( emvl::fileExists( outputFileName ) );
    }

    emvl::directoryListing( testDirectoryName, 0, size, result );
    assert( emvl::lineCount( result ) > 10 );
    assert( emvl::lineCount( result ) < files );
    emvl::directoryListing( testDirectoryName, "..", size, result );
    assert( emvl::lineCount( result ) == 0 );
    emvl::directoryListing( testDirectoryName, ".x", size, result );
    assert( emvl::lineCount( result ) == 0 );
    emvl::directoryListing( testDirectoryName, ".txt", size, result );
    assert( emvl::lineCount( result ) > 10 );
    assert( emvl::lineCount( result ) < files );
    emvl::removeAllFiles( testDirectoryName );
  }
}



/******************************************************************************
PURPOSE: testBounds - Test Bounds class.
******************************************************************************/

static void testBounds() {
  printf( "testBounds()\n" );

  {
    emvl::Bounds bounds;
    assert( bounds.west() == -180.0 );
    assert( bounds.east() ==  180.0 );
    assert( bounds.south() == -90.0 );
    assert( bounds.north() ==  90.0 );
    emvl::Bounds bounds2;
    assert( bounds == bounds2 );
    assert( ! ( bounds != bounds2 ) );
    assert( bounds.overlaps( bounds2 ) );
    assert( bounds2.overlaps( bounds ) );
    bounds2.west( bounds2.west() * 0.5 );
    assert( ! ( bounds2 == bounds ) );
    assert( bounds2 != bounds );
    assert( bounds.overlaps( bounds2 ) );
    assert( bounds2.overlaps( bounds ) );
    bounds2.east( bounds2.east() * 0.5 );
    assert( bounds.overlaps( bounds2 ) );
    assert( bounds2.overlaps( bounds ) );
    bounds2.south( bounds2.south() * 0.5 );
    assert( bounds.overlaps( bounds2 ) );
    assert( bounds2.overlaps( bounds ) );
    bounds2.north( bounds2.north() * 0.5 );
    assert( bounds.overlaps( bounds2 ) );
    assert( bounds2.overlaps( bounds ) );
    bounds2 = bounds;
    assert( bounds2 == bounds );
    bounds.west( -150.0 );
    bounds.east( -149.0 );
    bounds.south( 70.0 );
    bounds.north( 71.0 );
    bounds2.west( -90.0 );
    bounds2.east( -65.0 );
    bounds2.south( 20.0 );
    bounds2.north( 40.0 );
    assert( bounds != bounds2 );
    assert( ! bounds.overlaps( bounds2 ) );
    assert( ! bounds2.overlaps( bounds ) );
    bounds.west( -150.0 );
    bounds.east(  -50.0 );
    assert( bounds != bounds2 );
    assert( ! bounds.overlaps( bounds2 ) );
    assert( ! bounds2.overlaps( bounds ) );
    bounds.south( 30.0 );
    bounds.north( 31.0 );
    assert( bounds != bounds2 );
    assert( bounds.overlaps( bounds2 ) );
    assert( bounds2.overlaps( bounds ) );
  }

  for ( size_t index1 = 0; index1 < rcount; ++index1 ) {
    const double west = rvalues[ index1 ];

    if ( AND2( ! emvl::isNan( west ), emvl::isLongitude( west ) ) ) {

      for ( size_t index2 = 0; index2 < rcount; index2 += 7 ) {
        const double east = rvalues[ index2 ];

        if ( AND3( ! emvl::isNan( east ), emvl::isLatitude( east ),
                   west <= east ) ) {

          for ( size_t index3 = 0; index3 < rcount; index3 += 5 ) {
            const double south = rvalues[ index3 ];

            if ( AND2( ! emvl::isNan( south ), emvl::isLatitude( south ) ) ) {

              for ( size_t index4 = 0; index4 < rcount; ++index4 ) {
                const double north = rvalues[ index4 ];

                if ( AND3( ! emvl::isNan( north ), emvl::isLatitude( north ),
                           south <= north ) ) {
                  emvl::Bounds bounds1;
                  emvl::Bounds bounds2( bounds1 );
                  assert( bounds1 == bounds2 );
                  emvl::Bounds bounds3( west, east, south, north );
                  assert( IMPLIES_ELSE( bounds3 == bounds1,
                                        AND7( bounds3 == bounds2,
                                              ! ( bounds3 != bounds2 ),
                                              ! ( bounds3 != bounds1 ),
                                              bounds3.overlaps( bounds1 ),
                                              bounds1.overlaps( bounds3 ),
                                              bounds3.overlaps( bounds2 ),
                                              bounds2.overlaps( bounds3 ) ),
                                        AND3( bounds3 != bounds2,
                                              ! ( bounds3 == bounds2 ),
                                              ! ( bounds3 == bounds1 ) ) ) );
                  bounds2.west( bounds2.west() * 0.99 );
                  bounds2.west( bounds2.east() * 0.99 );
                  bounds2.west( bounds2.south() * 0.99 );
                  bounds2.west( bounds2.north() * 0.99 );
                  assert( bounds2.overlaps( bounds1 ) );
                  assert( bounds1.overlaps( bounds2 ) );
                }
              }
            }
          }
        }
      }
    }
  }
}



/******************************************************************************
PURPOSE: testTimestamp - Test Timestamp class.
******************************************************************************/

static void testTimestamp() {
  printf( "testTimestamp()\n" );

  assert( emvl::Timestamp::isYYYYDDD( 2012001 ) );
  assert( emvl::Timestamp::isYYYYDDD( 2012366 ) );
  assert( ! emvl::Timestamp::isYYYYDDD( 1012366 ) );
  assert( ! emvl::Timestamp::isYYYYDDD( 2012000 ) );
  assert( ! emvl::Timestamp::isYYYYDDD( 2012367 ) );

  assert( emvl::Timestamp::isYYYYMMDD( 20120101 ) );
  assert( emvl::Timestamp::isYYYYMMDD( 20121231 ) );
  assert( ! emvl::Timestamp::isYYYYMMDD( -1 ) );
  assert( ! emvl::Timestamp::isYYYYMMDD( 10121231 ) );
  assert( ! emvl::Timestamp::isYYYYMMDD( 20120031 ) );
  assert( ! emvl::Timestamp::isYYYYMMDD( 20121331 ) );
  assert( ! emvl::Timestamp::isYYYYMMDD( 20121300 ) );
  assert( ! emvl::Timestamp::isYYYYMMDD( 20121332 ) );

  assert( emvl::Timestamp::isHHMMSS( 0 ) );
  assert( emvl::Timestamp::isHHMMSS( 59 ) );
  assert( emvl::Timestamp::isHHMMSS( 100 ) );
  assert( emvl::Timestamp::isHHMMSS( 5900 ) );
  assert( emvl::Timestamp::isHHMMSS( 10000 ) );
  assert( emvl::Timestamp::isHHMMSS( 235959 ) );
  assert( ! emvl::Timestamp::isHHMMSS( -1 ) );
  assert( ! emvl::Timestamp::isHHMMSS( 60 ) );
  assert( ! emvl::Timestamp::isHHMMSS( 6059 ) );
  assert( ! emvl::Timestamp::isHHMMSS( 245959 ) );

  assert( emvl::Timestamp::isUTCString( "2013-12-31T23:59:59-0000" ) );
  assert( emvl::Timestamp::isUTCString( "2013-12-31T23:59:59+0000" ) );
  assert( emvl::Timestamp::isUTCString( "2013-12-31T23:59:59-0001" ) );
  assert( emvl::Timestamp::isUTCString( "2013-12-31T23:59:59+0001" ) );
  assert( emvl::Timestamp::isUTCString( "2013-12-31T23:59:59-2359" ) );
  assert( emvl::Timestamp::isUTCString( "2013-12-31T23:59:59+2359" ) );
  assert( ! emvl::Timestamp::isUTCString( "1013-12-31T23:59:59-0000" ) );
  assert( ! emvl::Timestamp::isUTCString( "2013-00-31T23:59:59-0000" ) );
  assert( ! emvl::Timestamp::isUTCString( "2013-13-31T23:59:59-0000" ) );
  assert( ! emvl::Timestamp::isUTCString( "2013-12-00T23:59:59-0000" ) );
  assert( ! emvl::Timestamp::isUTCString( "2013-12-32T23:59:59-0000" ) );
  assert( ! emvl::Timestamp::isUTCString( "2013-12-31T24:59:59-0000" ) );
  assert( ! emvl::Timestamp::isUTCString( "2013-12-31T23:60:59-0000" ) );
  assert( ! emvl::Timestamp::isUTCString( "2013-12-31T23:59:60-0000" ) );

  assert( emvl::Timestamp::fileDateUTC( nonExistentFileName ) == 19000101 );
  assert( emvl::Timestamp::fileDateUTC( emptyFileName ) == 20130127 );

  assert( emvl::Timestamp::timeZoneOffset( " " ) == 0 );
  assert( emvl::Timestamp::timeZoneOffset( "AST" ) == -4 );
  assert( emvl::Timestamp::timeZoneOffset( "EST" ) == -5 );
  assert( emvl::Timestamp::timeZoneOffset( "EDT" ) == -4 );
  assert( emvl::Timestamp::timeZoneOffset( "CST" ) == -6 );
  assert( emvl::Timestamp::timeZoneOffset( "CDT" ) == -5 );
  assert( emvl::Timestamp::timeZoneOffset( "MST" ) == -7 );
  assert( emvl::Timestamp::timeZoneOffset( "MDT" ) == -6 );
  assert( emvl::Timestamp::timeZoneOffset( "PST" ) == -8 );
  assert( emvl::Timestamp::timeZoneOffset( "PDT" ) == -7 );
  assert( emvl::Timestamp::timeZoneOffset( "AKST" ) == -9 );
  assert( emvl::Timestamp::timeZoneOffset( "AKDT" ) == -8 );
  assert( emvl::Timestamp::timeZoneOffset( "HAST" ) == -10 );
  assert( emvl::Timestamp::timeZoneOffset( "HASDT" ) == -9 );

  {
    emvl::Timestamp::UTCString utc = "";
    emvl::Timestamp::UTCString utc2 = "";
    emvl::Timestamp timestamp;
    assert( time( 0 ) - timestamp.seconds() <= 1 );

    emvl::Timestamp timestamp2( timestamp );
    assert( timestamp == timestamp2 );
    assert( ! ( timestamp != timestamp2 ) );
    assert( timestamp <= timestamp2 );
    assert( timestamp >= timestamp2 );
    assert( ! ( timestamp < timestamp2 ) );
    assert( ! ( timestamp > timestamp2 ) );
    assert( timestamp.seconds() == timestamp2.seconds() );
    assert( timestamp.yyyyddd() == timestamp2.yyyyddd() );
    assert( timestamp.yyyymmdd() == timestamp2.yyyymmdd() );
    assert( timestamp.hhmmss() == timestamp2.hhmmss() );
    assert( timestamp.sameYear( timestamp2 ) );
    assert( timestamp.sameMonth( timestamp2 ) );
    assert( timestamp.sameDay( timestamp2 ) );
    assert( timestamp.sameHour( timestamp2 ) );
    timestamp.toUTCString( utc );
    timestamp2.toUTCString( utc2 );
    assert( ! strcmp( utc, utc2 ) );

    timestamp2 = timestamp;
    assert( timestamp == timestamp2 );
    assert( ! ( timestamp != timestamp2 ) );
    assert( timestamp <= timestamp2 );
    assert( timestamp >= timestamp2 );
    assert( ! ( timestamp < timestamp2 ) );
    assert( ! ( timestamp > timestamp2 ) );
    assert( timestamp.seconds() == timestamp2.seconds() );
    assert( timestamp.yyyyddd() == timestamp2.yyyyddd() );
    assert( timestamp.yyyymmdd() == timestamp2.yyyymmdd() );
    assert( timestamp.hhmmss() == timestamp2.hhmmss() );
    assert( timestamp.sameYear( timestamp2 ) );
    assert( timestamp.sameMonth( timestamp2 ) );
    assert( timestamp.sameDay( timestamp2 ) );
    assert( timestamp.sameHour( timestamp2 ) );
    timestamp.toUTCString( utc );
    timestamp2.toUTCString( utc2 );
    assert( ! strcmp( utc, utc2 ) );

    timestamp2.offsetHours( 1 );
    assert( timestamp != timestamp2 );
    assert( ! ( timestamp == timestamp2 ) );
    assert( timestamp <= timestamp2 );
    assert( timestamp2 >= timestamp );
    assert( timestamp < timestamp2 );
    assert( timestamp2 > timestamp );
    assert( timestamp2.seconds() - timestamp.seconds() == 3600 );
    assert( timestamp2.yyyyddd() >= timestamp.yyyyddd() );
    assert( timestamp2.yyyymmdd() >= timestamp.yyyymmdd() );
    assert( timestamp2.hhmmss() != timestamp.hhmmss() );
    assert( IMPLIES( timestamp2.yyyymmdd() == timestamp.yyyymmdd(),
                     timestamp2.sameYear( timestamp ) ) );
    assert( IMPLIES( timestamp2.yyyymmdd() == timestamp.yyyymmdd(),
                          timestamp2.sameMonth( timestamp ) ) );
    assert( IMPLIES( timestamp2.yyyymmdd() == timestamp.yyyymmdd(),
                     timestamp2.sameDay( timestamp ) ) );
    assert( ! timestamp2.sameHour( timestamp ) );
    timestamp.toUTCString( utc );
    timestamp2.toUTCString( utc2 );
    assert( strcmp( utc, utc2 ) );

    timestamp2.fromUTCString( "2006-07-03T23:59:59-2359" );
    assert( timestamp2 != timestamp );
    assert( ! ( timestamp2 == timestamp ) );
    assert( timestamp2 <= timestamp );
    assert( ! ( timestamp2 >= timestamp ) );
    assert( timestamp2 < timestamp );
    assert( timestamp > timestamp2 );
    assert( timestamp2.yyyyddd() / 1000 == 2006 );
    assert( timestamp2.yyyymmdd() / 10000 == 2006 );
    assert( timestamp2.yyyymmdd() / 100 % 100 == 7 );
    assert( ! timestamp2.sameYear( timestamp ) );
    assert( ! timestamp2.sameMonth( timestamp ) );
    assert( ! timestamp2.sameDay( timestamp ) );
    assert( ! timestamp2.sameHour( timestamp ) );
    timestamp2.toUTCString( utc2 );

    const emvl::Timestamp t1( utc );
    t1.toUTCString( utc2 );
    assert( ! strcmp( utc, utc2 ) );

    const emvl::Timestamp t2( 20060703, 235959 );
    assert( t2.yyyymmdd() == 20060703 );
    assert( t2.hhmmss() == 235959 );

    timestamp2.fromUTCString( "2006-07-03T23:59:59+2359" );
    assert( timestamp2 != timestamp );
    assert( ! ( timestamp2 == timestamp ) );
    assert( timestamp2 <= timestamp );
    assert( ! ( timestamp2 >= timestamp ) );
    assert( timestamp2 < timestamp );
    assert( timestamp > timestamp2 );
    assert( timestamp2.yyyyddd() / 1000 == 2006 );
    assert( timestamp2.yyyymmdd() / 10000 == 2006 );
    assert( timestamp2.yyyymmdd() / 100 % 100 == 7 );
    assert( ! timestamp2.sameYear( timestamp ) );
    assert( ! timestamp2.sameMonth( timestamp ) );
    assert( ! timestamp2.sameDay( timestamp ) );
    assert( ! timestamp2.sameHour( timestamp ) );

    timestamp2.fromUTCString( utc );
    assert( timestamp == timestamp2 );
    assert( ! ( timestamp != timestamp2 ) );
    assert( timestamp <= timestamp2 );
    assert( timestamp >= timestamp2 );
    assert( ! ( timestamp < timestamp2 ) );
    assert( ! ( timestamp > timestamp2 ) );
    assert( timestamp.seconds() == timestamp2.seconds() );
    assert( timestamp.yyyyddd() == timestamp2.yyyyddd() );
    assert( timestamp.yyyymmdd() == timestamp2.yyyymmdd() );
    assert( timestamp.hhmmss() == timestamp2.hhmmss() );
    assert( timestamp.sameYear( timestamp2 ) );
    assert( timestamp.sameMonth( timestamp2 ) );
    assert( timestamp.sameDay( timestamp2 ) );
    assert( timestamp.sameHour( timestamp2 ) );
    timestamp.toUTCString( utc );
    timestamp2.toUTCString( utc2 );
    assert( ! strcmp( utc, utc2 ) );

    timestamp2.offsetHours( 24 );
    assert( timestamp != timestamp2 );
    assert( ! ( timestamp == timestamp2 ) );
    assert( timestamp <= timestamp2 );
    assert( timestamp2 >= timestamp );
    assert( timestamp < timestamp2 );
    assert( timestamp2 > timestamp );
    assert( timestamp2.seconds() - timestamp.seconds() == 86400 );
    assert( timestamp2.yyyyddd() > timestamp.yyyyddd() );
    assert( timestamp2.yyyymmdd() > timestamp.yyyymmdd() );
    assert( timestamp2.hhmmss() == timestamp.hhmmss() );
    assert( IMPLIES_ELSE( timestamp2.yyyymmdd() / 10000 ==
                            timestamp.yyyymmdd() / 10000,
                          timestamp2.sameYear( timestamp ),
                          ! timestamp2.sameYear( timestamp ) ) );
    assert( IMPLIES_ELSE( timestamp2.yyyymmdd() / 100 % 100 ==
                            timestamp.yyyymmdd() / 100 % 100,
                          timestamp2.sameMonth( timestamp ),
                          ! timestamp2.sameMonth( timestamp ) ) );
    assert( ! timestamp2.sameDay( timestamp ) );
    assert( ! timestamp2.sameHour( timestamp ) );
    timestamp.toUTCString( utc );
    timestamp2.toUTCString( utc2 );
    assert( strcmp( utc, utc2 ) );


    timestamp2 = timestamp;
    timestamp2.offsetHours( 24 * 7 );
    assert( timestamp != timestamp2 );
    assert( ! ( timestamp == timestamp2 ) );
    assert( timestamp <= timestamp2 );
    assert( timestamp2 >= timestamp );
    assert( timestamp < timestamp2 );
    assert( timestamp2 > timestamp );
    assert( timestamp2.seconds() - timestamp.seconds() == 604800 );
    assert( IMPLIES( timestamp2.yyyyddd() / 1000 == timestamp.yyyyddd() / 1000,
                     timestamp2.yyyyddd() - timestamp.yyyyddd() == 7 ) );
    assert( timestamp2.yyyymmdd() >= timestamp.yyyymmdd() );
    assert( timestamp2.hhmmss() == timestamp.hhmmss() );
    assert( IMPLIES_ELSE( timestamp2.yyyymmdd() / 10000 ==
                            timestamp.yyyymmdd() / 10000,
                          timestamp2.sameYear( timestamp ),
                          ! timestamp2.sameYear( timestamp ) ) );
    assert( IMPLIES_ELSE( timestamp2.yyyymmdd() / 100 % 100 ==
                            timestamp.yyyymmdd() / 100 % 100,
                          timestamp2.sameMonth( timestamp ),
                          ! timestamp2.sameMonth( timestamp ) ) );
    assert( ! timestamp2.sameDay( timestamp ) );
    assert( ! timestamp2.sameHour( timestamp ) );
    timestamp.toUTCString( utc );
    timestamp2.toUTCString( utc2 );
    assert( strcmp( utc, utc2 ) );

    timestamp2.yyyydddhhmmss( 2012366, 235959 );
    assert( ! ( timestamp2 == timestamp ) );
    assert( timestamp2 != timestamp );
    assert( timestamp2 <= timestamp );
    assert( timestamp >= timestamp2 );
    assert( timestamp2 < timestamp );
    assert( timestamp > timestamp2 );
    assert( timestamp2.seconds() > 10000000 );
    assert( timestamp2.yyyyddd() == 2012366 );
    assert( timestamp2.yyyymmdd() == 20121231 );
    assert( timestamp2.hhmmss() == 235959 );
    assert( ! timestamp2.sameYear( timestamp ) );
    assert( ! timestamp2.sameMonth( timestamp ) );
    assert( ! timestamp2.sameDay( timestamp ) );
    assert( ! timestamp2.sameHour( timestamp ) );
    timestamp.toUTCString( utc );
    timestamp2.toUTCString( utc2 );
    assert( strcmp( utc, utc2 ) );

    timestamp2.yyyydddhhmmss( timestamp.yyyyddd(), timestamp.hhmmss() );
    assert( timestamp == timestamp2 );
    assert( ! ( timestamp != timestamp2 ) );
    assert( timestamp <= timestamp2 );
    assert( timestamp >= timestamp2 );
    assert( ! ( timestamp < timestamp2 ) );
    assert( ! ( timestamp > timestamp2 ) );
    assert( timestamp.seconds() == timestamp2.seconds() );
    assert( timestamp.yyyyddd() == timestamp2.yyyyddd() );
    assert( timestamp.yyyymmdd() == timestamp2.yyyymmdd() );
    assert( timestamp.hhmmss() == timestamp2.hhmmss() );
    assert( timestamp.sameYear( timestamp2 ) );
    assert( timestamp.sameMonth( timestamp2 ) );
    assert( timestamp.sameDay( timestamp2 ) );
    assert( timestamp.sameHour( timestamp2 ) );
    timestamp.toUTCString( utc );
    timestamp2.toUTCString( utc2 );
    assert( ! strcmp( utc, utc2 ) );

    timestamp2.yyyymmddhhmmss( 20121231, 235959 );
    assert( ! ( timestamp2 == timestamp ) );
    assert( timestamp2 != timestamp );
    assert( timestamp2 <= timestamp );
    assert( timestamp >= timestamp2 );
    assert( timestamp2 < timestamp );
    assert( timestamp > timestamp2 );
    assert( timestamp2.seconds() > 0 );
    assert( timestamp2.yyyyddd() == 2012366 );
    assert( timestamp2.yyyymmdd() == 20121231 );
    assert( timestamp2.hhmmss() == 235959 );
    assert( ! timestamp2.sameYear( timestamp ) );
    assert( ! timestamp2.sameMonth( timestamp ) );
    assert( ! timestamp2.sameDay( timestamp ) );
    assert( ! timestamp2.sameHour( timestamp ) );
    timestamp.toUTCString( utc );
    timestamp2.toUTCString( utc2 );
    assert( strcmp( utc, utc2 ) );

    timestamp2.yyyymmddhhmmss( timestamp.yyyymmdd(), timestamp.hhmmss() );
    assert( timestamp == timestamp2 );
    assert( ! ( timestamp != timestamp2 ) );
    assert( timestamp <= timestamp2 );
    assert( timestamp >= timestamp2 );
    assert( ! ( timestamp < timestamp2 ) );
    assert( ! ( timestamp > timestamp2 ) );
    assert( timestamp.seconds() == timestamp2.seconds() );
    assert( timestamp.yyyyddd() == timestamp2.yyyyddd() );
    assert( timestamp.yyyymmdd() == timestamp2.yyyymmdd() );
    assert( timestamp.hhmmss() == timestamp2.hhmmss() );
    assert( timestamp.sameYear( timestamp2 ) );
    assert( timestamp.sameMonth( timestamp2 ) );
    assert( timestamp.sameDay( timestamp2 ) );
    assert( timestamp.sameHour( timestamp2 ) );
    timestamp.toUTCString( utc );
    timestamp2.toUTCString( utc2 );
    assert( ! strcmp( utc, utc2 ) );

    timestamp2.fromScanStartTime( 429006049.3968 );
    timestamp2.toUTCString( utc2 );
    assert( ! strcmp( utc2, "2006-08-06T08:20:43-0000" ) );
    assert( timestamp2.yyyymmdd() == 20060806 );
    assert( timestamp2.yyyyddd() == 2006218 );
    assert( timestamp2.hhmmss() == 82043 );
  }
}



/******************************************************************************
PURPOSE: testMapProjection - Test MapProjection class.
******************************************************************************/

static void testMapProjection() {
  printf( "testMapProjection()\n" );

  assert( emvl::aboutEqual( PI_OVER_2, M_PI * 0.5 ) );
  assert( emvl::aboutEqual( PI_OVER_4, M_PI * 0.25 ) );
  assert( SQUARE( 0.0 ) == 0.0 );
  assert( SQUARE( -1.0 ) == 1.0 );
  assert( SQUARE( -10.0 ) == 100.0 );
  assert( SQUARE( 1.0 ) == 1.0 );
  assert( SQUARE( 10.0 ) == 100.0 );

  assert( emvl::MapProjection::MAP_PROJECTION_TOLERANCE > 0.0 );
  assert( emvl::MapProjection::MAP_PROJECTION_TOLERANCE < 1.0 );

  assert( emvl::MapProjection::MAP_PROJECTION_CONVERGENCE_TOLERANCE > 0.0 );
  assert( emvl::MapProjection::MAP_PROJECTION_CONVERGENCE_TOLERANCE < 1.0 );

  assert(emvl::MapProjection::MAP_PROJECTION_CONVERGENCE_MAXIMUM_ITERATIONS>0);
  assert( emvl::MapProjection::MAP_PROJECTION_CONVERGENCE_MAXIMUM_ITERATIONS
          < 100 );

  assert( emvl::MapProjection::isSpheroid(
            emvl::MapProjection::AIRY_1830_MAJOR_SEMIAXIS,
            emvl::MapProjection::AIRY_1830_MINOR_SEMIAXIS ) );

  assert( emvl::MapProjection::isSpheroid(
            emvl::MapProjection::MODIFIED_AIRY_MAJOR_SEMIAXIS,
            emvl::MapProjection::MODIFIED_AIRY_MINOR_SEMIAXIS ) );

  assert( emvl::MapProjection::isSpheroid(
            emvl::MapProjection::ANDRAE_1876_MAJOR_SEMIAXIS,
            emvl::MapProjection::ANDRAE_1876_MINOR_SEMIAXIS ) );

  assert( emvl::MapProjection::isSpheroid(
            emvl::MapProjection::APPLIED_PHYSICS_1965_MAJOR_SEMIAXIS,
            emvl::MapProjection::APPLIED_PHYSICS_1965_MINOR_SEMIAXIS ) );

  assert( emvl::MapProjection::isSpheroid(
            emvl::MapProjection::AUSTRALIAN_NATL_SA_1969_MAJOR_SEMIAXIS,
            emvl::MapProjection::AUSTRALIAN_NATL_SA_1969_MINOR_SEMIAXIS ) );

  assert( emvl::MapProjection::isSpheroid(
            emvl::MapProjection::BESSEL_1841_MAJOR_SEMIAXIS,
            emvl::MapProjection::BESSEL_1841_MINOR_SEMIAXIS ) );

  assert( emvl::MapProjection::isSpheroid(
            emvl::MapProjection::BESSEL_NAMIBIA_1841_MAJOR_SEMIAXIS,
            emvl::MapProjection::BESSEL_NAMIBIA_1841_MINOR_SEMIAXIS ) );

  assert( emvl::MapProjection::isSpheroid(
            emvl::MapProjection::CLARKE_1866_MAJOR_SEMIAXIS,
            emvl::MapProjection::CLARKE_1866_MINOR_SEMIAXIS ) );

  assert( emvl::MapProjection::isSpheroid(
            emvl::MapProjection::CLARKE_1880_MAJOR_SEMIAXIS,
            emvl::MapProjection::CLARKE_1880_MINOR_SEMIAXIS ) );

  assert( emvl::MapProjection::isSpheroid(
            emvl::MapProjection::COMM_DES_POIDS_ET_MESURES_1799_MAJOR_SEMIAXIS,
          emvl::MapProjection::COMM_DES_POIDS_ET_MESURES_1799_MINOR_SEMIAXIS));

  assert( emvl::MapProjection::isSpheroid(
            emvl::MapProjection::DELAMBRE_1810_BELGIUM_MAJOR_SEMIAXIS,
            emvl::MapProjection::DELAMBRE_1810_BELGIUM_MINOR_SEMIAXIS ) );

  assert( emvl::MapProjection::isSpheroid(
            emvl::MapProjection::ENGELIS_1985_MAJOR_SEMIAXIS,
            emvl::MapProjection::ENGELIS_1985_MINOR_SEMIAXIS ) );

  assert( emvl::MapProjection::isSpheroid(
            emvl::MapProjection::EVEREST_1830_MAJOR_SEMIAXIS,
            emvl::MapProjection::EVEREST_1830_MINOR_SEMIAXIS ) );

  assert( emvl::MapProjection::isSpheroid(
            emvl::MapProjection::EVEREST_1948_MAJOR_SEMIAXIS,
            emvl::MapProjection::EVEREST_1948_MINOR_SEMIAXIS ) );

  assert( emvl::MapProjection::isSpheroid(
            emvl::MapProjection::EVEREST_1956_MAJOR_SEMIAXIS,
            emvl::MapProjection::EVEREST_1956_MINOR_SEMIAXIS ) );

  assert( emvl::MapProjection::isSpheroid(
            emvl::MapProjection::EVEREST_1969_MAJOR_SEMIAXIS,
            emvl::MapProjection::EVEREST_1969_MINOR_SEMIAXIS ) );

  assert( emvl::MapProjection::isSpheroid(
            emvl::MapProjection::EVEREST_SABAH_SARAWAK_MAJOR_SEMIAXIS,
            emvl::MapProjection::EVEREST_SABAH_SARAWAK_MINOR_SEMIAXIS ) );

  assert( emvl::MapProjection::isSpheroid(
            emvl::MapProjection::FISCHER_MERCURY_DATUM_1960_MAJOR_SEMIAXIS,
            emvl::MapProjection::FISCHER_MERCURY_DATUM_1960_MINOR_SEMIAXIS ) );

  assert( emvl::MapProjection::isSpheroid(
            emvl::MapProjection::MODIFIED_FISCHER_1960_MAJOR_SEMIAXIS,
            emvl::MapProjection::MODIFIED_FISCHER_1960_MINOR_SEMIAXIS ) );

  assert( emvl::MapProjection::isSpheroid(
            emvl::MapProjection::FISCHER_1968_MAJOR_SEMIAXIS,
            emvl::MapProjection::FISCHER_1968_MINOR_SEMIAXIS ) );

  assert( emvl::MapProjection::isSpheroid(
            emvl::MapProjection::GRS_IUGG_1967_MAJOR_SEMIAXIS,
            emvl::MapProjection::GRS_IUGG_1967_MINOR_SEMIAXIS ) );

  assert( emvl::MapProjection::isSpheroid(
            emvl::MapProjection::GRS_IUGG_1980_MAJOR_SEMIAXIS,
            emvl::MapProjection::GRS_IUGG_1980_MINOR_SEMIAXIS ) );

  assert( emvl::MapProjection::isSpheroid(
            emvl::MapProjection::HELMERT_1906_MAJOR_SEMIAXIS,
            emvl::MapProjection::HELMERT_1906_MINOR_SEMIAXIS ) );

  assert( emvl::MapProjection::isSpheroid(
            emvl::MapProjection::HOUGH_MAJOR_SEMIAXIS,
            emvl::MapProjection::HOUGH_MINOR_SEMIAXIS ) );

  assert( emvl::MapProjection::isSpheroid(
            emvl::MapProjection::IAU_1976_MAJOR_SEMIAXIS,
            emvl::MapProjection::IAU_1976_MINOR_SEMIAXIS ) );

  assert( emvl::MapProjection::isSpheroid(
            emvl::MapProjection::INTL_HAYFORD_1909_MAJOR_SEMIAXIS,
            emvl::MapProjection::INTL_HAYFORD_1909_MINOR_SEMIAXIS ) );

  assert( emvl::MapProjection::isSpheroid(
            emvl::MapProjection::KRASSOVSKY_1942_MAJOR_SEMIAXIS,
            emvl::MapProjection::KRASSOVSKY_1942_MINOR_SEMIAXIS ) );

  assert( emvl::MapProjection::isSpheroid(
            emvl::MapProjection::KAULA_1961_MAJOR_SEMIAXIS,
            emvl::MapProjection::KAULA_1961_MINOR_SEMIAXIS ) );

  assert( emvl::MapProjection::isSpheroid(
            emvl::MapProjection::LERCH_1979_MAJOR_SEMIAXIS,
            emvl::MapProjection::LERCH_1979_MINOR_SEMIAXIS ) );

  assert( emvl::MapProjection::isSpheroid(
            emvl::MapProjection::MAUPERTIUS_1738_MAJOR_SEMIAXIS,
            emvl::MapProjection::MAUPERTIUS_1738_MINOR_SEMIAXIS ) );

  assert( emvl::MapProjection::isSpheroid(
            emvl::MapProjection::MERIT_1983_MAJOR_SEMIAXIS,
            emvl::MapProjection::MERIT_1983_MINOR_SEMIAXIS ) );

  assert( emvl::MapProjection::isSpheroid(
            emvl::MapProjection::NAVAL_WEAPONS_LAB_1965_MAJOR_SEMIAXIS,
            emvl::MapProjection::NAVAL_WEAPONS_LAB_1965_MINOR_SEMIAXIS ) );

  assert( emvl::MapProjection::isSpheroid(
            emvl::MapProjection::NEW_INTERNATIONAL_1967_MAJOR_SEMIAXIS,
            emvl::MapProjection::NEW_INTERNATIONAL_1967_MINOR_SEMIAXIS ) );

  assert( emvl::MapProjection::isSpheroid(
            emvl::MapProjection::PLESSIS_1817_MAJOR_SEMIAXIS,
            emvl::MapProjection::PLESSIS_1817_MINOR_SEMIAXIS ) );

  assert( emvl::MapProjection::isSpheroid(
            emvl::MapProjection::SGS_1985_MAJOR_SEMIAXIS,
            emvl::MapProjection::SGS_1985_MINOR_SEMIAXIS ) );

  assert( emvl::MapProjection::isSpheroid(
            emvl::MapProjection::SOUTHEAST_ASIA_MAJOR_SEMIAXIS,
            emvl::MapProjection::SOUTHEAST_ASIA_MINOR_SEMIAXIS ) );

  assert( emvl::MapProjection::isSpheroid(
            emvl::MapProjection::WALBECK_MAJOR_SEMIAXIS,
            emvl::MapProjection::WALBECK_MINOR_SEMIAXIS ) );

  assert( emvl::MapProjection::isSpheroid(
            emvl::MapProjection::WGS_1960_MAJOR_SEMIAXIS,
            emvl::MapProjection::WGS_1960_MINOR_SEMIAXIS ) );

  assert( emvl::MapProjection::isSpheroid(
            emvl::MapProjection::WGS_1966_MAJOR_SEMIAXIS,
            emvl::MapProjection::WGS_1966_MINOR_SEMIAXIS ) );

  assert( emvl::MapProjection::isSpheroid(
            emvl::MapProjection::WGS_1972_MAJOR_SEMIAXIS,
            emvl::MapProjection::WGS_1972_MINOR_SEMIAXIS ) );

  assert( emvl::MapProjection::isSpheroid(
            emvl::MapProjection::WGS_1984_MAJOR_SEMIAXIS,
            emvl::MapProjection::WGS_1984_MINOR_SEMIAXIS ) );

  assert( emvl::MapProjection::isSpheroid(
            emvl::MapProjection::MM5_RADIUS,
            emvl::MapProjection::MM5_RADIUS ) );

  assert( emvl::MapProjection::isSpheroid(
            emvl::MapProjection::CMAQ_RADIUS,
            emvl::MapProjection::CMAQ_RADIUS ) );

  assert( emvl::MapProjection::isSpheroid(
            emvl::MapProjection::MCIDAS_RADIUS,
            emvl::MapProjection::MCIDAS_RADIUS ) );

  assert( emvl::MapProjection::isSpheroid(
            emvl::MapProjection::MARS_MAJOR_SEMIAXIS,
            emvl::MapProjection::MARS_MINOR_SEMIAXIS ) );

  assert( emvl::MapProjection::isSpheroid(
            emvl::MapProjection::VENUS_RADIUS,
            emvl::MapProjection::VENUS_RADIUS ) );

  assert( emvl::MapProjection::isSpheroid( 4.0 / 3.0, 4.0 / 3.0 ) );
  assert( emvl::MapProjection::isSpheroid( 4.0 / 3.0 + 1e-20, 1.0 ) );
  assert( emvl::MapProjection::isSpheroid( 1e14, 1e14 ) );

  double longitude = 0.0;
  double latitude = 0.0;
  emvl::MapProjection::toLongitudeLatitude( 0.0, 0.0, longitude, latitude );
  assert( longitude == 0.0 );
  assert( latitude == 0.0 );
  emvl::MapProjection::toLongitudeLatitude( -M_PI, 0.0, longitude, latitude );
  assert( longitude == -180.0 );
  assert( latitude == 0.0 );
  emvl::MapProjection::toLongitudeLatitude( M_PI, 0.0, longitude, latitude );
  assert( longitude == 180.0 );
  assert( latitude == 0.0 );
  emvl::MapProjection::toLongitudeLatitude( 0.0, -PI_OVER_2,
                                            longitude, latitude );
  assert( longitude == 0.0 );
  assert( latitude == -90.0 );
  emvl::MapProjection::toLongitudeLatitude( 0.0, PI_OVER_2,
                                            longitude, latitude );
  assert( longitude == 0.0 );
  assert( latitude == 90.0 );

  const double t = emvl::MapProjection::MAP_PROJECTION_TOLERANCE;
  assert( emvl::MapProjection::lambdaDifference( 0.0, 0.0 ) == 0.0 );
  assert( emvl::MapProjection::lambdaDifference( M_PI - t, 0.0 ) == M_PI - t );
  assert( emvl::MapProjection::lambdaDifference( -M_PI + t, 0.0) == -M_PI + t);
  assert( emvl::MapProjection::lambdaDifference( 0.0, M_PI ) == -M_PI + t );
  assert( emvl::aboutEqual(
            emvl::MapProjection::lambdaDifference( -M_PI + t, -M_PI ), t ) );
  assert( emvl::aboutEqual(
            emvl::MapProjection::lambdaDifference( -M_PI + t, M_PI ), -t ) );
  assert( emvl::aboutEqual(
            emvl::MapProjection::lambdaDifference( M_PI - t, -M_PI ), M_PI ) );
  assert( emvl::aboutEqual(
            emvl::MapProjection::lambdaDifference( M_PI - t, M_PI ), M_PI ) );

  double lambda = 0.0;
  double phi = 0.0;
  emvl::MapProjection::toLambdaPhi( 0.0, 0.0, lambda, phi );
  assert( lambda == 0.0 );
  assert( phi == 0.0 );
  emvl::MapProjection::toLambdaPhi( -180.0, 0.0, lambda, phi );
  assert( emvl::aboutEqual( lambda, -M_PI, 1e-4 ) );
  assert( phi == 0.0 );
  emvl::MapProjection::toLambdaPhi( 180.0, 0.0, lambda, phi );
  assert( emvl::aboutEqual( lambda, M_PI, 1e-4 ) );
  assert( phi == 0.0 );
  emvl::MapProjection::toLambdaPhi( 0.0, -90.0, lambda, phi );
  assert( lambda == 0.0 );
  assert( emvl::aboutEqual( phi, -PI_OVER_2, 1e-4 ) );
  emvl::MapProjection::toLambdaPhi( 0.0, 90.0, lambda, phi );
  assert( lambda == 0.0 );
  assert( emvl::aboutEqual( phi, PI_OVER_2, 1e-4 ) );

  assert( ! emvl::isNan( emvl::MapProjection::ssfn( 0.0, sin( 0.0 ), 0.0 ) ) );
  assert( ! emvl::isNan( emvl::MapProjection::ssfn( 1.0, sin( 1.0 ), 0.0 ) ) );
  assert( ! emvl::isNan( emvl::MapProjection::ssfn( -1.0, sin( -1.0 ), 0.0 )));
  assert( ! emvl::isNan( emvl::MapProjection::ssfn( 0.0, sin( 0.0 ), 0.7 ) ) );
  assert( ! emvl::isNan( emvl::MapProjection::ssfn( 1.0, sin( 1.0 ), 0.7 ) ) );
  assert( ! emvl::isNan( emvl::MapProjection::ssfn( -1.0, sin( -1.0 ), 0.7 )));
  assert( ! emvl::isNan( emvl::MapProjection::tsfn( 0.0, sin( 0.0 ), 0.0 ) ) );
  assert( ! emvl::isNan( emvl::MapProjection::tsfn( 1.0, sin( 1.0 ), 0.0 ) ) );
  assert( ! emvl::isNan( emvl::MapProjection::tsfn( -1.0, sin( -1.0 ), 0.0 )));
  assert( ! emvl::isNan( emvl::MapProjection::tsfn( 0.0, sin( 0.0 ), 0.7 ) ) );
  assert( ! emvl::isNan( emvl::MapProjection::tsfn( 1.0, sin( 1.0 ), 0.7 ) ) );
  assert( ! emvl::isNan( emvl::MapProjection::tsfn( -1.0, sin( -1.0 ), 0.7 )));
  assert( ! emvl::isNan( emvl::MapProjection::msfn( sin( 0.1 ),
                                                    cos( 0.1 ), 0.0 ) ) );

  assert( ! emvl::isNan( emvl::MapProjection::msfn( sin( 1.1 ),
                                                    cos( 1.1 ), 0.5 ) ) );

  assert( ! emvl::isNan( emvl::MapProjection::msfn( sin( -0.1 ),
                                                    cos( -0.1 ), 0.5 ) ) );

  assert( ! emvl::isNan( emvl::MapProjection::msfn( sin( -0.1 ),
                                                    cos( -0.1 ), 0.5 ) ) );
  assert( ! emvl::isNan( emvl::MapProjection::qsfn( sin( 0.0 ), 0.0, 1.0 )));
  assert( ! emvl::isNan( emvl::MapProjection::qsfn( sin( 0.0 ), 0.7, 0.51 )));
  assert( ! emvl::isNan( emvl::MapProjection::qsfn( sin( 1.0 ), 0.0, 1.0 )));
  assert( ! emvl::isNan( emvl::MapProjection::qsfn( sin( 1.0 ), 0.7, 0.51 )));
  assert( ! emvl::isNan( emvl::MapProjection::qsfn( sin( -1.0 ), 0.0, 1.0 )));
  assert( ! emvl::isNan( emvl::MapProjection::qsfn( sin( -1.0 ), 0.7, 0.51 )));
  assert( ! emvl::isNan( emvl::MapProjection::phi2Iterate( 0.0, 0.0 ) ) );
  assert( ! emvl::isNan( emvl::MapProjection::phi2Iterate( 1.0, 0.0 ) ) );
  assert( ! emvl::isNan( emvl::MapProjection::phi2Iterate( 0.0, 0.7 ) ) );
  assert( ! emvl::isNan( emvl::MapProjection::phi2Iterate( 1.0, 0.7 ) ) );

  for ( size_t index1 = 0; index1 < rcount; ++index1 ) {
    const double value1 = rvalues[ index1 ];
    assert( ! emvl::isNan( value1 ) );
    const double sinePhi = sin( value1 );
    const double cosinePhi = cos( value1 );

    if ( AND8( ! emvl::isNan( sinePhi ),
               ! emvl::isNan( cosinePhi ),
               sinePhi   > -1.0,
               sinePhi   < 1.0,
               cosinePhi > -1.0,
               cosinePhi < 1.0,
               cosinePhi != 0.0,
               emvl::aboutEqual( fabs( sinePhi ),
                                 sqrt( 1.0 - SQUARE( cosinePhi ) ),
                 emvl::MapProjection::MAP_PROJECTION_TOLERANCE ) ) ) {

      for ( size_t index2 = 0; index2 < rcount; ++index2 ) {
        const double value2 = rvalues[ index2 ];
        assert( ! emvl::isNan( value2 ) );
        const double e = emvl::clampedToRange( value2, 0.0, 0.7 );
        const double e2 = SQUARE( e );

        if ( AND3( IN_RANGE( e2, 0.0, 0.7 ),
                   e2 * SQUARE( sinePhi ) < 1.0,
                   sqrt( 1.0 - e2 * SQUARE( sinePhi ) ) != 0.0 ) ) {
          assert( ! emvl::isNan( emvl::MapProjection::ssfn( value1,
                                                       sin( value1 ), e ) ) );
          assert( ! emvl::isNan( emvl::MapProjection::tsfn( value1,
                                                       sin( value1 ), e ) ) );
          assert( ! emvl::isNan(
                    emvl::MapProjection::msfn( sin( value1 ),
                                               cos( value1 ), e2 ) ) );
          assert( ! emvl::isNan(
                    emvl::MapProjection::qsfn( sin( value1 ),
                                               e, 1.0 - e2 ) ) );
          assert( ! emvl::isNan(
                    emvl::MapProjection::phi2Iterate( value1, e ) ) );

        }
      }
    }
  }
}



/******************************************************************************
PURPOSE: testLambert - Test Lambert class.
******************************************************************************/

static void testLambert() {
  printf( "testLambert()\n" );

  {
    double longitude = 0.0;
    double latitude =  0.0;
    double x = 0.0;
    double y = 0.0;
    const emvl::Lambert lambert(6371000.0, 6371000.0, -90.0, 40.0, 30.0, 60.0);
    assert( ! strcmp( lambert.name(), "Lambert" ) );
    assert( emvl::aboutEqual( lambert.majorSemiaxis(), 6371000.0 ) );
    assert( emvl::aboutEqual( lambert.minorSemiaxis(), 6371000.0 ) );
    assert( emvl::aboutEqual( lambert.centralLongitude(), -90.0 ) );
    assert( emvl::aboutEqual( lambert.centralLatitude(), 40.0 ) );
    assert( emvl::aboutEqual( lambert.lowerLatitude(), 30.0 ) );
    assert( emvl::aboutEqual( lambert.upperLatitude(), 60.0 ) );
    assert( lambert == lambert );
    assert( ! ( lambert != lambert ) );
    assert( emvl::aboutEqual( lambert.majorSemiaxis(),
                              lambert.majorSemiaxis() ) );
    assert( emvl::aboutEqual( lambert.minorSemiaxis(),
                              lambert.minorSemiaxis() ) );
    assert( emvl::aboutEqual( lambert.centralLongitude(),
                              lambert.centralLongitude() ) );
    assert( emvl::aboutEqual( lambert.centralLatitude(),
                              lambert.centralLatitude() ) );
    assert( emvl::aboutEqual( lambert.lowerLatitude(),
                              lambert.lowerLatitude() ) );
    assert( emvl::aboutEqual( lambert.upperLatitude(),
                              lambert.upperLatitude() ) );
    lambert.project( lambert.centralLongitude(), lambert.centralLatitude(),
                     x, y );
    assert( emvl::aboutEqual( x, 0.0 ) );
    assert( emvl::aboutEqual( y, 0.0 ) );
    lambert.unproject( x, y, longitude, latitude );
    assert( emvl::aboutEqual( longitude, lambert.centralLongitude() ) );
    assert( emvl::aboutEqual( latitude, lambert.centralLatitude() ) );
    lambert.project( -75.0, 35.0, x, y );
    assert( ! emvl::isNan( x ) );
    assert( ! emvl::isNan( y ) );
    lambert.unproject( x, y, longitude, latitude );
    assert( emvl::aboutEqual( longitude, -75.0 ) );
    assert( emvl::aboutEqual( latitude, 35.0 ) );

    emvl::MapProjection* mapProjection = lambert.newCopy(); //x
    assert( mapProjection );
    assert( mapProjection->invariant() );
    assert( ! strcmp( mapProjection->name(), lambert.name() ) );
    delete mapProjection; mapProjection = 0;

    emvl::Lambert lambert2( lambert );
    assert( lambert == lambert2 );
    assert( ! ( lambert != lambert2 ) );
    assert( emvl::aboutEqual( lambert.majorSemiaxis(),
                              lambert2.majorSemiaxis() ) );
    assert( emvl::aboutEqual( lambert.minorSemiaxis(),
                              lambert2.minorSemiaxis() ) );
    assert( emvl::aboutEqual( lambert.centralLongitude(),
                              lambert2.centralLongitude() ) );
    assert( emvl::aboutEqual( lambert.centralLatitude(),
                              lambert2.centralLatitude() ) );
    assert( emvl::aboutEqual( lambert.lowerLatitude(),
                              lambert2.lowerLatitude() ) );
    assert( emvl::aboutEqual( lambert.upperLatitude(),
                              lambert2.upperLatitude() ) );

    lambert2 = lambert;
    assert( lambert == lambert2 );
    assert( ! ( lambert != lambert2 ) );
    assert( emvl::aboutEqual( lambert.majorSemiaxis(),
                              lambert2.majorSemiaxis() ) );
    assert( emvl::aboutEqual( lambert.minorSemiaxis(),
                              lambert2.minorSemiaxis() ) );
    assert( emvl::aboutEqual( lambert.centralLongitude(),
                              lambert2.centralLongitude() ) );
    assert( emvl::aboutEqual( lambert.centralLatitude(),
                              lambert2.centralLatitude() ) );
    assert( emvl::aboutEqual( lambert.lowerLatitude(),
                              lambert2.lowerLatitude() ) );
    assert( emvl::aboutEqual( lambert.upperLatitude(),
                              lambert2.upperLatitude() ) );

    const emvl::Lambert lambert3( emvl::MapProjection::WGS_1984_MAJOR_SEMIAXIS,
                                  emvl::MapProjection::WGS_1984_MINOR_SEMIAXIS,
                                  -90.0, 40.0, 30.0, 60.0 );
    assert( lambert3 != lambert );
    assert( ! ( lambert3 == lambert ) );
    lambert3.project( lambert3.centralLongitude(), lambert3.centralLatitude(),
                      x, y );
    assert( emvl::aboutEqual( x, 0.0 ) );
    assert( emvl::aboutEqual( y, 0.0 ) );
    lambert3.unproject( x, y, longitude, latitude );
    assert( emvl::aboutEqual( longitude, lambert3.centralLongitude() ) );
    assert( emvl::aboutEqual( latitude, lambert3.centralLatitude() ) );
    lambert3.project( -126.0, 45.0, x, y );
    lambert3.unproject( x, y, longitude, latitude );
    assert( emvl::aboutEqual( longitude, -126.0 ) );
    assert( emvl::aboutEqual( latitude, 45.0 ) );

    for ( double longitude = -180.0; longitude <= 180.0; ++longitude ) {

      for ( double latitude = -90.0; latitude <= 90.0; ++latitude ) {
        double x = 0.0;
        double y = 0.0;
        double lon = 0.0;
        double lat = 0.0;
        lambert3.project( longitude, latitude, x, y );
        lambert3.unproject( x, y, lon, lat );
        assert( emvl::aboutEqual( lon, longitude, 1e-4 ) );
        assert( emvl::aboutEqual( lat, latitude, 1e-4 ) );
      }
    }

    {
      double lon = 0.0;
      double lat = 0.0;
      const emvl::Lambert lambert(emvl::MapProjection::WGS_1984_MAJOR_SEMIAXIS,
                                  emvl::MapProjection::WGS_1984_MINOR_SEMIAXIS,
                                  -90.0, 45.0, 30.0, 60.0 );
      lambert.unproject( 0.0, 0.0, lon, lat );
      assert( emvl::aboutEqual( lon, lambert.centralLongitude(), 1e-4 ) );
      assert( emvl::aboutEqual( lat, lambert.centralLatitude(), 1e-4 ) );

      const emvl::Lambert lambert2( 6370000.0, 6370000.0,
                                    -45.0, -45.0, -45.0, -45.0 );
      lambert2.unproject( 0.0, 0.0, lon, lat );
      assert( emvl::aboutEqual( lon, lambert2.centralLongitude(), 1e-4 ) );
      assert( emvl::aboutEqual( lat, lambert2.centralLatitude(), 1e-4 ) );
      lambert2.unproject( 0.0, -6370000.0, lon, lat );
      assert( emvl::aboutEqual( lon, -45.0, 1e-4 ) );
      assert( emvl::aboutEqual( lat, -90.0, 1e-4 ) );

      for ( size_t index1 = 0; index1 < lonsCount; ++index1 ) {
        const double longitude = lons[ index1 ];

        for ( size_t index2 = 0; index2 < latsCount; ++index2 ) {
          const double latitude = lats[ index2 ];
          double x = 0.0;
          double y = 0.0;
          lambert.project( longitude, latitude, x, y );
          lambert.unproject( x, y, lon, lat );
          assert( emvl::aboutEqual( lon, longitude, 1e-4 ) );
          assert( emvl::aboutEqual( lat, latitude, 1e-4 ) );
          lambert2.project( longitude, latitude, x, y );
          lambert2.unproject( x, y, lon, lat );
          assert( emvl::aboutEqual( lon, longitude, 1e-4 ) );
          assert( emvl::aboutEqual( lat, latitude, 1e-4 ) );
        }
      }

      for ( size_t index1 = 0; index1 < rcount; ++index1 ) {
        const double x = rvalues[ index1 ];

        if ( emvl::isFinite( x ) ) {

          for ( size_t index2 = 0; index2 < rcount; ++index2 ) {
            const double y = rvalues[ index2 ];

            if ( emvl::isFinite( y ) ) {
              double x2 = 0.0;
              double y2 = 0.0;
              lambert.unproject( x, y, lon, lat );
              lambert.project( lon, lat, x2, y2 );
              lambert2.unproject( x, y, lon, lat );
              lambert2.project( lon, lat, x2, y2 );
            }
          }
        }
      }
    }

    for ( double longitude = -180.0; longitude <= 180.0; ++longitude ) {

      for ( double latitude = -90.0; latitude <= 90.0; ++latitude ) {
        double x = 0.0;
        double y = 0.0;
        double lon = 0.0;
        double lat = 0.0;
        lambert3.project( longitude, latitude, x, y );
        lambert3.unproject( x, y, lon, lat );
        assert( emvl::aboutEqual( lon, longitude, 1e-4 ) );
        assert( emvl::aboutEqual( lat, latitude, 1e-4 ) );
      }
    }
  }

  for ( size_t index1 = 0; index1 < rcount; index1 += 4 ) {
    const double majorSemiaxis = rvalues[ index1 ];

    for ( size_t index2 = 0; index2 < rcount; index2 += 8 ) {
      const double minorSemiaxis = rvalues[ index2 ];

      if ( emvl::MapProjection::isSpheroid( majorSemiaxis, minorSemiaxis ) ) {

        for ( size_t index3 = 0; index3 < lonsCount; index3 += 3 ) {
          const double centralLongitude = lons[ index3 ];

          for ( size_t index4 = 0; index4 < latsCount; index4 += 3 ) {
            const double centralLatitude = lats[ index4 ];

            if ( IN_RANGE( centralLatitude, -89.0, 89.0 ) ) {

              for ( size_t index5 = 0; index5 < latsCount; index5 += 5 ) {
                const double lowerLatitude = lats[ index5 ];

                for ( size_t index6 = 0; index6 < latsCount; index6 += 7 ) {
                  const double upperLatitude = lats[ index6 ];

                  if ( AND5( emvl::isLatitude( upperLatitude ),
                             lowerLatitude <= upperLatitude,
                             SIGN( lowerLatitude ) == SIGN( upperLatitude ),
                             IMPLIES_ELSE( lowerLatitude >= 0.0,
                               IN_RANGE( lowerLatitude, 1.0, 89.0 ),
                               IN_RANGE( lowerLatitude, -89.0, -1.0 ) ),
                             IMPLIES_ELSE( upperLatitude >= 0.0,
                               IN_RANGE( upperLatitude, 1.0, 89.0 ),
                               IN_RANGE( upperLatitude, -89.0, -1.0 ) ) ) ) {

                    const emvl::Lambert lambert( majorSemiaxis,
                                                 minorSemiaxis,
                                                 centralLongitude,
                                                 centralLatitude,
                                                 lowerLatitude,
                                                 upperLatitude );
                    const emvl::MapProjection& mapProjection = lambert;

                    for ( size_t index7 = 0; index7 < lonsCount; ++index7 ) {
                      const double longitude = lons[ index7 ];

                      for ( size_t index8 = 0; index8 < latsCount; ++index8 ) {
                        const double latitude = lats[ index8 ];
                        double lon = 0.0;
                        double lat = 0.0;
                        double x = 0.0;
                        double y = 0.0;
                        lambert.project( longitude, latitude, x, y );
                        lambert.unproject( x, y, lon, lat );
                        assert( emvl::aboutEqual( lon, longitude,1e-4 ) );
                        assert( emvl::aboutEqual( lat, latitude, 1e-4 ) );
                        mapProjection.project( longitude, latitude, x, y );
                        mapProjection.unproject( x, y, lon, lat );
                        assert( emvl::aboutEqual( lon, longitude, 1e-4 ) );
                        assert( emvl::aboutEqual( lat, latitude,  1e-4 ) );
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
}



/******************************************************************************
PURPOSE: testMercator - Test Mercator class.
******************************************************************************/

static void testMercator() {
  printf( "testMercator()\n" );

  {
    double longitude = 0.0;
    double latitude =  0.0;
    double x = 0.0;
    double y = 0.0;
    const emvl::Mercator mercator( 6371000.0, 6371000.0, -90.0 );
    assert( ! strcmp( mercator.name(), "Mercator" ) );
    assert( emvl::aboutEqual( mercator.majorSemiaxis(), 6371000.0 ) );
    assert( emvl::aboutEqual( mercator.minorSemiaxis(), 6371000.0 ) );
    assert( emvl::aboutEqual( mercator.centralLongitude(), -90.0 ) );
    assert( mercator.centralLatitude() == 0.0 );
    assert( mercator == mercator );
    assert( ! ( mercator != mercator ) );
    assert( emvl::aboutEqual( mercator.majorSemiaxis(),
                              mercator.majorSemiaxis() ) );
    assert( emvl::aboutEqual( mercator.minorSemiaxis(),
                              mercator.minorSemiaxis() ) );
    assert( emvl::aboutEqual( mercator.centralLongitude(),
                              mercator.centralLongitude() ) );
    assert( emvl::aboutEqual( mercator.centralLatitude(),
                              mercator.centralLatitude() ) );
    mercator.project( mercator.centralLongitude(), mercator.centralLatitude(),
                      x, y );
    assert( emvl::aboutEqual( x, 0.0 ) );
    assert( emvl::aboutEqual( y, 0.0 ) );
    mercator.unproject( x, y, longitude, latitude );
    assert( emvl::aboutEqual( longitude, mercator.centralLongitude() ) );
    assert( emvl::aboutEqual( latitude, mercator.centralLatitude() ) );
    mercator.project( -75.0, 35.0, x, y );
    assert( ! emvl::isNan( x ) );
    assert( ! emvl::isNan( y ) );
    mercator.unproject( x, y, longitude, latitude );
    assert( emvl::aboutEqual( longitude, -75.0 ) );
    assert( emvl::aboutEqual( latitude, 35.0 ) );

    emvl::MapProjection* mapProjection = mercator.newCopy(); //x
    assert( mapProjection );
    assert( mapProjection->invariant() );
    assert( ! strcmp( mapProjection->name(), mercator.name() ) );
    delete mapProjection; mapProjection = 0;

    emvl::Mercator mercator2( mercator );
    assert( mercator == mercator2 );
    assert( ! ( mercator != mercator2 ) );
    assert( emvl::aboutEqual( mercator.majorSemiaxis(),
                              mercator2.majorSemiaxis() ) );
    assert( emvl::aboutEqual( mercator.minorSemiaxis(),
                              mercator2.minorSemiaxis() ) );
    assert( emvl::aboutEqual( mercator.centralLongitude(),
                              mercator2.centralLongitude() ) );
    assert( emvl::aboutEqual( mercator.centralLatitude(),
                              mercator2.centralLatitude() ) );

    mercator2 = mercator;
    assert( mercator == mercator2 );
    assert( ! ( mercator != mercator2 ) );
    assert( emvl::aboutEqual( mercator.majorSemiaxis(),
                              mercator2.majorSemiaxis() ) );
    assert( emvl::aboutEqual( mercator.minorSemiaxis(),
                              mercator2.minorSemiaxis() ) );
    assert( emvl::aboutEqual( mercator.centralLongitude(),
                              mercator2.centralLongitude() ) );
    assert( emvl::aboutEqual( mercator.centralLatitude(),
                              mercator2.centralLatitude() ) );

    const emvl::Mercator
      mercator3( emvl::MapProjection::WGS_1984_MAJOR_SEMIAXIS,
                 emvl::MapProjection::WGS_1984_MINOR_SEMIAXIS, -90.0 );
    assert( mercator3 != mercator );
    assert( ! ( mercator3 == mercator ) );
    mercator3.project( mercator3.centralLongitude(),
                       mercator3.centralLatitude(), x, y );
    assert( emvl::aboutEqual( x, 0.0 ) );
    assert( emvl::aboutEqual( y, 0.0 ) );
    mercator3.unproject( x, y, longitude, latitude );
    assert( emvl::aboutEqual( longitude, mercator3.centralLongitude() ) );
    assert( emvl::aboutEqual( latitude, mercator3.centralLatitude() ) );
    mercator3.project( -126.0, 45.0, x, y );
    mercator3.unproject( x, y, longitude, latitude );
    assert( emvl::aboutEqual( longitude, -126.0 ) );
    assert( emvl::aboutEqual( latitude, 45.0 ) );

    for ( double longitude = -180.0; longitude <= 180.0; ++longitude ) {

      for ( double latitude = -90.0; latitude <= 90.0; ++latitude ) {
        double x = 0.0;
        double y = 0.0;
        double lon = 0.0;
        double lat = 0.0;
        mercator3.project( longitude, latitude, x, y );
        mercator3.unproject( x, y, lon, lat );
        assert( emvl::aboutEqual( lon, longitude, 1e-4 ) );
        assert( emvl::aboutEqual( lat, latitude, 1e-4 ) );
      }
    }

    {
      double lon = 0.0;
      double lat = 0.0;
      const emvl::Mercator
        mercator( emvl::MapProjection::WGS_1984_MAJOR_SEMIAXIS,
                  emvl::MapProjection::WGS_1984_MINOR_SEMIAXIS, -90.0 );
      mercator.unproject( 0.0, 0.0, lon, lat );
      assert( emvl::aboutEqual( lon, mercator.centralLongitude(), 1e-4 ) );
      assert( emvl::aboutEqual( lat, mercator.centralLatitude(), 1e-4 ) );

      const emvl::Mercator mercator2( 6370000.0, 6370000.0, -45.0 );
      mercator2.unproject( 0.0, 0.0, lon, lat );
      assert( emvl::aboutEqual( lon, mercator2.centralLongitude(), 1e-4 ) );
      assert( emvl::aboutEqual( lat, mercator2.centralLatitude(), 1e-4 ) );
      mercator2.unproject( 0.0, 1234.0, lon, lat );
      assert( emvl::aboutEqual( lon, mercator2.centralLongitude(), 1e-4 ) );
      mercator2.unproject( 1234.0, 0.0, lon, lat );
      assert( emvl::aboutEqual( lat, mercator2.centralLatitude(), 1e-4 ) );

      for ( size_t index1 = 0; index1 < lonsCount; ++index1 ) {
        const double longitude = lons[ index1 ];

        for ( size_t index2 = 0; index2 < latsCount; ++index2 ) {
          const double latitude = lats[ index2 ];
          double x = 0.0;
          double y = 0.0;
          mercator.project( longitude, latitude, x, y );
          mercator.unproject( x, y, lon, lat );
          assert( emvl::aboutEqual( lon, longitude, 1e-4 ) );
          assert( emvl::aboutEqual( lat, latitude, 1e-4 ) );
          mercator2.project( longitude, latitude, x, y );
          mercator2.unproject( x, y, lon, lat );
          assert( emvl::aboutEqual( lon, longitude, 1e-4 ) );
          assert( emvl::aboutEqual( lat, latitude, 1e-4 ) );
        }
      }

      for ( size_t index1 = 0; index1 < rcount; ++index1 ) {
        const double x = rvalues[ index1 ];

        if ( AND2( IN_RANGE( x, -M_PI * mercator.majorSemiaxis(),
                                 M_PI * mercator.majorSemiaxis() ),
                   IN_RANGE( x, -M_PI * mercator2.majorSemiaxis(),
                                 M_PI * mercator2.majorSemiaxis() ) ) ) {

          for ( size_t index2 = 0; index2 < rcount; ++index2 ) {
            const double y = rvalues[ index2 ];

            if ( AND2( IN_RANGE( y, -6.0 * M_PI * mercator.minorSemiaxis(),
                                     6.0 * M_PI * mercator.minorSemiaxis() ),
                       IN_RANGE( y, -6.0 * M_PI * mercator2.minorSemiaxis(),
                                     6.0 * M_PI * mercator2.minorSemiaxis()))){
              double x2 = 0.0;
              double y2 = 0.0;
              mercator.unproject( x, y, lon, lat );
              mercator.project( lon, lat, x2, y2 );
              assert( emvl::aboutEqual( x, x2 ) );
              assert( emvl::aboutEqual( y, y2 ) );
              mercator2.unproject( x, y, lon, lat );
              mercator2.project( lon, lat, x2, y2 );
              assert( emvl::aboutEqual( x, x2 ) );
              assert( emvl::aboutEqual( y, y2 ) );
            }
          }
        }
      }
    }

    for ( double longitude = -180.0; longitude <= 180.0; ++longitude ) {

      for ( double latitude = -90.0; latitude <= 90.0; ++latitude ) {
        double x = 0.0;
        double y = 0.0;
        double lon = 0.0;
        double lat = 0.0;
        mercator3.project( longitude, latitude, x, y );
        mercator3.unproject( x, y, lon, lat );
        assert( emvl::aboutEqual( lon, longitude, 1e-4 ) );
        assert( emvl::aboutEqual( lat, latitude, 1e-4 ) );
      }
    }
  }

  for ( size_t index1 = 0; index1 < rcount; ++index1 ) {
    const double majorSemiaxis = rvalues[ index1 ];

    for ( size_t index2 = 0; index2 < rcount; ++index2 ) {
      const double minorSemiaxis = rvalues[ index2 ];

      if ( emvl::MapProjection::isSpheroid( majorSemiaxis, minorSemiaxis ) ) {

        for ( size_t index3 = 0; index3 < lonsCount; ++index3 ) {
          const double centralLongitude = lons[ index3 ];
          const emvl::Mercator mercator( majorSemiaxis, minorSemiaxis,
                                         centralLongitude );
          const emvl::MapProjection& mapProjection = mercator;

          for ( size_t index4 = 0; index4 < lonsCount; ++index4 ) {
            const double longitude = lons[ index4 ];

            for ( size_t index5 = 0; index5 < latsCount; ++index5 ) {
              const double latitude = lats[ index5 ];
              double lon = 0.0;
              double lat = 0.0;
              double x = 0.0;
              double y = 0.0;
              mercator.project( longitude, latitude, x, y );
              mercator.unproject( x, y, lon, lat );
              assert( emvl::aboutEqual( lon, longitude,1e-4 ) );
              assert( emvl::aboutEqual( lat, latitude, 1e-4 ) );
              mapProjection.project( longitude, latitude, x, y );
              mapProjection.unproject( x, y, lon, lat );
              assert( emvl::aboutEqual( lon, longitude, 1e-4 ) );
              assert( emvl::aboutEqual( lat, latitude,  1e-4 ) );
            }
          }
        }
      }
    }
  }
}



/******************************************************************************
PURPOSE: testAlbers - Test Albers class.
******************************************************************************/

static void testAlbers() {
  printf( "testAlbers()\n" );

  {
    double longitude = 0.0;
    double latitude =  0.0;
    double x = 0.0;
    double y = 0.0;
    const emvl::Albers albers(6371000.0, 6371000.0, -90.0, 40.0, 30.0, 60.0);
    assert( ! strcmp( albers.name(), "Albers" ) );
    assert( emvl::aboutEqual( albers.majorSemiaxis(), 6371000.0 ) );
    assert( emvl::aboutEqual( albers.minorSemiaxis(), 6371000.0 ) );
    assert( emvl::aboutEqual( albers.centralLongitude(), -90.0 ) );
    assert( emvl::aboutEqual( albers.centralLatitude(), 40.0 ) );
    assert( emvl::aboutEqual( albers.lowerLatitude(), 30.0 ) );
    assert( emvl::aboutEqual( albers.upperLatitude(), 60.0 ) );
    assert( albers == albers );
    assert( ! ( albers != albers ) );
    assert( emvl::aboutEqual( albers.majorSemiaxis(),
                              albers.majorSemiaxis() ) );
    assert( emvl::aboutEqual( albers.minorSemiaxis(),
                              albers.minorSemiaxis() ) );
    assert( emvl::aboutEqual( albers.centralLongitude(),
                              albers.centralLongitude() ) );
    assert( emvl::aboutEqual( albers.centralLatitude(),
                              albers.centralLatitude() ) );
    assert( emvl::aboutEqual( albers.lowerLatitude(),
                              albers.lowerLatitude() ) );
    assert( emvl::aboutEqual( albers.upperLatitude(),
                              albers.upperLatitude() ) );
    albers.project( albers.centralLongitude(), albers.centralLatitude(),
                     x, y );
    assert( emvl::aboutEqual( x, 0.0 ) );
    assert( emvl::aboutEqual( y, 0.0 ) );
    albers.unproject( x, y, longitude, latitude );
    assert( emvl::aboutEqual( longitude, albers.centralLongitude() ) );
    assert( emvl::aboutEqual( latitude, albers.centralLatitude() ) );
    albers.project( -75.0, 35.0, x, y );
    assert( ! emvl::isNan( x ) );
    assert( ! emvl::isNan( y ) );
    albers.unproject( x, y, longitude, latitude );
    assert( emvl::aboutEqual( longitude, -75.0 ) );
    assert( emvl::aboutEqual( latitude, 35.0 ) );

    emvl::MapProjection* mapProjection = albers.newCopy(); //x
    assert( mapProjection );
    assert( mapProjection->invariant() );
    assert( ! strcmp( mapProjection->name(), albers.name() ) );
    delete mapProjection; mapProjection = 0;

    emvl::Albers albers2( albers );
    assert( albers == albers2 );
    assert( ! ( albers != albers2 ) );
    assert( emvl::aboutEqual( albers.majorSemiaxis(),
                              albers2.majorSemiaxis() ) );
    assert( emvl::aboutEqual( albers.minorSemiaxis(),
                              albers2.minorSemiaxis() ) );
    assert( emvl::aboutEqual( albers.centralLongitude(),
                              albers2.centralLongitude() ) );
    assert( emvl::aboutEqual( albers.centralLatitude(),
                              albers2.centralLatitude() ) );
    assert( emvl::aboutEqual( albers.lowerLatitude(),
                              albers2.lowerLatitude() ) );
    assert( emvl::aboutEqual( albers.upperLatitude(),
                              albers2.upperLatitude() ) );

    albers2 = albers;
    assert( albers == albers2 );
    assert( ! ( albers != albers2 ) );
    assert( emvl::aboutEqual( albers.majorSemiaxis(),
                              albers2.majorSemiaxis() ) );
    assert( emvl::aboutEqual( albers.minorSemiaxis(),
                              albers2.minorSemiaxis() ) );
    assert( emvl::aboutEqual( albers.centralLongitude(),
                              albers2.centralLongitude() ) );
    assert( emvl::aboutEqual( albers.centralLatitude(),
                              albers2.centralLatitude() ) );
    assert( emvl::aboutEqual( albers.lowerLatitude(),
                              albers2.lowerLatitude() ) );
    assert( emvl::aboutEqual( albers.upperLatitude(),
                              albers2.upperLatitude() ) );

    const emvl::Albers albers3( emvl::MapProjection::WGS_1984_MAJOR_SEMIAXIS,
                                emvl::MapProjection::WGS_1984_MINOR_SEMIAXIS,
                                -90.0, 40.0, 30.0, 60.0 );
    assert( albers3 != albers );
    assert( ! ( albers3 == albers ) );
    albers3.project( albers3.centralLongitude(), albers3.centralLatitude(),
                      x, y );
    assert( emvl::aboutEqual( x, 0.0 ) );
    assert( emvl::aboutEqual( y, 0.0 ) );
    albers3.unproject( x, y, longitude, latitude );
    assert( emvl::aboutEqual( longitude, albers3.centralLongitude() ) );
    assert( emvl::aboutEqual( latitude, albers3.centralLatitude() ) );
    albers3.project( -126.0, 45.0, x, y );
    albers3.unproject( x, y, longitude, latitude );
    assert( emvl::aboutEqual( longitude, -126.0 ) );
    assert( emvl::aboutEqual( latitude, 45.0 ) );

    for ( double longitude = -180.0; longitude <= 180.0; ++longitude ) {

      for ( double latitude = -90.0; latitude <= 90.0; ++latitude ) {
        double x = 0.0;
        double y = 0.0;
        double lon = 0.0;
        double lat = 0.0;
        albers3.project( longitude, latitude, x, y );
        albers3.unproject( x, y, lon, lat );
        assert( emvl::aboutEqual( lon, longitude, 1e-4 ) );
        assert( emvl::aboutEqual( lat, latitude, 1e-4 ) );
      }
    }

    {
      double lon = 0.0;
      double lat = 0.0;
      const emvl::Albers albers( emvl::MapProjection::WGS_1984_MAJOR_SEMIAXIS,
                                 emvl::MapProjection::WGS_1984_MINOR_SEMIAXIS,
                                 -90.0, 45.0, 30.0, 60.0 );
      albers.unproject( 0.0, 0.0, lon, lat );
      assert( emvl::aboutEqual( lon, albers.centralLongitude(), 1e-4 ) );
      assert( emvl::aboutEqual( lat, albers.centralLatitude(), 1e-4 ) );
      const emvl::Albers albers2( 6370000.0, 6370000.0,
                                  -45.0, -45.0, -45.0, -45.0 );
      albers2.unproject( 0.0, 0.0, lon, lat );
      assert( emvl::aboutEqual( lon, albers2.centralLongitude(), 1e-4 ) );
      assert( emvl::aboutEqual( lat, albers2.centralLatitude(), 1e-4 ) );
      albers2.unproject( 0.0, -6370000.0, lon, lat );
      assert( emvl::aboutEqual( lon, -45.0, 1e-4 ) );
      assert( emvl::aboutEqual( lat, -90.0, 1e-4 ) );

      for ( size_t index1 = 0; index1 < lonsCount; ++index1 ) {
        const double longitude = lons[ index1 ];

        for ( size_t index2 = 0; index2 < latsCount; ++index2 ) {
          const double latitude = lats[ index2 ];
          double x = 0.0;
          double y = 0.0;
          albers.project( longitude, latitude, x, y );
          albers.unproject( x, y, lon, lat );
          assert( emvl::aboutEqual( lon, longitude, 1e-4 ) );
          assert( emvl::aboutEqual( lat, latitude, 1e-4 ) );
          albers2.project( longitude, latitude, x, y );
          albers2.unproject( x, y, lon, lat );
          assert( emvl::aboutEqual( lon, longitude, 1e-4 ) );
          assert( emvl::aboutEqual( lat, latitude, 1e-4 ) );
        }
      }

      for ( size_t index1 = 0; index1 < rcount; ++index1 ) {
        const double x = rvalues[ index1 ];

        if ( AND2( IN_RANGE( x, -2.5 * albers.majorSemiaxis(),
                                 2.5 * albers.majorSemiaxis() ),
                   IN_RANGE( x, -2.5 * albers2.majorSemiaxis(),
                                 2.5 * albers2.majorSemiaxis() ) ) ) {

          for ( size_t index2 = 0; index2 < rcount; ++index2 ) {
            const double y = rvalues[ index2 ];

            if ( AND2( IN_RANGE( y, -2.5 * M_PI * albers.minorSemiaxis(),
                                     2.5 * M_PI * albers.minorSemiaxis() ),
                       IN_RANGE( y, -2.5 * M_PI * albers2.minorSemiaxis(),
                                     2.5 * M_PI * albers2.minorSemiaxis()))) {
              double x2 = 0.0;
              double y2 = 0.0;
              albers.unproject( x, y, lon, lat );
              albers.project( lon, lat, x2, y2 );
              albers2.unproject( x, y, lon, lat );
              albers2.project( lon, lat, x2, y2 );
            }
          }
        }
      }
    }

    for ( double longitude = -180.0; longitude <= 180.0; ++longitude ) {

      for ( double latitude = -90.0; latitude <= 90.0; ++latitude ) {
        double x = 0.0;
        double y = 0.0;
        double lon = 0.0;
        double lat = 0.0;
        albers3.project( longitude, latitude, x, y );
        albers3.unproject( x, y, lon, lat );
        assert( emvl::aboutEqual( lon, longitude, 1e-4 ) );
        assert( emvl::aboutEqual( lat, latitude, 1e-4 ) );
      }
    }
  }

  for ( size_t index1 = 0; index1 < rcount; index1 += 4 ) {
    const double majorSemiaxis = rvalues[ index1 ];

    for ( size_t index2 = 0; index2 < rcount; index2 += 8 ) {
      const double minorSemiaxis = rvalues[ index2 ];

      if ( emvl::MapProjection::isSpheroid( majorSemiaxis, minorSemiaxis ) ) {

        for ( size_t index3 = 0; index3 < lonsCount; index3 += 3 ) {
          const double centralLongitude = lons[ index3 ];

          for ( size_t index4 = 0; index4 < latsCount; index4 += 3 ) {
            const double centralLatitude = lats[ index4 ];

            if ( IN_RANGE( centralLatitude, -89.0, 89.0 ) ) {

              for ( size_t index5 = 0; index5 < latsCount; index5 += 5 ) {
                const double lowerLatitude = lats[ index5 ];

                for ( size_t index6 = 0; index6 < latsCount; index6 += 7 ) {
                  const double upperLatitude = lats[ index6 ];

                  if ( AND5( emvl::isLatitude( upperLatitude ),
                             lowerLatitude <= upperLatitude,
                             SIGN( lowerLatitude ) == SIGN( upperLatitude ),
                             IMPLIES_ELSE( lowerLatitude >= 0.0,
                               IN_RANGE( lowerLatitude, 1.0, 89.0 ),
                               IN_RANGE( lowerLatitude, -89.0, -1.0 ) ),
                             IMPLIES_ELSE( upperLatitude >= 0.0,
                               IN_RANGE( upperLatitude, 1.0, 89.0 ),
                               IN_RANGE( upperLatitude, -89.0, -1.0 ) ) ) ) {

                    const emvl::Albers albers( majorSemiaxis,
                                               minorSemiaxis,
                                               centralLongitude,
                                               centralLatitude,
                                               lowerLatitude,
                                               upperLatitude );
                    const emvl::MapProjection& mapProjection = albers;

                    for ( size_t index7 = 0; index7 < lonsCount; ++index7 ) {
                      const double longitude = lons[ index7 ];

                      for ( size_t index8 = 0; index8 < latsCount; ++index8 ) {
                        const double latitude = lats[ index8 ];
                        double lon = 0.0;
                        double lat = 0.0;
                        double x = 0.0;
                        double y = 0.0;
                        albers.project( longitude, latitude, x, y );
                        albers.unproject( x, y, lon, lat );
                        assert( emvl::aboutEqual( lon, longitude,1e-4 ) );
                        assert( emvl::aboutEqual( lat, latitude, 1e-4 ) );
                        mapProjection.project( longitude, latitude, x, y );
                        mapProjection.unproject( x, y, lon, lat );
                        assert( emvl::aboutEqual( lon, longitude, 1e-4 ) );
                        assert( emvl::aboutEqual( lat, latitude,  1e-4 ) );
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
}



/******************************************************************************
PURPOSE: testStereographic - Test Stereographic class.
******************************************************************************/

static void testStereographic() {
  printf( "testStereographic()\n" );

  {
    double longitude = 0.0;
    double latitude =  0.0;
    double x = 0.0;
    double y = 0.0;

    const emvl::Stereographic stereographic( 6371000.0, 6371000.0,
                                             -90.0, 90.0, 90.0 );
    assert( ! strcmp( stereographic.name(), "Stereographic" ) );
    assert( emvl::aboutEqual( stereographic.majorSemiaxis(), 6371000.0 ) );
    assert( emvl::aboutEqual( stereographic.minorSemiaxis(), 6371000.0 ) );
    assert( emvl::aboutEqual( stereographic.centralLongitude(), -90.0 ) );
    assert( emvl::aboutEqual( stereographic.centralLatitude(), 90.0 ) );
    assert( emvl::aboutEqual( stereographic.secantLatitude(), 90.0 ) );
    assert( stereographic == stereographic );
    assert( ! ( stereographic != stereographic ) );
    assert( emvl::aboutEqual( stereographic.majorSemiaxis(),
                              stereographic.majorSemiaxis() ) );
    assert( emvl::aboutEqual( stereographic.minorSemiaxis(),
                              stereographic.minorSemiaxis() ) );
    assert( emvl::aboutEqual( stereographic.centralLongitude(),
                              stereographic.centralLongitude() ) );
    assert( emvl::aboutEqual( stereographic.centralLatitude(),
                              stereographic.centralLatitude() ) );
    assert( emvl::aboutEqual( stereographic.secantLatitude(),
                              stereographic.secantLatitude() ) );
    stereographic.project( stereographic.centralLongitude(),
                           stereographic.centralLatitude(), x, y );
    stereographic.unproject( x, y, longitude, latitude );
    assert( emvl::aboutEqual( longitude, stereographic.centralLongitude() ) );
    stereographic.project( -75.0, 35.0, x, y );
    assert( ! emvl::isNan( x ) );
    assert( ! emvl::isNan( y ) );
    stereographic.unproject( x, y, longitude, latitude );
    assert( emvl::aboutEqual( longitude, -75.0, 1e-4 ) );
    assert( emvl::aboutEqual( latitude, 35.0, 1e-4 ) );

    emvl::MapProjection* mapProjection = stereographic.newCopy(); //x
    assert( mapProjection );
    assert( mapProjection->invariant() );
    assert( ! strcmp( mapProjection->name(), stereographic.name() ) );
    delete mapProjection; mapProjection = 0;

    emvl::Stereographic stereographic2( stereographic );
    assert( stereographic == stereographic2 );
    assert( ! ( stereographic != stereographic2 ) );
    assert( emvl::aboutEqual( stereographic.majorSemiaxis(),
                              stereographic2.majorSemiaxis() ) );
    assert( emvl::aboutEqual( stereographic.minorSemiaxis(),
                              stereographic2.minorSemiaxis() ) );
    assert( emvl::aboutEqual( stereographic.centralLongitude(),
                              stereographic2.centralLongitude() ) );
    assert( emvl::aboutEqual( stereographic.centralLatitude(),
                              stereographic2.centralLatitude() ) );
    assert( emvl::aboutEqual( stereographic.secantLatitude(),
                              stereographic2.secantLatitude() ) );
    stereographic2 = stereographic;
    assert( stereographic == stereographic2 );
    assert( ! ( stereographic != stereographic2 ) );
    assert( emvl::aboutEqual( stereographic.majorSemiaxis(),
                              stereographic2.majorSemiaxis() ) );
    assert( emvl::aboutEqual( stereographic.minorSemiaxis(),
                              stereographic2.minorSemiaxis() ) );
    assert( emvl::aboutEqual( stereographic.centralLongitude(),
                              stereographic2.centralLongitude() ) );
    assert( emvl::aboutEqual( stereographic.centralLatitude(),
                              stereographic2.centralLatitude() ) );
    assert( emvl::aboutEqual( stereographic.secantLatitude(),
                              stereographic2.secantLatitude() ) );

    const emvl::Stereographic
      stereographic3( emvl::MapProjection::WGS_1984_MAJOR_SEMIAXIS,
                      emvl::MapProjection::WGS_1984_MINOR_SEMIAXIS,
                      -90.0, 90.0, 90.0 );
    assert( stereographic3 != stereographic );
    assert( ! ( stereographic3 == stereographic ) );
    stereographic3.project( stereographic3.centralLongitude(),
                            stereographic3.centralLatitude(), x, y );
    stereographic3.unproject( x, y, longitude, latitude );
    assert( emvl::aboutEqual( longitude,
                              stereographic3.centralLongitude(), 1e-4 ) );
    assert( emvl::aboutEqual( latitude,
                              stereographic3.centralLatitude(), 1e-4 ) );
    stereographic3.project( -126.0, 45.0, x, y );
    stereographic3.unproject( x, y, longitude, latitude );
    assert( emvl::aboutEqual( longitude, -126.0, 1e-4 ) );
    assert( emvl::aboutEqual( latitude, 45.0, 1e-4 ) );

    for ( double longitude = -180.0; longitude <= 180.0; ++longitude ) {

      for ( double latitude = -90.0; latitude <= 90.0; ++latitude ) {
        double x = 0.0;
        double y = 0.0;
        double lon = 0.0;
        double lat = 0.0;
        stereographic3.project( longitude, latitude, x, y );
        stereographic3.unproject( x, y, lon, lat );
        assert( IMPLIES( fabs( latitude ) != 90.0,
                         OR2( emvl::aboutEqual( lon, longitude, 1e-2 ),
                              emvl::aboutEqual( fabs( lon ), 180.0, 1e-2 ) ) ) );
        assert( emvl::aboutEqual( lat, latitude, 1e-4 ) );
      }
    }

    {
      double lon = 0.0;
      double lat = 0.0;
      const emvl::Stereographic
        stereographic( emvl::MapProjection::WGS_1984_MAJOR_SEMIAXIS,
                       emvl::MapProjection::WGS_1984_MINOR_SEMIAXIS,
                       -90.0, 90.0, 90.0 );
      stereographic.unproject( 0.0, 0.0, lon, lat );
      assert( emvl::aboutEqual( lon, stereographic.centralLongitude(), 1e-4 ));
      assert( emvl::aboutEqual( lat, stereographic.centralLatitude(), 1e-4 ) );

      const emvl::Stereographic stereographic2( 6370000.0, 6370000.0,
                                                -90.0, -90.0, -90.0 );
      const emvl::Stereographic stereographic3( 6370000.0, 6370000.0,
                                                -98.0, 40.0, 45.0 );
      stereographic2.unproject( 0.0, 0.0, lon, lat );
      assert( emvl::aboutEqual( lon, stereographic2.centralLongitude(), 1e-4));
      assert( emvl::aboutEqual( lat, stereographic2.centralLatitude(), 1e-4 ));
      stereographic3.unproject( 0.0, 0.0, lon, lat );
      assert( emvl::aboutEqual( lon, stereographic3.centralLongitude(), 1e-4));
      assert( emvl::aboutEqual( lat, stereographic3.centralLatitude(), 1e-4 ));

      for ( size_t index1 = 0; index1 < lonsCount; ++index1 ) {
        const double longitude = lons[ index1 ];

        for ( size_t index2 = 0; index2 < latsCount; ++index2 ) {
          const double latitude = lats[ index2 ];
          double x = 0.0;
          double y = 0.0;
          stereographic.project( longitude, latitude, x, y );
          stereographic.unproject( x, y, lon, lat );
          assert( IMPLIES( ! emvl::aboutEqual( fabs( latitude ), 90.0, 1e-2 ),
                           OR2( emvl::aboutEqual( lon, longitude, 1e-2 ),
                                emvl::aboutEqual( fabs( lon ), 180.0, 1e-2 ) ) ) );
          assert( emvl::aboutEqual( lat, latitude, 1e-4 ) );
          stereographic2.project( longitude, latitude, x, y );
          stereographic2.unproject( x, y, lon, lat );
          assert( IMPLIES( ! emvl::aboutEqual( fabs( latitude ), 90.0, 1e-2 ),
                           OR2( emvl::aboutEqual( lon, longitude, 1e-2 ),
                                emvl::aboutEqual( fabs( lon ), 180.0, 1e-2 ) ) ) );
          assert( emvl::aboutEqual( lat, latitude, 1e-4 ) );
          stereographic3.project( longitude, latitude, x, y );
          stereographic3.unproject( x, y, lon, lat );
          assert( IMPLIES( ! emvl::aboutEqual( fabs( latitude ), 90.0, 1e-2 ),
                           OR2( emvl::aboutEqual( lon, longitude, 1e-2 ),
                                emvl::aboutEqual( fabs( lon ), 180.0, 1e-2 ) ) ) );
          assert( emvl::aboutEqual( lat, latitude, 1e-4 ) );
        }
      }

      for ( size_t index1 = 0; index1 < rcount; ++index1 ) {
        const double x = rvalues[ index1 ];

        if ( emvl::isFinite( x ) ) {

          for ( size_t index2 = 0; index2 < rcount; ++index2 ) {
            const double y = rvalues[ index2 ];

            if ( emvl::isFinite( y ) ) {
              double x2 = 0.0;
              double y2 = 0.0;
              stereographic.unproject( x, y, lon, lat );
              stereographic.project( lon, lat, x2, y2 );
              stereographic2.unproject( x, y, lon, lat );
              stereographic2.project( lon, lat, x2, y2 );
              stereographic3.unproject( x, y, lon, lat );
              stereographic3.project( lon, lat, x2, y2 );
            }
          }
        }
      }
    }

    for ( double longitude = -180.0; longitude <= 180.0; ++longitude ) {

      for ( double latitude = -90.0; latitude <= 90.0; ++latitude ) {
        double x = 0.0;
        double y = 0.0;
        double lon = 0.0;
        double lat = 0.0;
        stereographic3.project( longitude, latitude, x, y );
        stereographic3.unproject( x, y, lon, lat );
        assert( IMPLIES( ! emvl::aboutEqual( fabs( latitude ), 90.0, 1e-2 ),
                         OR2( emvl::aboutEqual( lon, longitude, 1e-2 ),
                              emvl::aboutEqual( fabs( lon ), 180.0, 1e-2 ) ) ) );
        assert( emvl::aboutEqual( lat, latitude, 1e-4 ) );
      }
    }
  }

  for ( size_t index1 = 0; index1 < rcount; index1 += 10 ) {
    const double majorSemiaxis = rvalues[ index1 ];

    for ( size_t index2 = 0; index2 < rcount; index2 += 10 ) {
      const double minorSemiaxis = rvalues[ index2 ];

      if ( emvl::MapProjection::isSpheroid( majorSemiaxis, minorSemiaxis ) ) {

        for ( size_t index3 = 0; index3 < lonsCount; index3 += 4 ) {
          const double centralLongitude = lons[ index3 ];

          for ( size_t index4 = 0; index4 < latsCount; index4 += 3 ) {
            const double centralLatitude = lats[ index4 ];

            for ( size_t index5 = 0; index5 < latsCount; index5 += 2 ) {
              const double secantLatitude = lats[ index5 ];
              const emvl::Stereographic
                stereographic( majorSemiaxis, minorSemiaxis,
                               centralLongitude, centralLatitude,
                               secantLatitude );

              const emvl::MapProjection& mapProjection = stereographic;

              for ( size_t index6 = 0; index6 < lonsCount; ++index6 ) {
                const double longitude = lons[ index6 ];

                for ( size_t index7 = 0; index7 < latsCount; ++index7 ) {
                  const double latitude = lats[ index7 ];
                  double lon = 0.0;
                  double lat = 0.0;
                  double x = 0.0;
                  double y = 0.0;
                  double lon2 = 0.0;
                  double lat2 = 0.0;
                  stereographic.project( longitude, latitude, x, y );
                  stereographic.unproject( x, y, lon, lat );

#if 0
                  DEBUG( if ( ! IMPLIES( ! emvl::aboutEqual( fabs( latitude ), 90.0,
                                                       1e-2 ),
                                   OR2( emvl::aboutEqual(lon, longitude, 1e-2),
                                        emvl::aboutEqual( fabs( lon ), 180.0,
                                                           1e-2 ) ) ) )
fprintf( stderr, "\nfailed: (%lf, %lf)->(%lf, %lf)->(%lf, %lf).\n",
longitude, latitude, x, y, lon, lat ); )
#endif

#if 0
                  assert( IMPLIES( ! emvl::aboutEqual( fabs( latitude ), 90.0,
                                                       1e-2 ),
                                   OR2( emvl::aboutEqual(lon, longitude, 1e-2),
                                        emvl::aboutEqual( fabs( lon ), 180.0,
                                                           1e-2 ) ) ) );
#endif

#if 0
                  DEBUG( if ( ! IMPLIES_ELSE(emvl::aboutEqual(fabs(lon), 180.0, 1e-3),
emvl::aboutEqual(fabs(lat), fabs( latitude ), 1e-3),
emvl::aboutEqual( lat, latitude, 1e-3)))
fprintf( stderr, "\nfailed: (%lf, %lf)->(%lf, %lf)->(%lf, %lf).\n",
longitude, latitude, x, y, lon, lat ); )
#endif

#if 0
                  assert(IMPLIES_ELSE(emvl::aboutEqual(fabs(lon), 180.0, 1e-3),
                                      emvl::aboutEqual(fabs(lat),
                                                       fabs( latitude ), 1e-3),
                                      emvl::aboutEqual( lat, latitude, 1e-3)));
#endif
                  mapProjection.project( longitude, latitude, x, y );
                  mapProjection.unproject( x, y, lon2, lat2 );
                  assert( lon2 == lon );
                  assert( lat2 == lat );
                }
              }
            }
          }
        }
      }
    }
  }
}


/******************************************************************************
PURPOSE: testHTTPConnection - Test emvl::HTTPConnection class.
******************************************************************************/

static void testHTTPConnection() {

  printf( "testHTTPConnection()\n" );

  int test = 0;

  for ( ; test < 2; ++test ) {
    char program[ 256 ] = "";

    if ( test == 0 ) {
      snprintf( program, sizeof program / sizeof *program,
               "%s%c%s", curlPath, emvl::slash, curlCommand );
    } else if ( test == 1 ) {
      strncpy( program, "/no/such/program",
               sizeof program / sizeof *program - 1 );
    }

    program[ sizeof program / sizeof *program - 1 ] = '\0';

    try {
      fprintf( stderr, "initializing HTTP with program = %s\n", program );
      emvl::HTTPConnection::initialize( program ); //x
    } catch ( std::exception& e ) {
      fprintf( stderr, "%s\n", e.what() );
    }

    if ( emvl::HTTPConnection::isInitialized() ) {
  
      // Test https connection (only works when using curl utility!):

      if ( strstr( program, "curl " ) ) {

        printf( "Test reading https web page...\n" );

        try {
          const char* const url = "https://www.google.com/";
          printf( "url:\n%s\n", url );
          const int timeout = 300;
          emvl::HTTPConnection connection( url, timeout ); //x
          FILE* const stream = connection.getStream();
          assert( stream );
          int lines = 0;
          char line[ 4096 ] = "";
          memset( line, 0, sizeof line );

          while ( connection.read( sizeof line / sizeof *line, line ) ) {
            ++lines;
            printf( "%s\n", line );
          }

          printf( "Read %d lines.\n", lines );
          assert( lines > 0 );
        } catch ( std::exception& e ) {
          fprintf( stderr, "%s\n", e.what() );
          assert( 0 );
        }
      }

      // Test failed connection:

      printf( "Test failure cases...\n" );

      try {
        emvl::HTTPConnection connection( "http://no_such_host", 0 ); //x
      } catch ( std::exception& e ) {
        fprintf( stderr, "%s\n", e.what() );
      }

      // Test successful connection and reading of ASCII data:

      if ( test < 1 ) {
        printf( "Test successful cases...\n" );
      }

      try {
        const char* url =
          "http://ofmpub.epa.gov/rsig/rsigserver?"
          "SERVICE=wcs&VERSION=1.0.0&REQUEST=GetCoverage&"
          "COVERAGE=uvnet.irradiance&"
          "TIME=1996-01-01T00:00:00Z/1996-01-02T23:59:59Z&"
          "BBOX=-118,33,-117,35&FORMAT=ascii";
        printf( "url:\n%s\n", url );
        const int timeout = 300;
        emvl::HTTPConnection connection( url, timeout ); //x
        FILE* const stream = connection.getStream();
        assert( stream );
        int lines = 0;
        char line[ 256 ] = "";
        memset( line, 0, sizeof line );

        printf( "Reading data:...\n" );

        while ( connection.read( sizeof line / sizeof *line, line ) ) {
          char timestamp[ 24 + 1 ] = "";
          float longitude = 0.0;
          float latitude = 0.0;
          int station = 0;
          float data = 0.0;
          memset( timestamp, 0, sizeof timestamp );

          if ( lines == 0 ) {
            ++lines;
            printf( "%s\n", line ); // Print header line.
          } else if ( sscanf( line, "%24s %f %f %d %f",
                              timestamp, &longitude, &latitude, &station,
                              &data ) == 5 ) {
            ++lines;
            printf( "%s\t%f\t%f\t%d\t%f\n",
                    timestamp, longitude, latitude, station, data );
          }
        }

        printf( "Read %d lines.\n", lines );
      } catch ( std::exception& e ) {
        fprintf( stderr, "%s\n", e.what() );
      }

      // Test successful connection and reading of binary data:

      try {
        const char* const url =
          "http://ofmpub.epa.gov/rsig/rsigserver?"
          "SERVICE=wcs&VERSION=1.0.0&REQUEST=GetCoverage&"
          "COVERAGE=uvnet.irradiance&"
          "TIME=1996-01-01T00:00:00Z/1996-01-02T23:59:59Z&"
          "BBOX=-118,33,-117,35&FORMAT=xdr";
        printf( "url:\n%s\n", url );
        const int timeout = 300;
        emvl::HTTPConnection connection( url, timeout ); //x
        FILE* const stream = connection.getStream();
        assert( stream );
        const int headerLines = 14;
        int lines = 0;
        int timesteps = 0;
        int stations = 0;
        char line[ 256 ] = "";
        memset( line, 0, sizeof line );

        printf( "Reading header...\n" );

        while ( AND2( lines < headerLines,
                      connection.read( sizeof line / sizeof *line, line ) ) ) {
          ++lines;
          printf( "%s\n", line );

          if ( lines == 5 ) {
            sscanf( line, "%d %d", &timesteps, &stations );
            assert( timesteps > 0 );
            assert( stations > 0 );
          }
        }

        if ( AND3( lines == headerLines, timesteps > 0, stations > 0 ) ) {

          // Read 16-bit data:

          short sbuffer[ 2 ] = { 0, 0 };
          int ok = fread( sbuffer, 2, sizeof *sbuffer, stream ) == 2;
          assert( ok );
          ungetc( 0, stream ); // Push 4 bytes back onto stream.
          ungetc( 0, stream );
          ungetc( 0, stream );
          ungetc( 0, stream );
          int* ids = (int*) malloc( stations * sizeof (int) );

          if ( ids ) {
            memset( ids, 0, stations * sizeof (int) );
            ok = connection.read( stations, sizeof *ids, ids );
            assert( ok );
            free( ids ), ids = 0;

            // Read 32-bit data:

            float* sites = (float*) malloc( stations * 2 * sizeof (float) );

            if ( sites ) {
              ok = connection.read( stations * 2, sizeof *sites, sites );
              assert( ok );

              for ( int station = 0; station < stations; ++station ) {
                const int station2 = station + station;
                const float longitude = sites[ station2 ];
                const float latitude = sites[ station2 + 1 ];
                assert( emvl::isLongitudeLatitude( longitude, latitude ) );
              }

              free( sites ), sites = 0;

              // Read 64-bit data:

              const int count = timesteps * stations / 2;
              double* data = (double*) malloc( count * sizeof (double) );

              if ( data ) {
                ok = connection.read( count, sizeof *data, data );
                assert( ok );
                free( data ), data = 0;
              }
            }
          }
        }
      } catch ( std::exception& e ) {
        fprintf( stderr, "%s\n", e.what() );
      }
    }
  }
}



/******************************************************************************
PURPOSE: testDataFilterParameters - Test emvl::DataFilterParameters class.
******************************************************************************/

static void testDataFilterParameters() {

  printf( "testDataFilterParameters()\n" );

  const emvl::DataFilterParameters object;
  std::string key( "INVALID" );
  std::string dataVersion( "4.00" );

  assert( object.invariant() );
  assert( object == object );
  assert( ! ( object != object ) );
  assert( object.CALIPSOMinimumAcceptedCADScore() == 20.0 );
  assert( object.CALIPSOMaximumAcceptedUncertainty() == 99.0 );
  assert( object.GASPAODMinimum() == -0.5 );
  assert( object.GASPAODMaximum() == 2.05 );
  assert( object.GASPSTDMinimum() == 0.0 );
  assert( object.GASPSTDMaximum() == 2.55 );
  assert( object.GASPCLSMinimum() == 0.0 );
  assert( object.GASPCLSMaximum() == 25.0 );
  assert( object.GASPCH1Minimum() == 0.0 );
  assert( object.GASPCH1Maximum() == 0.425 );
  assert( object.GASPSIGMinimum() == -0.5 );
  assert( object.GASPSIGMaximum() == 0.52 );
  assert( object.GASPSFCMinimum() == -0.1 );
  assert( object.GASPSFCMaximum() == 0.41 );
  assert( object.GASPMOSMinimum() == 0.0 );
  assert( object.GASPMOSMaximum() == 0.425 );
  assert( object.GASPSCAMinimum() == 0.0 );
  assert( object.GASPSCAMaximum() == 255.0 );
  assert( object.TROPOMIMinimumAcceptedQuality() == 100 );
  assert( object.TROPOMIMaximumCloudFraction() == 1.0 );
  assert( object.TROPOMIGroundPixelMinimum() == emvl::IMISSING );
  assert( object.TROPOMIGroundPixelMaximum() == emvl::IMISSING );
  assert( object.TROPOMIAllowNegativeCounts() == false );
  assert( ! strcmp( object.VIIRSNOAAMinimumAcceptedQuality(), "high" ) );
  assert( ! strcmp( object.PandoraMinimumAcceptedQuality(), "high" ) );
  assert( ! strcmp( object.TEMPOMinimumAcceptedQuality(), "normal" ) );
  assert( object.TEMPOMaximumCloudFraction() == 1.0 );
  assert( object.TEMPOMaximumSolarZenithAngle() == 90.0 );
  assert( object.TEMPOAllowNegativeCounts() == false );
  assert( object.OMIMaximumCloudFraction() == 1.0 );
  assert( object.OMIMaximumSolarZenithAngle() == 90.0 );
  assert( object.OMIAllowNegativeCounts() == false );
  assert( object.TEMPOKey().length() == 0 );
  assert( object.OAQPSKey().length() == 0 );
  assert( object.PurpleAirAPIKey().length() == 0 );
  assert( object.PurpleAirOutInFlag() == 2 );
  assert( object.PurpleAirMaximumDifference() == 5.0 );
  assert( object.PurpleAirMaximumRatio() == 0.7 );
  assert( object.PurpleAirMinimumAggregationCountPercentage() == 75.0 );
  assert( object.PurpleAirDefaultHumidity() == emvl::MISSING );
  assert( object.dataVersion().length() == 0 );

  emvl::DataFilterParameters object2( object );

  assert( object2.invariant() );
  assert( object2 == object );
  assert( ! ( object2 != object ) );
  assert( object2.CALIPSOMinimumAcceptedCADScore() == 20.0 );
  assert( object2.CALIPSOMaximumAcceptedUncertainty() == 99.0 );
  assert( object2.GASPAODMinimum() == -0.5 );
  assert( object2.GASPAODMaximum() == 2.05 );
  assert( object2.GASPSTDMinimum() == 0.0 );
  assert( object2.GASPSTDMaximum() == 2.55 );
  assert( object2.GASPCLSMinimum() == 0.0 );
  assert( object2.GASPCLSMaximum() == 25.0 );
  assert( object2.GASPCH1Minimum() == 0.0 );
  assert( object2.GASPCH1Maximum() == 0.425 );
  assert( object2.GASPSIGMinimum() == -0.5 );
  assert( object2.GASPSIGMaximum() == 0.52 );
  assert( object2.GASPSFCMinimum() == -0.1 );
  assert( object2.GASPSFCMaximum() == 0.41 );
  assert( object2.GASPMOSMinimum() == 0.0 );
  assert( object2.GASPMOSMaximum() == 0.425 );
  assert( object2.GASPSCAMinimum() == 0.0 );
  assert( object2.GASPSCAMaximum() == 255.0 );
  assert( object2.TROPOMIMinimumAcceptedQuality() == 100 );
  assert( object2.TROPOMIMaximumCloudFraction() == 1.0 );
  assert( object2.TROPOMIGroundPixelMinimum() == emvl::IMISSING );
  assert( object2.TROPOMIGroundPixelMaximum() == emvl::IMISSING );
  assert( object2.TROPOMIAllowNegativeCounts() == false );
  assert( ! strcmp( object2.VIIRSNOAAMinimumAcceptedQuality(), "high" ) );
  assert( ! strcmp( object2.PandoraMinimumAcceptedQuality(), "high" ) );
  assert( ! strcmp( object2.TEMPOMinimumAcceptedQuality(), "normal" ) );
  assert( object2.TEMPOMaximumCloudFraction() == 1.0 );
  assert( object2.TEMPOMaximumSolarZenithAngle() == 90.0 );
  assert( object2.TEMPOAllowNegativeCounts() == false );
  assert( object2.OMIMaximumCloudFraction() == 1.0 );
  assert( object2.OMIMaximumSolarZenithAngle() == 90.0 );
  assert( object2.OMIAllowNegativeCounts() == false );
  assert( object2.TEMPOKey().length() == 0 );
  assert( object2.OAQPSKey().length() == 0 );
  assert( object2.PurpleAirAPIKey().length() == 0 );
  assert( object2.PurpleAirOutInFlag() == 2 );
  assert( object2.PurpleAirMaximumDifference() == 5.0 );
  assert( object2.PurpleAirMaximumRatio() == 0.7 );
  assert( object2.PurpleAirMinimumAggregationCountPercentage() == 75.0 );
  assert( object2.PurpleAirDefaultHumidity() == emvl::MISSING );
  assert( object2.dataVersion().length() == 0 );

  assert( emvl::DataFilterParameters::isValidParameters(
                                      20.0,  99.0,
                                      -0.5,   2.05,
                                       0.0,   2.55,
                                       0.0,  25.0,
                                       0.0,   0.425,
                                      -0.5,   0.52,
                                      -0.1,   0.41,
                                       0.0,   0.425,
                                       0.0, 255.0,
                                      100,
                                      1.0,
                                      0,
                                      449,
                                      true,
                                      "high",
                                      "high",
                                      "normal",
                                      1.0,
                                      90.0,
                                      true,
                                      1.0,
                                      90.0,
                                      true,
                                      key,
                                      key,
                                      key,
                                      0,
                                      1.0,
                                      0.9,
                                      0.0,
                                      50.0,
                                      dataVersion ) );

  emvl::DataFilterParameters object3( 20.0,  99.0,
                                      -0.5,   2.05,
                                       0.0,   2.55,
                                       0.0,  25.0,
                                       0.0,   0.425,
                                      -0.5,   0.52,
                                      -0.1,   0.41,
                                       0.0,   0.425,
                                       0.0, 255.0,
                                      100,
                                      1.0,
                                      0,
                                      449,
                                      true,
                                      "high",
                                      "high",
                                      "normal",
                                      1.0,
                                      90.0,
                                      true,
                                      1.0,
                                      90.0,
                                      true,
                                      key,
                                      key,
                                      key,
                                      0,
                                      1.0,
                                      0.9,
                                      0.0,
                                      50.0,
                                      dataVersion );

  assert( object3.CALIPSOMinimumAcceptedCADScore() == 20.0 );
  assert( object3.CALIPSOMaximumAcceptedUncertainty() == 99.0 );
  assert( object3.GASPAODMinimum() == -0.5 );
  assert( object3.GASPAODMaximum() == 2.05 );
  assert( object3.GASPSTDMinimum() == 0.0 );
  assert( object3.GASPSTDMaximum() == 2.55 );
  assert( object3.GASPCLSMinimum() == 0.0 );
  assert( object3.GASPCLSMaximum() == 25.0 );
  assert( object3.GASPCH1Minimum() == 0.0 );
  assert( object3.GASPCH1Maximum() == 0.425 );
  assert( object3.GASPSIGMinimum() == -0.5 );
  assert( object3.GASPSIGMaximum() == 0.52 );
  assert( object3.GASPSFCMinimum() == -0.1 );
  assert( object3.GASPSFCMaximum() == 0.41 );
  assert( object3.GASPMOSMinimum() == 0.0 );
  assert( object3.GASPMOSMaximum() == 0.425 );
  assert( object3.GASPSCAMinimum() == 0.0 );
  assert( object3.GASPSCAMaximum() == 255.0 );
  assert( object3.TROPOMIMinimumAcceptedQuality() == 100 );
  assert( object3.TROPOMIMaximumCloudFraction() == 1.0 );
  assert( object3.TROPOMIGroundPixelMinimum() == 0 );
  assert( object3.TROPOMIGroundPixelMaximum() == 449 );
  assert( object3.TROPOMIAllowNegativeCounts() == true );
  assert( ! strcmp( object3.VIIRSNOAAMinimumAcceptedQuality(), "high" ) );
  assert( ! strcmp( object3.PandoraMinimumAcceptedQuality(), "high" ) );
  assert( ! strcmp( object3.TEMPOMinimumAcceptedQuality(), "normal" ) );
  assert( object3.TEMPOMaximumCloudFraction() == 1.0 );
  assert( object3.TEMPOMaximumSolarZenithAngle() == 90.0 );
  assert( object3.TEMPOAllowNegativeCounts() == true );
  assert( object3.OMIMaximumCloudFraction() == 1.0 );
  assert( object3.OMIMaximumSolarZenithAngle() == 90.0 );
  assert( object3.OMIAllowNegativeCounts() == true );
  assert( object3.TEMPOKey() == "INVALID" );
  assert( object3.OAQPSKey() == "INVALID" );
  assert( object3.PurpleAirAPIKey() == "INVALID" );
  assert( object3.PurpleAirOutInFlag() == 0 );
  assert( object3.PurpleAirMaximumDifference() == 1.0 );
  assert( object3.PurpleAirMaximumRatio() == 0.9 );
  assert( object3.PurpleAirMinimumAggregationCountPercentage() == 0.0 );
  assert( object3.PurpleAirDefaultHumidity() == 50.0 );
  assert( object3.dataVersion() == "4.00" );

  object3.CALIPSOMinimumAcceptedCADScore( 100.0 );
  object3.CALIPSOMaximumAcceptedUncertainty( 50.0 );
  object3.GASPAODRange( 0.0, 1.0 );
  object3.GASPSTDRange( 0.0, 0.1 );
  object3.GASPCLSRange( 0.0, 0.0 );
  object3.GASPCH1Range( 0.1, 0.425 );
  object3.GASPSIGRange( 0.1, 0.52 );
  object3.GASPSFCRange( -0.1, -0.1 );
  object3.GASPMOSRange( 0.1, 0.1 );
  object3.GASPSCARange( 0.2, 200.0 );
  object3.TROPOMIMinimumAcceptedQuality( 50 );
  object3.TROPOMIMaximumCloudFraction( 0.5 );
  object3.TROPOMIGroundPixelRange( 100, 300 );
  object3.TROPOMIAllowNegativeCounts( false );
  object3.VIIRSNOAAMinimumAcceptedQuality( "medium" );
  object3.PandoraMinimumAcceptedQuality( "medium" );
  object3.TEMPOMinimumAcceptedQuality( "suspect" );
  object3.TEMPOMaximumCloudFraction( 0.5 );
  object3.TEMPOMaximumSolarZenithAngle( 70.0 );
  object3.TEMPOAllowNegativeCounts( false );
  object3.OMIMaximumCloudFraction( 0.5 );
  object3.OMIMaximumSolarZenithAngle( 70.0 );
  object3.OMIAllowNegativeCounts( false );
  object3.TEMPOKey( "INVALID2" );
  object3.OAQPSKey( "INVALID2" );
  object3.PurpleAirAPIKey( "INVALID2" );
  object3.PurpleAirOutInFlag( 1 );
  object3.PurpleAirMaximumDifference( 0.5 );
  object3.PurpleAirMaximumRatio( 0.95 );
  object3.PurpleAirMinimumAggregationCountPercentage( 100.0 );
  object3.PurpleAirDefaultHumidity( emvl::MISSING );
  object3.dataVersion( "4.21" );

  assert( object3.CALIPSOMinimumAcceptedCADScore() == 100.0 );
  assert( object3.CALIPSOMaximumAcceptedUncertainty() == 50.0 );
  assert( object3.GASPAODMinimum() == 0.0 );
  assert( object3.GASPAODMaximum() == 1.0 );
  assert( object3.GASPSTDMinimum() == 0.0 );
  assert( object3.GASPSTDMaximum() == 0.1 );
  assert( object3.GASPCLSMinimum() == 0.0 );
  assert( object3.GASPCLSMaximum() == 0.0 );
  assert( object3.GASPCH1Minimum() == 0.1 );
  assert( object3.GASPCH1Maximum() == 0.425 );
  assert( object3.GASPSIGMinimum() == 0.1 );
  assert( object3.GASPSIGMaximum() == 0.52 );
  assert( object3.GASPSFCMinimum() == -0.1 );
  assert( object3.GASPSFCMaximum() == -0.1 );
  assert( object3.GASPMOSMinimum() == 0.1 );
  assert( object3.GASPMOSMaximum() == 0.1 );
  assert( object3.GASPSCAMinimum() == 0.2 );
  assert( object3.GASPSCAMaximum() == 200.0 );
  assert( object3.TROPOMIMinimumAcceptedQuality() == 50 );
  assert( object3.TROPOMIMaximumCloudFraction() == 0.5 );
  assert( object3.TROPOMIGroundPixelMinimum() == 100 );
  assert( object3.TROPOMIGroundPixelMaximum() == 300 );
  assert( object3.TROPOMIAllowNegativeCounts() == false );
  assert( ! strcmp( object3.VIIRSNOAAMinimumAcceptedQuality(), "medium" ) );
  assert( ! strcmp( object3.PandoraMinimumAcceptedQuality(), "medium" ) );
  assert( ! strcmp( object3.TEMPOMinimumAcceptedQuality(), "suspect" ) );
  assert( object3.TEMPOMaximumCloudFraction() == 0.5 );
  assert( object3.TEMPOMaximumSolarZenithAngle() == 70.0 );
  assert( object3.TEMPOAllowNegativeCounts() == false );
  assert( object3.OMIMaximumCloudFraction() == 0.5 );
  assert( object3.OMIMaximumSolarZenithAngle() == 70.0 );
  assert( object3.OMIAllowNegativeCounts() == false );
  assert( object3.TEMPOKey() == "INVALID2" );
  assert( object3.OAQPSKey() == "INVALID2" );
  assert( object3.PurpleAirAPIKey() == "INVALID2" );
  assert( object3.PurpleAirOutInFlag() == 1 );
  assert( object3.PurpleAirMaximumDifference() == 0.5 );
  assert( object3.PurpleAirMaximumRatio() == 0.95 );
  assert( object3.PurpleAirMinimumAggregationCountPercentage() == 100.0 );
  assert( object3.PurpleAirDefaultHumidity() == emvl::MISSING );
  assert( object3.dataVersion() == "4.21" );

  assert( object3 != object );
  assert( ! ( object3 == object ) );

  object3 = object;
  assert( object3 == object );
  assert( ! ( object3 != object ) );
  assert( object3.CALIPSOMinimumAcceptedCADScore() == 20.0 );
  assert( object3.CALIPSOMaximumAcceptedUncertainty() == 99.0 );
  assert( object3.GASPAODMinimum() == -0.5 );
  assert( object3.GASPAODMaximum() == 2.05 );
  assert( object3.GASPSTDMinimum() == 0.0 );
  assert( object3.GASPSTDMaximum() == 2.55 );
  assert( object3.GASPCLSMinimum() == 0.0 );
  assert( object3.GASPCLSMaximum() == 25.0 );
  assert( object3.GASPCH1Minimum() == 0.0 );
  assert( object3.GASPCH1Maximum() == 0.425 );
  assert( object3.GASPSIGMinimum() == -0.5 );
  assert( object3.GASPSIGMaximum() == 0.52 );
  assert( object3.GASPSFCMinimum() == -0.1 );
  assert( object3.GASPSFCMaximum() == 0.41 );
  assert( object3.GASPMOSMinimum() == 0.0 );
  assert( object3.GASPMOSMaximum() == 0.425 );
  assert( object3.GASPSCAMinimum() == 0.0 );
  assert( object3.GASPSCAMaximum() == 255.0 );
  assert( object3.TROPOMIMinimumAcceptedQuality() == 100 );
  assert( object3.TROPOMIMaximumCloudFraction() == 1.0 );
  assert( object3.TROPOMIGroundPixelMinimum() == emvl::IMISSING );
  assert( object3.TROPOMIGroundPixelMaximum() == emvl::IMISSING );
  assert( object3.TROPOMIAllowNegativeCounts() == false );
  assert( ! strcmp( object3.VIIRSNOAAMinimumAcceptedQuality(), "high" ) );
  assert( ! strcmp( object3.PandoraMinimumAcceptedQuality(), "high" ) );
  assert( ! strcmp( object3.TEMPOMinimumAcceptedQuality(), "normal" ) );
  assert( object3.TEMPOMaximumCloudFraction() == 1.0 );
  assert( object3.TEMPOMaximumSolarZenithAngle() == 90.0 );
  assert( object3.TEMPOAllowNegativeCounts() == false );
  assert( object3.OMIMaximumCloudFraction() == 1.0 );
  assert( object3.OMIMaximumSolarZenithAngle() == 90.0 );
  assert( object3.OMIAllowNegativeCounts() == false );
  assert( object3.TEMPOKey().length() == 0 );
  assert( object3.OAQPSKey().length() == 0 );
  assert( object3.PurpleAirAPIKey().length() == 0 );
  assert( object3.PurpleAirOutInFlag() == 2 );
  assert( object3.PurpleAirMaximumDifference() == 5.0 );
  assert( object3.PurpleAirMaximumRatio() == 0.7 );
  assert( object3.PurpleAirMinimumAggregationCountPercentage() == 75.0 );
  assert( object3.PurpleAirDefaultHumidity() == emvl::MISSING );
  assert( object3.dataVersion().length() == 0 );
}



/******************************************************************************
PURPOSE: testCMAQGridParameters - Test emvl::CMAQGridParameters class.
******************************************************************************/

static void testCMAQGridParameters() {

  printf( "testCMAQGridParameters()\n" );

  assert( emvl::CMAQGridParameters::LATGRD3 );
  assert( emvl::CMAQGridParameters::LAMGRD3 );
  assert( emvl::CMAQGridParameters::POLGRD3 );
  assert( emvl::CMAQGridParameters::EQMGRD3 );
  assert( emvl::CMAQGridParameters::MXLAYS3 > 0 );
  assert( emvl::CMAQGridParameters::IMISS3 );
  assert( emvl::CMAQGridParameters::VGSGPH3 );
  assert( emvl::CMAQGridParameters::VGSGPN3 );
  assert( emvl::CMAQGridParameters::VGSIGZ3 );
  assert( emvl::CMAQGridParameters::VGZVAL3 );
  assert( emvl::CMAQGridParameters::VGHVAL3 );
  assert( emvl::CMAQGridParameters::VGWRFEM );

  const double vglvls[ 2 ] = { 1.0, 0.0 };

  assert( ! emvl::CMAQGridParameters::isValidParameters(
              1, 1, 1, 2, 6370000.0, 33.0, 60.0, -90.0, -90.0, 40.0,
              -1000000.0, -1000000.0, 12000.0, 12000.0,
              emvl::CMAQGridParameters::VGSGPN3, 5000.0, vglvls,
              9.81, 287.04, 50.0, 275.0, 0.0 ) );

  const emvl::CMAQGridParameters object;
  printCMAQGridParameters( object );

  assert( object.invariant() );
  assert( object == object );
  assert( ! ( object != object ) );
  assert( object.ncols() == 459 );
  assert( object.nrows() == 299 );
  assert( object.nlays() == 34 );
  assert( object.gdtyp() == 2 );
  assert( object.sphereRadius() == 6370000.0 );
  assert( object.p_alp() == 33.0 );
  assert( object.p_bet() == 45.0 );
  assert( object.p_gam() == -97.0 );
  assert( object.xcent() == -97.0 );
  assert( object.ycent() == 40.0 );
  assert( object.xorig() == -2556000.0 );
  assert( object.yorig() == -1728000.0 );
  assert( object.xcell() == 12000.0 );
  assert( object.ycell() == 12000.0 );
  assert( object.vgtop() == 5000.0 );
  assert( object.vgtyp() == emvl::CMAQGridParameters::VGSGPN3 );
  assert( object.vglvls() );
  assert( object.g() == 9.81 );
  assert( object.R() == 287.04 );
  assert( object.A() == 50.0 );
  assert( object.T0() == 275.0 );
  assert( object.P0() == 100000.0 );

  emvl::CMAQGridParameters object2( object );
  printCMAQGridParameters( object2 );

  assert( object2.invariant() );
  assert( object2 == object );
  assert( ! ( object2 != object ) );
  assert( object2.ncols() == 459 );
  assert( object2.nrows() == 299 );
  assert( object2.nlays() == 34 );
  assert( object2.gdtyp() == 2 );
  assert( object2.sphereRadius() == 6370000.0 );
  assert( object2.p_alp() == 33.0 );
  assert( object2.p_bet() == 45.0 );
  assert( object2.p_gam() == -97.0 );
  assert( object2.xcent() == -97.0 );
  assert( object2.ycent() == 40.0 );
  assert( object2.xorig() == -2556000.0 );
  assert( object2.yorig() == -1728000.0 );
  assert( object2.xcell() == 12000.0 );
  assert( object2.ycell() == 12000.0 );
  assert( object2.vgtop() == 5000.0 );
  assert( object2.vgtyp() == emvl::CMAQGridParameters::VGSGPN3 );
  assert( object2.g() == 9.81 );
  assert( object2.R() == 287.04 );
  assert( object2.A() == 50.0 );
  assert( object2.T0() == 275.0 );
  assert( object2.P0() == 100000.0 );

  object2 = object;
  printCMAQGridParameters( object2 );

  assert( object2.invariant() );
  assert( object2 == object );
  assert( ! ( object2 != object ) );
  assert( object2.ncols() == 459 );
  assert( object2.nrows() == 299 );
  assert( object2.nlays() == 34 );
  assert( object2.gdtyp() == 2 );
  assert( object2.sphereRadius() == 6370000.0 );
  assert( object2.p_alp() == 33.0 );
  assert( object2.p_bet() == 45.0 );
  assert( object2.p_gam() == -97.0 );
  assert( object2.xcent() == -97.0 );
  assert( object2.ycent() == 40.0 );
  assert( object2.xorig() == -2556000.0 );
  assert( object2.yorig() == -1728000.0 );
  assert( object2.xcell() == 12000.0 );
  assert( object2.ycell() == 12000.0 );
  assert( object2.vgtop() == 5000.0 );
  assert( object2.vgtyp() == emvl::CMAQGridParameters::VGSGPN3 );
  assert( object2.g() == 9.81 );
  assert( object2.R() == 287.04 );
  assert( object2.A() == 50.0 );
  assert( object2.T0() == 275.0 );
  assert( object2.P0() == 100000.0 );

  {
    const double vglvls[ 25 ] = {
      1.0, 0.995, 0.99, 0.98, 0.97, 0.96, 0.94, 0.92, 0.9,
      0.88, 0.86, 0.84, 0.82, 0.8, 0.77, 0.74, 0.7, 0.65, 0.6,
      0.5, 0.4, 0.3, 0.2, 0.1, 0.0
    };
    const emvl::CMAQGridParameters cdc_east( 279, 240, 24, 2, 6370000.0,
                                             33.0, 45.0, -97.0, -97.0, 40.0,
                                             -1008000.0, -1620000.0,
                                             12000.0, 12000.0,
                                             2, 10000.0, vglvls,
                                             9.81, 287.04, 50.0, 290.0,
                                             1000000.0 );
    printCMAQGridParameters( cdc_east );
  }
  {
    const double vglvls[ 25 ] = {
      1.0, 0.995, 0.99, 0.98, 0.97, 0.96, 0.94, 0.92, 0.9,
      0.88, 0.86, 0.84, 0.82, 0.8, 0.77, 0.74, 0.7, 0.65, 0.6,
      0.5, 0.4, 0.3, 0.2, 0.1, 0.0
    };
    const emvl::CMAQGridParameters conus( 148, 112, 24, 2, 6370000.0,
                                          33.0, 45.0, -97.0, -97.0, 40.0,
                                          -2736000.0, -208800.0,
                                          36000.0, 36000.0,
                                          2, 10000.0, vglvls,
                                          9.81, 287.04, 50.0, 290.0,
                                          1000000.0 );
    printCMAQGridParameters( conus );
  }
  {
    const double vglvls[ 36 ] = {
      1.0, 0.9975, 0.995, 0.99, 0.985, 0.98, 0.97, 0.96, 0.95, 0.94, 0.93,
      0.92, 0.91, 0.9, 0.88, 0.86, 0.84, 0.82, 0.8,
      0.77, 0.74, 0.7,
      0.65, 0.6, 0.55, 0.5, 0.45, 0.4, 0.35, 0.3, 0.25, 0.2,
      0.15, 0.1, 0.05, 0.0
    };
    const emvl::CMAQGridParameters hemi( 187, 187, 35, 6, 6370000.0,
                                         1.0, 45.0, -98.0, -98.0, 90.0,
                                         -10098000.0, -10098000.0,
                                         108000.0, 108000.0,
                                         7, 5000.0, vglvls,
                                         9.81, 287.04, 50.0, 275.0,
                                         1000000.0 );
    printCMAQGridParameters( hemi );
  }

  const emvl::CMAQGridParameters valid( validCMAQFileName );
  printCMAQGridParameters( valid );

  try {
    printf( "Testing failure case...\n" );
    const emvl::CMAQGridParameters invalid( invalidCMAQFileName ); //x
  } catch ( std::exception& e ) {
    printf( "Caught expected exception: %s\n", e.what() );
  }
}



/******************************************************************************
PURPOSE: printCMAQGridParameters - Print emvl::CMAQGridParameters object.
INPUTS:  const emvl::CMAQGridParameters& grid  Object to print.
******************************************************************************/

static void printCMAQGridParameters( const emvl::CMAQGridParameters& grid ) {

  printf( "\nCMAQGridParameters:\n" );

  const int ncols = grid.ncols();
  const int nrows = grid.nrows();
  const int nlays = grid.nlays();
  const int gdtyp = grid.gdtyp();
  const double sphereRadius = grid.sphereRadius();
  const double p_alp = grid.p_alp();
  const double p_bet = grid.p_bet();
  const double p_gam = grid.p_gam();
  const double xcent = grid.xcent();
  const double ycent = grid.ycent();
  const double xorig = grid.xorig();
  const double yorig = grid.yorig();
  const double xcell = grid.xcell();
  const double ycell = grid.ycell();
  const int vgtyp = grid.vgtyp(); 
  const double vgtop = grid.vgtop();
  const double* const vglvls = grid.vglvls();
  const double g = grid.g();
  const double R = grid.R();
  const double A = grid.A();
  const double T0 = grid.T0();
  const double P0 = grid.P0();

  printf( "ncols = %d\n", ncols );
  printf( "nrows = %d\n", nrows );
  printf( "nlays = %d\n", nlays );
  printf( "gdtyp = %d\n", gdtyp );
  printf( "sphereRadius = %lf\n", sphereRadius );
  printf( "p_alp = %lf\n", p_alp );
  printf( "p_bet = %lf\n", p_bet );
  printf( "p_gam = %lf\n", p_gam );
  printf( "xcent = %lf\n", xcent );
  printf( "ycent = %lf\n", ycent );
  printf( "xorig = %lf\n", xorig );
  printf( "yorig = %lf\n", yorig );
  printf( "xcell = %lf\n", xcell );
  printf( "ycell = %lf\n", ycell );
  printf( "vgtyp = %d\n", vgtyp );
  printf( "vgtop = %lf\n", vgtop );
  printf( "vglvls:\n" );

  for ( int level = 0; level <= nlays; ++level ) {
    printf( "%lf ", vglvls[ level ] );
  }

  printf( "\n" );
  printf( "g = %lf\n", g );
  printf( "R = %lf\n", R );
  printf( "A = %lf\n", A );
  printf( "T0 = %lf\n", T0 );
  printf( "P0 = %lf\n", P0 );
  printf( "\n\n" );

}


/******************************************************************************
PURPOSE: testQueryParameters - Test emvl::QueryParameters class.
******************************************************************************/

static void testQueryParameters() {

  printf( "testQueryParameters()\n" );

  assert( IN_RANGE( emvl::QueryParameters::MINIMUM_ELEVATION, -1e3, 1e6 ) );
  assert( IN_RANGE( emvl::QueryParameters::MAXIMUM_ELEVATION,
                    emvl::QueryParameters::MINIMUM_ELEVATION, 1e6 ) );

  assert( IN_RANGE( emvl::QueryParameters::XDR,
                    0, emvl::QueryParameters::FILE_FORMATS - 1 ) );
  assert( IN_RANGE( emvl::QueryParameters::ASCII,
                    0, emvl::QueryParameters::FILE_FORMATS - 1 ) );
  assert( IN_RANGE( emvl::QueryParameters::NETCDF_COARDS,
                    0, emvl::QueryParameters::FILE_FORMATS - 1 ) );
  assert( IN_RANGE( emvl::QueryParameters::NETCDF_IOAPI,
                    0, emvl::QueryParameters::FILE_FORMATS - 1 ) );
  assert( IN_RANGE( emvl::QueryParameters::SHAPEFILE,
                    0, emvl::QueryParameters::FILE_FORMATS - 1 ) );
  assert( IN_RANGE( emvl::QueryParameters::KML,
                    0, emvl::QueryParameters::FILE_FORMATS - 1 ) );

  assert( IN_RANGE( emvl::QueryParameters::REGRID_NONE,
                    0, emvl::QueryParameters::REGRID_METHODS - 1 ) );
  assert( IN_RANGE( emvl::QueryParameters::REGRID_NEAREST,
                    0, emvl::QueryParameters::REGRID_METHODS - 1 ) );
  assert( IN_RANGE( emvl::QueryParameters::REGRID_MEAN,
                    0, emvl::QueryParameters::REGRID_METHODS - 1 ) );
  assert( IN_RANGE( emvl::QueryParameters::REGRID_WEIGHTED,
                    0, emvl::QueryParameters::REGRID_METHODS - 1 ) );

  assert( IN_RANGE( emvl::QueryParameters::AGGREGATE_NONE,
                    0, emvl::QueryParameters::AGGREGATE_METHODS - 1 ) );
  assert( IN_RANGE( emvl::QueryParameters::AGGREGATE_DAILY_MEAN,
                    0, emvl::QueryParameters::AGGREGATE_METHODS - 1 ) );
  assert( IN_RANGE( emvl::QueryParameters::AGGREGATE_DAILY_MAX,
                    0, emvl::QueryParameters::AGGREGATE_METHODS - 1 ) );
  assert( IN_RANGE( emvl::QueryParameters::AGGREGATE_DAILY_MAX8,
                    0, emvl::QueryParameters::AGGREGATE_METHODS - 1 ) );

  assert( IN_RANGE( emvl::QueryParameters::REGRID_AGGREGATE_NONE,
                    0, emvl::QueryParameters::REGRID_AGGREGATE_METHODS - 1 ) );
  assert( IN_RANGE( emvl::QueryParameters::REGRID_AGGREGATE_ALL,
                    0, emvl::QueryParameters::REGRID_AGGREGATE_METHODS - 1 ) );
  assert( IN_RANGE( emvl::QueryParameters::REGRID_AGGREGATE_DAILY,
                    0, emvl::QueryParameters::REGRID_AGGREGATE_METHODS - 1 ) );

  assert( emvl::QueryParameters::fileFormatNames[ 0 ] );
  assert( emvl::QueryParameters::fileFormatNames[ 0 ][ 0 ] );
  assert( emvl::QueryParameters::fileFormatNames[
            emvl::QueryParameters::FILE_FORMATS - 1 ] );
  assert( emvl::QueryParameters::fileFormatNames[
            emvl::QueryParameters::FILE_FORMATS - 1 ][ 0 ] );
  assert( emvl::QueryParameters::fileFormatExtensions[ 0 ] );
  assert( emvl::QueryParameters::fileFormatExtensions[ 0 ][ 0 ] );
  assert( emvl::QueryParameters::fileFormatExtensions[
            emvl::QueryParameters::FILE_FORMATS - 1 ] );
  assert( emvl::QueryParameters::fileFormatExtensions[
            emvl::QueryParameters::FILE_FORMATS - 1 ][ 0 ] );

  const emvl::QueryParameters object;

  assert( object.invariant() );
  assert( object == object );
  assert( ! ( object != object ) );
  assert( object.startingTimestamp() <= object.endingTimestamp() );
  assert( object.bounds().invariant() );
  assert( object.boundsZMinimum() <= object.boundsZMaximum() );
  assert( object.variables() == "aqs.pm25" );
  assert( object.format() == emvl::QueryParameters::XDR );
  assert( object.directory() == "." );
  assert( object.compress() == false );
  assert( object.integrateCMAQLayers() == false );
  assert( object.dataFilterParameters().invariant() );
  assert( object.aggregateMethod() == emvl::QueryParameters::AGGREGATE_NONE );
  assert( object.regridMethod() == emvl::QueryParameters::REGRID_NONE );
  assert( object.regridAggregateMethod() == emvl::QueryParameters::REGRID_AGGREGATE_NONE );
  assert( object.cmaqGridParameters().invariant() );

  emvl::QueryParameters object2( object );

  assert( object2.invariant() );
  assert( object2 == object );
  assert( ! ( object2 != object ) );
  assert( object2.startingTimestamp() <= object.endingTimestamp() );
  assert( object2.bounds().invariant() );
  assert( object2.boundsZMinimum() <= object.boundsZMaximum() );
  assert( object2.variables() == "aqs.pm25" );
  assert( object2.format() == emvl::QueryParameters::XDR );
  assert( object2.directory() == "." );
  assert( object2.compress() == false );
  assert( object2.integrateCMAQLayers() == false );
  assert( object2.dataFilterParameters().invariant() );
  assert( object2.aggregateMethod() == emvl::QueryParameters::AGGREGATE_NONE );
  assert( object2.regridMethod() == emvl::QueryParameters::REGRID_NONE );
  assert( object2.regridAggregateMethod() == emvl::QueryParameters::REGRID_AGGREGATE_NONE );
  assert( object2.cmaqGridParameters().invariant() );

  object2 = object;

  assert( object2.invariant() );
  assert( object2 == object );
  assert( ! ( object2 != object ) );
  assert( object2.startingTimestamp() <= object.endingTimestamp() );
  assert( object2.bounds().invariant() );
  assert( object2.boundsZMinimum() <= object.boundsZMaximum() );
  assert( object2.variables() == "aqs.pm25" );
  assert( object2.format() == emvl::QueryParameters::XDR );
  assert( object2.directory() == "." );
  assert( object2.compress() == false );
  assert( object2.integrateCMAQLayers() == false );
  assert( object2.dataFilterParameters().invariant() );
  assert( object2.aggregateMethod() == emvl::QueryParameters::AGGREGATE_NONE );
  assert( object2.regridMethod() == emvl::QueryParameters::REGRID_NONE );
  assert( object2.regridAggregateMethod() == emvl::QueryParameters::REGRID_AGGREGATE_NONE );
  assert( object2.cmaqGridParameters().invariant() );

  emvl::Timestamp startingTimestamp = object2.startingTimestamp();
  startingTimestamp.offsetHours( 24 );
  emvl::Timestamp endingTimestamp = object2.endingTimestamp();
  endingTimestamp.offsetHours( 48 );

  object2.timestamps( startingTimestamp, endingTimestamp );
  assert( object2.startingTimestamp().yyyymmdd() == 20060704 );
  assert( object2.startingTimestamp().hhmmss() == 0 );
  assert( object2.endingTimestamp().yyyymmdd() == 20060705 );
  assert( object2.endingTimestamp().hhmmss() == 10000 );

  object2.bounds( emvl::Bounds( -90.0, -70.0, 30.0, 50.0 ) );
  assert( object2.bounds().west() == -90.0 );
  assert( object2.bounds().east() == -70.0 );
  assert( object2.bounds().south() == 30.0 );
  assert( object2.bounds().north() == 50.0 );

  const double boundsZ[ 2 ] = { 0.0, 17000.0 };
  object2.boundsZ( boundsZ );
  assert( object2.boundsZMinimum() == 0.0 );
  assert( object2.boundsZMaximum() == 17000.0 );

  object2.variables( "aqs.ozone" );
  assert( object2.variables() == "aqs.ozone" );

  object2.format( emvl::QueryParameters::ASCII );
  assert( object2.format() == emvl::QueryParameters::ASCII );
  object2.compress( true );
  assert( object2.compress() == true );
  object2.integrateCMAQLayers( true );
  assert( object2.integrateCMAQLayers() == true );

#if ! defined( _WIN32 )
  object2.directory( "/tmp" );
  assert( object2.directory() == "/tmp" );
  std::string name( object2.pathedFileName() );
  printf( "%s\n", name.c_str() );
  assert( ! strcmp( name.c_str(),
                    "/tmp/aqs_ozone_2006-07-04_to_2006-07-05.txt" ) );
#endif

  object2.compress( true );
  assert( object2.compress() == true );
  object2.integrateCMAQLayers( true );
  assert( object2.integrateCMAQLayers() == true );

  emvl::DataFilterParameters dataFilterParameters;
  object2.dataFilterParameters( dataFilterParameters );
  assert( object2.dataFilterParameters() == dataFilterParameters );

  object2.aggregateMethod( emvl::QueryParameters::AGGREGATE_NONE );
  assert( object2.aggregateMethod() == emvl::QueryParameters::AGGREGATE_NONE );

  object2.regridMethod( emvl::QueryParameters::REGRID_NEAREST );
  assert( object2.regridMethod() == emvl::QueryParameters::REGRID_NEAREST );

  object2.regridAggregateMethod( emvl::QueryParameters::REGRID_AGGREGATE_ALL );
  assert( object2.regridAggregateMethod() == emvl::QueryParameters::REGRID_AGGREGATE_ALL );

  emvl::CMAQGridParameters cmaqGridParameters;
  object2.cmaqGridParameters( cmaqGridParameters );
  assert( object2.cmaqGridParameters() == cmaqGridParameters );

  const emvl::QueryParameters object3(
    object2.startingTimestamp(),
    object2.endingTimestamp(),
    object2.bounds(),
    boundsZ,
    object2.variables(),
    object2.format(),
    object2.directory(),
    object2.getMetadata(),
    object2.compress(),
    object2.integrateCMAQLayers(),
    object2.dataFilterParameters(),
    object2.aggregateMethod(),
    object2.regridMethod(),
    object2.regridAggregateMethod(),
    object2.cmaqGridParameters() );
  assert( object3 == object2 );
}



/******************************************************************************
PURPOSE: testDataset - Test emvl::Dataset class.
******************************************************************************/

static void testDataset() {

  printf( "testDataset()\n" );

  assert( IN_RANGE( emvl::Dataset::POINT,
                    0, emvl::Dataset::CELL_TYPES - 1 ) );
  assert( IN_RANGE( emvl::Dataset::QUADRILATERAL,
                    0, emvl::Dataset::CELL_TYPES - 1 ) );
  assert( IN_RANGE( emvl::Dataset::HEXAHEDRON,
                    0, emvl::Dataset::CELL_TYPES - 1 ) );
  const emvl::Point p1 = { 1.1, 2.2, 3.3 };
  const emvl::Point p2( p1 );
  assert( isPoint( p1 ) );
  assert( isPoint( p2 ) );
  assert( p1 == p2 );
  assert( ! ( p1 != p2 ) );
  const emvl::Dataset::SubsetPointers subset =
    { 0, 0, 0, 0, 0, 0 };
  assert( subset.coordinatesCount == 0 );
  assert( subset.coordinates == 0 );
  assert( subset.dataCount == 0 );
  assert( subset.data == 0 );
  assert( subset.notesCount == 0 );
  assert( subset.notes == 0 );
}



/******************************************************************************
PURPOSE: testDataset - Test an instance of a class derived from emvl::Dataset.
INPUTS:  const emvl::Dataset& dataset  Dataset to test.
         const bool writeKML           Write dataset to KML file?
******************************************************************************/

static void testDataset( const emvl::Dataset& dataset, const bool writeKML ) {

  printf( "testDataset(instance of derived class, writeKML = %d)\n", writeKML);

  assert( dataset.invariant() );
  assert( dataset.name().length() );
  printf( "name: %s\n", dataset.name().c_str() );
  assert( dataset.description().length() );
  printf( "description: %s\n", dataset.description().c_str() );
  const emvl::Bounds& bounds = dataset.bounds();
  printf( "bounds: [%lf %lf][ %lf %lf]\n",
          bounds.west(), bounds.east(), bounds.south(), bounds.north() );
  emvl::Timestamp::UTCString utcString = "";
  dataset.startingTimestamp().toUTCString( utcString );
  printf( "startingTimestamp: %s\n", utcString );
  dataset.endingTimestamp().toUTCString( utcString );
  printf( "endingTimestamp: %s\n", utcString );
  printf( "timesteps: %" LU "\n", dataset.timesteps() );
  printf( "variables: %d\n", dataset.variables() );
  assert( dataset.variables() );
  printf( "dataVariable: %d\n", dataset.dataVariable() );
  assert( IN_RANGE( dataset.dataVariable(), 0, dataset.variables() - 1 ) );
  printf( "cellType: %d\n", dataset.cellType() );
  printf( "hasElevation: %d\n", dataset.hasElevation() );
  printf( "hasArea: %d\n", dataset.hasArea() );
  printf( "hasVolume: %d\n", dataset.hasVolume() );
  printf( "isVector: %d\n", dataset.isVector() );
  printf( "timestepSize: %d\n", dataset.timestepSize() );
  printf( "isStationary: %d\n", dataset.isStationary() );
  printf( "isGroup: %d\n", dataset.isGroup() );
  printf( "isStructuredGrid: %d\n", dataset.isStructuredGrid() );
  printf( "isSurfaceData: %d\n", dataset.isSurfaceData() );

  if ( dataset.isStructuredGrid() ) {
    printf( "layers: %" LU "\n", dataset.layers() );
    printf( "rows: %" LU "\n", dataset.rows() );
    printf( "columns: %" LU "\n", dataset.columns() );
  }

  printf( "variable names, units, range:\n" );

  for ( int variable = 0; variable < dataset.variables(); ++variable ) {
    assert( dataset.variableName( variable ).length() );
    assert( dataset.variableUnits( variable ).length() );
    const double minimum = dataset.minimum( variable );
    const double maximum = dataset.maximum( variable );
    printf( "%s (%s) [%lg %lg]\n",
            dataset.variableName( variable ).c_str(),
            dataset.variableUnits( variable ).c_str(),
            minimum, maximum );
    assert( emvl::isFinite( minimum ) );
    assert( emvl::isFinite( maximum ) );
    assert( minimum <= maximum );
  }

  emvl::Point vertices[ 8 ];
  dataset.cellVertices( 0, vertices );
  const size_t cellVertexCount = dataset.cellVertexCount();
  assert( IN4( cellVertexCount, 1, 4, 8 ) );
  printf( "cell 0 vertices = \n" );

  for ( size_t vertex = 0; vertex < cellVertexCount; ++vertex ) {
    const emvl::Point& point = vertices[ vertex ];
    printf( "  (%lg %lg %lg)\n",
            point.longitude, point.latitude, point.elevation );
  }

  double longitude0 = vertices[ 0 ].longitude;
  double latitude0  = vertices[ 0 ].latitude;

  if ( AND2( dataset.cellType() != emvl::Dataset::POINT, cellVertexCount >= 4 ) ) {
    longitude0 = 0.25 *
      ( vertices[ 0 ].longitude +
        vertices[ 1 ].longitude +
        vertices[ 2 ].longitude +
        vertices[ 3 ].longitude );
    latitude0 = 0.25 *
      ( vertices[ 0 ].latitude +
        vertices[ 1 ].latitude +
        vertices[ 2 ].latitude +
        vertices[ 3 ].latitude );
  }

  double longitude = longitude0;
  double latitude = latitude0;

  // probe:

  double value =
    dataset.probe( dataset.startingTimestamp(), longitude, latitude );
  emvl::Timestamp::UTCString startTime = "";
  dataset.startingTimestamp().toUTCString( startTime );
  const std::string& probedNote = dataset.probedNote();
  printf( "probe @ %s (%lg %lg): %s %lf\n",
         startTime, longitude, latitude,
         probedNote.length() ? probedNote.c_str() : "", value );

  value = dataset.probe( dataset.startingTimestamp(),
                         longitude, latitude, emvl::MISSING, emvl::IMISSING, 0 );
  printf( "probe (v0) @ %s (%lg %lg): %lf\n",
          startTime, longitude, latitude, value );

  double elevation = 10.0;
  value = dataset.probe( dataset.startingTimestamp(),
                         longitude, latitude, elevation, emvl::IMISSING, 0 );
  printf( "probe (v0) @ %s (%lg %lg, %lg): %lf\n",
          startTime, longitude, latitude, elevation, value );

  elevation = 1000.0;
  value = dataset.probe( dataset.startingTimestamp(),
                         longitude, latitude, elevation, emvl::IMISSING, 0 );
  printf( "probe (v0) @ %s (%lg %lg, %lg): %lf\n",
          startTime, longitude, latitude, elevation, value );

  elevation = 40000.0;
  value = dataset.probe( dataset.startingTimestamp(),
                         longitude, latitude, elevation, emvl::IMISSING, 0 );
  printf( "probe (v0) @ %s (%lg %lg, %lg): %lf\n",
          startTime, longitude, latitude, elevation, value );

  const int probeLayer =
    dataset.layers() ? dataset.layers() - 1 : emvl::IMISSING;
  value = dataset.probe( dataset.startingTimestamp(),
                         longitude, latitude, elevation, probeLayer, 0 );
  printf( "probe (v0) @ %s (%lg %lg, %lg, probeLayer = %d): %lf\n",
          startTime, longitude, latitude, elevation, probeLayer, value );

  emvl::Timestamp::UTCString endTime = "";
  dataset.endingTimestamp().toUTCString( endTime );
  value = dataset.probe( dataset.endingTimestamp(), longitude, latitude );
  printf( "probe @ %s (%lg %lg): %lf\n",
          endTime, longitude, latitude, value );
  value = dataset.probe( dataset.endingTimestamp(),
                        longitude, latitude, emvl::MISSING, emvl::IMISSING, 0 );
  printf( "probe (v0) @ %s (%lg %lg): %lf\n",
          endTime, longitude, latitude, value );

  if ( emvl::isLongitude( longitude + 1e-5 ) ) {
    longitude += 1e-5;      
  }

  value = dataset.probe( dataset.endingTimestamp(), longitude, latitude );
  printf("probe @ %s (%lg %lg): %lf\n", endTime, longitude, latitude, value);
  value = dataset.probe( dataset.endingTimestamp(), longitude, latitude,
                         emvl::MISSING, emvl::IMISSING, 0 );
  printf( "probe (v0) @ %s (%lg %lg): %lf\n",
          endTime, longitude, latitude, value );

  if ( emvl::isLatitude( latitude - 0.1 ) ) {
    latitude -= 0.1;      
  }

  value = dataset.probe( dataset.endingTimestamp(), longitude, latitude );
  printf( "probe @ %s (%lg %lg): %lf\n",
          endTime, longitude, latitude, value );
  value = dataset.probe( dataset.endingTimestamp(), longitude, latitude,
                         emvl::MISSING, emvl::IMISSING, 0 );
  printf( "probe (v0) @ %s (%lg %lg): %lf\n",
          endTime, longitude, latitude, value );

  emvl::Timestamp start = dataset.startingTimestamp();
  emvl::Timestamp end = start;
  end.offsetHours( 48 );
  end.toUTCString( endTime );

  value = dataset.probe( end, longitude, latitude );
  printf( "probe @ %s (%lg %lg): %lf\n",
          endTime, longitude, latitude, value );
  assert( value == emvl::MISSING );

  // timeseries:

  for ( size_t trials = 0; trials < 10; ++trials ) {
    emvl::Timestamp beginTimestamp( dataset.startingTimestamp() );
    emvl::Timestamp endTimestamp( dataset.endingTimestamp() );
    std::vector<double> timeseries;
    longitude = longitude0;
    latitude = latitude0;
    elevation = emvl::MISSING;

    if ( trials == 1 ) {
      elevation = 1000.0;
    } else if ( trials == 2 ) {
      longitude += 0.1;
    } else if ( trials == 3 ) {
      latitude -= 0.1;
    } else if ( trials == 4 ) {
      beginTimestamp.offsetHours( -24 );
    } else if ( trials == 5 ) {
      endTimestamp.offsetHours( 24 );
    } else if ( trials == 6 ) {
      beginTimestamp.offsetHours( 24 );
      endTimestamp.offsetHours( 24 );
    } else if ( trials == 7 ) {
      beginTimestamp.offsetHours( -24 );
      endTimestamp.offsetHours( -24 );
    } else if ( trials == 8 ) {
      beginTimestamp.offsetHours( -24 );
      endTimestamp.offsetHours( 24 );
    } else if ( trials == 9 ) {
      beginTimestamp.offsetHours( 12 );
      endTimestamp = beginTimestamp.endOfHour();
    }

    printf( "\ntimeseries @ [%d %d ... %d %d ] (%lg %lg %le):\n",
            beginTimestamp.yyyymmdd(), beginTimestamp.hhmmss(),
            endTimestamp.yyyymmdd(), endTimestamp.hhmmss(),
            longitude, latitude, elevation );

    dataset.timeseries( beginTimestamp, endTimestamp, longitude, latitude,
                        elevation, probeLayer, timeseries );
           
    for ( size_t hour = 0; hour < timeseries.size(); ++hour ) {
      printf( " %lg", timeseries[ hour ] );
    }

    printf( "\n\n" );
  }

  // subset:

  start = dataset.startingTimestamp();
  end = dataset.endingTimestamp();
  start.offsetHours( -24 );
  end.offsetHours( 24 );

  for ( ; start < end; start.offsetHours( 6 ), end.offsetHours( -12 ) ) {
    start.toUTCString( startTime );
    end.toUTCString( endTime );
    std::vector<emvl::Dataset::SubsetPointers> subset =
      dataset.subset( start, end );

    for ( size_t group = 0; group < subset.size(); ++group ) {
      const emvl::Dataset::SubsetPointers& pointers = subset[ group ];
      const size_t coordinatesCount = pointers.coordinatesCount;
      const emvl::Point* const coordinates = pointers.coordinates;
      const size_t dataCount = pointers.dataCount;
      const double* const data = pointers.data;
      const size_t notesCount = pointers.notesCount;
      const std::string* const notes = pointers.notes;
      assert( coordinatesCount );
      assert( dataCount );
      assert( data );

      printf( "%s subset( %s %s ) group %" LU " = count = %" LU ", "
              "data: [%lg ... %lg] range [%lg %lg]\n",
              dataset.variableName( dataset.dataVariable() ).c_str(),
              startTime, endTime, group,
              dataCount, data[ 0 ], data[ dataCount - 1 ],
              emvl::minimum( &data[ 0 ], dataCount ),
              emvl::maximum( &data[ 0 ], dataCount ) );

      if ( coordinates ) { // Use non-0 pointers.coordinates:
        printf( "  stored vertices:\n" );

        for ( size_t vertex = 0; vertex < cellVertexCount; ++vertex ) {
          printf( "  cell 0: (%lg %lg %lg)\n",
                  coordinates[ vertex ].longitude,
                  coordinates[ vertex ].latitude,
                  coordinates[ vertex ].elevation );
          assert( emvl::isPoint( coordinates[ vertex ] ) );
        }

        printf( "  ...\n" );

        for ( size_t vertex = 0, offset = coordinatesCount - cellVertexCount;
              vertex < cellVertexCount; ++vertex ) {
          const size_t index = offset + vertex;
          printf( "  cell %" LU ": (%lg %lg %lg)\n",
                  offset,
                  coordinates[ index ].longitude,
                  coordinates[ index ].latitude,
                  coordinates[ index ].elevation );
          assert( emvl::isPoint( coordinates[ index ] ) );
        }

      } else { // Must use cellVertices():
        printf( "  computed vertices:\n" );
        dataset.cellVertices( 0, vertices );

        for ( size_t vertex = 0; vertex < cellVertexCount; ++vertex ) {
          printf( "  cell 0: (%lg %lg %lg)\n",
                  vertices[ vertex ].longitude,
                  vertices[ vertex ].latitude,
                  vertices[ vertex ].elevation );
          assert( emvl::isPoint( vertices[ vertex ] ) );
        }

        printf( "  ...\n" );
        const size_t lastCellIndex = coordinatesCount / cellVertexCount - 1;
        dataset.cellVertices( lastCellIndex, vertices );

        for ( size_t vertex = 0; vertex < cellVertexCount; ++vertex ) {
          printf( "  cell %" LU ": (%lg %lg %lg)\n",
                  lastCellIndex,
                  vertices[ vertex ].longitude,
                  vertices[ vertex ].latitude,
                  vertices[ vertex ].elevation );
          assert( emvl::isPoint( vertices[ vertex ] ) );
        }
      }

      if ( notesCount == 1 ) {
        printf( "  notes: '%s'", notes[ 0 ].c_str() );
      } else if ( notesCount > 1 ) {
        printf( "  notes: '%s' ... '%s'",
                notes[ 0 ].c_str(), notes[ notesCount - 1 ].c_str() );
      }

      printf( "\n" );
    }
  }

  const std::vector<std::string>& distinctNotes =
    dataset.allDistinctNotes();

  printf( "distinctNotes.size() = %" LU "\n", distinctNotes.size() );

  if ( distinctNotes.size() ) {
    printf( "distinctNotes = [%s ... %s]\n",
            distinctNotes[ 0 ].c_str(),
            distinctNotes[ distinctNotes.size() - 1 ].c_str() );
    const emvl::Point* const point =
      dataset.distinctNotePoint( distinctNotes[ distinctNotes.size() - 1 ] );

    if ( point ) {
      printf( "First point with distinctNotes %s = <%e, %e, %e>\n",
              distinctNotes[ distinctNotes.size() - 1 ].c_str(),
              point->longitude, point->latitude, point->elevation );
    }
  }

  if ( dataset.cellType() != emvl::Dataset::POLYGONS ) { // HACK:
    dataset.write( testDirectoryName, emvl::QueryParameters::XDR,
                   emvl::QueryParameters::REGRID_NONE,
                   emvl::CMAQGridParameters() );
    dataset.write( testDirectoryName, emvl::QueryParameters::XDR );
  }

  dataset.write( testDirectoryName, emvl::QueryParameters::ASCII );
  dataset.write( testDirectoryName, emvl::QueryParameters::NETCDF_COARDS );
  dataset.write( testDirectoryName, emvl::QueryParameters::NETCDF_IOAPI );

  if ( writeKML ) {
    dataset.outputKMLFile( outputKMLFile ); // Must set this before writing KML.
    dataset.write( testDirectoryName, emvl::QueryParameters::KML );
  }

#ifndef _WIN32

// No portable, reliable way to test for unwritable directory in Windows.

  printf( "\n\ntest failure cases:\n\n" );

  try {
    dataset.write( unwritableDirectoryName, emvl::QueryParameters::XDR );
  } catch ( std::exception& e ) {
    printf( "%s\n", e.what() );
  }

  try {
    dataset.write( unwritableDirectoryName, emvl::QueryParameters::ASCII);
  } catch ( std::exception& e ) {
    printf( "%s\n", e.what() );
  }

  try {
    dataset.write( unwritableDirectoryName,
                   emvl::QueryParameters::NETCDF_COARDS );
  } catch ( std::exception& e ) {
    printf( "%s\n", e.what() );
  }

  try {
    dataset.write( unwritableDirectoryName,
                   emvl::QueryParameters::NETCDF_IOAPI );
  } catch ( std::exception& e ) {
    printf( "%s\n", e.what() );
  }

#endif

}



/******************************************************************************
PURPOSE: testPointDataset - Test emvl::PointDataset class.
******************************************************************************/

static void testPointDataset() {

  printf( "testPointDataset()\n" );

  printf( "Testing failure to construct...\n" );

  try {
    const emvl::PointDataset dataset( emptyFileName ); //x
  } catch ( std::exception& e ) {
    fprintf( stderr, "%s\n", e.what() );
  }

  try {
    const emvl::ResourceHandler<FILE*>
    fileCloser( fopen( emptyFileName, "rb" ), fclose );
    FILE* const file = fileCloser.get();

    if ( file ) {
      const emvl::PointDataset dataset( file ); //x
    }
  } catch ( std::exception& e ) {
    fprintf( stderr, "%s\n", e.what() );
  }

  try {
    const emvl::PointDataset dataset( invalidPointTestFileName ); //x
  } catch ( std::exception& e ) {
    fprintf( stderr, "%s\n", e.what() );
  }

  try {
    const emvl::ResourceHandler<FILE*>
    fileCloser( fopen( invalidPointTestFileName, "rb" ), fclose );
    FILE* const file = fileCloser.get();

    if ( file ) {
      const emvl::PointDataset dataset( file ); //x
    }
  } catch ( std::exception& e ) {
    fprintf( stderr, "%s\n", e.what() );
  }

  printf( "\n\nTesting successful cases...\n\n" );

  for ( size_t index = 0; index < pointFileCount; ++index ) {
    const char* const fileName = pointFiles[ index ];
    printf( "%s\n", fileName );
    {
      emvl::PointDataset dataset( fileName ); //x

      assert( dataset.invariant() );
      assert( dataset == dataset );
      assert( ! ( dataset != dataset ) );
      const int variable = dataset.dataVariable();
      dataset.dataVariable( 0 );
      dataset.dataVariable( variable );
      assert( dataset.layers() == 0 );
      assert( dataset.rows() == 0 );
      assert( dataset.columns() == 0 );

      const emvl::PointDataset dataset2( dataset ); //x

      assert( dataset2.invariant() );
      assert( dataset2 == dataset );
      assert( ! ( dataset2 != dataset ) );

      dataset = dataset2;

      assert( dataset.invariant() );
      assert( dataset == dataset2 );
      assert( ! ( dataset != dataset2 ) );

      testDataset( dataset, true );
    }

    {
      const emvl::ResourceHandler<FILE*>
      fileCloser( fopen( fileName, "rb" ), fclose );
      FILE* const file = fileCloser.get();

      if ( file ) {
        emvl::PointDataset dataset( file ); //x

        assert( dataset.invariant() );
        assert( dataset == dataset );
        assert( ! ( dataset != dataset ) );
        const int variable = dataset.dataVariable();
        dataset.dataVariable( 0 );
        dataset.dataVariable( variable );
        assert( dataset.layers() == 0 );
        assert( dataset.rows() == 0 );
        assert( dataset.columns() == 0 );

        const emvl::PointDataset dataset2( dataset ); //x

        assert( dataset2.invariant() );
        assert( dataset2 == dataset );
        assert( ! ( dataset2 != dataset ) );

        dataset = dataset2;

        assert( dataset.invariant() );
        assert( dataset == dataset2 );
        assert( ! ( dataset != dataset2 ) );

        testDataset( dataset, false );
      }
    }
  }
}



/******************************************************************************
PURPOSE: testRegriddedPointDataset - Test emvl::RegriddedPointDataset class.
******************************************************************************/

static void testRegriddedPointDataset() {

  printf( "testRegriddedPointDataset()\n" );

  printf( "\nTesting failure to construct...\n" );

  try {
    const emvl::RegriddedPointDataset dataset( emptyFileName ); //x
  } catch ( std::exception& e ) {
    fprintf( stderr, "%s\n", e.what() );
  }

  try {
    const emvl::RegriddedPointDataset
      RegriddedPointDataset( invalidRegriddedPointTestFileName ); //x
  } catch ( std::exception& e ) {
    fprintf( stderr, "%s\n", e.what() );
  }

  try {
    const emvl::ResourceHandler<FILE*>
    fileCloser( fopen( emptyFileName, "rb" ), fclose );
    FILE* const file = fileCloser.get();

    if ( file ) {
      const emvl::RegriddedPointDataset dataset( file ); //x
    }
  } catch ( std::exception& e ) {
    fprintf( stderr, "%s\n", e.what() );
  }

  try {
    const emvl::ResourceHandler<FILE*>
    fileCloser( fopen( invalidRegriddedPointTestFileName, "rb" ), fclose );
    FILE* const file = fileCloser.get();

    if ( file ) {
      const emvl::RegriddedPointDataset RegriddedPointDataset( file ); //x
    }
  } catch ( std::exception& e ) {
    fprintf( stderr, "%s\n", e.what() );
  }

  printf( "\n\nTesting successful cases...\n\n" );

  for ( size_t index = 0; index < regriddedPointFileCount; ++index ) {
    const char* const fileName = regriddedPointFiles[ index ];
    printf( "%s\n", fileName );
    {
      emvl::RegriddedPointDataset dataset( fileName ); //x

      assert( dataset.invariant() );
      assert( dataset == dataset );
      assert( ! ( dataset != dataset ) );

      const emvl::RegriddedPointDataset dataset2( dataset ); //x

      assert( dataset2.invariant() );
      assert( dataset2 == dataset );
      assert( ! ( dataset2 != dataset ) );

      dataset = dataset2;

      assert( dataset.invariant() );
      assert( dataset == dataset2 );
      assert( ! ( dataset != dataset2 ) );

      testDataset( dataset, true );
    }
    {
      const emvl::ResourceHandler<FILE*>
        fileCloser( fopen( fileName, "rb" ), fclose );
      FILE* const file = fileCloser.get();

      if ( file ) {
        emvl::RegriddedPointDataset dataset( file ); //x

        assert( dataset.invariant() );
        assert( dataset == dataset );
        assert( ! ( dataset != dataset ) );

        const emvl::RegriddedPointDataset dataset2( dataset ); //x

        assert( dataset2.invariant() );
        assert( dataset2 == dataset );
        assert( ! ( dataset2 != dataset ) );

        dataset = dataset2;

        assert( dataset.invariant() );
        assert( dataset == dataset2 );
        assert( ! ( dataset != dataset2 ) );

        testDataset( dataset, false );
      }
    }
  }
}



/******************************************************************************
PURPOSE: testSiteDataset - Test emvl::SiteDataset class.
******************************************************************************/

static void testSiteDataset() {

  printf( "testSiteDataset()\n" );

  printf( "Testing failure to construct...\n" );

  try {
    const emvl::SiteDataset dataset( emptyFileName ); //x
  } catch ( std::exception& e ) {
    fprintf( stderr, "%s\n", e.what() );
  }

  try {
    const emvl::ResourceHandler<FILE*>
    fileCloser( fopen( emptyFileName, "rb" ), fclose );
    FILE* const file = fileCloser.get();

    if ( file ) {
      const emvl::SiteDataset dataset( file ); //x
    }
  } catch ( std::exception& e ) {
    fprintf( stderr, "%s\n", e.what() );
  }

  try {
    const emvl::SiteDataset dataset( invalidSiteTestFileName ); //x
  } catch ( std::exception& e ) {
    fprintf( stderr, "%s\n", e.what() );
  }

  try {
    const emvl::ResourceHandler<FILE*>
    fileCloser( fopen( invalidSiteTestFileName, "rb" ), fclose );
    FILE* const file = fileCloser.get();

    if ( file ) {
      const emvl::SiteDataset dataset( file ); //x
    }
  } catch ( std::exception& e ) {
    fprintf( stderr, "%s\n", e.what() );
  }

  printf( "\n\nTesting successful cases...\n\n" );

  for ( size_t index = 0; index < siteFileCount; ++index ) {
    const char* const fileName = siteFiles[ index ];
    printf( "%s\n", fileName );
    {
      emvl::SiteDataset dataset( fileName ); //x

      assert( dataset.invariant() );
      assert( dataset == dataset );
      assert( ! ( dataset != dataset ) );
      const int variable = dataset.dataVariable();
      dataset.dataVariable( 0 );
      dataset.dataVariable( variable );
      assert( dataset.layers() == 0 );
      assert( dataset.rows() == 0 );
      assert( dataset.columns() == 0 );

      const emvl::SiteDataset dataset2( dataset ); //x

      assert( dataset2.invariant() );
      assert( dataset2 == dataset );
      assert( ! ( dataset2 != dataset ) );

      dataset = dataset2;

      assert( dataset.invariant() );
      assert( dataset == dataset2 );
      assert( ! ( dataset != dataset2 ) );

      const size_t stations = dataset.stations();

      for ( size_t station = 0; station < stations; ++station ) {
        assert( dataset.note( station ).size() );
        printf( "%s\n", dataset.note( station ).c_str() );
      }

      testDataset( dataset, true );
    }

    {
      const emvl::ResourceHandler<FILE*>
      fileCloser( fopen( fileName, "rb" ), fclose );
      FILE* const file = fileCloser.get();

      if ( file ) {
        emvl::SiteDataset dataset( file ); //x

        assert( dataset.invariant() );
        assert( dataset == dataset );
        assert( ! ( dataset != dataset ) );
        const int variable = dataset.dataVariable();
        dataset.dataVariable( 0 );
        dataset.dataVariable( variable );
        assert( dataset.layers() == 0 );
        assert( dataset.rows() == 0 );
        assert( dataset.columns() == 0 );

        const emvl::SiteDataset dataset2( dataset ); //x

        assert( dataset2.invariant() );
        assert( dataset2 == dataset );
        assert( ! ( dataset2 != dataset ) );

        dataset = dataset2;

        assert( dataset.invariant() );
        assert( dataset == dataset2 );
        assert( ! ( dataset != dataset2 ) );

        const size_t stations = dataset.stations();

        for ( size_t station = 0; station < stations; ++station ) {
          assert( dataset.note( station ).size() );
          printf( "%s\n", dataset.note( station ).c_str() );
        }

        testDataset( dataset, false );
      }
    }
  }
}



/******************************************************************************
PURPOSE: testRegriddedSiteDataset - Test emvl::RegriddedSiteDataset class.
******************************************************************************/

static void testRegriddedSiteDataset() {

  printf( "testRegriddedSiteDataset()\n" );

  printf( "\nTesting failure to construct...\n" );

  try {
    const emvl::RegriddedSiteDataset dataset( emptyFileName ); //x
  } catch ( std::exception& e ) {
    fprintf( stderr, "%s\n", e.what() );
  }

  try {
    const emvl::RegriddedSiteDataset
      RegriddedSiteDataset( invalidRegriddedSiteTestFileName ); //x
  } catch ( std::exception& e ) {
    fprintf( stderr, "%s\n", e.what() );
  }

  try {
    const emvl::ResourceHandler<FILE*>
    fileCloser( fopen( emptyFileName, "rb" ), fclose );
    FILE* const file = fileCloser.get();

    if ( file ) {
      const emvl::RegriddedSiteDataset dataset( file ); //x
    }
  } catch ( std::exception& e ) {
    fprintf( stderr, "%s\n", e.what() );
  }

  try {
    const emvl::ResourceHandler<FILE*>
    fileCloser( fopen( invalidRegriddedSiteTestFileName, "rb" ), fclose );
    FILE* const file = fileCloser.get();

    if ( file ) {
      const emvl::RegriddedSiteDataset RegriddedSiteDataset( file ); //x
    }
  } catch ( std::exception& e ) {
    fprintf( stderr, "%s\n", e.what() );
  }

  printf( "\n\nTesting successful cases...\n\n" );

  for ( size_t index = 0; index < regriddedSiteFileCount; ++index ) {
    const char* const fileName = regriddedSiteFiles[ index ];
    printf( "%s\n", fileName );
    {
      emvl::RegriddedSiteDataset dataset( fileName ); //x

      assert( dataset.invariant() );
      assert( dataset == dataset );
      assert( ! ( dataset != dataset ) );

      const emvl::RegriddedSiteDataset dataset2( dataset ); //x

      assert( dataset2.invariant() );
      assert( dataset2 == dataset );
      assert( ! ( dataset2 != dataset ) );

      dataset = dataset2;

      assert( dataset.invariant() );
      assert( dataset == dataset2 );
      assert( ! ( dataset != dataset2 ) );

      testDataset( dataset, true );
    }
    {
      const emvl::ResourceHandler<FILE*>
        fileCloser( fopen( fileName, "rb" ), fclose );
      FILE* const file = fileCloser.get();

      if ( file ) {
        emvl::RegriddedSiteDataset dataset( file ); //x

        assert( dataset.invariant() );
        assert( dataset == dataset );
        assert( ! ( dataset != dataset ) );

        const emvl::RegriddedSiteDataset dataset2( dataset ); //x

        assert( dataset2.invariant() );
        assert( dataset2 == dataset );
        assert( ! ( dataset2 != dataset ) );

        dataset = dataset2;

        assert( dataset.invariant() );
        assert( dataset == dataset2 );
        assert( ! ( dataset != dataset2 ) );

        testDataset( dataset, false );
      }
    }
  }
}



/******************************************************************************
PURPOSE: testCMAQDataset - Test emvl::CMAQDataset class.
******************************************************************************/

static void testCMAQDataset() {

  printf( "testCMAQDataset()\n" );

  printf( "\nTesting failure to construct...\n" );

  try {
    const emvl::CMAQDataset dataset( emptyFileName ); //x
  } catch ( std::exception& e ) {
    fprintf( stderr, "%s\n", e.what() );
  }

  try {
    const emvl::CMAQDataset CMAQDataset( invalidCMAQTestFileName ); //x
  } catch ( std::exception& e ) {
    fprintf( stderr, "%s\n", e.what() );
  }

  try {
    const emvl::ResourceHandler<FILE*>
    fileCloser( fopen( emptyFileName, "rb" ), fclose );
    FILE* const file = fileCloser.get();

    if ( file ) {
      const emvl::CMAQDataset dataset( file ); //x
    }
  } catch ( std::exception& e ) {
    fprintf( stderr, "%s\n", e.what() );
  }

  try {
    const emvl::ResourceHandler<FILE*>
    fileCloser( fopen( invalidCMAQTestFileName, "rb" ), fclose );
    FILE* const file = fileCloser.get();

    if ( file ) {
      const emvl::CMAQDataset CMAQDataset( file ); //x
    }
  } catch ( std::exception& e ) {
    fprintf( stderr, "%s\n", e.what() );
  }

  printf( "\n\nTesting successful cases...\n\n" );

  for ( size_t index = 0; index < cmaqFileCount; ++index ) {
    const char* const fileName = cmaqFiles[ index ];
    printf( "%s\n", fileName );
    {
      emvl::CMAQDataset dataset( fileName ); //x

      assert( dataset.invariant() );
      assert( dataset == dataset );
      assert( ! ( dataset != dataset ) );

      const emvl::CMAQDataset dataset2( dataset ); //x

      assert( dataset2.invariant() );
      assert( dataset2 == dataset );
      assert( ! ( dataset2 != dataset ) );

      dataset = dataset2;

      assert( dataset.invariant() );
      assert( dataset == dataset2 );
      assert( ! ( dataset != dataset2 ) );

      assert( dataset.gridName().length() );
      printf( "%s\n", dataset.gridName().c_str() );
      printf( "dimensions: %" LU " %" LU " %" LU " %" LU "\n",
              dataset.timesteps(),
              dataset.layers(),
              dataset.rows(),
              dataset.columns() );

      const emvl::CMAQDataset::SubsetIndices& subsetIndices =
        dataset.subsetIndices();
      printf( "subsetIndices[] = [ %" LU " %" LU "  %" LU " %" LU "  %" LU " %" LU "  %" LU " %" LU " ]\n",
             subsetIndices[ emvl::TIME ][ emvl::FIRST ],
             subsetIndices[ emvl::TIME ][ emvl::LAST ],
             subsetIndices[ emvl::LAYER ][ emvl::FIRST ],
             subsetIndices[ emvl::LAYER ][ emvl::LAST ],
             subsetIndices[ emvl::ROW ][ emvl::FIRST ],
             subsetIndices[ emvl::ROW ][ emvl::LAST ],
             subsetIndices[ emvl::COLUMN ][ emvl::FIRST ],
             subsetIndices[ emvl::COLUMN ][ emvl::LAST ] );
      const emvl::CMAQGridParameters& gridParameters =
        dataset.gridParameters();
      assert( gridParameters.invariant() );
      const emvl::MapProjection* mapProjection = dataset.mapProjection();
      assert( IMPLIES_ELSE( gridParameters.gdtyp() ==
                              emvl::CMAQGridParameters::LATGRD3,
                            mapProjection == 0,
                            mapProjection->invariant() ) );

      if ( mapProjection ) {
        printf( "%s\n", mapProjection->name() );
      }

      const size_t levels = gridParameters.nlays() + 1;
      printf( "%" LU " elevation levels\n", levels );

      testDataset( dataset, true );
    }
    {
      const emvl::ResourceHandler<FILE*> fileCloser(fopen(fileName,"rb"),fclose);
      FILE* const file = fileCloser.get();

      if ( file ) {
        emvl::CMAQDataset dataset( file ); //x

        assert( dataset.invariant() );
        assert( dataset == dataset );
        assert( ! ( dataset != dataset ) );

        const emvl::CMAQDataset dataset2( dataset ); //x

        assert( dataset2.invariant() );
        assert( dataset2 == dataset );
        assert( ! ( dataset2 != dataset ) );

        dataset = dataset2;

        assert( dataset.invariant() );
        assert( dataset == dataset2 );
        assert( ! ( dataset != dataset2 ) );

        assert( dataset.gridName().length() );
        printf( "%s\n", dataset.gridName().c_str() );
        printf( "dimensions: %" LU " %" LU " %" LU " %" LU "\n",
                dataset.timesteps(),
                dataset.layers(),
                dataset.rows(),
                dataset.columns() );

        const emvl::CMAQDataset::SubsetIndices& subsetIndices =
          dataset.subsetIndices();
        printf( "subsetIndices[] = [ %" LU " %" LU "  %" LU " %" LU "  %" LU " %" LU "  %" LU " %" LU " ]\n",
               subsetIndices[ emvl::TIME ][ emvl::FIRST ],
               subsetIndices[ emvl::TIME ][ emvl::LAST ],
               subsetIndices[ emvl::LAYER ][ emvl::FIRST ],
               subsetIndices[ emvl::LAYER ][ emvl::LAST ],
               subsetIndices[ emvl::ROW ][ emvl::FIRST ],
               subsetIndices[ emvl::ROW ][ emvl::LAST ],
               subsetIndices[ emvl::COLUMN ][ emvl::FIRST ],
               subsetIndices[ emvl::COLUMN ][ emvl::LAST ] );
        const emvl::CMAQGridParameters& gridParameters =
          dataset.gridParameters();
        assert( gridParameters.invariant() );
        const emvl::MapProjection* mapProjection = dataset.mapProjection();
        assert( IMPLIES_ELSE( gridParameters.gdtyp() ==
                                emvl::CMAQGridParameters::LATGRD3,
                              mapProjection == 0,
                              mapProjection->invariant() ) );

        if ( mapProjection ) {
          printf( "%s\n", mapProjection->name() );
        }

        const size_t levels = gridParameters.nlays() + 1;
        printf( "%" LU " elevation levels\n", levels );

        testDataset( dataset, false );
      }
    }
  }
}



/******************************************************************************
PURPOSE: testSwathDataset - Test emvl::SwathDataset class.
******************************************************************************/

static void testSwathDataset() {

  printf( "testSwathDataset()\n" );

  printf( "Testing failure to construct...\n" );

  try {
    const emvl::SwathDataset dataset( emptyFileName ); //x
  } catch ( std::exception& e ) {
    fprintf( stderr, "%s\n", e.what() );
  }

  try {
    const emvl::ResourceHandler<FILE*>
      fileCloser( fopen( emptyFileName, "rb" ), fclose );
    FILE* const file = fileCloser.get();

    if ( file ) {
      const emvl::SwathDataset dataset( file ); //x
    }
  } catch ( std::exception& e ) {
    fprintf( stderr, "%s\n", e.what() );
  }

  // GASP:

  try {
    const emvl::SwathDataset dataset( invalidGASPTestFileName ); //x
  } catch ( std::exception& e ) {
    fprintf( stderr, "%s\n", e.what() );
  }

  try {
    const emvl::ResourceHandler<FILE*>
      fileCloser( fopen( invalidGASPTestFileName, "rb"), fclose );
    FILE* const file = fileCloser.get();

    if ( file ) {
      const emvl::SwathDataset dataset( file ); //x
    }
  } catch ( std::exception& e ) {
    fprintf( stderr, "%s\n", e.what() );
  }

  // MODIS:

  try {
    const emvl::SwathDataset dataset( invalidMODISTestFileName ); //x
  } catch ( std::exception& e ) {
    fprintf( stderr, "%s\n", e.what() );
  }

  try {
    const emvl::ResourceHandler<FILE*>
      fileCloser( fopen( invalidMODISTestFileName, "rb"), fclose );
    FILE* const file = fileCloser.get();

    if ( file ) {
      const emvl::SwathDataset dataset( file ); //x
    }
  } catch ( std::exception& e ) {
    fprintf( stderr, "%s\n", e.what() );
  }

  // VIIRS:

  try {
    const emvl::SwathDataset dataset( invalidVIIRSTestFileName ); //x
  } catch ( std::exception& e ) {
    fprintf( stderr, "%s\n", e.what() );
  }

  try {
    const emvl::ResourceHandler<FILE*>
      fileCloser( fopen( invalidVIIRSTestFileName, "rb"), fclose );
    FILE* const file = fileCloser.get();

    if ( file ) {
      const emvl::SwathDataset dataset( file ); //x
    }
  } catch ( std::exception& e ) {
    fprintf( stderr, "%s\n", e.what() );
  }

  printf( "\n\nTesting successful cases...\n\n" );

  // GASP:

  for ( size_t index = 0; index < gaspFileCount; ++index ) {
    const char* const fileName = gaspFiles[ index ];
    printf( "%s\n", fileName );
    {
      emvl::SwathDataset dataset( fileName ); //x

      assert( dataset.invariant() );
      assert( dataset == dataset );
      assert( ! ( dataset != dataset ) );
      const int variable = dataset.dataVariable();
      dataset.dataVariable( 0 );
      dataset.dataVariable( variable );
      assert( dataset.layers() == 0 );
      assert( dataset.rows() == 0 );
      assert( dataset.columns() == 0 );

      const emvl::SwathDataset dataset2( dataset ); //x

      assert( dataset2.invariant() );
      assert( dataset2 == dataset );
      assert( ! ( dataset2 != dataset ) );

      dataset = dataset2;

      assert( dataset.invariant() );
      assert( dataset == dataset2 );
      assert( ! ( dataset != dataset2 ) );

      testDataset( dataset, true );
    }
    {
      const emvl::ResourceHandler<FILE*>
        fileCloser( fopen( fileName, "rb" ), fclose );
      FILE* const file = fileCloser.get();

      if ( file ) {
        emvl::SwathDataset dataset( file ); //x

        assert( dataset.invariant() );
        assert( dataset == dataset );
        assert( ! ( dataset != dataset ) );
        const int variable = dataset.dataVariable();
        dataset.dataVariable( 0 );
        dataset.dataVariable( variable );
        assert( dataset.layers() == 0 );
        assert( dataset.rows() == 0 );
        assert( dataset.columns() == 0 );

        const emvl::SwathDataset dataset2( dataset ); //x

        assert( dataset2.invariant() );
        assert( dataset2 == dataset );
        assert( ! ( dataset2 != dataset ) );

        dataset = dataset2;

        assert( dataset.invariant() );
        assert( dataset == dataset2 );
        assert( ! ( dataset != dataset2 ) );

        testDataset( dataset, false );
      }
    }
  }

  // MODIS:

  for ( size_t index = 0; index < modisFileCount; ++index ) {
    const char* const fileName = modisFiles[ index ];
    printf( "%s\n", fileName );
    {
      emvl::SwathDataset dataset( fileName ); //x

      assert( dataset.invariant() );
      assert( dataset == dataset );
      assert( ! ( dataset != dataset ) );
      const int variable = dataset.dataVariable();
      dataset.dataVariable( 0 );
      dataset.dataVariable( variable );
      assert( dataset.layers() == 0 );
      assert( dataset.rows() == 0 );
      assert( dataset.columns() == 0 );

      const emvl::SwathDataset dataset2( dataset ); //x

      assert( dataset2.invariant() );
      assert( dataset2 == dataset );
      assert( ! ( dataset2 != dataset ) );

      dataset = dataset2;

      assert( dataset.invariant() );
      assert( dataset == dataset2 );
      assert( ! ( dataset != dataset2 ) );

      testDataset( dataset, true );
    }
    {
      const emvl::ResourceHandler<FILE*>
        fileCloser( fopen( fileName, "rb" ), fclose );
      FILE* const file = fileCloser.get();

      if ( file ) {
        emvl::SwathDataset dataset( file ); //x

        assert( dataset.invariant() );
        assert( dataset == dataset );
        assert( ! ( dataset != dataset ) );
        const int variable = dataset.dataVariable();
        dataset.dataVariable( 0 );
        dataset.dataVariable( variable );
        assert( dataset.layers() == 0 );
        assert( dataset.rows() == 0 );
        assert( dataset.columns() == 0 );

        const emvl::SwathDataset dataset2( dataset ); //x

        assert( dataset2.invariant() );
        assert( dataset2 == dataset );
        assert( ! ( dataset2 != dataset ) );

        dataset = dataset2;

        assert( dataset.invariant() );
        assert( dataset == dataset2 );
        assert( ! ( dataset != dataset2 ) );

        testDataset( dataset, false );
      }
    }
  }

  // VIIRS:

  for ( size_t index = 0; index < viirsFileCount; ++index ) {
    const char* const fileName = viirsFiles[ index ];
    printf( "%s\n", fileName );
    {
      emvl::SwathDataset dataset( fileName ); //x

      assert( dataset.invariant() );
      assert( dataset == dataset );
      assert( ! ( dataset != dataset ) );
      const int variable = dataset.dataVariable();
      dataset.dataVariable( 0 );
      dataset.dataVariable( variable );
      assert( dataset.layers() == 0 );
      assert( dataset.rows() == 0 );
      assert( dataset.columns() == 0 );

      const emvl::SwathDataset dataset2( dataset ); //x

      assert( dataset2.invariant() );
      assert( dataset2 == dataset );
      assert( ! ( dataset2 != dataset ) );

      dataset = dataset2;

      assert( dataset.invariant() );
      assert( dataset == dataset2 );
      assert( ! ( dataset != dataset2 ) );

      testDataset( dataset, true );
    }
    {
      const emvl::ResourceHandler<FILE*>
        fileCloser( fopen( fileName, "rb" ), fclose );
      FILE* const file = fileCloser.get();

      if ( file ) {
        emvl::SwathDataset dataset( file ); //x

        assert( dataset.invariant() );
        assert( dataset == dataset );
        assert( ! ( dataset != dataset ) );
        const int variable = dataset.dataVariable();
        dataset.dataVariable( 0 );
        dataset.dataVariable( variable );
        assert( dataset.layers() == 0 );
        assert( dataset.rows() == 0 );
        assert( dataset.columns() == 0 );

        const emvl::SwathDataset dataset2( dataset ); //x

        assert( dataset2.invariant() );
        assert( dataset2 == dataset );
        assert( ! ( dataset2 != dataset ) );

        dataset = dataset2;

        assert( dataset.invariant() );
        assert( dataset == dataset2 );
        assert( ! ( dataset != dataset2 ) );

        testDataset( dataset, false );
      }
    }
  }

}



/******************************************************************************
PURPOSE: testRegriddedSwathDataset - Test emvl::RegriddedSwathDataset class.
******************************************************************************/

static void testRegriddedSwathDataset() {

  printf( "testRegriddedSwathDataset()\n" );

  printf( "\nTesting failure to construct...\n" );

  try {
    const emvl::RegriddedSwathDataset dataset( emptyFileName ); //x
  } catch ( std::exception& e ) {
    fprintf( stderr, "%s\n", e.what() );
  }

  try {
    const emvl::ResourceHandler<FILE*>
      fileCloser( fopen( emptyFileName, "rb" ), fclose );
    FILE* const file = fileCloser.get();

    if ( file ) {
      const emvl::RegriddedSwathDataset dataset( file ); //x
    }
  } catch ( std::exception& e ) {
    fprintf( stderr, "%s\n", e.what() );
  }

  // GASP:

  try {
   const emvl::RegriddedSwathDataset
      dataset( invalidRegriddedGASPTestFileName ); //x
  } catch ( std::exception& e ) {
    fprintf( stderr, "%s\n", e.what() );
  }

  try {
    const emvl::ResourceHandler<FILE*>
      fileCloser( fopen( invalidRegriddedGASPTestFileName, "rb" ), fclose );
    FILE* const file = fileCloser.get();

    if ( file ) {
      const emvl::RegriddedSwathDataset dataset( file ); //x
    }
  } catch ( std::exception& e ) {
    fprintf( stderr, "%s\n", e.what() );
  }

  // MODIS:

  try {
    const emvl::RegriddedSwathDataset
      RegriddedSwathDataset( invalidRegriddedMODISTestFileName ); //x
  } catch ( std::exception& e ) {
    fprintf( stderr, "%s\n", e.what() );
  }

  try {
    const emvl::ResourceHandler<FILE*>
      fileCloser( fopen( invalidRegriddedMODISTestFileName, "rb" ), fclose );
    FILE* const file = fileCloser.get();

    if ( file ) {
      const emvl::RegriddedSwathDataset regriddedSwathDataset( file ); //x
    }
  } catch ( std::exception& e ) {
    fprintf( stderr, "%s\n", e.what() );
  }

  // VIIRS:

  try {
    const emvl::RegriddedSwathDataset
      RegriddedSwathDataset( invalidRegriddedVIIRSTestFileName ); //x
  } catch ( std::exception& e ) {
    fprintf( stderr, "%s\n", e.what() );
  }

  try {
    const emvl::ResourceHandler<FILE*>
      fileCloser( fopen( invalidRegriddedVIIRSTestFileName, "rb" ), fclose );
    FILE* const file = fileCloser.get();

    if ( file ) {
      const emvl::RegriddedSwathDataset regriddedSwathDataset( file ); //x
    }
  } catch ( std::exception& e ) {
    fprintf( stderr, "%s\n", e.what() );
  }

  printf( "\n\nTesting successful cases...\n\n" );

  // GASP:

  for ( size_t index = 0; index < regriddedGASPFileCount; ++index ) {
    const char* const fileName = regriddedGASPFiles[ index ];
    printf( "%s\n", fileName );
    {
      emvl::RegriddedSwathDataset dataset( fileName ); //x

      assert( dataset.invariant() );
      assert( dataset == dataset );
      assert( ! ( dataset != dataset ) );

      const emvl::RegriddedSwathDataset dataset2( dataset ); //x

      assert( dataset2.invariant() );
      assert( dataset2 == dataset );
      assert( ! ( dataset2 != dataset ) );

      dataset = dataset2;

      assert( dataset.invariant() );
      assert( dataset == dataset2 );
      assert( ! ( dataset != dataset2 ) );

      testDataset( dataset, true );
    }
    {
      const emvl::ResourceHandler<FILE*>
        fileCloser( fopen( fileName, "rb" ), fclose );
      FILE* const file = fileCloser.get();

      if ( file ) {
        emvl::RegriddedSwathDataset dataset( file ); //x

        assert( dataset.invariant() );
        assert( dataset == dataset );
        assert( ! ( dataset != dataset ) );

        const emvl::RegriddedSwathDataset dataset2( dataset ); //x

        assert( dataset2.invariant() );
        assert( dataset2 == dataset );
        assert( ! ( dataset2 != dataset ) );

        dataset = dataset2;

        assert( dataset.invariant() );
        assert( dataset == dataset2 );
        assert( ! ( dataset != dataset2 ) );

        testDataset( dataset, false );
      }
    }
  }

  // MODIS:

  for ( size_t index = 0; index < regriddedMODISFileCount; ++index ) {
    const char* const fileName = regriddedMODISFiles[ index ];
    printf( "%s\n", fileName );
    {
      emvl::RegriddedSwathDataset dataset( fileName ); //x

      assert( dataset.invariant() );
      assert( dataset == dataset );
      assert( ! ( dataset != dataset ) );

      const emvl::RegriddedSwathDataset dataset2( dataset ); //x

      assert( dataset2.invariant() );
      assert( dataset2 == dataset );
      assert( ! ( dataset2 != dataset ) );

      dataset = dataset2;

      assert( dataset.invariant() );
      assert( dataset == dataset2 );
      assert( ! ( dataset != dataset2 ) );

      testDataset( dataset, true );
    }
    {
      const emvl::ResourceHandler<FILE*>
        fileCloser( fopen( fileName, "rb" ), fclose );
      FILE* const file = fileCloser.get();

      if ( file ) {
        emvl::RegriddedSwathDataset dataset( file ); //x

        assert( dataset.invariant() );
        assert( dataset == dataset );
        assert( ! ( dataset != dataset ) );

        const emvl::RegriddedSwathDataset dataset2( dataset ); //x

        assert( dataset2.invariant() );
        assert( dataset2 == dataset );
        assert( ! ( dataset2 != dataset ) );

        dataset = dataset2;

        assert( dataset.invariant() );
        assert( dataset == dataset2 );
        assert( ! ( dataset != dataset2 ) );

        testDataset( dataset, false );
      }
    }
  }

  // VIIRS:

  for ( size_t index = 0; index < regriddedVIIRSFileCount; ++index ) {
    const char* const fileName = regriddedVIIRSFiles[ index ];
    printf( "%s\n", fileName );
    {
      emvl::RegriddedSwathDataset dataset( fileName ); //x

      assert( dataset.invariant() );
      assert( dataset == dataset );
      assert( ! ( dataset != dataset ) );

      const emvl::RegriddedSwathDataset dataset2( dataset ); //x

      assert( dataset2.invariant() );
      assert( dataset2 == dataset );
      assert( ! ( dataset2 != dataset ) );

      dataset = dataset2;

      assert( dataset.invariant() );
      assert( dataset == dataset2 );
      assert( ! ( dataset != dataset2 ) );

      testDataset( dataset, true );
    }
    {
      const emvl::ResourceHandler<FILE*>
        fileCloser( fopen( fileName, "rb" ), fclose );
      FILE* const file = fileCloser.get();

      if ( file ) {
        emvl::RegriddedSwathDataset dataset( file ); //x

        assert( dataset.invariant() );
        assert( dataset == dataset );
        assert( ! ( dataset != dataset ) );

        const emvl::RegriddedSwathDataset dataset2( dataset ); //x

        assert( dataset2.invariant() );
        assert( dataset2 == dataset );
        assert( ! ( dataset2 != dataset ) );

        dataset = dataset2;

        assert( dataset.invariant() );
        assert( dataset == dataset2 );
        assert( ! ( dataset != dataset2 ) );

        testDataset( dataset, false );
      }
    }
  }
}



/******************************************************************************
PURPOSE: testAircraftDataset - Test emvl::AircraftDataset class.
******************************************************************************/

static void testAircraftDataset() {

  printf( "testAircraftDataset()\n" );

  printf( "Testing failure to construct...\n" );

  try {
    const emvl::AircraftDataset dataset( emptyFileName ); //x
  } catch ( std::exception& e ) {
    fprintf( stderr, "%s\n", e.what() );
  }

  try {
    const emvl::ResourceHandler<FILE*>
      fileCloser( fopen( emptyFileName, "rb" ), fclose );
    FILE* const file = fileCloser.get();

    if ( file ) {
      const emvl::AircraftDataset dataset( file ); //x
    }
  } catch ( std::exception& e ) {
    fprintf( stderr, "%s\n", e.what() );
  }

  try {
    const emvl::AircraftDataset dataset( invalidAircraftTestFileName ); //x
  } catch ( std::exception& e ) {
    fprintf( stderr, "%s\n", e.what() );
  }

  try {
    const emvl::ResourceHandler<FILE*>
      fileCloser( fopen( invalidAircraftTestFileName, "rb"), fclose );
    FILE* const file = fileCloser.get();

    if ( file ) {
      const emvl::AircraftDataset dataset( file ); //x
    }
  } catch ( std::exception& e ) {
    fprintf( stderr, "%s\n", e.what() );
  }

  printf( "\n\nTesting successful cases...\n\n" );

  for ( size_t index = 0; index < aircraftFileCount; ++index ) {
    const char* const fileName = aircraftFiles[ index ];
    printf( "%s\n", fileName );
    {
      emvl::AircraftDataset dataset( fileName ); //x

      assert( dataset.invariant() );
      assert( dataset == dataset );
      assert( ! ( dataset != dataset ) );
      const int variable = dataset.dataVariable();
      dataset.dataVariable( 0 );
      dataset.dataVariable( variable );
      assert( dataset.layers() == 0 );
      assert( dataset.rows() == 0 );
      assert( dataset.columns() == 0 );

      const emvl::AircraftDataset dataset2( dataset ); //x

      assert( dataset2.invariant() );
      assert( dataset2 == dataset );
      assert( ! ( dataset2 != dataset ) );

      dataset = dataset2;

      assert( dataset.invariant() );
      assert( dataset == dataset2 );
      assert( ! ( dataset != dataset2 ) );

      testDataset( dataset, true );
    }
    {
      const emvl::ResourceHandler<FILE*>
        fileCloser( fopen( fileName, "rb" ), fclose );
      FILE* const file = fileCloser.get();

      if ( file ) {
        emvl::AircraftDataset dataset( file ); //x

        assert( dataset.invariant() );
        assert( dataset == dataset );
        assert( ! ( dataset != dataset ) );
        const int variable = dataset.dataVariable();
        dataset.dataVariable( 0 );
        dataset.dataVariable( variable );
        assert( dataset.layers() == 0 );
        assert( dataset.rows() == 0 );
        assert( dataset.columns() == 0 );

        const emvl::AircraftDataset dataset2( dataset ); //x

        assert( dataset2.invariant() );
        assert( dataset2 == dataset );
        assert( ! ( dataset2 != dataset ) );

        dataset = dataset2;

        assert( dataset.invariant() );
        assert( dataset == dataset2 );
        assert( ! ( dataset != dataset2 ) );

        testDataset( dataset, false );
      }
    }
  }
}



/******************************************************************************
PURPOSE: testRegriddedAircraftDataset - Test emvl::RegriddedAircraftDataset.
******************************************************************************/

static void testRegriddedAircraftDataset() {

  printf( "testRegriddedAircraftDataset()\n" );

  printf( "\nTesting failure to construct...\n" );

  try {
    const emvl::RegriddedAircraftDataset dataset( emptyFileName );//x
  } catch ( std::exception& e ) {
    fprintf( stderr, "%s\n", e.what() );
  }

  try {
    const emvl::ResourceHandler<FILE*>
      fileCloser( fopen( emptyFileName, "rb" ), fclose );
    FILE* const file = fileCloser.get();

    if ( file ) {
      const emvl::RegriddedAircraftDataset dataset( file );//x
    }
  } catch ( std::exception& e ) {
    fprintf( stderr, "%s\n", e.what() );
  }

  try {
    const emvl::RegriddedAircraftDataset
      RegriddedAircraftDataset( invalidRegriddedAircraftTestFileName );//x
  } catch ( std::exception& e ) {
    fprintf( stderr, "%s\n", e.what() );
  }

  try {
    const emvl::ResourceHandler<FILE*>
      fileCloser( fopen( invalidRegriddedAircraftTestFileName, "rb" ), fclose);
    FILE* const file = fileCloser.get();

    if ( file ) {
      const emvl::RegriddedAircraftDataset RegriddedAircraftDataset( file );//x
    }
  } catch ( std::exception& e ) {
    fprintf( stderr, "%s\n", e.what() );
  }

  printf( "\n\nTesting successful cases...\n\n" );

  for ( size_t index = 0; index < regriddedAircraftFileCount; ++index ) {
    const char* const fileName = regriddedAircraftFiles[ index ];
    printf( "%s\n", fileName );
    {
      emvl::RegriddedAircraftDataset dataset( fileName ); //x

      assert( dataset.invariant() );
      assert( dataset == dataset );
      assert( ! ( dataset != dataset ) );

      const emvl::RegriddedAircraftDataset
        dataset2( dataset ); //x

      assert( dataset2.invariant() );
      assert( dataset2 == dataset );
      assert( ! ( dataset2 != dataset ) );

      dataset = dataset2;

      assert( dataset.invariant() );
      assert( dataset == dataset2 );
      assert( ! ( dataset != dataset2 ) );

      testDataset( dataset, true );
    }
    {
      const emvl::ResourceHandler<FILE*>
        fileCloser( fopen( fileName, "rb" ), fclose );
      FILE* const file = fileCloser.get();

      if ( file ) {
        emvl::RegriddedAircraftDataset dataset( file ); //x

        assert( dataset.invariant() );
        assert( dataset == dataset );
        assert( ! ( dataset != dataset ) );

        const emvl::RegriddedAircraftDataset
          dataset2( dataset ); //x

        assert( dataset2.invariant() );
        assert( dataset2 == dataset );
        assert( ! ( dataset2 != dataset ) );

        dataset = dataset2;

        assert( dataset.invariant() );
        assert( dataset == dataset2 );
        assert( ! ( dataset != dataset2 ) );

        testDataset( dataset, false );
      }
    }
  }
}



/******************************************************************************
PURPOSE: testProfileDataset - Test emvl::ProfileDataset class.
******************************************************************************/

static void testProfileDataset() {

  printf( "testProfileDataset()\n" );

  printf( "Testing failure to construct...\n" );

  try {
    const emvl::ProfileDataset dataset( emptyFileName ); //x
  } catch ( std::exception& e ) {
    fprintf( stderr, "%s\n", e.what() );
  }

  try {
    const emvl::ResourceHandler<FILE*>
      fileCloser( fopen( emptyFileName, "rb" ), fclose );
    FILE* const file = fileCloser.get();

    if ( file ) {
      const emvl::ProfileDataset dataset( file ); //x
    }
  } catch ( std::exception& e ) {
    fprintf( stderr, "%s\n", e.what() );
  }

  try {
    const emvl::ProfileDataset dataset( invalidProfileTestFileName ); //x
  } catch ( std::exception& e ) {
    fprintf( stderr, "%s\n", e.what() );
  }

  try {
    const emvl::ResourceHandler<FILE*>
      fileCloser( fopen( invalidProfileTestFileName, "rb"), fclose );
    FILE* const file = fileCloser.get();

    if ( file ) {
      const emvl::ProfileDataset dataset( file ); //x
    }
  } catch ( std::exception& e ) {
    fprintf( stderr, "%s\n", e.what() );
  }

  printf( "\n\nTesting successful cases...\n\n" );

  for ( size_t index = 0; index < profileFileCount; ++index ) {
    const char* const fileName = profileFiles[ index ];
    printf( "%s\n", fileName );
    {
      emvl::ProfileDataset dataset( fileName ); //x

      assert( dataset.invariant() );
      assert( dataset == dataset );
      assert( ! ( dataset != dataset ) );
      const int variable = dataset.dataVariable();
      dataset.dataVariable( 0 );
      dataset.dataVariable( variable );
      assert( dataset.layers() == 0 );
      assert( dataset.rows() == 0 );
      assert( dataset.columns() == 0 );

      const emvl::ProfileDataset dataset2( dataset ); //x

      assert( dataset2.invariant() );
      assert( dataset2 == dataset );
      assert( ! ( dataset2 != dataset ) );

      dataset = dataset2;

      assert( dataset.invariant() );
      assert( dataset == dataset2 );
      assert( ! ( dataset != dataset2 ) );

      assert( dataset.profiles() );
      assert( dataset.timestamp( 0 ).invariant() );
      assert( dataset.points( 0 ) );

      const size_t profiles = dataset.profiles();

      for ( size_t profile = 0; profile < profiles; ++profile ) {
        assert( dataset.note( profile ).size() );
        printf( "%s\n", dataset.note( profile ).c_str() );
      }

      testDataset( dataset, true );
    }
    {
      const emvl::ResourceHandler<FILE*>
        fileCloser( fopen( fileName, "rb" ), fclose );
      FILE* const file = fileCloser.get();

      if ( file ) {
        emvl::ProfileDataset dataset( file ); //x

        assert( dataset.invariant() );
        assert( dataset == dataset );
        assert( ! ( dataset != dataset ) );
        const int variable = dataset.dataVariable();
        dataset.dataVariable( 0 );
        dataset.dataVariable( variable );
        assert( dataset.layers() == 0 );
        assert( dataset.rows() == 0 );
        assert( dataset.columns() == 0 );

        const emvl::ProfileDataset dataset2( dataset ); //x

        assert( dataset2.invariant() );
        assert( dataset2 == dataset );
        assert( ! ( dataset2 != dataset ) );

        dataset = dataset2;

        assert( dataset.invariant() );
        assert( dataset == dataset2 );
        assert( ! ( dataset != dataset2 ) );

        assert( dataset.profiles() );
        assert( dataset.timestamp( 0 ).invariant() );
        assert( dataset.points( 0 ) );

        const size_t profiles = dataset.profiles();

        for ( size_t profile = 0; profile < profiles; ++profile ) {
          assert( dataset.note( profile ).size() );
          printf( "%s\n", dataset.note( profile ).c_str() );
        }

        testDataset( dataset, false );
      }
    }
  }
}



/******************************************************************************
PURPOSE: testRegriddedProfileDataset - Test emvl::RegriddedProfileDataset.
******************************************************************************/

static void testRegriddedProfileDataset() {

  printf( "testRegriddedProfileDataset()\n" );

  printf( "\nTesting failure to construct...\n" );

  try {
    const emvl::RegriddedProfileDataset dataset( emptyFileName );//x
  } catch ( std::exception& e ) {
    fprintf( stderr, "%s\n", e.what() );
  }

  try {
    const emvl::ResourceHandler<FILE*>
      fileCloser( fopen( emptyFileName, "rb" ), fclose );
    FILE* const file = fileCloser.get();

    if ( file ) {
      const emvl::RegriddedProfileDataset dataset( file );//x
    }
  } catch ( std::exception& e ) {
    fprintf( stderr, "%s\n", e.what() );
  }

  try {
    const emvl::RegriddedProfileDataset
      RegriddedProfileDataset( invalidRegriddedProfileTestFileName );//x
  } catch ( std::exception& e ) {
    fprintf( stderr, "%s\n", e.what() );
  }

  try {
    const emvl::ResourceHandler<FILE*>
      fileCloser( fopen( invalidRegriddedProfileTestFileName, "rb" ), fclose);
    FILE* const file = fileCloser.get();

    if ( file ) {
      const emvl::RegriddedProfileDataset RegriddedProfileDataset( file );//x
    }
  } catch ( std::exception& e ) {
    fprintf( stderr, "%s\n", e.what() );
  }

  printf( "\n\nTesting successful cases...\n\n" );

  for ( size_t index = 0; index < regriddedProfileFileCount; ++index ) {
    const char* const fileName = regriddedProfileFiles[ index ];
    printf( "%s\n", fileName );
    {
      emvl::RegriddedProfileDataset dataset( fileName ); //x

      assert( dataset.invariant() );
      assert( dataset == dataset );
      assert( ! ( dataset != dataset ) );

      const emvl::RegriddedProfileDataset
        dataset2( dataset ); //x

      assert( dataset2.invariant() );
      assert( dataset2 == dataset );
      assert( ! ( dataset2 != dataset ) );

      dataset = dataset2;

      assert( dataset.invariant() );
      assert( dataset == dataset2 );
      assert( ! ( dataset != dataset2 ) );

      testDataset( dataset, true );
    }
    {
      const emvl::ResourceHandler<FILE*>
        fileCloser( fopen( fileName, "rb" ), fclose );
      FILE* const file = fileCloser.get();

      if ( file ) {
        emvl::RegriddedProfileDataset dataset( file ); //x

        assert( dataset.invariant() );
        assert( dataset == dataset );
        assert( ! ( dataset != dataset ) );

        const emvl::RegriddedProfileDataset
          dataset2( dataset ); //x

        assert( dataset2.invariant() );
        assert( dataset2 == dataset );
        assert( ! ( dataset2 != dataset ) );

        dataset = dataset2;

        assert( dataset.invariant() );
        assert( dataset == dataset2 );
        assert( ! ( dataset != dataset2 ) );

        testDataset( dataset, false );
      }
    }
  }
}



/******************************************************************************
PURPOSE: testCALIPSODataset - Test emvl::CALIPSODataset class.
******************************************************************************/

static void testCALIPSODataset() {

  printf( "testCALIPSODataset()\n" );

  printf( "Testing failure to construct...\n" );

  try {
    const emvl::CALIPSODataset dataset( emptyFileName ); //x
  } catch ( std::exception& e ) {
    fprintf( stderr, "%s\n", e.what() );
  }

  try {
    const emvl::ResourceHandler<FILE*>
      fileCloser( fopen( emptyFileName, "rb" ), fclose );
    FILE* const file = fileCloser.get();

    if ( file ) {
      const emvl::CALIPSODataset dataset( file ); //x
    }
  } catch ( std::exception& e ) {
    fprintf( stderr, "%s\n", e.what() );
  }

  try {
    const emvl::CALIPSODataset dataset( invalidCALIPSOTestFileName ); //x
  } catch ( std::exception& e ) {
    fprintf( stderr, "%s\n", e.what() );
  }

  try {
    const emvl::ResourceHandler<FILE*>
      fileCloser( fopen( invalidCALIPSOTestFileName, "rb"), fclose );
    FILE* const file = fileCloser.get();

    if ( file ) {
      const emvl::CALIPSODataset dataset( file ); //x
    }
  } catch ( std::exception& e ) {
    fprintf( stderr, "%s\n", e.what() );
  }

  printf( "\n\nTesting successful cases...\n\n" );

  for ( size_t index = 0; index < calipsoFileCount; ++index ) {
    const char* const fileName = calipsoFiles[ index ];
    printf( "%s\n", fileName );
    {
      emvl::CALIPSODataset dataset( fileName ); //x

      assert( dataset.invariant() );
      assert( dataset == dataset );
      assert( ! ( dataset != dataset ) );
      const int variable = dataset.dataVariable();
      dataset.dataVariable( 0 );
      dataset.dataVariable( variable );
      assert( dataset.layers() == 0 );
      assert( dataset.rows() == 0 );
      assert( dataset.columns() == 0 );

      const emvl::CALIPSODataset dataset2( dataset ); //x

      assert( dataset2.invariant() );
      assert( dataset2 == dataset );
      assert( ! ( dataset2 != dataset ) );

      dataset = dataset2;

      assert( dataset.invariant() );
      assert( dataset == dataset2 );
      assert( ! ( dataset != dataset2 ) );

      testDataset( dataset, true );
    }
    {
      const emvl::ResourceHandler<FILE*>
        fileCloser( fopen( fileName, "rb" ), fclose );
      FILE* const file = fileCloser.get();

      if ( file ) {
        emvl::CALIPSODataset dataset( file ); //x

        assert( dataset.invariant() );
        assert( dataset == dataset );
        assert( ! ( dataset != dataset ) );
        const int variable = dataset.dataVariable();
        dataset.dataVariable( 0 );
        dataset.dataVariable( variable );
        assert( dataset.layers() == 0 );
        assert( dataset.rows() == 0 );
        assert( dataset.columns() == 0 );

        const emvl::CALIPSODataset dataset2( dataset ); //x

        assert( dataset2.invariant() );
        assert( dataset2 == dataset );
        assert( ! ( dataset2 != dataset ) );

        dataset = dataset2;

        assert( dataset.invariant() );
        assert( dataset == dataset2 );
        assert( ! ( dataset != dataset2 ) );

        testDataset( dataset, false );
      }
    }
  }
}



/******************************************************************************
PURPOSE: testRegriddedCALIPSODataset - Test emvl::RegriddedCALIPSODataset.
******************************************************************************/

static void testRegriddedCALIPSODataset() {

  printf( "testRegriddedCALIPSODataset()\n" );

  printf( "\nTesting failure to construct...\n" );

  try {
    const emvl::RegriddedCALIPSODataset dataset( emptyFileName );//x
  } catch ( std::exception& e ) {
    fprintf( stderr, "%s\n", e.what() );
  }

  try {
    const emvl::ResourceHandler<FILE*>
      fileCloser( fopen( emptyFileName, "rb" ), fclose );
    FILE* const file = fileCloser.get();

    if ( file ) {
      const emvl::RegriddedCALIPSODataset dataset( file );//x
    }
  } catch ( std::exception& e ) {
    fprintf( stderr, "%s\n", e.what() );
  }

  try {
    const emvl::RegriddedCALIPSODataset
      dataset( invalidRegriddedCALIPSOTestFileName );//x
  } catch ( std::exception& e ) {
    fprintf( stderr, "%s\n", e.what() );
  }

  try {
    const emvl::ResourceHandler<FILE*>
      fileCloser( fopen( invalidRegriddedCALIPSOTestFileName, "rb" ), fclose);
    FILE* const file = fileCloser.get();

    if ( file ) {
      const emvl::RegriddedCALIPSODataset RegriddedCALIPSODataset( file );//x
    }
  } catch ( std::exception& e ) {
    fprintf( stderr, "%s\n", e.what() );
  }

  printf( "\n\nTesting successful cases...\n\n" );

  for ( size_t index = 0; index < regriddedCALIPSOFileCount; ++index ) {
    const char* const fileName = regriddedCALIPSOFiles[ index ];
    printf( "%s\n", fileName );
    {
      emvl::RegriddedCALIPSODataset dataset( fileName ); //x

      assert( dataset.invariant() );
      assert( dataset == dataset );
      assert( ! ( dataset != dataset ) );

      const emvl::RegriddedCALIPSODataset
        dataset2( dataset ); //x

      assert( dataset2.invariant() );
      assert( dataset2 == dataset );
      assert( ! ( dataset2 != dataset ) );

      dataset = dataset2;

      assert( dataset.invariant() );
      assert( dataset == dataset2 );
      assert( ! ( dataset != dataset2 ) );

      testDataset( dataset, true );
    }
    {
      const emvl::ResourceHandler<FILE*>
        fileCloser( fopen( fileName, "rb" ), fclose );
      FILE* const file = fileCloser.get();

      if ( file ) {
        emvl::RegriddedCALIPSODataset dataset( file ); //x

        assert( dataset.invariant() );
        assert( dataset == dataset );
        assert( ! ( dataset != dataset ) );

        const emvl::RegriddedCALIPSODataset
          dataset2( dataset ); //x

        assert( dataset2.invariant() );
        assert( dataset2 == dataset );
        assert( ! ( dataset2 != dataset ) );

        dataset = dataset2;

        assert( dataset.invariant() );
        assert( dataset == dataset2 );
        assert( ! ( dataset != dataset2 ) );

        testDataset( dataset, false );
      }
    }
  }
}



/******************************************************************************
PURPOSE: testGridDataset - Test emvl::GridDataset class.
******************************************************************************/

static void testGridDataset() {

  printf( "testGridDataset()\n" );

  printf( "Testing failure to construct...\n" );

  try {
    const emvl::GridDataset dataset( emptyFileName ); //x
  } catch ( std::exception& e ) {
    fprintf( stderr, "%s\n", e.what() );
  }

  try {
    const emvl::ResourceHandler<FILE*>
      fileCloser( fopen( emptyFileName, "rb" ), fclose );
    FILE* const file = fileCloser.get();

    if ( file ) {
      const emvl::GridDataset dataset( file ); //x
    }
  } catch ( std::exception& e ) {
    fprintf( stderr, "%s\n", e.what() );
  }

  try {
    const emvl::GridDataset dataset( invalidGridTestFileName ); //x
  } catch ( std::exception& e ) {
    fprintf( stderr, "%s\n", e.what() );
  }

  try {
    const emvl::ResourceHandler<FILE*>
      fileCloser( fopen( invalidGridTestFileName, "rb"), fclose );
    FILE* const file = fileCloser.get();

    if ( file ) {
      const emvl::GridDataset dataset( file ); //x
    }
  } catch ( std::exception& e ) {
    fprintf( stderr, "%s\n", e.what() );
  }

  printf( "\n\nTesting successful cases...\n\n" );

  for ( size_t index = 0; index < gridFileCount; ++index ) {
    const char* const fileName = gridFiles[ index ];
    printf( "%s\n", fileName );
    {
      emvl::GridDataset dataset( fileName ); //x

      assert( dataset.invariant() );
      assert( dataset == dataset );
      assert( ! ( dataset != dataset ) );
      const int variable = dataset.dataVariable();
      dataset.dataVariable( 0 );
      dataset.dataVariable( variable );
      assert( dataset.isStationary() );
      assert( dataset.isStructuredGrid() );
      assert( dataset.layers() == 0 );
      assert( dataset.rows() > 0 );
      assert( dataset.columns() > 0 );

      const emvl::GridDataset dataset2( dataset ); //x

      assert( dataset2.invariant() );
      assert( dataset2 == dataset );
      assert( ! ( dataset2 != dataset ) );

      dataset = dataset2;

      assert( dataset.invariant() );
      assert( dataset == dataset2 );
      assert( ! ( dataset != dataset2 ) );

      testDataset( dataset, true );
    }
    {
      const emvl::ResourceHandler<FILE*>
        fileCloser( fopen( fileName, "rb" ), fclose );
      FILE* const file = fileCloser.get();

      if ( file ) {
        emvl::GridDataset dataset( file ); //x

        assert( dataset.invariant() );
        assert( dataset == dataset );
        assert( ! ( dataset != dataset ) );
        const int variable = dataset.dataVariable();
        dataset.dataVariable( 0 );
        dataset.dataVariable( variable );
        assert( dataset.isStationary() );
        assert( dataset.isStructuredGrid() );
        assert( dataset.layers() == 0 );
        assert( dataset.rows() > 0 );
        assert( dataset.columns() > 0 );

        const emvl::GridDataset dataset2( dataset ); //x

        assert( dataset2.invariant() );
        assert( dataset2 == dataset );
        assert( ! ( dataset2 != dataset ) );

        dataset = dataset2;

        assert( dataset.invariant() );
        assert( dataset == dataset2 );
        assert( ! ( dataset != dataset2 ) );

        testDataset( dataset, false );
      }
    }
  }
}



/******************************************************************************
PURPOSE: testRegriddedGridDataset - Test emvl::RegriddedGridDataset.
******************************************************************************/

static void testRegriddedGridDataset() {

  printf( "testRegriddedGridDataset()\n" );

  printf( "\nTesting failure to construct...\n" );

  try {
    const emvl::RegriddedGridDataset dataset( emptyFileName );//x
  } catch ( std::exception& e ) {
    fprintf( stderr, "%s\n", e.what() );
  }

  try {
    const emvl::ResourceHandler<FILE*>
      fileCloser( fopen( emptyFileName, "rb" ), fclose );
    FILE* const file = fileCloser.get();

    if ( file ) {
      const emvl::RegriddedGridDataset dataset( file );//x
    }
  } catch ( std::exception& e ) {
    fprintf( stderr, "%s\n", e.what() );
  }

  try {
    const emvl::RegriddedGridDataset
      dataset( invalidRegriddedGridTestFileName );//x
  } catch ( std::exception& e ) {
    fprintf( stderr, "%s\n", e.what() );
  }

  try {
    const emvl::ResourceHandler<FILE*>
      fileCloser( fopen( invalidRegriddedGridTestFileName, "rb" ), fclose);
    FILE* const file = fileCloser.get();

    if ( file ) {
      const emvl::RegriddedGridDataset RegriddedGridDataset( file );//x
    }
  } catch ( std::exception& e ) {
    fprintf( stderr, "%s\n", e.what() );
  }

  printf( "\n\nTesting successful cases...\n\n" );

  for ( size_t index = 0; index < regriddedGridFileCount; ++index ) {
    const char* const fileName = regriddedGridFiles[ index ];
    printf( "%s\n", fileName );
    {
      emvl::RegriddedGridDataset dataset( fileName ); //x

      assert( dataset.invariant() );
      assert( dataset == dataset );
      assert( ! ( dataset != dataset ) );

      const emvl::RegriddedGridDataset dataset2( dataset ); //x

      assert( dataset2.invariant() );
      assert( dataset2 == dataset );
      assert( ! ( dataset2 != dataset ) );

      dataset = dataset2;

      assert( dataset.invariant() );
      assert( dataset == dataset2 );
      assert( ! ( dataset != dataset2 ) );

      testDataset( dataset, true );
    }
    {
      const emvl::ResourceHandler<FILE*>
        fileCloser( fopen( fileName, "rb" ), fclose );
      FILE* const file = fileCloser.get();

      if ( file ) {
        emvl::RegriddedGridDataset dataset( file ); //x

        assert( dataset.invariant() );
        assert( dataset == dataset );
        assert( ! ( dataset != dataset ) );

        const emvl::RegriddedGridDataset dataset2( dataset ); //x

        assert( dataset2.invariant() );
        assert( dataset2 == dataset );
        assert( ! ( dataset2 != dataset ) );

        dataset = dataset2;

        assert( dataset.invariant() );
        assert( dataset == dataset2 );
        assert( ! ( dataset != dataset2 ) );

        testDataset( dataset, false );
      }
    }
  }
}



/******************************************************************************
PURPOSE: testPolygonDataset - Test emvl::PolygonDataset class.
******************************************************************************/

static void testPolygonDataset() {

  printf( "testPolygonDataset()\n" );

  printf( "Testing failure to construct...\n" );

  try {
    const emvl::PolygonDataset dataset( emptyFileName ); //x
  } catch ( std::exception& e ) {
    fprintf( stderr, "%s\n", e.what() );
  }

  try {
    const emvl::ResourceHandler<FILE*>
    fileCloser( fopen( emptyFileName, "rb" ), fclose );
    FILE* const file = fileCloser.get();

    if ( file ) {
      const emvl::PolygonDataset dataset( file ); //x
    }
  } catch ( std::exception& e ) {
    fprintf( stderr, "%s\n", e.what() );
  }

  try {
    const emvl::PolygonDataset dataset( invalidPolygonTestFileName ); //x
  } catch ( std::exception& e ) {
    fprintf( stderr, "%s\n", e.what() );
  }

  try {
    const emvl::ResourceHandler<FILE*>
    fileCloser( fopen( invalidPolygonTestFileName, "rb" ), fclose );
    FILE* const file = fileCloser.get();

    if ( file ) {
      const emvl::PolygonDataset dataset( file ); //x
    }
  } catch ( std::exception& e ) {
    fprintf( stderr, "%s\n", e.what() );
  }

  printf( "\n\nTesting successful cases...\n\n" );

  for ( size_t index = 0; index < polygonFileCount; ++index ) {
    const char* const fileName = polygonFiles[ index ];
    printf( "%s\n", fileName );
    {
      emvl::PolygonDataset dataset( fileName ); //x

      assert( dataset.invariant() );
      assert( dataset == dataset );
      assert( ! ( dataset != dataset ) );
      const int variable = dataset.dataVariable();
      dataset.dataVariable( 0 );
      dataset.dataVariable( variable );
      assert( dataset.layers() == 0 );
      assert( dataset.rows() == 0 );
      assert( dataset.columns() == 0 );

      const emvl::PolygonDataset dataset2( dataset ); //x

      assert( dataset2.invariant() );
      assert( dataset2 == dataset );
      assert( ! ( dataset2 != dataset ) );

      dataset = dataset2;

      assert( dataset.invariant() );
      assert( dataset == dataset2 );
      assert( ! ( dataset != dataset2 ) );

      testDataset( dataset, true );
    }

    {
      const emvl::ResourceHandler<FILE*>
      fileCloser( fopen( fileName, "rb" ), fclose );
      FILE* const file = fileCloser.get();

      if ( file ) {
        emvl::PolygonDataset dataset( file ); //x

        assert( dataset.invariant() );
        assert( dataset == dataset );
        assert( ! ( dataset != dataset ) );
        const int variable = dataset.dataVariable();
        dataset.dataVariable( 0 );
        dataset.dataVariable( variable );
        assert( dataset.layers() == 0 );
        assert( dataset.rows() == 0 );
        assert( dataset.columns() == 0 );

        const emvl::PolygonDataset dataset2( dataset ); //x

        assert( dataset2.invariant() );
        assert( dataset2 == dataset );
        assert( ! ( dataset2 != dataset ) );

        dataset = dataset2;

        assert( dataset.invariant() );
        assert( dataset == dataset2 );
        assert( ! ( dataset != dataset2 ) );

        testDataset( dataset, false );
      }
    }
  }
}



/******************************************************************************
PURPOSE: testDatasetManager - Test emvl::DatasetManager class.
******************************************************************************/

static void testDatasetManager() {

  printf( "testDatasetManager()\n" );

  try {
    printf( "calling emvl::DatasetManager::checkAndSetServerHost( curlPath = '%s')\n",
           curlPath );
    emvl::DatasetManager::checkAndSetServerHost( curlPath ); //x
    printf("emvl::DatasetManager::remoteVersion() = '%d\n",
            emvl::DatasetManager::remoteVersion() );

    // Test isValidPurpleAirKey():

    printf( "calling DatasetManager::isValidPurpleAirKey()\n" );
    const bool ok = emvl::DatasetManager::isValidPurpleAirKey( "EPA" );
    printf( "key valid = %d\n", ok );

    printf( "constructing DatasetManager\n" );
    emvl::DatasetManager datasetManager( true, 0 ); //x
    printf( "ok\n" );

    typedef struct {
      const int aggregateMethod;  // emvl::QueryParameters::AGGREGATE_NONE...
      const int regridMethod;     // emvl::QueryParameters::REGRID_NONE...
      const char* const variable; // E.g., "aqs.ozone".
      const emvl::Timestamp::UTCString startTime; // "2011-03-23T00:00:00-0000"
      const emvl::Timestamp::UTCString endTime;   // "2011-03-23T23:59:59-0000"
      const emvl::Bounds bounds; // Lon-lat subset bounding box.
    } Entry;
    const Entry table[] = {
      {
        emvl::QueryParameters::AGGREGATE_NONE,
        emvl::QueryParameters::REGRID_NONE,
        "airnow.pm25",
        "2021-03-23T00:00:00-0000",
        "2021-03-24T12:59:59-0000",
        emvl::Bounds( -90.0, -88.0, 30.0, 32.0 )
      },
      {
        emvl::QueryParameters::AGGREGATE_NONE,
        emvl::QueryParameters::REGRID_WEIGHTED,
        "airnow.pm25",
        "2021-03-23T00:00:00-0000",
        "2021-03-24T12:59:59-0000",
        emvl::Bounds( -90.0, -88.0, 30.0, 32.0 )
      },
      {
        emvl::QueryParameters::AGGREGATE_NONE,
        emvl::QueryParameters::REGRID_NONE,
        "aqs.ozone",
        "2001-08-26T00:00:00-0000",
        "2001-08-26T12:59:59-0000",
        emvl::Bounds( -90.0, -89.0, 28.0, 30.0 )
      },
      {
        emvl::QueryParameters::AGGREGATE_NONE,
        emvl::QueryParameters::REGRID_NONE,
        "faqsd.ozone_daily_8hour_maximum",
        "2002-08-28T00:00:00-0000",
        "2002-08-28T23:59:59-0000",
        emvl::Bounds( -90.0, -89.5, 30.0, 30.5 )
      },
      {
        emvl::QueryParameters::AGGREGATE_NONE,
        emvl::QueryParameters::REGRID_NONE,
        "nesdis.pm25",
        "2007-08-26T00:00:00-0000",
        "2007-08-27T01:59:59-0000",
        emvl::Bounds( -75.0, -70.5, 35.0, 40.0 )
      },
      {
        emvl::QueryParameters::AGGREGATE_NONE,
        emvl::QueryParameters::REGRID_NONE,
        "uvnet.irradiance",
        "1996-12-01T00:00:00-0000",
        "1997-01-31T23:59:59-0000",
        emvl::Bounds( -118.0, -117.0, 33.0, 35.0 )
      },
      {
        emvl::QueryParameters::AGGREGATE_NONE,
        emvl::QueryParameters::REGRID_NONE,
        "cmaq.equates.conus.conc.no2",
        "2006-08-28T00:00:00-0000",
        "2006-08-28T23:59:59-0000",
        emvl::Bounds( -90.0, -89.0, 30.0, 31.0 )
      },
      {
        emvl::QueryParameters::AGGREGATE_NONE,
        emvl::QueryParameters::REGRID_NONE,
        "cmaq.equates.hemi.dep.totdep_n",
        "2006-08-28T00:00:00-0000",
        "2006-08-28T00:59:59-0000",
        emvl::Bounds( -180.0, 180.0, -90.0, 90.0 )
      },
      {
        emvl::QueryParameters::AGGREGATE_DAILY_MAX8,
        emvl::QueryParameters::REGRID_NONE,
        "cmaq.equates.conus.aconc.o3",
        "2006-08-28T00:00:00-0000",
        "2006-08-29T23:59:59-0000",
        emvl::Bounds( -180.0, 180.0, -90.0, 90.0 )
      },
      {
        emvl::QueryParameters::AGGREGATE_NONE,
        emvl::QueryParameters::REGRID_NONE,
        "gasp.aod",
        "2011-03-23T00:00:00-0000",
        "2011-03-23T23:59:59-0000",
        emvl::Bounds( -90.0, -89.0, 30.0, 50.0 )
      },
      {
        emvl::QueryParameters::AGGREGATE_NONE,
        emvl::QueryParameters::REGRID_WEIGHTED,
        "gasp.aod",
        "2011-03-23T00:00:00-0000",
        "2011-03-23T23:59:59-0000",
        emvl::Bounds( -90.0, -89.0, 30.0, 50.0 )
      },
      {
        emvl::QueryParameters::AGGREGATE_NONE,
        emvl::QueryParameters::REGRID_NONE,
        "modis.mod4.optical_depth_land_and_ocean",
        "2005-08-28T00:00:00-0000",
        "2005-08-28T23:59:59-0000",
        emvl::Bounds( -97.0, -90.0, 28.0, 32.0 )
      },
      {
        emvl::QueryParameters::AGGREGATE_NONE,
        emvl::QueryParameters::REGRID_WEIGHTED,
        "modis.mod4.optical_depth_land_and_ocean",
        "2005-08-28T00:00:00-0000",
        "2005-08-28T23:59:59-0000",
        emvl::Bounds( -97.0, -90.0, 28.0, 32.0 )
      },
      {
        emvl::QueryParameters::AGGREGATE_NONE,
        emvl::QueryParameters::REGRID_NONE,
        "viirsnoaa.jrraod.AOD550",
        "2019-06-15T00:00:00-0000",
        "2019-06-15T23:59:59-0000",
        emvl::Bounds( -120.0, -70.0, 25.0, 30.0 )
      },
      {
        emvl::QueryParameters::AGGREGATE_NONE,
        emvl::QueryParameters::REGRID_WEIGHTED,
        "viirsnoaa.jrraod.AOD550",
        "2019-06-15T00:00:00-0000",
        "2019-06-15T23:59:59-0000",
        emvl::Bounds( -120.0, -70.0, 25.0, 30.0 )
      },
      {
        emvl::QueryParameters::AGGREGATE_NONE,
        emvl::QueryParameters::REGRID_NONE,
        "pandora.L2_rfuh5p1_8.formaldehyde_layer_amount",
        "2022-07-01T00:00:00-0000",
        "2022-07-01T23:59:59-0000",
        emvl::Bounds( -130.0, -65.0, 24.0, 50.0 )
      },
      {
        emvl::QueryParameters::AGGREGATE_HOURLY,
        emvl::QueryParameters::REGRID_NONE,
        "pandora.L2_rfuh5p1_8.formaldehyde_layer_amount",
        "2022-07-01T00:00:00-0000",
        "2022-07-01T23:59:59-0000",
        emvl::Bounds( -130.0, -65.0, 24.0, 50.0 )
      },
      {
        emvl::QueryParameters::AGGREGATE_DAILY,
        emvl::QueryParameters::REGRID_NONE,
        "pandora.L2_rfuh5p1_8.formaldehyde_layer_amount",
        "2022-07-01T00:00:00-0000",
        "2022-07-01T23:59:59-0000",
        emvl::Bounds( -130.0, -65.0, 24.0, 50.0 )
      },
      {
        emvl::QueryParameters::AGGREGATE_NONE,
        emvl::QueryParameters::REGRID_NONE,
        "calipso.l2_05kmapro.extinction_coefficient_532",
        "2014-07-05T00:00:00-0000",
        "2014-07-05T23:59:59-0000",
        emvl::Bounds( -110.0, -70.0, 25.0, 50.0 )
      },
      {
        emvl::QueryParameters::AGGREGATE_NONE,
        emvl::QueryParameters::REGRID_WEIGHTED,
        "calipso.l2_05kmapro.extinction_coefficient_532",
        "2014-07-05T00:00:00-0000",
        "2014-07-05T23:59:59-0000",
        emvl::Bounds( -110.0, -70.0, 25.0, 50.0 )
      },
      {
        emvl::QueryParameters::AGGREGATE_NONE,
        emvl::QueryParameters::REGRID_NONE,
        "calipso.l2_05kmalay.feature_optical_depth_532",
        "2006-07-05T00:00:00-0000",
        "2006-07-05T23:59:59-0000",
        emvl::Bounds( -110.0, -70.0, 25.0, 50.0 )
      },
      {
        emvl::QueryParameters::AGGREGATE_NONE,
        emvl::QueryParameters::REGRID_WEIGHTED,
        "calipso.l2_05kmalay.feature_optical_depth_532",
        "2006-07-05T00:00:00-0000",
        "2006-07-05T23:59:59-0000",
        emvl::Bounds( -110.0, -70.0, 25.0, 50.0 )
      },
      {
        emvl::QueryParameters::AGGREGATE_NONE,
        emvl::QueryParameters::REGRID_NONE,
        "mozaic.ozone",
        "2006-07-03T00:00:00-0000",
        "2006-07-03T23:59:59-0000",
        emvl::Bounds( -90.0, -70.0, 33.0, 34.0 )
      },
      {
        emvl::QueryParameters::AGGREGATE_NONE,
        emvl::QueryParameters::REGRID_WEIGHTED,
        "mozaic.ozone",
        "2006-07-03T00:00:00-0000",
        "2006-07-03T23:59:59-0000",
        emvl::Bounds( -90.0, -70.0, 33.0, 34.0 )
      },
      {
        emvl::QueryParameters::AGGREGATE_NONE,
        emvl::QueryParameters::REGRID_NONE,
        "neubrew.ozone",
        "2010-07-10T00:00:00-0000",
        "2010-07-10T23:59:59-0000",
        emvl::Bounds( -96.0, -95.0, 29.0, 30.0 ),
      },
      {
        emvl::QueryParameters::AGGREGATE_NONE,
        emvl::QueryParameters::REGRID_WEIGHTED,
        "neubrew.ozone",
        "2010-07-10T00:00:00-0000",
        "2010-07-10T23:59:59-0000",
        emvl::Bounds( -96.0, -95.0, 29.0, 30.0 )
      },
      {
        emvl::QueryParameters::AGGREGATE_NONE,
        emvl::QueryParameters::REGRID_NONE,
        "hrrr.wind",
        "2016-07-15T00:00:00-0000",
        "2016-07-15T00:59:59-0000",
        emvl::Bounds( -77.0, -76.0, 37.0, 38.0 ),
      },
      {
        emvl::QueryParameters::AGGREGATE_NONE,
        emvl::QueryParameters::REGRID_WEIGHTED,
        "hrrr.wind",
        "2016-07-15T00:00:00-0000",
        "2016-07-15T00:59:59-0000",
        emvl::Bounds( -77.0, -76.0, 37.0, 38.0 ),
      },
      {
        emvl::QueryParameters::AGGREGATE_NONE,
        emvl::QueryParameters::REGRID_NONE,
        "hms.smoke",
        "2018-07-01T00:00:00-0000",
        "2018-07-01T23:59:59-0000",
        emvl::Bounds( -120.0, -110.0, 30.0, 50.0 ),
      }
    };
    size_t count = sizeof table / sizeof *table;

    std::cout << "RSIG3D_VERSION = " << emvl::DatasetManager::remoteVersion()
              << std::endl;
    std::cout << "webservice = " << emvl::DatasetManager::webservice()
              << std::endl;
    std::cout << "server host = " << emvl::DatasetManager::serverHost()
              << std::endl;
    emvl::DatasetManager::checkAndSetServerHost( curlPath );
    std::cout << "webservice = " << emvl::DatasetManager::webservice()
              << std::endl;
    std::cout << "server host = " << emvl::DatasetManager::serverHost()
              << std::endl;

    // Test remote file retrieval:

    const std::string remoteFile( "download/RSIG3D_VERSION" );
    const std::string localFile( tempFileName );
    datasetManager.retrieveFile( remoteFile, localFile, 30 ); //x
    assert( emvl::fileSize( localFile.c_str() ) == 8 + 1 ); // YYYYMMDD\n


    // Test concurent retrieval:

// #pragma omp parallel for

    for ( long long index = 0; index < (long long) count; ++index ) {
      const Entry& entry = table[ index ];
      double boundsZ[ 2 ] = { emvl::MISSING, emvl::MISSING };
      const int regridAggregateMethod =
        AND4( ! strcmp( entry.variable, "gasp.aod" ),
              entry.aggregateMethod ==
                emvl::QueryParameters::AGGREGATE_NONE,
              entry.regridMethod != emvl::QueryParameters::REGRID_NONE,
              strncmp( entry.startTime, entry.endTime, 9 ) ) ?
          emvl::QueryParameters::REGRID_AGGREGATE_DAILY
        : emvl::QueryParameters::REGRID_AGGREGATE_NONE;
      const emvl::QueryParameters
        query( emvl::Timestamp( entry.startTime ),
               emvl::Timestamp( entry.endTime ),
               entry.bounds,
               boundsZ,
               std::string( entry.variable ),
               emvl::QueryParameters::XDR,
               ".",
               false,
               false,
               strcmp( entry.variable, "cmaq.equates.conus.conc.no2" ) == 0,
               emvl::DataFilterParameters(),
               entry.aggregateMethod,
               entry.regridMethod,
               regridAggregateMethod,
               emvl::CMAQGridParameters() );

      try {
        printf( "retrieving dataset %s%s%s...\n",
               entry.aggregateMethod ? "aggregated " : "",
               entry.regridMethod ? "regridded " : "", entry.variable );
        emvl::Dataset* dataset = datasetManager.retrieveDataset( query, 300 ); //x
        printf( "appending dataset %s...\n", dataset->name().c_str() );
// #pragma omp critical
        datasetManager.append( dataset ); //x
      } catch ( std::exception& e ) {
        fprintf( stderr, "\a\n\nFailure: %s\n\n", e.what() );
      }
    }

    count = datasetManager.count(); // HACK since web servers are often down.
    assert( datasetManager.count() == count );

    // Print and save names of retrieved datasets:

    printf( "\nDatasets:\n" );
    std::vector<std::string> retrievedDatasetNames;

    for ( size_t index = 0; index < count; ++index ) {
      const emvl::Dataset& dataset = datasetManager[ index ];
      assert( dataset.invariant() );
      printf( "%s\n", dataset.name().c_str() );

      if ( dataset.cellType() == emvl::Dataset::POLYGONS ) {
        datasetManager.remove( index ); // HACK: can't write if not read from file.
      } else {
        retrievedDatasetNames.push_back( dataset.name() ); //x
      }
    }

    count = datasetManager.count();

    // Write %" LU " datasets to output directory:

    printf( "\nWriting %" LU " datasets to %s\n", count, testDirectoryName );
    const std::string testDirectory( testDirectoryName );

    datasetManager.writeAll( testDirectory, emvl::QueryParameters::XDR ); //x

    // Read the datasets back into memory:

    printf( "\nReading %" LU " datasets from %s\n", count, testDirectoryName );

    for ( size_t index = 0; index < count; ++index ) {
      const std::string& name = retrievedDatasetNames[ index ];
      std::string fileName( testDirectory + '/' + name + ".xdr" );
      printf( "\nreading %s\n", fileName.c_str() );
      datasetManager.appendDatasetFromFile( fileName ); //x
      assert( datasetManager.count() == count + 1 );
      printf( "read dataset name %s\n\n",
              datasetManager[ datasetManager.count() - 1 ].name().c_str() );
      datasetManager.remove( datasetManager.count() - 1 );
      assert( datasetManager.count() == count );
    }

    // Test retrieveDatasetToFile():

    {
      const Entry& entry = table[ 2 ];
      const double boundsZ[ 2 ] = { emvl::MISSING, emvl::MISSING };
      const emvl::QueryParameters
        query( emvl::Timestamp( entry.startTime ),
               emvl::Timestamp( entry.endTime ),
               entry.bounds,
               boundsZ,
               std::string( entry.variable ),
               emvl::QueryParameters::XDR,
               testDirectory,
               false,
               false,
               false,
               emvl::DataFilterParameters(),
               emvl::QueryParameters::AGGREGATE_NONE,
               emvl::QueryParameters::REGRID_NONE,
               emvl::QueryParameters::REGRID_AGGREGATE_NONE,
               emvl::CMAQGridParameters() );

      try {
        printf( "\nretrieving dataset %s to file\n", entry.variable );
        datasetManager.retrieveDatasetToFile( query ); //x
        assert( datasetManager.count() == count ); // No change.
      } catch ( std::exception& e ) {
        fprintf( stderr, "\a\n\nFailure: %s\n\n", e.what() );
      }

      try {
        printf( "\nretrieving and appending dataset %s\n", entry.variable );
        datasetManager.appendRetrievedDataset( query ); //x
        assert( datasetManager.count() == count + 1 );
      } catch ( std::exception& e ) {
        fprintf( stderr, "\a\n\nFailure: %s\n\n", e.what() );
      }
    }

    if ( datasetManager.count() == count + 1 ) {
      datasetManager.remove( 0 );
      assert( datasetManager.count() == count );      
    }

    datasetManager.removeAll();
    assert( datasetManager.count() == 0 );

  } catch ( std::exception& e ) {
    fprintf( stderr, "\a\n\nFailure: %s\n\n", e.what() );
  }

  testCMAQSubset();

  printf( "\n\n\n-------------- testing failure cases --------------\n\n\n" );

  try {
    emvl::DatasetManager datasetManager( false, printURL );
    const emvl::Bounds bogusBounds( -50.0, -40.0, 50.0, 60.0 ); // Not in US.
    const double boundsZ[ 2 ] = { 0.0, 0.0 };
    emvl::QueryParameters
        bogusQuery( emvl::Timestamp(),
                    emvl::Timestamp(),
                    bogusBounds,
                    boundsZ,
                    std::string( "aqs.ozone" ),
                    emvl::QueryParameters::XDR,
                    ".",
                    false,
                    false,
                    false,
                    emvl::DataFilterParameters(),
                    emvl::QueryParameters::AGGREGATE_NONE,
                    emvl::QueryParameters::REGRID_NONE,
                    emvl::QueryParameters::REGRID_AGGREGATE_NONE,
                    emvl::CMAQGridParameters() );

    emvl::Dataset* dataset = 0;
    const int secondsToFail = 10;

    try {
      dataset = datasetManager.retrieveDataset( bogusQuery, secondsToFail ); //x
    } catch ( std::exception& e ) {
      fprintf( stderr, "%s\n", e.what() );
    }

    assert( dataset == 0 );

    try {
      datasetManager.appendRetrievedDataset( bogusQuery, secondsToFail ); //x
    } catch ( std::exception& e ) {
      fprintf( stderr, "%s\n", e.what() );
    }

    assert( datasetManager.count() == 0 );

    const std::string testDirectory( testDirectoryName );
    bogusQuery.directory( testDirectory );

    try {
      datasetManager.retrieveDatasetToFile( bogusQuery, secondsToFail ); //x
    } catch ( std::exception& e ) {
      fprintf( stderr, "%s\n", e.what() );
    }

    assert( datasetManager.count() == 0 );

    const std::string nonExistentFile( nonExistentFileName );

    try {
      datasetManager.readDataset( nonExistentFile ); //x
    } catch ( std::exception& e ) {
      fprintf( stderr, "%s\n", e.what() );
    }
    
    assert( datasetManager.count() == 0 );

    try {
      datasetManager.appendDatasetFromFile( nonExistentFile ); //x
    } catch ( std::exception& e ) {
      fprintf( stderr, "%s\n", e.what() );
    }
    
    assert( datasetManager.count() == 0 );
  } catch ( std::exception& e ) {
    fprintf( stderr, "\a\n\nFailure: %s\n\n", e.what() );
  }
}



static void testCMAQSubset() {

  printf( "\n\n\n----- testing reading CMAQ files using CMAQSubset ---\n\n\n" );

  const emvl::Bounds bounds( -75.0, -74.0, 40.0, 41.0 );
  const emvl::Timestamp firstTimestamp( 20170701, 0 );
  const emvl::Timestamp lastTimestamp( 20170701, 230000 );
  bool ok = false;
  std::string user;
  std::string host;
  std::string name;
  std::string result;

  std::cout << "\n  ---- Test with pathed CMAQSubset:\n" << std::endl;

  // Set path to CMAQSubset:

  const std::string subsetterDirectory( emvl::DatasetManager::getCurlPath() +
                                        emvl::slash + "bin" +
                                        emvl::slash + emvl::platform );
  std::cout << "set subsetterDirectory = " << subsetterDirectory << std::endl;
  emvl::DatasetManager::setLocalSubsetterDirectory( subsetterDirectory );

  const std::string remoteSubsetterDirectory( ".rsig" );
  std::cout << "set remoteSubsetterDirectory = " << remoteSubsetterDirectory << std::endl;
  emvl::DatasetManager::setRemoteSubsetterDirectory(remoteSubsetterDirectory);

  result = emvl::DatasetManager::getPlatform( user, host );
  std::cout << "emvl::DatasetManager::getPlatform( "
  << user << ", " << host << " ) = " << result << std::endl;
  assert( ! result.empty() );

  result = emvl::DatasetManager::getWorkingDirectory( user, host );
  std::cout << "emvl::DatasetManager::getWorkingDirectory( "
  << user << ", " << host << " ) = " << result << std::endl;
  assert( ! result.empty() );


  name = "junkdir";
  ok = emvl::DatasetManager::ensureDirectoryExists( user, host, name );
  std::cout << "emvl::DatasetManager::ensureDirectoryExists( "
    << user << ", " << host << ", " << name << " ) = " << ok << std::endl;
  assert( ok );
  assert( emvl::isDirectory( name ) );

  name = "test/testdata";
  result = emvl::DatasetManager::getDirectoryListing( user, host, name );
  std::cout << "emvl::DatasetManager::getDirectoryListing( "
    << user << ", " << host << ", " << name << " ) = " << std::endl;
  std::cout << result << std::endl;
  assert( ! result.empty() );

  name = ".";
  result = emvl::DatasetManager::getDirectoryListing( user, host, name );
  std::cout << "emvl::DatasetManager::getDirectoryListing( "
    << user << ", " << host << ", " << name << " ) = " << std::endl;
  std::cout << result << std::endl;
  assert( ! result.empty() );

  name = "test/testdata/CONC.ncf";
  result = emvl::DatasetManager::getVariableListing( user, host, name );
  std::cout << "emvl::DatasetManager::getVariableListing( "
    << user << ", " << host << ", " << name << " ) = " << std::endl;
  std::cout << result << std::endl;
  assert( ! result.empty() );

  result =
    emvl::DatasetManager::subsetCMAQFile( user, host,
                                          "test/testdata/MET/METDOT3D",
                                          "UWIND",
                                          "test/testdata/MET/GRIDCRO2D",
                                          "test/testdata/MET/METCRO3D",
                                          "test/testdata/WWIND.ncf",
                                          "junkdir",
                                          bounds,
                                          firstTimestamp,
                                          lastTimestamp,
                                          1, 1 );

  std::cout << result << std::endl;
  assert( ! result.empty() );

  {
    emvl::DatasetManager datasetManager( false, 0 );
    datasetManager.appendDatasetFromFile( result );
    assert( datasetManager.count() == 1 );
  }

  std::cout << "\n\nChecking if able to access remote host:" << std::endl;

  ok = emvl::readCommandResult( "nslookup atmos1.hesc.epa.gov", result );

  ok = ok && result.find( " can't find " ) == std::string::npos;

  std::cout << "ok = " << ok << ", nslookup result = " << result << std::endl;

  if ( ! ok ) {
    std::cout << "skipping remote file reading tests." << std::endl;
  } else {

    if ( remoteUser ) {
      user = remoteUser;
    } else {
      user.clear();
    }

    host = "atmos1.hesc.epa.gov";

    std::cout << "\n---- Test with remote access " << user << '@' << host
      << std::endl;

    std::cout << "Calling emvl::DatasetManager::installRemoteSubsetter( "
      << user << ", " << host << " )" << std::endl;
    bool ok = emvl::DatasetManager::installRemoteSubsetter( user, host );
    std::cout << "emvl::DatasetManager::installRemoteSubsetter() = " << ok << std::endl;
    assert( ok );

    // Retest with pathed CMAQSubset:

    result = emvl::DatasetManager::getPlatform( user, host );
    std::cout << "emvl::DatasetManager::getPlatform( "
    << user << ", " << host << " ) = " << result << std::endl;
    assert( ! result.empty() );

    result = emvl::DatasetManager::getWorkingDirectory( user, host );
    std::cout << "emvl::DatasetManager::getWorkingDirectory( "
    << user << ", " << host << " ) = " << result << std::endl;
    assert( ! result.empty() );

    name = ".";
    result = emvl::DatasetManager::getDirectoryListing( user, host, name );
    std::cout << "emvl::DatasetManager::getDirectoryListing( "
      << user << ", " << host << ", " << name << " ) = " << std::endl;
    std::cout << result << std::endl;
    assert( ! result.empty() );

    name = "testdata";
    result = emvl::DatasetManager::getDirectoryListing( user, host, name );
    std::cout << "emvl::DatasetManager::getDirectoryListing( "
      << user << ", " << host << ", " << name << " ) = " << std::endl;
    std::cout << result << std::endl;
    assert( ! result.empty() );

    name = "junkdir";
    ok = emvl::DatasetManager::ensureDirectoryExists( user, host, name );
    std::cout << "emvl::DatasetManager::ensureDirectoryExists( "
      << user << ", " << host << ", " << name << " ) = " << ok << std::endl;
    assert( ok );
    assert( emvl::isDirectory( name ) );

    name = "testdata/CONC.ncf";
    result = emvl::DatasetManager::getVariableListing( user, host, name );
    std::cout << "emvl::DatasetManager::getVariableListing( "
      << user << ", " << host << ", " << name << " ) = " << std::endl;
    std::cout << result << std::endl;
    assert( ! result.empty() );

    result =
      emvl::DatasetManager::subsetCMAQFile( user, host,
                                            "testdata/MET/METDOT3D",
                                            "UWIND",
                                            "testdata/MET/GRIDCRO2D",
                                            "testdata/MET/METCRO3D",
                                            "testdata/WWIND.ncf",
                                            "junkdir",
                                            bounds,
                                            firstTimestamp,
                                            lastTimestamp,
                                            0, 0 );

    std::cout << result << std::endl;
    assert( ! result.empty() );

    emvl::DatasetManager datasetManager( false, 0 );
    datasetManager.appendDatasetFromFile( result );
    assert( datasetManager.count() == 1 );
  }
}



