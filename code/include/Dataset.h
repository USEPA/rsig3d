#ifndef DATASET_H
#define DATASET_H

/******************************************************************************
PURPOSE: Dataset.h - Dataset non-instantiable ADT ABC.
HISTORY: 2013-02-17 plessel.todd@epa.gov
STATUS:  unreviewed tested
******************************************************************************/

//================================ INCLUDES ===================================

#include <stdio.h> // For FILE.

#include <string>    // For std::string.
#include <vector>    // For std::vector.
#include <stdexcept> // For std::runtime_error.

#include <Assertions.h> // For PRE(), POST().
#include <Utilities.h>  // For DataColor.

#include <Bounds.h>             // For Bounds.
#include <Timestamp.h>          // For Timestamp.
#include <DataUtilities.h>      // For MISSING, IMISSING.
#include <MapProjection.h>      // For MapProjection.
#include <CMAQGridParameters.h> // For CMAQGridParameters.
#include <QueryParameters.h>    // For QueryParameters.

//================================== TYPES ====================================

namespace emvl {

/*
 * Point must be a compact POD (for C compatibility, e.g., graphics pipeline).
 * longitude is in [-180, 180], latitude is in [-90, 90].
 * elevation is in meters above mean sea level.
*/

typedef struct { double longitude; double latitude; double elevation; } Point;
bool operator==( const Point& a, const Point& b );
bool operator!=( const Point& a, const Point& b );
bool isPoint( const Point& point ); // Does point have valid geo-coordinates?

class Dataset {

public:

  // Types:

  /*
   * If dataset xdr file is larger than PAGE_THREHOLD_GB bytes then use buffing
   * to page-in one timestep at a time from disk into memory.
   * This is needed to reduce the memory used by a dataset since 32-bit Windows
   * can fail to read the entire dataset into memory.
   * Even with paging a single timestep at a time (see BUFFERED_DATA_TIMESTEPS
   * below) Windows can still fail to read a single timestep of data when the
   * single timestep of data is still too large to allocate and read into
   * 32-bit limited memory. E.g., TEMPO L3 gridded data for ite entire grid.
   */

  enum { PAGE_THREHOLD_GB = 256 * 1024 * 1024 };

  /*
   * If dataFileOffset_ != 0, then data_[variable][time][cells] contains only
   * data for BUFFERED_DATA_TIMESTEPS timesteps.
   * This value was previously set to 24 (e.g., 24 hours = 1 day of data)
   * but for data such as CMAQ EQUATES CONUS METDOT3D WIND
   * (24 hours x 4 variables (ELEVATION, UWIND, VWIND, WWIND) x 35 x 300 x 460
   * x 8 byte real), this requires 3.7GB of memory which fails to allocate on
   * 32-bit Windows laptops. Tests reveal 1 timestep (0.15GB) is workable.
   */

  enum { BUFFERED_DATA_TIMESTEPS = 1 };
  enum { POINT, QUADRILATERAL, POLYGONS, HEXAHEDRON, CELL_TYPES };
  enum { SW, SE, NE, NW }; // QUADRILATERAL cell vertex order.
  enum { HOURS, DAYS, MONTHS, TRIMONTHS, YEARS, TIMESTEP_SIZES };

  /*
   * QUADRILATERAL vertex order =
   *  (west, south), (east, south), (east, north), (west, north)
   * HEXAHEDRON vertex order =
   *  (west, south, lower), (east, south, lower),
   *  (east, north, lower), (west, north, lower)
   *  (west, south, upper), (east, south, upper),
   *  (east, north, upper), (west, north, upper)
   * POLYGONS = one or more triangle strips per data value.
   */

  /*
   * subset() returns a vector (possibly of size 1) of these PODs.
   * Some datasets, such as MOZAIC aircraft flights, have a variable number of
   * flights within a time range so the result of subset() is a vector of
   * SubsetPointers - one per flight. Whereas other datasets such as ground
   * sites have just one set of SubsetPointers.
   * For isStationary(), ! isGroup() datasets such as ground stations,
   * coordinates is the same for all data - coordinates == &coordinates[ 0 ].
   * For other datasets such as aircraft flights, coordinates is an offset into
   * coordinates_.
   * For some datasets such as CMAQ grids, coordinates are computed rather than
   * stored (to greatly reduce memory) so coordinates == 0 and cellVertices()
   * must be called to get a copy of cellVertexCount() vertices for each data
   * value.
   * cellType() is called to determine the type of cell, e.g., HEXAHEDRON.
   */

