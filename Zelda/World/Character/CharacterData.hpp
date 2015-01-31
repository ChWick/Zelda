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

#ifndef CHARACTERDATA_HPP
#define CHARACTERDATA_HPP

#include "../../Common/World/WorldEntityConstructionInfo.hpp"
#include "CharacterAttitude.hpp"

namespace tinyxml2 {
class XMLElement;
}

//! character animation data
struct SCharacterAnimationData {
  //! Name of the animation
  std::string mName;
  //! id of the animation
  uint8_t mId;
  //! loop the animation
  bool mLoop;
  //! may the character move during the animation
  bool mAllowMoving;

  explicit SCharacterAnimationData(const tinyxml2::XMLElement *e);
  SCharacterAnimationData();

  SCharacterAnimationData &operator=(const SCharacterAnimationData &) = default;
};

typedef std::vector<SCharacterAnimationData> CharacterAnimationDataList;

#endif /* CHARACTERDATA_HPP */
