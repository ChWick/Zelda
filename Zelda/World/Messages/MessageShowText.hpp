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

#ifndef _MESSAGE_SHOW_TEXT_HPP_
#define _MESSAGE_SHOW_TEXT_HPP_

#include "../../Common/Message/Message.hpp"


class CMessageShowText: public CMessage {
public:
protected:
  const std::string m_sLanguageString;
public:
  CMessageShowText(const std::string &sLanguageString);
  CMessageShowText(const tinyxml2::XMLElement *pElem);

  const std::string &getLanguageString() const {return m_sLanguageString;}
};

#endif // _MESSAGE_SHOW_TEXT_HPP_
