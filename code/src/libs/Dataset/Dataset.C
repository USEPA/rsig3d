/******************************************************************************
PURPOSE: Dataset.C - Dataset Non-instantiable ADT ABC.
HISTORY: 2013-02-17 plessel.todd@epa.gov
******************************************************************************/

//================================ INCLUDES ===================================

#include <stdio.h>  // For FILE, fgets(), fscanf().
#include <stdlib.h> // For strtoul().
#include <string.h> // For memset().
#include <ctype.h>  // For isalnum(), isprint(), toupper().
#include <limits.h> // For ULONG_MAX.
#include <float.h>  // For FLT_MAX.
#include <math.h>   // For sqrt().

#include <string>    // For std::string.
#include <vector>    // For std::vector.
#include <stdexcept> // For std::runtime_error.
#include <memory>    // For std::auto_ptr.

#include <Assertions.h>      // For PRE(), POST(), AND(), IN_RANGE().
#include <Utilities.h>       // For daysInMonth().
#include <DataUtilities.h>   // For MISSING, isNan(), minimum().
#include <Bounds.h>          // For Bounds.
#include <Timestamp.h>       // For Timestamp.
#include <MapProjection.h>   // For MapProjection.
#include <Lambert.h>         // For Lambert.
#include <Albers.h>          // For Albers.
#include <Mercator.h>        // For Mercator.
#include <Stereographic.h>   // For Stereographic.
#include <QueryParameters.h> // For QueryParameters::MINIMUM_ELEVATION.
#include <NetCDFUtilities.h> // For writeNetCDFAll*Data().
#include <Dataset.h>         // For Dataset.

assert_static( sizeof (emvl::Point) == 24 ); // Point must be a compact POD.

