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
#include "StereographicProjection.h"
#include "MarbleDebug.h"

// Marble
#include "ViewportParams.h"
#include "MathHelper.h"
#include "GeoDataPoint.h"
#include "GeoDataLineString.h"
#include "GeoDataCoordinates.h"
#include "MarbleGlobal.h"


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


//Stereographic Projection
//http://mathworld.wolfram.com/StereographicProjection.html

inline void toScreenCoordinate(const double lambda,  //longitude, input
                               const double phi,     //latitude, input
                               const double lambda0, //reference longitude
                               const double phi1,    //reference latitudes
                               double& x, double &y) //output screen x, y (unit not known yet)
{
    const double cos_phi1 = cos(phi1);
    const double sin_phi1 = sin(phi1);

    const double R = 1.0;
    const double t = lambda - lambda0;
    const double cos_t = cos(t);
    const double cos_phi = cos(phi);
    const double sin_phi = sin(phi);

    double a = (1.0+ sin_phi1*sin_phi + cos_phi1*cos_phi*cos_t);
    const double zero = 1e-10;
    if (fabs(a)< zero){
        if (a>0.0) a=+zero;
        else a=-zero;
    }
    const double k = 2.0*R/a;

    x = k*cos_phi*sin(t);
    y = k*(cos_phi1*sin_phi - sin_phi1*cos_phi*cos_t);
}

//the inverse transform is from:
//http://mathworld.wolfram.com/LambertConformalConicProjection.html
inline void toGeoCoordinate(const double x,       //screen x, input
                            const double y,       //screen y, input
                            const double lambda0, //reference longitude
                            const double phi1,    //reference latitude
                            double& lambda, double &phi)   //output <lon,lat>
{
    const double cos_phi1 = cos(phi1);
    const double sin_phi1 = sin(phi1);

    const double zero = 1e-10;
    const double R = 1.0;
    double rho = sqrt(x*x+y*y);
    if (rho<zero) rho=zero;
    const double c = 2.0*atan(rho / (2.0 * R));

    const double cos_c = cos(c);
    const double sin_c = sin(c);

    lambda = lambda0 + atan2(x*sin_c,  (rho*cos_phi1*cos_c - y *sin_phi1*sin_c)); //atan2 has range [-PI, PI]
    phi = asin(cos_c*sin_phi1 + y*sin_c*cos_phi1/rho);   //range is [-PI/2, PI/2]
}


//=====================================================================================
using namespace Marble;

StereographicProjection::StereographicProjection()
    : AbstractProjection()
    ,m_referenceLatitude(36.50 * DEG2RAD)
{
    setRepeatX( false );
	setMinLat( -90.0 * DEG2RAD );
	setMaxLat( +90.0 * DEG2RAD );
}

StereographicProjection::~StereographicProjection()
{
}


bool StereographicProjection::repeatableX() const
{
    return false;
}

qreal StereographicProjection::maxValidLat() const
{
    return +90.0 * DEG2RAD;
}

qreal StereographicProjection::minValidLat() const
{
    return -90.0 * DEG2RAD;
}

bool StereographicProjection::screenCoordinates(const qreal lon, const qreal _lat,
                                                const ViewportParams *viewport,
                                                qreal& x, qreal& y ) const
{
	const qreal minlat = minLat();
	const qreal maxlat = maxLat();
    const bool isLatValid = (minlat <= _lat) && (_lat <= maxlat);
    qreal lat = _lat;
    if ( lat > maxlat ) lat = maxlat;
    else if ( lat < minlat ) lat = minlat;
    
    // Convenience variables
    const qreal radius = (qreal)(viewport->radius());
    const qreal width  = qreal(viewport->width());
    const qreal height = qreal(viewport->height());
    const qreal rad2Pixel = radius / M_PI_2;

    // EMVL HACK - Fix center lat/lon so that stereographic projection
    // always displays CMAQ HEMI data in a sensible way.
    //const double lambda0 = viewport->centerLongitude();
    //const double phi1 = viewport->centerLatitude();
    const double lambda0 = -98.0 * DEG2RAD;
    const double phi1 = 90.0 * DEG2RAD;
    
    double x1, y1;
    toScreenCoordinate(lon, lat, lambda0, phi1, x1, y1);
    x = width *0.5 + rad2Pixel * x1;
    y = height * 0.5 - rad2Pixel * y1;
    return isLatValid && ( ( 0 <= y && y < height ) && ( ( 0 <= x && x < width )
            || ( 0 <= x - 4 * radius && x - 4 * radius < width )
            || ( 0 <= x + 4 * radius && x + 4 * radius < width ) ) );
}


