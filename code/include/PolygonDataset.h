#ifndef POLYGONDATASET_H
#define POLYGONDATASET_H

/******************************************************************************
PURPOSE: PolygonDataset.h - Dataset of 2D polygon data.
HISTORY: 2018-11-21 plessel.todd@epa.gov
STATUS:  unreviewed tested
******************************************************************************/

//================================ INCLUDES ===================================

#include <stdio.h> // For FILE.

#include <string> // For std::string.
#include <vector> // For std::vector.

#include <Timestamp.h>          // For Timestamp.
#include <DataUtilities.h>      // For MISSING, IMISSING.
#include <CMAQGridParameters.h> // For CMAQGridParameters.
#include <QueryParameters.h>    // For QueryParameters::REGRID_NONE.
#include <Dataset.h>            // For Dataset.

//================================== TYPES ====================================


namespace emvl {

class PolygonDataset : public Dataset {
private:

  struct PolygonShapeData {
    size_t count;             // Number of shapes.
    void* polygonShapes;      // Array of polygon shapes (allocated in C).
    void* shapeData;          // Shape data for array (allocated in C).

    void deallocate();
    void copy( const PolygonShapeData& other ); //x
    ~PolygonShapeData() { deallocate(); }
    PolygonShapeData() { count = 0; polygonShapes = 0; shapeData = 0; }
    PolygonShapeData( const PolygonShapeData& other ) { copy( other ); } //x

    PolygonShapeData& operator=( const PolygonShapeData& other ) { //x
      if ( this != &other ) copy( other );
      return *this;
    }

    bool operator==( const PolygonShapeData& other ) const;
    bool operator!=( const PolygonShapeData& other ) const {
      return ! ( *this == other );
    }
  };

  std::vector<Timestamp> startingTimestamps_; // Of each datum.
  std::vector<Timestamp> endingTimestamps_;   // Of each datum.
  PolygonShapeData polygonShapeData_;   // Representation as sets of triangles.

  // Helpers:

  bool findNearestPoint( const double longitude,
                         const double latitude,
                         const double elevation,
                         const double nearEnough,
                         const size_t first,
                         const size_t last,
                         size_t& nearest ) const;

  virtual double vprobe( const Timestamp timestamp,
                         const double longitude,
                         const double latitude,
                         const double,
                         const int,
                         const int variable ) const;

  virtual std::vector<SubsetPointers> vsubset( const Timestamp beginTimestamp,
                                               const Timestamp endTimestamp,
                                               const int variable ) const;

  void readXDRHeader( FILE* stream ); //x
  void readXDRData( FILE* stream ); //x
  void writeXDR( FILE* stream ) const; //x
  void writeASCII( FILE* stream ) const; //x
  void writeCOARDS( const std::string& fileName ) const; //x
  void writeShapefile( const std::string& fileName ) const;
  void unbinShapefile( FILE* stream ) const;
  void writeKML() const; //x

public:

  // Commands:

  virtual ~PolygonDataset();
  PolygonDataset( FILE* file ); //x
  PolygonDataset( const std::string& fileName ); //x
  PolygonDataset( const PolygonDataset& other ); //x
  PolygonDataset& operator=( const PolygonDataset& other ); //x
  virtual Dataset* sample( const Dataset& other ) const; //x

  virtual Dataset* regrid( const int regridMethod,
                           const CMAQGridParameters&
                             cmaqGridParameters ) const; //x

  virtual void write( const std::string& directory,
                      const int format ) const; //x

  // Queries:

  virtual bool invariant() const;
  bool operator==( const PolygonDataset& other ) const;
  bool operator!=( const PolygonDataset& other ) const;
};

} // namespace emvl.

#endif /* POLYGONDATASET_H */



