#ifndef CMAQDATASET_H
#define CMAQDATASET_H

/******************************************************************************
PURPOSE: CMAQDataset.h - Dataset of CMAQ gridded model data.
HISTORY: 2013-02-20 plessel.todd@epa.gov
STATUS:  unreviewed tested
******************************************************************************/

//================================ INCLUDES ===================================

#include <stdio.h> // For FILE.

#include <string>  // For std::string.
#include <vector>  // For std::vector.

#include <Assertions.h>         // For PRE(), POST().
#include <Timestamp.h>          // For Timestamp.
#include <DataUtilities.h>      // For MISSING, IMISSING.
#include <CMAQGridParameters.h> // For CMAQGridParameters.
#include <QueryParameters.h>    // For QueryParameters::REGRID_NONE.
#include <Dataset.h>            // For Dataset.
#include <MapProjection.h>      // For MapProjection.

//================================== TYPES ====================================

namespace emvl {

class CMAQDataset : public Dataset {
public:

  typedef size_t SubsetIndices[4][2]; // [TIME,LAYER,ROW,COLUMN][FIRST,LAST].

private:

  size_t layers_;                     // Number of layers of grid cells.
  size_t rows_;                       // Number of rows of grid cells.
  size_t columns_;                    // Number of columns of grid cells.
  std::string gridName_;              // E.g., "C_12_OAQPS".
  SubsetIndices subsetIndices_;       // [TIME,LAYER,ROW,COLUMN][FIRST,LAST].
  CMAQGridParameters gridParameters_; // Of original unsubsetted grid.
  MapProjection* mapProjection_;      // Used to unproject vertices. 0 if lonlat
  std::vector<double> longitudes_;    // longitudes_[rows_ + 1][ columns_ + 1]
                                      // if mapProjection != 0.
  std::vector<double> latitudes_;     // latitudes_[ rows_ + 1][ columns_ + 1]
                                      // if mapProjection != 0.
  mutable std::vector<double> elevations_; // elevations_[layers_+1][rows_][columns_]
  mutable size_t elevationDataIndex_;  // First index of data_[] of elevations_
  double elevation1_;            // Single elevation for all grid cell centers.
  bool is64bit_;                       // Is the input stream/file 64-bit data?

  // Helpers:

  bool timestepOfTimestamp( const Timestamp timestamp, size_t& timestep) const;

  size_t timestepsOfTimestamps( const Timestamp beginTimestamp,
                                const Timestamp endTimestamp,
                                size_t& firstTimestep,
                                size_t& lastTimestep ) const;

  virtual bool loadedDataSubset( const Timestamp firstTimestamp,
                                 const Timestamp lastTimestamp ) const;

  bool readBufferedDataSubset( const size_t firstTimestep,
                               const bool checkAll ) const;

  bool probedCell( const double longitude, const double latitude,
                   const double elevation,
                   size_t& column, size_t& row, size_t& layer ) const;

  virtual double vprobe( const Timestamp timestamp,
                         const double longitude,
                         const double latitude,
                         const double elevation,
                         const int probeLayer,
                         const int variable ) const;

  virtual void vtimeseries( const Timestamp beginTimestamp,
                            const Timestamp endTimestamp,
                            const double longitude,
                            const double latitude,
                            const double elevation,
                            const int probeLayer,
                            std::vector<double>& result ) const;

  virtual std::vector<SubsetPointers> vsubset( const Timestamp beginTimestamp,
                                               const Timestamp endTimestamp,
                                               const int variable ) const;

  void readSubsetIndices( FILE* stream ); //x
  void readXDRHeader( FILE* stream, bool& readAndSkipLonLats ); //x
  void readXDRData( FILE* stream, const bool readAndSkipLonLats ); //x
  void computeElevations( const double cellCenterElevations[] ) const ; //x
  void computeCMAQGridElevations( std::vector<float>& elevations ) const; //x
  void computeLongitudesLatitudes(); //x
  void writeXDR( FILE* stream ) const; //x
  void writeASCII( FILE* stream ) const; //x
  void writeCOARDS( const std::string& fileName ) const; //x
  void writeKML() const; //x
  void writeIOAPI( const std::string& fileName ) const; //x
  void writeIOAPIData( const int file ) const; //x
  void writeIOAPICoordinates( const int file ) const; //x

  virtual void computeBounds();

  template <typename T>
  void copyCellCenterCoordinates( const int coordinate, T output[] ) const;

  virtual void copyOutLongitudes( std::vector<float>& result ) const {
    PRE( result.size() >= layers_ * rows_ * columns_ );
    copyCellCenterCoordinates( LONGITUDE, &result[ 0 ] );
  }

