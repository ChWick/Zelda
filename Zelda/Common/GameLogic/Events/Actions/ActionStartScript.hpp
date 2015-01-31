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

#ifndef _ACTION_START_SCRIPT_HPP_
#define _ACTION_START_SCRIPT_HPP_


#include "Action.hpp"
#include "../../../Lua/LuaScript.hpp"

class CMessage;

namespace events {
class CActionStartScriptConstructionInfo;

class CActionStartScript : public CAction {
 protected:
  CMessage *m_pMessage;
  CLuaScriptPtr mScript;
 public:
  CActionStartScript(const tinyxml2::XMLElement *pElem, const CEvent &owner);
  CActionStartScript(
      const std::shared_ptr<CActionStartScriptConstructionInfo> info,
      const CEvent &owner);
  
  ~CActionStartScript();
  
 protected:
  void start();
};

}  // namespace events

#endif // _ACTION_START_SCRIPT_HPP_
