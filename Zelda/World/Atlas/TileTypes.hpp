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
  TT_GREEN_SOIL = 0,
  TT_GREEN_SOIL_BUSH_SHADOW,
  TT_GREEN_SOIL_BUSH_LIFTED,
  TT_GREEN_SOIL_STONE_SHADOW,
  TT_GREEN_SOIL_STONE_LIFTED,
  TT_GREEN_SOIL_STONE_PILE_SHADOW,

  TT_COUNT,
};

struct ETileData {
  std::string sMeshName;
};

class CTileTypeIdMap : public CStringEnumIdMap<CTileTypeIdMap, ETileTypes> {
 public:
  void init();
};

class CTileDataMap : public CEnumIdMap<CTileDataMap, ETileTypes, ETileData> {
 public:
  void init();
};

#endif // TILE_TYPES_HPP
