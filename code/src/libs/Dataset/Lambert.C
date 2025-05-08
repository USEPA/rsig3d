/******************************************************************************
PURPOSE: Lambert.C - Define Lambert Conformal Conic MapProjection ADT.
NOTES:   Derived from USGS PROJ Library.
HISTORY: 2013-01-27 plessel.todd@epa.gov
******************************************************************************/

//================================ INCLUDES ===================================

#include <string.h> // For strcmp().
#include <math.h>   // For M_PI, sqrt(), sin(), cos(), atan2(), log(), pow().
#include <float.h>  // For DBL_MAX.

#include <Assertions.h>    // For PRE*(), POST*(), CHECK*(), IN_RANGE().
#include <DataUtilities.h> // For isNan(), aboutEqual(), isLongitude().
#include <Lambert.h>       // For public interface.

//=========================== FORWARD DECLARATIONS ============================

namespace emvl {

const char* const Lambert::name_ = "Lambert"; // Initialize static attribute.

//============================= PUBLIC FUNCTIONS ==============================



/******************************************************************************
PURPOSE: ~Lambert - Destruct a Lambert.
******************************************************************************/

Lambert::~Lambert() {
  PRE( true );
  centralLongitude_ = 0.0;
  centralLatitude_  = 0.0;
  lowerLatitude_    = 0.0;
  upperLatitude_    = 0.0;
  eccentricity_     = 0.0;
  lambda0_          = 0.0;
  rho0_             = 0.0;
  n_                = 0.0;
  c_                = 0.0;
}



/******************************************************************************
PURPOSE: Lambert - Construct a Lambert.
INPUTS:  double majorSemiaxis    Mean equitorial radius of spheroid in meters.
         double minorSemiaxis    Mean polar      radius of spheroid in meters.
         double centralLongitude Longitude that projects to 0.
         double centralLatitude  Latitude that projects to 0.
         double lowerLatitude    Latitude of lower secant.
         double upperLatitude    latitude of upper secant.
******************************************************************************/

Lambert::Lambert( double majorSemiaxis, double minorSemiaxis,
                  double centralLongitude, double centralLatitude,
                  double lowerLatitude, double upperLatitude ) {

  PRE010( isSpheroid( majorSemiaxis, minorSemiaxis ),
          isLongitude( centralLongitude ),
          isLatitude( centralLatitude ),
          IN_RANGE( centralLatitude, -89.0, 89.0 ),
          isLatitude( lowerLatitude ),
          isLatitude( upperLatitude ),
          lowerLatitude <= upperLatitude,
          SIGN( lowerLatitude ) == SIGN( upperLatitude ),
          IMPLIES_ELSE( lowerLatitude >= 0.0,
                        IN_RANGE( lowerLatitude, 1.0, 89.0 ),
                        IN_RANGE( lowerLatitude, -89.0, -1.0 ) ),
          IMPLIES_ELSE( upperLatitude >= 0.0,
                        IN_RANGE( upperLatitude, 1.0, 89.0 ),
                        IN_RANGE( upperLatitude, -89.0, -1.0 ) ) );

  majorSemiaxis_    = majorSemiaxis;
  minorSemiaxis_    = minorSemiaxis;
  centralLongitude_ = centralLongitude;
  centralLatitude_  = centralLatitude;
  lowerLatitude_    = lowerLatitude;
  upperLatitude_    = upperLatitude;
  computeDerivedTerms();

  POST7( ! strcmp( this->name(), "Lambert" ),
         this->majorSemiaxis() == majorSemiaxis,
         this->minorSemiaxis() == minorSemiaxis,
         this->centralLongitude() == centralLongitude,
         this->centralLatitude() == centralLatitude,
         this->lowerLatitude() == lowerLatitude,
         this->upperLatitude() == upperLatitude );
}



/******************************************************************************
PURPOSE: Lambert - Copy construct a Lambert.
INPUTS:  const Lambert& other  Other object to copy.
******************************************************************************/

Lambert::Lambert( const Lambert& other ) {

  PRE0( other.invariant() );

  majorSemiaxis_    = other.majorSemiaxis_;
  minorSemiaxis_    = other.minorSemiaxis_;
  centralLongitude_ = other.centralLongitude_;
  centralLatitude_  = other.centralLatitude_;
  lowerLatitude_    = other.lowerLatitude_;
  upperLatitude_    = other.upperLatitude_;
  eccentricity_     = other.eccentricity_;
  lambda0_          = other.lambda0_;
  rho0_             = other.rho0_;
  n_                = other.n_;
  c_                = other.c_;

  POST( *this == other );
}



/******************************************************************************
PURPOSE: operator= - Assign a Lambert.
INPUTS:  const Lambert& other  Other object to copy.
RETURNS: Lambert& *this.
******************************************************************************/

Lambert& Lambert::operator=( const Lambert& other ) {

  PRE0( other.invariant() );

  if ( this != &other ) {
    majorSemiaxis_    = other.majorSemiaxis_;
    minorSemiaxis_    = other.minorSemiaxis_;
    centralLongitude_ = other.centralLongitude_;
    centralLatitude_  = other.centralLatitude_;
    lowerLatitude_    = other.lowerLatitude_;
    upperLatitude_    = other.upperLatitude_;
    eccentricity_     = other.eccentricity_;
    lambda0_          = other.lambda0_;
    rho0_             = other.rho0_;
    n_                = other.n_;
    c_                = other.c_;
  }

  POST( *this == other );

  return *this;
}



/******************************************************************************
PURPOSE: newCopy - Allocate, copy and return this.
NOTES:   Caller must delete returned result when no longer needed.
******************************************************************************/

MapProjection* Lambert::newCopy() const {
  PRE( true );
  MapProjection* const result =
    new Lambert( majorSemiaxis_, minorSemiaxis_,
                 centralLongitude_, centralLatitude_,
                 lowerLatitude_,  upperLatitude_ );
  POST2( result, result->invariant() );
  return result;
}



/******************************************************************************
PURPOSE: invariant - Class invariant.
RETURNS: bool true if valid, else false and the code is defective.
******************************************************************************/

bool Lambert::invariant() const {

  const bool result =
    AND15( ! strcmp( Lambert::name_, "Lambert" ),
           isSpheroid( majorSemiaxis_, minorSemiaxis_ ),
           isLongitude( centralLongitude_ ),
           isLatitude( centralLatitude_ ),
           isLatitude( lowerLatitude_ ),
           isLatitude( upperLatitude_ ),
           lowerLatitude_ <= upperLatitude_,
           SIGN( lowerLatitude_ ) == SIGN( upperLatitude_ ),
           IMPLIES_ELSE( lowerLatitude_ >= 0.0,
                         IN_RANGE( lowerLatitude_, 1.0, 89.0 ),
                         IN_RANGE( lowerLatitude_, -89.0, -1.0 ) ),
           IMPLIES_ELSE( upperLatitude_ >= 0.0,
                         IN_RANGE( upperLatitude_, 1.0, 89.0 ),
                         IN_RANGE( upperLatitude_, -89.0, -1.0 ) ),
           IN_RANGE( eccentricity_, 0.0, 1.0 ),
           ! isNan( lambda0_ ),
           ! isNan( rho0_ ),
           ! isNan( n_ ),
           ! isNan( c_ ) );

  return result;
}



/******************************************************************************
PURPOSE: name - Name of projection.
RETURNS: const char* "Lambert".
******************************************************************************/

const char* Lambert::name() const {
  const char* const result = name_;
  POST0( ! strcmp( result, "Lambert" ) );
  return result;
}



/******************************************************************************
PURPOSE: operator== - Are objects equal?
INPUTS:  const Lambert& other  Object to compare to.
RETURNS: bool true if equal, else false.
******************************************************************************/

bool Lambert::operator==( const Lambert& other ) const {

  PRE( other.invariant() );

  const bool result =
    AND7( name_ == other.name_,
          aboutEqual( majorSemiaxis_,    other.majorSemiaxis_ ),
          aboutEqual( minorSemiaxis_,    other.minorSemiaxis_ ),
          aboutEqual( centralLongitude_, other.centralLongitude_ ),
          aboutEqual( centralLatitude_,  other.centralLatitude_ ),
          aboutEqual( lowerLatitude_,    other.lowerLatitude_ ),
          aboutEqual( upperLatitude_,    other.upperLatitude_ ) );

  return result;
}



/******************************************************************************
PURPOSE: operator!= - Are objects unequal?
INPUTS:  const Lambert& other  Object to compare to.
RETURNS: bool true if unequal, else false.
******************************************************************************/

bool Lambert::operator!=( const Lambert& other ) const {
  PRE( other.invariant() );
  const bool result = ! ( *this == other );
  return result;
}



/******************************************************************************
PURPOSE: centralLongitude - Longitude that projects to 0.
RETURNS: double longitude that projects to 0.
******************************************************************************/

double Lambert::centralLongitude() const {
  PRE( true );
  const double result = centralLongitude_;
  POST( isLongitude( result ) );
  return result;
}



/******************************************************************************
PURPOSE: centralLatitude - Latitude that projects to 0.
RETURNS: double latitude that projects to 0.
******************************************************************************/

double Lambert::centralLatitude() const {
  PRE( true );
  const double result = centralLatitude_;
  POST( isLatitude( result ) );
  return result;
}



/******************************************************************************
PURPOSE: project - Project a point from longitude-latitude space to x-y space.
INPUTS:  double longitude  Longitude of point to project.
         double latitude   Latitude  of point to project.
OUTPUTS: double& x         X coordinate of projected longitude.
         double& y         Y coordinate of projected latitude.
******************************************************************************/

void Lambert::project( double longitude, double latitude,
                       double& x, double& y ) const {

  PRE2( isLongitude( longitude ), isLatitude( latitude ) );

  double lambda = 0.0; // Longitude in radians.
  double phi    = 0.0; // Latitude in radians.
  toLambdaPhi( longitude, latitude, lambda, phi );
  const double lambdaDelta = lambdaDifference( lambda, lambda0_ );
  const double nLambdaDelta = n_ * lambdaDelta;
  const double rho = c_ * pow( tsfn( phi, sin( phi ), eccentricity_ ), n_ );
  x = rho * sin( nLambdaDelta ) * majorSemiaxis_;
  y = ( rho0_ - rho * cos( nLambdaDelta ) ) * majorSemiaxis_;

  POST2( ! isNan( x ), ! isNan( y ) );
}



/******************************************************************************
PURPOSE: unproject - Unproject a point from x-y space to longitude-latitude.
INPUTS:  double x           X-coordinate of point to unproject.
         double y           X-coordinate of point to unproject.
OUTPUTS: double* longitude  Longitude of unprojected point X.
         double* latitude   Latitude  of unprojected point Y.
******************************************************************************/

void Lambert::unproject( double x, double y,
                         double& longitude, double& latitude ) const {

  PRE2( isFinite( x ), isFinite( y ) );

  const double oneOverMajorSemiaxis = 1.0 / majorSemiaxis_;
  double xp = x * oneOverMajorSemiaxis;
  const double yp = y * oneOverMajorSemiaxis;
  double ypDelta = rho0_ - yp; // Dist from yp to central lat (in radians).
  double rho     = hypot( xp, ypDelta );
  double lambda  = 0.0;       // Radians of longitude.
  double phi     = PI_OVER_2; // Radians of latitude.

  if ( rho != 0.0 ) {

    if ( n_ < 0.0 ) {
      rho = -rho;
      xp = -xp;
      ypDelta = -ypDelta;
    }

    CHECK3( c_ != 0.0, n_ != 0.0, rho != 0.0 );

    if ( eccentricity_ == 0.0 ) { // Sphere:
      phi = 2.0 * atan( pow( c_ / rho, 1.0 / n_ ) ) - PI_OVER_2;
    } else { // Ellipsoid:
      phi = phi2Iterate( pow( rho / c_, 1.0 / n_ ), eccentricity_ );
    }

    lambda = atan2( xp, ypDelta ) / n_;
  } else if ( n_ < 0.0 ) {
    phi = -PI_OVER_2;
  }

  lambda += lambda0_;
  toLongitudeLatitude( lambda, phi, longitude, latitude );

  POST2( isLongitude( longitude ), isLatitude( latitude ) );
}



/******************************************************************************
PURPOSE: lowerLatitude - Lower latitude of secant plane.
RETURNS: double result lower latitude of secant plane.
******************************************************************************/

double Lambert::lowerLatitude() const {
  PRE( true );
  const double result = lowerLatitude_;
  POST( isLatitude( result ) );
  return result;
}




/******************************************************************************
PURPOSE: upperLatitude - Upper latitude of secant plane.
RETURNS: double result upper latitude of secant plane.
******************************************************************************/

double Lambert::upperLatitude() const {
  PRE( true );
  const double result = upperLatitude_;
  POST2( isLatitude( result ), result >= lowerLatitude() );
  return result;
}



//============================ PRIVATE FUNCTIONS ==============================



/******************************************************************************
PURPOSE: computeDerivedTerms - Compute trigonometry terms independent of
         longitude/latitude of projection point:
         eccentricity_  Of spheroid approximation of planet.
         lambda0_       Central longitude in radians.
         rho0_          See USGS PROJ Library.
         n_             See USGS PROJ Library.
         c_             See USGS PROJ Library.
******************************************************************************/

void Lambert::computeDerivedTerms() {
#if 0
  const double eccentricity0 = majorSemiaxis_ == minorSemiaxis_ ? 0.0 :
    safeQuotient( sqrt( safeDifference( SQUARE( majorSemiaxis_ ),
                                        SQUARE( minorSemiaxis_ ) ) ),
                  majorSemiaxis_ );
#else
  const double eccentricity0 = majorSemiaxis_ == minorSemiaxis_ ? 0.0 :
    sqrt( SQUARE( majorSemiaxis_ ) - SQUARE( minorSemiaxis_)) / majorSemiaxis_;
#endif
  const double eccentricity1 = eccentricity0 > 1.0 ? 1.0 : eccentricity0;
  const double eccentricitySquared = SQUARE( eccentricity1 );
  const double phi0 = radians( centralLatitude_ );
  const double phi1 = radians( lowerLatitude_ );
  const double phi2 = radians( upperLatitude_ );
  const double sinePhi1   = sin( phi1 );
  const double cosinePhi1 = cos( phi1 );
  const double sinePhi2   = sin( phi2 );
  const double cosinePhi2 = cos( phi2 );
  // Are lower/upperLatitude about equal?
  const bool isTangent = phi1 + MAP_PROJECTION_TOLERANCE >= phi2;

  eccentricity_ = eccentricity1;
  lambda0_ = radians( centralLongitude_ );
  rho0_ = 0.0;
  n_ = sinePhi1;
  c_ = 0.0;

  if ( eccentricitySquared != 0.0 ) { // Non-sphere planet:
    const double m1  = msfn( sinePhi1, cosinePhi1, eccentricitySquared );
    const double ml1 = tsfn( phi1, sinePhi1, eccentricity_ );

    if ( ! isTangent ) { // Secant form:
      const double numerator =
        log( m1 / msfn( sinePhi2, cosinePhi2, eccentricitySquared ) );
      const double denominator =
        log( ml1 / tsfn( phi2, sinePhi2, eccentricity_ ) );
      CHECK( denominator != 0.0 );
      n_ = numerator / denominator;
    }

    CHECK( n_ != 0.0 );
    c_ = m1 * pow( ml1, -n_ ) / n_;

    if ( fabs( fabs( phi0 ) - PI_OVER_2 ) < MAP_PROJECTION_TOLERANCE ) {
      rho0_ = 0.0; // Prevented by constructor precondition.
    } else {
      rho0_ = c_ * pow( tsfn( phi0, sin( phi0 ), eccentricity_ ), n_ );
    }
  } else { // Sphere planet:
    const double denominator = tan( PI_OVER_4 + 0.5 * phi1 );

    if ( ! isTangent ) { // Secant form:
      CHECK( ! aboutEqual( fabs( phi1 ), PI_OVER_2 ) ); // Not near pole.
      CHECK( ! aboutEqual( fabs( phi2 ), PI_OVER_2 ) ); // Not near pole.
      CHECK( cosinePhi1 != 0.0 );
      CHECK( cosinePhi2 != 0.0 );
      CHECK( tan( PI_OVER_4 + 0.5 * phi2 ) != 0.0 );
      CHECK( denominator != 0.0 );
      n_ = log( cosinePhi1 / cosinePhi2 ) /
                log( tan( PI_OVER_4 + 0.5 * phi2 ) / denominator );
    }

    c_ = cosinePhi1 * pow( denominator, n_ ) / n_;

    if ( fabs( fabs( phi0 ) - PI_OVER_2 ) < MAP_PROJECTION_TOLERANCE ) {
      rho0_ = 0.0; // Prevented by constructor precondition.
    } else {
      rho0_ = c_ * pow( tan( PI_OVER_4 + 0.5 * phi0 ), -n_ );
    }
  }

  POST6( ! isNan( eccentricity_ ),
         IN_RANGE( eccentricity_, 0.0, 1.0 ),
         ! isNan( lambda0_ ),
         ! isNan( rho0_ ),
         ! isNan( n_ ),
         ! isNan( c_ ) );
}



} // namespace emvl.



