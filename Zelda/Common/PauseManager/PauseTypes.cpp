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

#include "PauseTypes.hpp"

void CPauseTypesIdMap::init() {
  m_Map[PAUSE_NONE] = "none";
  m_Map[PAUSE_WORLD_UPDATE] = "world_update";
  m_Map[PAUSE_WORLD_RENDER] = "world_render";
  m_Map[PAUSE_ATLAS_UPDATE] = "atlas_update";
  m_Map[PAUSE_ATLAS_RENDER] = "atlas_render";
  m_Map[PAUSE_MAP_UPDATE] = "map_update";
  m_Map[PAUSE_MAP_RENDER] = "map_render";
  m_Map[PAUSE_PLAYER_UPDATE] = "player_update";
  m_Map[PAUSE_PLAYER_INPUT] = "player_input";

  m_Map[PAUSE_ALL_ATLAS_UPDATE] = "all_atlas_update";
  m_Map[PAUSE_ALL_ATLAS_RENDER] = "all_atlas_render";
  m_Map[PAUSE_ALL_ATLAS] = "all_atlas";
  m_Map[PAUSE_ALL_WORLD_UPDATE] = "all_world_update";
  m_Map[PAUSE_ALL_WORLD_RENDER] = "all_world_render";
  m_Map[PAUSE_ALL_WORLD] = "all_world";
  m_Map[PAUSE_ALL] = "all";
}