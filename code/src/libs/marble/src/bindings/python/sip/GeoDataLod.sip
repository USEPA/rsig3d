// Copyright 2009 Simon Edwards <simon@simonzone.com>

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
class GeoDataLod : Marble::GeoDataObject
{
%TypeHeaderCode
#include <GeoDataLod.h>
%End

public:
                            GeoDataLod ();
                            GeoDataLod (const Marble::GeoDataLod& other);
    virtual ~GeoDataLod ();
    virtual const char*     nodeType () const;
    qreal                   minLodPixels () const;
    void                    setMinLodPixels (qreal pixels);
    qreal                   maxLodPixels () const;
    void                    setMaxLodPixels (qreal pixels);
    qreal                   minFadeExtent () const;
    void                    setMinFadeExtent (qreal pixels);
    qreal                   maxFadeExtent () const;
    void                    setMaxFadeExtent (qreal pixels);
    virtual void            pack (QDataStream& stream) const;
    virtual void            unpack (QDataStream& stream);
};
};
