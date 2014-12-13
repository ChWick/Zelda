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

#ifndef _MESSAGE_CREATOR_HPP_
#define _MESSAGE_CREATOR_HPP_

#include <OgreSingleton.h>
#include <OgreAny.h>
#include "Message.hpp"

namespace tinyxml2 {
  class XMLElement;
};

class CMessageCreator : public Ogre::Singleton<CMessageCreator> {
protected:
  const std::string m_sMessageTypeAttributeName;
public:
  CMessageCreator();
  virtual ~CMessageCreator();

  static CMessageCreator &getSingleton();
  static CMessageCreator *getSingletonPtr();


  virtual CMessagePtr createMessage(const tinyxml2::XMLElement *pElem, const std::string &creationFile, const Ogre::Any &any = Ogre::Any());
};


#endif // _MESSAGE_CREATOR_HPP_

