// Copyright 2010 Jens-Michael Hoffmann <jmho@c-xx.com>
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library. If not, see <http://www.gnu.org/licenses/>.

#include "TextureTile.h"

#include <limits>

#include <QtCore/QString>

namespace Marble
{

TextureTile::TextureTile( TileId const & tileId, QImage const & image, const Blending * blending )
    : m_id( tileId ),
      m_blending( blending ),
      m_image( image )
{
    Q_ASSERT( !image.isNull() );
}

TextureTile::~TextureTile()
{
}

}
