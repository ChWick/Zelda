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

#ifndef TILE_TYPES_HPP
#define TILE_TYPES_HPP

#include "../../Common/Util/EnumIdMap.hpp"

enum ETileTypes {
  TT_GREEN_SOIL_BUSH_SHADOW = 0,
  TT_GREEN_SOIL_STONE_SHADOW,
  TT_GREEN_SOIL_STONE_PILE_SHADOW,

  TT_GREEN_SOIL,
  TT_GREEN_SOIL_GRASS_BL_BR,
  TT_GREEN_SOIL_GRASS_TL_TR,
  TT_GREEN_SOIL_GRASS_BL_BR_TL_TR,
  TT_GREEN_SOIL_GRASS_BR_BL_TR,
  TT_GREEN_SOIL_GRASS_BR_TL_TR,

  TT_GREEN_BUSH_TRUNK,

  TT_COUNT,
};

struct ETileData {
  std::string sMeshName;
};

class CTileTypeIdMap : public CEnumIdMap<ETileTypes, ETileData> {
public:
  CTileTypeIdMap();
};

extern CTileTypeIdMap TILE_TYPE_ID_MAP;

#endif // TILE_TYPES_HPP