namespace emvl {

//============================= PUBLIC FUNCTIONS ==============================

bool operator==( const Point& a, const Point& b ) {
  return a.longitude == b.longitude &&
         a.latitude  == b.latitude &&
         a.elevation == b.elevation;
}

bool operator!=( const Point& a, const Point& b ) { return ! ( a == b ); }

bool isPoint( const Point& point ) {
  return AND3( isLongitude( point.longitude ),
               isLatitude(  point.latitude ),
               isElevation( point.elevation ) );
}

//=========================== PROTECTED FUNCTIONS =============================



/******************************************************************************
PURPOSE: Dataset - Construct a Dataset.
******************************************************************************/

Dataset::Dataset() {
  outputKMLFile_     = 0;
  outputKMLDataColorFunction_ = 0;
  variables_         = 0;
  dataVariable_      = 0;
  timesteps_         = 0;
  dataFileOffset_    = 0;
  firstDataTimestep_ = 0;
  lastDataTimestep_  = 0;
  cellType_          = 0;
  hasElevation_      = false;
  hasArea_           = false;
  hasVolume_         = false;
  isStationary_      = false;
  isGroup_           = false;
  isStructuredGrid_  = false;
  isVector_          = false;
  timestepSize_      = HOURS;
}



/******************************************************************************
PURPOSE: ~Dataset - Destruct a Dataset.
******************************************************************************/

Dataset::~Dataset() {
  outputKMLFile_     = 0;
  outputKMLDataColorFunction_ = 0;
  variables_         = 0;
  dataVariable_      = 0;
  timesteps_         = 0;
  dataFileOffset_    = 0;
  firstDataTimestep_ = 0;
  lastDataTimestep_  = 0;
  cellType_          = 0;
  hasElevation_      = false;
  hasArea_           = false;
  hasVolume_         = false;
  isStationary_      = false;
  isGroup_           = false;
  isStructuredGrid_  = false;
  isVector_          = false;
  timestepSize_      = HOURS;
}



/******************************************************************************
PURPOSE: dataVariable - Select variable to be used as data.
RETURNS: const int variable  0-based index of variable to be used as data.
******************************************************************************/

void Dataset::dataVariable( const int variable ) {
  PRE( IN_RANGE( variable, 0, variables() - 1 ) );
  dataVariable_ = variable;
  POST( this->dataVariable() == variable );
}



/******************************************************************************
PURPOSE: name - Set name of dataset.
INPUTS:  const std::string& name  Name of dataset.
******************************************************************************/

void Dataset::name( const std::string& name ) {
  PRE( name.length() );
  name_ = name;
  POST( this->name() == name );
}



/******************************************************************************
PURPOSE: write - Write regridded dataset to a directory file in the specified
         format.
INPUTS:  const std::string& directory  Name of directory to write to.
         const int format              Format of file to write.
         const int regridMethod        REGRID_NEAREST...REGRID_WEIGHTED.
         const CMAQGridParameters&     Regridding parameters.
NOTES:   The name of the file is determined by the dataset name, variable, etc.
         Calls virtual members regrid() and write(). Template Method Pattern.
         Throws exception upon failure.
******************************************************************************/

void Dataset::write( const std::string& directory,
                     const int format,
                     const int regridMethod,
                     const CMAQGridParameters& cmaqGridParameters) const {
  PRE4( directory.length(),
        IN_RANGE( format, 0, QueryParameters::FILE_FORMATS - 1 ),
        IN_RANGE( regridMethod, 0, QueryParameters::REGRID_METHODS - 1 ),
        cmaqGridParameters.invariant() );

  if ( regridMethod == QueryParameters::REGRID_NONE ) {
    write( directory, format ); //x Non-regridded write.
  } else { // Regrid data first:
    std::auto_ptr<Dataset> deleter(regrid(regridMethod,cmaqGridParameters));//x
    const Dataset* const dataset = deleter.get();
    CHECK2( dataset, dataset->invariant() );
    dataset->write( directory, format ); //x
  }

  POST( true );
}



/******************************************************************************
PURPOSE: invariant - Class invariant.
RETURNS: bool true if valid, else false and the code is defective.
******************************************************************************/

bool Dataset::invariant() const {
  const bool result =
    AND23( name_.length() > 0,
           description_.length() > 0,
           bounds_.invariant(),
           startingTimestamp_.invariant(),
           endingTimestamp_.invariant(),
           startingTimestamp_ <= endingTimestamp_,
           timesteps_ > 0,
           IMPLIES_ELSE( dataFileOffset_,
                         AND3( ! fileName_.empty(),
                               firstDataTimestep_ <= lastDataTimestep_,
                               IMPLIES( isStructuredGrid_,
                                 AND4(lastDataTimestep_ < timesteps_,
                                      timesteps_ > BUFFERED_DATA_TIMESTEPS,
                                      timesteps_ % BUFFERED_DATA_TIMESTEPS == 0,
                                      data_.size() % BUFFERED_DATA_TIMESTEPS
                                        == 0 ) ) ),
                        IS_ZERO2( firstDataTimestep_, lastDataTimestep_ ) ),
           variables_ > 0,
           IMPLIES( isVector_, variables_ > 1 ),
           IN_RANGE( dataVariable_, 0, variables_ - 1 ),
           IN_RANGE( cellType_, 0, CELL_TYPES - 1 ),
           variableNames_.size() == (size_t) variables_,
           variableUnits_.size() == (size_t) variables_,
           minimums_.size() == (size_t) variables_,
           maximums_.size() == (size_t) variables_,
           data_.size() > 0,
           isValidName(),
           isValidDescription(),
           isValidVariableNamesAndUnits(),
           isValidCoordinates(),
           isValidData(),
           isValidRange() );
  return result;
}



/******************************************************************************
PURPOSE: fileName - Name of file this dataset was read from or else empty if
         it was not read from a file (e.g., from a socket instead).
RETURNS: const std::string&  Name of file this dataset was read from or empty.
******************************************************************************/

const std::string& Dataset::fileName() const {
  PRE( true );
  const std::string& result = fileName_;
  POST( true );
  return result;
}



/******************************************************************************
PURPOSE: name - Name of this dataset.
RETURNS: const std::string&  Name of this dataset.
******************************************************************************/

const std::string& Dataset::name() const {
  PRE( true );
  const std::string& result = name_;
  POST( result.length() > 0 );
  return result;
}



/******************************************************************************
PURPOSE: description - Description (e.g., metadata link) of this dataset.
RETURNS: const std::string&  Description of this dataset.
******************************************************************************/

const std::string& Dataset::description() const {
  PRE( true );
  const std::string& result = description_;
  POST( result.length() > 0 );
  return result;
}



/******************************************************************************
PURPOSE: bounds - Longitude-latitude extent of this dataset.
RETURNS: const Bounds&  Bounds of this dataset.
******************************************************************************/

const Bounds& Dataset::bounds() const {
  PRE( true );
  const Bounds& result = bounds_;
  POST( result.invariant() );
  return result;
}



/******************************************************************************
PURPOSE: startingTimestamp - Starting timestamp from retrieval query.
RETURNS: Timestamp  Starting timestamp of this dataset.
******************************************************************************/

Timestamp Dataset::startingTimestamp() const {
  PRE( true );
  const Timestamp result = startingTimestamp_;
  POST( result.invariant() );
  return result;
}



/******************************************************************************
PURPOSE: endingTimestamp - Ending timestamp from retrieval query.
RETURNS: Timestamp  Ending timestamp of this dataset.
******************************************************************************/

Timestamp Dataset::endingTimestamp() const {
  PRE( true );
  const Timestamp result = endingTimestamp_;
  POST( result.invariant() );
  return result;
}



/******************************************************************************
PURPOSE: variables - Number of data variables (excluding coordinates).
RETURNS: int Number of variables.
******************************************************************************/

int Dataset::variables() const {
  PRE( true );
  const int result = variables_;
  POST( result > 0 );
  return result;
}



/******************************************************************************
PURPOSE: dataVariable - Index of variable to be used as data.
RETURNS: int Index of data variable.
******************************************************************************/

int Dataset::dataVariable() const {
  PRE( true );
  const int result = dataVariable_;
  POST( IN_RANGE( result, 0, variables() - 1 ) );
  return result;
}



/******************************************************************************
PURPOSE: timesteps - Number of timesteps of data.
RETURNS: size_t number of timesteps of data.
******************************************************************************/

size_t Dataset::timesteps() const {
  const size_t result = timesteps_;
  return result;
}



/******************************************************************************
PURPOSE: cellType - Type of coordinate cell.
RETURNS: int type of coordinate cell.
******************************************************************************/

int Dataset::cellType() const {
  const int result = cellType_;
  return result;
}



/******************************************************************************
PURPOSE: cellVertexCount - Number of vertices per coordinate cell.
RETURNS: int number of vertices per coordinate cell.
******************************************************************************/

int Dataset::cellVertexCount() const {
  const int result =
    cellType_ == HEXAHEDRON ? 8 : cellType_ == QUADRILATERAL ? 4 : 1;
  return result;
}



/******************************************************************************
PURPOSE: hasElevation - Does this dataset have elevation coordinates?
RETURNS: bool true if this dataset has elevation coordinates, else false.
******************************************************************************/

bool Dataset::hasElevation() const {
  const bool result = hasElevation_;
  return result;
}



/******************************************************************************
PURPOSE: hasArea - Does this dataset have area cells?
RETURNS: bool true if this dataset has area cells, else false.
******************************************************************************/

bool Dataset::hasArea() const {
  const bool result = hasArea_;
  return result;
}



/******************************************************************************
PURPOSE: hasVolume - Does this dataset have volume cells?
RETURNS: bool true if this dataset has volume cells, else false.
******************************************************************************/

bool Dataset::hasVolume() const {
  const bool result = hasVolume_;
  return result;
}



/******************************************************************************
PURPOSE: isStationary - Does this dataset have non-time-varying coordinates?
RETURNS: bool true if this dataset has fixed coordinates, else false.
******************************************************************************/

bool Dataset::isStationary() const {
  const bool result = isStationary_;
  return result;
}



/******************************************************************************
PURPOSE: isGroup - Does this dataset have disjoint sets of coordinates?
RETURNS: bool true if this dataset has groups of coordinates, else false.
******************************************************************************/

bool Dataset::isGroup() const {
  const bool result = isGroup_;
  return result;
}



/******************************************************************************
PURPOSE: isStructuredGrid - Does this dataset have a structued grid with one or
         more layers, rows and columns?
RETURNS: bool true if this dataset has a structued grid, else false.
******************************************************************************/

bool Dataset::isStructuredGrid() const {
  const bool result = isStructuredGrid_;
  return result;
}



/******************************************************************************
PURPOSE: isVector - Is this dataset variable a vector?
RETURNS: bool true if this dataset variable is a vector, else false.
******************************************************************************/

bool Dataset::isVector() const {
  const bool result = isVector_;
  return result;
}



/******************************************************************************
PURPOSE: isPagedFromDisk - True if this dataset data is paged from disk
         (per-timestep) else false means this dataset data is entirely in
         memory (therefore multiple timesteps can be accessed).
******************************************************************************/

bool Dataset::isPagedFromDisk() const {
  const bool result = dataFileOffset_ != 0;
  return result;
}



/******************************************************************************
PURPOSE: timestepSize - Size of data timesteps?
RETURNS: int HOURS, DAYS, MONTHS, TRIMONTHS, YEARS.
******************************************************************************/

int Dataset::timestepSize() const {
  const int result = timestepSize_;
  return result;
}



/******************************************************************************
PURPOSE: isSurfaceData - Is this dataset only on the Earth surface?
RETURNS: bool true if this dataset has data only on the Earth surface,
         else false if the data is in the atmosphere.
NOTES:   Default implementation of this virtual function returns layers() == 0.
******************************************************************************/

bool Dataset::isSurfaceData() const {
  const bool result = layers() == 0;
  return result;
}



/******************************************************************************
PURPOSE: layers - Number of layers in this structured grid.
RETURNS: size_t Number of layers in this structured grid.
NOTES:   Default implementation of this virtual function returns 0.
******************************************************************************/

size_t Dataset::layers() const {
  const size_t result = 0;
  return result;
}



/******************************************************************************
PURPOSE: rows - Number of rows in this structured grid.
RETURNS: size_t Number of rows in this structured grid.
NOTES:   Default implementation of this virtual function returns 0.
******************************************************************************/

size_t Dataset::rows() const {
  const size_t result = 0;
  return result;
}



/******************************************************************************
PURPOSE: columns - Number of columns in this structured grid.
RETURNS: size_t Number of columns in this structured grid.
NOTES:   Default implementation of this virtual function returns 0.
******************************************************************************/

size_t Dataset::columns() const {
  const size_t result = 0;
  return result;
}



/******************************************************************************
PURPOSE: variableName - Name of indexed variable.
INPUTS:  int variable  Index of variable to name.
RETURNS: const std::string& Name of variable.
******************************************************************************/

const std::string& Dataset::variableName( const int variable ) const {
  PRE( OR2( variable == IMISSING, IN_RANGE( variable, 0, variables() - 1 ) ) );
  const int v = variable == IMISSING ? dataVariable_ : variable;
  const std::string& result = variableNames_[ v ];
  POST( result.length() > 0 );
  return result;
}



/******************************************************************************
PURPOSE: variableUnits - Units of indexed variable.
INPUTS:  int variable  Index of variable.
RETURNS: const std::string& Units of variable.
******************************************************************************/

const std::string& Dataset::variableUnits( const int variable ) const {
  PRE( OR2( variable == IMISSING, IN_RANGE( variable, 0, variables() - 1 ) ) );
  const int v = variable == IMISSING ? dataVariable_ : variable;
  const std::string& result = variableUnits_[ v ];
  POST( result.length() > 0 );
  return result;
}



/******************************************************************************
PURPOSE: pathedFileName - Construct a pathed file name for this Dataset.
INPUTS:  const std::string& directory  Directory to write this Dataset.
         const int format              File format to write this Dataset.
RETURNS: const std::string  Pathed file name of this Dataset.
******************************************************************************/

std::string Dataset::pathedFileName( const std::string& directory,
                                     const int format ) const {
  PRE02( directory.length(),
         IN_RANGE( format, 0, QueryParameters::FILE_FORMATS - 1 ) );
  const char* const slash =
    directory[ directory.length() - 1 ] == '/' ? "" : "/";
  const std::string result( directory + slash + name_ +
                            QueryParameters::fileFormatExtensions[ format ] );
  POST0( result.length() > directory.length() + name_.length() );
  return result;
}



/******************************************************************************
PURPOSE: minimum - Minimum value of variable.
INPUTS:  int variable  Index of variable or IMISSING.
RETURNS: double minimum value of variable.
******************************************************************************/

double Dataset::minimum( const int variable ) const {
  PRE( OR2( variable == IMISSING, IN_RANGE( variable, 0, variables() - 1 ) ) );
  double result = 0.0;

  if ( OR2( ! isVector_, variable != IMISSING ) ) {
    const int variable0 = variable == IMISSING ? dataVariable_ : variable;
    result = minimums_[ variable0 ];
  }

  POST( ! isNan( result ) );
  return result;
}



/******************************************************************************
PURPOSE: maximum - Maximum value of variable.
INPUTS:  int variable  Index of variable or IMISSING.
RETURNS: double maximum value of variable.
******************************************************************************/

double Dataset::maximum( const int variable ) const {
  PRE( OR2( variable == IMISSING, IN_RANGE( variable, 0, variables() - 1 ) ) );
  double result = 0.0;

  if ( OR2( ! isVector_, variable != IMISSING ) ) {
    const int variable0 = variable == IMISSING ? dataVariable_ : variable;
    result = maximums_[ variable0 ];
  } else { // Compute magnitude of absolute maximum of variable ranges:
    const int index = // Note: only CMAQ has 3D vector <UWIND,VWIND,WWIND>.
      AND2( variables_ > 2,
            ! strcmp( variableNames_[ variables_ - 3 ].c_str(), "UWIND" ) ) ?
        variables_ - 3
      : variables_ - 2;

    for ( int component = index; component < variables_; ++component ) {
      CHECK3( component >= 0,
              (size_t) component < minimums_.size(),
              (size_t) component < maximums_.size() );
      const double theMinimum = minimums_[ component ];
      const double theMaximum = maximums_[ component ];
      const double absMinimum = theMinimum < 0.0 ? -theMinimum : theMinimum;
      const double absMaximum = theMaximum < 0.0 ? -theMaximum : theMaximum;
      const double absComponent =
        absMinimum > absMaximum ? absMinimum : absMaximum;
      result += absComponent * absComponent;
    }

    result = sqrt( result );
  }

  POST( ! isNan( result ) );
  return result;
}



/******************************************************************************
PURPOSE: pointInCell - Is point within a cell with given vertices?
INPUTS:  const double longitude   Longitude of point to check.
         const double latitude    Latitude  of point to check.
         const double elevation   Elevation of point to check or MISSING.
         const size_t count       Number of cell vertices.
         const Point  vertices[]  Array of cell vertices in order
                                  described in Dataset.h.
RETURNS: true if inside else false.
******************************************************************************/

bool Dataset::pointInCell( const double longitude, const double latitude,
                           const double elevation,
                           const size_t count, const Point vertices[] ) {

  PRE05( isLongitudeLatitude( longitude, latitude ),
         IN4( count, 3, 4, 8 ),
         vertices,
         isPoint( vertices[ 0 ] ),
         isPoint( vertices[ count - 1 ] ) );

  bool result = true;

  if ( AND2( count == 8, elevation != MISSING ) ) {
    result = IN_RANGE( elevation,
                       vertices[ 0 ].elevation,
                       vertices[ 4 ].elevation );
  }

  if ( result ) {
    const Point& pointSW = vertices[ 0 ];
    const Point& pointSE = vertices[ 1 ];
    const Point& pointNE = vertices[ 2 ];
    CHECK2( isPoint( vertices[ 1 ] ), isPoint( vertices[ 2 ] ) );

    result =
      pointInsideTriangle( longitude, latitude,
                           pointSW.longitude, pointSW.latitude,
                           pointSE.longitude, pointSE.latitude,
                           pointNE.longitude, pointNE.latitude );

    if ( AND2( ! result, count > 3 ) ) {
      const Point& pointNW = vertices[ 3 ];
      CHECK( isPoint( vertices[ 3 ] ) );
      result =
         pointInsideTriangle( longitude, latitude,
                              pointSW.longitude, pointSW.latitude,
                              pointNE.longitude, pointNE.latitude,
                              pointNW.longitude, pointNW.latitude );
    }
  }

  return result;
}



/******************************************************************************
PURPOSE: probe - Return nearest data point to time-space location.
INPUTS:  const Timestamp& timestamp  Timestamp of probe.
         const double longitude      Longitude of probe.
         const double latitude       Latitude of probe.
         const double elevation      Elevation of probe or MISSING.
         const int probeLayer        0-based index of layer or IMISSING.
         const int variable          0-based index of variable or IMISSING.
RETURNS: double Data value nearest probe or MISSING if not near any data.
NOTES:   Calls pure virtual member function vprobe() which must be implemented
         by concrete derived classes. Template Method Design Pattern.
******************************************************************************/

double Dataset::probe( const Timestamp timestamp,
                       const double longitude,
                       const double latitude,
                       const double elevation,
                       const int probeLayer,
                       const int variable ) const {
  PRE5( timestamp.invariant(),
        isLongitudeLatitude( longitude, latitude ),
        OR2( elevation == MISSING, isElevation( elevation ) ),
        OR2( probeLayer == IMISSING,
             AND2( probeLayer >= 0, (size_t) probeLayer < layers() ) ),
        OR2( variable == IMISSING, IN_RANGE( variable, 0, variables() - 1 ) ) );

  const bool loaded = loadedDataSubset( timestamp, timestamp );
  const int probeVariable =
    AND2( variable == IMISSING, ! isVector_ ) ? dataVariable_ : variable;
  const double result =
    ! loaded ? MISSING :
    vprobe( timestamp, longitude, latitude, elevation, probeLayer,
            probeVariable );

  POST( OR2( result == MISSING,
             IN_RANGE( result, minimum( variable ), maximum( variable ) ) ) );
  return result;
}



/******************************************************************************
PURPOSE: probedNote - Note of a last probed point.
RETURNS: const std::string& note of last probed point. Default is empty.
******************************************************************************/

const std::string& Dataset::probedNote() const {
  PRE( true );
  const std::string& result = probedNote_;
  return result;
}



/******************************************************************************
PURPOSE: timeseries - Probe dataset over time range at a specified location.
INPUTS:  const Timestamp& beginTimestamp  Beginning timestamp of sample.
         const Timestamp& endTimestamp    Ending timestamp of sample.
         const double longitude           Longitude of probe.
         const double latitude            Latitude of probe.
         const double elevation           Elevation of probe or MISSING.
         const int probeLayer             0-based index of layer or IMISSING.
OUTPUTS: std::vector<double>& result      Hourly values (or MISSING) over time
                                          range at location.
NOTES:   Calls non-pure virtual member function vtimeseries() which may be
         implemented by concrete derived classes. Template Method Pattern.
******************************************************************************/

void Dataset::timeseries( const Timestamp beginTimestamp,
                          const Timestamp endTimestamp,
                          const double longitude,
                          const double latitude,
                          const double elevation,
                          const int probeLayer,
                          std::vector<double>& result )
                          const {

  PRE6( beginTimestamp.invariant(),
        endTimestamp.invariant(),
        beginTimestamp <= endTimestamp,
        isLongitudeLatitude( longitude, latitude ),
        OR2( elevation == MISSING, isElevation( elevation ) ),
        OR2( probeLayer == IMISSING,
             AND2( probeLayer >= 0, (size_t) probeLayer < layers() ) ) );

  if ( IN3(timestepSize_, HOURS, DAYS ) ) {
    const size_t hours = 1 + beginTimestamp.hoursUntil( endTimestamp );
    result.assign( hours, MISSING );
  } else {
    const size_t months = 1 + beginTimestamp.monthsUntil( endTimestamp );
    result.assign( months, MISSING );
  }
                            
  vtimeseries( beginTimestamp, endTimestamp, longitude, latitude, elevation,
               probeLayer, result );

  POST3( IMPLIES_ELSE( IN3( timestepSize(), HOURS, DAYS ),
                       result.size() >=
                         (size_t) 1 + beginTimestamp.hoursUntil( endTimestamp ),
                       result.size() >=
                         (size_t) 1 + beginTimestamp.monthsUntil(endTimestamp)),
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
PURPOSE: subset - Extract a reference to the subset of data for the given
         time-range for data with time-varying coordinates.
INPUTS:  const Timestamp& beginTimestamp  Beginning timestamp of subset.
         const Timestamp& endTimestamp    End timestamp of subset.
         const int variable               Index of data variable.
RETURNS: std::vector<Dataset::SubsetPointers>
                                       Pointers to subset coordinates and data.
NOTES:   Calls virtual member function vsubset() which has a default
         implementation that returns an empty vector
         but may be overridden by concrete derived classes.
         Template Method Design Pattern.
******************************************************************************/

std::vector<Dataset::SubsetPointers>
Dataset::subset( const Timestamp beginTimestamp,
                 const Timestamp endTimestamp,
                 const int variable ) const {

  PRE4( beginTimestamp.invariant(),
        endTimestamp.invariant(),
        beginTimestamp <= endTimestamp,
        OR2( variable == IMISSING, IN_RANGE( variable, 0, variables() - 1 )));

  std::vector<Dataset::SubsetPointers> result;
  const bool loaded = loadedDataSubset( beginTimestamp, endTimestamp );

  if ( loaded ) {
    const int subsetVariable =
      AND2( variable == IMISSING, ! isVector_ ) ? dataVariable_ : variable;
    result = vsubset( beginTimestamp, endTimestamp, subsetVariable );
  }

  POST( IMPLIES( result.size(),
                 AND16( result[ 0 ].dataCount,
                        result[ 0 ].coordinatesCount,
                        IMPLIES( cellType_ != POLYGONS,
                                 result[0].coordinatesCount / cellVertexCount()
                          <= result[ 0 ].dataCount ),
                        result[ 0 ].data,
                        IMPLIES( result[ 0 ].coordinates,
                                 AND2( isPoint( result[ 0 ].coordinates[ 0 ] ),
                                       isPoint( result[ 0 ].coordinates[
                                         result[ 0 ].coordinatesCount - 1 ]))),
                        IMPLIES( AND2( variable != IMISSING,
                                       emvl::minimum( result[ 0 ].data,
                                                      result[ 0 ].dataCount )
                                       > MISSING ),
                                 IN_RANGE( emvl::minimum( result[0].data,
                                                          result[0].dataCount),
                                           minimum( variable ),
                                           maximum( variable ) ) ),
                        IMPLIES( AND2( variable != IMISSING,
                                       emvl::maximum( result[ 0 ].data,
                                                      result[ 0 ].dataCount )
                                       > MISSING ),
                                 IN_RANGE( emvl::maximum( result[0].data,
                                                          result[0].dataCount),
                                           minimum( variable ),
                                           maximum( variable ) ) ),
                        IMPLIES( result[ 0 ].notesCount, result[ 0 ].notes ),
                        result[ result.size() - 1 ].dataCount,
                        result[ result.size() - 1 ].coordinatesCount,
                        IMPLIES( cellType_ != POLYGONS,
                                 result[ result.size() - 1 ].coordinatesCount
                                   / cellVertexCount()
                                   <= result[ result.size() - 1 ].dataCount ),
                        result[ result.size() - 1 ].data,
                        IMPLIES( result[ result.size() - 1 ].coordinates,
                                 AND2( isPoint( result[ result.size() - 1 ]
                                                .coordinates[ 0 ] ),
                                       isPoint( result[ result.size() - 1 ]
                                                .coordinates[
                          result[ result.size() - 1 ].coordinatesCount - 1]))),
                        IMPLIES( AND2( variable != IMISSING,
                                       emvl::minimum(
                                         result[result.size() - 1].data,
                                         result[result.size() - 1].dataCount )
                                       > MISSING ),
                                 IN_RANGE( emvl::minimum(
                                          result[result.size() - 1].data,
                                          result[result.size() - 1].dataCount),
                                           minimum( variable ),
                                           maximum( variable ) ) ),
                        IMPLIES( AND2( variable != IMISSING,
                                       emvl::maximum(
                                        result[ result.size() - 1 ].data,
                                        result[ result.size() - 1 ].dataCount )
                                       > MISSING ),
                                 IN_RANGE( emvl::maximum(
                                          result[result.size() - 1].data,
                                          result[result.size() - 1].dataCount),
                                           minimum( variable ),
                                           maximum( variable ) ) ),
                        IMPLIES( result[ result.size() - 1 ].notesCount,
                                 result[ result.size() - 1 ].notes ) ) ) );
  return result;
}



/******************************************************************************
PURPOSE: allDistinctNotes - Return a reference to an array of distict notes.
RETURNS: const std::vector<std::string>&  Default is empty.
NOTES:   If the class has notes this returns all of them made distinct - e.g.,
         by by prepending id and a space before the note.
******************************************************************************/

const std::vector<std::string>& Dataset::allDistinctNotes() const {
  PRE( true );
  static const std::vector<std::string> result;
  POST( true );
  return result;
}



/******************************************************************************
PURPOSE: distinctNotePoint - Return a const pointer to the first point matching
         distictNote or 0 if none matched.
INPUTS:  const std::string& distinctNote  Distinct note to lookup.
RETURNS: const Point* if found match else 0.
NOTES: Default implementation returns 0.
******************************************************************************/

const Point*
Dataset::distinctNotePoint( const std::string& distinctNote ) const {
  PRE( true );
  const Point* const result = 0;
  POST( true );
  return result;
}



/******************************************************************************
PURPOSE: cellVertices - Get a copy of a cell's vertices. This is needed when
         subset() results have coordinates == 0 indicating that coordinats are
         not stored, but rather computed by this routine.
INPUTS:  const size_t cell  Index of cell.
OUTPUTS: Point points[ cellVertexCount() ]  Cell's vertices.
NOTES:   If subset() returns SubsetPointers with coordinates != 0 then the
         cell vertices can be accessed directly without copying.
******************************************************************************/

void Dataset::cellVertices( const size_t cell, Point points[] ) const {
  PRE3( coordinates_.size(),
        points,
        cell < coordinates_.size() / cellVertexCount() );
  const size_t cellVertices = cellVertexCount();
  const size_t offset = cell * cellVertices;

  for ( size_t vertex = 0; vertex < cellVertices; ++vertex ) {
    const size_t coordinateIndex = offset + vertex;
    CHECK( coordinateIndex < coordinates_.size() );
    points[ vertex ] = coordinates_[ coordinateIndex ];
  }
}



//============================ PROTECTED FUNCTIONS ============================



/******************************************************************************
PURPOSE: copy - Copy other's attributes.
INPUTS:  const Dataset& other  Other object to copy.
******************************************************************************/

void Dataset::copy( const Dataset& other ) {
  PRE0( other.invariant() );

  if ( this != &other ) {
    fileName_          = other.fileName_;
    name_              = other.name_;
    description_       = other.description_;
    bounds_            = other.bounds_;
    startingTimestamp_ = other.startingTimestamp_;
    endingTimestamp_   = other.endingTimestamp_;
    timesteps_         = other.timesteps_;
    dataFileOffset_    = other.dataFileOffset_;
    firstDataTimestep_ = other.firstDataTimestep_;
    lastDataTimestep_  = other.lastDataTimestep_;
    variables_         = other.variables_;
    dataVariable_      = other.dataVariable_;
    cellType_          = other.cellType_;
    hasElevation_      = other.hasElevation_;
    hasArea_           = other.hasArea_;
    hasVolume_         = other.hasVolume_;
    isStationary_      = other.isStationary_;
    isGroup_           = other.isGroup_;
    isStructuredGrid_  = other.isStructuredGrid_;
    isVector_          = other.isVector_;
    timestepSize_      = other.timestepSize_;
    variableNames_     = other.variableNames_;
    variableUnits_     = other.variableUnits_;
    minimums_          = other.minimums_;
    maximums_          = other.maximums_;
    coordinates_       = other.coordinates_;
    data_              = other.data_;
  }
}



/******************************************************************************
PURPOSE: equal - Compare to other's attributes.
INPUTS:  const Dataset& other  Other object to compare to.
RETURNS: bool true if equal, else false.
******************************************************************************/

bool Dataset::equal( const Dataset& other ) const {
  PRE( other.invariant() );
  bool result = true;

  if ( this != &other ) {
    result = AND2( result, fileName_          == other.fileName_ );
    result = AND2( result, name_              == other.name_ );
    result = AND2( result, description_       == other.description_ );
    result = AND2( result, bounds_            == other.bounds_ );
    result = AND2( result, startingTimestamp_ == other.startingTimestamp_ );
    result = AND2( result, endingTimestamp_   == other.endingTimestamp_ );
    result = AND2( result, variables_         == other.variables_ );
    result = AND2( result, timesteps_         == other.timesteps_ );
    result = AND2( result, dataFileOffset_    == other.dataFileOffset_ );
    result = AND2( result, firstDataTimestep_ == other.firstDataTimestep_ );
    result = AND2( result, lastDataTimestep_  == other.lastDataTimestep_ );
    result = AND2( result, dataVariable_      == other.dataVariable_ );
    result = AND2( result, cellType_          == other.cellType_ );
    result = AND2( result, hasElevation_      == other.hasElevation_ );
    result = AND2( result, hasArea_           == other.hasArea_ );
    result = AND2( result, hasVolume_         == other.hasVolume_ );
    result = AND2( result, isStationary_      == other.isStationary_ );
    result = AND2( result, isGroup_           == other.isGroup_ );
    result = AND2( result, isStructuredGrid_  == other.isStructuredGrid_ );
    result = AND2( result, isVector_          == other.isVector_ );
    result = AND2( result, timestepSize_      == other.timestepSize_ );
    result = AND2( result, variableNames_     == other.variableNames_ );
    result = AND2( result, variableUnits_     == other.variableUnits_ );
    result = AND2( result, minimums_          == other.minimums_ );
    result = AND2( result, maximums_          == other.maximums_ );
    result = AND2( result, coordinates_       == other.coordinates_ );
    result = AND2( result, data_              == other.data_ );
  }

  POST( true );
  return result;
}



/******************************************************************************
PURPOSE: loadedDataSubset - Determine if data is available for the given time
         range and, if available and not already in data_ then read it from
         fileName_ into data_.
INPUTS:  const Timestamp firstTimestamp  Timestamp of start of data needed.
         const Timestamp lastTimestamp   Timestamp of end   of data needed.
OUTPUTS: firstDataTimestep_, lastDataTimestep_, data_.
RETURNS: bool true if data is available, else false if no data is available.
NOTES:   This is called by Dataset::probe() and Dataset::subset().
         Default implementation does nothing but return true.
******************************************************************************/

bool Dataset::loadedDataSubset( const Timestamp firstTimestamp,
                                const Timestamp lastTimestamp ) const {
  PRE( true );
  POST( true );
  return true;
}



/******************************************************************************
PURPOSE: isValidName - Does name_ start with a letter and contain only
         [a-z,A-Z,0-9,_,.]?
******************************************************************************/

bool Dataset::isValidName() const {
  const size_t count = name_.length();
  const char firstCharacter = count > 0 ? name_[ 0 ] : '\0';
  bool result = AND2( count > 0,
                      OR2( IN_RANGE( firstCharacter, 'a', 'z' ),
                           IN_RANGE( firstCharacter, 'A', 'Z' ) ) );

  for ( size_t index = 1; AND2( result, index < count ); ++index ) {
    const char c = name_[ index ];
    result = OR4( c == '_', c == '-', c == '.', isalnum( c ) );
  }

  return result;
}



/******************************************************************************
PURPOSE: isValidDescription - Does description_ start with a letter or digit
         and contain only printable characters?
******************************************************************************/

bool Dataset::isValidDescription() const {
  const size_t count = description_.length();
  const char firstCharacter = count > 0 ? description_[ 0 ] : '\0';
  bool result = AND2( count > 0,
                      OR3( IN_RANGE( firstCharacter, 'a', 'z' ),
                           IN_RANGE( firstCharacter, 'A', 'Z' ),
                           IN_RANGE( firstCharacter, '0', '9' ) ) );

  for ( size_t index = 1; AND2( result, index < count ); ++index ) {
    const char c = description_[ index ];
    result = OR2( isprint( c ), AND2( c == '\n', index == count - 1 ) );
  }

  return result;
}



/******************************************************************************
PURPOSE: isValidVariableNamesAndUnits - Do variableNames_ consist only of
         letters, numbers and _,
         and do the matching variableUnits_ consist only of
         [A-Z,a-z,0-9,-,_,%,/,*,^]
         and are the counts matching variables_?
******************************************************************************/

bool Dataset::isValidVariableNamesAndUnits() const {
  const size_t count = variableNames_.size();
  bool result = AND2( count > 0, variableUnits_.size() == count );

  // Check variable names:

  for ( size_t index = 0; AND2( result, index < count ); ++index ) {
    const std::string& variableName = variableNames_[ index ];
    const size_t length = variableName.length();
    result = length > 0;

    for ( size_t index2 = 0; AND2( result, index2 < length ); ++index2 ) {
      const char c = variableName[ index2 ];
      result = OR3( c == '_', c == '.', isalnum( c ) );
    }
  }

  // Check unit names:

  for ( size_t index = 0; AND2( result, index < count ); ++index ) {
    const std::string& unitName = variableUnits_[ index ];
    const size_t length = unitName.length();
    result = length > 0;

    for ( size_t index2 = 0; AND2( result, index2 < length ); ++index2 ) {
      const char c = unitName[ index2 ];
      result =
        OR2( isalnum( c ),
             IN10( c, '-', '_', '%', '/', '*', '^', '.', '-', ':' ) );
    }
  }

  return result;
}



/******************************************************************************
PURPOSE: isValidCoordinates - Are coordinates valid?
******************************************************************************/

bool Dataset::isValidCoordinates() const {
  bool result = false;
  const size_t count = coordinates_.size();

  // Check that coordinates are Earth georeferenced:

  if ( count ) { // Stored vertices:
    result = true;

    for ( size_t index = 0; AND2( result, index < count ); ++index ) {
      result = isPoint( coordinates_[ index ] );
    }

  } else if ( AND2( isStructuredGrid_, isStationary_ ) ) {
    const size_t cells = layers() * rows() * columns();

    if ( cells ) {
      const size_t vertexCount = cellVertexCount();
      Point vertices[ 8 ];
      result = OR2( AND2( cellType_ == QUADRILATERAL, vertexCount == 4 ),
                    AND2( cellType_ == HEXAHEDRON, vertexCount == 8 ) );

      for ( size_t cell = 0; AND2( result, cell < cells ); ++cell ) {
        cellVertices( cell, vertices );

        for (size_t vertex = 0; AND2(result, vertex < vertexCount); ++vertex) {
          result = isPoint( vertices[ vertex ] );
        }
      }
    }
  }

  return result;
}



/******************************************************************************
PURPOSE: isValidData - Are data valid?
******************************************************************************/

bool Dataset::isValidData() const {
  const size_t dataCount = data_.size();
  const size_t coordinateCount = coordinates_.size();
  const size_t verticesPerCell = cellVertexCount();
  const size_t cellCount =
    GT_ZERO2( coordinateCount, verticesPerCell ) ?
      coordinateCount / verticesPerCell
    : layers() * rows() * columns();
  bool result =
    AND4( dataCount > 0,
          variables_ > 0,
          cellCount > 0,
          IMPLIES( cellType_ != POLYGONS,
                   dataCount >= variables_ * cellCount ) );
  const double* data = &data_[ 0 ];
  const size_t step0 = dataCount / 1000; // For performance, only sample values
  const size_t step = step0 > 0 ? step0 : 1;

  // Check that data is finite:

  for ( size_t index = 0; AND2( result, index < dataCount ); index += step ) {
    result = isFinite( data[ index ] );
  }

  return result;
}



/******************************************************************************
PURPOSE: isValidRange - Are minimums and maximums valid?
******************************************************************************/

bool Dataset::isValidRange() const {
  const size_t count = minimums_.size();
  bool result =
    AND3( variables_ > 0,
          count == (size_t) variables_,
          count == maximums_.size() );

  // Check that variable minimums <= maximums:

  for ( size_t index = 0; AND2( result, index < count ); ++index ) {
    result = minimums_[ index ] <= maximums_[ index ];
  }

  return result;
}



/******************************************************************************
PURPOSE: isInBounds - Is probe point within bounds (approximately)?
INPUTS:  const double longitude   Longitude of point to check.
         const double latitude    Latitude of point to check.
         const double nearEnough  Distance considered within bounds.
RETURNS: bool true if within bounds, else false.
******************************************************************************/

bool Dataset::isInBounds( const double longitude, const double latitude,
                          const double nearEnough ) const {
  PRE2( isLongitudeLatitude( longitude, latitude ),
        IN_RANGE( nearEnough, 0.0, 1.0 ) );
  const bool result =
    AND2( IN_RANGE( longitude,
                    bounds_.west() - nearEnough,
                    bounds_.east() + nearEnough ),
          IN_RANGE( latitude,
                    bounds_.south() - nearEnough,
                    bounds_.north() + nearEnough ) );
  POST( true );
  return result;
}



/******************************************************************************
PURPOSE: secondsRange - Find seconds range for timestamps and timestepSize_.
INPUTS:  const Timestamp& beginTimestamp  Beginning timestamp.
         const Timestamp& endTimestamp    Ending timestamp.
OUTPUTS: long long& beginSeconds          Seconds of beginTimestamp
                                          adjusted by timestepSize_.
         long long& endSeconds            Seconds of endTimestamp.
                                          adjusted by timestepSize_.
******************************************************************************/

void Dataset::secondsRange( const Timestamp& beginTimestamp,
                            const Timestamp& endTimestamp,
                            long long& beginSeconds,
                            long long& endSeconds ) const {

  PRE03( beginTimestamp.invariant(),
         endTimestamp.invariant(),
         beginTimestamp <= endTimestamp );

  beginSeconds = 0;
  endSeconds   = 0;

  switch ( timestepSize_ ) {
  case HOURS:
    beginSeconds = beginTimestamp.startOfHour().seconds();
    endSeconds   = endTimestamp.endOfHour().seconds();
    break;
  case DAYS:
    beginSeconds = beginTimestamp.startOfDay().seconds();
    endSeconds   = endTimestamp.endOfDay().seconds();
    break;
  case MONTHS:
    beginSeconds = beginTimestamp.startOfMonth().seconds();
    endSeconds   = endTimestamp.endOfMonth().seconds();
    break;
  case YEARS:
    beginSeconds = beginTimestamp.startOfYear().seconds();
    endSeconds   = endTimestamp.endOfYear().seconds();
    break;
  default:
    CHECK( timestepSize_ == TRIMONTHS );
    // 3-month seasons range over months [12, 2], [3, 5], [6, 8], [9, 11].
    {
      int yyyymmdd = beginTimestamp.yyyymmdd();
      int yyyy = yyyymmdd / 10000;
      int mm   = yyyymmdd / 100 % 100;

      if ( mm <= 2 ) {
        mm = 12;
        --yyyy;
      } else if ( mm <= 5 ) {
        mm = 3;
      } else if ( mm <= 8 ) {
        mm = 6;
      } else {
        mm = 9;
      }

      yyyymmdd = ( yyyy * 100 + mm ) * 100 + 1;
      const Timestamp begin( yyyymmdd, 0 );
      beginSeconds = begin.seconds();

      yyyymmdd = endTimestamp.yyyymmdd();
      yyyy = yyyymmdd / 10000;
      mm   = yyyymmdd / 100 % 100;

      if ( mm <= 2 ) {
        mm = 2;
      } else if ( mm <= 5 ) {
        mm = 5;
      } else if ( mm <= 8 ) {
        mm = 8;
      } else {
        mm = 11;
      }

      const int dd = Timestamp::daysInMonth( yyyy, mm );
      yyyymmdd = ( yyyy * 100 + mm ) * 100 + dd;
      Timestamp end( yyyymmdd, 235959 );
      endSeconds = end.seconds();
    }
    break;
  }

  POST0( beginSeconds <= endSeconds );
}



/******************************************************************************
PURPOSE: isInTimeRange - Is timestamp within time range of dataset
         expanded based on timestepSize_?
INPUTS:  const Timestamp& timestamp  Timestamp to check.
RETURNS: bool true if within time range (within hour), else false.
******************************************************************************/

bool Dataset::isInTimeRange( const Timestamp& timestamp ) const {
  PRE( timestamp.invariant() );

  // Compute seconds range of dataset expanded based on timestepSize_:

  long long startingSeconds = 0;
  long long endingSeconds   = 0;
  secondsRange( startingTimestamp_, endingTimestamp_,
                startingSeconds, endingSeconds );
  const long long timestampSeconds = timestamp.seconds();
  const bool result =
    IN_RANGE( timestampSeconds, startingSeconds, endingSeconds );
  POST( true );
  return result;
}



/******************************************************************************
PURPOSE: isInTimeRange - Is timestamp within time range of dataset
         expanded based on timestepSize_? If so, set timestep index.
INPUTS:  const Timestamp& timestamp  Timestamp to check.
OUTPUTS: size_t& timestep            Index of timestep matching timestamp.
RETURNS: bool true if within time range (within hour), else false.
******************************************************************************/

bool Dataset::isInTimeRange( const Timestamp& timestamp,
                             size_t& timestep ) const {
  PRE( timestamp.invariant() );

  // Compute seconds range of dataset expanded based on timestepSize_:

  long long startingSeconds = 0;
  long long endingSeconds   = 0;
  secondsRange( startingTimestamp_, endingTimestamp_,
                startingSeconds, endingSeconds );
  const long long timestampSeconds = timestamp.seconds();
  const bool result =
    IN_RANGE( timestampSeconds, startingSeconds, endingSeconds );

  const Timestamp start( startingTimestamp_.yyyymmdd(),
                         timestepSize_ != HOURS ? 0 :
                         startingTimestamp_.hhmmss() / 10000 * 10000 );
  const Timestamp end( endingTimestamp_.yyyymmdd(),
                       timestepSize_ != HOURS ? 235959 :
                       endingTimestamp_.hhmmss() / 10000 * 10000 + 5959 );

  timestep = 0;

  if ( result ) {
    const long long seconds = timestamp.seconds() - start.seconds();
    CHECK( seconds >= 0 );

    if ( IN3( timestepSize_, HOURS, DAYS ) ) {
      const size_t secondsPerMinute = 60;
      const size_t minutesPerHour = 60;
      const size_t secondsPerHour = secondsPerMinute * minutesPerHour;
      const size_t hours = seconds / secondsPerHour;
      const size_t hoursPerDay = 24;
      timestep = timestepSize_ == HOURS ? hours : hours / hoursPerDay;
    } else {
      Timestamp yyyymmt( timestamp.yyyymmdd() / 100 * 100 + 1, 0 );
      Timestamp yyyymm( start.yyyymmdd() / 100 * 100 + 1, 0 );
      const int monthsOffset =
        timestepSize_ == MONTHS ? 1
        : timestepSize_ == TRIMONTHS ? 3
        : timestepSize_ == YEARS ? 12
        : 0;

      if ( timestepSize_ == TRIMONTHS ) {
        yyyymm.offsetMonths(monthsOffset - 1); // Start at last month of season
      }

      CHECK( monthsOffset );
      //CHECK( yyyymmt >= yyyymm );

      while ( AND2( yyyymm <= end, yyyymmt > yyyymm ) ) {
        ++timestep;
        yyyymm.offsetMonths( monthsOffset );
      }
    }

    if ( dataFileOffset_ ) {
      timestep %= BUFFERED_DATA_TIMESTEPS;
    }
  }

  POST( IMPLIES_ELSE( result, timestep < timesteps_, timestep == 0 ) );
  return result;
}



/******************************************************************************
PURPOSE: isInTimeRange - Is timestamp within time range of dataset
         expanded based on timestepSize_?
INPUTS:  const Timestamp& timestamp  Timestamp to check.
OUTPUTS: Timestamp& firstHour        Hour of first data.
RETURNS: bool true if within time range, else false.
******************************************************************************/

bool Dataset::isInTimeRange( const Timestamp& timestamp,
                             Timestamp& firstHour ) const {
  PRE2( timestamp.invariant(), firstHour.invariant() );

  // Compute seconds range of dataset expanded based on timestepSize_:

  long long startingSeconds = 0;
  long long endingSeconds   = 0;
  secondsRange( startingTimestamp_, endingTimestamp_,
                startingSeconds, endingSeconds );
  const long long timestampSeconds = timestamp.seconds();
  const bool result =
    IN_RANGE( timestampSeconds, startingSeconds, endingSeconds );
  const Timestamp start( startingTimestamp_.yyyymmdd(),
                         timestepSize_ != HOURS ? 0 :
                         startingTimestamp_.hhmmss() / 10000 * 10000 );
  firstHour = start;
  POST( firstHour.invariant() );
  return result;
}



/******************************************************************************
PURPOSE: overlappedTimeRange - Does time range overlap this dataset time range?
INPUTS:  Timestamp& beginTimestamp  Beginning timestamp to check.
         Timestamp& endTimestamp    Ending timestamp to check.
OUTPUTS: Timestamp& beginTimestamp  Adjusted to start of hour (and day if daily)
         Timestamp& endTimestamp    Adjusted to end   of hour (and day if daily)
RETURNS: bool true if within time range (within hour), else false.
******************************************************************************/

bool Dataset::overlappedTimeRange( Timestamp& beginTimestamp,
                                   Timestamp& endTimestamp ) const {

  PRE3( beginTimestamp.invariant(), endTimestamp.invariant(),
        beginTimestamp <= endTimestamp );

  // Compute seconds range of dataset expanded based on timestepSize_:

  long long startingSeconds = 0;
  long long endingSeconds   = 0;
  secondsRange( startingTimestamp_, endingTimestamp_,
                startingSeconds, endingSeconds );

  const Timestamp start( startingSeconds );
  const Timestamp end( endingSeconds );

  const Timestamp beginTime( beginTimestamp.yyyymmdd(),
                             beginTimestamp.hhmmss() / 10000 * 10000 );

  const Timestamp endTime( endTimestamp.yyyymmdd(),
                           endTimestamp.hhmmss() / 10000 * 10000 + 5959 );

  const bool outside = OR2( endTime < start, beginTime > end );
  const bool result = ! outside;
  beginTimestamp = beginTime;
  endTimestamp   = endTime;

  POST2( beginTimestamp.invariant(), endTimestamp.invariant() );
  return result;
}



/******************************************************************************
PURPOSE: overlappedTimeRange - Does time range overlap this data time range?
         If so output the first and last timestep indices within range.
INPUTS:  const Timestamp beginTimestamp  Beginning timestamp to check.
         const Timestamp endTimestamp    Ending timestamp to check.
OUTPUTS: size_t& firstTimestep           First timestep index within range.
         size_t& matchingTimesteps       Number of timesteps within range.
RETURNS: bool true if within time range (within hour), else false.
NOTES:   Used for temporally non-sparse hourly or daily data.
******************************************************************************/

bool Dataset::overlappedTimeRange( const Timestamp& beginTimestamp,
                                   const Timestamp& endTimestamp,
                                   size_t& firstTimestep,
                                   size_t& matchingTimesteps ) const {

  PRE3( beginTimestamp.invariant(), endTimestamp.invariant(),
        beginTimestamp <= endTimestamp );

  size_t unused = 0;
  const bool result =
    overlappedTimeRange( beginTimestamp, endTimestamp,
                         firstTimestep, matchingTimesteps, unused );

  POST3( beginTimestamp.invariant(), endTimestamp.invariant(),
         IMPLIES_ELSE( result,
                       AND2( firstTimestep < timesteps_,
                             IN_RANGE( firstTimestep + matchingTimesteps,
                                       1, timesteps_ ) ),
                       IS_ZERO2( firstTimestep, matchingTimesteps ) ) );
  return result;
}



/******************************************************************************
PURPOSE: overlappedTimeRange - Does time range overlap this data time range?
         If so output the first and last timestep indices within range.
INPUTS:  const Timestamp beginTimestamp  Beginning timestamp to check.
         const Timestamp endTimestamp    Ending timestamp to check.
OUTPUTS: size_t& firstTimestep           First timestep index within range.
         size_t& matchingTimesteps       Number of timesteps within range.
         size_t& skippedHours            Number of hours in range skipped
                                         before firstTimestep.
RETURNS: bool true if within time range (within hour), else false.
NOTES:   Used for temporally non-sparse hourly or daily data.
******************************************************************************/

bool Dataset::overlappedTimeRange( const Timestamp& beginTimestamp,
                                   const Timestamp& endTimestamp,
                                   size_t& firstTimestep,
                                   size_t& matchingTimesteps,
                                   size_t& skippedHours ) const {

  PRE3( beginTimestamp.invariant(), endTimestamp.invariant(),
        beginTimestamp <= endTimestamp );

  // Compute seconds range of dataset expanded based on timestepSize_:

  long long startingSeconds = 0;
  long long endingSeconds   = 0;
  secondsRange( startingTimestamp_, endingTimestamp_,
                startingSeconds, endingSeconds );

  const Timestamp startExpanded( startingSeconds );
  const Timestamp endExpanded( endingSeconds );

  const Timestamp beginTime( beginTimestamp.yyyymmdd(),
                             beginTimestamp.hhmmss() / 10000 * 10000 );

  const Timestamp endTime( endTimestamp.yyyymmdd(),
                           endTimestamp.hhmmss() / 10000 * 10000 + 5959 );

  const bool outside = OR2( endTime < startExpanded, beginTime > endExpanded );
  const bool result = ! outside;
  firstTimestep = 0;
  matchingTimesteps = 0;
  skippedHours = 0;

  if ( result ) {

    // Expand actual starting/ending timestamps to full hour:

    const Timestamp start( startingTimestamp_.yyyymmdd(),
                           timestepSize_ != HOURS ? 0 :
                           startingTimestamp_.hhmmss() / 10000 * 10000 );

    const Timestamp end( endingTimestamp_.yyyymmdd(),
                         timestepSize_ != HOURS ? 235959 :
                         endingTimestamp_.hhmmss() / 10000 * 10000 + 5959 );


    if ( IN3( timestepSize_, HOURS, DAYS ) ) {

      // Compute first matching timestep (i.e., within the same hour):

      const long long firstSeconds0 = beginTime.seconds() - start.seconds();
      const long long firstSeconds = firstSeconds0 < 0 ? 0 : firstSeconds0;
      const size_t firstHours = firstSeconds / 3600;
      const size_t firstDays = firstHours / 24;
      firstTimestep = timestepSize_ != HOURS ? firstDays : firstHours;
      CHECK( firstTimestep < timesteps_ );

      skippedHours = firstSeconds0 < 0 ? -firstSeconds0 / 3600 : 0;

      // Compute last matching timestep (i.e., within the same hour):

      const long long lastSeconds0 = endTime.seconds() - start.seconds();
      const long long lastSeconds = lastSeconds0 < 0 ? 0 : lastSeconds0;
      const size_t lastHours = lastSeconds / 3600;
      const size_t lastDays = lastHours / 24;
      const size_t lastTimestep0 =
        timestepSize_ != HOURS ? lastDays : lastHours;
      const size_t lastTimestep =
        clampedToRange( lastTimestep0, firstTimestep, timesteps_ - 1 );
      CHECK( IN_RANGE( lastTimestep, firstTimestep, timesteps_ - 1 ) );
      matchingTimesteps = 1 + lastTimestep - firstTimestep;
    } else {
      const int monthsOffset =
        timestepSize_ == MONTHS ? 1
        : timestepSize_ == TRIMONTHS ? 3
        : timestepSize_ == YEARS ? 12
        : 0;
      CHECK( monthsOffset );
      Timestamp yyyymm( start.yyyymmdd() / 100 * 100 + 1, 0 );

      if ( timestepSize_ == TRIMONTHS ) {
        yyyymm.offsetMonths(monthsOffset - 1); // Start at last month of season
      }

      while ( AND2( yyyymm < beginTime, ! yyyymm.sameMonth( beginTime ) ) ) {
        ++firstTimestep;
        yyyymm.offsetMonths( monthsOffset );
      }

      matchingTimesteps = 1;

      while ( AND2( yyyymm < endTime, ! yyyymm.sameMonth( endTime ) ) ) {
        ++matchingTimesteps;
        yyyymm.offsetMonths( monthsOffset );
      }
    }
  }

  POST3( beginTimestamp.invariant(), endTimestamp.invariant(),
         IMPLIES_ELSE( result,
                       AND2( firstTimestep < timesteps_,
                             IN_RANGE( firstTimestep + matchingTimesteps,
                                       1, timesteps_ ) ),
                       IS_ZERO2( firstTimestep, matchingTimesteps ) ) );
  return result;
}



/******************************************************************************
PURPOSE: overlappedTimeRange - Does time range overlap this data time range?
INPUTS:  Timestamp& beginTimestamp  Beginning timestamp to check.
         Timestamp& endTimestamp    Ending timestamp to check.
OUTPUTS: Timestamp& beginTimestamp  Adjusted to start of hour (and day if daily)
         Timestamp& endTimestamp    Adjusted to end   of hour (and day if daily)
         Timestamp& firstHour       startingTimestamp adjusted to start of hour
                                    (and day if daily variable).
         Timestamp& lastHour        endingTimestamp adjusted to end of hour
                                    (and day if daily variable).
RETURNS: bool true if within time range (within hour), else false.
******************************************************************************/

bool Dataset::overlappedTimeRange( Timestamp& beginTimestamp,
                                   Timestamp& endTimestamp,
                                   Timestamp& firstHour,
                                   Timestamp& lastHour ) const {

  PRE6( beginTimestamp.invariant(), endTimestamp.invariant(),
        beginTimestamp <= endTimestamp,
        firstHour.invariant(), lastHour.invariant(), firstHour <= lastHour );

  // Compute seconds range of dataset expanded based on timestepSize_:

  long long startingSeconds = 0;
  long long endingSeconds   = 0;
  secondsRange( startingTimestamp_, endingTimestamp_,
                startingSeconds, endingSeconds );

  const Timestamp startExpanded( startingSeconds );
  const Timestamp endExpanded( endingSeconds );

  const Timestamp beginTime( beginTimestamp.yyyymmdd(),
                             beginTimestamp.hhmmss() / 10000 * 10000 );

  const Timestamp endTime( endTimestamp.yyyymmdd(),
                           endTimestamp.hhmmss() / 10000 * 10000 + 5959 );

  const bool outside = OR2( endTime < startExpanded, beginTime > endExpanded );
  const bool result = ! outside;

  // Expand actual starting/ending timestamps to full hour:

  const Timestamp start( startingTimestamp_.yyyymmdd(),
                         timestepSize_ != HOURS ? 0 :
                         startingTimestamp_.hhmmss() / 10000 * 10000 );

  const Timestamp end( endingTimestamp_.yyyymmdd(),
                       timestepSize_ != HOURS ? 235959 :
                       endingTimestamp_.hhmmss() / 10000 * 10000 + 5959 );

  beginTimestamp = beginTime;
  endTimestamp   = endTime;
  firstHour      = start;
  lastHour       = end;

  POST4( beginTimestamp.invariant(), endTimestamp.invariant(),
         firstHour.invariant(), lastHour.invariant() );
  return result;
}



/******************************************************************************
PURPOSE: matchingTimestamps - Find index of first and last timestamp matching
         a given timestamp - i.e., same day for daily data else same hour.
INPUTS:  const Timestamp& timestamp                Timestamp to find.
         const std::vector<Timestamp>& timestamps  Sorted Timestamps to search.
OUTPUTS: size_t& first                       Index of first matching timestamp.
         size_t& last                        Index of last  matching timestamp.
RETURNS: bool true if found matches else false if no matches found.
******************************************************************************/

bool Dataset::matchingTimestamps( const Timestamp& timestamp,
                                  const std::vector<Timestamp>& timestamps,
                                  size_t& first, size_t& last ) const {

  PRE04( timestamp.invariant(),
         timestamps.size(),
         timestamps[ 0 ].invariant(),
         timestamps[ timestamps.size() - 1 ].invariant() );

  const bool result =
    matchingTimestamps( timestamp, timestamp, timestamps, timestamps,
                        first, last );

  POST0( IMPLIES( result,
                  AND2( first < timestamps.size(),
                        IN_RANGE( last, first, timestamps.size() - 1 ) ) ) );
  return result;
}



/******************************************************************************
PURPOSE: matchingTimestamps - Find index of first and last timestamp matching
         a given timestamp - i.e., same day for daily data else same hour.
INPUTS:  const Timestamp& timestamp                Timestamp to find.
         const std::vector<Timestamp>& startingTimestamps  Sorted starting
                                                           Timestamps to search.
         const std::vector<Timestamp>& endingTimestamps    Sorted ending
                                                           Timestamps to search.
OUTPUTS: size_t& first                       Index of first matching timestamp.
         size_t& last                        Index of last  matching timestamp.
RETURNS: bool true if found matches else false if no matches found.
******************************************************************************/

bool Dataset::matchingTimestamps( const Timestamp& timestamp,
                                  const std::vector<Timestamp>&
                                    startingTimestamps,
                                  const std::vector<Timestamp>&
                                    endingTimestamps,
                                  size_t& first, size_t& last ) const {

  PRE07( timestamp.invariant(),
         startingTimestamps.size(),
         startingTimestamps[ 0 ].invariant(),
         startingTimestamps[ startingTimestamps.size() - 1 ].invariant(),
         endingTimestamps.size() == startingTimestamps.size(),
         endingTimestamps[ 0 ].invariant(),
         endingTimestamps[ endingTimestamps.size() - 1 ].invariant() );

  const bool result =
    matchingTimestamps( timestamp, timestamp,
                        startingTimestamps, endingTimestamps, first, last );

  POST0( IMPLIES( result,
                  AND2( first < startingTimestamps.size(),
                        IN_RANGE( last, first, startingTimestamps.size()-1))));
  return result;
}



/******************************************************************************
PURPOSE: matchingTimestamps - Find index of first and last timestamp matching
         a given timestamp range - i.e., same day for daily data
         else same hour.
INPUTS:  const Timestamp& beginTimestamp          Beginning timestamp to find
         const Timestamp& endTimestamp            Ending    timestamp to find
         const std::vector<Timestamp>& timestamps Sorted Timestamps to search.
OUTPUTS: size_t& first                      Index of first matching timestamp.
         size_t& last                       Index of last  matching timestamp.
RETURNS: bool true if found matches else false if no matches found.
******************************************************************************/

bool Dataset::matchingTimestamps( const Timestamp& beginTimestamp,
                                  const Timestamp& endTimestamp,
                                  const std::vector<Timestamp>& timestamps,
                                  size_t& first, size_t& last ) const {

  PRE06( beginTimestamp.invariant(),
         endTimestamp.invariant(),
         beginTimestamp <= endTimestamp,
         timestamps.size(),
         timestamps[ 0 ].invariant(),
         timestamps[ timestamps.size() - 1 ].invariant() );

  const bool result =
    matchingTimestamps( beginTimestamp, endTimestamp, timestamps, timestamps,
                        first, last );

  POST0( IMPLIES( result,
                  AND2( first < timestamps.size(),
                        IN_RANGE( last, first, timestamps.size() - 1 ) ) ) );
  return result;
}



/******************************************************************************
PURPOSE: matchingTimestamps - Find index of first and last timestamp matching
         a given timestamp - i.e., same day for daily data else same hour.
INPUTS:  const Timestamp& beginTimestamp          Beginning timestamp to find
         const Timestamp& endTimestamp            Ending    timestamp to find
         const std::vector<Timestamp>& startingTimestamps  Sorted starting
                                                           Timestamps to search.
         const std::vector<Timestamp>& endingTimestamps    Sorted ending
                                                           Timestamps to search.
OUTPUTS: size_t& first                       Index of first matching timestamp.
         size_t& last                        Index of last  matching timestamp.
RETURNS: bool true if found matches else false if no matches found.
******************************************************************************/

bool Dataset::matchingTimestamps( const Timestamp& beginTimestamp,
                                  const Timestamp& endTimestamp,
                                  const std::vector<Timestamp>&
                                    startingTimestamps,
                                  const std::vector<Timestamp>&
                                    endingTimestamps,
                                  size_t& first, size_t& last ) const {

  PRE09( beginTimestamp.invariant(),
         endTimestamp.invariant(),
         beginTimestamp <= endTimestamp,
         startingTimestamps.size(),
         startingTimestamps[ 0 ].invariant(),
         startingTimestamps[ startingTimestamps.size() - 1 ].invariant(),
         endingTimestamps.size() == startingTimestamps.size(),
         endingTimestamps[ 0 ].invariant(),
         endingTimestamps[ endingTimestamps.size() - 1 ].invariant() );

  // Get seconds range of begin/endTimestamp expanded based on timestepSize_:

  long long beginSeconds = 0;
  long long endSeconds   = 0;
  secondsRange( beginTimestamp, endTimestamp, beginSeconds, endSeconds );

  // Find first and last timesteps within the range [beginSeconds, endSeconds]:

  const size_t count = startingTimestamps.size();
  size_t result = false;
  first = count;
  last = count;

  for ( size_t index = 0; index < count; ++index ) {
    const Timestamp& thisEndingTimestamp = endingTimestamps[ index ];
    const long long thisEndingSeconds = thisEndingTimestamp.seconds();
    const bool before = thisEndingSeconds < beginSeconds;

    if ( ! before ) {
      const Timestamp& thisStartingTimestamp = startingTimestamps[ index ];
      const long long thisStartingSeconds = thisStartingTimestamp.seconds();
      const bool after = endSeconds < thisStartingSeconds;

      if ( after ) {
        index = count; // Stop looping.
      } else { // In range:

        if ( first == count ) { // Initialize both:
          first = last = index;
          result = true;
        } else {
          last = index; // Update last.
        }
      }
    }
  }

  POST0( IMPLIES( result,
                  AND2( first < startingTimestamps.size(),
                        IN_RANGE( last, first, startingTimestamps.size()-1))));
  return result;
}



/******************************************************************************
PURPOSE: vtimeseries - Probe dataset over time range at a specified location.
INPUTS:  const Timestamp& beginTimestamp  Beginning timestamp of sample.
         const Timestamp& endTimestamp    Ending timestamp of sample.
         const double longitude           Longitude of probe.
         const double latitude            Latitude of probe.
         const double elevation           Elevation of probe or MISSING.
         const int layer                  Layer of probe or IMISSING.
OUTPUTS: std::vector<double>& result      Hourly values (or MISSING) over time
                                          range at location.
NOTES:   Called by base class member function timeseries().
         Template Method Design Pattern.
         This vitual default implementation calls probe() for each hour in
         the specified time range. Derived classes may have their own
         implementation that is more efficient.
******************************************************************************/

void Dataset::vtimeseries( const Timestamp beginTimestamp,
                           const Timestamp endTimestamp,
                           const double longitude,
                           const double latitude,
                           const double elevation,
                           const int layer,
                           std::vector<double>& result ) const {

  PRE6( beginTimestamp.invariant(),
        endTimestamp.invariant(),
        beginTimestamp <= endTimestamp,
        isLongitudeLatitude( longitude, latitude ),
        OR2( elevation == MISSING, isElevation( elevation ) ),
        OR2( layer == IMISSING, AND2( layer >= 0, (size_t) layer < layers())));

  Timestamp timestamp( beginTimestamp.startOfHour() );
  Timestamp end( endTimestamp.endOfHour() );

  for ( size_t hour = 0;
        timestamp <= endTimestamp;
        ++hour, timestamp.offsetHours( 1 ) ) {
    CHECK( hour < result.size() );
    double value = MISSING;

    if ( ! isVector_ ) {
      value =
        probe( timestamp, longitude, latitude, elevation, layer, dataVariable_);
    } else { // Compute vector magnitude:
      const double value0 =
        probe( timestamp, longitude, latitude, elevation, layer, 0 );

      if ( value0 != MISSING ) {
        const double value1 =
          probe( timestamp, longitude, latitude, elevation, layer, 1 );

        if ( value1 != MISSING ) {
          const double value2 =
            variables_ < 3 ? 0.0
            : probe( timestamp, longitude, latitude, elevation, layer, 2 );

          if ( value2 != MISSING ) {
            value = sqrt(value0 * value0 + value1 * value1 + value2 * value2);
          }
        }
      }
    }

    result[ hour ] = value;
  }

  POST3( result.size() >=
           (size_t) 1 + beginTimestamp.hoursUntil( endTimestamp ),
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
PURPOSE: allZeroHour - Do all Timestamps have hhmm == 00:00?
INPUTS:  const std::vector<Timestamp>& timestamps  Timestamps to search.
RETURNS: bool true if all Timestamps have hhmm == 00:00, else false.
******************************************************************************/

bool Dataset::allZeroHour( const std::vector<Timestamp>& timestamps ) {
  PRE03( timestamps.size(),
         timestamps[ 0 ].invariant(),
         timestamps[ timestamps.size() - 1 ].invariant() );

  const size_t count = timestamps.size();
  bool result = true;

  for ( size_t index = 0; AND2( result, index < count ); ++index ) {
    const Timestamp& timestamp = timestamps[ index ];
    result = timestamp.hhmmss() == 0;
  }

  POST0( IMPLIES_ELSE( result,
                       timestamps[ 0 ].hhmmss() == 0,
                       timestamps[ 0 ].hhmmss() != 0 ) );
  return result;
}



/******************************************************************************
PURPOSE: computeBounds - Compute bounds from coordinates.
******************************************************************************/

void Dataset::computeBounds() {
  PRE02( coordinates_.size(), isValidCoordinates() );
  const size_t points = coordinates_.size();
  double minimumLongitude = coordinates_[ 0 ].longitude;
  double maximumLongitude = minimumLongitude;
  double minimumLatitude = coordinates_[ 0 ].latitude;
  double maximumLatitude = minimumLatitude;

  for ( size_t point = 1; point < points; ++point ) {
    const double longitude = coordinates_[ point ].longitude;
    const double latitude  = coordinates_[ point ].latitude;

    if ( longitude < minimumLongitude ) {
      minimumLongitude = longitude;
    } else if ( longitude > maximumLongitude ) {
      maximumLongitude = longitude;
    }

    if ( latitude < minimumLatitude ) {
      minimumLatitude = latitude;
    } else if ( latitude > maximumLatitude ) {
      maximumLatitude = latitude;
    }
  }

  bounds_.west( minimumLongitude );
  bounds_.east( maximumLongitude );
  bounds_.south( minimumLatitude );
  bounds_.north( maximumLatitude );
  POST0( bounds_.invariant() );
}



/******************************************************************************
PURPOSE: cellCenter - Average of cell vertices.
INPUTS:  const size_t cell  Index of cell.
RETURNS: Point center  Average of cell vertices.
******************************************************************************/

Point Dataset::cellCenter( const size_t cell ) const {
  PRE( cell < coordinates_.size() / cellVertexCount() );
  const size_t verticesPerCell = cellVertexCount();
  const size_t offset = cell * verticesPerCell;
  Point result = { 0.0, 0.0, 0.0 };

  for ( size_t vertex = 0; vertex < verticesPerCell; ++vertex ) {
    const size_t index = offset + vertex;
    CHECK( index < coordinates_.size() );
    const Point& point = coordinates_[ index ];
    result.longitude += point.longitude;
    result.latitude  += point.latitude;
    result.elevation += point.elevation;
  }

  result.longitude /= verticesPerCell;
  result.latitude  /= verticesPerCell;
  result.elevation /= verticesPerCell;

  POST2( isLongitudeLatitude( result.longitude, result.latitude ),
         isElevation( result.elevation ) );
  return result;
}



/******************************************************************************
PURPOSE: computeVerticalRectangleCoordinates - Given cell center longitude,
         latitude, and elevation points, compute and store vertices of
         vertically-oriented rectangular cells for each cell center.
INPUTS:  const size_t groundPoints         Number of ground points.
         const size_t verticalPoints       Number of vertical points above
                                           each ground point.
         const double longitudeCenters[ groundPoints ] Cell center longitudes.
         const double latitudeCenters[  groundPoints ] Cell center latitudes.
         const double elevationCenters[  groundPoints * verticalPoints ]
                                         Cell center elevations.
         const double thicknessCenters[  groundPoints * verticalPoints ]
                                         Cell thicknesses or 0.
OUTPUTS: std::vector<Point>& coordinates  Coordinates to append each cell's
                                          4 vertices to.
NOTES:   Throws exception upon failure if invalid coordinate are in the data.
******************************************************************************/

void Dataset::computeVerticalRectangleCoordinates( const size_t groundPoints,
                                                   const size_t verticalPoints,
                                              const double longitudeCenters[],
                                              const double latitudeCenters[],
                                              const double elevationCenters[],
                                              const double thicknessCenters[],
                                            std::vector<Point>& coordinates ) {
  PRE05( groundPoints > 0, verticalPoints > 0,
         longitudeCenters, latitudeCenters, elevationCenters);

  const double minimumWidth = 1e-3; // Size of single-point cell (deg) ~100m.
  const size_t groundPoints_1 = groundPoints - 1;
  const size_t verticalPoints_1 = verticalPoints - 1;
  bool ok = true;

  for ( size_t groundPoint = 0;
        AND2( ok, groundPoint < groundPoints );
        ++groundPoint ) {

    // Compute ground cell edge longitudes, latitudes from centers:

    double longitude1 = 0.0; // Ground cell edge 1st longitude.
    double longitude2 = 0.0; // Ground cell edge 2nd longitude.
    double latitude1  = 0.0; // Ground cell edge 1st latitude.
    double latitude2  = 0.0; // Ground cell edge 2nd latitude.

    const double longitudeCenter0 = longitudeCenters[ groundPoint ];
    const double longitudeCenter1 = // Next/other center.
      groundPoint < groundPoints_1 ? longitudeCenters[ groundPoint + 1 ]
      : groundPoint > 0 ? longitudeCenters[ groundPoint - 1 ]
      : isLongitude( longitudeCenter0 ) ?
        clampedToRange( longitudeCenter0 +
                          SIGN( longitudeCenter0 ) * minimumWidth,
                        -180.0, 180.0 )
      : 0.0;

    const double latitudeCenter0 = latitudeCenters[ groundPoint ];
    const double latitudeCenter1 = // Next/other center.
      groundPoint < groundPoints_1 ? latitudeCenters[ groundPoint + 1 ]
      : groundPoint > 0 ? latitudeCenters[ groundPoint - 1 ]
      : isLatitude( latitudeCenter0 ) ?
        clampedToRange( latitudeCenter0 +
                        SIGN( latitudeCenter0 ) * minimumWidth, -90.0, 90.0 )
      : 0.0;

    ok =
      AND2( isLongitudeLatitude( longitudeCenter0, latitudeCenter0 ),
            isLongitudeLatitude( longitudeCenter1, latitudeCenter1 ) );

    if ( ok ) { // Inputs are valid so compute valid edge coordinates:

      // Compute cell edge longitudes:

      longitude2 = 0.5 * ( longitudeCenter0 + longitudeCenter1 ); // Average.
      CHECK( isLongitude( longitude2 ) );
      longitude1 = longitudeCenter0 + longitudeCenter0 - longitude2;

      if ( longitude1 <= -180.0 ) { // Clamp when at or past West edge:
        longitude2 = -180.0 + minimumWidth;
        longitude1 = longitude2 + minimumWidth;
      } else if ( longitude1 >= 180.0 ) { // Clamp when at or past East edge:
        longitude2 = 180.0 - minimumWidth;
        longitude1 = longitude2 - minimumWidth;
      }

      CHECK2( isLongitude( longitude1 ), isLongitude( longitude2 ) );

      // Compute cell edge latitudes:

      latitude2 = 0.5 * ( latitudeCenter0 + latitudeCenter1 ); // Average.
      CHECK( isLatitude( latitude2 ) );
      latitude1 = latitudeCenter0 + latitudeCenter0 - latitude2;

      if ( latitude1 <= -90.0 ) { // Clamp when at or past South edge:
        latitude2 = -90.0 + minimumWidth;
        latitude1 = latitude2 + minimumWidth;
      } else if ( latitude1 >= 90.0 ) { // Clamp when at or past North edge:
        latitude2 = 90.0 - minimumWidth;
        latitude1 = latitude2 - minimumWidth;
      }

      CHECK2( isLatitude( latitude1 ), isLatitude( latitude2 ) );
    }

    // Compute elevations of vertical cell bottom and top edges:

    for ( size_t verticalPoint = 0, offset = groundPoint * verticalPoints;
          AND2( ok, verticalPoint < verticalPoints );
          ++verticalPoint ) {
      const size_t index = offset + verticalPoint;
      const double elevationCenter = elevationCenters[ index ];
      ok = isElevation( elevationCenter );
  
      if ( ok ) {
        double halfThickness = 0.0;

        if ( thicknessCenters ) { // Use given cell thicknesses:
          const double thickness = thicknessCenters[ index ];
          ok = IN_RANGE( thickness, 0.0,
                         0.5 * QueryParameters::MAXIMUM_ELEVATION );
          halfThickness = 0.5 * thickness;
        } else { // Compute cell thicknesses from adjacent cell centers:
          const double elevationCenter1 = // Next center.
            verticalPoint < verticalPoints_1 ?
              elevationCenters[ verticalPoint + 1 ]
            : verticalPoint > 0 ? elevationCenters[ verticalPoint - 1 ]
            : elevationCenter + minimumWidth; // Degenerate case.
          ok = isElevation( elevationCenter1 );
          halfThickness = 0.5 * ( elevationCenter1 - elevationCenter );
        }

        if ( ok ) {
          const double elevation1 =
            clampedToRange( elevationCenter - halfThickness,
                            QueryParameters::MINIMUM_ELEVATION,
                            QueryParameters::MAXIMUM_ELEVATION );

          const double elevation2 =
            clampedToRange( elevationCenter + halfThickness,
                            QueryParameters::MINIMUM_ELEVATION,
                            QueryParameters::MAXIMUM_ELEVATION );

          const Point point0 = { longitude1, latitude1, elevation1 };
          const Point point1 = { longitude2, latitude2, elevation1 };
          const Point point2 = { longitude2, latitude2, elevation2 };
          const Point point3 = { longitude1, latitude1, elevation2 };
          coordinates.push_back( point0 );
          coordinates.push_back( point1 );
          coordinates.push_back( point2 );
          coordinates.push_back( point3 );
        }
      }
    }
  }

  if ( ! ok ) {
    throw std::runtime_error("Failed to read valid vertical coordinates."); //x
  }
}



/******************************************************************************
PURPOSE: computeVariableRanges - Compute range of each data variable.
INPUTS:  variables_, data_
OUTPUTS: minimums_, maximums_
NOTES:   Throws exception upon failure.
******************************************************************************/

void Dataset::computeVariableRanges() {
  PRE04( variables_ > 0,
         data_.size() > 0,
         data_.size() % variables_ == 0,
         allFinite( &data_[ 0 ], data_.size() ) );

  const size_t valuesPerVariable = data_.size() / variables_;
  size_t offset = 0;
  minimums_.resize( variables_ ); //x
  maximums_.resize( variables_ ); //x

  DEBUG( fprintf( stderr, "dataFileOffset_ = %" LU ", variables_ = %d, "
                  "valuesPerVariable = %" LU "\n",
                  dataFileOffset_, variables_, valuesPerVariable ); )

  for ( int variable = 0; variable < variables_;
        ++variable, offset += valuesPerVariable ) {
    CHECK( offset < data_.size() );
    const double* const variableData = &data_[ offset ];
    emvl::range( variableData, valuesPerVariable, MISSING,
                 minimums_[ variable ], maximums_[ variable ] );

    DEBUG( fprintf( stderr, "%d: offset = %" LU ", first, last = [%f, %f], "
                    "minmax = [%f, %f]\n",
                    variable, offset,
                    data_[ offset ], data_[ offset + valuesPerVariable - 1 ],
                    minimums_[ variable ], maximums_[ variable ] ); )
  }

  POST0( isValidRange() );
}



/******************************************************************************
PURPOSE: skipLine - Read and discard a line (of maximum length 255 characters)
         from a stream.
INPUTS:  FILE* file  Stream to read from.
OUTPUTS: FILE* file  Advanced stream.
NOTES:   throws std::runtime_error if unable to read a line (of length <= 255).
******************************************************************************/

void Dataset::skipLine( FILE* file ) {
  PRE0( file );
  enum { MAXIMUM_LENGTH = 255 };
  char line[ MAXIMUM_LENGTH + 1 ] = "";
  memset( line, 0, sizeof line );
  bool ok = false;

  if ( fgets( line, MAXIMUM_LENGTH, file ) ) {

    if ( OR2( isprint( *line ), *line == '\n' ) ) {
      const size_t length = strlen( line );
      CHECK( IN_RANGE( length, 1, MAXIMUM_LENGTH ) );
      const char lastCharacter = line[ length - 1 ];
      ok = lastCharacter == '\n';
    }
  }

  if ( ! ok ) {
    throw std::runtime_error( "Failed to read a line from a stream." ); //x
  }
}



/******************************************************************************
PURPOSE: skipBytes - Read and discard bytes from a stream.
INPUTS:  FILE* file          Stream to read from.
         const size_t bytes  Bytes to skip.
OUTPUTS: FILE* file          Advanced stream.
NOTES:   throws std::runtime_error if unable to read bytes.
******************************************************************************/

void Dataset::skipBytes( FILE* file, const size_t bytes ) {
  PRE02( file, bytes );
  enum { BUFFER_SIZE = 1024 };
  char buffer[ BUFFER_SIZE ] = "";
  memset( buffer, 0, sizeof buffer );
  size_t bytesRemaining = bytes;
  bool ok = false;

  do {
    const size_t bytesToRead =
      bytesRemaining < BUFFER_SIZE ? bytesRemaining : BUFFER_SIZE;
    ok = fread( buffer, sizeof *buffer, bytesToRead, file ) == bytesToRead;

    if ( ok ) {
      bytesRemaining -= bytesToRead;
    }

  } while ( AND2( ok, bytesRemaining ) );

  if ( ! ok ) {
    throw std::runtime_error( "Failed to read/skip bytes from a stream." ); //x
  }
}



/******************************************************************************
PURPOSE: readAndMatchLine - Read and match a line from a stream.
INPUTS:  FILE* file               Stream to read from.
         const char* const match  Line to match.
OUTPUTS: FILE* file               Advanced stream.
NOTES:   throws std::runtime_error if unable to read a matched line.
******************************************************************************/

void Dataset::readAndMatchLine( FILE* file, const char* match ) {
  PRE05( file, match, *match, strlen( match ) < 256,
         match[ strlen( match ) - 1 ] == '\n' );
  enum { MAXIMUM_LENGTH = 255 };
  char line[ MAXIMUM_LENGTH + 1 ] = "";
  memset( line, 0, sizeof line );
  bool ok = false;

  if ( fgets( line, MAXIMUM_LENGTH, file ) ) {
    ok = ! strcmp( line, match );
  }

  if ( ! ok ) {
    throw std::runtime_error("Failed to read matched line from a stream."); //x
  }
}



/******************************************************************************
PURPOSE: readAndMatchLine2 - Read and match one of two possible lines from a
         stream.
INPUTS:  FILE* file                Stream to read from.
         const char* const match1  1st possible line to match.
         const char* const match2  2nd possible line to match.
OUTPUTS: FILE* file                Advanced stream.
RETURNS: int 1 if first line matched or 2 if second line matched.
NOTES:   throws std::runtime_error if unable to read a matched line.
******************************************************************************/

int Dataset::readAndMatchLine2( FILE* file,
                                const char* const match1,
                                const char* const match2 ) {
  PRE09( file,
         match1,
         *match1,
         strlen( match1 ) < 256,
         match1[ strlen( match1 ) - 1 ] == '\n',
         match2,
         *match2,
         strlen( match2 ) < 256,
         match2[ strlen( match2 ) - 1 ] == '\n' );

  enum { MAXIMUM_LENGTH = 255 };
  char line[ MAXIMUM_LENGTH + 1 ] = "";
  memset( line, 0, sizeof line );
  int result = 0;

  if ( fgets( line, MAXIMUM_LENGTH, file ) ) {

    if ( ! strcmp( line, match1 ) ) {
      result = 1;
    } else if ( ! strcmp( line, match2 ) ) {
      result = 2;
    }
  }

  if ( ! result ) {
    throw std::runtime_error( "Failed to read a matched line "
                              "from a stream." ); //x
  }

  return result;
}



/******************************************************************************
PURPOSE: readDescription - Read description line from a stream.
INPUTS:  FILE* file    Stream to read from.
OUTPUTS: FILE* file    Advanced stream.
         description_  Description read.
NOTES:   throws std::runtime_error if unable to read a valid description.
******************************************************************************/

void Dataset::readDescription( FILE* file ) {
  PRE0( file );
  enum { MAXIMUM_LENGTH = 255 };
  char line[ MAXIMUM_LENGTH + 1 ] = "";
  memset( line, 0, sizeof line );
  description_.clear();

  if ( fgets( line, MAXIMUM_LENGTH, file ) ) {
    const size_t length = strlen( line );

    if ( AND2( length > 0, line[ length - 1 ] == '\n' ) ) {
      line[ length - 1 ] = '\0'; // Erase '\n'.
      description_ = line;
      CHECK( ! strcmp( description_.c_str(), line ) );
    }
  }

  if ( ! isValidDescription() ) {
    throw std::runtime_error( "Failed to read valid dataset description."); //x
  }

  POST0( isValidDescription() );
}



/******************************************************************************
PURPOSE: readTimestamp - Read timestamp line from a stream.
INPUTS:  FILE* file      Stream to read from.
         const bool two  Read two timestamps?
OUTPUTS: FILE* file      Advanced stream.
         startingTimestamp_  1st timestamp read.
         endingTimestamp_    if two then 2nd timestamp read else same as 1st.
NOTES:   throws std::runtime_error if unable to read a valid description.
******************************************************************************/

void Dataset::readTimestamp( FILE* file, const bool two ) {
  PRE0( file );
  Timestamp::UTCString timestamp = "";
  memset( timestamp, 0, sizeof timestamp );
  bool ok = false;
  CHECK( sizeof timestamp / sizeof (char) >= 24 );

  if ( fscanf( file, "%24s", timestamp ) ) {

    if ( Timestamp::isUTCString( timestamp ) ) {
      startingTimestamp_.fromUTCString( timestamp );
      endingTimestamp_ = startingTimestamp_;

      if ( two ) {
        memset( timestamp, 0, sizeof timestamp );

        if ( fscanf( file, "%24s", timestamp ) ) {

          if ( Timestamp::isUTCString( timestamp ) ) {
            endingTimestamp_.fromUTCString( timestamp );
            ok = endingTimestamp_ >= startingTimestamp_;
          }
        }
      } else {
        ok = true;
      }
    }
  }

  // Read and discard end of line:

  if ( ok ) {
    skipLine( file ); //x
  } else {
    throw std::runtime_error( "Failed to read valid dataset timestamp." ); //x
  }

  timesteps_ = 1 + startingTimestamp_.hoursUntil( endingTimestamp_ );

  POST04( startingTimestamp_.invariant(),
          endingTimestamp_.invariant(),
          endingTimestamp_ >= startingTimestamp_,
          timesteps_ ==
            (size_t) 1 + startingTimestamp_.hoursUntil( endingTimestamp_ ) );
}



/******************************************************************************
PURPOSE: readDimensions - Read integer dimensions from a stream.
INPUTS:  FILE* file                  Stream to read from.
         const size_t count          Number of integers to read.
OUTPUTS: FILE* file                  Advanced stream.
         size_t dimensions[ count ]  Integer dimensions read.
NOTES:   throws std::runtime_error if unable to read a valid dimensions.
******************************************************************************/

void Dataset::readDimensions( FILE* file, const size_t count,
                              size_t dimensions[] ) {
  PRE03( file, count > 0, dimensions );
  memset( dimensions, 0, count * sizeof *dimensions );
  size_t limit = ULONG_MAX;
  bool ok = true;

  for ( size_t index = 0; AND2( ok, index < count ); ++index ) {
    enum { MAXIMUM_LENGTH = 23 };
    char word[ MAXIMUM_LENGTH + 1 ] = "";
    memset( word, 0, sizeof word );
    ok = fscanf( file, "%23s", word ) != 0;

    if ( ok ) {
      char* end = 0;
      dimensions[ index ] = strtoul( word, &end, 10 );
      ok = AND2( *end == '\0', IN_RANGE( dimensions[ index ], 1, limit ) );

      if ( ok ) {
        CHECK ( dimensions[ index ] > 0 );
        limit /= dimensions[ index ];
      }
    }
  }

  // Read and discard end of line:

  if ( ok ) {
    skipLine( file ); //x
  } else {
    throw std::runtime_error( "Failed to read valid dataset dimensions."); //x
  }

  POST0( emvl::minimum( dimensions, count ) > 0 );
}



/******************************************************************************
PURPOSE: readVariablesAndUnits - Read variable names and units from a stream
         and append them to variableNames_ and variableUnits_.
INPUTS:  FILE* file          Stream to read from.
         const size_t count  Number of name/unit pairs to read
                             or 0 if unknown.
OUTPUTS: FILE* file          Advanced stream.
         variables_          Increased by count.
         dataVariable_       Set to variables_ - 1.
         variableNames_      Variable names read and appended.
         variableUnits_          Unit names read and appended.
NOTES:   throws std::runtime_error if unable to read a valid dimensions.
******************************************************************************/

void Dataset::readVariablesAndUnits( FILE* file, const size_t count ) {

  PRE02( file,
         IMPLIES_ELSE( variables_, isValidVariableNamesAndUnits(),
                       IS_ZERO3( dataVariable_,
                                 variableNames_.size(),
                                 variableUnits_.size() ) ) );
  bool ok = true;
  size_t count2 = 0;

  // Read and skip line '# Variable names:'

  skipLine( file ); //x

  if ( count == 0 ) { // Read variable names line then determine count:
    enum { MAXIMUM_LINE_LENGTH = 255 };
    char line[ MAXIMUM_LINE_LENGTH + 1 ] = "";
    memset( line, 0, sizeof line );
    ok = fgets( line, MAXIMUM_LINE_LENGTH, file ) != 0;

    if ( ok ) {
      const size_t lineLength = strlen( line );
      size_t offset = 0;
      count2 = wordCount( line );
      ok = AND2( count2, line[ lineLength - 1 ] == '\n' );
      variableNames_.reserve( variableNames_.size() + count2 ); //x

      for ( size_t index = 0;
            AND3( ok, index < count2, offset < lineLength );
            ++index ) {
        enum { MAXIMUM_LENGTH = 63 };
        char word[ MAXIMUM_LENGTH + 1 ] = "";
        memset( word, 0, sizeof word );
        ok = sscanf( line + offset, "%63s", word ) == 1;

        if ( ok ) {
          variableNames_.push_back( word ); //x
          offset += strlen( word ) + 1;
        }
      }
    }
  } else { // Match 'count' number of variables:
    variableNames_.reserve( variableNames_.size() + count ); //x

    // Read variableNames_:

    for ( size_t index = 0; AND2( ok, index < count ); ++index ) {
      enum { MAXIMUM_LENGTH = 63 };
      char word[ MAXIMUM_LENGTH + 1 ] = "";
      memset( word, 0, sizeof word );
      ok = fscanf( file, "%63s", word ) == 1;

      if ( ok ) {
        variableNames_.push_back( word ); //x
      }
    }

    if ( ok ) { // Read and discard end of line:
      skipLine( file ); //x
      count2 = count;
    }
  }

  if ( ok ) { // Read and discard line '# Unit names:'
    skipLine( file ); //x
    variableUnits_.reserve( variableUnits_.size() + count2 ); //x
  }

  // Read variableUnits_:

  for ( size_t index = 0; AND2( ok, index < count2 ); ++index ) {
    enum { MAXIMUM_LENGTH = 63 };
    char word[ MAXIMUM_LENGTH + 1 ] = "";
    memset( word, 0, sizeof word );
    ok = fscanf( file, "%63s", word ) == 1;

    if ( ok ) {
      variableUnits_.push_back( word ); //x
    }
  }

  if ( ok ) {
    skipLine( file ); //x
    variables_ += count2;
    dataVariable_ = variableNames_.size() - 1;
  }

  if ( ! AND2( ok, isValidVariableNamesAndUnits() ) ) {
    throw std::runtime_error( "Failed to read valid "
                              "dataset variable names and units."); //x
  }

  POST05( (size_t) variables_ >= count,
          (size_t) variables_ == variableNames_.size(),
          (size_t) variables_ == variableUnits_.size(),
          dataVariable_ == variables_ - 1,
          isValidVariableNamesAndUnits() );
}



/******************************************************************************
PURPOSE: setEndingTimestamp - Set endingTimestamp_ based on startingTimestamp_,
         timesteps_ and timestepSize_.
INPUTS:  startingTimestamp_
         timesteps_
         timestepSize_
OUTPUTS: endingTimestamp_
******************************************************************************/

void Dataset::setEndingTimestamp() {
  PRE02( startingTimestamp_.invariant(), timesteps_ > 0 );

  endingTimestamp_ = startingTimestamp_;

  const size_t hoursPerTimestep =
    timestepSize_ == HOURS ? 1
    : timestepSize_ == DAYS ? 24
    : 0;

  if ( hoursPerTimestep ) {
    endingTimestamp_.offsetHours( ( timesteps_ - 1 ) * hoursPerTimestep );
    endingTimestamp_ = endingTimestamp_.endOfHour();

    if ( timestepSize_ == DAYS ) {
      endingTimestamp_ = endingTimestamp_.endOfDay();
    }
  } else {
    const int months =
      timestepSize_ == MONTHS ? timesteps_ - 1
        : timestepSize_ == TRIMONTHS ? 3 * timesteps_ - 1
        : timestepSize_ == YEARS ? 12 * timesteps_ - 1
      : 0;
    endingTimestamp_.offsetMonths( months );
    endingTimestamp_ = endingTimestamp_.endOfMonth();
  }

  POST02( endingTimestamp_.invariant(), endingTimestamp_ > startingTimestamp_);
}



/******************************************************************************
PURPOSE: setDatasetNameFromDescriptionAndVariable - Parse description and
         name of current variable and use to set name of dataset and also set
         timestepSize_.
INPUTS:  dataVariable_
         variableNames_  E.g., "co".
         description_    E.g., "regridded_MOZAIC_profile_co"
         isVector_
OUTPUTS: name_, timestepSize_
******************************************************************************/

void Dataset::setDatasetNameFromDescriptionAndVariable() {
  PRE03( isValidDescription(),
         dataVariable_ == variables_ - 1,
         isValidVariableNamesAndUnits() );
  const char* const description = description_.c_str();
  CHECK2( description, *description );
  const char* const comma = strchr( description, ',' );
  name_.clear();

  if ( comma ) {
    const char* source = comma + 1; // Skip comma.
    const char* const site = strstr( comma, ",SiteSubset" );
    const char* const acam = strstr( comma, ",acamserver" );
    const char* const hsrl = strstr( comma, ",hsrlserver" );
    const char* const hms = strstr( comma, ",hmsserver" );
    const char* const serverSource =
      acam ? "ACAM_" : hsrl ? "HSRL_" : hms ? "HMS_" : 0;
    const char* subset = strstr( comma, "Subset" );
    const char* const xdrconvert = strstr( comma, ",XDRConvert" );
    const char* const regridded = xdrconvert ? "regridded_" : "";
    const char* const variableName = variableNames_[ dataVariable_ ].c_str();

    if ( OR3( strstr( variableName, "DAILY" ),
              strstr( variableName, "daily" ),
              AND3( strstr( description, "CMAQSubset" ),
                    ! fileName_.empty(),
                    strstr( fileName_.c_str(), "_ddep_" ) ) ) ) {
      timestepSize_ = DAYS;
    } else if ( OR3( strstr( variableName, "MONTHLY" ),
                     strstr( variableName, "monthly" ),
                     AND3( strstr( description, "CMAQSubset" ),
                           ! fileName_.empty(),
                           strstr( fileName_.c_str(), "_mdep_" ) ) ) ) {
      timestepSize_ = MONTHS;
    } else if ( OR2( strstr( variableName, "SEASONAL" ),
                     strstr( variableName, "seasonal" ) ) ) {
      timestepSize_ = TRIMONTHS;
    } else if ( OR3( strstr( variableName, "YEARLY" ),
                     strstr( variableName, "yearly" ),
                     AND3( strstr( description, "CMAQSubset" ),
                           ! fileName_.empty(),
                           strstr( fileName_.c_str(), "_adep_" ) ) ) ) {
      timestepSize_ = YEARS;
    }

    if ( serverSource ) {
      source = serverSource;
      size_t size =
        strlen( regridded ) + variableNames_[ dataVariable_ ].size();
      size += strlen( source );
      name_.reserve( size ); //x

      if ( xdrconvert ) {
        name_ += regridded;
      }

      name_ += source;
    } else if ( subset ) {
      size_t size =
        strlen( regridded ) + variableNames_[ dataVariable_ ].size();

      if ( site ) { // Determine source of Site data:

        if ( strstr( description, "/airnowgateway.org/" ) ) {
          source = "Airnow_";
        } else if ( strstr( description, "www.airnowapi.org" ) ) {
          source = "Airnow2_";
        } else if ( strstr( description, "/aqsdatamart" ) ) {
          source = "AQS_";
        } else if ( strstr( description, "_faqsd," ) ) {
          source = "FAQSD_";
        } else if ( strstr( description, ".nesdis." ) ) {
          source = "GOESBB_";
        } else if ( strstr( description, "/uvnet/" ) ) {
          source = "UVNet_";
        } else if ( strstr( description, "_surfmet_" ) ) {
          source = "SURFMET_";
        }

        size += strlen( source );
        name_.reserve( size ); //x

        if ( xdrconvert ) {
          name_ += regridded;
        }

        name_ += source;
      } else {
        const size_t length = (size_t) ( subset - source );
        size += length + 1;
        name_.reserve( size ); //x

        if ( xdrconvert ) {
          name_ += regridded;
        }

        name_.append( source, length );
        name_ += '_'; // Append _ to separate from variable name.
      }
    }
  }

  if ( ! isVector_ ) {
    name_ += variableNames_[ dataVariable_ ];
  } else { // Compute vector name from matched part of last two variable names:
    CHECK( variables_ > 1 );
    const size_t index1 = variables_ - 2;
    const size_t index2 = index1 + 1;
    const size_t length = variableNames_[ index1 ].length();
    CHECK( variableNames_[ index2 ].length() == length );
    size_t first = 0;

    // Skip leading unmatched part of name (e.g., UWIND vs VWIND):

    while ( AND2( first < length,
                  variableNames_[ index1 ][ first ] !=
                  variableNames_[ index2 ][ first ] ) ) {
      ++first;
    }

    // Trim leading matched underscores (X_Wind => _Wind => Wind):

    while ( AND2( first < length, variableNames_[ index1 ][ first ] == '_' ) ) {
      ++first;
    }

    if ( first < length ) {
      size_t last = first;

      // Skip trailing unmatched part of name (e.g., Wind_X, Wind_Y):

      while ( AND2( last < length,
                    variableNames_[ index1 ][ last ] ==
                    variableNames_[ index2 ][ last ] ) ) {
        ++last;
      }

      size_t commonLength = last - first;
      CHECK2( commonLength, commonLength < length );

      // Trim ending matched underscores (eg., Wind_ => Wind):

      while ( AND2( commonLength > 1,
                    variableNames_[ index1 ][ commonLength - 1 ] == '_' ) ) {
        --commonLength;
      }

      CHECK2( commonLength, commonLength < length );
      name_.append( variableNames_[ index1 ], first, commonLength );
    }
  }

  POST0( isValidName() );
}



/******************************************************************************
PURPOSE: readRegridded64BitVariables - Read XDR regridded variable 64-bit data
         from a stream.
INPUTS:  FILE* stream                 Stream to read from.
         const size_t layersPerPoint  Number of layers per point.
OUTPUTS: data_
         minimums_
         maximums_
NOTES:   Throws exception upon failure to read valid data from a stream.
******************************************************************************/

void Dataset::readRegridded64BitVariables( FILE* stream,
                                           const size_t layersPerPoint ) {
  PRE06( stream,
         variables_ > 0,
         coordinates_.size() / cellVertexCount(),
         data_.size() == 0,
         minimums_.size() == 0,
         maximums_.size() == 0 );

  /* Data looks like this:

    # MSB 64-bit integers columns[points] and
    # MSB 64-bit integers rows[points] and
    # MSB 64-bit integers layers[points] and (OPTIONAL)
    # IEEE-754 64-bit reals data[points]:

    or this (if layersPerPoint > 1 ):

    # MSB 64-bit integers columns[timesteps][points] and
    # MSB 64-bit integers rows[timesteps][points] and
    # MSB 64-bit integers layers[timesteps][points][layers] and
    # IEEE-754 64-bit reals data[timesteps][points][layers]:

  */

  assert_static( AND2( sizeof (long long) == 8, sizeof (double) == 8 ) );
  const size_t totalPoints = coordinates_.size() / cellVertexCount();
  const size_t count = variables_ * totalPoints;
  const int integerVariables = 2 + ( layersPerPoint != 0 );
  data_.resize( count ); //x
  CHECK( data_.size() == count );
  bool ok = false;

  if ( layersPerPoint > 1 ) {
    const size_t surfacePoints = totalPoints / layersPerPoint;
    long long* const columns = (long long*) &data_[ 0 ];
    long long* const rows    = columns + totalPoints;
    const size_t restCount = count / 2;
    double* const rest = &data_[ restCount ];

    ok = fread( columns, sizeof data_[ 0 ], surfacePoints, stream )
         == surfacePoints;

    ok = AND2( ok,
               fread( rows, sizeof data_[ 0 ], surfacePoints, stream )
                 == surfacePoints );

    ok = AND2( ok,
               fread( rest, sizeof data_[0], restCount, stream ) == restCount );

    if ( ok ) { // Replicate columns and rows for all layers:
      reverse8ByteWordsIfLittleEndian( &data_[ 0 ], count );
      updateMissingValues( &data_[ 0 ], count );

      for (size_t point = surfacePoints, index = surfacePoints *layersPerPoint;
            point > 0; ) {
        --point;
        const long long column = columns[ point ];
        const long long row = rows[ point ];

        for ( size_t layer = 0; layer < layersPerPoint; ++layer ) {
          --index;
          CHECK( index < totalPoints * layersPerPoint );
          columns[ index ] = column;
          rows[ index ] = row;
        }
      }
    }
  } else {
    ok = fread( &data_[ 0 ], sizeof data_[ 0 ], count, stream ) == count;

    if ( ok ) {
      reverse8ByteWordsIfLittleEndian( &data_[ 0 ], count );
      updateMissingValues( &data_[ 0 ], count );
    }
  }

  if ( ok ) { // Convert/check data_:
    const long long* const idata = (const long long*) &data_[ 0 ];
    size_t dataIndex = 0;

    for ( int variable = 0; variable < variables_; ++variable) {

      for ( size_t point = 0; point < totalPoints; ++point, ++dataIndex ) {
        CHECK( dataIndex < data_.size() );

        if ( variable < integerVariables ) {

          // Check/convert integer column/row/layer indices

          const long long ivalue = idata[ dataIndex ];
          ok = ivalue > 0;

          if ( ok ) {
            const double dvalue = (double) ivalue;
            data_[ dataIndex ] = dvalue;
          }
        }

        const double value = data_[ dataIndex ];
        ok = AND2( ok, isFinite( value ) );

        if ( ! ok ) {
          point = totalPoints; // Stop looping.
          variable = variables_;
        }
      }
    }
  }

  if ( ok ) {
    computeVariableRanges(); //x
  }

  if ( ! AND4( ok, data_.size() == variables_ * totalPoints,
               isValidData(), isValidRange() ) ) {
    throw std::runtime_error("Failed to read valid regridded 64-bit data.");//x
  }

  POST03( data_.size() == variables_ * coordinates_.size() / cellVertexCount(),
          isValidData(), isValidRange() );
}



/******************************************************************************
PURPOSE: readRegridded32BitVariables - Read XDR regridded variable 32-bit data
         from a stream.
INPUTS:  FILE* stream  Stream to read from.
OUTPUTS: data_
         minimums_
         maximums_
NOTES:   Throws exception upon failure to read valid data from a stream.
******************************************************************************/

void Dataset::readRegridded32BitVariables( FILE* stream ) {
  PRE06( stream,
         variables_ >= 3,
         coordinates_.size() / cellVertexCount(),
         data_.size() == 0,
         minimums_.size() == 0,
         maximums_.size() == 0 );

  /* Data looks like this:
    # MSB 32-bit integers columns[timesteps][points] and
    # MSB 32-bit integers rows[timesteps][points] and
    # IEEE-754 32-bit reals data[timesteps][points]:
    or
    # MSB 32-bit integers columns[timesteps][points] and
    # MSB 32-bit integers rows[timesteps][points] and
    # MSB 32-bit integers layers[timesteps][points] and
    # IEEE-754 32-bit reals data[timesteps][points]:
  */

  assert_static( AND3( sizeof (int) == 4,
                       sizeof (float) == 4,
                       sizeof (double) == 8 ) );

  // Allocate, read and byte-swap all data:

  const size_t totalPoints = coordinates_.size() / cellVertexCount();
  const size_t count = variables_ * totalPoints;
  data_.resize( count ); //x
  CHECK( data_.size() == count );
  bool ok = false;

  if ( fread( &data_[ 0 ], 4, count, stream ) == count ) {
    reverse4ByteWordsIfLittleEndian( &data_[ 0 ], count );

    // Expand 32-bit words to 64-bit words in-place,
    // starting at the end and moving to the start:

    const size_t integerVariableCount = 2 + hasElevation_;
    const size_t realVariableCount = variables_ - integerVariableCount;
    const size_t integerValueCount = integerVariableCount * totalPoints;
    const size_t realValueCount    = realVariableCount    * totalPoints;
    int* const ip = (int*) &data_[ 0 ];
    int* idata = ip + integerValueCount;
    float* const fp = (float*) idata; // Real data appears after integer data.
    float* fdata = fp + realValueCount;
    double* ddata = &data_[ 0 ] + count; // Read from the end.
    ok = true;

    // Expand and store real words:

    for ( size_t point = 0; AND2( ok, point < realValueCount ); ++point ) {
      --fdata;
      double value = *fdata;

      if ( ! ( value > -9999.0 ) ) { // Update old missing to new missing.
        value = MISSING;
      }

      --ddata;
      *ddata = value;
      ok = isFinite( value );
    }

    // Expand and store integer words:

    for ( size_t point = 0; AND2( ok, point < integerValueCount ); ++point ) {
      --idata;
      const int ivalue = *idata;
      const double value = ivalue;
      --ddata;
      *ddata = value;
      ok = AND2( ivalue > 0, isFinite( value ) );
    }
  }

  if ( ok ) {
    computeVariableRanges(); //x
  }

  if ( ! AND3( ok, isValidData(), isValidRange() ) ) {
    throw std::runtime_error("Failed to read valid regridded 32-bit data.");//x
  }

  POST03( data_.size() == variables_ * coordinates_.size() / cellVertexCount(),
          isValidData(), isValidRange() );
}



/******************************************************************************
PURPOSE: writeRegridded64BitCoordinatesAndVariables - Write XDR regridded
         64-bit coordinates and variable data to a stream.
INPUTS:  FILE* stream          Stream to write to.
NOTES:   Throws exception upon failure.
******************************************************************************/

void Dataset::writeRegridded64BitCoordinatesAndVariables( FILE* stream) const {
  PRE( stream );

  /* Data looks like this:
  # IEEE-754 64-bit reals longitudes[points] and
  # IEEE-754 64-bit reals latitudes[points] and
  # IEEE-754 64-bit reals elevations[points] and (OPTIONAL)
  # MSB 64-bit integers columns[points] and
  # MSB 64-bit integers rows[points] and
  # MSB 64-bit integers layers[points] and (OPTIONAL)
  # IEEE-754 64-bit reals data[points]:
  */

  assert_static( AND2( sizeof (long long) == 8, sizeof (double) == 8 ) );
  std::vector<double> buffer( coordinates_.size() / cellVertexCount() ); //x
  writeCoordinates( stream, &buffer[ 0 ] ); //x
  long long* const ibuffer = (long long*) &buffer[ 0 ]; // Reuse buffer.
  assert_static( sizeof ibuffer[ 0 ] == sizeof buffer[ 0 ] );
  writeVariable( stream, 0, 1LL, LLONG_MAX, ibuffer ); //x
  writeVariable( stream, 1, 1LL, LLONG_MAX, ibuffer ); //x

  if ( hasElevation() ) {
    writeVariable( stream, 2, 1LL, LLONG_MAX, ibuffer ); //x
  }

  for ( int variable = variables_ - ( 1 + isVector_ );
        variable < variables_; ++variable ) {
    const double minimum = minimums_[ variable ];
    const double maximum = maximums_[ variable ];
    writeVariable( stream, variable, minimum, maximum, &buffer[ 0 ] ); //x
  }
}



/******************************************************************************
PURPOSE: writeRegridded32BitCoordinatesAndVariables - Write XDR regridded
         32-bit coordinates and variable data to a stream.
INPUTS:  FILE* stream  Stream to write to.
NOTES:   Throws exception upon failure.
******************************************************************************/

void Dataset::writeRegridded32BitCoordinatesAndVariables( FILE* stream) const {
  PRE( stream );

  /* Data looks like this:
  # MSB 32-bit integers points[timesteps] and
  # IEEE-754 32-bit reals longitudes[timesteps][points] and
  # IEEE-754 32-bit reals latitudes[timesteps][points] and
  # MSB 32-bit integers columns[timesteps][points] and
  # MSB 32-bit integers rows[timesteps][points] and
  # IEEE-754 32-bit reals data[timesteps][points]:
  */

  std::vector<float> buffer( coordinates_.size() / cellVertexCount() ); //x
  writeCoordinates( stream, &buffer[ 0 ] ); //x
  int* const ibuffer = (int*) &buffer[ 0 ]; // Reuse buffer.
  assert_static( sizeof ibuffer[ 0 ] <= sizeof buffer[ 0 ] );
  writeVariable( stream, 0, 1, INT_MAX, ibuffer ); //x
  writeVariable( stream, 1, 1, INT_MAX, ibuffer ); //x

  if ( AND2( variables_ > 3,
             ! strcmp( variableNames_[ 2 ].c_str(), "layer" ) ) ) {
    writeVariable( stream, 2, 1, INT_MAX, ibuffer ); //x
  }

  for ( int variable = variables_ - ( 1 + isVector_ );
       variable < variables_; ++variable ) {
    const float minimum = minimums_[ variable ];
    const float maximum = maximums_[ variable ];
    writeVariable( stream, variable, minimum, maximum, &buffer[ 0 ] ); //x
  }
}



/******************************************************************************
PURPOSE: writeCOARDSCoordinates - Write COARDS-format longitude
         latitude and optionally elevation data to file.
INPUTS:  const int file            NetCDF file to write to.
         const bool hasCorners     Write corner coordinates?
         const bool withElevation  Write elevation?
         const int coordinateRank  Number of coordinate dimensions.
         const int dimensions      Number of dimensions.
         const size_t dimensionSizes[ dimensions ]  Size of coordinate dims.
         const char* const dimensionNames[ dimensions ]  Nmes of dimensions.
NOTES:   Throws exception upon failure.
******************************************************************************/

void Dataset::writeCOARDSCoordinates( const int file,
                                      const bool hasCorners,
                                      const bool withElevation,
                                      const int coordinateRank,
                                      const int dimensions,
                                      const size_t dimensionSizes[],
                                    const char* const dimensionNames[]) const {
  PRE8( file > 0,
        IN3( coordinateRank, 1, 2 ),
        dimensions > 0,
        dimensionSizes[ 0 ] >= 1,
        dimensionSizes[ dimensions - 1 ] >= 1,
        dimensionNames,
        dimensionNames[ 0 ],
        dimensionNames[ dimensions - 1 ] );

  const size_t cells =
    coordinates_.size() ? coordinates_.size() / cellVertexCount()
    : layers() * rows() * columns();
  std::vector<double> buffer( cells ); //x
  const size_t* longitudeDimensionSizes = dimensionSizes;
  const size_t* latitudeDimensionSizes  = dimensionSizes;

  if ( dimensions > 1 ) {

    if ( ! strcmp( dimensionNames[ 0 ], "elevation" ) ) {
      longitudeDimensionSizes += 1; /* Skip elevation. */
      latitudeDimensionSizes  += 1; /* Skip elevation. */

      if ( AND2( coordinateRank == 1,
                 ! strcmp( dimensionNames[ 1 ], "latitude" ) ) ) {
        longitudeDimensionSizes += 1; /* Skip latitude. */
      }
    } else if ( ! strcmp( dimensionNames[ 0 ], "latitude" ) ) {

      if ( coordinateRank == 1 ) {
        longitudeDimensionSizes += 1; /* Skip latitude. */
      }
    }
  }

  // Write longitudes:

  copyOutLongitudes( buffer );
  bool ok =
    writeNetCDFAllDoubleData( file, "longitude", longitudeDimensionSizes,
                              &buffer[ 0 ] );

  if ( ok ) { // Write latitudes:
    copyOutLatitudes( buffer );

    if ( AND2( isStructuredGrid_, coordinateRank == 1 ) ) {
      const size_t columnCount = columns();
      const size_t rowCount = rows();

      for ( size_t row = 0, offset = 0; row < rowCount;
            ++row, offset += columnCount ) {
        buffer[ row ] = buffer[ offset ];
      }
    }

    ok = writeNetCDFAllDoubleData( file, "latitude", latitudeDimensionSizes,
                                   &buffer[ 0 ] );

    if ( AND2( ok, withElevation ) ) { // Write elevations
      copyOutElevations( buffer );
      ok = writeNetCDFAllDoubleData( file, "elevation", dimensionSizes,
                                     &buffer[ 0 ] );
    }
  }

  if ( hasCorners ) {
    const char* const coordinateNames[ 3 ][ 4 ] = {
      { "longitudeSW", "longitudeSE", "longitudeNW", "longitudeNE" },
      { "latitudeSW", "latitudeSE", "latitudeNW", "latitudeNE" },
      { "elevationSW", "elevationSE", "elevationNW", "elevationNE" }
    };
    const size_t* const coordinateDimensions[ 3 ] = {
      longitudeDimensionSizes,
      latitudeDimensionSizes,
      dimensionSizes
    };

    for ( int coordinate = 0, z = withElevation;
          AND2( ok, coordinate < 2 + z );
          ++coordinate ) {

      for ( int corner = 0; AND2( ok, corner < 4 ); ++corner ) {
        copyOutCornerCoordinate( coordinate, corner, buffer );
        ok =
          writeNetCDFAllDoubleData( file,
                                    coordinateNames[ coordinate ][ corner ],
                                    coordinateDimensions[ coordinate ],
                                    &buffer[ 0 ] );
      }
    }
  }

  if ( ! ok ) {
    throw std::runtime_error( "Failed to write COARDS-format coordinates.");//x
  }
}



/******************************************************************************
PURPOSE: copyOutCornerCoordinate - Copy corner coordinate from coordinates_.
INPUTS:  const int coordinate      LONGITUDE, LATITUDE or ELEVATION.
         const int corner          SW, SE, NW, NE.
OUTPUTS: std::vector<double>& result  Copy of coordinates_[*].coordinate.
******************************************************************************/

void Dataset::copyOutCornerCoordinate( const int coordinate,
                                       const int corner,
                                       std::vector<double>& result ) const {

  PRE3( IN4( coordinate, LONGITUDE, LATITUDE, ELEVATION ),
        IN5( corner, SW, SE, NW, NE ),
        result.size() >= coordinates_.size() / cellVertexCount() );

  const size_t vertices = cellVertexCount();
  const size_t points = coordinates_.size();

  if ( coordinate == LONGITUDE ) {

    for ( size_t point = 0, output = 0; point < points; point += vertices,
          ++output ) {
      const size_t index = point + corner;
      CHECK( index < coordinates_.size() );
      const double value = coordinates_[ index ].longitude;
      CHECK2( isLongitude( value ), output < result.size() );
      result[ output ] = value;
    }

  } else if ( coordinate == LATITUDE ) {

    for ( size_t point = 0, output = 0; point < points; point += vertices,
          ++output ) {
      const size_t index = point + corner;
      CHECK( index < coordinates_.size() );
      const double value = coordinates_[ index ].latitude;
      CHECK2( isLatitude( value ), output < result.size() );
      result[ output ] = value;
    }

  } else {
    CHECK( coordinate == ELEVATION );

    for ( size_t point = 0, output = 0; point < points; point += vertices,
          ++output ) {
      const size_t index = point + corner;
      CHECK( index < coordinates_.size() );
      const double value = coordinates_[ index ].elevation;
      CHECK2( isElevation( value ), output < result.size() );
      result[ output ] = value;
    }

  }
}



/******************************************************************************
PURPOSE: copyOutLongitudes - Copy longitude coordinate from coordinates_
         computing the cell center for multi-vertex cells.
INPUTS:  coordinates_
OUTPUTS: std::vector<double>& result  Copy of coordinates_[*].longitude.
******************************************************************************/

void Dataset::copyOutLongitudes( std::vector<double>& result ) const {
  PRE( result.size() >= coordinates_.size() / cellVertexCount() );
  const size_t vertices = cellVertexCount();
  const double factor = 1.0 / vertices;
  const size_t points = coordinates_.size();

  for ( size_t point = 0, output = 0; point < points; point += vertices,
        ++output ) {
    double longitude = 0.0;

    for ( size_t vertex = 0; vertex < vertices; ++vertex ) {
      const size_t index = point + vertex;
      CHECK( index < coordinates_.size() );
      longitude += coordinates_[ index ].longitude;
    }

    longitude *= factor; // Compute mean coordinate of cell vertices.
    CHECK2( isLongitude( longitude ), output < result.size() );
    result[ output ] = longitude;
  }
}



/******************************************************************************
PURPOSE: copyOutLatitudes - Copy latitude coordinate from coordinates_
         computing the cell center for multi-vertex cells.
INPUTS:  coordinates_
OUTPUTS: std::vector<double>& result  Copy of coordinates_[*].latitude.
******************************************************************************/

void Dataset::copyOutLatitudes( std::vector<double>& result ) const {
  PRE( result.size() >= coordinates_.size() / cellVertexCount() );
  const size_t vertices = cellVertexCount();
  const double factor = 1.0 / cellVertexCount();
  const size_t points = coordinates_.size();

  for ( size_t point = 0, output = 0; point < points; point += vertices,
        ++output ) {
    double latitude = 0.0;

    for ( size_t vertex = 0; vertex < vertices; ++vertex ) {
      const size_t index = point + vertex;
      CHECK( index < coordinates_.size() );
      latitude += coordinates_[ index ].latitude;
    }

    latitude *= factor; // Compute mean coordinate of cell vertices.
    CHECK2( isLatitude( latitude ), output < result.size() );
    result[ output ] = latitude;
  }
}



/******************************************************************************
PURPOSE: copyOutElevations - Copy elevation coordinate from coordinates_
         computing the cell center for multi-vertex cells.
INPUTS:  coordinates_
OUTPUTS: std::vector<double>& result  Copy of coordinates_[*].elevation.
******************************************************************************/

void Dataset::copyOutElevations( std::vector<double>& result ) const {
  PRE( result.size() >= coordinates_.size() / cellVertexCount() );
  const size_t vertices = cellVertexCount();
  const double factor = 1.0 / cellVertexCount();
  const size_t points = coordinates_.size();

  for ( size_t point = 0, output = 0; point < points; point += vertices,
        ++output ) {
    double elevation = 0.0;

    for ( size_t vertex = 0; vertex < vertices; ++vertex ) {
      const size_t index = point + vertex;
      CHECK( index < coordinates_.size() );
      elevation += coordinates_[ index ].elevation;
    }

    elevation *= factor; // Compute mean coordinate of cell vertices.
    CHECK2( isElevation( elevation ), output < result.size() );
    result[ output ] = elevation;
  }
}



/******************************************************************************
PURPOSE: writeCOARDSVariables - Write COARDS-format variable data to file.
INPUTS:  const int file  NetCDF file to write to.
         const int skipVariables      Number of beginning data variables to
                                      skip / ignore / not write.
         const int integerVariables   Number of (remaining) data variables
                                      to write as integer data or 0.
         const size_t dimensionSizes[]   Size of each dimension.
NOTES:   Throws exception upon failure.
******************************************************************************/

void Dataset::writeCOARDSVariables( const int file,
                                    const int skipVariables,
                                    const int integerVariables,
                                    const size_t dimensionSizes[] ) const {
  PRE4( file > 0,
        IN_RANGE( skipVariables, 0, variables_ ),
        IN_RANGE( integerVariables, 0, variables_ ),
        dataFileOffset_ == 0 );

  if ( skipVariables < variables_ ) {
    const size_t count = data_.size() / variables_;
    std::vector<double> buffer( count ); //x
    int* const ibuffer = (int*) &buffer[ 0 ];
    bool ok = true;

    for ( int variable = skipVariables;
          AND2( ok, variable < variables_ );
          ++variable ) {
      CHECK( IN_RANGE( variable, 0, variables_ - 1 ) );
      const char* const name = variableNames_[ variable ].c_str();

      if ( variable - skipVariables < integerVariables ) {
        const size_t variableOffset = variable * count;
        copyArray( &data_[ variableOffset ], count, 1, INT_MAX, ibuffer );
        ok = writeNetCDFAllIntData( file, name, dimensionSizes, ibuffer );
      } else {
        const size_t offset = variable * count;
        const double* const variableData = &data_[ offset ];
        ok = writeNetCDFAllDoubleData( file, name, dimensionSizes,
                                       variableData );
      }
    }

    if ( ! ok ) {
      throw std::runtime_error("Failed to write COARDS-format variables."); //x
    }
  }
}



/******************************************************************************
PURPOSE: writeIOAPIVariables - Write IOAPI-format variable data to file.
INPUTS:  const int file  NetCDF file to write to.
         const int skipVariables      Number of beginning data variables to
                                      skip / ignore / not write.
         const int integerVariables   Number of (remaining) data variables
                                      to write as integer data or 0.
         const size_t dimensionSizes[]   Size of each dimension.
NOTES:   Throws exception upon failure.
******************************************************************************/

void Dataset::writeIOAPIVariables( const int file,
                                   const int skipVariables,
                                   const int integerVariables,
                                   const size_t dimensionSizes[] ) const {
  PRE4( file > 0,
        IN_RANGE( skipVariables, 0, variables_ ),
        IN_RANGE( integerVariables, 0, variables_ ),
        dataFileOffset_ == 0 );

  if ( skipVariables < variables_ ) {
    const size_t count = data_.size() / variables_;
    std::vector<float> buffer( count ); //x
    int* const ibuffer = (int*) &buffer[ 0 ];
    bool ok = true;

    for ( int variable = skipVariables;
          AND2( ok, variable < variables_ );
          ++variable ) {
      CHECK( IN_RANGE( variable, 0, variables_ - 1 ) );
      const char* const name = variableNames_[ variable ].c_str();
      const size_t variableOffset = variable * count;

      if ( variable - skipVariables < integerVariables ) {
        copyArray( &data_[ variableOffset ], count, 1, INT_MAX, ibuffer );
        ok = writeNetCDFAllIntData( file, name, dimensionSizes, ibuffer );
      } else {
        const float minimum = MISSING;
        const float maximum = this->maximum( variable );
        copyArray( &data_[ variableOffset ], count, minimum, maximum,
                   &buffer[ 0 ] );
        ok = writeNetCDFAllFloatData( file, name, dimensionSizes, &buffer[0] );
      }
    }

    if ( ! ok ) {
      throw std::runtime_error("Failed to write IOAPI-format variables."); //x
    }
  }
}



/******************************************************************************
PURPOSE: writeBufferedCOARDSVariables - Write COARDS-format variable data to
         file using buffering from fileName_.
INPUTS:  const int file  NetCDF file to write to.
         const int skipVariables      Number of beginning data variables to
                                      skip / ignore / not write.
         const int integerVariables   Number of (remaining) data variables
                                      to write as integer data or 0.
         const int dimensionIds[]     Dimension ids.
         const size_t dimensionSizes[]   Size of each dimension.
         const std::vector<Timestamp>& timestamps  timestamps to use.
NOTES:   Throws exception upon failure.
******************************************************************************/

void Dataset::writeBufferedCOARDSVariables( const int file,
                                            const int skipVariables,
                                            const int integerVariables,
                                            const int dimensions,
                                            const size_t dimensionSizes[],
                                            const std::vector<Timestamp>&
                                              timestamps ) const {
  PRE11( file > 0,
         IN_RANGE( skipVariables, 0, variables_ ),
         IN_RANGE( integerVariables, 0, variables_ ),
         IN_RANGE( dimensions, 1, 4 ),
         emvl::minimum( dimensionSizes, dimensions ) >= 1,
         timestamps.size() == (size_t) dimensionSizes[ 0 ],
         dataFileOffset_ > 0,
         ! fileName_.empty(),
         timesteps_ > BUFFERED_DATA_TIMESTEPS,
         timesteps_ % BUFFERED_DATA_TIMESTEPS == 0,
         data_.size() % BUFFERED_DATA_TIMESTEPS == 0 );

  if ( skipVariables < variables_ ) {
    //const size_t count = data_.size() / variables_ / timesteps_;
    const size_t count = data_.size() / variables_ / BUFFERED_DATA_TIMESTEPS;
    std::vector<double> buffer( count ); //x 1 variable, 1 timestep of data_.
    int* const ibuffer = (int*) &buffer[ 0 ];
    const size_t sizes[ 4 ] = {
      1,
      dimensions > 1 ? dimensionSizes[ 1 ] : 1,
      dimensions > 2 ? dimensionSizes[ 2 ] : 1,
      dimensions > 3 ? dimensionSizes[ 3 ] : 1
    };
    bool ok = true;
    const size_t timesteps = timestamps.size();

    for ( size_t timestep = 0; AND2( ok, timestep < timesteps ); ++timestep ) {
      const Timestamp& timestamp( timestamps[ timestep ] );
      ok = loadedDataSubset( timestamp, timestamp );

      for ( int variable = skipVariables;
            AND2( ok, variable < variables_ ); ++variable ) {
        CHECK( IN_RANGE( variable, 0, variables_ - 1 ) );
        const char* const name = variableNames_[ variable ].c_str();
        const size_t variableOffset =
          variable * BUFFERED_DATA_TIMESTEPS * count;
        CHECK( variableOffset <= data_.size() - count );

        if ( variable - skipVariables < integerVariables ) {
          copyArray( &data_[ variableOffset ], count, 1, INT_MAX, ibuffer );
          ok = writeNetCDFSomeIntData( file, name, timestep, sizes, ibuffer );
        } else {
          ok = writeNetCDFSomeDoubleData( file, name, timestep, sizes,
                                          &data_[ variableOffset ] );
        }
      }
    }

    if ( ! ok ) {
      throw std::runtime_error("Failed to write COARDS-format variables."); //x
    }
  }
}



/******************************************************************************
PURPOSE: writeBufferedIOAPISVariables - Write IOAPI-format variable data to
         file using buffering from fileName_.
INPUTS:  const int file  NetCDF file to write to.
         const int skipVariables      Number of beginning data variables to
                                      skip / ignore / not write.
         const int integerVariables   Number of (remaining) data variables
                                      to write as integer data or 0.
         const int dimensions         Number of dimensions.
         const sizes_t dimensionSizes[]   Size of each dimension.
         const std::vector<Timestamp>& timestamps  timestamps to use.
NOTES:   Throws exception upon failure.
******************************************************************************/

void Dataset::writeBufferedIOAPIVariables( const int file,
                                           const int skipVariables,
                                           const int integerVariables,
                                           const int dimensions,
                                           const size_t dimensionSizes[],
                                           const std::vector<Timestamp>&
                                              timestamps ) const {
  PRE11( file > 0,
         IN_RANGE( skipVariables, 0, variables_ ),
         IN_RANGE( integerVariables, 0, variables_ ),
         dimensions == 4,
         emvl::minimum( dimensionSizes, dimensions ) >= 1,
         timestamps.size() == (size_t) dimensionSizes[ 0 ],
         dataFileOffset_ > 0,
         ! fileName_.empty(),
         timesteps_ > BUFFERED_DATA_TIMESTEPS,
         timesteps_ % BUFFERED_DATA_TIMESTEPS == 0,
         data_.size() % BUFFERED_DATA_TIMESTEPS == 0 );

  if ( skipVariables < variables_ ) {
    //const size_t count = data_.size() / variables_ / timesteps_;
    const size_t count = data_.size() / variables_ / BUFFERED_DATA_TIMESTEPS;
    std::vector<float> buffer( count ); //x 1 variable, 1 timestep of data_.
    int* const ibuffer = (int*) &buffer[ 0 ];
    const size_t sizes[ 4 ] = {
      1,
      dimensionSizes[ 1 ],
      dimensionSizes[ 2 ],
      dimensionSizes[ 3 ]
    };
    bool ok = true;
    const size_t timesteps = timestamps.size();

    for ( size_t timestep = 0; AND2( ok, timestep < timesteps ); ++timestep ) {
      const Timestamp& timestamp( timestamps[ timestep ] );
      ok = loadedDataSubset( timestamp, timestamp );

      for ( int variable = skipVariables;
            AND2( ok, variable < variables_ ); ++variable ) {
        CHECK( IN_RANGE( variable, 0, variables_ - 1 ) );
        const char* const name = variableNames_[ variable ].c_str();
        const size_t variableOffset =
          variable * BUFFERED_DATA_TIMESTEPS * count;
        CHECK( variableOffset <= data_.size() - count );

        if ( variable - skipVariables < integerVariables ) {
          copyArray( &data_[ variableOffset ], count, 1, INT_MAX, ibuffer );
          ok = writeNetCDFSomeIntData( file, name, timestep, sizes, ibuffer );
        } else {
          const float minimum = MISSING;
          const float maximum = this->maximum( variable );
          copyArray( &data_[ variableOffset ], count, minimum, maximum,
                     &buffer[ 0 ] );
          ok = writeNetCDFSomeFloatData( file, name, timestep, sizes,
                                         &buffer[ 0 ] );
        }
      }
    }

    if ( ! ok ) {
      throw std::runtime_error("Failed to write IOAPI-format variables."); //x
    }
  }
}



/******************************************************************************
PURPOSE: createCOARDSVariables - Create definitions of COARDS-format
         coordinates, integer variables and time variables and data variables
         in file.
INPUTS:  const int file               NetCDF file to update.
         const bool isRegridded       Create variables for column, row, layer?
         const bool hasCorners        Create variables for corner coordinates?
         const bool withElevation     Create elevation?
         const int coordinateRank     Number of data variables to skip.
         const int skipVariables      Number of data variables to skip.
         const int dimensions         Number of dimensions for each variable.
         const int dimensionIds[]     Dimension ids.
         const int dimensionNames[]   Dimension names.
NOTES:   Throws exception upon failure.
******************************************************************************/

void Dataset::createCOARDSVariables( const int file,
                                     const bool isRegridded,
                                     const bool hasCorners,
                                     const bool withElevation,
                                     const int coordinateRank,
                                     const int skipVariables,
                                     const int dimensions,
                                     const int dimensionIds[],
                                     const char* const dimensionNames[]) const {
  PRE8( file > 0,
        IN3( coordinateRank, 1, 2 ),
        IN_RANGE( skipVariables, 0, variables_ ),
        dimensions > 0,
        emvl::minimum( dimensionIds, dimensions ) >= 0,
        dimensionNames,
        dimensionNames[ 0 ],
        dimensionNames[ dimensions - 1 ] );

  const int skipTimeDimension = ! strcmp( dimensionNames[ 0 ], "time" );
  const int* coordinateDimensionIds = dimensionIds + skipTimeDimension;
  const char* const * coordinateDimensionNames =
    dimensionNames + skipTimeDimension;

  // Create definitions of coordinate variables:

  bool ok =
    createNetCDFCOARDSCoordinates( file, hasCorners, withElevation,
                                   coordinateRank,
                                   dimensions - skipTimeDimension,
                                   coordinateDimensionIds,
                                   coordinateDimensionNames );

  // Create definitions of integer variables (from regridding):

  if ( isRegridded ) {
    ok = createNetCDFVariable( file, "column", "-", NC_INT, 0,
                               1, dimensionIds ) != -1;

    if ( ok ) {
      ok = createNetCDFVariable( file, "row", "-", NC_INT, 0,
                                 1, dimensionIds ) != -1;

      if ( ok ) {

        if ( hasElevation() ) {
          ok = createNetCDFVariable( file, "layer", "-", NC_INT, 0,
                                     1, dimensionIds ) != -1;
        }
      }
    }
  }

  if ( ok ) {
    ok = createNetCDFVariable( file, "yyyymmdd", "yyyymmdd", NC_INT, 0,
                               1, &dimensionIds[ 0 ] ) != -1;

    if ( ok ) {
      ok = createNetCDFVariable( file, "hhmmss", "hhmmss", NC_INT, 0,
                                 1, &dimensionIds[ 0 ] ) != -1;
    }
  }

  // Create definitions of remaining (non-integer) data variables:

  const int integerVariables = isRegridded * ( 2 + hasElevation() );

  for ( int variable = skipVariables + integerVariables;
        AND2( ok, variable < variables_ );
        ++variable ) {
    const char* const name = variableNames_[ variable ].c_str();
    const char* const units =  variableUnits_[ variable ].c_str();
    const int id =
      createNetCDFVariable( file, name, units, NC_DOUBLE, true,
                            dimensions, dimensionIds );

    if ( id != -1 ) {
      ok = writeNetCDFTextAttribute( file, id, "grid_mapping", "crs" );
    }
  }

  if ( ! ok ) {
    throw std::runtime_error( "Failed to create COARDS-format variables."); //x
  }
}



/******************************************************************************
PURPOSE: writeCOARDSCommon - Create definitions of common COARDS-format
         variables and attributes and write coordinates, data variables and
         auxiliary variables in file.
INPUTS:  const int file               NetCDF file to update.
         const int skipVariables      Number of data variables to skip.
         const bool isRegridded       Create variables for column, row, layer?
         const bool hasCorners        Create variables for corner coordinates?
         const bool withElevation     Create elevation variable?
         const bool writeCoordinatesAndData  Write coordinates and data values?
         const int coordinateRank     Number of dimensions for each coordinate.
         const int dimensions         Number of dimensions for each variable.
         const int dimensionIds[]     Dimension ids.
         const size_t dimensionSizes[]   Size of each dimension.
         const int dimensionNames[]   Name of each dimension.
         const std::vector<Timestamp>& timestamps  Timestamps to write.
NOTES:   Throws exception upon failure. Ends definition of NetCDF file header.
******************************************************************************/

void Dataset::writeCOARDSCommon( const int file,
                                 const int skipVariables,
                                 const bool isRegridded,
                                 const bool hasCorners,
                                 const bool withElevation,
                                 const bool writeCoordinatesAndData,
                                 const int coordinateRank,
                                 const int dimensions,
                                 const int dimensionIds[],
                                 const size_t dimensionSizes[],
                                 const char* const dimensionNames[],
                                 const std::vector<Timestamp>&
                                   timestamps ) const {

  PRE15( file > 0,
         IN_RANGE( skipVariables, 0, variables_ ),
         IMPLIES( isRegridded, AND2( skipVariables == 0, ! hasCorners ) ),
         IMPLIES( hasCorners, ! isRegridded ),
         IN3( coordinateRank, 1, 2 ),
         IN_RANGE( dimensions, 1, 4 ),
         dimensionIds,
         emvl::minimum( dimensionIds, dimensions ) >= 0,
         dimensionSizes,
         emvl::minimum( dimensionSizes, dimensions ) >= 1,
         dimensionNames,
         dimensionNames[ 0 ],
         dimensionNames[ dimensions - 1 ],
         timestamps.size() == (size_t) dimensionSizes[ 0 ],
         startingTimestamp_ <= timestamps[ 0 ] );

  createCOARDSVariables( file, isRegridded, hasCorners, withElevation,
                         coordinateRank, skipVariables, dimensions,
                         dimensionIds, dimensionNames ); //x

  const double domain[ 2 ][ 2 ] = {
    { bounds_.west(), bounds_.east() },
    { bounds_.south(), bounds_.north() }
  };
  Timestamp::UTCString startDateTime = "";
  startingTimestamp_.toUTCString( startDateTime );
  std::string history;
  history.reserve( 255 ); //x
  history += description_;
  history += ",RSIG3D";

  const size_t timesteps = timestamps.size();
  const long long seconds0 = startingTimestamp_.seconds();
  std::vector<float> hours( timesteps ); //x
  enum { SECONDS_PER_HOUR = 60 * 60 };

  for ( size_t timestep = 0; timestep < timesteps; ++timestep ) {
    CHECK( timestep < timestamps.size() );
    const Timestamp& timestamp = timestamps[ timestep ];
    const long long seconds = timestamp.seconds();
    const long long secondsDifference = seconds - seconds0;
    CHECK( secondsDifference >= 0 );
    const long long hoursDifference = secondsDifference / SECONDS_PER_HOUR;
    const float h = (float) hoursDifference;
    CHECK( timestep < hours.size() );
    hours[ timestep ] = h;
  }

  const bool ok =
    writeNetCDFCOARDSContents( file, domain, history.c_str(), startDateTime,
                               dimensions, dimensionIds, dimensionSizes,
                               &hours[ 0 ] );

  if ( ok ) {
    writeCOARDSDateTime( file, timestamps ); //x

    if ( writeCoordinatesAndData ) {
      const int skipTimeDimension =
        ! strcmp( dimensionNames[ 0 ], "time" ) ? 1 : 0;
      const int skipElevationDimension =
        AND3( ! withElevation,
              dimensions > 1,
              ! strcmp( dimensionNames[1], "elevation" ) ) ? 1
        : 0;
      const int skipDimensions = skipTimeDimension + skipElevationDimension;
      const int coordinateDimensions = dimensions - skipDimensions;
      const size_t* coordinateDimensionSizes = dimensionSizes + skipDimensions;
      const char* const * coordinateDimensionNames =
        dimensionNames + skipDimensions;
      writeCOARDSCoordinates( file, hasCorners, withElevation, coordinateRank,
                              coordinateDimensions,
                              coordinateDimensionSizes,
                              coordinateDimensionNames ); //x
      const int integerVariables = isRegridded * ( 2 + hasElevation() );

      if ( dataFileOffset_ ) {
        writeBufferedCOARDSVariables( file, skipVariables, integerVariables,
                                      dimensions, dimensionSizes,
                                      timestamps );//x
      } else {
        writeCOARDSVariables( file, skipVariables, integerVariables,
                              dimensionSizes ); //x
      }
    }
  }

  if ( ! ok ) {
    throw std::runtime_error("Failed to write COARDS-format common data."); //x
  }

  POST( true );
}



/******************************************************************************
PURPOSE: writeIOAPIHeader - Create definitions of common IOAPI-format
         dimensions, variables and attributes in file header.
INPUTS:  const int file               NetCDF file to write to.
         const bool andElevation      Create elevation variable definition?
         const bool forceUppercase    Ensure variable names are all capitals?
         const int skipVariables      Number of data variables to skip.
         const int layers             Number of grid layers.
         const int rows               Number of grid rows.
         const int columns            Number of grid columns.
         const CMAQGridParameters& gridParameters  Unsubsetted parameters.
         const size_t subsetIndices[ 4 ][ 2 ]  Optional subset grid indices:
                                          [TIME,LAYER,ROW,COLUMN][FIRST,LAST].
         const MapProjection* mapProjection  Map projcetion or 0 if lon-lat.
OUTPUTS: int dimensionIds[ 6 ]        NetCDF dimension ids:
                          dimensionIds[ TSTEP, DATE-TIME, LAY, VAR, ROW, COL ]
NOTES:   Throws exception upon failure. Ends definition of NetCDF file header.
******************************************************************************/

void Dataset::writeIOAPIHeader( const int file,
                                const bool andElevation,
                                const bool forceUppercase,
                                const int skipVariables,
                                const int layers,
                                const int rows,
                                const int columns,
                                const CMAQGridParameters& gridParameters,
                                const size_t subsetIndices[ 4 ][ 2 ],
                                const MapProjection* mapProjection,
                                int dimensionIds[ 6 ] ) const {

  PRE9( file > 0,
        skipVariables >= 0,
        layers > 0,
        rows > 0,
        columns > 0,
        gridParameters.invariant(),
        IMPLIES( subsetIndices,
                 AND11( subsetIndices[ TIME   ][ LAST  ] >=
                          subsetIndices[ TIME   ][ FIRST],
                        subsetIndices[ LAYER  ][ FIRST ] >= 1,
                        subsetIndices[ LAYER  ][ LAST  ] >=
                          subsetIndices[ LAYER ][ FIRST ],
                        subsetIndices[ ROW    ][ FIRST ] >= 1,
                        subsetIndices[ ROW    ][ LAST  ] >=
                          subsetIndices[ ROW   ][ FIRST ],
                        subsetIndices[ COLUMN ][ FIRST ] >= 1,
                        subsetIndices[ COLUMN ][ LAST  ] >=
                          subsetIndices[ COLUMN ][ FIRST ],
                        timesteps_ == 1 +
                          subsetIndices[ TIME ][ LAST ] -
                          subsetIndices[ TIME ][ FIRST ],
                        layers == (int) ( 1 +
                          subsetIndices[ LAYER ][ LAST ] -
                          subsetIndices[ LAYER ][ FIRST] ),
                        rows == (int) ( 1 +
                          subsetIndices[ ROW ][ LAST ] -
                          subsetIndices[ ROW ][ FIRST ] ),
                        columns == (int) ( 1 +
                          subsetIndices[ COLUMN ][ LAST ] -
                          subsetIndices[ COLUMN ][ FIRST ] ) ) ),
        IMPLIES( mapProjection, mapProjection->invariant() ),
        dimensionIds );

  enum { TSTEP, DATE_TIME, LAY, VAR, ROW_DIM, COL, DIMENSIONS };
  const char* const names[ DIMENSIONS ] = {
    "TSTEP", "DATE-TIME", "LAY", "VAR", "ROW", "COL"
  };
  const size_t outputVariables =
    2 + ( andElevation ? 1 : 0 ) + variables_ - skipVariables;
  const size_t dimensions[ DIMENSIONS ] = {
    timesteps_, 2, (size_t) layers, outputVariables,
    (size_t) rows, (size_t) columns
  };
  bool ok = false;

  // Initialize output:

  dimensionIds[ 0 ] = -1;
  dimensionIds[ 1 ] = -1;
  dimensionIds[ 2 ] = -1;
  dimensionIds[ 3 ] = -1;
  dimensionIds[ 4 ] = -1;
  dimensionIds[ 5 ] = -1;

  if ( createNetCDFDimensions( file, DIMENSIONS, names, dimensions,
                               dimensionIds ) ) {

    // Create copy of variable names and units for NetCDF C API:

    std::vector<const char*> variableNames;
    std::vector<const char*> variableUnits;
    const size_t dataVariables = variables_ - skipVariables;
    variableNames.resize( dataVariables ); //x
    variableUnits.resize( dataVariables ); //x
    CHECK3( variableNames.size(),
            variableNames.size() == dataVariables,
            variableNames.size() == variableUnits.size() );

    for ( size_t variable = 0; variable < dataVariables; ++variable ) {
      const size_t readIndex = variable + skipVariables;
      variableNames[ variable ] = variableNames_[ readIndex ].c_str();
      variableUnits[ variable ] = variableUnits_[ readIndex ].c_str();
      CHECK4( variableNames[ variable ], variableNames[ variable ][ 0 ],
              variableUnits[ variable ], variableUnits[ variable ][ 0 ] );
    }

    if ( createNetCDFIOAPIVariables( file, dimensionIds, andElevation ? 1 : 0,
                                     forceUppercase ? 1 : 0,
                                     dataVariables,
                                     &variableNames[0], &variableUnits[0] ) ) {

      writeIOAPIAttributes( file, andElevation, forceUppercase, skipVariables,
                            gridParameters, subsetIndices ); //x
      ok = endNetCDFHeaderDefinition( file );
    }
  }

  if ( ! ok ) {
    throw std::runtime_error( "Failed to write IOAPI-format header." ); //x
  }

  POST( true );
}



/******************************************************************************
PURPOSE: writeIOAPIAttributes - Create definitions of common IOAPI-format
         dimensions, variables and attributes in file header.
INPUTS:  const int file               NetCDF file to write to.
         const bool andElevation      Include ELEVATION variable?
         const bool forceUppercase    Ensure variable names are all capitals?
         const int skipVariables      Number of variables to skip.
         const CMAQGridParameters& gridParameters  Unsubsetted parameters.
         const size_t subsetIndices[ 4 ][ 2 ]  Optional subset grid indices:
                                          [TIME,LAYER,ROW,COLUMN][FIRST,LAST].
NOTES:   Throws exception upon failure.
******************************************************************************/

void Dataset::writeIOAPIAttributes( const int file,
                                    const bool andElevation,
                                    const bool forceUppercase,
                                    const int skipVariables,
                                    const CMAQGridParameters& gridParameters,
                                    const size_t subsetIndices[4][2] ) const {

  PRE4( file > 0,
        skipVariables >= 0,
        gridParameters.invariant(),
        IMPLIES( subsetIndices,
                 AND8( subsetIndices[ TIME   ][ LAST  ] >=
                         subsetIndices[ TIME   ][ FIRST],
                       subsetIndices[ LAYER  ][ FIRST ] >= 1,
                       subsetIndices[ LAYER  ][ LAST  ] >=
                         subsetIndices[ LAYER ][ FIRST ],
                       subsetIndices[ ROW    ][ FIRST ] >= 1,
                       subsetIndices[ ROW    ][ LAST  ] >=
                         subsetIndices[ ROW   ][ FIRST ],
                       subsetIndices[ COLUMN ][ FIRST ] >= 1,
                       subsetIndices[ COLUMN ][ LAST  ] >=
                         subsetIndices[ COLUMN ][ FIRST ],
                       timesteps_ == 1 +
                         subsetIndices[ TIME ][ LAST ] -
                         subsetIndices[ TIME ][ FIRST ] ) ) );

  enum { NAMLEN3 = 16, MXDESC3 = 60, MXDLEN3 = 80 }; // IOAPI Spec.
  bool ok = false;

  /* NetCDF IOAPI header global attributes look like this:

		IOAPI_VERSION = "1.0 1997349 (Dec. 15, 1997)" ;
		EXEC_ID = "????????????????                                                                " ;
		FTYPE = 1 ;
		CDATE = 2005160 ;
		CTIME = 213523 ;
		WDATE = 2005160 ;
		WTIME = 213523 ;
		SDATE = 2002113 ;
		STIME = 0 ;
		TSTEP = 10000 ;
		NTHIK = 1 ;
		NCOLS = 4 ;
		NROWS = 5 ;
		NLAYS = 3 ;
		NVARS = 2 ;
		GDTYP = 2 ;
		P_ALP = 30. ;
		P_BET = 60. ;
		P_GAM = -100. ;
		XCENT = -100. ;
		YCENT = 40. ;
		XORIG = 1578000. ;
		YORIG = -1270000. ;
		XCELL = 2000. ;
		YCELL = 2000. ;
		VGTYP = 2 ;
		VGTOP = 10000.f ;
		VGLVLS = 1.f, 0.995f, 0.99f, 0.985f ;
		GDNAM = "M_02_99BRACE    " ;
		UPNAM = "                " ;
		VAR-LIST = "O3              NO              " ;
		FILEDESC = "Concentration file output From CMAQ model dyn alloc version...                                                                                                                   
    HISTORY = ""
  */

  ok = writeNetCDFTextAttribute( file, NC_GLOBAL, "IOAPI_VERSION",
                                 "1.0 1997349 (Dec. 15, 1997)" ) == 1;

  const char* const unknownString =
    "????????????????                                                                ";
  ok = AND2( ok,
             writeNetCDFTextAttribute( file, NC_GLOBAL, "EXEC_ID",
                                       unknownString ) == 1 );
  ok = AND2( ok, writeNetCDFIntAttribute( file, "FTYPE", 1 ) == 1 );
  const Timestamp now; // Current time UTC.
  int yyyyddd = now.yyyyddd();
  ok = AND2( ok, writeNetCDFIntAttribute( file, "CDATE", yyyyddd ) == 1 );
  int hhmmss = now.hhmmss();
  ok = AND2( ok, writeNetCDFIntAttribute( file, "CTIME", hhmmss ) == 1 );
  ok = AND2( ok, writeNetCDFIntAttribute( file, "WDATE", yyyyddd ) == 1 );
  ok = AND2( ok, writeNetCDFIntAttribute( file, "WTIME", hhmmss ) == 1 );
  yyyyddd = startingTimestamp_.yyyyddd();
  ok = AND2( ok, writeNetCDFIntAttribute( file, "SDATE", yyyyddd ) == 1 );
  hhmmss = startingTimestamp_.hhmmss();
  ok = AND2( ok, writeNetCDFIntAttribute( file, "STIME", hhmmss ) == 1 );
  const int tstep =
    timestepSize_ == HOURS ? 10000
    : timestepSize_ == DAYS ? 240000
    : timestepSize_ == MONTHS ? 31 * 24 * 10000
    : timestepSize_ == TRIMONTHS ? 3 * 31 * 24 * 10000
    : 366 * 24 * 10000;
  ok = AND2( ok, writeNetCDFIntAttribute( file, "TSTEP", tstep ) == 1 );
  ok = AND2( ok, writeNetCDFIntAttribute( file, "NTHIK", 1 ) == 1 );
  const int ncols =
    subsetIndices ? 1 + subsetIndices[ COLUMN ][ LAST ] -
                        subsetIndices[ COLUMN ][ FIRST ]
    : gridParameters.ncols();
  ok = AND2( ok, writeNetCDFIntAttribute( file, "NCOLS", ncols ) == 1 );
  const int nrows =
    subsetIndices ? 1 + subsetIndices[ ROW ][ LAST ] -
                        subsetIndices[ ROW ][ FIRST ]
    : gridParameters.nrows();
  ok = AND2( ok, writeNetCDFIntAttribute( file, "NROWS", nrows ) == 1 );
  const int nlays =
    subsetIndices ? 1 + subsetIndices[ LAYER ][ LAST ] -
                        subsetIndices[ LAYER ][ FIRST ]
    : gridParameters.nlays();
  ok = AND2( ok, writeNetCDFIntAttribute( file, "NLAYS", nlays ) == 1 );
  const int nvars = 2 + ( andElevation ? 1 : 0 ) + variables_ - skipVariables;
  CHECK( nvars > 0 );
  ok = AND2( ok, writeNetCDFIntAttribute( file, "NVARS", nvars ) == 1 );
  ok = AND2( ok, writeNetCDFIntAttribute( file, "GDTYP",
                                          gridParameters.gdtyp() ) == 1 );
  ok = AND2( ok, writeNetCDFDoubleAttribute( file, NC_GLOBAL, "P_ALP",
                                             gridParameters.p_alp() ) == 1 );
  ok = AND2( ok, writeNetCDFDoubleAttribute( file, NC_GLOBAL, "P_BET",
                                             gridParameters.p_bet() ) == 1 );
  ok = AND2( ok, writeNetCDFDoubleAttribute( file, NC_GLOBAL, "P_GAM",
                                             gridParameters.p_gam() ) == 1 );
  ok = AND2( ok, writeNetCDFDoubleAttribute( file, NC_GLOBAL, "XCENT",
                                             gridParameters.xcent() ) == 1 );
  ok = AND2( ok, writeNetCDFDoubleAttribute( file, NC_GLOBAL, "YCENT",
                                             gridParameters.ycent() ) == 1 );
  double xorig = gridParameters.xorig();
  double yorig = gridParameters.yorig();

  if ( subsetIndices ) {
    xorig += gridParameters.xcell() * ( subsetIndices[ COLUMN ][ FIRST ] - 1 );
    yorig += gridParameters.ycell() * ( subsetIndices[ ROW ][ FIRST ] - 1 );
  }

  ok = AND2( ok, writeNetCDFDoubleAttribute( file, NC_GLOBAL, "XORIG",
                                             xorig ) == 1 );
  ok = AND2( ok, writeNetCDFDoubleAttribute( file, NC_GLOBAL, "YORIG",
                                             yorig ) == 1 );
  ok = AND2( ok, writeNetCDFDoubleAttribute( file, NC_GLOBAL, "XCELL",
                                             gridParameters.xcell() ) == 1 );
  ok = AND2( ok, writeNetCDFDoubleAttribute( file, NC_GLOBAL, "YCELL",
                                             gridParameters.ycell() ) == 1 );
  ok = AND2( ok, writeNetCDFIntAttribute( file, "VGTYP",
                                          gridParameters.vgtyp() ) == 1 );
  ok = AND2( ok, writeNetCDFFloatAttribute( file, NC_GLOBAL, "VGTOP",
                                            gridParameters.vgtop() ) == 1 );

  if ( ok ) { // Construct and write VGLVLS:
    std::vector<float> vglvls;
    vglvls.resize( nlays + 1 ); //x
    const double* const vglvls0 = gridParameters.vglvls();
    const size_t first = subsetIndices ? subsetIndices[ LAYER ][FIRST] - 1 : 0;

    for ( size_t index = 0, count = nlays + 1;
          first + index < count; ++index ) {
      CHECK2( index < vglvls.size(),
              first + index <= (size_t) nlays );
      vglvls[ index ] = vglvls0[ first + index ];
    }

    ok = AND2( ok, writeNetCDFFloatArrayAttribute( file, "VGLVLS",
                                                   &vglvls[ 0 ],
                                                   vglvls.size() ) == 1 );
  }

  ok = AND2( ok, writeNetCDFTextAttribute( file, NC_GLOBAL, "GDNAM",
                                           "????????????????" ) == 1 );

  ok = AND2( ok, writeNetCDFTextAttribute( file, NC_GLOBAL, "UPNAM",
                                           "????????????????" ) == 1 );

  if ( ok ) { // Construct then write VAR-LIST:
    std::string varlist;
    varlist += "LONGITUDE       ";
    varlist += "LATITUDE        ";

    if ( andElevation ) {
      varlist += "ELEVATION       ";
    }

    for ( int variable = skipVariables; variable < variables_; ++variable ) {
      char name3[ 17 ] = "";
      makeNetCDFIOAPIVariableName( variableNames_[ variable ].c_str(), name3 );
      const int length = (int) strlen( name3 );
      const int count = std::min<int>( length, NAMLEN3 );
      int c = 0;

      for ( c = 0; c < count; ++c ) {
        const char ch = name3[ c ];
        varlist += forceUppercase ? toupper( ch ) : ch;
      }

      for ( ; c < NAMLEN3; ++c ) {
        varlist += ' ';
      }
    }

    CHECK( (int) varlist.length() == nvars * NAMLEN3 );
    ok = AND2( ok, writeNetCDFTextAttribute( file, NC_GLOBAL, "VAR-LIST",
                                             varlist.c_str() ) == 1 );
  }

  if ( ok ) { // Construct and write FILEDESC:
    std::string filedesc;
    filedesc.resize( MXDESC3 * MXDLEN3, ' ' );
    const char* content = "M3IO-format gridded data file.";
    CHECK( filedesc.length() > strlen( content ) );

    for ( size_t index = 0; content[ index ]; ++index ) {
      filedesc[ index ] = content[ index ];
    }

    CHECK( (int) filedesc.length() == MXDESC3 * MXDLEN3 );
    ok = AND2( ok, writeNetCDFTextAttribute( file, NC_GLOBAL, "FILEDESC",
                                             filedesc.c_str() ) == 1 );
  }

  if ( ok ) {  // Construct and write HISTORY:
    std::string history;
    history.resize( MXDLEN3, ' ' );
    const char* const content = "RSIG-processed file. www.epa.gov/rsig";
    CHECK( history.length() > strlen( content ) );

    for ( size_t index = 0; content[ index ]; ++index ) {
      history[ index ] = content[ index ];
    }

    CHECK( history.length() == MXDLEN3 );
    ok = AND2( ok, writeNetCDFTextAttribute( file, NC_GLOBAL, "HISTORY",
                                             history.c_str() ) == 1 );
  }

  if ( ! ok ) {
    throw std::runtime_error( "Failed to write IOAPI-format attributes." ); //x
  }

  POST( true );
}



/******************************************************************************
PURPOSE: writeIOAPITFLAGVariable - Write IOAPI-format TFLAG variable.
INPUTS:  const int file               NetCDF file to write to.
         const int outputVariables    Number of variables to write.
NOTES:   Throws exception upon failure.
******************************************************************************/

void Dataset::writeIOAPITFLAGVariable( const int file,
                                       const int outputVariables ) const {

  PRE2( file > 0, outputVariables >= 3 );
    const int hoursPerTimestep =
    timestepSize_ == HOURS ? 1 : timestepSize_ == DAYS ? 24 : 0;
  const int monthsPerTimestep =
    timestepSize_ == MONTHS ? 1
    : timestepSize_ == TRIMONTHS ? 3
    : timestepSize_ == YEARS ? 12
    : 0;
  Timestamp timestamp = startingTimestamp_;
  std::vector<int> tflag( timesteps_ * outputVariables * 2); //x YYYYDDD,HHMMSS

  for ( size_t timestep = 0, index = 0; timestep < timesteps_; ++timestep ) {
    const int yyyyddd = timestamp.yyyyddd();
    const int hhmmss  = timestamp.hhmmss();

    for ( int variable = 0; variable < outputVariables; ++variable ) {
      tflag[ index++ ] = yyyyddd;
      tflag[ index++ ] = hhmmss;
    }

    if ( hoursPerTimestep ) {
      timestamp.offsetHours( hoursPerTimestep );
    } else {
      timestamp.offsetMonths( monthsPerTimestep );
    }
  }

  const size_t dimensions[ 3 ] = {
    timesteps_, (size_t) outputVariables, 2
  }; //DATE-TIME
  const bool ok =
    writeNetCDFAllIntData( file, "TFLAG", dimensions, &tflag[ 0 ] );

  if ( ! ok ) {
    throw std::runtime_error( "Failed to write IOAPI TFLAG variable." ); //x
  }

}



/******************************************************************************
PURPOSE: computeCMAQGridLongitudesLatitudes - Compute longitude/latitude at
         each grid cell center using gridParameters.
INPUTS:  const CMAQGridParameters& gridParameters  Unsubsetted parameters.
         const size_t subsetIndices[ 4 ][ 2 ]  Optional subset grid indices:
                                          [TIME,LAYER,ROW,COLUMN][FIRST,LAST].
         const MapProjection* mapProjection  Map projcetion or 0 if lon-lat.
OUTPUTS: std::vector<float>& longitudes  Longitudes at grid cell centers.
         std::vector<float>& latitudes   Latitudes  at grid cell centers.
NOTES:   Throws exception upon failure to allocate data.
******************************************************************************/

void Dataset::computeCMAQGridLongitudesLatitudes(
                                    const CMAQGridParameters& gridParameters,
                                    const size_t subsetIndices[ 4 ][ 2 ],
                                    const MapProjection* mapProjection,
                                    std::vector<float>& longitudes,
                                    std::vector<float>& latitudes ) const {

  PRE3( gridParameters.invariant(),
        IMPLIES( subsetIndices,
                 AND8( subsetIndices[ TIME   ][ LAST  ] >=
                         subsetIndices[ TIME   ][ FIRST],
                       subsetIndices[ LAYER  ][ FIRST ] >= 1,
                       subsetIndices[ LAYER  ][ LAST  ] >=
                         subsetIndices[ LAYER ][ FIRST ],
                       subsetIndices[ ROW    ][ FIRST ] >= 1,
                       subsetIndices[ ROW    ][ LAST  ] >=
                         subsetIndices[ ROW   ][ FIRST ],
                       subsetIndices[ COLUMN ][ FIRST ] >= 1,
                       subsetIndices[ COLUMN ][ LAST  ] >=
                         subsetIndices[ COLUMN ][ FIRST ],
                       timesteps_ == 1 +
                         subsetIndices[ TIME ][ LAST ] -
                         subsetIndices[ TIME ][ FIRST ] ) ),
        IMPLIES( mapProjection, mapProjection->invariant() ) );

  const size_t layers =
    subsetIndices ? 1 + subsetIndices[ LAYER ][ LAST ] -
                        subsetIndices[ LAYER ][ FIRST ]
   : hasElevation() ? gridParameters.nlays()
   : 1;
  const size_t rows =
    subsetIndices ? 1 + subsetIndices[ ROW ][ LAST ] -
                        subsetIndices[ ROW ][ FIRST ]
    : gridParameters.nrows();
  const size_t columns =
    subsetIndices ? 1 + subsetIndices[ COLUMN ][ LAST ] -
                        subsetIndices[ COLUMN ][ FIRST ]
    : gridParameters.ncols();
  const size_t cells = layers * rows * columns;
  longitudes.clear();
  latitudes.clear();
  longitudes.resize( cells ); //x
  latitudes.resize( cells ); //x

  // Compute surface vertices by unprojecting cell centers:

  double xcell = gridParameters.xcell();
  double ycell = gridParameters.ycell();
  double xorig = gridParameters.xorig();
  double y1    = gridParameters.yorig();

  if ( subsetIndices ) {
    xorig += gridParameters.xcell() * ( subsetIndices[ COLUMN ][ FIRST ] - 1 );
    y1 += ycell * ( subsetIndices[ ROW ][ FIRST ] - 1 );
  }

  for ( size_t row = 0, cell = 0; row < rows; ++row, y1 += ycell ) {
    const double y2 = y1 + ycell;
    const double y  = 0.5 * ( y1 + y2 );
    double latitude = y;
    double x1 = xorig;

    for ( size_t column = 0; column < columns; ++column, x1 += xcell, ++cell) {
      const double x2 = x1 + xcell;
      const double x = 0.5 * ( x1 + x2 );
      double longitude = x;

      if ( mapProjection ) {
        mapProjection->unproject( x, y, longitude, latitude );
      }

      CHECK4( cell < rows * columns,
              cell < longitudes.size(),
              isLongitude( longitude ),
              isLatitude( latitude ) );

      longitudes[ cell ] = longitude;
      latitudes[  cell ] = latitude;
    }
  }

  // Copy surface values to remaining layers:

  const size_t layerCells = rows * columns;

  for ( size_t layer = 1, layerCell = layerCells; layer < layers; ++layer ) {

    for ( size_t cell = 0, surfaceCell = 0;
          cell < layerCells;
          ++cell, ++surfaceCell, ++layerCell ) {

      CHECK6( surfaceCell < layerCells,
              IN_RANGE( layerCell, layerCells, cells - 1 ),
              layerCell < longitudes.size(),
              surfaceCell < layerCell,
              isLongitude( longitudes[ surfaceCell ] ),
              isLatitude( latitudes[   surfaceCell ] ) );

      longitudes[ layerCell ] = longitudes[ surfaceCell ];
      latitudes[  layerCell ] = latitudes[  surfaceCell ];
    }
  }

  POST4( isLongitude( emvl::minimum( &longitudes[ 0 ], longitudes.size() ) ),
         isLongitude( emvl::maximum( &longitudes[ 0 ], longitudes.size() ) ),
         isLatitude(  emvl::minimum( &latitudes[  0 ], latitudes.size() ) ),
         isLatitude(  emvl::maximum( &latitudes[  0 ], latitudes.size() ) ) );
}



/******************************************************************************
PURPOSE: readBounds - Read XDR-format array of bounds from a stream.
INPUTS:  FILE* stream            Stream to read from.
         const size_t count      Number of bounds to read.
OUTPUTS: Bounds output[ count ]  Array of bounds read.
NOTES:   Throws exception upon failure to read valid data from a stream.
******************************************************************************/

void Dataset::readBounds( FILE* stream, const size_t count, Bounds bounds[] ) {
  PRE03( stream, count >= 1, bounds );

  // Read array: IEEE-754 64-bit reals bounds[count][2=lon,lat][2=min,max]

  double tempBounds[ 2 ][ 2 ] = { { 0.0, 0.0 }, { 0.0, 0.0 } };
  bool ok = true;

  for ( size_t index = 0; AND2( ok, index < count ); ++index ) {
    ok = fread( &tempBounds[ 0 ][ 0 ], sizeof tempBounds, 1, stream ) == 1;

    if ( ok ) {
      reverse8ByteWordsIfLittleEndian( &tempBounds[ 0 ][ 0 ],
                                       sizeof tempBounds /
                                         sizeof tempBounds[ 0 ][ 0] );
      ok = AND6( isLongitude( tempBounds[ LONGITUDE ][ MINIMUM ] ),
                 isLongitude( tempBounds[ LONGITUDE ][ MAXIMUM ] ),
                 tempBounds[ LONGITUDE ][ MINIMUM ] <=
                   tempBounds[ LONGITUDE ][ MAXIMUM ],
                 isLatitude( tempBounds[ LATITUDE ][ MINIMUM ] ),
                 isLatitude( tempBounds[ LATITUDE ][ MAXIMUM ] ),
                 tempBounds[ LATITUDE ][ MINIMUM ] <=
                   tempBounds[ LATITUDE ][ MAXIMUM ] );

    }

    if ( ok ) {
      const Bounds b( tempBounds[ LONGITUDE ][ MINIMUM ],
                      tempBounds[ LONGITUDE ][ MAXIMUM ],
                      tempBounds[ LATITUDE ][ MINIMUM ],
                      tempBounds[ LATITUDE ][ MAXIMUM ] );
      bounds[ index ] = b;
    }
  }

  if ( ! ok ) {
    throw std::runtime_error( "Failed to read valid array of bounds." ); //x
  }

  POST02( bounds[ 0 ].invariant(), bounds[ count - 1 ].invariant() );
}



/******************************************************************************
PURPOSE: writeBounds - Write XDR-format array of bounds to a stream.
INPUTS:  FILE* stream  Stream to write to.
         const size_t count      Number of bounds to write.
         const Bounds bounds[ count ]  Array of bounds to write.
NOTES:   Throws exception upon failure to write valid data to a stream.
******************************************************************************/

void Dataset::writeBounds( FILE* stream, const size_t count,
                           const Bounds bounds[] ) {
  PRE05( stream, count >= 1, bounds,
         bounds[ 0 ].invariant(), bounds[ count - 1 ].invariant() );

  // Write array: # IEEE-754 64-bit reals bounds[count][2=lon,lat][2=min,max]

  bool ok = true;

  for ( size_t index = 0; AND2( ok, index < count ); ++index ) {
    const Bounds& b = bounds[ index ];
    double tempBounds[ 2 ][ 2 ] = { { 0.0, 0.0 }, { 0.0, 0.0 } };
    tempBounds[ LONGITUDE ][ MINIMUM ] = b.west();
    tempBounds[ LONGITUDE ][ MAXIMUM ] = b.east();
    tempBounds[ LATITUDE  ][ MINIMUM ] = b.south();
    tempBounds[ LATITUDE  ][ MAXIMUM ] = b.north();
    reverse8ByteWordsIfLittleEndian( &tempBounds[ 0 ][ 0 ],
                                     sizeof tempBounds /
                                       sizeof tempBounds[ 0 ][ 0 ] );
    ok = fwrite( &tempBounds[ 0 ][ 0 ], sizeof tempBounds, 1, stream ) == 1;
  }

  if ( ! ok ) {
    throw std::runtime_error( "Failed to write array of bounds." ); //x
  }
}



/******************************************************************************
PURPOSE: readStrings - Read an array of strings from a stream.
INPUTS:  FILE* stream                      Stream to read from.
         const size_t length               Blank-padded + \n length.
OUTPUTS: std::vector<std::string>& strings Strings read w/o trailing spaces.
         FILE* stream                      Updated stream.
NOTES:   Throws exception upon failure to read valid data from a stream.
******************************************************************************/

void Dataset::readStrings( FILE* stream, const size_t length,
                           std::vector<std::string>& strings ) {
  PRE03( stream, IN_RANGE( length, 1, 1024 ), strings.size() >= 1 );

  const size_t count = strings.size();
  enum { LENGTH = 1024 };
  char temp[ LENGTH + 1 ] = "";
  bool ok = true;

  for ( size_t index = 0; AND2( ok, index < count ); ++index ) {
    memset( temp, 0, sizeof temp );
    ok = fread( temp, sizeof temp[ 0 ], length, stream ) == length;

    if ( ok ) {
      ok = AND2( temp[ length - 1 ] == '\n', strlen( temp ) == length );
      temp[ length - 1 ] = '\0'; // Erase newline;
      bool allBlank = true;

      for ( size_t c = length - 1; AND2( ok, c > 0 ); ) {
        --c;
        const char ch = temp[ c ];

        if ( AND2( allBlank, isspace( ch ) ) ) {
          temp[ c ] = '\0'; // Trim trailing spaces.
        } else {
          allBlank = false;
          // PurpleAir note has Korean and other (ASCII Extended) characters!
          // ok = AND4( ch, ch != '\t', ch != '\n', isprint( ch ) );
          ok = AND3( ch, ch != '\t', ch != '\n' ); // Allow them.
          // if ( ! isprint( ch ) ) fprintf( stderr, "temp = '%s', strlen( temp ) = %" LU ", isprint( %d ) = %d\n", temp, strlen( temp ), ch, isprint( ch ) );
        }
      }

      if ( AND2( ok, ! allBlank ) ) {
        strings[ index ] = temp; //x
      } else {
        ok = false;
      }
    }
  }

  if ( ! ok ) {
    throw std::runtime_error( "Failed to read valid array of strings." ); //x
  }

  POST02( strings[ 0 ].length() < length,
          strings[ strings.size() - 1 ].length() < length );
}



/******************************************************************************
PURPOSE: writeStrings - Write an array of strings to a stream.
INPUTS:  FILE* stream                             Stream to write to.
         const size_t length                      Blank-padded + \n length.
         const std::vector<std::string>& strings  String to write.
OUTPUTS: FILE* stream                             Updated stream.
NOTES:   Throws exception upon failure.
******************************************************************************/

void Dataset::writeStrings( FILE* stream, const size_t length,
                            const std::vector<std::string>& strings ) {
  PRE03( stream, IN_RANGE( length, 1, 1024 ), strings.size() >= 1 );

  const size_t count = strings.size();
  enum { LENGTH = 1024 };
  char temp[ LENGTH + 1 ] = "";
  bool ok = true;
  memset( temp, 0, sizeof temp );

  for ( size_t index = 0; AND2( ok, index < count ); ++index ) {
    const char* const s = strings[ index ].c_str();
    size_t c = 0;

    // Copy s:

    for ( c = 0; s[ c ]; ++c ) {
      CHECK2( c < strings[ index ].length(), c < LENGTH );
      temp[ c ] = s[ c ];
    }

    // Pad with trailing blanks:

    for ( ; c < length; ++c ) {
      CHECK( c < LENGTH );
      temp[ c ] = ' ';
    }

    temp[ length - 1 ] = '\n';
    temp[ length     ] = '\0';
    ok = fwrite( temp, sizeof temp[ 0 ], length, stream ) == length;
  }

  if ( ! ok ) {
    throw std::runtime_error( "Failed to write array of strings." ); //x
  }
}



/******************************************************************************
PURPOSE: readProjectionAndGrid - Read map projection and grid parameters of XDR
         header from a stream.
INPUTS:  FILE* stream                   Stream to read from.
         MapProjection*& mapProjection  Current projection object or 0.
OUTPUTS: MapProjection*& mapProjection  New projection object or 0 if lonlat.
         CMAQGridParameters& gridParameters Grid parameters read.
NOTES:   Throws exception upon failure to read valid input from a stream.
******************************************************************************/

void Dataset::readProjectionAndGrid( FILE* stream,
                                     MapProjection*& mapProjection,
                                     CMAQGridParameters& gridParameters ) {
  PRE0( stream );

  // Read map projection parameters:

  double sphereRadius = 0.0;
  double p_alp = 0.0;
  double p_bet = 0.0;
  double xcent = 0.0;
  double ycent = 0.0;
  int gdtyp = 0;

  readProjectionParameters( stream, sphereRadius, p_alp, p_bet, xcent, ycent,
                            gdtyp ); //x

  const double p_gam = xcent;

  // Read grid parameters:

  int ncols = 0;
  int nrows = 0;
  int nlays = 0;
  double xorig = 0.0;
  double yorig = 0.0;
  double xcell = 0.0;
  double ycell = 0.0;
  int vgtyp = 0;
  double vgtop = 0;
  double vglvls[ CMAQGridParameters::MXLAYS3 + 1 ];
  memset( vglvls, 0, sizeof vglvls );
  const double g = 9.81;
  const double R = 287.04;
  const double A = 50.0;
  const double T0 = 290.0; // HACK: AMAD HEMI and AQMEII are supposedly 275.
  const double P0 = 100000.0;
  int levels = 0;
  bool ok = AND2( fscanf( stream,
                  "# Grid: ncols nrows xorig yorig xcell ycell vgtyp vgtop "
                  "vglvls[%d]:\n", &levels ) == 1,
                  levels >= 2 );

  nlays = levels - 1;

  ok = AND2( ok, fscanf( stream, "%d %d %lf %lf %lf %lf %d %lf ",
                         &ncols, &nrows, &xorig, &yorig, &xcell, &ycell,
                         &vgtyp, &vgtop ) == 8 );

  for ( int level = 0; AND2( ok, level < levels ); ++level ) {
    ok = fscanf( stream, "%lf", vglvls + level ) == 1;
  }

  fscanf( stream, "\n" );

  if ( ok ) {
    ok = CMAQGridParameters::isValidParameters( ncols, nrows, nlays,
                                                gdtyp, sphereRadius,
                                                p_alp, p_bet, p_gam,
                                                xcent, ycent,
                                                xorig, yorig, xcell, ycell,
                                                vgtyp, vgtop, vglvls,
                                                g, R, A, T0, P0 );

    if ( ok ) {
      gridParameters = CMAQGridParameters( ncols, nrows, nlays,
                                           gdtyp, sphereRadius,
                                           p_alp, p_bet, p_gam,
                                           xcent, ycent,
                                           xorig, yorig, xcell, ycell,
                                           vgtyp, vgtop, vglvls,
                                           g, R, A, T0, P0 ); //x

      if ( mapProjection ) {
        delete mapProjection, mapProjection = 0;
      }

      switch ( gdtyp ) {
      case CMAQGridParameters::LAMGRD3:
        mapProjection = new Lambert( sphereRadius, sphereRadius,
                                     xcent, ycent, p_alp, p_bet ); //x
        break;
      case CMAQGridParameters::ALBGRD3:
        mapProjection = new Albers( sphereRadius, sphereRadius,
                                    xcent, ycent, p_alp, p_bet ); //x
        break;
      case CMAQGridParameters::EQMGRD3:
        mapProjection = new Mercator( sphereRadius, sphereRadius, xcent ); //x
        break;
      case CMAQGridParameters::POLGRD3:
        mapProjection = new Stereographic( sphereRadius, sphereRadius,
                                           xcent, ycent, p_bet ); //x
        break;
      default:
        CHECK( gdtyp == CMAQGridParameters::LATGRD3 );
        break;
      }
    }
  }

  if ( ! ok ) {
    throw std::runtime_error( "Failed to read valid grid parameters."); //x
  }

  POST02( gridParameters.invariant(),
          IMPLIES_ELSE( gridParameters.gdtyp() == CMAQGridParameters::LATGRD3,
                        mapProjection == 0,
                        AND2( mapProjection, mapProjection->invariant() ) ) );
}



/******************************************************************************
PURPOSE: writeProjectionAndGrid - Write XDR format map projection and grid
         parameters to a stream.
INPUTS:  FILE* stream                              Stream to write to.
         const CMAQGridParameters& gridParameters  Grid description.
OUTPUTS: FILE* stream  Updated stream.
NOTES:   Throws exception upon failure.
******************************************************************************/

void Dataset::writeProjectionAndGrid( FILE* stream,
                                      const CMAQGridParameters&
                                        gridParameters ) {
  PRE02( stream, gridParameters.invariant() );

  writeProjectionParameters( stream, gridParameters ); //x

  /* Now write portion of XDR header that looks like this:

    # Grid: ncols nrows xorig yorig xcell ycell vgtyp vgtop vglvls[25]:
    279 240 -1.008e+06 -1.62e+06 12000 12000 2 10000 1 0.995 0.99 0.98 0.97 \
      0.96 0.94 0.92 0.9 0.88 0.86 0.84 0.82 0.8 0.77 0.74 0.7 0.65 0.6 0.5 \
      0.4 0.3 0.2 0.1 0

  */

  const char* const format =
    "# Grid: ncols nrows xorig yorig xcell ycell vgtyp vgtop vglvls[%d]:\n"
    "%d %d %lg %lg %lg %lg %d %lg";
  const int levels = gridParameters.nlays() + 1;
  bool ok = fprintf( stream, format,
                     levels,
                     gridParameters.ncols(), gridParameters.nrows(),
                     gridParameters.xorig(), gridParameters.yorig(),
                     gridParameters.xcell(), gridParameters.ycell(),
                     gridParameters.vgtyp(), gridParameters.vgtop()) > 70;
  const double* const vglvls = gridParameters.vglvls();

  for ( int level = 0; AND2( ok, level < levels ); ++level ) {
    const double value = vglvls[ level ];
    ok = fprintf( stream, " %lg", value ) > 1;
  }

  ok = AND2( ok, fprintf( stream, "\n" ) == 1 );

  if ( ! ok ) {
    throw std::runtime_error("Failed to write grid parameters to XDR header.");
    //x
  }
}



/******************************************************************************
PURPOSE: readProjectionParameters - Read map projection parameters of XDR
         header from a stream.
INPUTS:  FILE* stream          Stream to read from.
OUTPUTS: FILE* stream          Advanced stream.
         double& sphereRadius  Spheroid radius.
         double& p_alp         p_alp projection parameter.
         double& p_bet         p_bet projection parameter.
         double& xcent         xcent projection parameter.
         double& ycent         ycent projection parameter.
         int& gdtyp            Projection type.
NOTES:   Throws exception upon failure to read valid data from a stream.
******************************************************************************/

void Dataset::readProjectionParameters( FILE* stream,
                                        double& sphereRadius,
                                        double& p_alp,
                                        double& p_bet,
                                        double& xcent,
                                        double& ycent,
                                        int& gdtyp ) {
  PRE0( stream );
  enum { MAXIMUM_LENGTH = 256 };
  char line[ MAXIMUM_LENGTH + 1 ] = "";
  memset( line, 0, sizeof line );
  bool ok = false;

  sphereRadius = 0.0;
  p_alp = 0.0;
  p_bet = 0.0;
  xcent = 0.0;
  ycent = 0.0;
  gdtyp = 0;

  if ( fgets( line, sizeof line / sizeof *line, stream ) ) {
    double minorSemiaxis = 0.0;

    if ( strstr( line, "# lcc projection:" ) ) {
      gdtyp = CMAQGridParameters::LAMGRD3;

      if ( fscanf( stream, "%lf %lf %lf %lf %lf %lf\n",
                   &p_alp, &p_bet, &ycent, &xcent,
                   &sphereRadius, &minorSemiaxis ) == 6 ) {
        ok = sphereRadius == minorSemiaxis;
      }

    } else if ( strstr( line, "# albers projection:" ) ) {
      gdtyp = CMAQGridParameters::ALBGRD3;

      if ( fscanf( stream, "%lf %lf %lf %lf %lf %lf\n",
                   &p_alp, &p_bet, &ycent, &xcent,
                   &sphereRadius, &minorSemiaxis ) == 6 ) {
        ok = sphereRadius == minorSemiaxis;
      }

    } else if ( strstr( line, "# mercator projection:" ) ) {
      gdtyp = CMAQGridParameters::EQMGRD3;

      if ( fscanf( stream, "%lf %lf %lf\n",
                   &xcent, &sphereRadius, &minorSemiaxis ) == 3 ) {
        ok = sphereRadius == minorSemiaxis;
      }

    } else if ( strstr( line, "# stereographic projection:" ) ) {
      gdtyp = CMAQGridParameters::POLGRD3;

      if ( fscanf( stream, "%lf %lf %lf %lf %lf\n",
                   &ycent, &xcent, &p_bet,
                   &sphereRadius, &minorSemiaxis ) == 5 ) {
        ok = sphereRadius == minorSemiaxis;
        p_alp = ycent == 90.0 ? 1.0 : ycent == -90.0 ? -1.0 : 0.0;
      }
    } else if ( strstr( line, "# lonlat projection:" ) ) {
      gdtyp = CMAQGridParameters::LATGRD3;

      if ( fscanf( stream, "%lf %lf\n", &sphereRadius, &minorSemiaxis ) == 2) {
        ok = sphereRadius == minorSemiaxis;
      }
    }
  }

  if ( ! ok ) {
    throw std::runtime_error("Failed to read valid projection parameters.");//x
  }
}



/******************************************************************************
PURPOSE: writeProjectionParameters - Write XDR format map projection parameters
         to a stream.
INPUTS:  FILE* stream  Stream to write to.
         const CMAQGridParameters& gridParameters  Grid description.
OUTPUTS: FILE* stream  Updated stream.
NOTES:   Throws exception upon failure.
******************************************************************************/

void Dataset::writeProjectionParameters( FILE* stream,
                                         const CMAQGridParameters&
                                           gridParameters ) {
  PRE02( stream, gridParameters.invariant() );

  /* Write portion of XDR header that looks like this:

    # lcc projection: lat_1 lat_2 lat_0 lon_0 major_semiaxis minor_semiaxis
    33 45 40 -97 6.37e+06 6.37e+06

  */

  bool ok = false;
  const double sphereRadius = gridParameters.sphereRadius();

  switch ( gridParameters.gdtyp() ) {
  case CMAQGridParameters::LAMGRD3:
    {
      const char* const format =
        "# lcc projection: "
        "lat_1 lat_2 lat_0 lon_0 major_semiaxis minor_semiaxis\n"
        "%lg %lg %lg %lg %lg %lg\n";
      ok = fprintf( stream, format,
                    gridParameters.p_alp(), gridParameters.p_bet(),
                    gridParameters.ycent(), gridParameters.xcent(),
                    sphereRadius, sphereRadius ) > (int) strlen( format ) - 12;
    }

    break;
  case CMAQGridParameters::ALBGRD3:
    {
      const char* const format =
        "# albers projection: "
        "lat_1 lat_2 lat_0 lon_0 major_semiaxis minor_semiaxis\n"
        "%lg %lg %lg %lg %lg %lg\n";
      ok = fprintf( stream, format,
                    gridParameters.p_alp(), gridParameters.p_bet(),
                    gridParameters.ycent(), gridParameters.xcent(),
                    sphereRadius, sphereRadius ) > (int) strlen( format ) - 12;
    }

    break;
  case CMAQGridParameters::EQMGRD3:
    {
      const char* const format =
        "# mercator projection:"
        " lon_0 major_semiaxis minor_semiaxis\n"
        "%lg %lg %lg\n";
      ok = fprintf( stream, format,
                    gridParameters.xcent(),
                    sphereRadius, sphereRadius ) > (int) strlen( format ) - 6;
    }

    break;
  case CMAQGridParameters::POLGRD3:
    {
      const char* const format =
        "# stereographic projection:"
        " lat_0 lon_0 lat_sec major_semiaxis minor_semiaxis\n"
        "%lg %lg %lg %lg %lg\n";
      ok = fprintf( stream, format,
                    gridParameters.ycent(), gridParameters.xcent(),
                    gridParameters.p_bet(),
                    sphereRadius, sphereRadius ) > (int) strlen( format ) - 10;
    }

    break;
  default:
    CHECK( gridParameters.gdtyp() == CMAQGridParameters::LATGRD3 );
    {
      const char* const format =
        "# lonlat projection: major_semiaxis minor_semiaxis\n %lg %lg\n";
      ok = fprintf( stream, format,
                    sphereRadius, sphereRadius ) > (int) strlen( format ) - 4;
    }

    break;
  }

  if ( ! ok ) {
    throw std::runtime_error( "Failed to write map projection info to "
                              "XDR header." ); //x
  }
}



/******************************************************************************
PURPOSE: writeCOARDSNotes - Write COARDS-format notes array to file.
INPUTS:  const int file                        NetCDF file to write to.
         const size_t length                   Padded length of each string.
         const std::vector<std::string> notes  Notes to write.
NOTES:   Throws exception upon failure.
******************************************************************************/

void Dataset::writeCOARDSNotes( const int file, const size_t length,
                                const std::vector<std::string>& notes ) {
  PRE03( file > 0, IN_RANGE( length, 1, 1024 ), notes.size() );
  const size_t count = notes.size();
  enum { LENGTH = 1024 };
  char temp[ LENGTH + 1 ] = "";
  bool ok = true;
  memset( temp, 0, sizeof temp );

  for ( size_t index = 0; index < count; ++index ) {
    const char* const s = notes[ index ].c_str();
    size_t c = 0;

    // Copy s:

    for ( c = 0; s[ c ]; ++c ) {
      CHECK2( c < notes[ index ].length(), c < LENGTH );
      temp[ c ] = s[ c ];
    }

    // Pad with trailing blanks:

    for ( ; c < length; ++c ) {
      CHECK( c < LENGTH );
      temp[ c ] = ' ';
    }

    temp[ length     ] = '\0';
    ok = writeNetCDFCharData( file, "notes", index, length, temp );
  }

  if ( ! ok ) {
    throw std::runtime_error( "Failed to write COARDS-format notes." ); //x
  }
}



/******************************************************************************
PURPOSE: writeCOARDSDateTime - Write COARDS-format date and time data to file.
INPUTS:  const int file  NetCDF file to write to.
         const std::vector<Timestamp>& timestamps  Timestamps to write.
NOTES:   Throws exception upon failure.
******************************************************************************/

void Dataset::writeCOARDSDateTime( const int file,
                                   const std::vector<Timestamp>& timestamps ) {
  PRE02( file > 0, timestamps.size() );
  const size_t count = timestamps.size();
  std::vector<int> buffer( count ); //x

  for ( size_t timestamp = 0; timestamp < count; ++timestamp ) {
    CHECK2( timestamp < timestamps.size(), timestamp < buffer.size() );
    buffer[ timestamp ] = timestamps[ timestamp ].yyyymmdd();
  }

  bool ok = writeNetCDFAllIntData(file, "yyyymmdd", &count, &buffer[ 0 ]);

  if ( ok ) {

    for ( size_t timestamp = 0; timestamp < count; ++timestamp ) {
      CHECK2( timestamp < timestamps.size(), timestamp < buffer.size() );
      buffer[ timestamp ] = timestamps[ timestamp ].hhmmss();
    }

    ok = writeNetCDFAllIntData( file, "hhmmss", &count, &buffer[ 0 ] );
  }

  if ( ! ok ) {
    throw std::runtime_error( "Failed to write COARDS-format date/time." ); //x
  }
}


} // namespace emvl.



