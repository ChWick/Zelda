#include "LuaScriptBridge.hpp"
#include <lua.hpp>
#include "../Log.hpp"

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

  return 1;
}

int entity(lua_State *l) {
  return 1;
}
