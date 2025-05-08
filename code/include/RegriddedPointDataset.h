#ifndef REGRIDDEDPOINTDATASET_H
#define REGRIDDEDPOINTDATASET_H

/******************************************************************************
PURPOSE: RegriddedPointDataset.h - Dataset of regridded 2D/3D point data.
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

class RegriddedPointDataset : public RegriddedDataset {
private:

  friend class PointDataset; // PointDataset::regrid returns RegriddedPointDataset
  RegriddedPointDataset() {} // Called only by PointDataset::regrid().

  // Helpers:

  void readXDR( FILE* stream ); //x
  void writeXDR( FILE* stream ) const; //x

public:

  // Commands:

  virtual ~RegriddedPointDataset();
  RegriddedPointDataset( FILE* file ); //x
  RegriddedPointDataset( const std::string& fileName ); //x
  RegriddedPointDataset( const RegriddedPointDataset& other ); //x
  RegriddedPointDataset& operator=( const RegriddedPointDataset& other ); //x
  virtual Dataset* sample( const Dataset& other ) const; //x

  virtual Dataset* regrid( const int regridMethod,
                           const CMAQGridParameters&
                             cmaqGridParameters ) const; //x

  virtual void write( const std::string& directory,
                      const int format ) const; //x

  // Queries:

  virtual bool invariant() const;
  bool operator==( const RegriddedPointDataset& other ) const;
  bool operator!=( const RegriddedPointDataset& other ) const;
};

} // namespace emvl.

#endif /* REGRIDDEDPOINTDATASET_H */



