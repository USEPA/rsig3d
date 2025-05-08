//
// This file is part of the Marble Virtual Globe.
//
// This program is free software licensed under the GNU LGPL. You can
// find a copy of this license in LICENSE.txt in the top directory of
// the source code.
//
// Copyright 2013  Nan Zhang  <zhang.nan@epa.gov>
//

#include <math.h>

// Local
#include "LambertConformalConicProjection.h"
#include "MarbleDebug.h"

// Marble
#include "ViewportParams.h"
#include "MathHelper.h"
#include "GeoDataPoint.h"


//Lambert Conformal Conic Projection
//http://en.wikipedia.org/wiki/Lambert_conformal_conic_projection
#ifndef M_PI
#define M_PI (3.1415926535897932384626433)
#endif
#ifndef M_2_PI
#define M_2_PI (2.0*M_PI)
#endif
#ifndef M_PI_2
#define M_PI_2 (M_PI*0.5)
#endif
#ifndef M_PI_4 
#define M_PI_4 (M_PI*0.25)
#endif

inline double cot(const double& x){
	return  tan(M_PI_2 - x);
}

inline double sec(const double& x){
	return 1.0/cos(x);
}

inline double sgn(const double& x){
	double s;
    if (x==0.0) s=0.0;
    else if (x>0.0) s=1.0;
    else s=-1.0;
	return s;
}

inline void toScreenCoordinate(const double & lambda,  //longitude, input
					  const double & phi,     //latitude, input
					  const double & lambda0, //reference longitude
					  const double & phi0,    //reference latitude
					  const double & phi1, const double & phi2,  //two standard parallels (latitudes)
					  double& x, double &y)   //output screen x, y (unit not known yet)
{
    const double temp1 = tan(M_PI_4+0.5*phi1);
    const double n = log(cos(phi1)*sec(phi2))/log(tan(M_PI_4+0.5*phi2)/temp1);
    const double F = cos(phi1)*pow(temp1, n)/n;
	const double rho0 = F * pow(cot(M_PI_4 + 0.5*phi0), n);
    const double rho = F * pow(cot(M_PI_4 + 0.5*phi), n);
    
	const double t = n*(lambda - lambda0);
	x = rho * sin(t);	
	y = rho0 - rho * cos(t);
}

inline void toScreenCoordinate(const double & lambda,  //longitude, input
							   const double & phi,     //latitude, input
							   const double & lambda0, //reference longitude
							   const double & phi0,    //reference latitude
							   const double & phi1, const double & phi2, //two standard parallels (latitudes)
							   const double &n, const double &F, const double rho0,
							   double& x, double &y)   //output screen x, y (unit not known yet)
{
    const double rho = F * pow(cot(M_PI_4 + 0.5*phi), n);    
	const double t = n*(lambda - lambda0);
	x = rho * sin(t);	
	y = rho0 - rho * cos(t);
}

//the inverse transform is from:
//http://mathworld.wolfram.com/LambertConformalConicProjection.html
inline void toGeoCoordinate(const double & x,       //screen x, input
				   const double & y,       //screen y, input
				   const double & lambda0, //reference longitude
				   const double & phi0,    //reference latitude
				   const double & phi1, const double & phi2, //two standard parallels (latitudes)
				   double& lambda, double &phi)   //output <lon,lat>
{
    const double temp1 = tan(M_PI_4+0.5*phi1);
    const double n = log(cos(phi1)*sec(phi2))/log(tan(M_PI_4+0.5*phi2)/temp1);
    const double F = cos(phi1)*pow(temp1, n)/n;
	const double rho0 = F * pow(cot(M_PI_4 + 0.5*phi0), n);
	const double rho0y = rho0 - y;
    
	const double rho = sgn(n) * sqrt(x*x + rho0y*rho0y);
    const double theta = atan2(x, rho0y);
    
    lambda = lambda0 + theta/n;
	phi = 2.0*atan( pow(F/rho, 1.0/n) ) - M_PI_2;
}

inline void toGeoCoordinate(const double & x,       //screen x, input
							const double & y,       //screen y, input
							const double & lambda0, //reference longitude
							const double & phi0,    //reference latitude
							const double & phi1, const double & phi2, //two standard parallels (latitudes)
							const double &n, const double &F, const double& rho0,
							double& lambda, double &phi)   //output <lon,lat>
{
	const double rho0y = rho0 - y;    
	const double rho = sgn(n) * sqrt(x*x + rho0y*rho0y);
    const double theta = atan2(x, rho0y);
    
	lambda = lambda0 + theta/n;	
	phi = 2.0*atan( pow(F/rho, 1.0/n) ) - M_PI_2;
}


//=====================================================================================

using namespace Marble;

