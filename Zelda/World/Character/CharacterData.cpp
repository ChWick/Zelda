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

#include "CharacterData.hpp"
#include "../../Common/Util/XMLHelper.hpp"

using XMLHelper::Attribute;
using XMLHelper::RealAttribute;
using XMLHelper::IntAttribute;
using XMLHelper::BoolAttribute;

SCharacterAnimationData::SCharacterAnimationData(
    const tinyxml2::XMLElement *e) {
  mName = Attribute(e, "name");
  mId = IntAttribute(e, "id");
  mLoop = BoolAttribute(e, "loop", true);
  mAllowMoving = BoolAttribute(e, "allow_moving", true);
}

SCharacterAnimationData::SCharacterAnimationData() {
}

SCharacterData::SCharacterData(const tinyxml2::XMLElement *e) {
  mAttitude = CHARACTER_ATTITUDE_ID_MAP.parseString(
      Attribute(e, "attitude"));

  uint8_t animCount = IntAttribute(e, "animations_count");
  mAnimations.resize(animCount);

  int readAnimations = 0;

  for (const tinyxml2::XMLElement *c = e->FirstChildElement();
       c;
       c = c->NextSiblingElement()) {
    if (strcmp(c->Value(), "animation") == 0) {
      SCharacterAnimationData data(c);
      mAnimations[data.mId] = data;
      ++readAnimations;
    }
  }

  // consistency check
  ASSERT(readAnimations == animCount);
  for (size_t i = 0; i < mAnimations.size(); ++i) {
    ASSERT(mAnimations[i].mName.size() > 0);
  }
}

SCharacterData::SCharacterData() {
}
