/******************************************************************************
PURPOSE: SiteDataset.C - Dataset for site (ground station) data.
HISTORY: 2013-02-20 plessel.todd@epa.gov
******************************************************************************/

//================================ INCLUDES ===================================

#include <stdio.h>  // For FILE, fgets(), fread(), fscanf().
#include <string.h> // For memset(), strstr().
#include <math.h>   // For sqrt().
#include <float.h>  // For FLT_MAX.
#include <unistd.h> // For unlink().

#include <stdexcept> // For std::runtime_error.
#include <string>    // For std::string.
#include <vector>    // For std::vector.
#include <memory>    // For std::auto_ptr.

#include <Assertions.h> // For PRE(), POST(), AND(), IN_RANGE().
#include <KMLFile.h>    // For write*KML().

#include <DataUtilities.h>      // For isNan(), isLongitude(), clampedToRange()
#include <ResourceHandler.h>    // For ResourceHandler.
#include <Bounds.h>             // For Bounds.
#include <Timestamp.h>          // For Timestamp.
#include <QueryParameters.h>    // For QueryParameters.
#include <CMAQGridParameters.h> // For CMAQGridParameters.
#include <NetCDFUtilities.h>    // For *NetCDF*() routines.
#include <SiteDataset.h>        // For SiteDataset.

enum { NOTE_LENGTH = 80 };