  virtual void copyOutLongitudes( std::vector<double>& result ) const {
    PRE( result.size() >= layers_ * rows_ * columns_ );
    copyCellCenterCoordinates( LONGITUDE, &result[ 0 ] );
  }

  virtual void copyOutLatitudes( std::vector<float>& result ) const {
    PRE( result.size() >= layers_ * rows_ * columns_ );
    copyCellCenterCoordinates( LATITUDE, &result[ 0 ] );
  }

  virtual void copyOutLatitudes( std::vector<double>& result ) const {
    PRE( result.size() >= layers_ * rows_ * columns_ );
    copyCellCenterCoordinates( LATITUDE, &result[ 0 ] );
  }

  virtual void copyOutElevations( std::vector<float>& result ) const {
    PRE( result.size() >= layers_ * rows_ * columns_ );
    copyCellCenterCoordinates( ELEVATION, &result[ 0 ] );
  }

  virtual void copyOutElevations( std::vector<double>& result ) const {
    PRE( result.size() >= layers_ * rows_ * columns_ );
    copyCellCenterCoordinates( ELEVATION, &result[ 0 ] );
  }

public:

  // Commands:

  virtual ~CMAQDataset();
  CMAQDataset( FILE* file ); //x
  CMAQDataset( const std::string& fileName ); //x
  CMAQDataset( const CMAQDataset& other ); //x
  CMAQDataset& operator=( const CMAQDataset& other ); //x
  virtual Dataset* sample( const Dataset& other ) const; //x

  virtual Dataset* regrid( const int regridMethod,
                           const CMAQGridParameters&
                             cmaqGridParameters ) const; //x

  virtual void write( const std::string& directory,
                      const int format ) const; //x

  // Queries:

