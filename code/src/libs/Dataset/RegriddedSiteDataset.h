#ifndef REGRIDDEDSITEDATASET_H
#define REGRIDDEDSITEDATASET_H

/******************************************************************************
PURPOSE: RegriddedSiteDataset.h - Dataset of regridded site (ground station)
         data.
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
#include <RegriddedDataset.h>   // For RegriddedDataset.

//================================== TYPES ====================================

namespace emvl {

class RegriddedSiteDataset : public RegriddedDataset {
private:

  friend class SiteDataset; // SiteDataset::regrid returns RegriddedSiteDataset
  RegriddedSiteDataset() {} // Called only by SiteDataset::regrid().

  // Helpers:

  void readXDR( FILE* stream ); //x
  void writeXDR( FILE* stream ) const; //x

public:

  // Commands:

  virtual ~RegriddedSiteDataset();
  RegriddedSiteDataset( FILE* file ); //x
  RegriddedSiteDataset( const std::string& fileName ); //x
  RegriddedSiteDataset( const RegriddedSiteDataset& other ); //x
  RegriddedSiteDataset& operator=( const RegriddedSiteDataset& other ); //x
  virtual Dataset* sample( const Dataset& other ) const; //x

  virtual Dataset* regrid( const int regridMethod,
                           const CMAQGridParameters&
                             cmaqGridParameters ) const; //x

  virtual void write( const std::string& directory,
                      const int format ) const; //x

  // Queries:

  virtual bool invariant() const;
  bool operator==( const RegriddedSiteDataset& other ) const;
  bool operator!=( const RegriddedSiteDataset& other ) const;
};

} // namespace emvl.

#endif /* REGRIDDEDSITEDATASET_H */