LambertConformalConicProjection::LambertConformalConicProjection()
    : ConicalProjection()
    ,m_standardParalle1(33.0 * DEG2RAD) // EMVL HACK. Changed parallel to what is used in CMAQ
    ,m_standardParalle2(45.0 * DEG2RAD) // EMVL HACK. Changed parallel to what is used in CMAQ
{
	/*
    setRepeatX( repeatableX() );
    setMinLat( minValidLat() );
    setMaxLat( maxValidLat() );
	*/
  
    setRepeatX( false );
	const qreal angle = 87.50 * DEG2RAD;
    setMinLat( -angle );
    setMaxLat( +angle );
}

LambertConformalConicProjection::~LambertConformalConicProjection()
{
}

void LambertConformalConicProjection::get_n_F_rho0(const double phi0, double &n, double &F, double& rho0) const
{
	const double phi1 = standardParallel1();
	const double phi2 = standardParallel2();	
    n = log(cos(phi1)*sec(phi2))/log(tan(M_PI_4+0.5*phi2)*cot(M_PI_4+0.5*phi1));
	F = cos(phi1)*pow(tan(M_PI_4+0.5*phi1), n)/n;
	rho0 = F * pow(cot(M_PI_4 + 0.5*phi0), n);	
}

qreal LambertConformalConicProjection::standardParallel1() const
{
	return m_standardParalle1;
}

qreal LambertConformalConicProjection::standardParallel2() const
{
	return m_standardParalle2;
}

void LambertConformalConicProjection::setStandardParallel1(qreal x)
{
	m_standardParalle1 = x;
}

void LambertConformalConicProjection::setStandardParallel2(qreal x)
{
	m_standardParalle2 = x;
}

bool LambertConformalConicProjection::screenCoordinates(const qreal lon, const qreal _lat,
	const ViewportParams *viewport, 
    const qreal n, const qreal F, const qreal rho0,
	qreal& x, qreal& y ) const
{
    const bool isLatValid = minLat() <= _lat && _lat <= maxLat();
    qreal lat = _lat;
    if ( lat > maxLat() ) lat = maxLat();
    if ( lat < minLat() ) lat = minLat();
    
    // Convenience variables
    const int radius = viewport->radius();
    const qreal width  = qreal(viewport->width());
    const qreal height = qreal(viewport->height());
    const qreal rad2Pixel = (2 * radius) / M_PI;

    const double lambda0 = viewport->centerLongitude();
    const double phi0 = viewport->centerLatitude();
	const double phi1 = standardParallel1();
	const double phi2 = standardParallel2();	

    // Let (x, y) be the position on the screen of the placemark..
    double x1, y1;
    toScreenCoordinate(lon, lat, lambda0, phi0, phi1, phi2, n, F, rho0, x1, y1);
    x = width * 0.5 + rad2Pixel * (x1);
    y = height * 0.5 - rad2Pixel * (y1);

    return isLatValid && ( ( 0 <= y && y < height ) && ( ( 0 <= x && x < width )
                  || ( 0 <= x - 4 * radius && x - 4 * radius < width )
                  || ( 0 <= x + 4 * radius && x + 4 * radius < width ) ) );
}


bool LambertConformalConicProjection::screenCoordinates(const qreal lon, const qreal _lat,
														const ViewportParams *viewport,
														qreal& x, qreal& y ) const
{
    const bool isLatValid = minLat() <= _lat && _lat <= maxLat();
    qreal lat = _lat;
    if ( lat > maxLat() ) lat = maxLat();
    if ( lat < minLat() ) lat = minLat();
    
    // Convenience variables
    const int radius = viewport->radius();
    const qreal width  = qreal(viewport->width());
    const qreal height = qreal(viewport->height());
    const qreal rad2Pixel = radius / M_PI_2;
		
    // Let (x, y) be the position on the screen of the placemark..
    const double lambda0 = viewport->centerLongitude();
    const double phi0 = viewport->centerLatitude();
	const double phi1 = standardParallel1();
	const double phi2 = standardParallel2();
	double x1, y1;
    toScreenCoordinate(lon, lat, lambda0, phi0, phi1, phi2, x1, y1);
    x = width *0.5 + rad2Pixel * x1;
    y = height * 0.5 - rad2Pixel * y1;
    return isLatValid && ( ( 0 <= y && y < height ) && ( ( 0 <= x && x < width )
							  || ( 0 <= x - 4 * radius && x - 4 * radius < width )
							  || ( 0 <= x + 4 * radius && x + 4 * radius < width ) ) );
}


