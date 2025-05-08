/*****************************************************************************
PURPOSE: Bounds.C - Longitude-latitude rectangle domain.
HISTORY: 2012-11-20 plessel.todd@epa.gov
*****************************************************************************/

//================================ INCLUDES ==================================

#include <algorithm> // For std::min(), std::max().

#include <Assertions.h>    // For PRE*(), POST*().
#include <DataUtilities.h> // For isLongitude(), isLatitude().
#include <Bounds.h>        // For Bounds.

//============================ PUBLIC FUNCTIONS ==============================

namespace emvl {



/******************************************************************************
PURPOSE: ~Bounds - Destruct a Bounds.
******************************************************************************/

Bounds::~Bounds() {
  PRE( true );
  west_  = 0.0;
  east_  = 0.0;
  south_ = 0.0;
  north_ = 0.0;
}



/******************************************************************************
PURPOSE: Bounds - Construct a Bounds.
INPUTS:  double west  West bound.
         double east  East bound.
         double south South bound.
         double north North bound.
******************************************************************************/

Bounds::Bounds( double west, double east, double south, double north ) {
  PRE06( isLongitude( west ), isLongitude( east ),
         isLatitude( south ), isLatitude( north ),
         west <= east, south <= north );
  west_ = west;
  east_ = east;
  south_ = south;
  north_ = north;
  POST4( this->west() == west, this->east() == east,
         this->south() == south, this->north() == north );
}



/******************************************************************************
PURPOSE: Bounds - Copy construct a Bounds.
INPUTS:  const Bounds& other  Other bounds to copy.
******************************************************************************/

Bounds::Bounds( const Bounds& other ) {
  PRE0( other.invariant() );
  west_ = other.west_;
  east_ = other.east_;
  south_ = other.south_;
  north_ = other.north_;
  POST4( west() == other.west(), east() == east(),
         south() == other.south(), north() == other.north() );
}



/******************************************************************************
PURPOSE: oprator= - Assign a Bounds.
INPUTS:  const Bounds& other  Bounds to copy.
RETURNS: Bounds& this.
******************************************************************************/

Bounds& Bounds::operator=( const Bounds& other ) {
  PRE( other.invariant() );

  if ( this != &other ) {
    west_ = other.west_;
    east_ = other.east_;
    south_ = other.south_;
    north_ = other.north_;
  }

  POST( operator==( other ) );
  return *this;
}



/******************************************************************************
PURPOSE: west - Set west edge of bounds.
INPUTS: double west edge of bounds.
******************************************************************************/

void Bounds::west( double west ) {
  PRE( isLongitude( west ) );
  west_ = west;
  east_ = std::max<double>( east_, west_ );
  POST( this->west() == west );
}



/******************************************************************************
PURPOSE: east - Set east edge of bounds.
INPUTS: double east edge of bounds.
******************************************************************************/

void Bounds::east( double east ) {
  PRE( isLongitude( east ) );
  east_ = east;
  west_ = std::min<double>( west_, east_ );
  POST( this->east() == east );
}



/******************************************************************************
PURPOSE: south - Set south edge of bounds.
INPUTS: double south edge of bounds.
******************************************************************************/

void Bounds::south( double south ) {
  PRE( isLatitude( south ) );
  south_ = south;
  north_ = std::max<double>( north_, south_ );
  POST( this->south() == south );
}



/******************************************************************************
PURPOSE: north - Set north edge of bounds.
INPUTS: double north edge of bounds.
******************************************************************************/

void Bounds::north( double north ) {
  PRE( isLatitude( north ) );
  north_ = north;
  south_ = std::min<double>( south_, north_ );
  POST( this->north() == north );
}



/******************************************************************************
PURPOSE: west - West edge of bounds.
RETURNS: double west edge of bounds.
******************************************************************************/

double Bounds::west() const {
  PRE( true );
  const double result = west_;
  POST( isLongitude( result ) );
  return result;
}



/******************************************************************************
PURPOSE: east - East edge of bounds.
RETURNS: double east edge of bounds.
******************************************************************************/

double Bounds::east() const {
  PRE( true );
  const double result = east_;
  POST( isLongitude( result ) );
  return result;
}



/******************************************************************************
PURPOSE: south - South edge of bounds.
RETURNS: double south edge of bounds.
******************************************************************************/

double Bounds::south() const {
  PRE( true );
  const double result = south_;
  POST( isLatitude( result ) );
  return result;
}



/******************************************************************************
PURPOSE: north - North edge of bounds.
RETURNS: double north edge of bounds.
******************************************************************************/

double Bounds::north() const {
  PRE( true );
  const double result = north_;
  POST( isLatitude( result ) );
  return result;
}



/******************************************************************************
PURPOSE: operator== - Are bounds equal?
INPUTS:  const Bounds& other  Bounds to compare to.
RETURNS: bool true if equal, else false.
******************************************************************************/

bool Bounds::operator==( const Bounds& other ) const {
  PRE( other.invariant() );
  const bool result =
    AND4( west_ == other.west_, east_ == other.east_,
          south_ == other.south_, north_ == other.north_ );
  return result;
}



/******************************************************************************
PURPOSE: operator!= - Are bounds unequal?
INPUTS:  const Bounds& other  Bounds to compare to.
RETURNS: bool true if equal, else false.
******************************************************************************/

bool Bounds::operator!=( const Bounds& other ) const {
  PRE( other.invariant() );
  const bool result =
    OR4( west_ != other.west_, east_ != other.east_,
         south_ != other.south_, north_ != other.north_ );
  return result;
}



/******************************************************************************
PURPOSE: overlaps - Do bounds overlap?
INPUTS:  const Bounds& other  Bounds to compare to.
RETURNS: bool true if they overlap, else false.
******************************************************************************/

bool Bounds::overlaps( const Bounds& other ) const {
  PRE( other.invariant() );
  const bool outside =
    OR4( south_ > other.north_,
         north_ < other.south_,
         west_ > other.east_,
         east_ < other.west_ );
  const bool result = ! outside;
  return result;
}



/******************************************************************************
PURPOSE: invariant - Is object valid?
RETURNS: bool true if valid, else false and the code contains a defect.
******************************************************************************/

bool Bounds::invariant() const {
  const bool result =
    AND6( isLongitude( west_ ), isLongitude( east_ ),
          isLatitude( south_ ), isLatitude( north_ ),
          west_ <= east_, south_ <= north_ );
  return result;
}



} // namepsace emvl.



