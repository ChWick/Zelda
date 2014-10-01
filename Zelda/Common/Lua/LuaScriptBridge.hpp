#ifndef _LUA_SCRIPT_BRIDGE_HPP_

#include <lua.hpp>

// generate default variables and exit if script canceled
#define LUA_BRIDGE_START                                                                      \
  CLuaScriptPtr luaScript(CLuaScriptManager::getSingleton().getByLuaState(l));                \
  std::mutex &luaStateMutex(luaScript->getLuaStateMutex());                                   \
  {                                                                                           \
    std::lock_guard<std::mutex> lock(luaStateMutex);                                          \
    if (luaScript->getLuaState() == nullptr) {return -1;}                                     \
  }

#define LUA_WAIT(t)                                                                           \
  msleep(t);                                                                                  \
  {                                                                                           \
    std::lock_guard<std::mutex> lock(luaStateMutex);                                          \
    if (luaScript->getLuaState() == nullptr) {return -1;}                                     \
  }


//! function to register the c functions to the given lua state
void registerCFunctionsToLua(lua_State *l);
//! function to register a single function with a label to the lua state
void registerSingleCFunctionsToLua(lua_State *l, lua_CFunction fn, const char *label);

//! c function call by lua to write log messages
int log(lua_State *l);

//! c function call by lua to access messages
int message(lua_State *l);

#endif // _LUA_SCRIPT_BRIDGE_HPP_
