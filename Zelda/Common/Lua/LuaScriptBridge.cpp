#include "LuaScriptBridge.hpp"
#include "LuaScriptManager.hpp"
#include <lua.hpp>
#include "../Log.hpp"
#include "../Message/MessageCreator.hpp"
#include "../tinyxml2/tinyxml2.hpp"
#include "../Message/MessageHandler.hpp"
#include "../Util/Sleep.hpp"
#include "../../GUIComponents/GUITextBox.hpp"
#include <memory>

using namespace tinyxml2;

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

int entity(lua_State *l) {
  LUA_BRIDGE_START;

  return 1;
}

int textMessage(lua_State *l) {
  LUA_BRIDGE_START;

  LOGV("Lua call: textMessage");

  if (lua_gettop(l) != 1) {
    LOGW("Wrong argument count for textMessage call");
    return -1;
  }

  std::shared_ptr<CGUITextBox::SResult> result(new CGUITextBox::SResult);
  result->mResult = CGUITextBox::RESULT_NONE;

  XMLDocument doc;
  doc.Parse(lua_tostring(l, 1));

  CMessageHandler::getSingleton().addMessage(CMessageCreator::getSingleton().createMessage(doc.FirstChildElement(), Ogre::Any(result)));

  while (true) {
    LUA_WAIT(50);
    result->mMutex.lock();
    if (result->mResult != CGUITextBox::RESULT_NONE) {
      break;
    }
    result->mMutex.unlock();
  }
  // still locked
  lua_pushnumber(l, result->mResult);
  result->mMutex.unlock();
  return 1; // 1 return value
}
