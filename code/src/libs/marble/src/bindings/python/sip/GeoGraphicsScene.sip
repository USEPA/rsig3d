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
class GeoGraphicsScene : QObject
{
%TypeHeaderCode
#include <GeoGraphicsScene.h>
%End

public:
                            GeoGraphicsScene (QObject* parent /TransferThis/ = 0);
    ~GeoGraphicsScene ();
    void                    removeItem (Marble::GeoGraphicsItem* item);
    void                    clear ();
    QList<Marble::GeoGraphicsItem*>  items () const;
    QList<Marble::GeoGraphicsItem*>  items (const Marble::GeoDataLatLonAltBox& box, int maxZoomLevel) const;
    static int s_tileZoomLevel;
    void                    addItem (Marble::GeoGraphicsItem* item);
};
};
