#ifndef BOUNDS_H
#define BOUNDS_H

/*****************************************************************************
PURPOSE: Bounds.h - Longitude-latitude rectangle domain.
HISTORY: 2012-11-20 plessel.todd@epa.gov
STATUS: unreviewed tested
*****************************************************************************/

//================================= TYPES ====================================

namespace emvl {

class Bounds {
private:

  // Attributes:

  double west_;
  double east_;
  double south_;
  double north_;

public:

  // Commands:

  ~Bounds();
  Bounds( double west  = -180.0, double east  =  180.0,
          double south =  -90.0, double north =   90.0 );
  Bounds( const Bounds& );
  Bounds& operator=( const Bounds& );
  void west( double west );
  void east( double east );
  void south( double south );
  void north( double north );

  // Queries:

  double west() const;
  double east() const;
  double south() const;
  double north() const;
  bool operator==( const Bounds& ) const;
  bool operator!=( const Bounds& ) const;
  bool overlaps( const Bounds& ) const;
  bool invariant() const;
};

} // namespace emvl.

#endif /* BOUNDS_H */


