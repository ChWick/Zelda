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

#include "MessageCreator.hpp"
#include "MessageTypes.hpp"
#include "../Util/XMLHelper.hpp"
#include <OgreException.h>

#include "MessageSwitchMap.hpp"

using namespace XMLHelper;

CMessage *createMessage(const tinyxml2::XMLElement *pElem) {
  EMessageTypes type(MESSAGE_TYPES_MAP.parseString(Attribute(pElem, "message_type")));

  switch (type) {
  case MSG_SWITCH_MAP:
    return new CMessageSwitchMap(pElem);
  default:
    break;
  }

  throw Ogre::Exception(0, "New message type not added in createMessage", __FILE__);
}