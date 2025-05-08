#ifndef POINTDATASET_H
#define POINTDATASET_H

/******************************************************************************
PURPOSE: PointDataset.h - Dataset of 2D/3D point data.
HISTORY: 2018-11-06 plessel.todd@epa.gov
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

class PointDataset : public Dataset {
private:

  std::vector<Timestamp> timestamps_; // Timestamp of each point.
  std::vector<std::string> notes_;    // Notes per point (if any).
  std::vector<std::string> idNotes_; // IdNotes[ point ] if any.

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

  virtual const std::vector<std::string>& allDistinctNotes() const;
  virtual const Point* distinctNotePoint(const std::string& distinctNote) const;

  void readXDRHeader( FILE* stream ); //x
  void readXDRData( FILE* stream ); //x
  void writeXDR( FILE* stream ) const; //x
  void writeASCII( FILE* stream ) const; //x
  void writeCOARDS( const std::string& fileName ) const; //x
  void writeKML() const; //x

public:

  // Commands:

  virtual ~PointDataset();
  PointDataset( FILE* file ); //x
  PointDataset( const std::string& fileName ); //x
  PointDataset( const PointDataset& other ); //x
  PointDataset& operator=( const PointDataset& other ); //x
  virtual Dataset* sample( const Dataset& other ) const; //x

  virtual Dataset* regrid( const int regridMethod,
                           const CMAQGridParameters&
                             cmaqGridParameters ) const; //x

  virtual void write( const std::string& directory,
                      const int format ) const; //x

  // Queries:

  virtual bool invariant() const;
  bool operator==( const PointDataset& other ) const;
  bool operator!=( const PointDataset& other ) const;
  size_t notes() const;
  const std::string& note( const size_t point ) const;
};

} // namespace emvl.

#endif /* POINTDATASET_H */



