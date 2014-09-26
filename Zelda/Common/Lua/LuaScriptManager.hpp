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
