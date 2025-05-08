/******************************************************************************
PURPOSE: MapProjection.C - Define map projection ADT ABC.
HISTORY: 2013-01-27 plessel.todd@epa.gov
******************************************************************************/

//================================ INCLUDES ===================================

#include <float.h> // For DBL_MIN, DBLMAX.
#include <math.h>  // For M_PI, fabs(), sqrt().

#include <Assertions.h>    // For PRE0*(), POST0*(), IN_RANGE(), IS_BOOL().
#include <DataUtilities.h> // For isNan(), isLongitude(), isLatitude().
#include <MapProjection.h> // For public interface.

namespace emvl {

//============================= PUBLIC CONSTANTS ==============================
const double MapProjection::MAP_PROJECTION_TOLERANCE = 1e-8;
const double MapProjection::MAP_PROJECTION_CONVERGENCE_TOLERANCE = 1e-12;
const int MapProjection::MAP_PROJECTION_CONVERGENCE_MAXIMUM_ITERATIONS = 15;

const double MapProjection::AIRY_1830_MAJOR_SEMIAXIS = 6377563.4;
const double MapProjection::AIRY_1830_MINOR_SEMIAXIS = 6356256.9;
const double MapProjection::MODIFIED_AIRY_MAJOR_SEMIAXIS = 6377340.2;
const double MapProjection::MODIFIED_AIRY_MINOR_SEMIAXIS = 6356034.4;
const double MapProjection::ANDRAE_1876_MAJOR_SEMIAXIS = 6377104.4;
const double MapProjection::ANDRAE_1876_MINOR_SEMIAXIS = 6355847.4;
const double MapProjection::APPLIED_PHYSICS_1965_MAJOR_SEMIAXIS = 6378137.0;
const double MapProjection::APPLIED_PHYSICS_1965_MINOR_SEMIAXIS = 6356751.8;
const double MapProjection::AUSTRALIAN_NATL_SA_1969_MAJOR_SEMIAXIS = 6378160.0;
const double MapProjection::AUSTRALIAN_NATL_SA_1969_MINOR_SEMIAXIS = 6356774.7;
const double MapProjection::BESSEL_1841_MAJOR_SEMIAXIS = 6377397.2;
const double MapProjection::BESSEL_1841_MINOR_SEMIAXIS = 6356079.0;
const double MapProjection::BESSEL_NAMIBIA_1841_MAJOR_SEMIAXIS = 6377483.9;
const double MapProjection::BESSEL_NAMIBIA_1841_MINOR_SEMIAXIS = 6356165.4;
const double MapProjection::CLARKE_1866_MAJOR_SEMIAXIS = 6378206.4;
const double MapProjection::CLARKE_1866_MINOR_SEMIAXIS = 6356583.8;
const double MapProjection::CLARKE_1880_MAJOR_SEMIAXIS = 6378249.1;
const double MapProjection::CLARKE_1880_MINOR_SEMIAXIS = 6356515.0;
const double MapProjection::COMM_DES_POIDS_ET_MESURES_1799_MAJOR_SEMIAXIS =6375738.7;
const double MapProjection::COMM_DES_POIDS_ET_MESURES_1799_MINOR_SEMIAXIS =6356666.2;
const double MapProjection::DELAMBRE_1810_BELGIUM_MAJOR_SEMIAXIS = 6376428.0;
const double MapProjection::DELAMBRE_1810_BELGIUM_MINOR_SEMIAXIS = 6355957.9;
const double MapProjection::ENGELIS_1985_MAJOR_SEMIAXIS = 6378136.1;
const double MapProjection::ENGELIS_1985_MINOR_SEMIAXIS = 6356751.3;
const double MapProjection::EVEREST_1830_MAJOR_SEMIAXIS = 6377276.3;
const double MapProjection::EVEREST_1830_MINOR_SEMIAXIS = 6356075.4;
const double MapProjection::EVEREST_1948_MAJOR_SEMIAXIS = 6377304.1;
const double MapProjection::EVEREST_1948_MINOR_SEMIAXIS = 6356103.0;
const double MapProjection::EVEREST_1956_MAJOR_SEMIAXIS = 6377301.2;
const double MapProjection::EVEREST_1956_MINOR_SEMIAXIS = 6356100.2;
const double MapProjection::EVEREST_1969_MAJOR_SEMIAXIS = 6377295.7;
const double MapProjection::EVEREST_1969_MINOR_SEMIAXIS = 6356094.7;
const double MapProjection::EVEREST_SABAH_SARAWAK_MAJOR_SEMIAXIS = 6377298.6;
const double MapProjection::EVEREST_SABAH_SARAWAK_MINOR_SEMIAXIS = 6356097.6;
const double MapProjection::FISCHER_MERCURY_DATUM_1960_MAJOR_SEMIAXIS = 6378166.0;
const double MapProjection::FISCHER_MERCURY_DATUM_1960_MINOR_SEMIAXIS = 6356784.3;
const double MapProjection::MODIFIED_FISCHER_1960_MAJOR_SEMIAXIS = 6378155.0;
const double MapProjection::MODIFIED_FISCHER_1960_MINOR_SEMIAXIS = 6356773.3;
const double MapProjection::FISCHER_1968_MAJOR_SEMIAXIS = 6378150.0;
const double MapProjection::FISCHER_1968_MINOR_SEMIAXIS = 6356768.3;
const double MapProjection::GRS_IUGG_1967_MAJOR_SEMIAXIS = 6378160.0;
const double MapProjection::GRS_IUGG_1967_MINOR_SEMIAXIS = 6352363.3;
const double MapProjection::GRS_IUGG_1980_MAJOR_SEMIAXIS = 6378137.0;
const double MapProjection::GRS_IUGG_1980_MINOR_SEMIAXIS = 6356752.3;
const double MapProjection::HELMERT_1906_MAJOR_SEMIAXIS = 6378200.0;
const double MapProjection::HELMERT_1906_MINOR_SEMIAXIS = 6356818.2;
const double MapProjection::HOUGH_MAJOR_SEMIAXIS = 6378270.0;
const double MapProjection::HOUGH_MINOR_SEMIAXIS = 6356794.3;
const double MapProjection::IAU_1976_MAJOR_SEMIAXIS = 6378140.0;
const double MapProjection::IAU_1976_MINOR_SEMIAXIS = 6356755.3;
const double MapProjection::INTL_HAYFORD_1909_MAJOR_SEMIAXIS = 6378388.0;
const double MapProjection::INTL_HAYFORD_1909_MINOR_SEMIAXIS = 6356911.9;
const double MapProjection::KRASSOVSKY_1942_MAJOR_SEMIAXIS = 6378245.0;
const double MapProjection::KRASSOVSKY_1942_MINOR_SEMIAXIS = 6356863.0;
const double MapProjection::KAULA_1961_MAJOR_SEMIAXIS = 6378163.0;
const double MapProjection::KAULA_1961_MINOR_SEMIAXIS = 6356777.0;
const double MapProjection::LERCH_1979_MAJOR_SEMIAXIS = 6378139.0;
const double MapProjection::LERCH_1979_MINOR_SEMIAXIS = 6356754.3;
const double MapProjection::MAUPERTIUS_1738_MAJOR_SEMIAXIS = 6397300.0;
const double MapProjection::MAUPERTIUS_1738_MINOR_SEMIAXIS = 6363806.3;
const double MapProjection::MERIT_1983_MAJOR_SEMIAXIS = 6378137.0;
const double MapProjection::MERIT_1983_MINOR_SEMIAXIS = 6356752.3;
const double MapProjection::NAVAL_WEAPONS_LAB_1965_MAJOR_SEMIAXIS = 6378145.0;
const double MapProjection::NAVAL_WEAPONS_LAB_1965_MINOR_SEMIAXIS = 6356759.8;
const double MapProjection::NEW_INTERNATIONAL_1967_MAJOR_SEMIAXIS = 6378157.5;
const double MapProjection::NEW_INTERNATIONAL_1967_MINOR_SEMIAXIS = 6356772.2;
const double MapProjection::PLESSIS_1817_MAJOR_SEMIAXIS = 6376523.0;
const double MapProjection::PLESSIS_1817_MINOR_SEMIAXIS = 6355863.0;
const double MapProjection::SGS_1985_MAJOR_SEMIAXIS = 6378136.0;
const double MapProjection::SGS_1985_MINOR_SEMIAXIS = 6356751.3;
const double MapProjection::SOUTHEAST_ASIA_MAJOR_SEMIAXIS = 6378155.0;
const double MapProjection::SOUTHEAST_ASIA_MINOR_SEMIAXIS = 6356773.0;
const double MapProjection::WALBECK_MAJOR_SEMIAXIS = 6376896.0;
const double MapProjection::WALBECK_MINOR_SEMIAXIS = 6355835.0;
const double MapProjection::WGS_1960_MAJOR_SEMIAXIS = 6378165.0;
const double MapProjection::WGS_1960_MINOR_SEMIAXIS = 6356783.3;
const double MapProjection::WGS_1966_MAJOR_SEMIAXIS = 6378145.0;
const double MapProjection::WGS_1966_MINOR_SEMIAXIS = 6356759.8;
const double MapProjection::WGS_1972_MAJOR_SEMIAXIS = 6378135.0;
const double MapProjection::WGS_1972_MINOR_SEMIAXIS = 6356750.5;
const double MapProjection::WGS_1984_MAJOR_SEMIAXIS = 6378137.0;
const double MapProjection::WGS_1984_MINOR_SEMIAXIS = 6356752.3;
const double MapProjection::MM5_RADIUS = 6370997.0;
const double MapProjection::CMAQ_RADIUS = 637000.0;
const double MapProjection::MCIDAS_RADIUS = 6371230.0;
const double MapProjection::MOON_RADIUS = 1738000.0;
const double MapProjection::MARS_MAJOR_SEMIAXIS = 3394500.0;
const double MapProjection::MARS_MINOR_SEMIAXIS = 3376400.0;
const double MapProjection::VENUS_RADIUS = 6051000.0;

//============================= PUBLIC FUNCTIONS ==============================


/******************************************************************************
PURPOSE: ~MapProjection - Destruct a MapProjection.
******************************************************************************/

MapProjection::~MapProjection() {
  PRE( true );
  majorSemiaxis_ = 0.0;
  minorSemiaxis_ = 0.0;
}



/******************************************************************************
PURPOSE: isSpheroid  - Do the arguments define a valid spheroid planet shape?
INPUTS:  double majorSemiaxis Mean equitorial radius of planet approximation.
         double minorSemiaxis Mean polar      radius of planet approximation.
RETURNS: bool true if valid, else false.
******************************************************************************/

bool MapProjection::isSpheroid( double majorSemiaxis, double minorSemiaxis ) {
  const bool result =
    AND8( ! isNan( majorSemiaxis ),
          ! isNan( minorSemiaxis ),
          majorSemiaxis >= minorSemiaxis,
          majorSemiaxis >= 4.0 / 3.0,
          minorSemiaxis >= 1.0,
          majorSemiaxis < 1e15,
          minorSemiaxis < 1e15,
          minorSemiaxis / majorSemiaxis >= 0.75 );
  POST0( IS_BOOL( result ) );
  return result;
}



/******************************************************************************
PURPOSE: lambdaDifference - Tolerance-adjusted difference lambda - lambda0.
INPUTS:  double lambda   Angle (in radians) of longitude of point to project.
         double lambda0  Central longitude in radians.
RETURNS: double angle (in radians) of lambda - lambda0.
******************************************************************************/

double MapProjection::lambdaDifference( double lambda, double lambda0 ) {

  PRE02( IN_RANGE( lambda,  -M_PI + MAP_PROJECTION_TOLERANCE,
                             M_PI - MAP_PROJECTION_TOLERANCE ),
         IN_RANGE( lambda0, -M_PI, M_PI ) );

  const double tolerance = MAP_PROJECTION_TOLERANCE;
  double result = lambda - lambda0;

  /*
   * If result is too near +/-180 tweak it so that
   * unprojecting and projecting yields original point.
   */

  if ( IN_RANGE( result, -M_PI - tolerance, -M_PI + tolerance ) ) {
    result = -M_PI + tolerance;
  } else if ( IN_RANGE( lambda, M_PI - tolerance, M_PI + tolerance ) ) {
    result = M_PI - tolerance;
  } else {

    while ( ! IN_RANGE( result, -M_PI, M_PI ) ) {

      if ( result < 0.0 ) {
        result += M_PI + M_PI;
      } else {
        result -= M_PI + M_PI;
      }

      CHECK( emvl::isFinite( result ) );
    }
  }

  POST0( IN_RANGE( result, -M_PI, M_PI ) );
  return result;
}



/******************************************************************************
PURPOSE: toLambdaPhi - Convert (longitude, latitude) to point on
         sphere/spheroid in radians.
OUTPUTS: double longitude  Longitude in degrees.
         double latitude   Latitude  in degrees.
INPUTS:  double& lambda    Longitude in radians.
         double& phi       Latitude  in radians.
******************************************************************************/

void MapProjection::toLambdaPhi( double longitude, double latitude,
                                 double& lambda, double& phi ) {

  PRE02( isLongitude( longitude ), isLatitude( latitude ) );
  const double tolerance = 1e-6;

  /*
   * If longitude is too near +/-180 tweak it so that
   * unprojecting and projecting yields original point.
   */

  if ( IN_RANGE( longitude, -180.0, -180.0 + tolerance ) ) {
    lambda = -M_PI + tolerance;
  } else if ( IN_RANGE( longitude, 180.0 - tolerance, 180.0 ) ) {
    lambda = M_PI - tolerance;
  } else {
    lambda = radians( longitude  );
  }

  /*
   * If latitude is too near +/-90 tweak it so that
   * unprojecting and projecting yields original point.
   */

  if ( IN_RANGE( latitude, -90.0, -90.0 + tolerance ) ) {
    phi = -PI_OVER_2 + tolerance;
  } else if ( IN_RANGE( latitude, 90.0 - tolerance, 90.0 ) ) {
    phi = PI_OVER_2 - tolerance;
  } else {
    phi = radians( latitude  );
  }

  POST02( IN_RANGE( lambda, -M_PI + MAP_PROJECTION_TOLERANCE,
                             M_PI - MAP_PROJECTION_TOLERANCE ),
          IN_RANGE( phi, -PI_OVER_2 + MAP_PROJECTION_TOLERANCE,
                          PI_OVER_2 - MAP_PROJECTION_TOLERANCE ) );
}



/******************************************************************************
PURPOSE: toLongitudeLatitude - Convert point on sphere/spheroid to
         (longitude, latitude).
INPUTS:  double lambda      Longitude in radians.
         double phi         Latitude  in radians.
OUTPUTS: double& longitude  Longitude in degrees.
         double& latitude   Latitude  in degrees.
******************************************************************************/

void MapProjection::toLongitudeLatitude( double lambda, double phi,
                                         double& longitude, double& latitude) {

  PRE04( ! isNan( lambda ), isFinite( lambda ),
         ! isNan( phi ), isFinite( phi ) );
  const double tolerance = MAP_PROJECTION_TOLERANCE;
  longitude = degrees( lambda );
  latitude = degrees( phi );
  latitude = clampedToRange( latitude, -90.0, 90.0 );

  if ( IN_RANGE( longitude, -180.0 - tolerance, -180.0 ) ) {
    longitude = -180.0;
  } else if (IN_RANGE( longitude, 180.0, 180.0 + tolerance ) ) {
    longitude = 180.0;
  } else {

    while ( longitude < -180.0 ) {
      longitude += 360.0;
      CHECK( isFinite( longitude ) );
    }

    while ( longitude > 180.0 ) {
      longitude -= 360.0;
      CHECK( isFinite( longitude ) );
    }
  }

  POST02( isLongitude( longitude ), isLatitude( latitude ) );
}



/******************************************************************************
PURPOSE: ssfn - See USGS PROJ Library.
INPUTS:  double phi           Angle in radians.
         double sinePhi       Sine of phi.
         double eccentricity  Of planet approximation.
RETURNS: double See USGS PROJ Library.
******************************************************************************/

double MapProjection::ssfn( double phi, double sinePhi, double eccentricity ) {

  PRE07( ! isNan( phi ),
         ! isNan( sinePhi ),
         ! isNan( eccentricity ),
         sinePhi > -1.0,
         sinePhi < 1.0,
         aboutEqual( sinePhi, sin( phi ), MAP_PROJECTION_TOLERANCE ),
         IN_RANGE( eccentricity, 0.0, 0.7 ) );

  const double eccentricitySinePhi = eccentricity * sinePhi;
  const double exponent = eccentricity * 0.5;
  const double factor1 = tan( ( PI_OVER_2 + phi ) * 0.5 );
  const double factor2 = pow( ( ( 1.0 - eccentricitySinePhi ) /
                                ( 1.0 + eccentricitySinePhi ) ),
                              exponent );
  const double result = factor1 * factor2;

  POST0( ! isNan( result ) );
  return result;
}



/******************************************************************************
PURPOSE: tsfn - See USGS PROJ Library.
INPUTS:  double phi      Angle in radians.
         double sinePhi  Sine of phi.
         double eccentricity  Of planet approximation.
RETURNS: double See USGS PROJ Library.
******************************************************************************/

double MapProjection::tsfn( double phi, double sinePhi, double eccentricity ) {

  PRE010( ! isNan( phi ),
          ! isNan( sinePhi ),
          ! isNan( eccentricity ),
          sinePhi > -1.0,
          sinePhi < 1.0,
          aboutEqual( sinePhi, sin( phi ), MAP_PROJECTION_TOLERANCE ),
          IN_RANGE( eccentricity, 0.0, 0.7 ),
          tan( ( PI_OVER_2 - phi ) * 0.5 ) != 0.0,
          fabs( eccentricity * sinePhi ) != 1.0,
          ( 1.0 + eccentricity * sinePhi ) != 0.0 );

  const double eccentricitySinePhi = eccentricity * sinePhi;
  const double exponent = eccentricity * 0.5;
  const double numerator = tan( ( PI_OVER_2 - phi ) * 0.5 );
  const double denominator = pow( ( ( 1.0 - eccentricitySinePhi ) /
                                    ( 1.0 + eccentricitySinePhi ) ),
                                  exponent );
  const double result = numerator / denominator;

  POST02( ! isNan( result ), result != 0.0 );
  return result;
}



/******************************************************************************
PURPOSE: msfn - See USGS PROJ Library.
INPUTS:  double sinePhi    Sine of phi.
         double cosinePhi  Cosine of phi.
         double eccentricitySquared  Of planet approximation.
RETURNS: double See USGS PROJ Library.
******************************************************************************/

double MapProjection::msfn( double sinePhi, double cosinePhi,
                            double eccentricitySquared ) {

  PRE012( ! isNan( sinePhi ),
          ! isNan( cosinePhi ),
          ! isNan( eccentricitySquared ),
          sinePhi   > -1.0,
          sinePhi   < 1.0,
          cosinePhi > -1.0,
          cosinePhi < 1.0,
          cosinePhi != 0.0,
          aboutEqual( fabs( sinePhi ), sqrt( 1.0 - SQUARE( cosinePhi ) ),
                      MAP_PROJECTION_TOLERANCE ),
          IN_RANGE( eccentricitySquared, 0.0, 0.5 ),
          eccentricitySquared * SQUARE( sinePhi ) < 1.0,
          sqrt( 1.0 - eccentricitySquared * SQUARE( sinePhi ) ) != 0.0 );

  const double result =
    cosinePhi /  sqrt( 1.0 - eccentricitySquared * SQUARE( sinePhi ) );

  POST02( ! isNan( result ), result != 0.0 );
  return result;
}



/******************************************************************************
PURPOSE: qsfn - See USGS PROJ Library.
INPUTS:  double sinePhi                Sine of phi.
         double eccentricity           Of planet approximation.
         double oneMinusEccentricitySquared 1 - eccentricity^2.
RETURNS: double See USGS PROJ Library.
******************************************************************************/

double MapProjection::qsfn( double sinePhi, double eccentricity,
                            double oneMinusEccentricitySquared ) {

  PRE08( ! isNan( sinePhi ),
         ! isNan( eccentricity ),
         ! isNan( oneMinusEccentricitySquared ),
         sinePhi > -1.0,
         sinePhi < 1.0,
         IN_RANGE( eccentricity, 0.0, 0.7 ),
         IN_RANGE( oneMinusEccentricitySquared, 0.0, 1.0 ),
         aboutEqual( oneMinusEccentricitySquared,
                     1.0 - SQUARE( eccentricity ),
                     MAP_PROJECTION_TOLERANCE ) );

  double result = 0.0;

  if ( eccentricity < MAP_PROJECTION_TOLERANCE ) {
    result = sinePhi + sinePhi;
  } else {
    const double con = eccentricity * sinePhi;
    CHECK3( con != 1.0, con != -1.0, eccentricity != 0.0 );
    result =
      oneMinusEccentricitySquared *
        ( sinePhi / ( 1.0 - SQUARE( con ) ) -
          0.5 / eccentricity *
          log( ( 1.0 - con ) / ( 1.0 + con ) ) );
  }

  POST0( ! isNan( result ) );
  return result;
}



/******************************************************************************
PURPOSE: phi2Iterate - Iterate on unprojected y coordinate.
INPUTS:  double ts            See USGS PROJ Library.
         double eccentricity  Eccentrity of spheroid.
RETURNS: double converged phi.
******************************************************************************/

double MapProjection::phi2Iterate( double ts, double eccentricity ) {

  PRE0( IN_RANGE( eccentricity, 0.0, 0.7 ) );

  const double halfEccentricity = eccentricity * 0.5;
  double deltaPhi = 0.0;
  int iteration = 0;
  double result = PI_OVER_2 - 2.0 * atan( ts );

  do {
    const double con = eccentricity * sin( result );
    CHECK( con != -1.0 );
    deltaPhi =
      PI_OVER_2 -
      2.0 * atan( ts * pow( ( 1.0 - con ) / ( 1.0 + con ), halfEccentricity ) )
      - result;
    result += deltaPhi;
    ++iteration;
  } while ( AND2( fabs( deltaPhi ) >= MAP_PROJECTION_CONVERGENCE_TOLERANCE,
                  iteration < MAP_PROJECTION_CONVERGENCE_MAXIMUM_ITERATIONS ));

  POST0( ! isNan( result ) );
  return result;
}



/******************************************************************************
PURPOSE: invariant - Is object valid?
RETURNS: bool true if valid, else false and the code contains a defect.
******************************************************************************/

bool MapProjection::invariant() const {
  const bool result = isSpheroid( majorSemiaxis_, minorSemiaxis_ );
  return result;
}



/******************************************************************************
PURPOSE: majorSemiaxis - Mean equitorial radius of spheroid.
RETURNS: double mean equitorial radius of spheroid.
******************************************************************************/

double MapProjection::majorSemiaxis() const {
  PRE( true );
  const double result = majorSemiaxis_;
  POST3( ! isNan( result ),
         IN_RANGE( result, 4.0 / 3.0, 1e15 ),
         result >= minorSemiaxis_ );
  return result;
}



/******************************************************************************
PURPOSE: minorSemiaxis - Mean polar radius of spheroid.
RETURNS: double mean polar radius of spheroid.
******************************************************************************/

double MapProjection::minorSemiaxis() const {
  PRE( true );
  const double result = minorSemiaxis_;
  POST3( ! isNan( result ),
         IN_RANGE( result, 1.0, 1e15 ),
         result <= majorSemiaxis_ );
  return result;
}



} // namespace emvl.



