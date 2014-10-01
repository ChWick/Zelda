#include "LuaScriptBridge.hpp"
#include "LuaScriptManager.hpp"
#include <lua.hpp>
#include "../Log.hpp"
#include "../Message/MessageCreator.hpp"
#include "../Message/MessageHandler.hpp"
#include "../tinyxml2/tinyxml2.hpp"
#include "../Util/Sleep.hpp"
#include <memory>

using namespace tinyxml2;

void registerCFunctionsToLua(lua_State *l) {
  registerSingleCFunctionsToLua(l, log, "log");
  registerSingleCFunctionsToLua(l, message, "message");
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
