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

#ifndef _PAUSE_TYPES_HPP_
#define _PAUSE_TYPES_HPP_

#include "../Util/EnumIdMap.hpp"

typedef uint32_t PauseInt;

enum EPauseTypes {
  PAUSE_NONE              = 0,

  PAUSE_WORLD_UPDATE      = 1,
  PAUSE_WORLD_RENDER      = 2,

  PAUSE_ATLAS_UPDATE      = 4,
  PAUSE_ATLAS_RENDER      = 8,

  PAUSE_MAP_UPDATE        = 16,
  PAUSE_MAP_RENDER        = 32,

  PAUSE_PLAYER_UPDATE     = 64,
  PAUSE_PLAYER_INPUT      = 128,

  PAUSE_ALL_ATLAS_UPDATE  = PAUSE_ATLAS_UPDATE | PAUSE_MAP_UPDATE,
  PAUSE_ALL_ATLAS_RENDER  = PAUSE_ATLAS_RENDER | PAUSE_MAP_RENDER,
  PAUSE_ALL_ATLAS         = PAUSE_ALL_ATLAS_UPDATE | PAUSE_ALL_ATLAS_RENDER,

  PAUSE_ALL_WORLD_UPDATE  = PAUSE_WORLD_UPDATE | PAUSE_ALL_ATLAS_UPDATE,
  PAUSE_ALL_WORLD_RENDER  = PAUSE_WORLD_RENDER | PAUSE_ALL_ATLAS_RENDER,
  PAUSE_ALL_WORLD         = PAUSE_ALL_WORLD_RENDER | PAUSE_ALL_WORLD_UPDATE,

  PAUSE_ALL               = 4095,
};

class CPauseTypesIdMap : public CStringEnumIdMap<CPauseTypesIdMap, EPauseTypes> {
public:
  void init();
};

#endif // _PAUSE_TYPES_HPP_
