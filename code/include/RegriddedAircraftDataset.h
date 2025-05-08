#ifndef REGRIDDEDAIRCRAFTDATASET_H
#define REGRIDDEDAIRCRAFTDATASET_H

/******************************************************************************
PURPOSE: RegriddedAircraftDataset.h - Dataset for Aircraft measured data.
HISTORY: 2013-10-21 plessel.todd@epa.gov
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

class RegriddedAircraftDataset : public RegriddedDataset {
private:

  friend class AircraftDataset; // AircraftDataset::regrid() returns this.
  RegriddedAircraftDataset() {} // Called only by AircraftDataset::regrid().

  // RegriddedDataset::vsubset() calls this virtual subsetNotes( skipPoints )
  // which returns &notes_[ skipPoints ]:

  virtual const std::string* subsetNotes( const size_t skipPoints ) const;
  void readXDR( FILE* stream ); //x
  void writeXDR( FILE* stream ) const; //x
  virtual void writeASCII( FILE* stream ) const; //x
  virtual void writeCOARDS( const std::string& fileName ) const; //x

public:

  // Commands:

  virtual ~RegriddedAircraftDataset();
  RegriddedAircraftDataset( FILE* file ); //x
  RegriddedAircraftDataset( const std::string& fileName ); //x
  RegriddedAircraftDataset( const RegriddedAircraftDataset& other ); //x
  RegriddedAircraftDataset& operator=(
                                  const RegriddedAircraftDataset& other ); //x
  virtual Dataset* sample( const Dataset& other ) const; //x

  virtual Dataset* regrid( const int regridMethod,
                           const CMAQGridParameters&
                             cmaqGridParameters ) const; //x

  virtual void write( const std::string& directory,
                      const int format ) const; //x

  // Queries:

  virtual bool invariant() const;
  virtual bool isSurfaceData() const; // Returns false.
  bool operator==( const RegriddedAircraftDataset& other ) const;
  bool operator!=( const RegriddedAircraftDataset& other ) const;
};

} // namespace emvl.

#endif /* REGRIDDEDAIRCRAFTDATASET_H */



