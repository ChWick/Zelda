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

#include "LuaScriptManager.hpp"
#include "../Util/Assert.hpp"

template<> CLuaScriptManager *Ogre::Singleton<CLuaScriptManager>::msSingleton = 0;

CLuaScriptManager *CLuaScriptManager::getSingletonPtr()
{
   return msSingleton;
}

CLuaScriptManager &CLuaScriptManager::getSingleton()
{
   assert(msSingleton);
   return(*msSingleton);
}

CLuaScriptManager::CLuaScriptManager()
{
   mResourceType = "LuaScript";

   // low, because it will likely reference other resources
   mLoadOrder = 30.0f;

   // this is how we register the ResourceManager with OGRE
   Ogre::ResourceGroupManager::getSingleton()._registerResourceManager(mResourceType, this);
}

CLuaScriptManager::~CLuaScriptManager()
{
   // and this is how we unregister it
   Ogre::ResourceGroupManager::getSingleton()._unregisterResourceManager(mResourceType);
}

CLuaScriptPtr CLuaScriptManager::load(const Ogre::String &name, const Ogre::String &group)
{
   CLuaScriptPtr ptr = getResourceByName(name, group).dynamicCast<CLuaScript>();

   if (ptr.isNull())
       ptr = createResource(name, group).dynamicCast<CLuaScript>();

   ptr->load();
   return ptr;
}

CLuaScriptPtr CLuaScriptManager::getByLuaState(lua_State *state) {
  OGRE_LOCK_AUTO_MUTEX;
  ASSERT(state);
  ResourceMapIterator it(getResourceIterator());
  while (it.hasMoreElements()) {
      CLuaScriptPtr ptr(it.getNext().dynamicCast<CLuaScript>());
    if (ptr->getLuaState() == state) {
      return ptr;
    }
  }

  throw Ogre::Exception(0, "Lua script not found.", __FILE__);
}

Ogre::Resource *CLuaScriptManager::createImpl(const Ogre::String &name, Ogre::ResourceHandle handle,
                                           const Ogre::String &group, bool isManual, Ogre::ManualResourceLoader *loader,
                                           const Ogre::NameValuePairList *createParams)
{
   return new CLuaScript(this, name, handle, group, isManual, loader);
}
