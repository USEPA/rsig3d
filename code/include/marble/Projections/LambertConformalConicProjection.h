//
// This file is part of the Marble Virtual Globe.
//
// This program is free software licensed under the GNU LGPL. You can
// find a copy of this license in LICENSE.txt in the top directory of
// the source code.
//
// Copyright 2007-2012 Nan Zhang  <zhang.nan@epa.gov>
//


#ifndef MARBLE_LAMBERTCONFORMALCONICRPROJECTION_H
#define MARBLE_LAMBERTCONFORMALCONICRPROJECTION_H


/** @file
 * This file contains the headers for LambertConformalConicProjection.
 *
 */


#include "ConicalProjection.h"

namespace Marble
{

class LambertConformalConicProjectionPrivate;

/**
 * @short A class to implement the Mercator projection.
 */

class LambertConformalConicProjection : public ConicalProjection
{
 public:

    /**
     * @brief Construct a new LambertConformalConicProjection.
     */
    LambertConformalConicProjection();

    virtual ~LambertConformalConicProjection();
	
    void get_n_F_rho0(const double phi0, double &n, double &F, double& rho0) const;
		
    virtual qreal standardParallel1() const;
    virtual qreal standardParallel2() const;
    virtual void setStandardParallel1(qreal x);
    virtual void setStandardParallel2(qreal x);
		
    virtual PreservationType preservationType() const { return Conformal; }

    /**
     * @brief Get the screen coordinates corresponding to geographical coordinates in the map.
     * @param lon    the lon coordinate of the requested pixel position
     * @param lat    the lat coordinate of the requested pixel position
     * @param x      the x coordinate of the pixel is returned through this parameter
     * @param y      the y coordinate of the pixel is returned through this parameter
     * @return @c true  if the geographical coordinates are visible on the screen
     *         @c false if the geographical coordinates are not visible on the screen
     */
    bool screenCoordinates( const qreal lon, const qreal lat,
						   const ViewportParams *params, 
                           const qreal n, const qreal F, const qreal rho0,
						   qreal& x, qreal& y ) const;

    bool screenCoordinates( const qreal lon, const qreal lat,
						   const ViewportParams *params,
						   qreal& x, qreal& y ) const;
	
    bool screenCoordinates( const GeoDataCoordinates &coordinates, 
                            const ViewportParams *params,
                            qreal &x, qreal &y, bool &globeHidesPoint ) const;

    bool screenCoordinates( const GeoDataCoordinates &coordinates,
                            const ViewportParams * viewport,
                            qreal *x, qreal &y, int &pointRepeatNum,
                            const QSizeF& size,
                            bool &globeHidesPoint ) const;

    using ConicalProjection::screenCoordinates;

   /**
     * @brief Get the earth coordinates corresponding to a pixel in the map.
     *
     * If the pixel (x, y) is outside the globe, only @p lon will be calculated,
     * and lat will be unchanged.
     *
     * @param x      the x coordinate of the pixel
     * @param y      the y coordinate of the pixel
     * @param lon    the longitude angle is returned through this parameter
     * @param lat    the latitude angle is returned through this parameter
     * @return @c true  if the pixel (x, y) is within the globe
     *         @c false if the pixel (x, y) is outside the globe, i.e. in space.
     */
    bool geoCoordinates( const int x, const int y,
						const ViewportParams *params,
                        const qreal n, const qreal F, const qreal rho0,
						qreal& lon, qreal& lat,
						GeoDataCoordinates::Unit = GeoDataCoordinates::Degree ) const;

    bool geoCoordinates( const int x, const int y,
						const ViewportParams *params,
						qreal& lon, qreal& lat,
						GeoDataCoordinates::Unit = GeoDataCoordinates::Degree ) const;
	
    GeoDataLatLonAltBox latLonAltBox( const QRect &screenRect,
                                      const ViewportParams *viewport ) const;

    bool  mapCoversViewport( const ViewportParams *viewport ) const;

 private:
    Q_DISABLE_COPY( LambertConformalConicProjection )
	qreal m_standardParalle1;
	qreal m_standardParalle2;
};

}

#endif //MARBLE_LAMBERTCONFORMALCONICRPROJECTION_H
