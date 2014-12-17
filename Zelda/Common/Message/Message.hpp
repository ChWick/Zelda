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

#ifndef _MESSAGE_HPP_
#define _MESSAGE_HPP_

#include <memory>
#include <string>
#include <OgreStringConverter.h>
#include "MessageTypes.hpp"

namespace tinyxml2 {
  class XMLElement;
};

class CMessage {
protected:
  const uint8_t mType;
  //! Store location where message was created
  const std::string mCreationFile;

public:
  CMessage(const std::string &creationFile, uint8_t type, const tinyxml2::XMLElement *pElement);
  CMessage(const std::string &creationFile, uint8_t type);
  virtual ~CMessage();

  uint8_t getType() const {return mType;}
  const std::string &getCreationFile() const {return mCreationFile;}
};

typedef std::shared_ptr<CMessage> CMessagePtr;
typedef std::shared_ptr<const CMessage> CMessageConstPtr;

#define __MSG_LOCATION__ std::string("File: ") + __FILE__ + std::string(", Line: ") + Ogre::StringConverter::toString(__LINE__)

#endif