  typedef struct {
    size_t coordinatesCount;  // Number of coordinates.
    const Point* coordinates; // Offset into coordinates_ or 0 if computed and
                              // cellVertices() must be used instead.
    size_t dataCount;         // Number of values in data[].
    const double* data;       // data[dataCount] offset into data_ for subset.
    size_t notesCount;        // Number of values in notes[] or 0.
    const std::string* notes; // notes[notesCount] MOZAIC flight info or 0.
  } SubsetPointers;

protected:

  std::string fileName_; // File name if data was read from a file, else empty.
  mutable FILE* outputKMLFile_; // Opened file for writing KML content or 0.
  mutable DataColor outputKMLDataColorFunction_; // Data color function for KML.
  std::string name_;
  std::string description_;
  mutable Bounds bounds_; // Mutable due to disk paging of swath data.
  Timestamp startingTimestamp_;
  Timestamp endingTimestamp_;
  size_t timesteps_;            // Total number of timesteps of data.
  size_t dataFileOffset_;       // Byte offset in file of first data value.
  mutable size_t firstDataTimestep_; // 0-based 1st  time index of data_ in mem
  mutable size_t lastDataTimestep_;  // 0-based last time index of data_ in mem
  int variables_;
  int dataVariable_;
  int cellType_;
  bool hasElevation_;
  bool hasArea_;
  bool hasVolume_;
  bool isStationary_;
  bool isGroup_;
  bool isStructuredGrid_;
  bool isVector_; // Default is false. Length is variables_.
  int timestepSize_; // HOURS, DAYS, MONTHS, etc.
  std::vector<std::string> variableNames_;
  std::vector<std::string> variableUnits_;
  mutable std::vector<double> minimums_;   // [variable].
  mutable std::vector<double> maximums_;   // [variable].
  mutable std::vector<double> data_;       // [variable][time][cell].
  mutable std::vector<Point> coordinates_; // [cell].
  mutable std::string probedNote_;    // Note of probed point (if any).

  Dataset();

  // Helper routines:

  static bool pointInCell( const double longitude, const double latitude,
                           const double elevation, const size_t count,
                           const Point vertices[] );

  /*
   * Some dataset's time-stepped data is too large to fit into memory.
   * When this is the case, then:
   *   1. The data must be read from a file (! fileName_.empty()).
   *   2. dataFileOffset_ is used to seek from file.
   *   3. [firstDataTimestep_, lastDataTimestep_] is a sub-range of
   *      [0, timesteps_ - 1].
   *   4. loadedDataSubset() is used to check/load a sub-time-range of data
   *      from fileName_ into data_.
   *      It returns false if the dataset does not contain any data for the
   *      time-range.
   *      It throws stl::runtime_error if it was unable to load the data from
   *      the file.
   *      This is called by probe(), subset(), etc.
   */

  virtual bool loadedDataSubset( const Timestamp firstTimestamp,
                                 const Timestamp lastTimestamp ) const;

  /*
   * The following protected virtual member functions:
   * vprobe(), vtimeseries(), vsubset()
   * are called by their corresponding non-virtual public member functions:
   * probe(), timeseries(), subset().
   * This design allows for default arguments.
   * (Template Method Design Pattern):
   */

  virtual double vprobe( const Timestamp timestamp,
                         const double longitude,
                         const double latitude,
                         const double elevation,
                         const int probeLayer,
                         const int variable ) const = 0;

  // Default implementation of vtimeseries() calls probe() per hour in range:

  virtual void vtimeseries( const Timestamp beginTimestamp,
                            const Timestamp endTimestamp,
                            const double longitude,
                            const double latitude,
                            const double elevation,
                            const int probeLayer,
                            std::vector<double>& result ) const;

  virtual std::vector<SubsetPointers> vsubset( const Timestamp beginTimestamp,
                                               const Timestamp endTimestamp,
                                               const int variable ) const = 0;

