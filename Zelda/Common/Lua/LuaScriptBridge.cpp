#include "LuaScriptBridge.hpp"
#include <lua.hpp>
#include "../Log.hpp"

int message(lua_State *l) {
  return 1;
}

int entity(lua_State *l) {
  return 1;
}
