#ifndef MAPPROJECTION_H
#define MAPPROJECTION_H

/*****************************************************************************
PURPOSE: MapProjection.h - Declare map projection ADT ABC.
HISTORY: 2013-01-27 plessel.todd@epa.gov
STATUS:  unreviewed tested
*****************************************************************************/

//================================= MACROS ===================================

#define PI_OVER_2 1.57079632679489661923
#define PI_OVER_4 0.78539816339744830962
#define SQUARE(x) ((x)*(x))

//================================= TYPES ====================================

namespace emvl {

class MapProjection {
protected:

  double majorSemiaxis_; // Equitorial radius spheroid approximation of planet.
  double minorSemiaxis_; // Polar radius of spheroid approximation of planet.

public:

  // Constants:

  static const double MAP_PROJECTION_TOLERANCE;
  static const double MAP_PROJECTION_CONVERGENCE_TOLERANCE;
  static const int MAP_PROJECTION_CONVERGENCE_MAXIMUM_ITERATIONS;

  static const double AIRY_1830_MAJOR_SEMIAXIS;
  static const double AIRY_1830_MINOR_SEMIAXIS;
  static const double MODIFIED_AIRY_MAJOR_SEMIAXIS;
  static const double MODIFIED_AIRY_MINOR_SEMIAXIS;
  static const double ANDRAE_1876_MAJOR_SEMIAXIS;
  static const double ANDRAE_1876_MINOR_SEMIAXIS;
  static const double APPLIED_PHYSICS_1965_MAJOR_SEMIAXIS;
  static const double APPLIED_PHYSICS_1965_MINOR_SEMIAXIS;
  static const double AUSTRALIAN_NATL_SA_1969_MAJOR_SEMIAXIS;
  static const double AUSTRALIAN_NATL_SA_1969_MINOR_SEMIAXIS;
  static const double BESSEL_1841_MAJOR_SEMIAXIS;
  static const double BESSEL_1841_MINOR_SEMIAXIS;
  static const double BESSEL_NAMIBIA_1841_MAJOR_SEMIAXIS;
  static const double BESSEL_NAMIBIA_1841_MINOR_SEMIAXIS;
  static const double CLARKE_1866_MAJOR_SEMIAXIS;
  static const double CLARKE_1866_MINOR_SEMIAXIS;
  static const double CLARKE_1880_MAJOR_SEMIAXIS;
  static const double CLARKE_1880_MINOR_SEMIAXIS;
  static const double COMM_DES_POIDS_ET_MESURES_1799_MAJOR_SEMIAXIS;
  static const double COMM_DES_POIDS_ET_MESURES_1799_MINOR_SEMIAXIS;
  static const double DELAMBRE_1810_BELGIUM_MAJOR_SEMIAXIS;
  static const double DELAMBRE_1810_BELGIUM_MINOR_SEMIAXIS;
  static const double ENGELIS_1985_MAJOR_SEMIAXIS;
  static const double ENGELIS_1985_MINOR_SEMIAXIS;
  static const double EVEREST_1830_MAJOR_SEMIAXIS;
  static const double EVEREST_1830_MINOR_SEMIAXIS;
  static const double EVEREST_1948_MAJOR_SEMIAXIS;
  static const double EVEREST_1948_MINOR_SEMIAXIS;
  static const double EVEREST_1956_MAJOR_SEMIAXIS;
  static const double EVEREST_1956_MINOR_SEMIAXIS;
  static const double EVEREST_1969_MAJOR_SEMIAXIS;
  static const double EVEREST_1969_MINOR_SEMIAXIS;
  static const double EVEREST_SABAH_SARAWAK_MAJOR_SEMIAXIS;
  static const double EVEREST_SABAH_SARAWAK_MINOR_SEMIAXIS;
  static const double FISCHER_MERCURY_DATUM_1960_MAJOR_SEMIAXIS;
  static const double FISCHER_MERCURY_DATUM_1960_MINOR_SEMIAXIS;
  static const double MODIFIED_FISCHER_1960_MAJOR_SEMIAXIS;
  static const double MODIFIED_FISCHER_1960_MINOR_SEMIAXIS;
  static const double FISCHER_1968_MAJOR_SEMIAXIS;
  static const double FISCHER_1968_MINOR_SEMIAXIS;
  static const double GRS_IUGG_1967_MAJOR_SEMIAXIS;
  static const double GRS_IUGG_1967_MINOR_SEMIAXIS;
  static const double GRS_IUGG_1980_MAJOR_SEMIAXIS;
  static const double GRS_IUGG_1980_MINOR_SEMIAXIS;
  static const double HELMERT_1906_MAJOR_SEMIAXIS;
  static const double HELMERT_1906_MINOR_SEMIAXIS;
  static const double HOUGH_MAJOR_SEMIAXIS;
  static const double HOUGH_MINOR_SEMIAXIS;
  static const double IAU_1976_MAJOR_SEMIAXIS;
  static const double IAU_1976_MINOR_SEMIAXIS;
  static const double INTL_HAYFORD_1909_MAJOR_SEMIAXIS;
  static const double INTL_HAYFORD_1909_MINOR_SEMIAXIS;
  static const double KRASSOVSKY_1942_MAJOR_SEMIAXIS;
  static const double KRASSOVSKY_1942_MINOR_SEMIAXIS;
  static const double KAULA_1961_MAJOR_SEMIAXIS;
  static const double KAULA_1961_MINOR_SEMIAXIS;
  static const double LERCH_1979_MAJOR_SEMIAXIS;
  static const double LERCH_1979_MINOR_SEMIAXIS;
  static const double MAUPERTIUS_1738_MAJOR_SEMIAXIS;
  static const double MAUPERTIUS_1738_MINOR_SEMIAXIS;
  static const double MERIT_1983_MAJOR_SEMIAXIS;
  static const double MERIT_1983_MINOR_SEMIAXIS;
  static const double NAVAL_WEAPONS_LAB_1965_MAJOR_SEMIAXIS;
  static const double NAVAL_WEAPONS_LAB_1965_MINOR_SEMIAXIS;
  static const double NEW_INTERNATIONAL_1967_MAJOR_SEMIAXIS;
  static const double NEW_INTERNATIONAL_1967_MINOR_SEMIAXIS;
  static const double PLESSIS_1817_MAJOR_SEMIAXIS;
  static const double PLESSIS_1817_MINOR_SEMIAXIS;
  static const double SGS_1985_MAJOR_SEMIAXIS;
  static const double SGS_1985_MINOR_SEMIAXIS;
  static const double SOUTHEAST_ASIA_MAJOR_SEMIAXIS;
  static const double SOUTHEAST_ASIA_MINOR_SEMIAXIS;
  static const double WALBECK_MAJOR_SEMIAXIS;
  static const double WALBECK_MINOR_SEMIAXIS;
  static const double WGS_1960_MAJOR_SEMIAXIS;
  static const double WGS_1960_MINOR_SEMIAXIS;
  static const double WGS_1966_MAJOR_SEMIAXIS;
  static const double WGS_1966_MINOR_SEMIAXIS;
  static const double WGS_1972_MAJOR_SEMIAXIS;
  static const double WGS_1972_MINOR_SEMIAXIS;
  static const double WGS_1984_MAJOR_SEMIAXIS;
  static const double WGS_1984_MINOR_SEMIAXIS;
  static const double MM5_RADIUS;
  static const double CMAQ_RADIUS;
  static const double MCIDAS_RADIUS;
  static const double MOON_RADIUS;
  static const double MARS_MAJOR_SEMIAXIS;
  static const double MARS_MINOR_SEMIAXIS;
  static const double VENUS_RADIUS;

