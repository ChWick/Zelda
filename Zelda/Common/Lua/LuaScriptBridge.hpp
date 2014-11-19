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

#ifndef _LUA_SCRIPT_BRIDGE_HPP_
#define _LUA_SCRIPT_BRIDGE_HPP_

extern "C"{
  #include <lua.h>
  #include <lualib.h>
  #include <lauxlib.h>
}

// generate default variables and exit if script canceled
#define LUA_BRIDGE_START(returnValues)                                           \
  int numberOfReturnValues = (returnValues);                                     \
  CLuaScriptPtr luaScript(CLuaScriptManager::getSingleton().getByLuaState(l));   \
  std::mutex &luaStateMutex(luaScript->getLuaStateMutex());                      \
  {                                                                                           \
    if (!luaScript->isStarted()) {return numberOfReturnValues;}                                     \
  }

#define LUA_WAIT(t)                                                                           \
  msleep(t);                                                                                  \
  {                                                                                           \
    if (!luaScript->isStarted()) {return numberOfReturnValues;}                                     \
  }


//! function to register the c functions to the given lua state
void registerCFunctionsToLua(lua_State *l);
//! function to register a single function with a label to the lua state
void registerSingleCFunctionsToLua(lua_State *l, lua_CFunction fn, const char *label);

//! c function call by lua to write log messages
int log(lua_State *l);

//! c function call by lua to access messages
int message(lua_State *l);

//! c function call by lua to write a int data to game memory
int writeIntToMemory(lua_State *l);

//! c function call by lua to read a int data to game memory
int readIntFromMemory(lua_State *l);

//! c function call by lua to write a Ogre::Real data to game memory
int writeRealToMemory(lua_State *l);

//! c function call by lua to read a Ogre::Real data to game memory
int readRealFromMemory(lua_State *l);

#endif // _LUA_SCRIPT_BRIDGE_HPP_
