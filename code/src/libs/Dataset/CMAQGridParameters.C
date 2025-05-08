/******************************************************************************
PURPOSE: CMAQGridParameters.C - CMAQ grid parameters used for regridding.
HISTORY: 2013-02-27 plessel.todd@epa.gov
NOTES: See http://www.baronams.com/products/ioapi/GRIDS.html
******************************************************************************/

//================================ INCLUDES ===================================

#include <string.h> // For memset(), memcpy().
#include <math.h>   // For sqrt(), exp(), log().

#include <stdexcept> // For std::runtime_error.
#include <string>    // For std::string.

#include <Assertions.h>         // For PRE0*(), POST0*(), AND?().
#include <DataUtilities.h>      // For isNan().
#include <NetCDFUtilities.h>    // For *NetCDF*().
#include <CMAQGridParameters.h> // For public interface.

//============================= PUBLIC FUNCTIONS ==============================



namespace emvl {



/******************************************************************************
PURPOSE: ~CMAQGridParameters - Destructor.
******************************************************************************/

CMAQGridParameters::~CMAQGridParameters() {
  PRE( true );
  ncols_ = 0;
  nrows_ = 0;
  nlays_ = 0;
  gdtyp_ = 0;
  sphereRadius_ = 0.0;
  p_alp_ = 0.0;
  p_bet_ = 0.0;
  p_gam_ = 0.0;
  xcent_ = 0.0;
  ycent_ = 0.0;
  xorig_ = 0.0;
  yorig_ = 0.0;
  xcell_ = 0.0;
  ycell_ = 0.0;
  vgtop_ = 0.0;
  vgtyp_ = 0;
  memset( vglvls_, 0, sizeof vglvls_ );
  g_ = 0.0;
  R_ = 0.0;
  A_ = 0.0;
  T0_ = 0.0;
  P0_ = 0.0;
  memset( thicknesses_, 0, sizeof thicknesses_ );
}



/******************************************************************************
PURPOSE: CMAQGridParameters - Default constructor.
******************************************************************************/

CMAQGridParameters::CMAQGridParameters() {
  ncols_ = 459;
  nrows_ = 299;
  nlays_ = 34;
  gdtyp_ = 2;
  sphereRadius_ = 6370000.0;
  p_alp_ = 33.0;
  p_bet_ = 45.0;
  p_gam_ = -97.0;
  xcent_ = -97.0;
  ycent_ = 40.0;
  xorig_ = -2556000.0;
  yorig_ = -1728000.0;
  xcell_ = 12000.0;
  ycell_ = 12000.0;
  vgtop_ = 5000.0;
  vgtyp_ = VGSGPN3;
  memset( vglvls_, 0, sizeof vglvls_ );
  vglvls_[ 0  ] = 1.0;
  vglvls_[ 1  ] = 0.995;
  vglvls_[ 2  ] = 0.99;
  vglvls_[ 3  ] = 0.985;
  vglvls_[ 4  ] = 0.98;
  vglvls_[ 5  ] = 0.97;
  vglvls_[ 6  ] = 0.96;
  vglvls_[ 7  ] = 0.95;
  vglvls_[ 8  ] = 0.94;
  vglvls_[ 9  ] = 0.93;
  vglvls_[ 10 ] = 0.92;
  vglvls_[ 11 ] = 0.91;
  vglvls_[ 12 ] = 0.9;
  vglvls_[ 13 ] = 0.88;
  vglvls_[ 14 ] = 0.86;
  vglvls_[ 15 ] = 0.84;
  vglvls_[ 16 ] = 0.82;
  vglvls_[ 17 ] = 0.8;
  vglvls_[ 18 ] = 0.77;
  vglvls_[ 19 ] = 0.74;
  vglvls_[ 20 ] = 0.7;
  vglvls_[ 21 ] = 0.65;
  vglvls_[ 22 ] = 0.6;
  vglvls_[ 23 ] = 0.55;
  vglvls_[ 24 ] = 0.5;
  vglvls_[ 25 ] = 0.45;
  vglvls_[ 26 ] = 0.4;
  vglvls_[ 27 ] = 0.35;
  vglvls_[ 28 ] = 0.3;
  vglvls_[ 29 ] = 0.25;
  vglvls_[ 30 ] = 0.2;
  vglvls_[ 31 ] = 0.15;
  vglvls_[ 32 ] = 0.1;
  vglvls_[ 33 ] = 0.05;
  vglvls_[ 34 ] = 0.0;
  g_ = 9.81;
  R_ = 287.04;
  A_ = 50.0;
  T0_ = 275.0;
  P0_ = 100000.0;

  memset( thicknesses_, 0, sizeof thicknesses_ );
  computeThicknesses();

  POST21( ncols() == 459,
          nrows() == 299,
          nlays() == 34,
          gdtyp() == 2,
          sphereRadius() == 6370000.0,
          p_alp() == 33.0,
          p_bet() == 45.0,
          p_gam() == -97.0,
          xcent() == -97.0,
          ycent() == 40.0,
          xorig() == -2556000.0,
          yorig() == -1728000.0,
          xcell() == 12000.0,
          ycell() == 12000.0,
          vgtop() == 5000.0,
          vgtyp() == VGSGPN3,
          g() == 9.81,
          R() == 287.04,
          A() == 50.0,
          T0() == 275.0,
          P0() == 100000.0 );
}



/******************************************************************************
PURPOSE: CMAQGridParameters - Contructor.
INPUTS: const int ncols     Number of CMAQ grid columns.
        const int nrows     Number of CMAQ grid rows.
        const int nlays     Number of CMAQ grid layers.
        const int gdtyp     Projection type (LATGRD3, LAMGRD3, etc.)
        const double sphereRadius  Radius (m) of Earth e.g., 6,370,000.
        const double p_alp  gdtyp-specific parameters.
        const double p_bet
        const double p_gam
        const double xcent  Longitude of center of projection.
        const double ycent  Latitude of center of projection.
        const double xorig  X-coordinate of south-west corner of grid.
        const double yorig  Y-coordinate of south-west corner of grid.
        const double xcell  West-East width of grid cell (m or deg iff LATGRD3)
        const double ycell  South-North height of a grid cell (m) (deg iff ").
        const double vgtop  Pressure (Pa) at top of model.
        const int vgtyp     Vertical grid type: VGSGPN3, etc.
        const double vglvls[ MXLAYS3 + 1 ]  Sigma-pressures at layer faces.
        const double g      Gravitational constant m/s2.
        const double R      J/kg/K.
        const double A      K.
        const double T0     K.
        const double P0    Pa.

******************************************************************************/

CMAQGridParameters::CMAQGridParameters(
  const int ncols, const int nrows, const int nlays,
  const int gdtyp, const double sphereRadius,
  const double p_alp, const double p_bet, const double p_gam,
  const double xcent, const double ycent,
  const double xorig, const double yorig,
  const double xcell, const double ycell,
  const int vgtyp,  const double vgtop, const double vglvls[],
  const double g, const double R, const double A, const double T0,
  const double P0 ) {

  PRE0( isValidParameters( ncols, nrows, nlays,
                           gdtyp, sphereRadius,
                           p_alp, p_bet, p_gam,
                           xcent, ycent,
                           xorig, yorig,
                           xcell, ycell,
                           vgtyp, vgtop, vglvls,
                           g, R, A, T0, P0 ) );

  ncols_ = ncols;
  nrows_ = nrows;
  nlays_ = nlays;
  gdtyp_ = gdtyp;
  sphereRadius_ = sphereRadius;
  p_alp_ = p_alp;
  p_bet_ = p_bet;
  p_gam_ = p_gam;
  xcent_ = xcent;
  ycent_ = ycent;
  xorig_ = xorig;
  yorig_ = yorig;
  xcell_ = xcell;
  ycell_ = ycell;
  vgtop_ = vgtop;
  vgtyp_ = vgtyp;
  memset( vglvls_, 0, sizeof vglvls_ );
  memcpy( vglvls_, vglvls, ( nlays_ + 1 ) * sizeof vglvls_[ 0 ] );
  g_ = g;
  R_ = R;
  A_ = A;
  T0_ = T0;
  P0_ = P0;

  memset( thicknesses_, 0, sizeof thicknesses_ );
  computeThicknesses();

  POST( true );
}



/******************************************************************************
PURPOSE: CMAQGridParameters - Construct from a file.
INPUTS:  const char* const fileName  Name of CMAQ file to read.
NOTES:   throws std::runtime_error if failed to read valid CMAQ file.
******************************************************************************/

CMAQGridParameters::CMAQGridParameters( const char* const fileName ) {
  PRE02( fileName, *fileName );

  // Initialize all attributes:

  ncols_ = 0;
  nrows_ = 0;
  nlays_ = 0;
  gdtyp_ = 0;
  sphereRadius_ = 6370000.0;
  p_alp_ = 0.0;
  p_bet_ = 0.0;
  p_gam_ = 0.0;
  xcent_ = 0.0;
  ycent_ = 0.0;
  xorig_ = 0.0;
  yorig_ = 0.0;
  xcell_ = 0.0;
  ycell_ = 0.0;
  vgtop_ = 0.0;
  vgtyp_ = 0;
  memset( vglvls_, 0, sizeof vglvls_ );
  g_ = 9.81;
  R_ = 287.04;
  A_ = 50.0;
  T0_ = 275.0;
  P0_ = 100000.0;

  // Read attributes from file:

  const int file = openReadableNetCDFFile( fileName );
  int ok = 0;

  if ( file ) {
    ok = readNetCDFDimension( file, "COL", &ncols_ );
    ok = AND2( ok, readNetCDFDimension( file, "ROW", &nrows_ ) );
    ok = AND2( ok, readNetCDFDimension( file, "LAY", &nlays_ ) );
    ok = AND2( ok, readNetCDFIntAttribute( file, "GDTYP", &gdtyp_ ) );
    ok = AND2( ok, readNetCDFDoubleAttribute( file, "P_ALP", &p_alp_ ) );
    ok = AND2( ok, readNetCDFDoubleAttribute( file, "P_BET", &p_bet_ ) );
    ok = AND2( ok, readNetCDFDoubleAttribute( file, "P_GAM", &p_gam_ ) );
    ok = AND2( ok, readNetCDFDoubleAttribute( file, "XCENT", &xcent_ ) );
    ok = AND2( ok, readNetCDFDoubleAttribute( file, "YCENT", &ycent_ ) );
    ok = AND2( ok, readNetCDFDoubleAttribute( file, "XORIG", &xorig_ ) );
    ok = AND2( ok, readNetCDFDoubleAttribute( file, "YORIG", &yorig_ ) );
    ok = AND2( ok, readNetCDFDoubleAttribute( file, "XCELL", &xcell_ ) );
    ok = AND2( ok, readNetCDFDoubleAttribute( file, "YCELL", &ycell_ ) );
    ok = AND2( ok, readNetCDFIntAttribute( file, "VGTYP", &vgtyp_ ) );
    float vgtop = 0.0;
    ok = AND2( ok, readNetCDFFloatAttribute( file, "VGTOP", &vgtop ) );
    vgtop_ = vgtop;
    float vglvls[ MXLAYS3 + 1 ];
    memset( vglvls, 0, sizeof vglvls );

    ok = AND3( ok, nlays_ <= MXLAYS3,
               readNetCDFFloatArrayAttribute( file, "VGLVLS", nlays_ + 1,
                                              vglvls ) );

    if ( ok ) {

      for ( int level = 0; level <= nlays_; ++level ) {
        vglvls_[ level ] = vglvls[ level ];
      }
    }

    ok = AND2( ok, isValidParameters( ncols_, nrows_, nlays_,
                                      gdtyp_, sphereRadius_,
                                      p_alp_, p_bet_, p_gam_,
                                      xcent_,  ycent_,
                                      xorig_, yorig_,
                                      xcell_, ycell_,
                                      vgtyp_, vgtop_, vglvls_,
                                      g_, R_, A_, T0_,
                                      P0_ ) );
    closeNetCDFFile( file );
  }

  if ( ! ok ) {
    throw std::runtime_error( "Failed to read valid CMAQ grid parameters "
                              "from file " + std::string( fileName ) ); //x
  }

  memset( thicknesses_, 0, sizeof thicknesses_ );
  computeThicknesses();

  POST( true );
}



/******************************************************************************
PURPOSE: CMAQGridParameters - Copy construct a timestamp.
INPUTS:  const CMAQGridParameters& other  Object to copy.
******************************************************************************/

CMAQGridParameters::CMAQGridParameters( const CMAQGridParameters& other ) {
  PRE0( other.invariant() );
  ncols_ = other.ncols_;
  nrows_ = other.nrows_;
  nlays_ = other.nlays_;
  gdtyp_ = other.gdtyp_;
  sphereRadius_ = other.sphereRadius_;
  p_alp_ = other.p_alp_;
  p_bet_ = other.p_bet_;
  p_gam_ = other.p_gam_;
  xcent_ = other.xcent_;
  ycent_ = other.ycent_;
  xorig_ = other.xorig_;
  yorig_ = other.yorig_;
  xcell_ = other.xcell_;
  ycell_ = other.ycell_;
  vgtop_ = other.vgtop_;
  vgtyp_ = other.vgtyp_;
  memcpy( vglvls_, other.vglvls_, sizeof vglvls_ );
  g_ = other.g_;
  R_ = other.R_;
  A_ = other.A_;
  T0_ = other.T0_;
  P0_ = other.P0_;
  memcpy( thicknesses_, other.thicknesses_, sizeof thicknesses_ );
  POST( *this == other );
}



/******************************************************************************
PURPOSE: operator= - Copy a timestamp.
INPUTS:  const CMAQGridParameters& other  Object to copy.
******************************************************************************/

CMAQGridParameters& CMAQGridParameters::operator=(
  const CMAQGridParameters& other ) {
  PRE( other.invariant() );

  if ( this != &other ) {
    ncols_ = other.ncols_;
    nrows_ = other.nrows_;
    nlays_ = other.nlays_;
    gdtyp_ = other.gdtyp_;
    sphereRadius_ = other.sphereRadius_;
    p_alp_ = other.p_alp_;
    p_bet_ = other.p_bet_;
    p_gam_ = other.p_gam_;
    xcent_ = other.xcent_;
    ycent_ = other.ycent_;
    xorig_ = other.xorig_;
    yorig_ = other.yorig_;
    xcell_ = other.xcell_;
    ycell_ = other.ycell_;
    vgtop_ = other.vgtop_;
    vgtyp_ = other.vgtyp_;
    memcpy( vglvls_, other.vglvls_, sizeof vglvls_ );
    g_ = other.g_;
    R_ = other.R_;
    A_ = other.A_;
    T0_ = other.T0_;
    P0_ = other.P0_;
    memcpy( thicknesses_, other.thicknesses_, sizeof thicknesses_ );
  }

  POST( *this == other );
  return *this;
}



/******************************************************************************
PURPOSE: operator== - Are objects equal?
INPUTS:  const CMAQGridParameters& other  Object to compare to.
RETURNS: bool true if equal, else false.
******************************************************************************/

bool CMAQGridParameters::operator==( const CMAQGridParameters& other ) const {
  PRE( other.invariant() );
  const bool result =
    AND23( ncols_ == other.ncols_,
           nrows_ == other.nrows_,
           nlays_ == other.nlays_,
           gdtyp_ == other.gdtyp_,
           sphereRadius_ == other.sphereRadius_,
           p_alp_ == other.p_alp_,
           p_bet_ == other.p_bet_,
           p_gam_ == other.p_gam_,
           xcent_ == other.xcent_,
           ycent_ == other.ycent_,
           xorig_ == other.xorig_,
           yorig_ == other.yorig_,
           xcell_ == other.xcell_,
           ycell_ == other.ycell_,
           vgtop_ == other.vgtop_,
           vgtyp_ == other.vgtyp_,
           ! memcmp( vglvls_, other.vglvls_, sizeof vglvls_ ),
           g_ == other.g_,
           R_ == other.R_,
           A_ == other.A_,
           T0_ == other.T0_,
           P0_ == other.P0_,
           ! memcmp( thicknesses_, other.thicknesses_, sizeof thicknesses_ ) );
  return result;
}



/******************************************************************************
PURPOSE: operator!= - Are objects unequal?
INPUTS:  const CMAQGridParameters& other  Object to compare to.
RETURNS: bool true if unequal, else false.
******************************************************************************/

bool CMAQGridParameters::operator!=( const CMAQGridParameters& other ) const {
  PRE( other.invariant() );
  const bool result = ! ( *this == other );
  return result;
}



/******************************************************************************
PURPOSE: invariant - Class invariant.
RETURNS: bool true otherwise the code is defective.
******************************************************************************/

bool CMAQGridParameters::invariant() const {
  const bool result =
    isValidParameters( ncols_, nrows_, nlays_,
                       gdtyp_, sphereRadius_,
                       p_alp_, p_bet_, p_gam_,
                       xcent_, ycent_,
                       xorig_, yorig_,
                       xcell_, ycell_,
                       vgtyp_, vgtop_, vglvls_,
                       g_, R_, A_, T0_, P0_ );
  return result;
}



/******************************************************************************
PURPOSE: ncols - Returns number of columns of grid cells.
RETURNS: int number of columns of grid cells.
******************************************************************************/

int CMAQGridParameters::ncols() const {
  PRE( true );
  const int result = ncols_;
  POST( result > 0 );
  return result;
}



/******************************************************************************
PURPOSE: nrows - Returns number of rows of grid cells.
RETURNS: int number of rows of grid cells.
******************************************************************************/

int CMAQGridParameters::nrows() const {
  PRE( true );
  const int result = nrows_;
  POST( result > 0 );
  return result;
}



/******************************************************************************
PURPOSE: nlays - Returns number of layers of grid cells.
RETURNS: int number of layers of grid cells.
******************************************************************************/

int CMAQGridParameters::nlays() const {
  PRE( true );
  const int result = nlays_;
  POST( IN_RANGE( result, 0, MXLAYS3 ) );
  return result;
}



/******************************************************************************
PURPOSE: gdtyp - Returns projection of grid.
RETURNS: int projection of grid.
******************************************************************************/

int CMAQGridParameters::gdtyp() const {
  PRE( true );
  const int result = gdtyp_;
  POST( IN6( result, LATGRD3, LAMGRD3, POLGRD3, EQMGRD3, ALBGRD3 ) );
  return result;
}



/******************************************************************************
PURPOSE: sphereRadius - Returns mean radius of Earth approximation, in meters.
RETURNS: double mean radius of Earth approximation, in meters.
******************************************************************************/

double CMAQGridParameters::sphereRadius() const {
  PRE( true );
  const double result = sphereRadius_;
  POST( IN_RANGE( result, 1.0, 1e8 ) );
  return result;
}



/******************************************************************************
PURPOSE: p_alp - Returns p_alp projection parameter.
RETURNS: double p_alp projection parameter.
******************************************************************************/

double CMAQGridParameters::p_alp() const {
  PRE( true );
  const double result = p_alp_;
  POST( IMPLIES_ELSE( gdtyp() == LATGRD3, result == 0.0,
                      isLatitude( result ) ) );
  return result;
}



/******************************************************************************
PURPOSE: p_bet - Returns p_bet projection parameter.
RETURNS: double p_bet projection parameter.
******************************************************************************/

double CMAQGridParameters::p_bet() const {
  PRE( true );
  const double result = p_bet_;
  POST( IMPLIES_ELSE( gdtyp() == LATGRD3, result == 0.0,
                      isLatitude( result ) ) );
  return result;
}



/******************************************************************************
PURPOSE: p_gam - Returns p_gam projection parameter.
RETURNS: double p_gam projection parameter.
******************************************************************************/

double CMAQGridParameters::p_gam() const {
  PRE( true );
  const double result = p_gam_;
  POST( IMPLIES_ELSE( gdtyp() == LATGRD3, result == 0.0,
                      isLongitude( result ) ) );
  return result;
}



/******************************************************************************
PURPOSE: xcent - Returns xcent projection parameter.
RETURNS: double X-coordinate of projection origin.
******************************************************************************/

double CMAQGridParameters::xcent() const {
  PRE( true );
  const double result = xcent_;
  POST( IMPLIES_ELSE( gdtyp() == LATGRD3, result == 0.0, isLongitude(result)));
  return result;
}



/******************************************************************************
PURPOSE: ycent - Returns ycent projection parameter.
RETURNS: double Y-coordinate of projection origin.
******************************************************************************/

double CMAQGridParameters::ycent() const {
  PRE( true );
  const double result = ycent_;
  POST( IMPLIES_ELSE( gdtyp() == LATGRD3, result == 0.0, isLatitude( result)));
  return result;
}



/******************************************************************************
PURPOSE: xorig - Returns xorig projection parameter.
RETURNS: double X-coordinate of lower-left corner of grid.
******************************************************************************/

double CMAQGridParameters::xorig() const {
  PRE( true );
  const double result = xorig_;
  POST( IN_RANGE( result, -1e15, 1e15 ) );
  return result;
}



/******************************************************************************
PURPOSE: yorig - Returns yorig projection parameter.
RETURNS: double Y-coordinate of lower-left corner of grid.
******************************************************************************/

double CMAQGridParameters::yorig() const {
  PRE( true );
  const double result = yorig_;
  POST( IN_RANGE( result, -1e15, 1e15 ) );
  return result;
}



/******************************************************************************
PURPOSE: xcell - Returns grid cell width.
RETURNS: double grid cell width in meters or degrees iff gdtyp() is LATGRD3.
******************************************************************************/

double CMAQGridParameters::xcell() const {
  PRE( true );
  const double result = xcell_;
  POST( IMPLIES_ELSE( gdtyp() == LATGRD3,
                      IN_RANGE( result, 0.01, 10.0 ),
                      IN_RANGE( result, 1.0, 1e6 ) ) );
  return result;
}



/******************************************************************************
PURPOSE: ycell - Returns grid cell height.
RETURNS: double grid cell height in meters or degrees iff gdtyp() is LATGRD3.
******************************************************************************/

double CMAQGridParameters::ycell() const {
  PRE( true );
  const double result = ycell_;
  POST( IMPLIES_ELSE( gdtyp() == LATGRD3,
                      IN_RANGE( result, 0.01, 10.0 ),
                      IN_RANGE( result, 1.0, 1e6 ) ) );
  return result;
}



/******************************************************************************
PURPOSE: vgtyp - Returns grid cell height.
RETURNS: int grid cell height in meters or degrees iff gdtyp() is LATGRD3.
******************************************************************************/

int CMAQGridParameters::vgtyp() const {
  PRE( true );
  const int result = vgtyp_;
  POST( IN9( result,
             IMISS3,
             VGSGPH3, VGSGPN3, VGSIGZ3, VGPRES3, VGZVAL3, VGHVAL3, VGWRFEM ) );
  return result;
}



/******************************************************************************
PURPOSE: vgtop - Returns pressure (Pa) at top of model.
RETURNS: double pressure (Pa) at top of model.
******************************************************************************/

double CMAQGridParameters::vgtop() const {
  PRE( true );
  const double result = vgtop_;
  POST( IN_RANGE( result, 1.0, 1e5 ) );
  return result;
}



/******************************************************************************
PURPOSE: vglvls - Returns vertical level coordinates.
RETURNS: const double* vertical level coordinates (nlays + 1 of them).
******************************************************************************/

const double* CMAQGridParameters::vglvls() const {
  PRE( true );
  const double* const result = vglvls_;
  POST( isNanFree( result, nlays() + 1 ) );
  return result;
}



/******************************************************************************
PURPOSE: g - Returns gravitational constant.
RETURNS: double g (m/s2).
******************************************************************************/

double CMAQGridParameters::g() const {
  PRE( true );
  const double result = g_;
  POST( IN_RANGE( result, 1.0, 100.0 ) );
  return result;
}



/******************************************************************************
PURPOSE: R - Returns R constant.
RETURNS: double (J/kg/K).
******************************************************************************/

double CMAQGridParameters::R() const {
  PRE( true );
  const double result = R_;
  POST( IN_RANGE( result, 200.0, 400.0 ) );
  return result;
}



/******************************************************************************
PURPOSE: A - Returns A constant.
RETURNS: double (K).
******************************************************************************/

double CMAQGridParameters::A() const {
  PRE( true );
  const double result = A_;
  POST( IN_RANGE( result, 10.0, 100.0 ) );
  return result;
}



/******************************************************************************
PURPOSE: T0 - Returns T0 constant.
RETURNS: double (K).
******************************************************************************/

double CMAQGridParameters::T0() const {
  PRE( true );
  const double result = T0_;
  POST( IN_RANGE( result, 250.0, 350.0 ) );
  return result;
}




/******************************************************************************
PURPOSE: P0 - Returns P0 constant.
RETURNS: double (Pa).
******************************************************************************/

double CMAQGridParameters::P0() const {
  PRE( true );
  const double result = P0_;
  POST( IN_RANGE( result, 1000.0, 1e6 ) );
  return result;
}



/******************************************************************************
PURPOSE: layerThickness - Returns thickness in meters of a given layer.
INPUTS:  const int layer  0-based index of layer.
RETURNS: double meters.
******************************************************************************/

double CMAQGridParameters::layerThickness( const int layer ) const {
  PRE( IN_RANGE( layer, 0, nlays_ - 1 ) );
  const double result = thicknesses_[ layer ];
  POST( IN_RANGE( result, 0.0, 1e5 ) );
  return result;
}



/******************************************************************************
PURPOSE: isValidParameters - Are the parameters valid?
INPUTS: const int ncols     Number of CMAQ grid columns.
        const int nrows     Number of CMAQ grid rows.
        const int nlays     Number of CMAQ grid layers.
        const int gdtyp     Projection type (LATGRD3, LAMGRD3, etc.)
        const double sphereRadius  Radius (m) of Earth e.g., 6,370,000.
        const double p_alp  gdtyp-specific parameters.
        const double p_bet
        const double p_gam
        const double xcent  Longitude of center of projection.
        const double ycent  Latitude of center of projection.
        const double xorig  X-coordinate of south-west corner of grid.
        const double yorig  Y-coordinate of south-west corner of grid.
        const double xcell  West-East width of grid cell (m or deg iff LATGRD3)
        const double ycell  South-North height of a grid cell (m) (deg iff ").
        const double vgtop  Pressure (Pa) at top of model.
        const int vgtyp     Vertical grid type: VGSGPN3, etc.
        const double vglvls[ MXLAYS3 + 1 ]  Sigma-pressures at layer faces.
        const double g      Gravitational constant m/s2.
        const double R      J/kg/K.
        const double A      K.
        const double T0     K.
        const double P0     Pa.
RETURNS: bool true if valid, else false.
******************************************************************************/

bool CMAQGridParameters::isValidParameters(
  const int ncols, const int nrows, const int nlays,
  const int gdtyp, const double sphereRadius,
  const double p_alp, const double p_bet, const double p_gam,
  const double xcent, const double ycent,
  const double xorig, const double yorig,
  const double xcell, const double ycell,
  const int vgtyp,  const double vgtop, const double vglvls[],
  const double g, const double R, const double A, const double T0,
  const double P0 ) {

  const bool result =
    AND24( ncols > 0,
           nrows > 0,
           ncols * nrows > 0,
           IN_RANGE( nlays, 1, MXLAYS3 ),
           ncols * nrows * nlays > 0,
           IN6( gdtyp, LATGRD3, LAMGRD3, POLGRD3, EQMGRD3, ALBGRD3 ),
           IN_RANGE( sphereRadius, 1.0, 1e8 ),
           IMPLIES( IN3( gdtyp, LAMGRD3, ALBGRD3 ),
                    AND6( isLatitude( p_alp ),
                          isLatitude( p_bet ),
                          p_alp <= p_bet,
                          SIGN( p_alp ) == SIGN( p_bet ),
                          IMPLIES_ELSE( p_alp >= 0.0,
                                        IN_RANGE( p_alp, 1.0, 89.0 ),
                                        IN_RANGE( p_alp, -89.0, -1.0 ) ),
                          IMPLIES_ELSE( p_bet >= 0.0,
                                        IN_RANGE( p_bet, 1.0, 89.0 ),
                                        IN_RANGE( p_bet, -89.0, -1.0 ) ) ) ),
           IMPLIES( gdtyp == POLGRD3,
                    AND3( IN3( p_alp, -1.0, 1.0 ),
                          isLatitude( p_bet ),
                          isLongitude( p_gam ) ) ),
           IMPLIES( gdtyp == EQMGRD3,
                    AND3( isLatitude( p_alp ),
                          p_bet == 0.0,
                          isLongitude( p_gam ) ) ),
           IN_RANGE( xorig, -1e15, 1e15 ),
           IN_RANGE( xorig, -1e15, 1e15 ),
           IMPLIES_ELSE( gdtyp == LATGRD3,
                      IN_RANGE( xcell, 0.01, 10.0 ),
                      IN_RANGE( xcell, 1.0, 1e6 ) ),
           IMPLIES_ELSE( gdtyp == LATGRD3,
                      IN_RANGE( ycell, 0.01, 10.0 ),
                      IN_RANGE( ycell, 1.0, 1e6 ) ),
           IN9( vgtyp,
                IMISS3,
                VGSGPH3, VGSGPN3, VGSIGZ3, VGPRES3, VGZVAL3, VGHVAL3, VGWRFEM),
           IN_RANGE( vgtop, 1.0, 1e5 ),
           vglvls,
           isNanFree( vglvls, nlays + 1 ),
           IMPLIES_ELSE( IN4( vgtyp, VGSIGZ3, VGZVAL3, VGHVAL3 ),
                         AND3( isIncreasing( vglvls, nlays + 1 ),
                               minimum( vglvls, nlays + 1 ) >= -1000.0,
                               maximum( vglvls, nlays + 1 ) <= 1000000.0 ),
                         AND3( isDecreasing( vglvls, nlays + 1 ),
                               minimum( vglvls, nlays + 1 ) >= 0.0,
                               maximum( vglvls, nlays + 1 ) <= 1.0 ) ),
           IN_RANGE( g, 1.0, 100.0 ),
           IN_RANGE( R, 200.0, 400.0 ),
           IN_RANGE( A, 10.0, 100.0 ),
           IN_RANGE( T0, 250.0, 350.0 ),
           IN_RANGE( P0, 1000.0, 1e6 ) );

  return result;
}



/******************************************************************************
PURPOSE: computeThicknesses - Compute thickness in meters of each layer.
******************************************************************************/

void CMAQGridParameters::computeThicknesses() {

  const int levels = nlays_ + 1;
  memset( thicknesses_, 0, sizeof thicknesses_ );

  if ( IN3( vgtyp_, VGSGPH3, VGSGPN3 ) ) {
    const double surfaceElevation = 0.0; // Not needed for computing thickness?
    const double H0s            = R_ * T0_ / g_;
    const double one_over_H0s   = 1.0 / H0s;
    const double A_over_T0s     = A_ / T0_;
    const double A_over_two_T0s = A_ / ( T0_ + T0_ );
    const double Pt             = vgtop_;
    const double Zs             = surfaceElevation;
    const double two_Zs         = Zs + Zs;
    const double sqrt_factor = sqrt( 1.0 - A_over_T0s * one_over_H0s * two_Zs );
    const double q_factor =
     ( Pt / P0_ ) * exp( two_Zs * one_over_H0s / sqrt_factor );
    double elevations[ MXLAYS3 + 1 ];
    memset( elevations, 0, sizeof elevations );

    // Compute elevations at sigma-pressures:

    for ( int level = 0; level < levels; ++level ) {
      const double sigma_p0   = vglvls_[ level ];
      const double q0_star    = sigma_p0 + ( 1.0 - sigma_p0 ) * q_factor;
      const double ln_q0_star = log( q0_star );
      const double z_level    =
        Zs - H0s * ln_q0_star * ( A_over_two_T0s * ln_q0_star + sqrt_factor );
      elevations[ level ] = z_level;
    }

    // Compute thickness of each layer:

    for ( int layer = 0; layer < nlays_; ++layer ) {
      const double lower_elevation = elevations[ layer ];
      const double upper_elevation = elevations[ layer + 1 ];
      const double thickness = upper_elevation - lower_elevation;
      thicknesses_[ layer ] = thickness;
    }

  } else if ( IN3( vgtyp_, VGZVAL3, VGHVAL3 ) ) {

    // Compute thickness of each layer:

    for ( int layer = 0; layer < nlays_; ++layer ) {
      const double lower_elevation = vglvls_[ layer ];
      const double upper_elevation = vglvls_[ layer + 1 ];
      const double thickness = upper_elevation - lower_elevation;
      thicknesses_[ layer ] = thickness;
    }
  }
  // UNIMPLEMENTED: other vgtyp schemes!
}



} // End namespace emvl.



