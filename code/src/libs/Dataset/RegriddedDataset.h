#ifndef REGRIDDEDDATASET_H
#define REGRIDDEDDATASET_H

/******************************************************************************
PURPOSE: RegriddedDataset.h - RegriddedDataset non-instantiable mixin.
HISTORY: 2013-12-18 plessel.todd@epa.gov
STATUS:  unreviewed tested
******************************************************************************/

//================================ INCLUDES ===================================

#include <vector>  // For std::vector.

#include <MapProjection.h>      // For MapProjection.
#include <CMAQGridParameters.h> // For CMAQGridParameters.
#include <Dataset.h>            // For Dataset.

//================================== TYPES ====================================

namespace emvl {

class RegriddedDataset : public Dataset {
protected:
  std::vector<size_t> pointsPerTimestep_; // pointsPerTimestep_[ timesteps_ ].
  size_t totalPoints_;                    // Sum of pointsPerTimestep_.
  std::vector<std::string> notes_;        // notes[ totalPoints ] Note or empty.
  CMAQGridParameters gridParameters_;     // Of regridding.
  MapProjection* mapProjection_;          // Or 0 if lon-lat grid.

  RegriddedDataset();
  RegriddedDataset( const RegriddedDataset& other );
  RegriddedDataset& operator=( const RegriddedDataset& other );

  virtual double vprobe( const Timestamp timestamp,
                         const double longitude,
                         const double latitude,
                         const double elevation,
                         const int probeLayer,
                         const int variable ) const;

  virtual std::vector<SubsetPointers> vsubset( const Timestamp beginTimestamp,
                                               const Timestamp endTimestamp,
                                               const int variable ) const;

  /*
   * Above vsubset() calls the below virtual subsetNotes().
   * The default implementation returns 0.
   * Derived classes may implement subsetNotes() to return notes pointers.
   */

  virtual const std::string* subsetNotes( const size_t skipPoints ) const;

  void copy( const RegriddedDataset& other ); //x
  bool equal( const RegriddedDataset& other ) const;
  void computeCMAQCellCoordinates( const size_t layersPerPoint,
                                   const double centerElevations[] );
  void computeCMAQGridElevations( std::vector<float>& elevations ) const; //x
  void writeIOAPICoordinates( const int file ) const; //x
  void writeIOAPIVariable( const int file ) const; //x
  void writeIOAPI( const std::string& fileName ) const; //x
  std::vector<Timestamp> timestampsPerDataPoint() const; //x
  virtual void writeASCII( FILE* stream ) const; //x
  virtual void writeCOARDS( const std::string& fileName ) const; //x
  virtual void writeKML() const; //x
public:
  virtual ~RegriddedDataset();
  virtual bool invariant() const;
};

} // namespace emvl.

#endif /* REGRIDDEDDATASET_H */