  void copy( const Dataset& other ); //x Copy above attributes.
  bool equal( const Dataset& other ) const; // Compare with above attributes.
  bool isValidName() const;
  bool isValidDescription() const;
  bool isValidVariableNamesAndUnits() const;
  bool isValidCoordinates() const;
  bool isValidData() const;
  bool isValidRange() const;

  bool isInBounds( const double longitude, const double latitude,
                   const double nearEnough ) const;

  Point cellCenter( const size_t cellIndex ) const;
  static void computeVerticalRectangleCoordinates( const size_t groundPoints,
                                                   const size_t verticalPoints,
                                              const double longitudeCenters[],
                                              const double latitudeCenters[],
                                              const double elevationCenters[],
                                              const double thicknessCenters[],
                                        std::vector<Point>& coordinates ); //x

  void secondsRange( const Timestamp& beginTimestamp,
                     const Timestamp& endTimestamp,
                     long long& beginSeconds,
                     long long& endSeconds ) const;

  bool isInTimeRange( const Timestamp& timestamp ) const;
  bool isInTimeRange( const Timestamp& timestamp, size_t& timestep ) const;
  bool isInTimeRange( const Timestamp& timestamp, Timestamp& firstHour ) const;

  bool overlappedTimeRange( Timestamp& beginTimestamp,
                            Timestamp& endTimestamp ) const;

  bool overlappedTimeRange( const Timestamp& beginTimestamp,
                            const Timestamp& endTimestamp,
                            size_t& firstTimestep,
                            size_t& matchingTimesteps ) const;

  bool overlappedTimeRange( const Timestamp& beginTimestamp,
                            const Timestamp& endTimestamp,
                            size_t& firstTimestep,
                            size_t& matchingTimesteps,
                            size_t& skippedHours ) const;

  bool overlappedTimeRange( Timestamp& beginTimestamp,
                            Timestamp& endTimestamp,
                            Timestamp& firstHour,
                            Timestamp& lastHour ) const;

  bool matchingTimestamps( const Timestamp& timestamp,
                           const std::vector<Timestamp>& timestamps,
                           size_t& first, size_t& last ) const;

  bool matchingTimestamps( const Timestamp& timestamp,
                           const std::vector<Timestamp>& startingTimestamps,
                           const std::vector<Timestamp>& endingTimestamps,
                           size_t& first, size_t& last ) const;

  bool matchingTimestamps( const Timestamp& startTimestamp,
                           const Timestamp& endTimestamp,
                           const std::vector<Timestamp>& timestamps,
                           size_t& first, size_t& last ) const;

  bool matchingTimestamps( const Timestamp& beginTimestamp,
                           const Timestamp& endTimestamp,
                           const std::vector<Timestamp>& startingTimestamps,
                           const std::vector<Timestamp>& endingTimestamps,
                           size_t& first, size_t& last ) const;

  static bool allZeroHour( const std::vector<Timestamp>& timestamps );

  virtual void computeBounds();
  void computeVariableRanges(); //x
  void skipLine( FILE* file ); //x
  void skipBytes( FILE* file, const size_t bytes ); //x
  void readAndMatchLine( FILE* file, const char* match ); //x

  int readAndMatchLine2( FILE* file,
                         const char* const match1,
                         const char* const match2 ); //x

  void readDescription( FILE* file ); //x
  void readTimestamp( FILE* file, const bool two ); //x
  void readDimensions( FILE* file, const size_t count, size_t dimensions[]);//x
  void readVariablesAndUnits( FILE* file, const size_t count ); //x
  void setEndingTimestamp(); // From startingTimestep_, timesteps_, timestepSize_.
  void setDatasetNameFromDescriptionAndVariable();
  void readRegridded64BitVariables(FILE* stream,const size_t layersPerPoint);//x
  void readRegridded32BitVariables( FILE* stream ); //x
  void writeRegridded64BitCoordinatesAndVariables( FILE* stream ) const; //x
  void writeRegridded32BitCoordinatesAndVariables( FILE* stream ) const; //x

  void writeIOAPIHeader( const int file,
                         const bool andElevation,
                         const bool forceUppercase,
                         const int skipVariables,
                         const int layers,
                         const int rows,
                         const int columns,
                         const CMAQGridParameters& gridParameters,
                         const size_t subsetIndices[ 4 ][ 2 ],
                         const MapProjection* mapProjection,
                         int dimensionIds[ 6 ] ) const; //x

