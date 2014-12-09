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

#ifndef CHARACTERATTITUDE_HPP
#define CHARACTERATTITUDE_HPP

#include "../../Common/Util/EnumIdMap.hpp"

//! Attitude of a character
enum ECharacterAttitude {
  ATTITUDE_FRIENDLY,            //!< Friendly
  ATTITUDE_ENEMY,               //!< a evil person, not the player... ganons folks
};

class CCharacterAttitudeIdMap : public CStringEnumIdMap<ECharacterAttitude> {
 public:
  CCharacterAttitudeIdMap();
};

extern CCharacterAttitudeIdMap CHARACTER_ATTITUDE_ID_MAP;

#endif  // CHARACTERATTITUDE_HPP
