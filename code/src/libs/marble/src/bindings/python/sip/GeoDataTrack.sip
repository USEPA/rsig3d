// Copyright 2011 Simon Edwards <simon@simonzone.com>

//                 Generated by twine2

// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Library General Public License as
// published by the Free Software Foundation; either version 2, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details

// You should have received a copy of the GNU Library General Public
// License along with this program; if not, write to the
// Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

namespace Marble
{
class GeoDataTrack : Marble::GeoDataGeometry
{
%TypeHeaderCode
#include <GeoDataTrack.h>
%End

public:
                            GeoDataTrack ();
                            GeoDataTrack (const Marble::GeoDataGeometry& other);
    int                     size () const;
    bool                    interpolate () const;
    void                    setInterpolate (bool on);
    QDateTime               firstWhen () const;
    QDateTime               lastWhen () const;
    QList<Marble::GeoDataCoordinates>  coordinatesList () const;
    QList<QDateTime>        whenList () const;
    Marble::GeoDataCoordinates  coordinatesAt (const QDateTime& when) const;
    Marble::GeoDataCoordinates  coordinatesAt (int index) const;
    void                    addPoint (const QDateTime& when, const Marble::GeoDataCoordinates& coord);
    void                    appendCoordinates (const Marble::GeoDataCoordinates& coord);
    void                    appendAltitude (qreal altitude);
    void                    appendWhen (const QDateTime& when);
    void                    clear ();
    void                    removeBefore (const QDateTime& when);
    void                    removeAfter (const QDateTime& when);
    const Marble::GeoDataLineString*  lineString () const;
//ig    GeoDataExtendedData&    extendedData () const;
//ig    void                    setExtendedData (const GeoDataExtendedData& extendedData);
    virtual const char*     nodeType () const;
    virtual Marble::EnumGeometryId  geometryId () const;
    virtual Marble::GeoDataLatLonAltBox  latLonAltBox () const;
    virtual void            pack (QDataStream& stream) const;
    virtual void            unpack (QDataStream& stream);
};
};
