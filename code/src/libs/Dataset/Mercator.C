/******************************************************************************
PURPOSE: Mercator.C - Define Mercator MapProjection ADT.
NOTES:   Derived from USGS PROJ Library.
HISTORY: 2013-02-05 plessel.todd@epa.gov
******************************************************************************/

//================================ INCLUDES ===================================

#include <string.h> // For strcmp().
#include <math.h>   // For M_PI, sqrt(), sin(), cos(), atan2(), log(), pow().
#include <float.h>  // For DBL_MAX.

#include <Assertions.h>    // For PRE*(), POST*(), CHECK*(), IN_RANGE().
#include <DataUtilities.h> // For isNan(), aboutEqual(), isLongitude().
#include <Mercator.h>      // For public interface.

//=========================== FORWARD DECLARATIONS ============================

namespace emvl {

const char* const Mercator::name_ = "Mercator"; // Initialize static attribute.

//============================= PUBLIC FUNCTIONS ==============================



/******************************************************************************
PURPOSE: ~Mercator - Destruct a Mercator.
******************************************************************************/

Mercator::~Mercator() {
  PRE( true );
  centralLongitude_ = 0.0;
  eccentricity_     = 0.0;
  lambda0_          = 0.0;
}

  
  
  
/******************************************************************************
PURPOSE: Mercator - Construct a Mercator.
INPUTS:  double majorSemiaxis    Mean equitorial radius of spheroid in meters.
         double minorSemiaxis    Mean polar      radius of spheroid in meters.
         double centralLongitude Longitude that projects to 0.
******************************************************************************/

Mercator::Mercator( double majorSemiaxis, double minorSemiaxis,
                    double centralLongitude ) {

  PRE02( isSpheroid( majorSemiaxis, minorSemiaxis ),
         isLongitude( centralLongitude ) );

  majorSemiaxis_    = majorSemiaxis;
  minorSemiaxis_    = minorSemiaxis;
  centralLongitude_ = centralLongitude;
  computeDerivedTerms();

  POST5( ! strcmp( this->name(), "Mercator" ),
         this->majorSemiaxis() == majorSemiaxis,
         this->minorSemiaxis() == minorSemiaxis,
         this->centralLongitude() == centralLongitude,
         this->centralLatitude() == 0.0 );
}



/******************************************************************************
PURPOSE: Mercator - Copy construct a Mercator.
INPUTS:  const Mercator& other  Other object to copy.
******************************************************************************/

Mercator::Mercator( const Mercator& other ) {

  PRE0( other.invariant() );

  majorSemiaxis_    = other.majorSemiaxis_;
  minorSemiaxis_    = other.minorSemiaxis_;
  centralLongitude_ = other.centralLongitude_;
  eccentricity_     = other.eccentricity_;
  lambda0_          = other.lambda0_;

  POST( *this == other );
}



/******************************************************************************
PURPOSE: operator= - Assign a Mercator.
INPUTS:  const Mercator& other  Other object to copy.
RETURNS: Mercator& *this.
******************************************************************************/

Mercator& Mercator::operator=( const Mercator& other ) {

  PRE0( other.invariant() );

  if ( this != &other ) {
    majorSemiaxis_    = other.majorSemiaxis_;
    minorSemiaxis_    = other.minorSemiaxis_;
    centralLongitude_ = other.centralLongitude_;
    eccentricity_     = other.eccentricity_;
    lambda0_          = other.lambda0_;
  }

  POST( *this == other );

  return *this;
}



/******************************************************************************
PURPOSE: newCopy - Allocate, copy and return this.
NOTES:   Caller must delete returned result when no longer needed.
******************************************************************************/

MapProjection* Mercator::newCopy() const {
  PRE( true );
  MapProjection* const result =
    new Mercator( majorSemiaxis_, minorSemiaxis_, centralLongitude_ );
  POST2( result, result->invariant() );
  return result;
}



/******************************************************************************
PURPOSE: invariant - Class invariant.
RETURNS: bool true if valid, else false and the code is defective.
******************************************************************************/

bool Mercator::invariant() const {

  const bool result =
    AND5( ! strcmp( Mercator::name_, "Mercator" ),
          isSpheroid( majorSemiaxis_, minorSemiaxis_ ),
          isLongitude( centralLongitude_ ),
          IN_RANGE( eccentricity_, 0.0, 1.0 ),
          aboutEqual( lambda0_ , radians( centralLongitude_ ) ) );

  return result;
}



/******************************************************************************
PURPOSE: name - Name of projection.
RETURNS: const char* "Mercator".
******************************************************************************/

const char* Mercator::name() const {
  const char* const result = name_;
  POST0( ! strcmp( result, "Mercator" ) );
  return result;
}



/******************************************************************************
PURPOSE: operator== - Are objects equal?
INPUTS:  const Mercator& other  Object to compare to.
RETURNS: bool true if equal, else false.
******************************************************************************/

bool Mercator::operator==( const Mercator& other ) const {

  PRE( other.invariant() );

  const bool result =
    AND4( name_ == other.name_,
          aboutEqual( majorSemiaxis_,    other.majorSemiaxis_ ),
          aboutEqual( minorSemiaxis_,    other.minorSemiaxis_ ),
          aboutEqual( centralLongitude_, other.centralLongitude_ ) );

  return result;
}


  
/******************************************************************************
PURPOSE: operator!= - Are objects unequal?
INPUTS:  const Mercator& other  Object to compare to.
RETURNS: bool true if unequal, else false.
******************************************************************************/

bool Mercator::operator!=( const Mercator& other ) const {
  PRE( other.invariant() );
  const bool result = ! ( *this == other );
  return result;
}



/******************************************************************************
PURPOSE: centralLongitude - Longitude that projects to 0.
RETURNS: double longitude that projects to 0.
******************************************************************************/

double Mercator::centralLongitude() const {
  PRE( true );
  const double result = centralLongitude_;
  POST( isLongitude( result ) );
  return result;
}



/******************************************************************************
PURPOSE: centralLatitude - Latitude that projects to 0.
RETURNS: double 0.0.
******************************************************************************/

double Mercator::centralLatitude() const {
  PRE( true );
  const double result = 0.0;
  POST( result == 0.0 );
  return result;
}



/******************************************************************************
PURPOSE: project - Project a point from longitude-latitude space to x-y space.
INPUTS:  double longitude  Longitude of point to project.
         double latitude   Latitude  of point to project.
OUTPUTS: double& x         X coordinate of projected longitude.
         double& y         Y coordinate of projected latitude.
******************************************************************************/

void Mercator::project( double longitude, double latitude,
                        double& x, double& y ) const {

  PRE2( isLongitude( longitude ), isLatitude( latitude ) );
  double lambda = 0.0; // Longitude in radians.
  double phi    = 0.0; // Latitude in radians.
  toLambdaPhi( longitude, latitude, lambda, phi );
  const double lambdaDelta = lambdaDifference( lambda, lambda0_ );
  x = lambdaDelta * majorSemiaxis_;

  if ( eccentricity_ == 0.0 ) { // Sphere:
    y = log( tan( PI_OVER_4 + phi * 0.5 ) );
  } else { // Non-sphere:
    y = -log( tsfn( phi, sin( phi ), eccentricity_ ) );
  }

  y *= majorSemiaxis_;

  x = clampedToRange( x, -M_PI * majorSemiaxis_ + MAP_PROJECTION_TOLERANCE,
                          M_PI * majorSemiaxis_ - MAP_PROJECTION_TOLERANCE );

  y = clampedToRange(y, -6.0 * M_PI * majorSemiaxis_ + MAP_PROJECTION_TOLERANCE,
                         6.0 * M_PI * majorSemiaxis_ -MAP_PROJECTION_TOLERANCE);

  POST2( IN_RANGE( x, -M_PI * majorSemiaxis(), M_PI * majorSemiaxis() ),
         IN_RANGE( y, -6.0 * M_PI * majorSemiaxis(),
                       6.0 * M_PI * majorSemiaxis() ) );
}



/******************************************************************************
PURPOSE: unproject - Unproject a point from x-y space to longitude-latitude.
INPUTS:  double x           X-coordinate of point to unproject.
         double y           X-coordinate of point to unproject.
OUTPUTS: double* longitude  Longitude of unprojected point X.
         double* latitude   Latitude  of unprojected point Y.
******************************************************************************/

void Mercator::unproject( double x, double y,
                          double& longitude, double& latitude ) const {

  PRE2( IN_RANGE( x, -M_PI * majorSemiaxis(),
                      M_PI * majorSemiaxis() ),
        IN_RANGE( y, -6.0 * M_PI * majorSemiaxis(),
                      6.0 * M_PI * majorSemiaxis() ) );

  const double oneOverMajorSemiaxis = 1.0 / majorSemiaxis_;
  const double expYP = exp( -y * oneOverMajorSemiaxis );
  const double lambda = x * oneOverMajorSemiaxis + lambda0_;
  const double phi =
    eccentricity_ == 0.0 ? PI_OVER_2 - 2.0 * atan( expYP )
    :                      phi2Iterate( expYP, eccentricity_ );
  toLongitudeLatitude( lambda, phi, longitude, latitude );

  POST2( isLongitude( longitude ), isLatitude( latitude ) );
}



//============================ PRIVATE FUNCTIONS ==============================



/******************************************************************************
PURPOSE: computeDerivedTerms - Compute trigonometry terms independent of
         longitude/latitude of projection point:
         eccentricity_  Of spheroid approximation of planet.
         lambda0_       Central longitude in radians.
******************************************************************************/

void Mercator::computeDerivedTerms() {
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
  eccentricity_ = eccentricity1;
  lambda0_ = radians( centralLongitude_ );

  POST2( IN_RANGE( eccentricity_, 0.0, 1.0 ),
         aboutEqual( lambda0_ , radians( centralLongitude_ ) ) );
}


} // namespace emvl.



