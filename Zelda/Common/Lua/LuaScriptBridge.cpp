#include "LuaScriptBridge.hpp"
#include <lua.hpp>
#include "../Log.hpp"
#include "../Message/MessageCreator.hpp"
#include "../tinyxml2/tinyxml2.hpp"
#include "../Message/MessageHandler.hpp"

using namespace tinyxml2;

int log(lua_State *l) {
  LOGV("Lua call: log");

  if (lua_gettop(l) != 1) {
    LOGW("Wrong argument count for log call");
    return -1;
  }

  LOGI("Lua log: %s", lua_tostring(l, 1));

  return 1;
}

int message(lua_State *l) {
  LOGV("Lua call: message");

  if (lua_gettop(l) != 1) {
    LOGW("Wrong argument count for log call");
    return -1;
  }

  XMLDocument doc;
  doc.Parse(lua_tostring(l, 1));

  CMessageHandler::getSingleton().addMessage(CMessageCreator::getSingleton().createMessage(doc.FirstChildElement()));

  return 1;
}

int entity(lua_State *l) {
  return 1;
}
