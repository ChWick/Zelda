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

#ifndef _LUA_SCRIPT_MANAGER_HPP_
#define _LUA_SCRIPT_MANAGER_HPP_

 #include <OgreResourceManager.h>
 #include "LuaScript.hpp"

class CLuaScriptManager : public Ogre::ResourceManager, public Ogre::Singleton<CLuaScriptManager>
{
protected:

   // must implement this from ResourceManager's interface
   Ogre::Resource *createImpl(const Ogre::String &name, Ogre::ResourceHandle handle,
       const Ogre::String &group, bool isManual, Ogre::ManualResourceLoader *loader,
       const Ogre::NameValuePairList *createParams);

public:

   CLuaScriptManager();
   virtual ~CLuaScriptManager();

   virtual CLuaScriptPtr load(const Ogre::String &name, const Ogre::String &group);
   CLuaScriptPtr getByLuaState(lua_State *state);

   static CLuaScriptManager &getSingleton();
   static CLuaScriptManager *getSingletonPtr();
};

#endif // _LUA_SCRIPT_MANAGER_HPP_
