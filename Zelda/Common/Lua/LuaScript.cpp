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

#include "LuaScript.hpp"
#include "../Util/Assert.hpp"
#include "../Log.hpp"
#include "LuaScriptManager.hpp"
#include "../Config/TypeDefines.hpp"
#include LUA_SCRIPT_BRIDGE_HEADER

CLuaScript::CLuaScript(Ogre::ResourceManager* creator, const Ogre::String &name,
                 Ogre::ResourceHandle handle, const Ogre::String &group, bool isManual,
                 Ogre::ManualResourceLoader *loader)
  : Ogre::Resource(creator, name, handle, group, isManual, loader),
    mLuaState(nullptr),
    mStarted(false)
{
  /* If you were storing a pointer to an object, then you would set that pointer to NULL here.
  */

  /* For consistency with StringInterface, but we don't add any parameters here
  That's because the Resource implementation of StringInterface is to
  list all the options that need to be set before loading, of which
  we have none as such. Full details can be set through scripts.
  */
  createParamDictionary("LuaScript");
}

CLuaScript::~CLuaScript() {
  unload();
  if (mThread.joinable()) {
    mThread.join();
  }
}

CLuaScriptPtr CLuaScript::clone(const Ogre::String& newName, bool changeGroup, const Ogre::String& newGroup) {
  CLuaScriptPtr clone;
  if (changeGroup)
  {
    clone = CLuaScriptManager::getSingleton().createResource(newName, newGroup).dynamicCast<CLuaScript>();
  }
  else
  {
    clone = CLuaScriptManager::getSingleton().createResource(newName, mGroup).dynamicCast<CLuaScript>();
  }

  // Keep handle (see below, copy overrides everything)
  Ogre::ResourceHandle newHandle = clone->getHandle();

  // HERE COMES PARAMETER CLONE IF NECESSARY

  // Restore new group if required, will have been overridden by operator
  if (changeGroup)
  {
    clone->mGroup = newGroup;
  }

  // Correct the name & handle, they get copied too
  clone->mName = newName;
  clone->mHandle = newHandle;

  return clone;
}

void CLuaScript::loadImpl() {
  Ogre::DataStreamPtr stream = Ogre::ResourceGroupManager::getSingleton().openResource(mName, mGroup, false, this);
  Ogre::String script = stream->getAsString();

  mLuaState = luaL_newstate();
  luaL_openlibs(mLuaState);
  int status = luaL_loadstring(mLuaState, script.c_str());
  if (status == LUA_OK) {
    lua_pcall(mLuaState, 0, LUA_MULTRET, 0);
  }
  else {
    throw Ogre::Exception(status, "Error while loading lua script '" + script + "' from string.", __FILE__);
  }

  // register c functions
  LUA_SCRIPT_BRIDGE_REGISTER_FUNCTION(mLuaState);

  mStarted = false;
}

void CLuaScript::unloadImpl() {
  /* If you were storing a pointer to an object, then you would check the pointer here,
  and if it is not NULL, you would destruct the object and set its pointer to NULL again.
  */
  mLuaStateMutex.lock();
  if (mLuaState) {
    lua_State *buffer = mLuaState;
    if (mStarted) {
      mStarted = false;
      mLuaStateMutex.unlock();
      mThread.join(); // w8 for finished
    }
    else {
      mLuaStateMutex.unlock();
    }
    lua_close(buffer);
  }
  else {
    mLuaStateMutex.unlock();
  }
  mLuaState = nullptr;  // at end set state to zero, since script will run to end and check maybe if script exists
}

size_t CLuaScript::calculateSize() const {
  return sizeof(mLuaState); // correct would be to compute the total size of the action lua state
}

void startLuaScriptThread(lua_State *pLuaState, CLuaScript *script) {
  std::mutex &luaStateMutex(script->getLuaStateMutex());
  script->setStarted(true);
  luaStateMutex.lock();
  lua_getglobal(pLuaState, "start");
  //ASSERT(lua_gettop(mLuaState) == 0); // no arguments passed
  ASSERT(lua_isfunction(pLuaState, -1));
  luaStateMutex.unlock();

  try {
    if (lua_pcall(pLuaState, 0, 0, 0) != LUA_OK) {
      LOGW("Lua call of 'start' failed");
    }
  }
  catch (...) {
    LOGW("Exception during lua script.");
  }

  script->setStarted(false);
}

void CLuaScript::start() {
  // be sure that the script is loaded!
  ASSERT(isLoaded());
  // if there is an asserten, check if the script is loaded from the correct
  // resource group, since it may happen that it find an script in the old map
  // if used AUTODETECT_RESOURCE_GROUP with identical script names
  

  std::lock_guard<std::mutex> lock(mLuaStateMutex);

  if (mStarted) {
    LOGW("Lua script already running");
    return;
  }
  ASSERT(mLuaState);
  if (mThread.joinable()) {
    mThread.join(); // old has to be finished.
  }
  mThread = std::thread(startLuaScriptThread, mLuaState, this);
  //startLuaScriptThread(mLuaState, this);
}
