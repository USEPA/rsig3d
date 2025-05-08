/******************************************************************************
PURPOSE: Stereographic.C - Define Stereographic MapProjection ADT.
NOTES:   Derived from USGS PROJ Library.
HISTORY: 2013-02-03 plessel.todd@epa.gov
******************************************************************************/

//================================ INCLUDES ===================================

#include <string.h> // For strcmp().
#include <math.h>   // For M_PI, sqrt(), sin(), cos(), atan2(), log(), pow().
#include <float.h>  // For DBL_MAX.

#include <Assertions.h>    // For PRE*(), POST*(), CHECK*(), IN_RANGE().
#include <DataUtilities.h> // For isNan(), aboutEqual(), isLongitude().
#include <Stereographic.h> // For public interface.

//=========================== FORWARD DECLARATIONS ============================

namespace emvl {

const char* const Stereographic::name_ = "Stereographic"; // Init static attr.

enum { NORTH_POLE, SOUTH_POLE, EQUITORIAL, OBLIQUE }; // Projection subtype.
#define IS_VALID_SUBTYPE( subtype ) IN_RANGE( subtype, NORTH_POLE, OBLIQUE )


//============================= PUBLIC FUNCTIONS ==============================



/******************************************************************************
PURPOSE: ~Stereographic - Destruct a Stereographic.
******************************************************************************/

Stereographic::~Stereographic() {
  PRE( true );
  centralLongitude_ = 0.0;
  centralLatitude_  = 0.0;
  secantLatitude_   = 0.0;
  eccentricity_     = 0.0;
  lambda0_          = 0.0;
  phi0_             = 0.0;
  sineX1_           = 0.0;
  cosineX1_         = 0.0;
  akm1_             = 0.0;
  projectedCenterX_ = 0.0;
  projectedCenterY_ = 0.0;
  subtype_          = 0;
}

  
  
  
/******************************************************************************
PURPOSE: Stereographic - Construct a Stereographic.
INPUTS:  double majorSemiaxis    Mean equitorial radius of spheroid in meters.
         double minorSemiaxis    Mean polar      radius of spheroid in meters.
         double centralLongitude Longitude that projects to 0.
         double centralLatitude  Latitude that projects to 0.
         double secantLatitude   Latitude where plane intersects the spheroid.
******************************************************************************/

Stereographic::Stereographic( double majorSemiaxis, double minorSemiaxis,
                              double centralLongitude, double centralLatitude,
                              double secantLatitude ) {

  PRE04( isSpheroid( majorSemiaxis, minorSemiaxis ),
         isLongitude( centralLongitude ),
         isLatitude( centralLatitude ),
         isLatitude( secantLatitude ) );

  majorSemiaxis_    = majorSemiaxis;
  minorSemiaxis_    = minorSemiaxis;
  centralLongitude_ = centralLongitude;
  centralLatitude_  = centralLatitude;
  secantLatitude_   = secantLatitude;
  eccentricity_     = 0.0;
  lambda0_          = 0.0;
  phi0_             = 0.0;
  sineX1_           = 0.0;
  cosineX1_         = 0.0;
  akm1_             = 0.0;
  projectedCenterX_ = 0.0;
  projectedCenterY_ = 0.0;
  subtype_          = 0;
  computeDerivedTerms();

  POST6( ! strcmp( this->name(), "Stereographic" ),
         this->majorSemiaxis() == majorSemiaxis,
         this->minorSemiaxis() == minorSemiaxis,
         this->centralLongitude() == centralLongitude,
         this->centralLatitude() == centralLatitude,
         this->secantLatitude() == secantLatitude );
}



/******************************************************************************
PURPOSE: Stereographic - Copy construct a Stereographic.
INPUTS:  const Stereographic& other  Other object to copy.
******************************************************************************/

Stereographic::Stereographic( const Stereographic& other ) {

  PRE0( other.invariant() );

  majorSemiaxis_    = other.majorSemiaxis_;
  minorSemiaxis_    = other.minorSemiaxis_;
  centralLongitude_ = other.centralLongitude_;
  centralLatitude_  = other.centralLatitude_;
  secantLatitude_   = other.secantLatitude_;
  eccentricity_     = other.eccentricity_;
  lambda0_          = other.lambda0_;
  phi0_             = other.phi0_;
  sineX1_           = other.sineX1_;
  cosineX1_         = other.cosineX1_;
  akm1_             = other.akm1_;
  projectedCenterX_ = other.projectedCenterX_;
  projectedCenterY_ = other.projectedCenterY_;
  subtype_          = other.subtype_;

  POST( *this == other );
}



/******************************************************************************
PURPOSE: operator= - Assign a Stereographic.
INPUTS:  const Stereographic& other  Other object to copy.
RETURNS: Stereographic& *this.
******************************************************************************/

Stereographic& Stereographic::operator=( const Stereographic& other ) {

  PRE0( other.invariant() );

  if ( this != &other ) {
    majorSemiaxis_    = other.majorSemiaxis_;
    minorSemiaxis_    = other.minorSemiaxis_;
    centralLongitude_ = other.centralLongitude_;
    centralLatitude_  = other.centralLatitude_;
    secantLatitude_   = other.secantLatitude_;
    eccentricity_     = other.eccentricity_;
    lambda0_          = other.lambda0_;
    phi0_             = other.phi0_;
    sineX1_           = other.sineX1_;
    cosineX1_         = other.cosineX1_;
    akm1_             = other.akm1_;
    projectedCenterX_ = other.projectedCenterX_;
    projectedCenterY_ = other.projectedCenterY_;
    subtype_          = other.subtype_;
  }

  POST( *this == other );

  return *this;
}



/******************************************************************************
PURPOSE: newCopy - Allocate, copy and return this.
NOTES:   Caller must delete returned result when no longer needed.
******************************************************************************/

MapProjection* Stereographic::newCopy() const {
  PRE( true );
  MapProjection* const result =
    new Stereographic( majorSemiaxis_, minorSemiaxis_,
                       centralLongitude_, centralLatitude_, secantLatitude_ );
  POST2( result, result->invariant() );
  return result;
}



/******************************************************************************
PURPOSE: invariant - Class invariant.
RETURNS: bool true if valid, else false and the code is defective.
******************************************************************************/

bool Stereographic::invariant() const {

  const bool result =
    AND14( ! strcmp( Stereographic::name_, "Stereographic" ),
           isSpheroid( majorSemiaxis_, minorSemiaxis_ ),
           isLongitude( centralLongitude_ ),
           isLatitude( centralLatitude_ ),
           isLatitude( secantLatitude_ ),
           IN_RANGE( eccentricity_, 0.0, 1.0 ),
           isLongitude( lambda0_ ),
           ! isNan( phi0_ ),
           ! isNan( sineX1_ ),
           ! isNan( cosineX1_ ),
           ! isNan( akm1_ ),
           ! isNan( projectedCenterX_ ),
           ! isNan( projectedCenterY_ ),
           IS_VALID_SUBTYPE( subtype_ ) );

  return result;
}



/******************************************************************************
PURPOSE: name - Name of projection.
RETURNS: const char* "Stereographic".
******************************************************************************/

const char* Stereographic::name() const {
  const char* const result = name_;
  POST0( ! strcmp( result, "Stereographic" ) );
  return result;
}



/******************************************************************************
PURPOSE: operator== - Are objects equal?
INPUTS:  const Stereographic& other  Object to compare to.
RETURNS: bool true if equal, else false.
******************************************************************************/

bool Stereographic::operator==( const Stereographic& other ) const {

  PRE( other.invariant() );

  const bool result =
    AND6( name_ == other.name_,
          aboutEqual( majorSemiaxis_,    other.majorSemiaxis_ ),
          aboutEqual( minorSemiaxis_,    other.minorSemiaxis_ ),
          aboutEqual( centralLongitude_, other.centralLongitude_ ),
          aboutEqual( centralLatitude_,  other.centralLatitude_ ),
          aboutEqual( secantLatitude_,   other.secantLatitude_ ) );

  return result;
}


  
/******************************************************************************
PURPOSE: operator!= - Are objects unequal?
INPUTS:  const Stereographic& other  Object to compare to.
RETURNS: bool true if unequal, else false.
******************************************************************************/

bool Stereographic::operator!=( const Stereographic& other ) const {
  PRE( other.invariant() );
  const bool result = ! ( *this == other );
  return result;
}



/******************************************************************************
PURPOSE: centralLongitude - Longitude that projects to 0.
RETURNS: double longitude that projects to 0.
******************************************************************************/

double Stereographic::centralLongitude() const {
  PRE( true );
  const double result = centralLongitude_;
  POST( isLongitude( result ) );
  return result;
}



/******************************************************************************
PURPOSE: centralLatitude - Latitude that projects to 0.
RETURNS: double latitude that projects to 0.
******************************************************************************/

double Stereographic::centralLatitude() const {
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

void Stereographic::project( double longitude, double latitude,
                             double& x, double& y ) const {

  PRE2( isLongitude( longitude ), isLatitude( latitude ) );

  double lambda = 0.0; // Longitude in radians.
  double phi    = 0.0; // Latitude  in radians.
  toLambdaPhi( longitude, latitude, lambda, phi );
  const double lambdaDelta = lambdaDifference( lambda, lambda0_ );
  const double sineLambda   = sin( lambdaDelta );
  const double cosineLambda = cos( lambdaDelta );
  const double sinePhi      = sin( phi );
  CHECK( IS_VALID_SUBTYPE( subtype_ ) );
  x = y = 0.0;

  if ( eccentricity_ ) { // Non-sphere planet:
    projectSpheroid( phi, sineLambda, cosineLambda, sinePhi, x, y );
  } else { // Sphere:
    projectSphere( phi, sineLambda, cosineLambda, sinePhi, x, y );
  }

  x *= majorSemiaxis_;
  y *= majorSemiaxis_;

  POST2( ! isNan( x ), ! isNan( y ) );
}



/******************************************************************************
PURPOSE: unproject - Unproject a point from x-y space to longitude-latitude.
INPUTS:  double x           X-coordinate of point to unproject.
         double y           X-coordinate of point to unproject.
OUTPUTS: double* longitude  Longitude of unprojected point X.
         double* latitude   Latitude  of unprojected point Y.
******************************************************************************/

void Stereographic::unproject( double x, double y,
                               double& longitude, double& latitude ) const {

  PRE2( isFinite( x ), isFinite( y ) );

  const double oneOverMajorSemiaxis = 1.0 / majorSemiaxis_;
  double xp = ( x + projectedCenterX_ ) * oneOverMajorSemiaxis;
  double yp = ( y + projectedCenterY_ ) * oneOverMajorSemiaxis;
  const double rho = hypot( xp, yp );
  double lambda = 0.0; // Radians of longitude.
  double phi    = 0.0; // Radians of latitude.

  if ( eccentricity_ ) { // Non-sphere planet:
    unprojectSpheroid( xp, yp, rho, lambda, phi );
  } else { // Sphere:
    unprojectSphere( xp, yp, rho, lambda, phi );
  }

  lambda += lambda0_;
  toLongitudeLatitude( lambda, phi, longitude, latitude );

  POST2( isLongitude( longitude ), isLatitude( latitude ) );
}



/******************************************************************************
PURPOSE: secantLatitude - Lower latitude of secant plane.
RETURNS: double result lower latitude of secant plane.
******************************************************************************/

double Stereographic::secantLatitude() const {
  PRE( true );
  const double result = secantLatitude_;
  POST( isLatitude( result ) );
  return result;
}



//============================ PRIVATE FUNCTIONS ==============================



/******************************************************************************
PURPOSE: computeDerivedTerms - Compute trigonometry terms independent of
         longitude/latitude of projection point:
           eccentricity_
           lambda0_
           phi0_
           sineX1_
           cosineX1_
           akm1_
           projectedCenterX_
           projectedCenterY_
           subtype_
******************************************************************************/

void Stereographic::computeDerivedTerms() {
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
  const double phits = fabs( radians( secantLatitude_ ) );
  const double k0 = ( 1.0 + sin( phits ) ) * 0.5;

  eccentricity_     = 0.0;
  lambda0_          = 0.0;
  phi0_             = 0.0;
  sineX1_           = 0.0;
  cosineX1_         = 0.0;
  akm1_             = 0.0;
  projectedCenterX_ = 0.0;
  projectedCenterY_ = 0.0;
  subtype_          = 0;
  eccentricity_ = eccentricity1;
  lambda0_ = radians( centralLongitude_ );
  phi0_    = radians( centralLatitude_  );

  const double t0 = fabs( phi0_ );

  if ( fabs( t0 - PI_OVER_2 ) < MAP_PROJECTION_TOLERANCE ) {
    subtype_ = phi0_ < 0.0 ? SOUTH_POLE : NORTH_POLE;
  } else {
    subtype_ = t0 > MAP_PROJECTION_TOLERANCE ? OBLIQUE : EQUITORIAL;
  }

  DEBUG( fprintf( stderr, "subtype_ = %d, eccentricity_ = %lf, "
                          "lambda0_ = %lf, phi0_ = %lf, "
                          "phits = %lf, k0 = %lf\n",
                  subtype_, eccentricity_, lambda0_, phi0_, phits, k0 ); )

  if ( eccentricity_ != 0.0 ) { // Non-sphere planet:

    switch ( subtype_ ) {
    case EQUITORIAL:
      akm1_ = k0 + k0;
      break;
    case OBLIQUE:
      {
        double t = sin( phi0_ );
        double X = atan( ssfn( phi0_, t, eccentricity_ ) );
        X += X;
        X -= PI_OVER_2;
        t *= eccentricity_;
        CHECK( SQUARE( t ) < 1.0 );
        akm1_ = ( k0 + k0 ) * cos( phi0_ ) / sqrt( 1.0 - SQUARE( t ) );
        sineX1_ = sin( X );
        cosineX1_ = cos( X );
      }
      break;
    default:
      CHECK( IN3( subtype_, NORTH_POLE, SOUTH_POLE ) );

      if ( fabs( phits - PI_OVER_2 ) < MAP_PROJECTION_TOLERANCE ) {
        const double X  = 1.0 + eccentricity_;
        const double X2 = 1.0 - eccentricity_;
        CHECK( sqrt( pow( X, X ) * pow( X2, X2 ) ) > 0.0 );
        akm1_ = ( k0 + k0 ) / sqrt( pow( X, X ) * pow( X2, X2 ) );
      } else {
        const double sinePhits = sin( phits );
        akm1_ = cos( phits ) / tsfn( phits, sinePhits, eccentricity_ );
        const double t = sinePhits * eccentricity_;
        CHECK2( SQUARE( t ) < 1.0, sqrt( 1.0 - SQUARE( t ) ) > 0.0 );
        akm1_ /= sqrt( 1.0 - SQUARE( t ) );
      }

      break;
    }

  } else { // Sphere planet:

    switch ( subtype_ ) {
    case EQUITORIAL:
      akm1_ = k0 + k0;
      break;
    case OBLIQUE:
      sineX1_   = sin( phi0_ );
      cosineX1_ = cos( phi0_ );
      akm1_ = k0 + k0;
      break;
    default:
      CHECK( IN3( subtype_, NORTH_POLE, SOUTH_POLE ) );
      akm1_ = fabs( phits - PI_OVER_2 ) >= MAP_PROJECTION_TOLERANCE ?
                cos( phits ) / tan( PI_OVER_4 - 0.5 * phits )
                : k0 + k0 ;
      break;
    }
  }

  DEBUG( fprintf( stderr,
                  "project: centralLongitude_ = %lf, centralLatitude_ = %lf\n",
                  centralLongitude_, centralLatitude_ ); )

  project( centralLongitude_, centralLatitude_,
           projectedCenterX_, projectedCenterY_ );

  DEBUG( fprintf( stderr,
                  "initialized:\n"
                  "  eccentricity_ = %lf\n"
                  "  lambda0_ = %lf\n"
                  "  phi0_ = %lf\n"
                  "  sineX1_ = %lf\n"
                  "  cosineX1_ = %lf\n"
                  "  akm1_ = %lf\n"
                  "  projectedCenterX_ = %lf\n"
                  "  projectedCenterY_ = %lf\n"
                  "  subtype_ = %d\n",
                  eccentricity_, lambda0_, phi0_, sineX1_, cosineX1_, akm1_,
                  projectedCenterX_, projectedCenterY_, subtype_ ); )

  POST010( ! isNan( eccentricity_ ),
           IN_RANGE( eccentricity_, 0.0, 1.0 ),
           ! isNan( lambda0_ ),
           ! isNan( phi0_ ),
           ! isNan( sineX1_ ),
           ! isNan( cosineX1_ ),
           IN_RANGE( sineX1_, -1.0, 1.0 ),
           IN_RANGE( cosineX1_, -1.0, 1.0 ),
           ! isNan(  akm1_ ),
           IS_VALID_SUBTYPE( subtype_ ) );
}



/******************************************************************************
PURPOSE: projectSpheroid - Project a point on a non-sphere planet.
INPUTS:  double phi           Angle in radians of adjusted latitude to project.
         double sineLambda    Sine of lambda.
         double cosineLambda  Cosine of lambda.
         double sinePhi       Sine of phi.
OUTPUTS: double& x            Projected longitude.
         double& y            Projected latitude.
******************************************************************************/

void Stereographic::projectSpheroid( double phi,
                                     double sineLambda, double cosineLambda,
                                     double sinePhi,
                                     double& x, double& y ) const {

  PRE06( eccentricity_,
         ! isNan( phi ),
         ! isNan( sineLambda ),
         ! isNan( cosineLambda ),
         ! isNan( sinePhi ),
         aboutEqual( sinePhi, sin( phi ) ) );

  x = y = 0.0;

  switch ( subtype_ ) {
  case OBLIQUE:

    {
      const double X =
        2.0 * atan( ssfn( phi, sinePhi, eccentricity_ ) ) - PI_OVER_2;
      const double sineX   = sin( X );
      const double cosineX = cos( X );
      const double A =  akm1_ /
        ( cosineX1_ * ( 1.0 + sineX1_ * sineX +
                              cosineX1_ * cosineX * cosineLambda ) );
      x = A * cosineX;
      y = A * ( cosineX1_ * sineX - sineX1_ * cosineX * cosineLambda );
    }

    break;
  case EQUITORIAL:

    {
      const double X =
        2.0 * atan( ssfn( phi, sinePhi, eccentricity_ ) ) - PI_OVER_2;
      const double sineX   = sin( X );
      const double cosineX = cos( X );
      const double A = 2.0 *  akm1_ / ( 1.0 + cosineX * cosineLambda );
      x = A * cosineX;
      y = A * sineX;
    }

    break;
  case SOUTH_POLE:
    x = akm1_ * tsfn( -phi, -sinePhi, eccentricity_ );
    y = x * cosineLambda;
    break;
  default:
    CHECK( subtype_ == NORTH_POLE );
    x = akm1_ * tsfn( phi, sinePhi, eccentricity_ );
    y = -x * cosineLambda;
   break;
  }

  x *= sineLambda;

  POST02( ! isNan( x ), ! isNan( y ) );
}



/******************************************************************************
PURPOSE: projectSphere - Project a point on a sphere planet.
INPUTS:  double phi           Angle in radians of adjusted latitude to project.
         double sineLambda    Sine of lambda.
         double cosineLambda  Cosine of lambda.
         double sinePhi       Sine of phi.
OUTPUTS: double& x            Projected longitude.
         double& y            Projected latitude.
******************************************************************************/

void Stereographic::projectSphere(  double phi,
                                    double sineLambda, double cosineLambda,
                                    double sinePhi,
                                    double& x, double& y ) const {

  PRE06( eccentricity_ == 0.0,
         ! isNan( phi ),
         ! isNan( sineLambda ),
         ! isNan( cosineLambda ),
         ! isNan( sinePhi ),
         aboutEqual( sinePhi, sin( phi ) ) );

  x = y = 0.0;

  switch ( subtype_ ) {
  case EQUITORIAL:

    {
      const double cosinePhi = cos( phi );
      y = 1.0 + cosinePhi * cosineLambda;

      if ( y != 0.0) {
        y =  akm1_ / y;
        x = y * cosinePhi * sineLambda;
        y *= sinePhi;
      }
    }

    break;
  case OBLIQUE:

    {
      const double cosinePhi = cos( phi );
      y = 1.0 + sineX1_ * sinePhi + cosineX1_ * cosinePhi * cosineLambda;

      if ( y != 0.0) {
        y =  akm1_ / y;
        x = y * cosinePhi * sineLambda;
        y *= cosineX1_ * sinePhi - sineX1_ * cosinePhi * cosineLambda;
      }
    }

    break;
  case NORTH_POLE:

    if ( fabs( phi - PI_OVER_2 ) >= MAP_PROJECTION_TOLERANCE ) {
      y =  akm1_ * tan( PI_OVER_4 + 0.5 * -phi );
      x = y * sineLambda;
      y *= -cosineLambda;
    }

    break;
  default:
    CHECK( subtype_ == SOUTH_POLE );

    if ( fabs( phi - PI_OVER_2 ) >= MAP_PROJECTION_TOLERANCE ) {
      y =  akm1_ * tan( PI_OVER_4 + 0.5 * phi );
      x = y * sineLambda;
      y *= cosineLambda;
    }

    break;
  }

  POST02( ! isNan( x ), ! isNan( y ) );
}



/******************************************************************************
PURPOSE: unprojectSpheroid - Unproject a point on a non-sphere planet.
INPUTS:  double xp       Adjusted x-coordinate of point to unproject.
         double yp       Adjusted y-coordinate of point to unproject.
         double rho      Hypoteneuse of xp, yp.
OUTPUTS: double& lambda  Unprojected xp.
         double& phi     Unprojected yp.
******************************************************************************/

void Stereographic::unprojectSpheroid( double xp, double yp, double rho,
                                       double& lambda, double& phi ) const {

  PRE04( eccentricity_, ! isNan( xp ), ! isNan( yp ), ! isNan( rho ) );

  double sinePhi   = 0.0;
  double cosinePhi = 0.0;
  double phiL      = 0.0;
  double tp        = 0.0;
  double halfPi    = PI_OVER_2;
  double halfEccentricity = 0.5 * eccentricity_;
  int iteration = 0;

  lambda = phi = 0.0;

  switch ( subtype_ ) {
  case EQUITORIAL:
    // Fall through:
  case OBLIQUE:
    tp = atan2( rho * cosineX1_,  akm1_ );
    tp += tp;
    cosinePhi = cos( tp );
    sinePhi   = sin( tp );
    CHECK( rho != 0.0 );
    phiL = asin( cosinePhi * sineX1_ +
                 ( yp * sinePhi * cosineX1_ / rho ) );
    tp = tan( 0.5 * ( PI_OVER_2 + phiL ) );
    xp *= sinePhi;
    yp = rho * cosineX1_ * cosinePhi - yp * sineX1_ * sinePhi;
    break;
  case NORTH_POLE:
    yp = -yp;
    // Fall through:
  default: // SOUTH_POLE:
    CHECK(  akm1_ != 0.0 );
    tp = -rho /  akm1_;
    phiL = PI_OVER_2 - 2.0 * atan( tp );
    halfPi = -halfPi;
    halfEccentricity = -halfEccentricity;
    break;
  }

  do {
    sinePhi = eccentricity_ * sin( phiL );
    CHECK( sinePhi != 1.0 );
    phi = 2.0 * atan( tp * pow( ( 1.0 + sinePhi ) / ( 1.0 - sinePhi ),
                                halfEccentricity ) )
          - halfPi;

    if ( fabs( phiL - phi ) < MAP_PROJECTION_CONVERGENCE_TOLERANCE ) {

      if ( subtype_ == SOUTH_POLE ) {
        phi = -phi;
      }

      iteration = MAP_PROJECTION_CONVERGENCE_MAXIMUM_ITERATIONS;
    }

    ++iteration;
    phiL = phi;
  } while ( iteration < MAP_PROJECTION_CONVERGENCE_MAXIMUM_ITERATIONS );

  if ( iteration == MAP_PROJECTION_CONVERGENCE_MAXIMUM_ITERATIONS ) {
    lambda = phi = 0.0; // Failed to converge!
  } else {
#if 0
    lambda = IS_ZERO2( xp, yp ) ? 0.0 : atan2( xp, yp );
#else
    lambda = atan2( xp, yp );
#endif
  }

  POST02( ! isNan( lambda ), ! isNan( phi ) );
}



/******************************************************************************
PURPOSE: unprojectSphere - Unproject a point on a sphere planet.
INPUTS:  double xp           Adjusted x-coordinate of point to unproject.
         double yp           Adjusted y-coordinate of point to unproject.
         double rho          Hypoteneuse of xp, yp.
OUTPUTS: double& lambda      Unprojected xp.
         double& phi         Unprojected yp.
******************************************************************************/

void Stereographic::unprojectSphere( double xp, double yp, double rho,
                                     double& lambda, double& phi ) const {

  PRE04( eccentricity_ == 0.0, ! isNan( xp ), ! isNan( yp ), ! isNan( rho ) );

  double c = 2.0 * atan( rho /  akm1_ );
  const double cosineC = cos( c );

  lambda = phi = 0.0;

  switch ( subtype_ ) {
  case EQUITORIAL:

    {
      const double sineC = sin( c );

      if ( fabs( rho ) > MAP_PROJECTION_CONVERGENCE_TOLERANCE ) {
        phi = asin( yp * sineC / rho );
      }

      if ( OR2( cosineC, xp ) ) {
        lambda = atan2( xp * sineC, cosineC * rho );
      }
    }

    break;
  case OBLIQUE:

    {
      const double sineC = sin( c );

      if ( fabs( rho ) <= MAP_PROJECTION_CONVERGENCE_TOLERANCE ) {
        phi = phi0_;
      } else {
        phi = asin( cosineC * sineX1_ + yp * sineC * cosineX1_ / rho );
      }

      c = cosineC - sineX1_ * sin( phi );

      if ( OR2( c, xp ) ) {
        lambda = atan2( xp * sineC * cosineX1_, c * rho );
      }
    }

    break;
  case NORTH_POLE:
    yp = -yp;
    // Fall through.
  default:
    CHECK( IN3( subtype_, NORTH_POLE, SOUTH_POLE ) );

    if ( fabs( rho ) <= MAP_PROJECTION_CONVERGENCE_TOLERANCE ) {
      phi = phi0_;
    } else {
      phi = asin( subtype_ == SOUTH_POLE ? -cosineC : cosineC );
    }

    lambda = IS_ZERO2( xp, yp ) ? 0.0 : atan2( xp, yp );
    break;
  }

  POST02( ! isNan( lambda ), ! isNan( phi ) );
}




} // namespace emvl.



