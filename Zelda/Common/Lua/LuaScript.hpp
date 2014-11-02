#ifndef _LUA_SCRIPT_HPP_
#define _LUA_SCRIPT_HPP_

#include <OgreResourceManager.h>
extern "C"{
  #include <lua.h>
}
#include <thread>
#include <mutex>

class CLuaScript;

typedef Ogre::SharedPtr<CLuaScript> CLuaScriptPtr;

class CLuaScript : public Ogre::Resource
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