bool StereographicProjection::screenCoordinates( const GeoDataCoordinates &geopoint,
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


QPainterPath StereographicProjection::mapShape( const ViewportParams *viewport ) const
{
    //int  radius    = viewport->radius();
    int  imgWidth  = viewport->width();
    int  imgHeight = viewport->height();

    QPainterPath fullRect;
    fullRect.addRect(  0 , 0 , imgWidth, imgHeight );
    return fullRect;
}


bool StereographicProjection::screenCoordinates( const GeoDataCoordinates &geopoint,
                                             const ViewportParams *viewport,
                                             qreal *x, qreal &y,
                                             int &pointRepeatNum,
                                             const QSizeF& size,
                                             bool &globeHidesPoint ) const
{

    const bool r = screenCoordinates(geopoint, viewport, *x, y, globeHidesPoint );

    // This projection doesn't have any repetitions,
    // so the number of screen points referring to the geopoint is one.
    pointRepeatNum = 1;
    globeHidesPoint = false;
    return r;
}


bool StereographicProjection::screenCoordinates( const GeoDataLineString &lineString,
                                                  const ViewportParams *viewport,
                                                  QVector<QPolygonF *> &polygons ) const
{
	qDebug()<<"Not implemented function: StereographicProjection::screenCoordinates()\n";
    // Compare bounding box size of the line string with the angularResolution
    // Immediately return if the latLonAltBox is smaller.
    if ( !viewport->resolves( lineString.latLonAltBox() ) ) {
        return false;
    }

    QVector<GeoDataLineString*> lineStrings;

    if (
         ( !traversablePoles() && lineString.latLonAltBox().containsPole( AnyPole ) ) ||
         ( lineString.latLonAltBox().crossesDateLine() )
       ) {
        // We correct for Poles and DateLines:
        lineStrings = lineString.toRangeCorrected();

        foreach ( GeoDataLineString * itLineString, lineStrings ) {
            QVector<QPolygonF *> subPolygons;
            //d->lineStringToPolygon( *itLineString, viewport, subPolygons );
            polygons << subPolygons;
        }
    }
    else {
        //d->lineStringToPolygon( lineString, viewport, polygons );
    }

    return polygons.isEmpty();
}


bool StereographicProjection::geoCoordinates
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
    // EMVL HACK - Fix center lat/lon so that stereographic projection
    // always displays CMAQ HEMI data in a sensible way.
    //const double lambda0 = viewport->centerLongitude();
    //const double phi0 = viewport->centerLatitude();
    const double lambda0 = -98.0 * DEG2RAD;
    const double phi0 = 90.0 * DEG2RAD;
    
    const double x1 = (x - width*0.5) * pixel2Rad;
    const double y1 = (height*0.5 - y) * pixel2Rad;
    toGeoCoordinate(x1, y1, lambda0, phi0, lon, lat);

	if (unit == GeoDataCoordinates::Degree){
		lon *= 180/M_PI;
		lat *= 180/M_PI;
	}
	return true;
}


GeoDataLatLonAltBox StereographicProjection::latLonAltBox( const QRect& screenRect,
                                                      const ViewportParams *viewport ) const
{
    GeoDataLatLonAltBox latLonAltBox;
    latLonAltBox.setEast( +M_PI );
    latLonAltBox.setWest( -M_PI );
    latLonAltBox.setNorth( maxLat() );
    latLonAltBox.setSouth( minLat() );
    return latLonAltBox;
}


bool StereographicProjection::mapCoversViewport( const ViewportParams * ) const
{
	return false;
}
