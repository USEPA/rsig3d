#ifndef REGRIDDEDGRIDDATASET_H
#define REGRIDDEDGRIDDATASET_H

/******************************************************************************
PURPOSE: RegriddedGridDataset.h - Dataset of regridded grid points data.
HISTORY: 2020-04-01 plessel.todd@epa.gov
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

class RegriddedGridDataset : public RegriddedDataset {
private:

  friend class GridDataset; // GridDataset::regrid returns RegriddedGridDataset
  RegriddedGridDataset() {} // Called only by GridDataset::regrid().

  // Helpers:

  void readXDR( FILE* stream ); //x
  void writeXDR( FILE* stream ) const; //x

public:

  // Commands:

  virtual ~RegriddedGridDataset();
  RegriddedGridDataset( FILE* file ); //x
  RegriddedGridDataset( const std::string& fileName ); //x
  RegriddedGridDataset( const RegriddedGridDataset& other ); //x
  RegriddedGridDataset& operator=( const RegriddedGridDataset& other ); //x
  virtual Dataset* sample( const Dataset& other ) const; //x

  virtual Dataset* regrid( const int regridMethod,
                           const CMAQGridParameters&
                             cmaqGridParameters ) const; //x

  virtual void write( const std::string& directory,
                      const int format ) const; //x

  // Queries:

  virtual bool invariant() const;
  bool operator==( const RegriddedGridDataset& other ) const;
  bool operator!=( const RegriddedGridDataset& other ) const;
};

} // namespace emvl.

#endif /* REGRIDDEDGRIDDATASET_H */



