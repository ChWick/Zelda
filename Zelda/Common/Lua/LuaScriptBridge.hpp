#ifndef _LUA_SCRIPT_BRIDGE_HPP_
#define _LUA_SCRIPT_BRIDGE_HPP_

extern "C"{
  #include <lua.h>
  #include <lualib.h>
  #include <lauxlib.h>
}

// generate default variables and exit if script canceled
#define LUA_BRIDGE_START                                                                      \
  CLuaScriptPtr luaScript(CLuaScriptManager::getSingleton().getByLuaState(l));                \
  std::mutex &luaStateMutex(luaScript->getLuaStateMutex());                                   \
  {                                                                                           \
    if (!luaScript->isStarted()) {return -1;}                                     \
  }

#define LUA_WAIT(t)                                                                           \
  msleep(t);                                                                                  \
  {                                                                                           \
    if (!luaScript->isStarted()) {return -1;}                                     \
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
