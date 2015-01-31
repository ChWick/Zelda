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

#include "ActionCreateEffectConstructionInfo.hpp"
#include <OgreException.h>
#include <string>
#include "../../../tinyxml2/tinyxml2.hpp"
#include "../../../Effects/EffectConstructionInfo.hpp"

using tinyxml2::XMLElement;

namespace events {

CActionCreateEffectConstructionInfo::CActionCreateEffectConstructionInfo(
    const tinyxml2::XMLElement *e)
    : CActionConstructionInfo(ACTION_CREATE_EFFECT),
      mAttachedParentData(e) {
  for (const XMLElement *c = e->FirstChildElement(); c;
       c = c->NextSiblingElement()) {
    if (strcmp(c->Value(), "effect") == 0) {
      mEffectConstructionInfos.push_back(
          std::make_shared<CEffectConstructionInfo>());
      mEffectConstructionInfos.back()->parse(c);
    } else {
      throw Ogre::Exception(0,
                            "'" + std::string(c->Value()) + "' is not allowed"
                            "as child element of a CActionCreateEffect node",
                            __FILE__);
    }
  }
}

}  // namespace events