namespace emvl {

//============================= PUBLIC FUNCTIONS ==============================



/******************************************************************************
PURPOSE: ~SiteDataset - Destruct a SiteDataset.
******************************************************************************/

SiteDataset::~SiteDataset() {
  PRE( true );
}



/******************************************************************************
PURPOSE: SiteDataset - Construct a SiteDataset from reading a stream.
INPUTS:  const FILE* stream  Stream to read from.
NOTES:   Throws exception upon failure to read/store valid site data.
******************************************************************************/

SiteDataset::SiteDataset( FILE* stream ) {
  PRE0( stream );

  variables_        = 0;
  dataVariable_     = 0;
  timesteps_        = 0;
  cellType_         = POINT;
  hasElevation_     = false;
  hasArea_          = false;
  hasVolume_        = false;
  isStationary_     = true;
  isGroup_          = false;
  isStructuredGrid_ = false;
  isVector_         = false;
  stations_         = 0;
  readXDRHeader( stream ); //x
  readXDRData( stream ); //x

  if ( ! invariant() ) {
    throw std::runtime_error( "Failed to read valid site data." ); //x
  }

  POST( true );
}



/******************************************************************************
PURPOSE: SiteDataset - Construct a SiteDataset from reading a file.
INPUTS:  const std::string& fileName  Name of file to read from.
NOTES:   Throws exception upon failure to read/store valid site data.
******************************************************************************/

SiteDataset::SiteDataset( const std::string& fileName ) {
  PRE0( fileName.length() );

  fileName_         = fileName;
  variables_        = 0;
  dataVariable_     = 0;
  timesteps_        = 0;
  cellType_         = POINT;
  hasElevation_     = false;
  hasArea_          = false;
  hasVolume_        = false;
  isStationary_     = true;
  isGroup_          = false;
  isStructuredGrid_ = false;
  isVector_         = false;
  stations_         = 0;
  ResourceHandler<FILE*> fileCloser( fopen( fileName.c_str(), "rb" ), fclose );
  FILE* const file = fileCloser.get();

  if ( file ) {
    readXDRHeader( file ); //x    
    readXDRData( file ); //x    
  }

  if ( ! invariant() ) {
    throw std::runtime_error( "Failed to read valid site data "
                              "from file " + fileName + '.' ); //x
  }

  POST( true );
}



/******************************************************************************
PURPOSE: SiteDataset - Copy-construct a SiteDataset.
INPUTS:  const SiteDataset& other  Object to copy.
******************************************************************************/

SiteDataset::SiteDataset( const SiteDataset& other ) {
  PRE0( other.invariant() );

  if ( this != &other ) {
    copy( other ); // Copy base class attributes.
    stations_ = other.stations_;
    notes_ = other.notes_;
    idNotes_ = other.idNotes_;
  }

  POST( *this == other );
}



/******************************************************************************
PURPOSE: operator= - Assign a SiteDataset.
INPUTS:  const SiteDataset& other  Object to copy.
RETURNS: SiteDataset& *this.
******************************************************************************/

SiteDataset& SiteDataset::operator=( const SiteDataset& other ) {
  PRE( other.invariant() );

  if ( this != &other ) {
    copy( other ); // Copy base class attributes.
    stations_ = other.stations_;
    notes_ = other.notes_;
    idNotes_ = other.idNotes_;
  }

  POST( *this == other );
  return *this;
}



/******************************************************************************
PURPOSE: sample - Sample other dataset at this dataset's coordinates.
INPUTS:  const Dataset& other  Dataset to sample.
RETURNS: Dataset* sampled dataset. Deallocate when no longer needed.
NOTES:   Throws exception upon failure.
******************************************************************************/

Dataset* SiteDataset::sample( const Dataset& other ) const {
  PRE( other.invariant() );
  Dataset* result = 0;
  throw std::runtime_error( "UNIMPLEMENTED: SiteDataset::sample()" ); //x
  POST( IMPLIES( result, result->invariant() ) );
  return result;
}



/******************************************************************************
PURPOSE: regrid - Regrid this dataset's data onto given grid.
INPUTS:  const int regridMethod  REGRID_NEAREST...REGRID_WEIGHTED.
         const CMAQGridParameters& cmaqGridParameters  Grid parameters.
RETURNS: Dataset* regridded dataset. Deallocate when no longer needed.
NOTES:   Throws exception upon failure.
******************************************************************************/

Dataset* SiteDataset::regrid( const int regridMethod,
                              const CMAQGridParameters&
                                cmaqGridParameters ) const {
  PRE3( IN_RANGE( regridMethod, 0, QueryParameters::REGRID_METHODS - 1 ),
        regridMethod != QueryParameters::REGRID_NONE,
        cmaqGridParameters.invariant() );
  Dataset* result = 0;
  throw std::runtime_error( "UNIMPLEMENTED: SiteDataset::regrid()" ); //x
  POST( IMPLIES( result, result->invariant() ) );
  return result;
}



/******************************************************************************
PURPOSE: write - Write dataset to a directory file in the specified format.
INPUTS:  const std::string& directory  Name of directory to write to.
         const int format              Format of file to write.
NOTES:   The name of the file is determined by the dataset name, variable, etc.
         Throws exception upon failure.
******************************************************************************/

void SiteDataset::write( const std::string& directory,
                         const int format ) const {
  PRE2( directory.length(),
        IN_RANGE( format, 0, QueryParameters::FILE_FORMATS - 1 ) );

  const std::string fileName( pathedFileName( directory, format ) ); //x
  DEBUG( fprintf( stderr, "Writing file '%s'...\n", fileName.c_str() ); )

  if ( IN3( format, QueryParameters::XDR, QueryParameters::ASCII ) ) {
    const char* const name = fileName.c_str();
    ResourceHandler<FILE*> fileCloser( fopen( name, "wb"), fclose);
    FILE* const file = fileCloser.get();
    fileCloser.handler2( name, unlink ); // Remove file upon failure.

    if ( ! file ) {
      throw std::runtime_error( "Failed to open writable file." ); //x
    } else if ( format == QueryParameters::XDR ) {
      writeXDR( file ); //x
    } else if ( format == QueryParameters::ASCII ) {
      writeASCII( file ); //x
    }

    fileCloser.handler2( 0, 0 ); // Successful so don't remove file.
  } else if ( format == QueryParameters::NETCDF_COARDS ) {
    writeCOARDS( fileName ); //x
  } else if ( format == QueryParameters::KML ) {
    writeKML(); //x
  }

  POST( true );
}



/******************************************************************************
PURPOSE: invariant - Class invariant.
RETURNS: bool true if valid, else false and the code is defective.
******************************************************************************/

bool SiteDataset::invariant() const {
  const bool result =
    AND18( Dataset::invariant(),
           variables_        == 2 + isVector_,
           cellType_         == POINT,
           hasElevation_     == false,
           hasArea_          == false,
           hasVolume_        == false,
           isStationary_     == true,
           isGroup_          == false,
           isStructuredGrid_ == false,
           stations_         > 0,
           coordinates_.size() == stations_,
           data_.size() == variables_ * timesteps_ * stations_,
           notes_.size() == stations_,
           notes_[ 0 ][ 0 ],
           notes_[ notes_.size() - 1 ][ 0 ],
           idNotes_.size() == stations_,
           idNotes_[ 0 ][ 0 ],
           idNotes_[ idNotes_.size() - 1 ][ 0 ] );
  return result;
}



/******************************************************************************
PURPOSE: operator== - Are object equal?
RETURNS: bool true if equal, else false.
******************************************************************************/

bool SiteDataset::operator==( const SiteDataset& other ) const {
  PRE( other.invariant() );
  const bool result =
    OR2( this == &other,
         AND4( equal( other ),
               stations_ == other.stations_,
               notes_ == other.notes_,
               idNotes_ == other.idNotes_ ) );
  return result;
}



/******************************************************************************
PURPOSE: operator!= - Are object unequal?
RETURNS: bool true if equal, else false.
******************************************************************************/

bool SiteDataset::operator!=( const SiteDataset& other ) const {
  PRE( other.invariant() );
  const bool result = ! ( *this == other );
  return result;
}



/******************************************************************************
PURPOSE: stations - Number of stations in dataset.
RETURNS: size_t number of stations in dataset.
******************************************************************************/

size_t SiteDataset::stations() const {
  PRE( true );
  const size_t result = stations_;
  POST( result );
  return result;
}



/******************************************************************************
PURPOSE: note - Note of a profile.
INPUTS:  const size_t station  Index of station.
RETURNS: const std::string& note of a station.
******************************************************************************/

const std::string& SiteDataset::note( const size_t station ) const {
  PRE( station < notes_.size() );
  const std::string& result = notes_[ station ];
  POST( result.length() );
  return result;
}



//============================= PRIVATE FUNCTIONS =============================



/******************************************************************************
PURPOSE: findNearestStation - Find index of station nearest a probe location.
INPUTS:  const double longitude  Longitude of probe.
         const double latitude   Latitude of probe.
OUTPUTS: size_t& nearestStation  Index of nearest station to probe.
RETURNS: bool true if found, else false.
******************************************************************************/

bool SiteDataset::findNearestStation( const double longitude,
                                      const double latitude,
                                      size_t& nearestStation ) const {

  PRE2( isLongitude( longitude ), isLatitude( latitude ) );

  bool result = false;
  const double nearEnough = 0.1; // Probe close enough to data point.
  nearestStation = 0;

  if ( isInBounds( longitude, latitude, nearEnough ) ) {
    const Point* const points = &coordinates_[ 0 ];
    double distance = 1.0; // Use Manhatten distance for speed.
    nearestStation = 0;

    for ( size_t station = 0; station < stations_; ++station ) {
      const Point* const stationPoint = points + station;
      const double stationLongitude = stationPoint->longitude;
      double longitudeDistance = 1.0;

      if ( IMPLIES_ELSE( stationLongitude <= longitude,
                          ( longitudeDistance = longitude - stationLongitude )
                            < distance,
                          ( longitudeDistance = stationLongitude - longitude )
                            < distance ) ) {

        const double stationLatitude = stationPoint->latitude;
        double latitudeDistance = 1.0;

        if ( IMPLIES_ELSE( stationLatitude <= latitude,
                           ( latitudeDistance = latitude - stationLatitude)
                              < distance,
                           ( latitudeDistance = stationLatitude - latitude )
                              < distance ) ) {
          nearestStation = station;
          distance = longitudeDistance + latitudeDistance;
        }
      }
    }

    result = distance <= nearEnough;

    if ( result ) {

      if ( nearestStation < idNotes_.size() ) {
        probedNote_ = idNotes_[ nearestStation ];
      }
    }
  }

  POST( IMPLIES( result, nearestStation < stations_ ) );
  return result;
}

  

/******************************************************************************
PURPOSE: vprobe - Return nearest data point to time-space location.
INPUTS:  const Timestamp& timestamp  Timestamp of probe.
         const double longitude      Longitude of probe.
         const double latitude       Latitude of probe.
         const double                Elevation of probe (ignored since 2D).
         const int probeLayer        0-based index of variable or IMISSING.
         const int variable          0-based index of variable or IMISSING.
RETURNS: double Data value nearest probe or MISSING if not near any data.
NOTES:   Called by base class member function probe().
         Template Method Design Pattern.
******************************************************************************/

double SiteDataset::vprobe( const Timestamp timestamp,
                            const double longitude,
                            const double latitude,
                            const double elevation,
                            const int probeLayer,
                            const int variable ) const {
  PRE4( timestamp.invariant(),
        isLongitude( longitude ),
        isLatitude( latitude ),
        OR2( variable == IMISSING, IN_RANGE( variable, 0, variables() - 1 )));

  double result = MISSING;
  size_t nearestStation = 0;

  if ( findNearestStation( longitude, latitude, nearestStation ) ) {
    size_t timestep = 0;

    if ( isInTimeRange( timestamp, timestep ) ) { 
      const int probeVariable =
        variable != IMISSING ? variable
        : isVector_ ? variables_ - 2
        : dataVariable_;
      const size_t totalPoints = timesteps_ * stations_;
      const size_t dataIndex =
        probeVariable * totalPoints +
          timestep * stations_ + nearestStation;
        CHECK( dataIndex < data_.size() );
        result = data_[ dataIndex ];

      if ( AND2( variable == IMISSING, isVector_ ) ) { // vector magnitude:
        const size_t dataIndex2 = dataIndex + totalPoints;
        CHECK( dataIndex2 < data_.size() );
        const double component2 = data_[ dataIndex2 ];
        result *= result;
        result += component2 * component2;
        result = sqrt( result );
      }
    }
  }

  POST( OR2( result == MISSING,
             IN_RANGE( result, minimum( variable ), maximum( variable ) ) ) );
  return result;
}



/******************************************************************************
PURPOSE: vtimeseries - Probe dataset over time range at a specified location.
INPUTS:  const Timestamp& beginTimestamp  Beginning timestamp of sample.
         const Timestamp& endTimestamp    Ending timestamp of sample.
         const double longitude           Longitude of probe.
         const double latitude            Latitude of probe.
         const double elevation           Elevation (ignored).
         const int probeLayer             0-based index of variable or IMISSING.
OUTPUTS: std::vector<double>& result      Hourly values (or MISSING) over time
                                          range at location.
NOTES:   Called by base class member function timeseries().
         Template Method Design Pattern.
******************************************************************************/

void SiteDataset::vtimeseries( const Timestamp beginTimestamp,
                               const Timestamp endTimestamp,
                               const double longitude,
                               const double latitude,
                               const double elevation,
                               const int probeLayer,
                               std::vector<double>& result ) const {

  PRE4( beginTimestamp.invariant(),
        endTimestamp.invariant(),
        beginTimestamp <= endTimestamp,
        isLongitudeLatitude( longitude, latitude ) );

  size_t nearestStation = 0;

  if ( findNearestStation( longitude, latitude, nearestStation ) ) {
    size_t firstTimestep = 0;
    size_t subsetTimesteps = 0;
    size_t skippedHours = 0;

    if ( overlappedTimeRange( beginTimestamp, endTimestamp,
                              firstTimestep, subsetTimesteps, skippedHours )) {
      const size_t sampleHours = result.size();
      const int hoursPerTimestep =
        timestepSize_ == HOURS ? 1
        : timestepSize_ == DAYS ? 24
        : 0;
      const int probeVariable = isVector_ ? variables_ - 2 : dataVariable_;
      const size_t totalPoints = timesteps_ * stations_;
      size_t dataIndex =
        probeVariable * totalPoints + firstTimestep * stations_ +
        nearestStation;

      for ( size_t dataTimestep = 0, hour = skippedHours;
            dataTimestep < subsetTimesteps;
            ++dataTimestep, dataIndex += stations_ ) {
        CHECK( dataIndex < data_.size() );
        double value = data_[ dataIndex ];

        if ( isVector_ ) { // vector magnitude:
          const size_t dataIndex2 = dataIndex + totalPoints;
          CHECK( dataIndex2 < data_.size() );
          const double component2 = data_[ dataIndex2 ];
          value *= value;
          value += component2 * component2;
          value = sqrt( value );
        }

        for ( size_t counter = hoursPerTimestep;
             AND2( hour < sampleHours, counter );
              --counter, ++hour ) {
          CHECK( hour < result.size() );
          result[ hour ] = value;
        }
      }
    }
  }

  POST3( result.size() >=
          (size_t) ( endTimestamp.seconds() - beginTimestamp.seconds() ) / 3600,
        OR2( ::emvl::minimum( &result[ 0 ], result.size() ) == MISSING,
              IMPLIES( ! isVector(),
                       IN_RANGE( ::emvl::minimum( &result[ 0 ], result.size() ),
                                 minimum(), maximum() ) ) ),
         OR2( ::emvl::maximum( &result[ 0 ], result.size() ) == MISSING,
              IMPLIES( ! isVector(),
                       IN_RANGE( ::emvl::maximum( &result[ 0 ], result.size() ),
                                 ::emvl::minimum( &result[ 0 ], result.size() ),
                                 maximum() ) ) ) );
}



/******************************************************************************
PURPOSE: vsubset - Extract a reference to the subset of data for the given
         time-range for data with stationary (non-time-varying) coordinates.
INPUTS:  const Timestamp& beginTimestamp  Beginning timestamp of subset.
         const Timestamp& endTimestamp    End timestamp of subset.
         const int variable               Index of data variable.
RETURNS: std::vector<Dataset::SubsetPointers>  Array of 1 set of pointers to
                                          data/coordinate subset.
                                          or empty vector if there are no data
                                          within the specified time range.
NOTES:   Called by public member function subsetData().
         Template Method Design Pattern.
******************************************************************************/

std::vector<Dataset::SubsetPointers>
SiteDataset::vsubset( const Timestamp beginTimestamp,
                      const Timestamp endTimestamp,
                      const int variable ) const {

  PRE4( beginTimestamp.invariant(),
        endTimestamp.invariant(),
        beginTimestamp <= endTimestamp,
        OR2( variable == IMISSING, IN_RANGE( variable, 0, variables() - 1 )));

  std::vector<SubsetPointers> result;
  size_t firstTimestep = 0;
  size_t subsetTimesteps = 0;

  if ( overlappedTimeRange( beginTimestamp, endTimestamp,
                            firstTimestep, subsetTimesteps ) ) {

    // Get variable data:

    const size_t totalPoints = timesteps_ * stations_;
    const size_t subsetPointOffset = firstTimestep * stations_;
    const size_t subsetCount = subsetTimesteps * stations_;

    if ( OR2( ! isVector_, variable != IMISSING ) ) {
      const int variable0 = variable == IMISSING ? dataVariable_ : variable;
      const size_t firstIndex = variable0 * totalPoints + subsetPointOffset;
      CHECK3( firstIndex < data_.size(),
              IN_RANGE( subsetCount, 1, data_.size() ),
              IN_RANGE( firstIndex + subsetCount, 1, data_.size() ) );
      const Dataset::SubsetPointers subsetPointers = {
        coordinates_.size(),
        &coordinates_[ 0 ],
        subsetCount,
        &data_[ firstIndex ],
        stations_,
        &notes_[ 0 ]
      };
      result.push_back( subsetPointers ); //x
    } else { // Append each vector component variable:
      CHECK2( isVector_, variables_ >= 2 );

      for ( int component = variables_ - 2; component < variables_;
            ++component ) {
        const size_t firstIndex = component * totalPoints + subsetPointOffset;
        CHECK3( firstIndex < data_.size(),
                IN_RANGE( subsetCount, 1, data_.size() ),
                IN_RANGE( firstIndex + subsetCount, 1, data_.size() ) );
        const Dataset::SubsetPointers subsetPointers = {
          coordinates_.size(),
          &coordinates_[ 0 ],
          subsetCount,
          &data_[ firstIndex ],
          stations_,
          &notes_[ 0 ]
        };
        result.push_back( subsetPointers ); //x
      }
    }
  }

  POST( IMPLIES( result.size(),
                 AND13( result[ 0 ].dataCount,
                        result[ 0 ].coordinatesCount == coordinates_.size(),
                        result[ 0 ].coordinates == &coordinates_[ 0 ],
                        result[ 0 ].data,
                        isPoint( result[ 0 ].coordinates[ 0 ] ),
                        isPoint( result[ 0 ].coordinates[
                                   result[ 0 ].coordinatesCount - 1 ] ),
                        IMPLIES( AND2( variable != IMISSING,
                                       emvl::minimum( result[ 0 ].data,
                                                      result[ 0 ].dataCount )
                                         > MISSING ),
                                 IN_RANGE( emvl::minimum( result[ 0 ].data,
                                                          result[ 0].dataCount),
                                           minimum( variable ),
                                           maximum( variable ) ) ),
                        IMPLIES( AND2( variable != IMISSING,
                                       emvl::maximum( result[ 0 ].data,
                                                      result[ 0 ].dataCount )
                                         > MISSING ),
                                 IN_RANGE( emvl::maximum( result[ 0 ].data,
                                                          result[ 0].dataCount),
                                           minimum( variable ),
                                           maximum( variable ) ) ),
                        result[ 0 ].notesCount,
                        result[ 0 ].notes,
                        result[ result.size() - 1 ].notesCount,
                        result[ result.size() - 1 ].notes,
                        IMPLIES( AND2( isVector(), variable == IMISSING ),
                                 result.size() >= 2 ) ) ) );
  return result;
}



/******************************************************************************
PURPOSE: allDistinctNotes - Return a reference to an array of distinct notes.
RETURNS: const std::vector<std::string>& idNotes_;
******************************************************************************/

