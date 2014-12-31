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

#ifndef _LUA_SCRIPT_HPP_
#define _LUA_SCRIPT_HPP_

#include <OgreResourceManager.h>
extern "C"{
  #include <lua.h>
}
#include <thread>
#include <mutex>
#include "../PauseManager/PauseCaller.hpp"

class CLuaScript;

typedef Ogre::SharedPtr<CLuaScript> CLuaScriptPtr;

class CLuaScript : public Ogre::Resource, public CPauseCaller
{
private:
  std::thread mThread;
  mutable std::mutex mLuaStateMutex;
  lua_State *mLuaState;
  bool mStarted;
protected:

  // must implement these from the Ogre::Resource interface
  void loadImpl();
  void unloadImpl();
  size_t calculateSize() const;

public:

  CLuaScript(Ogre::ResourceManager *creator, const Ogre::String &name,
     Ogre::ResourceHandle handle, const Ogre::String &group, bool isManual = false,
     Ogre::ManualResourceLoader *loader = 0);

  virtual ~CLuaScript();

  virtual CLuaScriptPtr clone(const Ogre::String& newName, bool changeGroup = false, const Ogre::String& newGroup = Ogre::StringUtil::BLANK);

  void start();
  std::mutex &getLuaStateMutex() {return mLuaStateMutex;}
  lua_State *getLuaState() {return mLuaState;}

  void setStarted(bool b) {std::lock_guard<std::mutex> l(mLuaStateMutex); mStarted = b;}
  bool isStarted() const {std::lock_guard<std::mutex> l(mLuaStateMutex); return mStarted;}

private:
};


#endif // _LUA_SCRIPT_HPP_
