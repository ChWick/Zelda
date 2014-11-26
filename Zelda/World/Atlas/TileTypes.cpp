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

#include "TileTypes.hpp"

CTileTypeIdMap TILE_TYPE_ID_MAP;
CTileDataMap TILE_DATA_MAP;

CTileTypeIdMap::CTileTypeIdMap() {
  m_Map[TT_GREEN_SOIL] = "green_soil";
  m_Map[TT_GREEN_SOIL_BUSH_SHADOW] = "green_soil_bush_shadow";
  m_Map[TT_GREEN_SOIL_BUSH_LIFTED] = "green_soil_bush_lifted";
  m_Map[TT_GREEN_SOIL_STONE_SHADOW] = "green_soil_stone_shadow";
  m_Map[TT_GREEN_SOIL_STONE_LIFTED] = "green_soil_stone_lifted";
  m_Map[TT_GREEN_SOIL_STONE_PILE_SHADOW] = "green_soil_stone_pile_shadow";
  m_Map[TT_COUNT] = "none";
}

CTileDataMap::CTileDataMap() {
}

