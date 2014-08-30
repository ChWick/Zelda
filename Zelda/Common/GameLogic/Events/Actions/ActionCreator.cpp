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
#include "ActionTypes.hpp"
#include "../../../Util/XMLHelper.hpp"
#include <OgreException.h>

#include "ActionMessage.hpp"
#include "ActionCreateObject.hpp"
#include "ActionDeleteObject.hpp"

using namespace XMLHelper;

namespace events {
  CAction *createAction(const tinyxml2::XMLElement *pElem, const CEvent &owner) {
    EActionTypes type(ACTION_TYPES_MAP.parseString(Attribute(pElem, "type")));

    switch (type) {
    case ACTION_MESSAGE:
      return new CActionMessage(pElem, owner);
    case ACTION_CREATE_OBJECT:
      return new CActionCreateObject(pElem, owner);
    case ACTION_DELETE_OBJECT:
      return new CActionDeleteObject(pElem, owner);
    }

    throw Ogre::Exception(0, "New action type not added in createAction", __FILE__);
  }
};
