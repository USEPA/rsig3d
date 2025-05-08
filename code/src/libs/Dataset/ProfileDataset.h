#ifndef PROFILEDATASET_H
#define PROFILEDATASET_H

/******************************************************************************
PURPOSE: ProfileDataset.h - Dataset of Profile (e.g., spectrophotometer)
         ground station data.
HISTORY: 2013-11-27 plessel.todd@epa.gov
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

class ProfileDataset : public Dataset {

private:

  std::vector<Timestamp> timestamps_; // Timestamp of each data point.
  std::vector<size_t> points_;        // Number of points in each profile.
  std::vector<std::string> notes_;    // Notes per profile.

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

  void storeSubsetPointers( const int variable,
                            const size_t firstSubsetPoint,
                            const size_t subsetPointCount,
                            const size_t profile,
                          std::vector<Dataset::SubsetPointers>& output) const;

  virtual const std::vector<std::string>& allDistinctNotes() const;
  virtual const Point* distinctNotePoint(const std::string& distinctNote) const;

  void readXDR( FILE* stream ); //x
  void readProfileData( FILE* stream ); //x
  void writeXDR( FILE* stream ) const; //x
  void writeProfileData( FILE* stream ) const; //x
  void writeASCII( FILE* stream ) const; //x
  void writeCOARDS( const std::string& fileName ) const; //x
  void writeKML() const; //x

public:

  // Commands:

  virtual ~ProfileDataset();
  ProfileDataset( FILE* file ); //x
  ProfileDataset( const std::string& fileName ); //x
  ProfileDataset( const ProfileDataset& other ); //x
  ProfileDataset& operator=( const ProfileDataset& other ); //x
  virtual Dataset* sample( const Dataset& other ) const; //x

  virtual Dataset* regrid( const int regridMethod,
                           const CMAQGridParameters&
                             cmaqGridParameters ) const; //x

  virtual void write( const std::string& directory,
                      const int format ) const; //x

  // Queries:

  virtual bool invariant() const;
  virtual bool isSurfaceData() const; // Returns false.
  bool operator==( const ProfileDataset& other ) const;
  bool operator!=( const ProfileDataset& other ) const;
  size_t profiles() const;
  Timestamp timestamp( const size_t point ) const;
  size_t points( const size_t profile ) const;
  const std::string& note( const size_t profile ) const;
};

} // namespace emvl.

#endif /* PROFILEDATASET_H */



