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

#endif // _USER_LUA_SCRIPT_BRIDGE_HPP_