bool LambertConformalConicProjection::
screenCoordinates( const GeoDataCoordinates &geopoint, 
				  const ViewportParams *viewport,
				  qreal &x, qreal &y, bool &globeHidesPoint ) const
{
    globeHidesPoint = false;
    qreal lon, lat;
    geopoint.geoCoordinates( lon, lat );

    const bool isLatValid = minLat() <= lat && lat <= maxLat();
    if ( lat > maxLat() ) {
        GeoDataCoordinates approxCoords( geopoint );
        approxCoords.setLatitude( maxLat() );
        approxCoords.geoCoordinates( lon, lat );
    }
    if ( lat < minLat() ) {
        GeoDataCoordinates approxCoords( geopoint );
        approxCoords.setLatitude( minLat() );
        approxCoords.geoCoordinates( lon, lat );
    }

	const bool r = screenCoordinates(lon, lat, viewport, x, y );
	return isLatValid && r;
}

bool LambertConformalConicProjection::
screenCoordinates( const GeoDataCoordinates &coordinates,
				  const ViewportParams *viewport,
				  qreal *x, qreal &y, int &pointRepeatNum,
				  const QSizeF& size,
				  bool &globeHidesPoint ) const
{
	qDebug()<<"An unimplemented function called: screenCoordinates(). "<<__FILE__ << __LINE__<<"\n";
    // On flat projections the observer's view onto the point won't be 
    // obscured by the target planet itself.
    globeHidesPoint = false;
    qreal lon, lat;
    coordinates.geoCoordinates( lon, lat );
    const bool isLatValid= minLat() <= lat && lat <= maxLat();

    if ( lat > maxLat() ) {
        GeoDataCoordinates approxCoords( coordinates );
        approxCoords.setLatitude( maxLat() );
        approxCoords.geoCoordinates( lon, lat );
    }
    if ( lat < minLat() ) {
        GeoDataCoordinates approxCoords( coordinates );
        approxCoords.setLatitude( minLat() );
        approxCoords.geoCoordinates( lon, lat );
    }

    // Convenience variables
    int  radius = viewport->radius();
    qreal  width  = (qreal)(viewport->width());
    qreal  height = (qreal)(viewport->height());

    qreal  rad2Pixel = 2.0 * radius / M_PI;

    const qreal centerLon = viewport->centerLongitude();
    const qreal centerLat = viewport->centerLatitude();

    // Let (itX, y) be the first guess for one possible position on screen..
    qreal itX = ( width  / 2.0 + rad2Pixel * ( lon - centerLon ) );
    y = ( height / 2.0 - rad2Pixel * ( atanh( sin( lat ) ) - atanh( sin( centerLat ) ) ) );

    // Make sure that the requested point is within the visible y range:
    if ( 0 <= y + size.height() / 2.0 && y < height + size.height() / 2.0 ) {
        // First we deal with the case where the repetition doesn't happen
        if ( !repeatX() ) {
            *x = itX;
            if ( 0 < itX + size.width() / 2.0  && itX < width + size.width() / 2.0 ) {
                return isLatValid;
            }
            else {
                // the requested point is out of the visible x range:
                return false;
            }
        }
        // For the repetition case the same geopoint gets displayed on 
        // the map many times.across the longitude.

        int xRepeatDistance = 4 * radius;

        // Finding the leftmost positive x value
        if ( itX + size.width() / 2.0 >= xRepeatDistance ) {
            const int repeatNum = (int)( ( itX + size.width() / 2.0 ) / xRepeatDistance );
            itX = itX - repeatNum * xRepeatDistance;
        }
        if ( itX + size.width() / 2.0 < 0 ) {
            itX += xRepeatDistance;
        }
        // the requested point is out of the visible x range:
        if ( itX > width + size.width() / 2.0 ) {
            return false;
        }

        // Now iterate through all visible x screen coordinates for the point 
        // from left to right.
        int itNum = 0;
        while ( itX - size.width() / 2.0 < width ) {
            *x = itX;
            ++x;
            ++itNum;
            itX += xRepeatDistance;
        }

        pointRepeatNum = itNum;

        return isLatValid && true;
    }

    // the requested point is out of the visible y range:
    return false;
}


bool LambertConformalConicProjection::geoCoordinates
    ( 
	 const int x, const int y,
	 const ViewportParams *viewport,
	 const qreal n, const qreal F, const qreal rho0, 
	 qreal& lon, qreal& lat,
	 GeoDataCoordinates::Unit unit ) const
{
    // Convenience variables	
    const qreal width  = qreal(viewport->width());
    const qreal height = qreal(viewport->height());
    const int radius = viewport->radius();

    const double lambda0 = viewport->centerLongitude();
    const double phi0 = viewport->centerLatitude();
	const double phi1 = standardParallel1();
	const double phi2 = standardParallel2();
	
    // Calculate how many pixel are being represented per radians.
    const qreal pixel2Rad = M_PI_2 / qreal( radius );
	
    // Calculate translation of center point
    const double x1 = (x - width*0.5) * pixel2Rad;
    const double y1 = (height * 0.5 - y) * pixel2Rad;
    toGeoCoordinate(x1, y1, lambda0, phi0, phi1, phi2, lon, lat);
    if (unit == GeoDataCoordinates::Degree){
        lon *= 180/M_PI;
        lat *= 180/M_PI;
    }

	return true;
}


