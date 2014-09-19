#ifndef _LUA_SCRIPT_HPP_
#define _LUA_SCRIPT_HPP_

#include <OgreResourceManager.h>
#include <lua.hpp>

class CLuaScript;

typedef Ogre::SharedPtr<CLuaScript> CLuaScriptPtr;

class CLuaScript : public Ogre::Resource
{
private:
  lua_State *mLuaState;
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

private:
  void registerCFunctionsToLua();
  void registerSingleCFunctionsToLua(lua_CFunction fn, const char *label);
};


#endif // _LUA_SCRIPT_HPP_
