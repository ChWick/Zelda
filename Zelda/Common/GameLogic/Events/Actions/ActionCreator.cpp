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

#include "ActionCreator.hpp"
#include "ActionConstructionInfo.hpp"
#include "ActionTypes.hpp"
#include "../../../Util/XMLHelper.hpp"
#include <OgreException.h>

#include "ActionMessage.hpp"
#include "ActionCreateObject.hpp"
#include "ActionDeleteObject.hpp"
#include "ActionStartScript.hpp"
#include "ActionStartScriptConstructionInfo.hpp"
#include "ActionCreateEffect.hpp"
#include "ActionCreateEffectConstructionInfo.hpp"

using XMLHelper::Attribute;

namespace events {

CAction *createAction(const tinyxml2::XMLElement *pElem, const CEvent &owner) {
  EActionTypes type(CActionTypesMap::getSingleton().parseString(
      Attribute(pElem, "type")));

  switch (type) {
    case ACTION_MESSAGE:
      return new CActionMessage(pElem, owner);
    case ACTION_CREATE_OBJECT:
      return new CActionCreateObject(pElem, owner);
    case ACTION_DELETE_OBJECT:
      return new CActionDeleteObject(pElem, owner);
    case ACTION_START_SCRIPT:
      return new CActionStartScript(pElem, owner);
  }

  throw Ogre::Exception(type,
                        "New action type not added in createAction",
                        __FILE__);
}

CAction *createAction(std::shared_ptr<CActionConstructionInfo> info,
                      const CEvent &owner) {
  switch (info->getType()) {
    case ACTION_CREATE_EFFECT:
      return new CActionCreateEffect(
          std::dynamic_pointer_cast<CActionCreateEffectConstructionInfo>(info),
          owner);
    case ACTION_START_SCRIPT:
      return new CActionStartScript(
          std::dynamic_pointer_cast<CActionStartScriptConstructionInfo>(info),
          owner);
  }

  throw Ogre::Exception(info->getType(),
                        "New action type not added in createAction",
                        __FILE__);
}

}  // namespace events
