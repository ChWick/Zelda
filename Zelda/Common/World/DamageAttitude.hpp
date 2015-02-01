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

#ifndef DAMAGEATTITUDE_HPP
#define DAMAGEATTITUDE_HPP

#include "../Util/EnumIdMap.hpp"

//! Attitude of a hitable interface
enum EDamageAttitude {
  ATTITUDE_NONE     = 0,       //!< Something that wont be hit
  ATTITUDE_PLAYER   = 2 << 0,  //!< the player
  ATTITUDE_FRIENDLY = 2 << 1,  //!< Friendly
  ATTITUDE_ENEMY    = 2 << 2,  //!< a evil person, not the player...
  ATTITUDE_NEUTRAL  = 2 << 3,  //!< neutral elements (map, ...)

  ATTITUDE_MASK_FRIEND_OR_ENEMY = ATTITUDE_FRIENDLY | ATTITUDE_ENEMY,
  ATTITUDE_MASK_ALL = ATTITUDE_PLAYER | ATTITUDE_FRIENDLY | ATTITUDE_ENEMY | ATTITUDE_NEUTRAL,
  ATTITUDE_MASK_ENEMY_OR_NEUTRAL = ATTITUDE_ENEMY | ATTITUDE_NEUTRAL,
};

typedef int8_t DamageAttitudeMask;

class CDamageAttitudeIdMap :
    public CStringListEnumIdMap<CDamageAttitudeIdMap,
                                DamageAttitudeMask> {
 public:
  void init();
};

#endif  // DAMAGEATTITUDE_HPP
