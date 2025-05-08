#ifndef LAMBERT_H
#define LAMBERT_H

/*****************************************************************************
PURPOSE: Lambert.h - Declare Lambert Conformal Conic MapProjection ADT.
HISTORY: 2013-01-27 plessel.todd@epa.gov
STATUS:  unreviewed tested
*****************************************************************************/

//================================ INCLUDES ==================================

#include <MapProjection.h> // For MapProjection.

//================================== TYPES ===================================

namespace emvl {
  
class Lambert : public MapProjection {
private:

  // Attributes:

  static const char* const name_;
  double centralLongitude_; // Longitude that projects to 0.
  double centralLatitude_;  // Latitude that projects to 0.
  double lowerLatitude_;    // Lower secant latitude.
  double upperLatitude_;    // Upper secant latitude.
  double eccentricity_;     // Of spheroid approximation of planet.
  double lambda0_;          // Central longitude in radians.
  double rho0_;             // See USGS PROJ Library.
  double n_;                // See USGS PROJ Library.
  double c_;                // See USGS PROJ Library.

  // Helpers:

  void computeDerivedTerms();

public:

  // Commands:

  virtual ~Lambert();
  Lambert( double majorSemiaxis, double minorSemiaxis,
           double centralLongitude, double centralLatitude,
           double lowerLatitude, double upperLatitude );
  Lambert( const Lambert& other );
  Lambert& operator=( const Lambert& other );

  // Queries:

  virtual MapProjection* newCopy() const;
  virtual bool invariant() const;
  virtual const char* name() const;
  bool operator==( const Lambert& other ) const;
  bool operator!=( const Lambert& other ) const;
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

#endif /* LAMBERT_H */



