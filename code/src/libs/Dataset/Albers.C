/******************************************************************************
PURPOSE: Albers.C - Define Albers Conformal Conic MapProjection ADT.
NOTES:   Derived from USGS PROJ Library.
HISTORY: 2013-02-05 plessel.todd@epa.gov
******************************************************************************/

//================================ INCLUDES ===================================

#include <string.h> // For strcmp().
#include <math.h>   // For M_PI, sqrt(), sin(), cos(), atan2(), log(), pow().
#include <float.h>  // For DBL_MAX.

#include <Assertions.h>    // For PRE*(), POST*(), CHECK*(), IN_RANGE().
#include <DataUtilities.h> // For isNan(), aboutEqual(), isLongitude().
#include <Albers.h>        // For public interface.

//=========================== FORWARD DECLARATIONS ============================

namespace emvl {

const char* const Albers::name_ = "Albers"; // Initialize static attribute.

static double phi1Iterate( double phi, double eccentricity,
                           double oneMinusEccentricitySquared );

//============================= PUBLIC FUNCTIONS ==============================



/******************************************************************************
PURPOSE: ~Albers - Destruct a Albers.
******************************************************************************/

Albers::~Albers() {
  PRE( true );
  centralLongitude_            = 0.0;
  centralLatitude_             = 0.0;
  lowerLatitude_               = 0.0;
  upperLatitude_               = 0.0;
  eccentricity_                = 0.0;
  oneMinusEccentricitySquared_ = 0.0;
  lambda0_                     = 0.0;
  rho0_                        = 0.0;
  n_                           = 0.0;
  n2_                          = 0.0;
  c_                           = 0.0;
  ec_                          = 0.0;
  dd_                          = 0.0;
}

  
  
  
/******************************************************************************
PURPOSE: Albers - Construct a Albers.
INPUTS:  double majorSemiaxis    Mean equitorial radius of spheroid in meters.
         double minorSemiaxis    Mean polar      radius of spheroid in meters.
         double centralLongitude Longitude that projects to 0.
         double centralLatitude  Latitude that projects to 0.
         double lowerLatitude    Latitude of lower secant.
         double upperLatitude    latitude of upper secant.
******************************************************************************/

Albers::Albers( double majorSemiaxis, double minorSemiaxis,
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

  POST7( ! strcmp( this->name(), "Albers" ),
         this->majorSemiaxis() == majorSemiaxis,
         this->minorSemiaxis() == minorSemiaxis,
         this->centralLongitude() == centralLongitude,
         this->centralLatitude() == centralLatitude,
         this->lowerLatitude() == lowerLatitude,
         this->upperLatitude() == upperLatitude );
}



/******************************************************************************
PURPOSE: Albers - Copy construct a Albers.
INPUTS:  const Albers& other  Other object to copy.
******************************************************************************/

Albers::Albers( const Albers& other ) {

  PRE0( other.invariant() );

  majorSemiaxis_               = other.majorSemiaxis_;
  minorSemiaxis_               = other.minorSemiaxis_;
  centralLongitude_            = other.centralLongitude_;
  centralLatitude_             = other.centralLatitude_;
  lowerLatitude_               = other.lowerLatitude_;
  upperLatitude_               = other.upperLatitude_;
  eccentricity_                = other.eccentricity_;
  oneMinusEccentricitySquared_ = other.oneMinusEccentricitySquared_;
  lambda0_                     = other.lambda0_;
  rho0_                        = other.rho0_;
  n_                           = other.n_;
  n2_                          = other.n2_;
  c_                           = other.c_;
  ec_                          = other.ec_;
  dd_                          = other.dd_;

  POST( *this == other );
}



/******************************************************************************
PURPOSE: operator= - Assign a Albers.
INPUTS:  const Albers& other  Other object to copy.
RETURNS: Albers& *this.
******************************************************************************/

Albers& Albers::operator=( const Albers& other ) {

  PRE0( other.invariant() );

  if ( this != &other ) {
    majorSemiaxis_               = other.majorSemiaxis_;
    minorSemiaxis_               = other.minorSemiaxis_;
    centralLongitude_            = other.centralLongitude_;
    centralLatitude_             = other.centralLatitude_;
    lowerLatitude_               = other.lowerLatitude_;
    upperLatitude_               = other.upperLatitude_;
    eccentricity_                = other.eccentricity_;
    oneMinusEccentricitySquared_ = other.oneMinusEccentricitySquared_;
    lambda0_                     = other.lambda0_;
    rho0_                        = other.rho0_;
    n_                           = other.n_;
    n2_                          = other.n2_;
    c_                           = other.c_;
    ec_                          = other.ec_;
    dd_                          = other.dd_;
  }

  POST( *this == other );

  return *this;
}



/******************************************************************************
PURPOSE: newCopy - Allocate, copy and return this.
NOTES:   Caller must delete returned result when no longer needed.
******************************************************************************/

MapProjection* Albers::newCopy() const {
  PRE( true );
  MapProjection* const result =
    new Albers( majorSemiaxis_, minorSemiaxis_,
                centralLongitude_, centralLatitude_,
                lowerLatitude_,  upperLatitude_ );
  POST2( result, result->invariant() );
  return result;
}



/******************************************************************************
PURPOSE: invariant - Class invariant.
RETURNS: bool true if valid, else false and the code is defective.
******************************************************************************/

bool Albers::invariant() const {

  const bool result =
  AND19( ! strcmp( Albers::name_, "Albers" ),
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
         aboutEqual( oneMinusEccentricitySquared_,
                     1.0 - SQUARE( eccentricity_ ) ),
         ! isNan( lambda0_ ),
         ! isNan( rho0_ ),
         ! isNan( n_ ),
         ! isNan( n2_ ),
         ! isNan( c_ ),
         ! isNan( ec_ ),
         ! isNan( dd_ ) );

  POST0( IS_BOOL( result ) );
  return result;
}



/******************************************************************************
PURPOSE: name - Name of projection.
RETURNS: const char* "Albers".
******************************************************************************/

const char* Albers::name() const {
  const char* const result = name_;
  POST0( ! strcmp( result, "Albers" ) );
  return result;
}



/******************************************************************************
PURPOSE: operator== - Are objects equal?
INPUTS:  const Albers& other  Object to compare to.
RETURNS: bool true if equal, else false.
******************************************************************************/

bool Albers::operator==( const Albers& other ) const {

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
INPUTS:  const Albers& other  Object to compare to.
RETURNS: bool true if unequal, else false.
******************************************************************************/

bool Albers::operator!=( const Albers& other ) const {
  PRE( other.invariant() );
  const bool result = ! ( *this == other );
  return result;
}



/******************************************************************************
PURPOSE: centralLongitude - Longitude that projects to 0.
RETURNS: double longitude that projects to 0.
******************************************************************************/

double Albers::centralLongitude() const {
  PRE( true );
  const double result = centralLongitude_;
  POST( isLongitude( result ) );
  return result;
}



/******************************************************************************
PURPOSE: centralLatitude - Latitude that projects to 0.
RETURNS: double latitude that projects to 0.
******************************************************************************/

double Albers::centralLatitude() const {
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

void Albers::project( double longitude, double latitude,
                      double& x, double& y ) const {

  PRE2( isLongitude( longitude ), isLatitude( latitude ) );

  double lambda = 0.0; // Longitude in radians.
  double phi    = 0.0; // Latitude in radians.
  toLambdaPhi( longitude, latitude, lambda, phi );
  const double lambdaDelta = lambdaDifference( lambda, lambda0_ );
  const double rho1 =
    c_ - n_ * qsfn( sin( phi ), eccentricity_, oneMinusEccentricitySquared_ );
  CHECK( rho1 >= 0.0 );
  const double rho = sqrt( rho1 ) * dd_;
  const double nLambdaDelta = n_ * lambdaDelta;
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

void Albers::unproject( double x, double y,
                        double& longitude, double& latitude ) const {

  PRE2( IN_RANGE( x, -4.0 * majorSemiaxis(),
                      4.0 * majorSemiaxis() ),
        IN_RANGE( y, -4.0 * M_PI * majorSemiaxis(),
                      4.0 * M_PI * majorSemiaxis() ) );

  if ( AND2( IN_RANGE( x, -1.0 * MAP_PROJECTION_TOLERANCE,
                          MAP_PROJECTION_TOLERANCE ),
             IN_RANGE( y, -1.0 * MAP_PROJECTION_TOLERANCE,
                          MAP_PROJECTION_TOLERANCE ) ) ) {
    longitude = centralLongitude_;
    latitude  = centralLatitude_;
  } else {
    const double oneOverMajorSemiaxis = 1.0 / majorSemiaxis_;
    double xp       = x * oneOverMajorSemiaxis;
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

      CHECK4( c_ != 0.0, n_ != 0.0, rho != 0.0, dd_ != 0.0 );
      phi = rho / dd_;

      if ( eccentricity_ != 0.0 ) { // Non-sphere:
        phi = ( c_ - SQUARE( phi ) ) / n_;

        if ( AND2( IN_RANGE( phi, -2.0, 2.0 ),
                   fabs( ec_ - fabs( phi ) ) > MAP_PROJECTION_TOLERANCE ) ) {
          phi = phi1Iterate( phi, eccentricity_, oneMinusEccentricitySquared_);
        } else {
          phi = phi < 0.0 ? -PI_OVER_2 : PI_OVER_2;
        }
      } else { // Sphere:
        phi = ( c_ - SQUARE( phi ) ) / n2_;

        if ( fabs( phi ) < 1.0 ) {
          phi = asin( phi );
        } else {
          phi = phi < 0.0 ? -PI_OVER_2 : PI_OVER_2;
        }
      }

      lambda = atan2( xp, ypDelta ) / n_;
    } else {
      phi = n_ > 0.0 ? PI_OVER_2 : -PI_OVER_2;
    }

    lambda += lambda0_;
    toLongitudeLatitude( lambda, phi, longitude, latitude );
  }

  POST2( isLongitude( longitude ), isLatitude( latitude ) );
}



/******************************************************************************
PURPOSE: lowerLatitude - Lower latitude of secant plane.
RETURNS: double result lower latitude of secant plane.
******************************************************************************/

double Albers::lowerLatitude() const {
  PRE( true );
  const double result = lowerLatitude_;
  POST( isLatitude( result ) );
  return result;
}




/******************************************************************************
PURPOSE: upperLatitude - Upper latitude of secant plane.
RETURNS: double result upper latitude of secant plane.
******************************************************************************/

double Albers::upperLatitude() const {
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
         oneMinusEccentricitySquared_  Of spheroid approximation of planet.
         lambda0_       Central longitude in radians.
         rho0_          See USGS PROJ Library.
         n_             See USGS PROJ Library.
         n2_            See USGS PROJ Library.
         c_             See USGS PROJ Library.
         ec_            See USGS PROJ Library.
         dd_            See USGS PROJ Library.
******************************************************************************/

void Albers::computeDerivedTerms() {
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
  const double sinePhi0   = sin( phi0 );
  const double sinePhi1   = sin( phi1 );
  const double cosinePhi1 = cos( phi1 );
  const double sinePhi2   = sin( phi2 );
  const double cosinePhi2 = cos( phi2 );
  // Are lower/upper_latitude about equal?
  const bool isTangent = phi1 + MAP_PROJECTION_TOLERANCE >= phi2;

  CHECK( sinePhi1 != 0.0 );

  eccentricity_ = eccentricity1;
  oneMinusEccentricitySquared_ = 1.0 - eccentricitySquared;
  lambda0_ = radians( centralLongitude_ );
  rho0_    = radians( centralLatitude_ );
  n_ = sinePhi1;
  n2_ = 0.0;
  c_  = 0.0;
  ec_ = 0.0;
  dd_ = 1.0 / n_;

  if ( eccentricitySquared != 0.0 ) { // Non-sphere planet:
    const double m1  = msfn( sinePhi1, cosinePhi1, eccentricitySquared );
    const double ml1 =
      qsfn( sinePhi1, eccentricity_, oneMinusEccentricitySquared_ );

    if ( ! isTangent ) { // Secant form:
      const double m2  = msfn( sinePhi2, cosinePhi2, eccentricitySquared );
      const double ml2 =
        qsfn( sinePhi2, eccentricity_, oneMinusEccentricitySquared_ );
      CHECK( ml1 != ml2 );
      n_ = ( SQUARE( m1 ) - SQUARE( m2 ) ) / ( ml2 - ml1 );
    }

    CHECK2( n_ != 0.0, eccentricity_ != 0.0 );
    ec_ =
      1.0 - 0.5 * oneMinusEccentricitySquared_ *
      log( ( 1.0 - eccentricity_ ) / ( 1.0 + eccentricity_ ) ) / eccentricity_;
    c_ = SQUARE( m1 ) + n_ * ml1;
    dd_ = 1.0 / n_;
    rho0_ =
      dd_ * sqrt( c_ - n_ *
                  qsfn(sinePhi0, eccentricity_, oneMinusEccentricitySquared_));

  } else { // Sphere planet:

    if ( ! isTangent ) { // Secant form:
      n_ = 0.5 * ( n_ + sinePhi2 );
    }

    CHECK( ! aboutEqual( fabs( phi1 ), PI_OVER_2 ) ); // Not near pole.
    CHECK( ! aboutEqual( fabs( phi2 ), PI_OVER_2 ) ); // Not near pole.
    CHECK( cosinePhi1 != 0.0 );
    CHECK( cosinePhi2 != 0.0 );

    n2_ = n_ + n_;
    c_ = SQUARE( cosinePhi1 ) + n2_ * sinePhi1;
    CHECK2( n_ != 0.0, c_ > n2_ * sinePhi0 );
    dd_ = 1.0 / n_;
    rho0_ = dd_ * sqrt( c_ - n2_ * sinePhi0 );
  }

  POST10( ! isNan( eccentricity_ ),
          IN_RANGE( eccentricity_, 0.0, 1.0 ),
          aboutEqual( oneMinusEccentricitySquared_,
                      1.0 - SQUARE( eccentricity_ ) ),
         ! isNan( lambda0_ ),
         ! isNan( rho0_ ),
         ! isNan( n_ ),
         ! isNan( n2_ ),
         ! isNan( c_ ),
         ! isNan( ec_ ),
         ! isNan( dd_ ) );
}



/******************************************************************************
PURPOSE: phi1Iterate - Iterate on unprojected y coordinate.
INPUTS:  double ts            See USGS PROJ Library.
         double eccentricity  Eccentrity of spheroid.
RETURNS: double converged phi.
******************************************************************************/

static double phi1Iterate( double phi, double eccentricity,
                           double oneMinusEccentricitySquared ) {

  PRE03( IN_RANGE( phi, -2.0, 2.0 ),
         IN_RANGE( eccentricity, 0.0, 1.0 ),
         IN_RANGE( oneMinusEccentricitySquared, 0.0, 1.0 ) );
  double result = asin( 0.5 * phi );

  if ( eccentricity > MapProjection::MAP_PROJECTION_TOLERANCE ) {
    double deltaPhi = 0.0;
    int iteration = 0;

    do {
      const double sinePhi = sin( result );
      const double cosinePhi = cos( result );
      const double con = eccentricity * sinePhi;
      const double com = 1.0 - SQUARE( con );
      CHECK4( cosinePhi != 0.0, con != -1.0, com != 0.0,
              oneMinusEccentricitySquared != 0.0 );
      deltaPhi =
        0.5 * SQUARE( com ) / cosinePhi *
        ( phi / oneMinusEccentricitySquared -
          sinePhi / com +
          0.5 / eccentricity * log( ( 1.0 - con ) / ( 1.0 + con ) ) );
      result += deltaPhi;
      ++iteration;
    } while ( AND2( fabs( deltaPhi ) >=
                      MapProjection::MAP_PROJECTION_CONVERGENCE_TOLERANCE,
                    iteration <
                MapProjection::MAP_PROJECTION_CONVERGENCE_MAXIMUM_ITERATIONS));
  }

  POST0( ! isNan( result ) );
  return result;
}



} // namespace emvl.



