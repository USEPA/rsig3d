
#ifndef QUERYPARAMETERS_H
#define QUERYPARAMETERS_H

/******************************************************************************
PURPOSE: QueryParameters.h - Remote web service query parameters used to
         construct Datasets.
HISTORY: 2013-02-27 plessel.todd@epa.gov
STATUS: unreviewed tested
******************************************************************************/

//================================ INCLUDES ===================================

#include <string> // For std::string.

#include <Bounds.h>               // For Bounds.
#include <Timestamp.h>            // For Timestamp.
#include <DataFilterParameters.h> // For DataFilterParameters.
#include <CMAQGridParameters.h>   // For CMAQGridParameters.

//================================== TYPES ====================================

namespace emvl {

bool isElevation( const double elevation );

class QueryParameters {

private:

  // Attributes:

  Timestamp startingTimestamp_; // Starting and ending date-time range to get.
  Timestamp endingTimestamp_;
  Bounds bounds_;         // Longitude-latitude bounds of subset to retrieve.
  double boundsZ_[ 2 ];   // Minimum and maximum z bounds to retrieve.
  std::string variables_; // E.g., "cmaq.cdc.east.conc.o3,co,no".
  int format_;         // XDR, NETCDF_COARDS/IOAPI, GEOTIFF_RGB/F32, SHAPEFILE.
  std::string directory_; // To write data, if not XDR.
  bool getMetadata_;      // Get (plain text) metadata rather than data?
  bool compress_;  // Use compression/decompression while streaming over net?
  bool integrateCMAQLayers_; // Integrate CMAQ data over layers?
  DataFilterParameters dataFilterParameters_;
  int aggregateMethod_; // AGGREGATE_NONE, AGGREGATE_DAILY_MEAN, ...
  int regridMethod_; // REGRID_NONE, REGRID_NEAREST, REGRID_MEAN, ...
  int regridAggregateMethod_; // REGRID_AGGREGATE_NONE, REGRID_AGGREGATE_ALL.
  CMAQGridParameters cmaqGridParameters_; // Used when regridding.

public:

  // Constants:

  static const double MINIMUM_ELEVATION;
  static const double MAXIMUM_ELEVATION;

  enum { XDR, ASCII, NETCDF_COARDS, NETCDF_IOAPI, SHAPEFILE, KML, ORIGINAL,
         FILE_FORMATS };

  enum {
    REGRID_NONE, REGRID_NEAREST, REGRID_MEAN, REGRID_WEIGHTED, REGRID_METHODS
  };

  enum {
    AGGREGATE_NONE, AGGREGATE_DAILY_MEAN, AGGREGATE_DAILY_MAX,
    AGGREGATE_DAILY_MAX8, AGGREGATE_HOURLY, AGGREGATE_DAILY, AGGREGATE_METHODS
  };

  enum {
    REGRID_AGGREGATE_NONE, REGRID_AGGREGATE_ALL, REGRID_AGGREGATE_DAILY,
    REGRID_AGGREGATE_METHODS
  };

  static const char* const fileFormatNames[ FILE_FORMATS ];
  static const char* const fileFormatExtensions[ FILE_FORMATS ];
  static const char* const aggregateMethodNames[ AGGREGATE_METHODS ];
  static const char* const regridMethodNames[ REGRID_METHODS ];
  static const char* const regridAggregateMethodNames[REGRID_AGGREGATE_METHODS];

  // Commands:

  ~QueryParameters();
  QueryParameters();

  QueryParameters( const Timestamp startingTimestamp,
                   const Timestamp endingTimestamp,
                   const Bounds& bounds,
                   const double boundsZ[ 2 ],
                   const std::string& variables,
                   const int format,
                   const std::string& directory,
                   const bool getMetadata,
                   const bool compress,
                   const bool integrateCMAQLayers,
                   const DataFilterParameters& dataFilterParameters,
                   const int aggregateMethod,
                   const int regridMethod,
                   const int regridAggregateMethod,
                   const CMAQGridParameters& cmaqGridParameters );

  QueryParameters( const QueryParameters& other );
  QueryParameters& operator=( const QueryParameters& other );

  void timestamps( const Timestamp startingTimestamp,
                   const Timestamp endingTimestamp );

  void bounds( const Bounds& bounds );
  void boundsZ( const double boundsZ[ 2 ] );
  void variables( const std::string& variables );
  void format( const int format );
  void directory( const std::string& directory );
  void getMetadata( const bool getMetadata );
  void compress( const bool compress );
  void integrateCMAQLayers( const bool integrate );
  void dataFilterParameters( const DataFilterParameters& dataFilterParameters);
  void aggregateMethod( const int aggregateMethod );
  void regridMethod( const int regridMethod );
  void regridAggregateMethod( const int regridAggregateMethod );
  void cmaqGridParameters( const CMAQGridParameters& cmaqGridParameters );

  // Queries:

  bool operator==( const QueryParameters& other ) const;
  bool operator!=( const QueryParameters& other ) const;
  bool invariant() const;
  Timestamp startingTimestamp() const;
  Timestamp endingTimestamp() const;
  const Bounds& bounds() const;
  double boundsZMinimum() const;
  double boundsZMaximum() const;
  const std::string& variables() const;
  int format() const;
  const std::string& directory() const;
  bool getMetadata() const;
  bool compress() const;
  bool integrateCMAQLayers() const;
  const DataFilterParameters& dataFilterParameters() const;
  int aggregateMethod() const;
  int regridMethod() const;
  int regridAggregateMethod() const;
  const CMAQGridParameters& cmaqGridParameters() const;
  std::string pathedFileName() const;
};


} // namespace emvl.

#endif /* QUERYPARAMETERS_H */


