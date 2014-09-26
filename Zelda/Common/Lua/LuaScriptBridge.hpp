#ifndef _LUA_SCRIPT_BRIDGE_HPP_

struct lua_State;

//! c function call by lua to write log messages
int log(lua_State *l);

//! c function call by lua to access messages
int message(lua_State *l);

//! c function call by lua to access entities
int entity(lua_State *l);

//! c function call by lua to create a text message
int textMessage(lua_State *l);

#endif // _LUA_SCRIPT_BRIDGE_HPP_
