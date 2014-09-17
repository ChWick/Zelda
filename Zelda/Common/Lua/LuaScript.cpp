#include "LuaScript.hpp"
#include <lua.hpp>

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

// farm out to CLuaScriptSerializer
void CLuaScript::loadImpl() {
  Ogre::DataStreamPtr stream = Ogre::ResourceGroupManager::getSingleton().openResource(mName, mGroup, true, this);
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
