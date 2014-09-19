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

#ifndef GAME_HPP
#define GAME_HPP

#define DEBUG_SHOW_OGRE_TRAY

#include <OgrePlatform.h>
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE && !defined(__LP64__)
#  include <Carbon/Carbon.h>
#endif
#include <OgrePrerequisites.h>
#include <OgreSingleton.h>
#include "Input/InputListener.hpp"
#include "dependencies/OgreSdkUtil/SdkCameraMan.h"
#include "dependencies/OgreSdkUtil/SdkTrays.h"
#include "OgreFileSystemLayer.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
#include <android_native_app_glue.h>
#include "Android/OgreAPKFileSystemArchive.h"
#include "Android/OgreAPKZipArchive.h"
#include <OgreArchiveManager.h>
#endif

#include "ShaderGenerator.hpp"
#include "Message/MessageInjector.hpp"
#include <OgreWindowEventUtilities.h>

class CGameStateManager;

class CGame : public CInputListener,
              public Ogre::FrameListener,
              public Ogre::WindowEventListener,
              protected OgreBites::SdkTrayListener,
              public Ogre::Singleton<CGame>,
              public CMessageInjector {
protected:
  CGameStateManager *m_pGameStateManager;
  Ogre::Viewport *m_pMainViewPort;

private:
  bool m_bDebugDrawerEnabled;
  std::vector<std::string> m_vAdditionalLevelDirPaths;

#ifdef INCLUDE_RTSHADER_SYSTEM
  Ogre::RTShader::ShaderGenerator*	    mShaderGenerator;	  //!< The Shader generator instance.
  ShaderGeneratorTechniqueResolverListener* mMaterialMgrListener; //!< Shader generator material manager listener.
#endif // INCLUDE_RTSHADER_SYSTEM
public:
  CGame(void);
  virtual ~CGame(void);

  static CGame &getSingleton();
  static CGame *getSingletonPtr();

  std::vector<std::string> &getAdditionalLevelDirPaths() {return m_vAdditionalLevelDirPaths;}

  OIS::Keyboard* getKeyboard() {return mInputContext.mKeyboard;}
  OIS::Mouse *getMouse() {return mInputContext.mMouse;}
  const OgreBites::InputContext &getInputContext() const {return mInputContext;}

  OgreBites::ParamsPanel* getDetailsPanel() {return mDetailsPanel;}
  Ogre::FileSystemLayer* getFileSystemLayer() {return mFSLayer;}
  Ogre::Root *getRoot() {return mRoot;}
  Ogre::RenderWindow *getRenderWindow() {return mWindow;}
  Ogre::SceneManager *getSceneManager() const {return mSceneMgr;}
  OgreBites::SdkTrayManager *getTrayMgr() {return mTrayMgr;}
  Ogre::Viewport *getMainViewPort() {return m_pMainViewPort;}

  void go();
  void initApp();
  void closeApp();

  void requestShutDown() { mShutDown = true; }

  void createResources();
  void destroyResources();

  OgreBites::SdkTrayManager* showLoadingBar(unsigned int numGroupsInit = 1, unsigned int numGroupsLoad = 1);
  void hideLoadingBar();

  bool renderOneFrame();

#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
  void initAppForAndroid(Ogre::RenderWindow *window, struct android_app* app, OIS::MultiTouch *mouse, OIS::Keyboard *keyboard) {
    Ogre::LogManager::getSingletonPtr()->logMessage("Initializing App for Android");
    assert(mouse);
    assert(keyboard);
    mApp = app;
    mWindow = window;
    mInputContext.mMultiTouch = mouse;
    mInputContext.mKeyboard = keyboard;

    if(app != NULL) {
      mNativeActivity = app->activity;
      mAssetMgr = app->activity->assetManager;
      Ogre::ArchiveManager::getSingleton().addArchiveFactory(
        new Ogre::APKFileSystemArchiveFactory(app->activity->assetManager));
      Ogre::ArchiveManager::getSingleton().addArchiveFactory(
	new Ogre::APKZipArchiveFactory(app->activity->assetManager));
    }
  }
#endif

protected:
#if OGRE_VERSION >= ((1 << 16) | (9 << 8) | 0)
  Ogre::OverlaySystem *mOverlaySystem;
#endif
  Ogre::Root *mRoot;
  Ogre::Camera* mCamera;
  Ogre::SceneManager* mSceneMgr;
  Ogre::RenderWindow* mWindow;
  Ogre::String mResourcesCfg;
  Ogre::String mPluginsCfg;


  // OgreBites
  OgreBites::SdkTrayManager* mTrayMgr;
  OgreBites::SdkCameraMan* mCameraMan;      // basic camera controller
  OgreBites::ParamsPanel* mDetailsPanel;    // sample details panel
  bool mCursorWasVisible;                   // was cursor visible before dialog appeared
  bool mShutDown;

  // OIS Input devices
  OIS::InputManager* mInputManager;
  OgreBites::InputContext mInputContext;

  // Ogre::FrameListener
  virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
  virtual bool frameStarted(const Ogre::FrameEvent& evt);
  virtual bool frameEnded(const Ogre::FrameEvent& evt);

  // OIS::KeyListener
  virtual bool keyPressed( const OIS::KeyEvent &arg );
  virtual bool keyReleased( const OIS::KeyEvent &arg );

  // OIS::MouseListener
  virtual bool mouseMoved( const OIS::MouseEvent &arg );
  virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
  virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

  // OIS::MultitouchListener
  virtual bool touchMoved(const OIS::MultiTouchEvent& evt);
  virtual bool touchPressed(const OIS::MultiTouchEvent& evt);
  virtual bool touchReleased(const OIS::MultiTouchEvent& evt);
  virtual bool touchCancelled(const OIS::MultiTouchEvent& evt);

  // Ogre::WindowEventListener
  virtual void windowResized(Ogre::RenderWindow* rw);
  virtual void windowClosed(Ogre::RenderWindow* rw);

  // MessageInjector
  void sendMessageToAll(const CMessage &message);
private:
  void createRoot();
  void setup();
  void setupInput(bool nograb = false);
  void createInputDevices();
  void shutdown();
  void shutdownInput();
  void locateResources();
  void loadResources();
  Ogre::RenderWindow *createWindow();
  void createScene();
  void destroyScene();

  bool oneTimeConfig();
private:
  Ogre::FileSystemLayer* mFSLayer; // File system abstraction layer
#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
  Ogre::DataStreamPtr openAPKFile(const Ogre::String& fileName) {
    Ogre::DataStreamPtr stream;
    AAsset* asset = AAssetManager_open(mAssetMgr, fileName.c_str(), AASSET_MODE_BUFFER);
    if(asset) {
      off_t length = AAsset_getLength(asset);
      void* membuf = OGRE_MALLOC(length, Ogre::MEMCATEGORY_GENERAL);
      memcpy(membuf, AAsset_getBuffer(asset), length);
      AAsset_close(asset);

      stream = Ogre::DataStreamPtr(new Ogre::MemoryDataStream(membuf, length, true, true));
    }
    return stream;
  }
  struct android_app* mApp;
  AAssetManager* mAssetMgr;       // Android asset manager to access files inside apk
  ANativeActivity* mNativeActivity;
#endif
#ifdef INCLUDE_RTSHADER_SYSTEM
  bool initialiseRTShaderSystem(Ogre::SceneManager* sceneMgr);
  void destroyRTShaderSystem();
#endif


protected:
  // virtual function to setup user game
  virtual void initSingletons() {}
  virtual void initEnumIdMaps();
  virtual void chooseGameState() {}
  virtual void postGUIManagerInitialised() {}
};

#endif // GAME_HPP
