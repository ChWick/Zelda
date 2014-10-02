#include "LuaScriptBridge.hpp"
#include "LuaScriptManager.hpp"
#include "../Log.hpp"
#include "../Message/MessageCreator.hpp"
#include "../Message/MessageHandler.hpp"
#include "../tinyxml2/tinyxml2.hpp"
#include "../Util/Sleep.hpp"
#include "../Util/Assert.hpp"
#include <memory>
#include "../Util/GameMemory.hpp"

using namespace tinyxml2;

void registerCFunctionsToLua(lua_State *l) {
  registerSingleCFunctionsToLua(l, log, "log");
  registerSingleCFunctionsToLua(l, message, "message");

  // memory access
  registerSingleCFunctionsToLua(l, writeIntToMemory, "writeIntToMemory");
  registerSingleCFunctionsToLua(l, readIntFromMemory, "readIntFromMemory");
  registerSingleCFunctionsToLua(l, writeRealToMemory, "writeRealToMemory");
  registerSingleCFunctionsToLua(l, readRealFromMemory, "readRealFromMemory");
}

void registerSingleCFunctionsToLua(lua_State *l, lua_CFunction fn, const char *label) {
  lua_pushcfunction(l, fn);
  lua_setglobal(l, label);
}

int log(lua_State *l) {
  LUA_BRIDGE_START;

  LOGV("Lua call: log");

  if (lua_gettop(l) != 1) {
    LOGW("Wrong argument count for log call");
    return -1;
  }

  LOGI("Lua log: %s", lua_tostring(l, 1));

  return 1;
}

int message(lua_State *l) {
  LUA_BRIDGE_START;

  LOGV("Lua call: message");

  if (lua_gettop(l) != 1) {
    LOGW("Wrong argument count for log call");
    return -1;
  }

  XMLDocument doc;
  doc.Parse(lua_tostring(l, 1));

  CMessageHandler::getSingleton().addMessage(CMessageCreator::getSingleton().createMessage(doc.FirstChildElement()));

  while (true) {
    LUA_WAIT(10);
  }

  return 1;
}

int writeIntToMemory(lua_State *l) {
  LUA_BRIDGE_START;
  ASSERT(lua_gettop(l) == 2);

  std::string id = lua_tostring(l, 1);
  int value = lua_tointeger(l, 2);

  CGameMemory::getSingleton().setIntData(id, value);

  return 0;
}

int readIntFromMemory(lua_State *l) {
  LUA_BRIDGE_START;
  ASSERT(lua_gettop(l) >= 1);

  std::string id = lua_tostring(l, 1);
  int defaultValue = 0;
  if (lua_gettop(l) == 2) {
    defaultValue = lua_tointeger(l, 2);
  }

  lua_pushinteger(l,CGameMemory::getSingleton().getIntData(id, defaultValue));

  return 1;
}

int writeRealToMemory(lua_State *l) {
  LUA_BRIDGE_START;
  ASSERT(lua_gettop(l) == 2);

  std::string id = lua_tostring(l, 1);
  Ogre::Real value = lua_tonumber(l, 2);

  CGameMemory::getSingleton().setRealData(id, value);

  return 0;
}

int readRealFromMemory(lua_State *l) {
  LUA_BRIDGE_START;
  ASSERT(lua_gettop(l) >= 1);

  std::string id = lua_tostring(l, 1);
  Ogre::Real defaultValue = 0;
  if (lua_gettop(l) == 2) {
    defaultValue = lua_tonumber(l, 2);
  }

  lua_pushnumber(l,CGameMemory::getSingleton().getRealData(id, defaultValue));

  return 1;
}
