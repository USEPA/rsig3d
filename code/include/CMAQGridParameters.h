
#ifndef CMAQGRIDPARAMETERS_H
#define CMAQGRIDPARAMETERS_H

/******************************************************************************
PURPOSE: CMAQGridParameters.h - CMAQ grid parameters used for regridding.
HISTORY: 2013-02-27 plessel.todd@epa.gov
STATUS: unreviewed tested
******************************************************************************/

//================================== TYPES ====================================

namespace emvl {

class CMAQGridParameters {
public:

  // Types:

  // GDTYP:

  enum { LATGRD3 = 1, LAMGRD3 = 2, POLGRD3 = 6, EQMGRD3 = 7, ALBGRD3 = 9 };
  
  enum { MXLAYS3 = 100 }; // NLAYS.
  
  // Vertical grid types:

  enum {
    IMISS3  = -9999, // None (becomes a single layer at 0.0).
    VGSGPH3 = 1,     // Hydrostatic sigma-P
    VGSGPN3,         // Non-h sigma-P
    VGSIGZ3,         // Sigma-Z
    VGPRES3,         // Pressure (pascals)
    VGZVAL3,         // Z (m) (above sea level)
    VGHVAL3,         // Z (m) (above terrain)
    VGWRFEM          // Sigma-P WRF.
  };

private:

  // Attributes:

  int ncols_; // Number of CMAQ grid columns.
  int nrows_; // Number of CMAQ grid rows.
  int nlays_; // Number of CMAQ grid layers.
  int gdtyp_; // Projection type (LATGRD3, LAMGRD3, etc.) and parameters:
  double sphereRadius_; // Radius (m) of spherical Earth e.g., 6,370,000.
  double p_alp_; // gdtyp-specific parameters.
  double p_bet_;
  double p_gam_;
  double xcent_; // Longitude of center of projection.
  double ycent_; // Latitude of center of projection.
  double xorig_; // X-coordinate of south-west corner of grid.
  double yorig_; // Y-coordinate of south-west corner of grid.
  double xcell_; // West-East width of a grid cell in meters (deg iff LATGRD3).
  double ycell_; // South-North height of a grid cell in meters (deg iff ").
  double vgtop_; // Pressure (Pa) at top of model.
  int vgtyp_; // Vertical grid type: VGSGPN3, etc.
  double vglvls_[ MXLAYS3 + 1 ]; // Sigma-pressures at layer faces.
  double g_;  // Gravitational constant m/s2.
  double R_;  // J/kg/K.
  double A_;  // K.
  double T0_; // K.
  double P0_; // Pa.

  double thicknesses_[ MXLAYS3 ]; // Layer thicknesses in meters.

  void computeThicknesses();

public:

  static bool isValidParameters(
    const int ncols,
    const int nrows,
    const int nlays,
    const int gdtyp,
    const double sphereRadius,
    const double p_alp,
    const double p_bet,
    const double p_gam,
    const double xcent,
    const double ycent,
    const double xorig,
    const double yorig,
    const double xcell,
    const double ycell,
    const int vgtyp, 
    const double vgtop,
    const double vglvls[],
    const double g,
    const double R,
    const double A,
    const double T0,
    const double P0
  );

  // Commands:

  ~CMAQGridParameters();
  CMAQGridParameters();
  CMAQGridParameters(
    const int ncols,
    const int nrows,
    const int nlays,
    const int gdtyp,
    const double sphereRadius,
    const double p_alp,
    const double p_bet,
    const double p_gam,
    const double xcent,
    const double ycent,
    const double xorig,
    const double yorig,
    const double xcell,
    const double ycell,
    const int vgtyp, 
    const double vgtop,
    const double vglvls[],
    const double g,
    const double R,
    const double A,
    const double T0,
    const double P0
  );
  CMAQGridParameters( const char* const fileName ); //x
  CMAQGridParameters( const CMAQGridParameters& other );
  CMAQGridParameters& operator=( const CMAQGridParameters& other );

  // Queries:

  bool operator==( const CMAQGridParameters& other ) const;
  bool operator!=( const CMAQGridParameters& other ) const;
  bool invariant() const;
  int ncols() const;
  int nrows() const;
  int nlays() const;
  int gdtyp() const;
  double sphereRadius() const;
  double p_alp() const;
  double p_bet() const;
  double p_gam() const;
  double xcent() const;
  double ycent() const;
  double xorig() const;
  double yorig() const;
  double xcell() const;
  double ycell() const;
  int vgtyp() const; 
  double vgtop() const;
  const double* vglvls() const;
  double g() const;
  double R() const;
  double A() const;
  double T0() const;
  double P0() const;

  double layerThickness( const int layer ) const;
};


} // namespace emvl.

#endif /* CMAQGRIDPARAMETERS_H */


