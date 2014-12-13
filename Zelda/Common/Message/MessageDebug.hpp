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

#ifndef _MESSAGE_DEBUG_HPP_
#define _MESSAGE_DEBUG_HPP_

#include "Message.hpp"



class CMessageDebug: public CMessage {
public:
  enum EDebugMessageTypes {
    DM_TOGGLE_PHYSICS,
    DM_TOGGLE_DEBUG_DRAWER,
  };
protected:
  const EDebugMessageTypes m_eDebugType;
  bool m_bActive;
public:
  CMessageDebug(const EDebugMessageTypes type, bool bActive, const std::string &creationFile);

  EDebugMessageTypes getDebugType() const {return m_eDebugType;}

  void setActive(bool bActive) {m_bActive = bActive;}
  bool isActive() const {return m_bActive;}

};

#endif // _MESSAGE_TARGET_REACHED_HPP_
