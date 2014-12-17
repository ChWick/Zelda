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

#ifndef _USER_MESSAGE_CREATOR_HPP_
#define _USER_MESSAGE_CREATOR_HPP_

#include "../../Common/Message/MessageCreator.hpp"

class CUserMessageCreator : public CMessageCreator {
public:
  virtual CMessagePtr createMessage(const std::string &creationFile,
                                    const tinyxml2::XMLElement *pElem,
                                    const Ogre::Any &any = Ogre::Any());
};

#endif // _USER_MESSAGE_CREATOR_HPP_
