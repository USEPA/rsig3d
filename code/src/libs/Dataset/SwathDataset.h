#ifndef SWATHDATASET_H
#define SWATHDATASET_H

/******************************************************************************
PURPOSE: SwathDataset.h - Dataset of swath satellite data.
HISTORY: 2013-10-14 plessel.todd@epa.gov
STATUS:  unreviewed tested
******************************************************************************/

//================================ INCLUDES ===================================

#include <stdio.h> // For FILE.

#include <string>  // For std::string.
#include <vector>  // For std::vector.

#include <Timestamp.h>          // For Timestamp.
#include <DataUtilities.h>      // For MISSING, IMISSING.
#include <CMAQGridParameters.h> // For CMAQGridParameters.
#include <QueryParameters.h>    // For QueryParameters::REGRID_NONE.
#include <Dataset.h>            // For Dataset.

//================================== TYPES ====================================

namespace emvl {

class SwathDataset : public Dataset {

private:

  std::vector<Timestamp> timestamps_;  // Timestamp of each scan.
  std::vector<size_t> points_;         // Number of points in each scan.
  mutable std::vector<double> buffer_; // Temp buffer when paging from disk.
  size_t totalPoints_;                 // Sum( points_[i] ).
  size_t maximumPoints_;               // Maximum number of points per scan.

  // Helpers:

  size_t computeMaximumPointsPerHour() const;

  virtual bool loadedDataSubset( const Timestamp firstTimestamp,
                                 const Timestamp lastTimestamp ) const;

  bool readBufferedDataSubset( const size_t first,
                               const bool checkAll ) const;

  bool readScans( FILE* stream, const size_t first, size_t last,
                  const bool checkAll ) const;

  bool probedPoint( const size_t scan,
                    const size_t skipPoints,
                    const double longitude,
                    const double latitude,
                    size_t& point ) const;

  virtual double vprobe( const Timestamp timestamp,
                         const double longitude,
                         const double latitude,
                         const double,
                         const int,
                         const int variable ) const;

  virtual std::vector<SubsetPointers> vsubset( const Timestamp beginTimestamp,
                                               const Timestamp endTimestamp,
                                               const int variable ) const;

  void readXDR( FILE* stream ); //x
  void readScanTimestamps( FILE* stream ); //x
  void readScanData( FILE* stream ); //x
  void writeXDR( FILE* stream ) const; //x
  void writeScanTimestamps( FILE* stream ) const; //x
  void writeScanPoints( FILE* stream ) const; //x
  void writeScanData( FILE* stream ) const; //x
  void writeASCII( FILE* stream ) const; //x
  void writeCOARDS( const std::string& fileName ) const; //x
  void writeBufferedCOARDSCoordinatesAndVariables( const int file ) const; //x
  void writeKML() const; //x

public:

  // Commands:

  virtual ~SwathDataset();
  SwathDataset( FILE* file ); //x
  SwathDataset( const std::string& fileName ); //x
  SwathDataset( const SwathDataset& other ); //x
  SwathDataset& operator=( const SwathDataset& other ); //x
  virtual Dataset* sample( const Dataset& other ) const; //x

  virtual Dataset* regrid( const int regridMethod,
                           const CMAQGridParameters&
                             cmaqGridParameters ) const; //x

  virtual void write( const std::string& directory,
                      const int format ) const; //x

  // Queries:

  virtual bool invariant() const;
  bool operator==( const SwathDataset& other ) const;
  bool operator!=( const SwathDataset& other ) const;
  size_t scans() const;
  Timestamp timestamp( const size_t scan ) const;
  size_t points( const size_t scan ) const;
};

} // namespace emvl.

#endif /* SWATHDATASET_H */



