#ifndef ALBERS_H
#define ALBERS_H

/*****************************************************************************
PURPOSE: Albers.h - Declare Albers MapProjection ADT.
HISTORY: 2013-02-05 plessel.todd@epa.gov
STATUS:  unreviewed tested
*****************************************************************************/

//================================ INCLUDES ==================================

#include <MapProjection.h> // For MapProjection.

//================================== TYPES ===================================

namespace emvl {
  
class Albers : public MapProjection {
private:

  // Attributes:

  static const char* const name_;
  double centralLongitude_; // Longitude that projects to 0.
  double centralLatitude_;  // Latitude that projects to 0.
  double lowerLatitude_;    // Lower secant latitude.
  double upperLatitude_;    // Upper secant latitude.
  double eccentricity_;     // Of spheroid approximation of planet.
  double oneMinusEccentricitySquared_;
  double lambda0_;           // Central longitude in radians.
  double rho0_;              // See USGS PROJ Library.
  double n_;                 // See USGS PROJ Library.
  double n2_;                // See USGS PROJ Library.
  double c_;                 // See USGS PROJ Library.
  double ec_;                // See USGS PROJ Library.
  double dd_;                // See USGS PROJ Library.

  // Helpers:

  void computeDerivedTerms();

public:

  // Commands:

  virtual ~Albers();
  Albers( double majorSemiaxis, double minorSemiaxis,
           double centralLongitude, double centralLatitude,
           double lowerLatitude, double upperLatitude );
  Albers( const Albers& other );
  Albers& operator=( const Albers& other );

  // Queries:

  virtual MapProjection* newCopy() const;
  virtual bool invariant() const;
  virtual const char* name() const;
  bool operator==( const Albers& other ) const;
  bool operator!=( const Albers& other ) const;
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

#endif /* ALBERS_H */



