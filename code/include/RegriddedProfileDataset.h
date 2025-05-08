#ifndef REGRIDDEDPROFILEDATASET_H
#define REGRIDDEDPROFILEDATASET_H

/******************************************************************************
PURPOSE: RegriddedProfileDataset.h - Dataset of regridded Profile data.
HISTORY: 2013-11-27 plessel.todd@epa.gov
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
#include <RegriddedDataset.h>   // For RegriddedDataset.

//================================== TYPES ====================================

namespace emvl {

class RegriddedProfileDataset : public RegriddedDataset {
private:

  friend class ProfileDataset; // ProfileDataset::regrid returns this.
  RegriddedProfileDataset() {} // Called only by ProfileDataset::regrid().

  // Helpers:

  void readXDR( FILE* stream ); //x
  void writeXDR( FILE* stream ) const; //x

public:

  // Commands:

  virtual ~RegriddedProfileDataset();
  RegriddedProfileDataset( FILE* file ); //x
  RegriddedProfileDataset( const std::string& fileName ); //x
  RegriddedProfileDataset( const RegriddedProfileDataset& other ); //x
  RegriddedProfileDataset& operator=( const RegriddedProfileDataset& other);//x
  virtual Dataset* sample( const Dataset& other ) const; //x

  virtual Dataset* regrid( const int regridMethod,
                           const CMAQGridParameters&
                             cmaqGridParameters ) const; //x

  virtual void write( const std::string& directory,
                      const int format ) const; //x

  // Queries:

  virtual bool invariant() const;
  virtual bool isSurfaceData() const; // Returns false.
  bool operator==( const RegriddedProfileDataset& other ) const;
  bool operator!=( const RegriddedProfileDataset& other ) const;
};

} // namespace emvl.

#endif /* REGRIDDEDPROFILEDATASET_H */



