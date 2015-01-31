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

#include "ActionConstructionInfo.hpp"
#include <OgreException.h>
#include "../../../Util/XMLHelper.hpp"

#include "ActionCreateObjectConstructionInfo.hpp"
#include "ActionCreateEffectConstructionInfo.hpp"
#include "ActionStartScriptConstructionInfo.hpp"


using XMLHelper::Attribute;

namespace events {

CActionConstructionInfo::CActionConstructionInfo(const EActionTypes t)
    : mType(t) {
}

std::shared_ptr<CActionConstructionInfo>
CActionConstructionInfo::createNew(const tinyxml2::XMLElement *e) {
  EActionTypes type(CActionTypesMap::getSingleton().parseString(
      Attribute(e, "type")));

  switch (type) {
    case ACTION_CREATE_OBJECT:
      return std::make_shared<CActionCreateObjectConstructionInfo>(e);
    case ACTION_CREATE_EFFECT:
      return std::make_shared<CActionCreateEffectConstructionInfo>(e);
    case ACTION_START_SCRIPT:
      return std::make_shared<CActionStartScriptConstructionInfo>(e);
    default:
      throw Ogre::Exception(type,
                            "ActionConstructionInfo type not implemented",
                            __FILE__);
  }
}


}  // namespace events
