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

#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <OgrePrerequisites.h>
#include <OgreSingleton.h>
#include <OgreString.h>
#include <OgreWindowEventUtilities.h>
#include <OgreFileSystemLayer.h>
#include <OgreArchiveManager.h>

#include "dependencies/OgreSdkUtil/SdkCameraMan.h"
#include "dependencies/OgreSdkUtil/SdkTrays.h"

#include "../Input/InputListener.hpp"
#include "../Util/Assert.hpp"

/**
 * Basic application
 * implements basic construction and deletion methods
 * override virtual methods to make it suitable for all platforms
 */
class CApplication :
    public Ogre::Singleton<CApplication>,
    public Ogre::WindowEventListener {
 protected:
  // Ogre
  // ----------------
  
  //! Root for Ogre
  Ogre::Root *mRoot;
  //! Overlay system of ogre
  Ogre::OverlaySystem *mOverlaySystem;
  //! File system abstaction layer
  Ogre::FileSystemLayer* mFSLayer;
  //! Render window
  Ogre::RenderWindow* mWindow;
  
  // OIS Input devices
  // -----------------
  
  //! OIS input manager
  OIS::InputManager* mInputManager;
  //! Input context
  OgreBites::InputContext mInputContext;

 public:
  CApplication();
  virtual ~CApplication();

  template<class T=CApplication>
  static T &getSingleton() {
    return dynamic_cast<T&>(*msSingleton);
  }

  template<class T=CApplication>
  static T *getSingletonPtr() {
    ASSERT(msSingleton);
    return dynamic_cast<T*>(msSingleton);
  }
  
  void go();

  // getter & setter
  // ---------------
  Ogre::Root *getRoot() {return mRoot;}
  Ogre::FileSystemLayer* getFileSystemLayer() {return mFSLayer;}
  Ogre::RenderWindow *getRenderWindow() {return mWindow;}
  
  OIS::Keyboard* getKeyboard() {return mInputContext.mKeyboard;}
  OIS::Mouse *getMouse() {return mInputContext.mMouse;}
  const OgreBites::InputContext &getInputContext() const {return mInputContext;}

  
  virtual void showLoadingBar(uint16_t numGroupsInit = 1,
                              uint16_t numGroupsLoad = 1) {}
  virtual void hideLoadingBar() {}

 protected:
  virtual void initApp();
  virtual void createRoot();
  virtual void createOverlaySystem();
  virtual bool oneTimeConfig();
  virtual void setup();
  virtual Ogre::RenderWindow *createWindow();
  virtual void setupInput(bool nograb);
  virtual void createInputSystem(bool nograb);
  virtual void createInputDevices();
  virtual void attachInputDevices();
  virtual void locateResources();
  virtual Ogre::String getConfigFilePath();
  virtual void loadResources();
  virtual void installWindowEventListener();

  virtual void createScene() = 0;
  virtual void destroyScene() = 0;

  virtual void closeApp();
  virtual void shutdown();
  virtual void shutdownInput();
  virtual void deleteInputDevices();
};


#endif /* APPLICATION_HPP */
