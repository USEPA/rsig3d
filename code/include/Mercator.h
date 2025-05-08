#ifndef MERCATOR_H
#define MERCATOR_H

/*****************************************************************************
PURPOSE: Mercator.h - Declare Mercator MapProjection ADT.
HISTORY: 2013-02-05 plessel.todd@epa.gov
STATUS:  unreviewed tested
*****************************************************************************/

//================================ INCLUDES ==================================

#include <MapProjection.h> // For MapProjection.

//================================== TYPES ===================================

namespace emvl {
  
class Mercator : public MapProjection {
private:

  // Attributes:

  static const char* const name_;
  double centralLongitude_; // Longitude that projects to 0.
  double eccentricity_;     // Of spheroid approximation of planet.
  double lambda0_;          // Central longitude in radians.

  // Helpers:

  void computeDerivedTerms();

public:

  // Commands:

  virtual ~Mercator();
  Mercator( double majorSemiaxis, double minorSemiaxis,
            double centralLongitude );
  Mercator( const Mercator& other );
  Mercator& operator=( const Mercator& other );

  // Queries:

  virtual MapProjection* newCopy() const;
  virtual bool invariant() const;
  virtual const char* name() const;
  bool operator==( const Mercator& other ) const;
  bool operator!=( const Mercator& other ) const;
  virtual double centralLongitude() const;
  virtual double centralLatitude() const;
  virtual void project( double longitude, double latitude,
                        double& x, double& y ) const;
  virtual void unproject( double x, double y,
                          double& longitude, double& latitude ) const;
  double lowerLatitude() const;
  double upperLatitude() const;
};


} // namespace emvl.

#endif /* MERCATOR_H */