  virtual bool invariant() const;
  bool operator==( const CMAQDataset& other ) const;
  bool operator!=( const CMAQDataset& other ) const;
  virtual bool isSurfaceData() const;
  virtual size_t layers() const;
  virtual size_t rows() const;
  virtual size_t columns() const;
  const std::string& gridName() const;
  const SubsetIndices& subsetIndices() const;
  const CMAQGridParameters& gridParameters() const;
  const MapProjection* mapProjection() const;
  virtual void cellVertices( const size_t cell, Point points[] ) const;
};



/******************************************************************************
PURPOSE: copyCellCenterCoordinates - Compute and store a copy of a coordinate
         for all grid cells.
INPUTS:  const int coordinate    LONGITUDE, LATITUDE or ELEVATION.
OUTPUTS: T output[]  output[ layers_ * rows_ * columns_ ]
******************************************************************************/

template <typename T>
void CMAQDataset::copyCellCenterCoordinates( const int coordinate,
                                             T output[] ) const {
  PRE2( IN4( coordinate, LONGITUDE, LATITUDE, ELEVATION ), output );

  const double* const coordinates =
    mapProjection_ == 0 ? 0
    : coordinate == LONGITUDE ? &longitudes_[ 0 ]
    : coordinate == LATITUDE ? &latitudes_[ 0 ]
    : 0;
  const size_t layerSize = rows_ * columns_;

  if ( IN3( coordinate, LONGITUDE, LATITUDE ) ) {
    const size_t firstColumn = subsetIndices_[ COLUMN ][ FIRST ];
    const size_t firstRow = subsetIndices_[ ROW ][ FIRST ];
    const double xorig = gridParameters_.xorig();
    const double yorig = gridParameters_.yorig();
    const double xcell = gridParameters_.xcell();
    const double ycell = gridParameters_.ycell();
    const double halfXcell = xcell * 0.5;
    const double halfYcell = ycell * 0.5;
    const size_t rowSize = columns_ + 1;
    const double west  = xorig + ( firstColumn - 1 ) * xcell;
    const double south = yorig + ( firstRow    - 1 ) * ycell;
    size_t outputIndex = 0;

    // Compute grid cell center coordinate by averaging 4 corner coordinates:

    for ( size_t row = 0; row < rows_; ++row ) {
      const size_t rowOffset = row * rowSize;

      for ( size_t column = 0; column < columns_; ++column, ++outputIndex ) {
        double centerCoordinate = 0.0;

        if ( mapProjection_ ) {
          const size_t indexSW = rowOffset + column;
          const size_t indexSE = indexSW + 1;
          const size_t indexNW = indexSW + rowSize;
          const size_t indexNE = indexNW + 1;
          CHECK4( IN_RANGE( indexSW, 0, ( rows_ + 1 ) * ( columns_ + 1 ) - 2 ),
                  IN_RANGE( indexSE, 1, ( rows_ + 1 ) * ( columns_ + 1 ) - 1 ),
                  IN_RANGE( indexNW, 0, ( rows_ + 1 ) * ( columns_ + 1 ) - 2 ),
                  IN_RANGE( indexNE, 1, ( rows_ + 1 ) * ( columns_ + 1 ) - 1 ) );
          const double coordinateSW = coordinates[ indexSW ];
          const double coordinateSE = coordinates[ indexSE ];
          const double coordinateNW = coordinates[ indexNW ];
          const double coordinateNE = coordinates[ indexNE ];

          CHECK( IMPLIES_ELSE( coordinate == LONGITUDE,
                               AND4( isLongitude( coordinateSW ),
                                     isLongitude( coordinateSE ),
                                     isLongitude( coordinateNW ),
                                     isLongitude( coordinateNE ) ),
                               AND4( isLatitude( coordinateSW ),
                                     isLatitude( coordinateSE ),
                                     isLatitude( coordinateNW ),
                                     isLatitude( coordinateNE ) ) ) );

          centerCoordinate =
            0.25 * (coordinateSW + coordinateSE + coordinateNW + coordinateNE);
        } else if ( coordinate == LONGITUDE ) {
          centerCoordinate = west + column * xcell + halfXcell;
          CHECK( isLongitude( centerCoordinate ) );
        } else {
          CHECK( coordinate == LATITUDE );
          centerCoordinate = south + row * ycell + halfYcell;
          CHECK( isLatitude( centerCoordinate ) );
        }

        CHECK( outputIndex < rows_ * columns_ );
        output[ outputIndex ] = centerCoordinate;
        CHECK( IMPLIES_ELSE( coordinate == LONGITUDE,
                             isLongitude( output[ outputIndex ] ),
                             isLatitude( output[ outputIndex ] ) ) );
      }
    }

    // Replicate values to all layers:

    const size_t layerBytes = layerSize * sizeof output[ 0 ];

    for ( size_t layer = 1; layer < layers_; ++layer,
          outputIndex += layerSize ) {
      CHECK2( IN_RANGE( outputIndex, layerSize, layers_ * rows_ * columns_ -1),
              IN_RANGE( outputIndex + layerSize,
                        layerSize, layers_ * rows_ * columns_ ) );
      memcpy( &output[ outputIndex ], &output[ 0 ], layerBytes );
    }

  } else { // Copy elevations:

    if ( cellType_ == HEXAHEDRON ) { // Average cell top and bottom coordinate:

      if ( elevations_.size() == ( layers_ + 1 ) * rows_ * columns_ ) {

        for ( size_t layer = 0, outputIndex = 0; layer < layers_; ++layer ) {
          const size_t layerOffset = layer * layerSize;

          for ( size_t cell = 0; cell < layerSize; ++cell, ++outputIndex ) {
            const size_t index = layerOffset + cell;
            const size_t nextLayerIndex = index + layerSize;
            CHECK2( index < layers_ * rows_ * columns_,
                    nextLayerIndex < ( layers_ + 1 ) * rows_ * columns_ );
            const double elevation = elevations_[ index ];
            const double nextLayerElevation = elevations_[ nextLayerIndex ];
            const double centerElevation =
              0.5 * ( elevation + nextLayerElevation );
            CHECK3( isElevation( elevation ),
                    isElevation( nextLayerElevation ),
                    outputIndex < layers_ * rows_ * columns_ );
            output[ outputIndex ] = centerElevation;
            CHECK( isElevation( output[ outputIndex ] ) );
          }
        }
      } else {

        for ( size_t cell = 0; cell < layerSize; ++cell ) {
          output[ cell ] = elevation1_;
        }
      }
    } else { // Copy center elevations:
      CHECK3( cellType_ == QUADRILATERAL, layers_ == 1,
              elevations_.size() == layerSize );

      for ( size_t cell = 0; cell < layerSize; ++cell ) {
        output[ cell ] = elevations_[ cell ];
        CHECK( isElevation( output[ cell ] ) );
      }
    }
  }

  POST( OR3( AND3( coordinate == LONGITUDE,
                   isLongitude( emvl::minimum( &output[ 0 ],
                                               layers_ * rows_ * columns_ ) ),
                   isLongitude( emvl::maximum( &output[ 0 ],
                                               layers_ * rows_ * columns_ ) )),
             AND3( coordinate == LATITUDE,
                   isLatitude( emvl::minimum( &output[ 0 ],
                                              layers_ * rows_ * columns_ ) ),
                   isLatitude( emvl::maximum( &output[ 0 ],
                                              layers_ * rows_ * columns_ ) ) ),
             AND3( coordinate == ELEVATION,
                   isElevation( emvl::minimum( &output[ 0 ],
                                               layers_ * rows_ * columns_ ) ),
                   isElevation( emvl::maximum( &output[ 0 ],
                                               layers_ * rows_ * columns_)))));
}



} // namespace emvl.

#endif /* CMAQDATASET_H */



