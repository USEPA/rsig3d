#ifndef AIRCRAFTDATASET_H
#define AIRCRAFTDATASET_H

/******************************************************************************
PURPOSE: AircraftDataset.h - Dataset of Aircraft measured data.
HISTORY: 2013-10-21 plessel.todd@epa.gov
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

class AircraftDataset : public Dataset {

private:

  std::vector<Timestamp> timestamps_; // Timestamp of each point.
  std::vector<size_t> points_;        // Number of points in each track.
  std::vector<Bounds> trackBounds_;   // Bounds per track.
  std::vector<std::string> notes_;    // Notes per track.

  // Helpers:

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
  void readTrackData( FILE* stream ); //x
  void writeXDR( FILE* stream ) const; //x
  void writeTrackData( FILE* stream ) const; //x
  void writeASCII( FILE* stream ) const; //x
  void writeCOARDS( const std::string& fileName ) const; //x
  void writeKML() const; //x

public:

  // Commands:

  virtual ~AircraftDataset();
  AircraftDataset( FILE* file ); //x
  AircraftDataset( const std::string& fileName ); //x
  AircraftDataset( const AircraftDataset& other ); //x
  AircraftDataset& operator=( const AircraftDataset& other ); //x
  virtual Dataset* sample( const Dataset& other ) const; //x

  virtual Dataset* regrid( const int regridMethod,
                           const CMAQGridParameters&
                             cmaqGridParameters ) const; //x

  virtual void write( const std::string& directory,
                      const int format ) const; //x

  virtual const std::vector<std::string>& allDistinctNotes() const;

  // Queries:

  virtual bool invariant() const;
  virtual bool isSurfaceData() const; // Returns false.
  bool operator==( const AircraftDataset& other ) const;
  bool operator!=( const AircraftDataset& other ) const;
  size_t tracks() const;
  size_t points( const size_t track ) const;
  const std::string& note( const size_t track ) const;
  const Bounds& trackBounds( const size_t track ) const;
};

} // namespace emvl.

#endif /* AIRCRAFTDATASET_H */



