#include "LuaScript.hpp"
#include <lua.hpp>
#include "../Util/Assert.hpp"
#include "../Log.hpp"
#include "LuaScriptManager.hpp"

CLuaScript::CLuaScript(Ogre::ResourceManager* creator, const Ogre::String &name,
                 Ogre::ResourceHandle handle, const Ogre::String &group, bool isManual,
                 Ogre::ManualResourceLoader *loader)
  : Ogre::Resource(creator, name, handle, group, isManual, loader),
    mLuaState(nullptr)
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
}

void CLuaScript::unloadImpl() {
  /* If you were storing a pointer to an object, then you would check the pointer here,
  and if it is not NULL, you would destruct the object and set its pointer to NULL again.
  */
  if (mLuaState) {
    lua_close(mLuaState);
    mLuaState = nullptr;
  }
}

size_t CLuaScript::calculateSize() const {
  return sizeof(mLuaState); // correct would be to compute the total size of the action lua state
}

void CLuaScript::start() {
  ASSERT(mLuaState);
  lua_getglobal(mLuaState, "start");
  ASSERT(lua_gettop(mLuaState) == 0); // no arguments passed
  ASSERT(lua_isfunction(mLuaState, -1));
  if (lua_pcall(mLuaState, 0, 0, 0) != LUA_OK) {
    LOGW("Lua call of 'start' failed");
  }
}
