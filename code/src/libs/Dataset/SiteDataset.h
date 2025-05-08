#ifndef SITEDATASET_H
#define SITEDATASET_H

/******************************************************************************
PURPOSE: SiteDataset.h - Dataset of site (ground station) data.
HISTORY: 2013-02-20 plessel.todd@epa.gov
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

class SiteDataset : public Dataset {
private:

  size_t stations_;                // Number of stations.
  std::vector<std::string> notes_; // Notes per station.
  std::vector<std::string> idNotes_; // IdNotes[ stations ].

  // Helpers:

  bool findNearestStation( const double longitude,
                           const double latitude,
                           size_t& nearestStation ) const;

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

  virtual ~SiteDataset();
  SiteDataset( FILE* file ); //x
  SiteDataset( const std::string& fileName ); //x
  SiteDataset( const SiteDataset& other ); //x
  SiteDataset& operator=( const SiteDataset& other ); //x
  virtual Dataset* sample( const Dataset& other ) const; //x

  virtual Dataset* regrid( const int regridMethod,
                           const CMAQGridParameters&
                             cmaqGridParameters ) const; //x

  virtual void write( const std::string& directory,
                      const int format ) const; //x

  // Queries:

  virtual bool invariant() const;
  bool operator==( const SiteDataset& other ) const;
  bool operator!=( const SiteDataset& other ) const;
  size_t stations() const;
  const std::string& note( const size_t station ) const;
};

} // namespace emvl.

#endif /* SITEDATASET_H */