  void writeIOAPIAttributes( const int file,
                             const bool andElevation,
                             const bool forceUppercase,
                             const int skipVariables,
                             const CMAQGridParameters& gridParameters,
                             const size_t subsetIndices[ 4 ][ 2 ] ) const; //x

  void writeIOAPITFLAGVariable( const int file,
                                const int outputVariables ) const; //x

  void computeCMAQGridLongitudesLatitudes(
                                    const CMAQGridParameters& gridParameters,
                                    const size_t subsetIndices[ 4 ][ 2 ],
                                    const MapProjection* mapProjection,
                                    std::vector<float>& longitudes,
                                    std::vector<float>& latitudes ) const; //x

  void writeCOARDSCommon( const int file,
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
                          const std::vector<Timestamp>& timestamps ) const; //x

  void createCOARDSVariables( const int file,
                              const bool isRegridded,
                              const bool hasCorners,
                              const bool withElevation,
                              const int coordinateRank,
                              const int skipVariables,
                              const int dimensions,
                              const int dimensionIds[],
                              const char* const dimensionNames[] ) const; //x

  void writeCOARDSVariables( const int file,
                             const int skipVariables,
                             const int integerVariables,
                             const size_t dimensionSizes[] ) const; //x

  void writeIOAPIVariables( const int file,
                             const int skipVariables,
                             const int integerVariables,
                             const size_t dimensionSizes[] ) const; //x

  void writeBufferedCOARDSVariables( const int file,
                                     const int skipVariables,
                                     const int integerVariables,
                                     const int dimensions,
                                     const size_t dimensionSizes[],
                                     const std::vector<Timestamp>&
                                       timestamps )  const; //x

  void writeBufferedIOAPIVariables( const int file,
                                    const int skipVariables,
                                    const int integerVariables,
                                    const int dimensions,
                                    const size_t dimensionSizes[],
                                    const std::vector<Timestamp>&
                                       timestamps )  const; //x

  void writeCOARDSCoordinates( const int file,
                               const bool hasCorners,
                               const bool withElevation,
                               const int coordinateRank,
                               const int dimensions,
                               const size_t dimensionSizes[],
                               const char* const dimensionNames[] ) const; //x

  static void readBounds(FILE* stream, const size_t count, Bounds bounds[]);//x

  static void writeBounds( FILE* stream, const size_t count,
                           const Bounds bounds[] ); //x

  static void readStrings( FILE* stream, const size_t length,
                           std::vector<std::string>& strings ); //x

  static void writeStrings( FILE* stream, const size_t length,
                            const std::vector<std::string>& strings ); //x

  static void readProjectionAndGrid( FILE* stream,
                                     MapProjection*& mapProjection,
                                     CMAQGridParameters& gridParameters ); //x

  static void writeProjectionAndGrid( FILE* stream,
                                      const CMAQGridParameters&
                                        gridParameters ); //x

  static void readProjectionParameters( FILE* stream,
                                        double& sphereRadius,
                                        double& p_alp,
                                        double& p_bet,
                                        double& xcent,
                                        double& ycent,
                                        int& gdtyp ); //x

  static void writeProjectionParameters( FILE* stream,
                                         const CMAQGridParameters&
                                           gridParameters ); //x

  static void writeCOARDSDateTime( const int file,
                                  const std::vector<Timestamp>& timestamps);//x

  static void writeCOARDSNotes( const int file, const size_t length,
                                const std::vector<std::string>& notes ); //x

  virtual void copyOutCornerCoordinate( const int coordinate,
                                        const int corner,
                                        std::vector<double>& result ) const;

  virtual void copyOutLongitudes( std::vector<double>& result ) const;

  virtual void copyOutLatitudes( std::vector<double>& result ) const;

  virtual void copyOutElevations( std::vector<double>& result ) const;

  template <typename T>
  void readCoordinates( FILE* stream, const bool readElevations,
                        T buffer[] ); //x

  template <typename T>
  void writeCoordinates( FILE* stream, T buffer[] ) const; //x

