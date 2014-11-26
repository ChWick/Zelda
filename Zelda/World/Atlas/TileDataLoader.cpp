/*****************************************************************************
 * Copyright 2014 Christoph Wick
 *
 * This file is part of Zelda.
 *
 * Zelda is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * Zelda is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * Zelda. If not, see http://www.gnu.org/licenses/.
 *****************************************************************************/

#include "TileDataLoader.hpp"
#include <string>
#include "../../Common/Util/XMLHelper.hpp"
#include "TileTypes.hpp"
#include <OgreStringConverter.h>

using XMLHelper::Attribute;
using XMLHelper::IntAttribute;
using XMLHelper::RealAttribute;

void CTileDataLoader::readGroupElement(const tinyxml2::XMLElement *e) {
  ETileData data;
  const std::string id = Attribute(e, "id");
  data.sMeshName = Attribute(e, "mesh_name");

  TILE_DATA_MAP.setData(TILE_TYPE_ID_MAP.parseString(id), data);
}
