#ifndef STEREOGRAPHIC_H
#define STEREOGRAPHIC_H

/*****************************************************************************
PURPOSE: Stereographic.h - Declare Stereographic MapProjection ADT.
HISTORY: 2013-02-03 plessel.todd@epa.gov
STATUS:  unreviewed untested
*****************************************************************************/

//================================ INCLUDES ==================================

#include <MapProjection.h> // For MapProjection.

//================================== TYPES ===================================

namespace emvl {
  
class Stereographic : public MapProjection {
private:

  // Attributes:

  static const char* const name_;
  double centralLongitude_; // Longitude that projects to 0.
  double centralLatitude_;  // Latitude that projects to 0.
  double secantLatitude_;   // Secant latitude of plane cutting the spheroid.
  double eccentricity_;     // Of spheroid approximation of planet.
  double lambda0_;          // Central longitude in radians.
  double phi0_;             // Secant latitude in radians.
  double sineX1_;           // See USGS PROJ Library.
  double cosineX1_;         // See USGS PROJ Library.
  double akm1_;             // See USGS PROJ Library.
  double projectedCenterX_; // Projected central longitude.
  double projectedCenterY_; // Projected central latitude.
  int subtype_;             // North polar, etc.

  // Helpers:

  void computeDerivedTerms();

  void projectSpheroid( double phi, double sineLambda, double cosineLambda,
                        double sinePhi, double& x, double& y ) const;


  void projectSphere(  double phi, double sineLambda, double cosineLambda,
                       double sinePhi, double& x, double& y ) const;

  void unprojectSpheroid( double xp, double yp, double rho,
                          double& lambda, double& phi ) const;

  void unprojectSphere( double xp, double yp, double rho,
                        double& lambda, double& phi ) const;

public:

  // Commands:

  virtual ~Stereographic();
  Stereographic( double majorSemiaxis, double minorSemiaxis,
                 double centralLongitude, double centralLatitude,
                 double secantLatitude );
  Stereographic( const Stereographic& other );
  Stereographic& operator=( const Stereographic& other );

  // Queries:

  virtual MapProjection* newCopy() const;
  virtual bool invariant() const;
  virtual const char* name() const;
  bool operator==( const Stereographic& other ) const;
  bool operator!=( const Stereographic& other ) const;
  virtual double centralLongitude() const;
  virtual double centralLatitude() const;
  virtual void project( double longitude, double latitude,
                        double& x, double& y ) const;
  virtual void unproject( double x, double y,
                          double& longitude, double& latitude ) const;
  double secantLatitude() const;
};


} // namespace emvl.

#endif /* STEREOGRAPHIC_H */