  template <typename T>
  void readVariable( FILE* stream, const int variable,
                     const T minimum, const T maximum, T buffer[] ); //x

  template <typename T>
  void writeVariable( FILE* stream, const int variable,
                      const T minimum, const T maximum, T buffer[] ) const; //x

  template <typename T>
  static void readIntegers( FILE* stream, const T minimum, const T maximum,
                            std::vector<size_t>& result ); //x

  template <typename T>
  static void writeIntegers( FILE* stream, const T minimum, const T maximum,
                             const std::vector<size_t>& result ); //x

public:

  // Commands:

  virtual ~Dataset();
  void dataVariable( const int variable );
  void name( const std::string& name );
  virtual Dataset* sample( const Dataset& other ) const = 0; //x

  virtual Dataset* regrid( const int regridMethod,
                           const CMAQGridParameters&
                             cmaqGridParameters ) const = 0; //x

  virtual void write( const std::string& directory,
                      const int format ) const = 0; //x

  void write( const std::string& directory,
              const int format,
              const int regridMethod,
              const CMAQGridParameters&
                cmaqGridParameters ) const; //x calls regrid();write().

  void outputKMLFile( FILE* file ) const { outputKMLFile_ = file; }
  void outputKMLDataColorFunction( DataColor dataColorFunction ) const {
    outputKMLDataColorFunction_ = dataColorFunction;
  }

  // Queries:

  virtual bool invariant() const;
  const std::string& fileName() const; // If read from a file, else empty.
  const std::string& name() const;
  const std::string& description() const;
  const Bounds& bounds() const;
  Timestamp startingTimestamp() const;
  Timestamp endingTimestamp() const;
  virtual size_t timesteps() const;
  int variables() const;
  int dataVariable() const;
  int cellType() const;
  int cellVertexCount() const;
  bool hasElevation() const;
  bool hasArea() const;
  bool hasVolume() const;
  bool isStationary() const;
  bool isGroup() const;
  bool isStructuredGrid() const;
  bool isVector() const;
  bool isPagedFromDisk() const;
  int timestepSize() const;
  virtual bool isSurfaceData() const;
  virtual size_t layers() const;
  virtual size_t rows() const;
  virtual size_t columns() const;
  const std::string& variableName( const int variable = IMISSING ) const;
  const std::string& variableUnits( const int variable = IMISSING ) const;
  std::string pathedFileName( const std::string& directory,
                              const int format ) const;
  FILE* outputKMLFile() const { return outputKMLFile_; }
  DataColor outputKMLDataColorFunction() const { return outputKMLDataColorFunction_; }
  double minimum( const int variable = IMISSING ) const;
  double maximum( const int variable = IMISSING ) const;
  virtual void cellVertices( const size_t cell, Point points[] ) const;

  // Return a single data value nearest a probed location. Calls vprobe():

  double probe( const Timestamp timestamp,
                const double longitude,
                const double latitude,
                const double elevation = MISSING,
                const int probeLayer = IMISSING,
                const int variable = IMISSING ) const;

  const std::string& probedNote() const;
  virtual const std::vector<std::string>& allDistinctNotes() const;
  virtual const Point* distinctNotePoint(const std::string& distinctNote) const;

  void timeseries( const Timestamp beginTimestamp,
                   const Timestamp endTimestamp,
                   const double longitude,
                   const double latitude,
                   const double elevation,
                   const int probeLayer,
                   std::vector<double>& result ) const;