  // Commands:

  virtual ~MapProjection() = 0; // Non-instantiable Abstract Base Class.

  // Queries:

  static bool isSpheroid( double majorSemiaxis, double minorSemiaxis );
  static double lambdaDifference( double lambda, double lambda0 );
  static void toLambdaPhi( double longitude, double latitude,
                           double& lambda, double& phi );
  static void toLongitudeLatitude( double lambda, double phi,
                                   double& longitude, double& latitude );

  static double ssfn( double phi, double sinePhi, double eccentricity );
  static double tsfn( double phi, double sinePhi, double eccentricity );
  static double msfn( double sinePhi, double cosinePhi,
                      double eccentricitySquared );
  static double qsfn( double sinePhi, double eccentricity,
                      double oneMinusEccentricitySquared );
  static double phi2Iterate( double ts, double eccentricity );

  virtual MapProjection* newCopy() const = 0;
  virtual bool invariant() const;
  virtual const char* name() const = 0;
  double majorSemiaxis() const;
  double minorSemiaxis() const;
  virtual double centralLongitude() const = 0;
  virtual double centralLatitude() const = 0;
  virtual void project( double longitude, double latitude,
                        double& x, double& y ) const = 0;
  virtual void unproject( double x, double y,
                          double& longitude, double& latitude ) const = 0;
};

} // namespace emvl.

#endif /* MAPPROJECTION_H */



