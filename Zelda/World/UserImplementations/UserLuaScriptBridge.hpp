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

#ifndef _USER_LUA_SCRIPT_BRIDGE_HPP_
#define _USER_LUA_SCRIPT_BRIDGE_HPP_

#include "../../Common/Lua/LuaScriptBridge.hpp"

void userRegisterCFunctionsToLua(lua_State *l);

//! c function call by lua to access entities
int entity(lua_State *l);

//! c function call by lua to create a text message
int textMessage(lua_State *l);

//! c function call by lua to move an entity to a given position
int moveTo(lua_State *l);

//! c function call by lua to delete an entity
int deleteEntity(lua_State *l);

//! c function call to check if the player has a item
int hasItem(lua_State *l);

//! c function to set the inner object of an object or chest
int setInnerObject(lua_State *l);

//! c function to play an animation of an entity
int playAnimation(lua_State *l);

//! c function to wait for animation stopped
int waitForAnimationHasStopped(lua_State *l);

#endif // _USER_LUA_SCRIPT_BRIDGE_HPP_
