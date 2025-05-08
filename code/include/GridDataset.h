#ifndef GridDATASET_H
#define GridDATASET_H

/******************************************************************************
PURPOSE: GridDataset.h - Dataset of gridded model surface point scalar/vector
         data.
HISTORY: 2020-03-27 plessel.todd@epa.gov
STATUS:  unreviewed tested
******************************************************************************/

//================================ INCLUDES ===================================

#include <stdio.h> // For FILE.

#include <string> // For std::string.
#include <vector> // For std::vector.

#include <Assertions.h> // For PRE(), POST().
#include <Timestamp.h>  // For Timestamp.
#include <Dataset.h>    // For Dataset.

//================================== TYPES ====================================

namespace emvl {

class GridDataset : public Dataset {

private:

  size_t rows_;                    // Number of rows of grid cells.
  size_t columns_;                 // Number of columns of grid cells.

  // Helpers:

  bool timestepOfTimestamp( const Timestamp timestamp, size_t& timestep) const;

  size_t timestepsOfTimestamps( const Timestamp beginTimestamp,
                                const Timestamp endTimestamp,
                                size_t& firstTimestep,
                                size_t& lastTimestep ) const;

  virtual bool loadedDataSubset( const Timestamp firstTimestamp,
                                 const Timestamp lastTimestamp ) const;

  bool readData( FILE* file,
                 size_t firstTimestep,
                 size_t lastTimestep ) const;

  bool probedPoint( const double longitude, const double latitude,
                    size_t& column, size_t& row ) const;

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

  void readXDRHeader( FILE* stream ); //x
  void readXDRData( FILE* stream ); //x
  void writeXDR( FILE* stream ) const; //x
  void writeASCII( FILE* stream ) const; //x
  void writeCOARDS( const std::string& fileName ) const; //x
  void writeKML() const; //x

public:

  // Commands:

  virtual ~GridDataset();
  GridDataset( FILE* file ); //x
  GridDataset( const std::string& fileName ); //x
  GridDataset( const GridDataset& other ); //x
  GridDataset& operator=( const GridDataset& other ); //x
  virtual Dataset* sample( const Dataset& other ) const; //x

  virtual Dataset* regrid( const int regridMethod,
                           const CMAQGridParameters&
                             cmaqGridParameters ) const; //x

  virtual void write( const std::string& directory,
                      const int format ) const; //x

  // Queries:

  virtual bool invariant() const;
  bool operator==( const GridDataset& other ) const;
  bool operator!=( const GridDataset& other ) const;
  virtual size_t rows() const;
  virtual size_t columns() const;
};

} // namespace emvl.

#endif /* GridDATASET_H */