  std::vector<SubsetPointers>
  subset( const Timestamp beginTimestamp,
          const Timestamp endTimestamp,
          const int variable = IMISSING ) const; // Calls vsubset().
};



/******************************************************************************
PURPOSE: readCoordinates - Read XDR binary coordinates from a stream.
INPUTS:  FILE* stream  Stream to read from.
         const bool readElevations  Read elevations after longitudes,latitudes?
         coordinates_.size()  Is number of points to read.
OUTPUTS: coordinates_[ * ]    Read/converted/checked coordinates.
         T buffer[]           Buffer of size coordinates_.size() to read into.
NOTES:   Throws exception upon failure to read valid data from a stream.
******************************************************************************/

template <typename T>
void Dataset::readCoordinates( FILE* stream, const bool readElevations,
                               T buffer[] ) {
  PRE03( stream, coordinates_.size(), buffer );
  const size_t points = coordinates_.size();
  const size_t coordinates = 2 + readElevations;
  const double ranges[ 3 ][ 2 ] = {
    { -180.0, 180.0 },
    { -90.0, 90.0 },
    { QueryParameters::MINIMUM_ELEVATION, QueryParameters::MAXIMUM_ELEVATION }
  };
  bool ok = true;

  for ( size_t coordinate = 0; AND2( ok, coordinate < coordinates );
        ++coordinate ) {
    ok = fread( buffer, sizeof buffer[ 0 ], points, stream ) == points;

    if ( ok ) {

      if ( sizeof buffer[ 0 ] == 4 ) {
        reverse4ByteWordsIfLittleEndian( buffer, points );
      } else if ( sizeof buffer[ 0 ] == 8 ) {
        reverse8ByteWordsIfLittleEndian( buffer, points );
      }

      for ( size_t point = 0; AND2( ok, point < points ) ; ++point ) {
        const double value = buffer[ point ];
        ok = IN_RANGE( value,
                       ranges[ coordinate ][ MINIMUM ],
                       ranges[ coordinate ][ MAXIMUM ] );

        if ( ok ) {
          Point& p = coordinates_[ point ];

          switch ( coordinate ) {
          case LONGITUDE:
            p.longitude = value;
            break;
          case LATITUDE:
            p.latitude = value;
            break;
          default:
            p.elevation = value;
            break;
          }
        }
      }
    }
  }

  if ( ! ok ) {
    throw std::runtime_error( "Failed to read valid coordinates." ); //x
  } else {
    computeBounds();
  }

  POST0( isValidCoordinates() );
}



/******************************************************************************
PURPOSE: writeCoordinates - Write XDR binary coordinates to a stream.
INPUTS:  FILE* stream   Stream to read from.
OUTPUTS: T buffer[ coordinates_.size() / cellVertexCount() ] Buffer to copy to.
NOTES:   Throws exception upon failure to read valid data from a stream.
******************************************************************************/

template <typename T>
void Dataset::writeCoordinates( FILE* stream, T buffer[] ) const {
  PRE2( stream, buffer );
  const size_t verticesPerCell = cellVertexCount();
  const size_t cells = coordinates_.size() / verticesPerCell;
  const size_t coordinates = hasElevation() ? 3 : 2;
  bool ok = true;

  for ( size_t coordinate = 0;
        AND2( ok, coordinate < coordinates );
        ++coordinate ) {

    for ( size_t cell = 0; cell < cells; ++cell ) {
      const size_t cellOffset = cell * verticesPerCell;
      buffer[ cell ] = 0.0;

      for ( size_t vertex = 0; vertex < verticesPerCell; ++vertex ) {
        const size_t index = cellOffset + vertex;
        CHECK( index <coordinates_.size() );
        const Point& coordinatePoint = coordinates_[ index ];
        const double value =
          coordinate == LONGITUDE ? coordinatePoint.longitude
          : coordinate == LATITUDE  ? coordinatePoint.latitude
          : coordinatePoint.elevation;
        buffer[ cell ] += value; // Sum values.
      }

      buffer[ cell ] /= verticesPerCell; // Mean is centroid.
    }

    if ( sizeof buffer[ 0 ] == 4 ) {
      reverse4ByteWordsIfLittleEndian( buffer, cells );
    } else if ( sizeof buffer[ 0 ] == 8 ) {
      reverse8ByteWordsIfLittleEndian( buffer, cells );
    }

    ok = fwrite( buffer, sizeof buffer[ 0 ], cells, stream ) == cells;
  }

  if ( ! ok ) {
    throw std::runtime_error( "Failed to write XDR binary coordinates." ); //x
  }

  POST( true );
}



/******************************************************************************
PURPOSE: readVariable - Read XDR binary variable from stream.
INPUTS:  FILE* stream        Stream to write to.
         const int variable  Index of variable to read.
         const T minimum     Minimum value to check input data.
         const T maximum     Maximum value to check input data.
OUTPUTS: FILE* stream        Updated stream.
         T buffer[]          Buffer to use for endian conversion.
NOTES:   Throws exception upon failure.
******************************************************************************/

template <typename T>
void Dataset::readVariable( FILE* stream, const int variable,
                            const T minimum, const T maximum,
                            T buffer[] ) {
  PRE07( stream, IN_RANGE( variable, 0, variables_ - 1 ),
        isFinite( minimum ), isFinite( maximum ), minimum <= maximum,
        buffer, data_.size() );
  const size_t points = data_.size() / variables_;
  const size_t dataOffset = variable * points;
  const double minimum2 = minimum;
  const double maximum2 = maximum;
  const bool ok = readConvertedArray( stream, buffer, points,
                                      minimum2, maximum2,
                                      &data_[ dataOffset ] );

  if ( ! ok ) {
    throw std::runtime_error( "Failed to read XDR binary variable." );//x
  }

  POST0( true );
}



/******************************************************************************
PURPOSE: writeVariable - Write XDR binary variable to stream.
INPUTS:  FILE* stream        Stream to write to.
         const int variable  Index of variable to write.
         const T minimum     Minimum value to clamp output data.
         const T maximum     Maximum value to clamp output data.
OUTPUTS: FILE* stream        Updated stream.
         T buffer[]          Buffer to use for endian conversion.
NOTES:   Throws exception upon failure.
******************************************************************************/

template <typename T>
void Dataset::writeVariable( FILE* stream, const int variable,
                             const T minimum, const T maximum,
                             T buffer[] ) const {
  PRE6( stream, IN_RANGE( variable, 0, variables_ - 1 ),
        isFinite( minimum ), isFinite( maximum ), minimum <= maximum,
        buffer );
  const size_t points = data_.size() / variables_;
  const size_t dataOffset = variable * points;
  const bool ok = writeConvertedArray( stream, buffer, points,
                                       minimum, maximum,
                                       &data_[ dataOffset ] );

  if ( ! ok ) {
    throw std::runtime_error( "Failed to write XDR binary variable." ); //x
  }

  POST( true );
}



/******************************************************************************
PURPOSE: readIntegers - Read and convert XDR-format array of integers from
         stream.
INPUTS:  FILE* stream                 Stream to read from.
         const T minimum              Minimum valid value to read.
         const T maximum              Maximum valid value to read.
OUTPUTS: std::vector<size_t>& result  Read/converted/checked values.
NOTES:   Throws exception upon failure to read valid data from a stream.
******************************************************************************/

template <typename T>
void Dataset::readIntegers( FILE* stream, const T minimum, const T maximum,
                            std::vector<size_t>& result ) {
  PRE04( stream, minimum >= 0, minimum <= maximum, result.size() >= 1 );
  const size_t count = result.size();
  std::vector<T> buffer( count ); //x
  const size_t minimum2 = minimum;
  const size_t maximum2 = maximum;
  const bool ok =
    readConvertedArray( stream, &buffer[ 0 ], count, minimum2, maximum2,
                        &result[ 0 ] );

  if ( ! ok ) {
    throw std::runtime_error( "Failed to read valid array of integers." ); //x
  }

  POST02( emvl::minimum( &result[ 0 ], result.size() ) >= (size_t) minimum,
          emvl::maximum( &result[ 0 ], result.size() ) <= (size_t) maximum );
}



/******************************************************************************
PURPOSE: writeIntegers - Write clamped/converted XDR-format array of integers
         to stream.
INPUTS:  FILE* stream                 Stream to write to.
         const T minimum              Minimum value to clamp to.
         const T maximum              Maximum value to clamp to.
         const std::vector<size_t>& result  Copied/converted/clamped values.
NOTES:   Throws exception upon failure to write data to a stream.
******************************************************************************/

template <typename T>
void Dataset::writeIntegers( FILE* stream, const T minimum, const T maximum,
                             const std::vector<size_t>& result ) {
  PRE04( stream, minimum >= 0, minimum <= maximum, result.size() >= 1 );
  const size_t count = result.size();
  std::vector<T> buffer( count ); //x
  const bool ok =
    writeConvertedArray( stream, &buffer[ 0 ], count, minimum, maximum,
                         &result[ 0 ] );

  if ( ! ok ) {
    throw std::runtime_error( "Failed to write array of integers." ); //x
  }
}


} // namespace emvl.

#endif /* DATASET_H */



