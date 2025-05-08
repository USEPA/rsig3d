#ifndef REGRIDDEDSWATHDATASET_H
#define REGRIDDEDSWATHDATASET_H

/******************************************************************************
PURPOSE: RegriddedSwathDataset.h - Dataset of regridded swath satellite data.
HISTORY: 2013-10-16 plessel.todd@epa.gov
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

class RegriddedSwathDataset : public RegriddedDataset {
private:

  friend class SwathDataset; // SwathDataset::regrid returns RegriddedSwathDataset
  RegriddedSwathDataset() {} // Called only by SwathDataset::regrid().

  // Helpers:

  void readXDR( FILE* stream ); //x
  void writeXDR( FILE* stream ) const; //x

public:

  // Commands:

  virtual ~RegriddedSwathDataset();
  RegriddedSwathDataset( FILE* file ); //x
  RegriddedSwathDataset( const std::string& fileName ); //x
  RegriddedSwathDataset( const RegriddedSwathDataset& other ); //x
  RegriddedSwathDataset& operator=( const RegriddedSwathDataset& other ); //x
  virtual Dataset* sample( const Dataset& other ) const; //x

  virtual Dataset* regrid( const int regridMethod,
                           const CMAQGridParameters&
                             cmaqGridParameters ) const; //x

  virtual void write( const std::string& directory,
                      const int format ) const; //x

  // Queries:

  virtual bool invariant() const;
  bool operator==( const RegriddedSwathDataset& other ) const;
  bool operator!=( const RegriddedSwathDataset& other ) const;
};

} // namespace emvl.

#endif /* REGRIDDEDSWATHDATASET_H */



