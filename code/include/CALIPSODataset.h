#ifndef CALIPSODATASET_H
#define CALIPSODATASET_H

/******************************************************************************
PURPOSE: CALIPSODataset.h - Dataset of CALIPSO satellite data.
HISTORY: 2013-12-14 plessel.todd@epa.gov
STATUS:  inchoate
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

class CALIPSODataset : public Dataset {

private:

  std::vector<Timestamp> timestamps_;  // Timestamp of each scan.
  std::vector<Bounds> scanBounds_;     // Number of points in each scan.
  std::vector<size_t> scanDimensions_; // scanDimensions_[scans][ground vert].

  // Helpers:

  virtual double vprobe( const Timestamp timestamp,
                         const double longitude,
                         const double latitude,
                         const double elevation,
                         const int probeLayer,
                         const int variable ) const;

  virtual std::vector<SubsetPointers> vsubset( const Timestamp beginTimestamp,
                                               const Timestamp endTimestamp,
                                               const int variable ) const;

  void readXDR( FILE* stream ); //x
  void readScanTimestamps( FILE* stream ); //x
  void readScanData( FILE* stream, const bool hasThickness ); //x
  void writeXDR( FILE* stream ) const; //x
  void writeScanTimestamps( FILE* stream ) const; //x
  void writeScanPoints( FILE* stream ) const; //x
  void writeScanData( FILE* stream ) const; //x
  void writeASCII( FILE* stream ) const; //x
  void writeCOARDS( const std::string& fileName ) const; //x
  void writeKML() const; //x

public:

  // Commands:

  virtual ~CALIPSODataset();
  CALIPSODataset( FILE* file ); //x
  CALIPSODataset( const std::string& fileName ); //x
  CALIPSODataset( const CALIPSODataset& other ); //x
  CALIPSODataset& operator=( const CALIPSODataset& other ); //x
  virtual Dataset* sample( const Dataset& other ) const; //x

  virtual Dataset* regrid( const int regridMethod,
                           const CMAQGridParameters&
                             cmaqGridParameters ) const; //x

  virtual void write( const std::string& directory,
                      const int format ) const; //x

  // Queries:

  virtual bool invariant() const;
  virtual bool isSurfaceData() const;
  bool operator==( const CALIPSODataset& other ) const;
  bool operator!=( const CALIPSODataset& other ) const;
  size_t scans() const;
  Timestamp timestamp( const size_t scan ) const;
  size_t scanGroundPoints( const size_t scan ) const;
  size_t scanVerticalPoints( const size_t scan ) const;
  const Bounds& scanBounds( const size_t scan ) const;
};

} // namespace emvl.

#endif /* CALIPSODATASET_H */



