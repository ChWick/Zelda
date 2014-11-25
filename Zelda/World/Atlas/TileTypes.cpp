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
  m_Map[TT_GREEN_SOIL_BUSH_SHADOW] = "green_soil_bush_shadow";
  m_Map[TT_GREEN_SOIL_STONE_SHADOW] = "green_soil_stone_shadow";
  m_Map[TT_GREEN_SOIL_STONE_PILE_SHADOW] = "green_soil_stone_pile_shadow";
  m_Map[TT_GREEN_SOIL] = "green_soil";
  m_Map[TT_GREEN_SOIL_GRASS_BL_BR] = "green_soil_grass_bl_br";
  m_Map[TT_GREEN_SOIL_GRASS_TL_TR] = "green_soil_grass_tl_tr";
  m_Map[TT_GREEN_SOIL_GRASS_BL_BR_TL_TR] = "green_soil_grass_bl_br_tl_tr";
  m_Map[TT_GREEN_SOIL_GRASS_BR_BL_TR] = "green_soil_grass_br_bl_tr";
  m_Map[TT_GREEN_SOIL_GRASS_BR_TL_TR] = "green_soil_grass_br_tl_tr";

  m_Map[TT_GREEN_SOIL_STONE_LIFTED] = "green_soil_stone_lifted";

  m_Map[TT_COUNT] = "none";
}

CTileDataMap::CTileDataMap() {
  m_Map[TT_GREEN_SOIL_BUSH_SHADOW] = {"soil_green_bush.mesh"};
  m_Map[TT_GREEN_SOIL_STONE_SHADOW] = {"soil_green_stone.mesh"};
  m_Map[TT_GREEN_SOIL_STONE_PILE_SHADOW] = {"soil_green_stone_pile_shadow.mesh"};
  
  m_Map[TT_GREEN_SOIL] = {"soil_green.mesh"};
  m_Map[TT_GREEN_SOIL_GRASS_BL_BR] = {"soil_green_bush_bl_br.mesh"};
  m_Map[TT_GREEN_SOIL_GRASS_TL_TR] = {"soil_green_bush_tl_tr.mesh"};
  m_Map[TT_GREEN_SOIL_GRASS_BL_BR_TL_TR] = {"soil_green_bush_bl_br_tl_tr.mesh"};
  m_Map[TT_GREEN_SOIL_GRASS_BR_BL_TR] = {"soil_green_bush_br_bl_tr.mesh"};
  m_Map[TT_GREEN_SOIL_GRASS_BR_TL_TR] = {"soil_green_bush_br_tl_tr.mesh"};
  
  m_Map[TT_GREEN_BUSH_TRUNK] = {"green_bush_trunk.mesh"};
  m_Map[TT_GREEN_SOIL_STONE_LIFTED] = {"green_bush_trunk.mesh"};
}

