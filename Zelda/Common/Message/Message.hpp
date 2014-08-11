/*****************************************************************************
 * Copyright 2014 Christoph Wick
 *
 * This file is part of Mencus.
 *
 * Mencus is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * Mencus is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * Mencus. If not, see http://www.gnu.org/licenses/.
 *****************************************************************************/

#ifndef _MESSAGE_HPP_
#define _MESSAGE_HPP_

#include "MessageTypes.hpp"

namespace tinyxml2 {
  class XMLElement;
};

class CMessage {
protected:
  const EMessageTypes m_Type;

public:
  CMessage(const tinyxml2::XMLElement *pElement);
  CMessage(EMessageTypes type);
};

#endif
