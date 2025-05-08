#ifndef REGRIDDEDCALIPSODATASET_H
#define REGRIDDEDCALIPSODATASET_H

/******************************************************************************
PURPOSE: RegriddedCALIPSODataset.h - Dataset of regridded CALIPSO satellite
         data.
HISTORY: 2013-12-14 plessel.todd@epa.gov
STATUS:  inchoate
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

class RegriddedCALIPSODataset : public RegriddedDataset {
private:

  size_t layers_;              // Layers of points.
  friend class CALIPSODataset; // CALIPSODataset::regrid returns this.
  RegriddedCALIPSODataset() {} // Called only by CALIPSODataset::regrid().
  void readXDR( FILE* stream ); //x
  void writeXDR( FILE* stream ) const; //x
  void writeCoordinates( FILE* stream ) const; //x
  void writeData( FILE* stream ) const; //x

public:

  // Commands:

  virtual ~RegriddedCALIPSODataset();
  RegriddedCALIPSODataset( FILE* file ); //x
  RegriddedCALIPSODataset( const std::string& fileName ); //x
  RegriddedCALIPSODataset( const RegriddedCALIPSODataset& other ); //x
  RegriddedCALIPSODataset& operator=( const RegriddedCALIPSODataset& other);//x
  virtual Dataset* sample( const Dataset& other ) const; //x

  virtual Dataset* regrid( const int regridMethod,
                           const CMAQGridParameters&
                             cmaqGridParameters ) const; //x

  virtual void write( const std::string& directory,
                      const int format ) const; //x

  // Queries:

  virtual bool invariant() const;
  virtual size_t layers() const;      // Returns layers_.
  virtual bool isSurfaceData() const; // Returns layers_ == 1.
  bool operator==( const RegriddedCALIPSODataset& other ) const;
  bool operator!=( const RegriddedCALIPSODataset& other ) const;
};

} // namespace emvl.

#endif /* REGRIDDEDCALIPSODATASET_H */