  const std::vector<std::string>& SiteDataset::allDistinctNotes() const {
  PRE( true );
  const std::vector<std::string>& result = idNotes_;
  POST( true );
  return result;
}



/******************************************************************************
PURPOSE: distinctNotePoint - Return a const pointer to the first point matching
         distictNote or 0 if none matched.
INPUTS:  const std::string& distinctNote  Distinct note to lookup.
RETURNS: const Point* if found match else 0.
******************************************************************************/

const Point*
SiteDataset::distinctNotePoint( const std::string& distinctNote ) const {
  PRE( true );
  const Point* result = 0;
  const size_t count = idNotes_.size();
  CHECK( idNotes_.size() == coordinates_.size() );

  for ( size_t index = 0; index < count; ++index ) {

    if ( distinctNote == idNotes_[ index ] ) {
      result = &coordinates_[ index ];
      index = count;
    }
  }

  POST( OR2( result == 0, isPoint( *result ) ) );
  return result;
}



/******************************************************************************
PURPOSE: readXDRHeader - Read XDR header from a stream.
INPUTS:  FILE* stream  Stream to read from.
OUTPUTS: name_
         description_
         variableNames_
         variableUnits_
         variables_
         dataVariable_
         timesteps_
         stations_
         startingTimestamp_
         endingTimestamp_
NOTES:   Throws exception upon failure to read valid header from a stream.
******************************************************************************/

void SiteDataset::readXDRHeader( FILE* stream ) {
  PRE0( stream );

  /* Read XDR header that looks like this:

    SITE 2.0
    http://airnow.gov/,reformat_airnow_obs,SiteSubset
    2005-08-26T00:00:00-0000
    # data dimensions: timesteps stations
    3 9
    # Variable names:
    PM25
    # Variable units:
    ug/m3
    # char notes[stations][80] and
    # MSB 64-bit integers ids[stations] and
    # IEEE-754 64-bit reals sites[stations][2=<longitude,latitude>] and
    # IEEE-754 64-bit reals data[timesteps][stations]:

  */

  readAndMatchLine( stream, "SITE 2.0\n" ); //x
  readDescription( stream ); //x
  readTimestamp( stream, false ); //x
  skipLine( stream ); //x
  size_t dimensions[ 2 ] = { 0, 0 };
  readDimensions( stream, 2, dimensions ); //x
  timesteps_ = dimensions[ 0 ];
  stations_  = dimensions[ 1 ];
  notes_.resize( stations_ ); //x

  // Prepend station variable:

  variableNames_.reserve( 2 ); //x
  variableUnits_.reserve( 2 ); //x
  variableNames_.push_back( "station" ); //x
  variableUnits_.push_back( "-" ); //x
  variables_ = 1;

  // Read variable and set timestepSize, vector and ending timestamp:

  readVariablesAndUnits( stream, 0 ); //x

  isVector_ =
    AND2( variables_ == 3,
          OR2( AND2( strcmp( variableNames_[ 1 ].c_str(), "windU" ) == 0,
                     strcmp( variableNames_[ 2 ].c_str(), "windV" ) == 0 ),
               AND2( strcmp( variableNames_[ 1 ].c_str(), "wind_u" ) == 0,
                     strcmp( variableNames_[ 2 ].c_str(), "wind_v" ) == 0 ) ) );

  setDatasetNameFromDescriptionAndVariable(); // in:isVector_,out:timestepSize_
  setEndingTimestamp(); // Reads timestepSize_.
  readAndMatchLine( stream,
                    "# char notes[stations][80] and\n" );//x
  readAndMatchLine( stream,
                    "# MSB 64-bit integers ids[stations] and\n" ); //x
  readAndMatchLine( stream, "# IEEE-754 64-bit reals "
                    "sites[stations][2=<longitude,latitude>] and\n" ); //x
  readAndMatchLine( stream, "# IEEE-754 64-bit reals "
                    "data[timesteps][stations]:\n" ); //x

  POST010( isValidName(),
           isValidDescription(),
           isValidVariableNamesAndUnits(),
           variables_ == 2 + isVector_,
           dataVariable_ == variables_ - 1,
           timesteps_ > 0,
           stations_ > 0,
           startingTimestamp_.invariant(),
           endingTimestamp_.invariant(),
           endingTimestamp_ > startingTimestamp_ );
}



/******************************************************************************
PURPOSE: readXDRData - Read XDR data from a stream.
INPUTS:  FILE* stream  Stream to read from.
OUTPUTS: coordinates_
         data_
NOTES:   Throws exception upon failure to read valid data from a stream.
******************************************************************************/

void SiteDataset::readXDRData( FILE* stream ) {
  POST011( stream,
           isValidName(),
           isValidDescription(),
           isValidVariableNamesAndUnits(),
           variables_ == 2 + isVector_,
           dataVariable_ == variables_ - 1,
           timesteps_ > 0,
           stations_ > 0,
           startingTimestamp_.invariant(),
           endingTimestamp_.invariant(),
           endingTimestamp_ > startingTimestamp_ );

  /* Data looks like this:

    # char notes[stations][80] and
    # MSB 64-bit integers ids[stations] and
    # IEEE-754 64-bit reals sites[stations][2=<longitude,latitude>] and
    # IEEE-754 64-bit reals data[timesteps][stations]:

  */

  readStrings( stream, NOTE_LENGTH, notes_ ); //x
  idNotes_.reserve( stations_ ); //x

  std::vector<long long> ids( stations_ ); //x

  if ( fread( &ids[ 0 ], sizeof ids[ 0 ], stations_, stream ) == stations_ ) {
    reverse8ByteWordsIfLittleEndian( &ids[ 0 ], stations_ );

    size_t count = stations_ * 2;
    std::vector<double> sites( count ); //x

    if ( fread( &sites[ 0 ], sizeof sites[ 0 ], count, stream ) == count ) {
      reverse8ByteWordsIfLittleEndian( &sites[ 0 ], count );

      // Create coordinates_ and bounds_:

      coordinates_.reserve( stations_ ); //x

      for ( size_t station = 0; station < stations_; ++station ) {
        const size_t index2 = station + station;
        CHECK( index2 < sites.size() );
        const double longitude = sites[ index2 ];
        const double latitude  = sites[ index2 + 1 ];
        const Point point = { longitude, latitude, 0.0 };
        coordinates_.push_back( point );
      }

      CHECK( coordinates_.size() == stations_ );
      computeBounds();

      // Read variable data:

      count = timesteps_ * stations_;
      std::vector<double> data( count ); //x

      if ( fread( &data[ 0 ], sizeof data[ 0 ], count, stream ) == count ) {
        reverse8ByteWordsIfLittleEndian( &data[ 0 ], count );
        updateMissingValues( &data[ 0 ], count );

        // Create data_:

        data_.reserve( variables_ * timesteps_ * stations_ ); //x
 
        // Append station IDs:

        for ( size_t timestep = 0; timestep < timesteps_; ++timestep ) {

          for ( size_t station = 0; station < stations_; ++station ) {
            CHECK( station < ids.size() );
            const long long stationId = ids[ station ];
            const double id = stationId;
            data_.push_back( id );

            if ( timestep == 0 ) {
              char idNote[ 128 ] = "";
              memset( idNote, 0, sizeof idNote );
              snprintf( idNote, sizeof idNote / sizeof *idNote,
                       "%0.0f %s", id, notes_[ station ].c_str() );
              idNotes_.push_back( std::string( idNote ) );
            }
          }
        }

        // Append data variable:

        for ( size_t timestep = 0; timestep < timesteps_; ++timestep ) {

          for ( size_t station = 0; station < stations_; ++station ) {
            const size_t index = timestep * stations_ + station;
            CHECK( index < data.size() );
            const double value = data[ index ];
            data_.push_back( value );
          }
        }

        if ( isVector_ ) {

          if ( fread( &data[ 0 ], sizeof data[ 0 ], count, stream ) == count ) {
            reverse8ByteWordsIfLittleEndian( &data[ 0 ], count );

            // Append 2nd component data variable:

            for ( size_t timestep = 0; timestep < timesteps_; ++timestep ) {

              for ( size_t station = 0; station < stations_; ++station ) {
                const size_t index = timestep * stations_ + station;
                CHECK( index < data.size() );
                const double value = data[ index ];
                data_.push_back( value );
              }
            }
          }
        }
      }
    }
  }

  if ( data_.size() == variables_ * timesteps_ * stations_ ) {
    computeVariableRanges(); //x
  }

  if ( ! AND3( isValidCoordinates(), isValidData(), isValidRange() ) ) {
    throw std::runtime_error( "Failed to read valid site data." ); //x
  }

  POST03( coordinates_.size() == stations_,
          data_.size() == variables_ * timesteps_ * stations_,
          isValidRange() );
}



/******************************************************************************
PURPOSE: writeXDR - Write XDR format to a stream.
INPUTS:  FILE* stream  Stream to write to.
OUTPUTS: FILE* stream  Updated stream.
NOTES:   Throws exception upon failure.
******************************************************************************/

void SiteDataset::writeXDR( FILE* stream ) const {
  PRE( stream );

  /* Write XDR header that looks like this:

    SITE 2.0
    http://airnow.gov/,reformat_airnow_obs,SiteSubset
    2005-08-26T00:00:00-0000
    # data dimensions: timesteps stations
    3 9
    # Variable names:
    PM25
    # Variable units:
    ug/m3
    # char notes[stations][80] and
    # MSB 64-bit integers ids[stations] and
    # IEEE-754 64-bit reals sites[stations][2=<longitude,latitude>] and
    # IEEE-754 64-bit reals data[timesteps][stations]:

  */

  Timestamp::UTCString timestamp = "";
  startingTimestamp_.toUTCString( timestamp );
  bool ok = false;

  if ( isVector_ ) {
    const char* const format =
      "SITE 2.0\n"
      "%s\n"
      "%s\n"
      "# data dimensions: timesteps stations\n"
      "%" LU " %" LU "\n"
      "# Variable names:\n"
      "%s %s\n"
      "# Variable units:\n"
      "%s %s\n"
      "# char notes[stations][80] and\n"
      "# MSB 64-bit integers ids[stations] and\n"
      "# IEEE-754 64-bit reals sites[stations][2=<longitude,latitude>] and\n"
      "# IEEE-754 64-bit reals data[timesteps][stations]:\n";
    ok = fprintf( stream, format,
                  description_.c_str(),
                  timestamp,
                  timesteps_, stations_,
                  variableNames_[ variables_ - 2 ].c_str(),
                  variableNames_[ variables_ - 1 ].c_str(),
                  variableUnits_[ variables_ - 2 ].c_str(),
                  variableUnits_[ variables_ - 1 ].c_str() )
         > (int) strlen( format );
  } else {
    const char* const format =
      "SITE 2.0\n"
      "%s\n"
      "%s\n"
      "# data dimensions: timesteps stations\n"
      "%" LU " %" LU "\n"
      "# Variable names:\n"
      "%s\n"
      "# Variable units:\n"
      "%s\n"
      "# char notes[stations][80] and\n"
      "# MSB 64-bit integers ids[stations] and\n"
      "# IEEE-754 64-bit reals sites[stations][2=<longitude,latitude>] and\n"
      "# IEEE-754 64-bit reals data[timesteps][stations]:\n";
    ok = fprintf( stream, format,
                  description_.c_str(),
                  timestamp,
                  timesteps_, stations_,
                  variableNames_[ dataVariable_ ].c_str(),
                  variableUnits_[ dataVariable_ ].c_str() )
         > (int) strlen( format );
  }

  if ( ok ) {
    writeStrings( stream, NOTE_LENGTH, notes_ ); //x
    const size_t count = stations_ * 2 + timesteps_ * stations_;
    std::vector<double> buffer( count ); //x
    long long* const ibuffer = (long long*) &buffer[ 0 ];
    double* const fbuffer = &buffer[ 0 ];

    // Write station ids:

    for ( size_t station = 0; station < stations_; ++station ) {
      CHECK2( station < count, station < data_.size() );
      const long long id = (long long) data_[ station ];
      ibuffer[ station ] = id;
    }

    reverse8ByteWordsIfLittleEndian( ibuffer, stations_ );
    ok = fwrite( ibuffer, sizeof *ibuffer, stations_, stream ) == stations_;

    if ( ok ) { // Write sites:

      for ( size_t station = 0; station < stations_; ++station ) {
        const size_t station2 = station + station;
        CHECK2( station2 < count - 1, station < coordinates_.size() );
        const double longitude = (double) coordinates_[ station ].longitude;
        const double latitude  = (double) coordinates_[ station ].latitude;
        fbuffer[ station2 ] = longitude;
        fbuffer[ station2 + 1 ] = latitude;
      }

      const size_t stations2 = stations_ + stations_;
      reverse8ByteWordsIfLittleEndian( fbuffer, stations2 );
      ok = fwrite( fbuffer, sizeof *fbuffer, stations2, stream ) == stations2;

      if ( ok ) { // Write data:
        const size_t totalPoints = timesteps_ * stations_;

        for ( int variable = 1; AND2( ok, variable < variables_ ); ++variable) {
          size_t dataIndex = variable * totalPoints;
          size_t bufferIndex = 0;

          for ( size_t timestep = 0; timestep < timesteps_; ++timestep ) {

            for ( size_t station = 0; station < stations_;
                  ++station, ++bufferIndex, ++dataIndex ) {
              CHECK2( bufferIndex < count, dataIndex < data_.size() );
              const double value = (double) data_[ dataIndex ];
              fbuffer[ bufferIndex ] = value;
            }
          }

          reverse8ByteWordsIfLittleEndian( fbuffer, totalPoints );
          ok = fwrite( fbuffer, sizeof *fbuffer, totalPoints, stream )
               == totalPoints;
        }
      }
    }
  }

  if ( ! ok ) {
    throw std::runtime_error( "Failed to write site data." ); //x
  }

  POST( true );
}



/******************************************************************************
PURPOSE: writeASCII - Write ASCII format to a stream.
INPUTS:  FILE* stream  Stream to write to.
OUTPUTS: FILE* stream  Updated stream.
NOTES:   Throws exception upon failure.
******************************************************************************/

void SiteDataset::writeASCII( FILE* stream ) const {
  PRE( stream );

  /* Write ASCII format that looks like this (with tabs as delimiters):

   Timestamp(UTC)  LONGITUDE(deg)  LATITUDE(deg)   STATION(-)      PM25(ug/m3)
   2005-08-26T00:00:00-0000    -86.8100    33.5500    010730023    38.0000
   ...

  */

  bool ok = false;

  if ( isVector_ ) {
    const char* const header =
      "Timestamp(UTC)\tLONGITUDE(deg)\tLATITUDE(deg)\tSTATION(-)\t%s(%s)\t%s(%s)\tNOTE\n";
    ok = fprintf( stream, header,
                  variableNames_[ variables_ - 2 ].c_str(),
                  variableUnits_[ variables_ - 2 ].c_str(),
                  variableNames_[ variables_ - 1 ].c_str(),
                  variableUnits_[ variables_ - 1 ].c_str() )
         >= (int) strlen( header ) - 12;

  } else {
    const char* const header =
      "Timestamp(UTC)\tLONGITUDE(deg)\tLATITUDE(deg)\tSTATION(-)\t%s(%s)\tNOTE\n";
    ok = fprintf( stream, header,
                  variableNames_[ dataVariable_ ].c_str(),
                  variableUnits_[ dataVariable_ ].c_str() )
         >= (int) strlen( header ) - 9;
  }


  if ( ok ) {
    const char* const dataFormat1 =
      "%s\t%11.5f\t%11.5f\t%20" LLD "\t%20.12f\t%46s\n";
    const char* const dataFormat2 =
      "%s\t%11.5f\t%11.5f\t%20" LLD "\t%20.12lf\t%20.12f\t%46s\n";
    const size_t totalPoints = timesteps_ * stations_;
    size_t dataIndex = totalPoints;
    size_t dataIndex2 = totalPoints + totalPoints;
    Timestamp timestamp( startingTimestamp_ );
    const int hoursPerTimestep =
      timestepSize_ == HOURS ? 1
      : timestepSize_ == DAYS ? 24
      : 0;
    const int monthsPerTimestep =
      timestepSize_ == MONTHS ? 1
      : timestepSize_ == TRIMONTHS ? 3
      : timestepSize_ == YEARS ? 12
      : 0;

    for ( size_t timestep = 0; timestep < timesteps_; ++timestep ) {
      Timestamp::UTCString timestampString = "";
      timestamp.toUTCString( timestampString );

      for ( size_t station = 0; station < stations_;
            ++station, ++dataIndex, ++dataIndex2 ) {
        CHECK3( station < data_.size(),
                station < coordinates_.size(),
                dataIndex < data_.size() );
        const long long id = (long long) data_[ station ];
        const double longitude = coordinates_[ station ].longitude;
        const double latitude  = coordinates_[ station ].latitude;
        const double measure   = data_[ dataIndex ];

        if ( isVector_ ) {
          const double measure2 = data_[ dataIndex2 ];

          if ( AND2( measure > MISSING, measure2 > MISSING ) ) {
            ok = fprintf( stream, dataFormat2,
                          timestampString, longitude, latitude, id, measure,
                          measure2, notes_[ station ].c_str() ) > 0;
          }
        } else {

          if ( measure > MISSING ) {
            ok = fprintf( stream, dataFormat1,
                          timestampString, longitude, latitude, id, measure,
                          notes_[ station ].c_str() ) > 0;
          }
        }

        if ( ! ok ) {
          station = stations_;
          timestep = timesteps_;
        }
      }

      if ( hoursPerTimestep ) {
        timestamp.offsetHours( hoursPerTimestep );
      } else {
        timestamp.offsetMonths( monthsPerTimestep );
      }
    }
  }

  if ( ! ok ) {
    throw std::runtime_error( "Failed to write site data." ); //x
  }

  POST( true );
}



/******************************************************************************
PURPOSE: writeCOARDS - Write COARDS-format data.
INPUTS:  const std::string& fileName  File name to create.
NOTES:   Throws exception upon failure.
******************************************************************************/

void SiteDataset::writeCOARDS( const std::string& fileName ) const {

  PRE( fileName.length() );

  const size_t fileSizeEstimate =
    stations_ * 2 * 4 + // longitude( station ), latitude( station ).
    ( 1 + isVector_ ) * stations_ * timesteps_ * 4 + // variable(station, time)
    timesteps_ * 4 + // time( time ).
    1000; // header.
  const size_t TWO_GB = 1UL << 31;
  const int create64BitFile = fileSizeEstimate > TWO_GB;
  const char* const name = fileName.c_str();
  const int file = createNetCDFFile( name, create64BitFile );
  ResourceHandler<int> fileCloser( file, closeNetCDFFile );
  fileCloser.handler2( name, unlink ); // Remove upon failure.
  bool ok = false;

  if ( file ) {
    enum { TIME, STATION, LENGTH, DIMENSIONS };
    const char* const names[ DIMENSIONS ] = { "time", "station", "length" };
    const size_t dimensions[ DIMENSIONS ] = {
      timesteps_, stations_, NOTE_LENGTH
    };
    int dimensionIds[ DIMENSIONS ] = { -1, -1, -1 };

    if ( createNetCDFDimensions( file, DIMENSIONS, names, dimensions,
                                 dimensionIds ) ) {

      if ( createNetCDFVariable( file, "station_id", "-",
                                 NC_DOUBLE, 0, 1, dimensionIds + 1 ) != -1 ) {


        if ( createNetCDFVariable( file, "notes", "-", NC_CHAR, 0, 2,
                                   dimensionIds + 1 ) != -1 ) {

          // Create temporary copy of timestamps:

          std::vector<Timestamp> timestamps( timesteps_ ); //x
          Timestamp timestamp = startingTimestamp_;
          const int hoursPerTimestep =
            timestepSize_ == HOURS ? 1
            : timestepSize_ == DAYS ? 24
            : 0;
          const int monthsPerTimestep =
            timestepSize_ == MONTHS ? 1
            : timestepSize_ == TRIMONTHS ? 3
            : timestepSize_ == YEARS ? 12
            : 0;

          for ( size_t timestep = 0; timestep < timesteps_; ++timestep ) {
            CHECK( timestep < timestamps.size() );
            timestamps[ timestep ] = timestamp;

            if ( hoursPerTimestep ) {
              timestamp.offsetHours( hoursPerTimestep );
            } else {
              timestamp.offsetMonths( monthsPerTimestep );
            }
          }

          writeCOARDSCommon( file, 1, false, false, false, true, 1,
                             DIMENSIONS - 1, dimensionIds, dimensions, names,
                             timestamps ); //x

          // Copy and write station ids as 64-bit reals:

          std::vector<double> ids( stations_ ); //x

          for ( size_t index = 0; index < stations_; ++index ) {
            CHECK2( index < data_.size(), index < stations_ );
            const long long id = (long long) data_[ index ];
            ids[ index ] = id;
          }

          ok = writeNetCDFAllDoubleData( file, "station_id", &stations_, &ids[1]);
          writeCOARDSNotes( file, NOTE_LENGTH, notes_ ); //x
        }
      }
    }
  }

  if ( ! ok ) {
    throw std::runtime_error("Failed to create NetCDF file for site data.");//x
  }

  fileCloser.handler2( 0, 0 ); // Successful so don't remove.
  POST( true );
}



/******************************************************************************
PURPOSE: writeKML - Write KML-format data.
NOTES:   Throws exception upon failure.
******************************************************************************/

void SiteDataset::writeKML() const {
  PRE( outputKMLFile_ );
  const char* const variable =
    isVector_ ? "Wind" : variableNames_[ dataVariable_ ].c_str();
  std::string source( name_ ); //x
  const size_t index = source.rfind( variable );

  if ( index != std::string::npos && index > 1 ) {
    source.erase( index - 1 );
  }

  const char* const units = variableUnits_[ dataVariable_ ].c_str();
  const char* const dataType = isVector_ ? "Vector" : "Point";
  const char* const description =
    isVector_ ? "Measured vector data." : "Measured point data.";
  DataColor dataColorFunction =
    outputKMLDataColorFunction_ ? outputKMLDataColorFunction_ : dataColor;

  writeStartFolderKML( outputKMLFile_, source.c_str(), variable, units,
                       dataType, description );

  const int components = isVector_ ? 2 : 1;
  const double variableMinimum = minimum();
  const double variableMaximum = maximum();

  const size_t totalPoints = timesteps_ * stations_;
  size_t dataIndex = totalPoints;
  size_t dataIndex2 = totalPoints + totalPoints;
  Timestamp timestamp( startingTimestamp_ );
  char expandedSource[ 256 ] = "";
  const int hoursPerTimestep =
    timestepSize_ == HOURS ? 1 : timestepSize_ == DAYS ? 24 : 0;
  const int monthsPerTimestep =
    timestepSize_ == MONTHS ? 1
    : timestepSize_ == TRIMONTHS ? 3
    : timestepSize_ == YEARS ? 12
    : 0;

  for ( size_t timestep = 0; timestep < timesteps_; ++timestep ) {
    const int yyyymmdd = timestamp.yyyymmdd();
    const int hhmmss = timestamp.hhmmss();
    const int valueTimestamp =
      timestepSize_ == HOURS ? yyyymmdd * 100 + hhmmss / 10000
      : timestepSize_ == DAYS ? yyyymmdd
      : timestepSize_ == MONTHS ? yyyymmdd / 100
      : timestepSize_ == TRIMONTHS ? yyyymmdd / 100
      : yyyymmdd / 10000;

    for ( size_t station = 0; station < stations_;
          ++station, ++dataIndex, ++dataIndex2 ) {
      CHECK3( station < data_.size(),
              station < coordinates_.size(),
              dataIndex < data_.size() );
      const double value = data_[ dataIndex ];

      if ( value > MISSING ) {
        const double value2 = isVector_ ? data_[ dataIndex2 ] : MISSING;

        if ( OR2( ! isVector_, value2 > MISSING ) ) {
          const double longitude = coordinates_[ station ].longitude;
          const double latitude  = coordinates_[ station ].latitude;
          const double elevation = coordinates_[ station ].elevation;
          const double pointCoordinates[ 3 ] =
            { longitude, latitude, elevation };
          const double magnitude =
            isVector_ ? sqrt( value * value + value2 * value2 ) : value;
          const Color color =
            dataColorFunction( magnitude, variableMinimum, variableMaximum );
          const double id = data_[ station ];
          const char* const note = notes_[ station ].c_str();
          memset( expandedSource, 0, sizeof expandedSource );
          snprintf( expandedSource,
                    sizeof expandedSource / sizeof *expandedSource - 1,
                    "%s station %0.0f %s", source.c_str(), id, note );

          writeCellDataToKML( outputKMLFile_, expandedSource, variable, units,
                              valueTimestamp, components, MISSING,
                              value, value2, MISSING,
                              1, pointCoordinates, color );
        }
      }
    }

    if ( hoursPerTimestep ) {
      timestamp.offsetHours( hoursPerTimestep );
    } else {
      timestamp.offsetMonths( monthsPerTimestep );
    }
  }

  writeEndFolderKML( outputKMLFile_ );

  POST( true );
}



} // namespace emvl.