bool LambertConformalConicProjection::geoCoordinates
( 
 const int x, const int y,
 const ViewportParams *viewport,
 qreal& lon, qreal& lat,
 GeoDataCoordinates::Unit unit ) const
{
    // Convenience variables	
    const qreal width  = qreal(viewport->width());
    const qreal height = qreal(viewport->height());
    const int radius = viewport->radius();
	
    // Calculate how many pixel are being represented per radians.
    //const qreal rad2Pixel = qreal( 2 * radius )/M_PI;
    const qreal pixel2Rad = M_PI_2/qreal(radius );
	
    // Let (x, y) be the position on the screen of the placemark..
    const double lambda0 = viewport->centerLongitude();
    const double phi0 = viewport->centerLatitude();
	const double phi1 = standardParallel1();
	const double phi2 = standardParallel2();
    const double x1 = (x - width*0.5) * pixel2Rad;
    const double y1 = (height*0.5 - y) * pixel2Rad;
    toGeoCoordinate(x1, y1, lambda0, phi0, phi1, phi2, lon, lat);

	if (unit == GeoDataCoordinates::Degree){
		lon *= 180/M_PI;
		lat *= 180/M_PI;
	}
	return true;
}


GeoDataLatLonAltBox LambertConformalConicProjection::latLonAltBox( const QRect& screenRect,
                                                      const ViewportParams *viewport ) const
{
    {
    GeoDataLatLonAltBox latLonAltBox;
    latLonAltBox.setEast( +M_PI );
    latLonAltBox.setWest( -M_PI );
    latLonAltBox.setNorth( maxLat() );
    latLonAltBox.setSouth( minLat() );
    return latLonAltBox;
    }

	//qDebug()<<"An unimplemented function called:latLonAltBox(). "<<__FILE__ << __LINE__<<"\n";
	
    qreal west;
    qreal north = maxValidLat();
    geoCoordinates( screenRect.left(), screenRect.top(), viewport, west, north, GeoDataCoordinates::Radian );

    qreal east;
    qreal south = minValidLat();
    geoCoordinates( screenRect.right(), screenRect.bottom(), viewport, east, south, GeoDataCoordinates::Radian );

    // For the case where the whole viewport gets covered there is a
    // pretty dirty and generic detection algorithm:
    GeoDataLatLonAltBox latLonAltBox;
    latLonAltBox.setNorth( north, GeoDataCoordinates::Radian );
    latLonAltBox.setSouth( south, GeoDataCoordinates::Radian );
    latLonAltBox.setWest( west, GeoDataCoordinates::Radian );
    latLonAltBox.setEast( east, GeoDataCoordinates::Radian );
    latLonAltBox.setMinAltitude(      -100000000.0 );
    latLonAltBox.setMaxAltitude( 100000000000000.0 );
	
    // The remaining algorithm should be pretty generic for all kinds of 
    // flat projections:

    // If the whole globe is visible we can easily calculate
    // analytically the lon-/lat- range.
    // qreal pitch = GeoDataPoint::normalizeLat( viewport->planetAxis().pitch() );

    if ( repeatX() ) {
        int xRepeatDistance = 4 * viewport->radius();
        if ( viewport->width() >= xRepeatDistance ) {
            latLonAltBox.setWest( -M_PI );
            latLonAltBox.setEast( +M_PI );
        }
    }
    else {
        // We need a point on the screen at maxLat that definitely gets displayed:
        qreal averageLatitude = ( latLonAltBox.north() + latLonAltBox.south() ) / 2.0;
    
        GeoDataCoordinates maxLonPoint( +M_PI, averageLatitude, GeoDataCoordinates::Radian );
        GeoDataCoordinates minLonPoint( -M_PI, averageLatitude, GeoDataCoordinates::Radian );
    
        qreal dummyX, dummyY; // not needed
        bool dummyVal;
    
        if ( screenCoordinates( maxLonPoint, viewport, dummyX, dummyY, dummyVal ) ) {
            latLonAltBox.setEast( +M_PI );
        }
        if ( screenCoordinates( minLonPoint, viewport, dummyX, dummyY, dummyVal ) ) {
            latLonAltBox.setWest( -M_PI );
        }
    }

    return latLonAltBox;
}


bool LambertConformalConicProjection::mapCoversViewport( const ViewportParams *viewport ) const
{
	return false;
}
