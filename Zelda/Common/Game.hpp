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


#include "ShaderGenerator.hpp"
#include "Message/MessageInjector.hpp"
#include <OgreWindowEventUtilities.h>

// application prototypes
#include "Application/Application.hpp"
#include "Application/AndroidApplication.hpp"

class CGameStateManager;

class CGame :
#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
    public CAndroidApplication,
#else
    public CApplication,
#endif
    public CInputListener,
    public Ogre::FrameListener,
    protected OgreBites::SdkTrayListener,
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

  std::vector<std::string> &getAdditionalLevelDirPaths() {return m_vAdditionalLevelDirPaths;}

  OgreBites::ParamsPanel* getDetailsPanel() {return mDetailsPanel;}
  Ogre::SceneManager *getSceneManager() const {return mSceneMgr;}
  OgreBites::SdkTrayManager *getTrayMgr() {return mTrayMgr;}
  Ogre::Viewport *getMainViewPort() {return m_pMainViewPort;}

  void requestShutDown() { mShutDown = true; }

  void showLoadingBar(uint16_t numGroupsInit = 1, uint16_t numGroupsLoad = 1);
  void hideLoadingBar();

  bool renderOneFrame();

protected:
  Ogre::Camera* mCamera;
  Ogre::SceneManager* mSceneMgr;
  Ogre::String mResourcesCfg;
  Ogre::String mPluginsCfg;


  // OgreBites
  OgreBites::SdkTrayManager* mTrayMgr;
  OgreBites::SdkCameraMan* mCameraMan;      // basic camera controller
  OgreBites::ParamsPanel* mDetailsPanel;    // sample details panel
  bool mCursorWasVisible;                   // was cursor visible before dialog appeared
  bool mShutDown;

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
  void sendMessageToAll(const CMessagePtr message);

  virtual void createScene();
  virtual void destroyScene();
private:
#ifdef INCLUDE_RTSHADER_SYSTEM
  void initialiseShaderSystem();
  bool initialiseRTShaderSystem(Ogre::SceneManager* sceneMgr);
  void destroyRTShaderSystem();
#endif


protected:
  // virtual function to setup user game
  virtual void initSingletons();
  virtual void deleteSingletons();
  virtual void initEnumIdMaps();
  virtual void chooseGameState() {}
  virtual void postGUIManagerInitialised() {}
};

#endif // GAME_HPP
