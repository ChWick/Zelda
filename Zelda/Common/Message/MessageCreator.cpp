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
#include "../Util/Assert.hpp"
#include "../Config/TypeDefines.hpp"
#include MESSAGE_USER_TYPES_HEADER

#include "MessageSwitchMap.hpp"

using namespace XMLHelper;

template <> CMessageCreator* Ogre::Singleton<CMessageCreator>::msSingleton = 0;

CMessageCreator &CMessageCreator::getSingleton() {
  ASSERT(msSingleton);
  return *msSingleton;
}
CMessageCreator *CMessageCreator::getSingletonPtr() {
  return msSingleton;
}

CMessageCreator::CMessageCreator()
  : m_sMessageTypeAttributeName("message_type") {
}

CMessageCreator::~CMessageCreator() {
}

CMessagePtr CMessageCreator::createMessage(const std::string &creationFile,
                                         const tinyxml2::XMLElement *pElem,
                                         const Ogre::Any &any) {
  uint8_t type(MESSAGE_USER_TYPES::getSingleton().
               parseString(Attribute(pElem,
                                     m_sMessageTypeAttributeName.c_str())));

  switch (type) {
  case MSG_SWITCH_MAP:
    return std::make_shared<CMessageSwitchMap>(creationFile, pElem);
  default:
    break;
  }

  throw Ogre::Exception(0, "New message type not added in createMessage", __FILE__);
}
