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

#include "Application.hpp"
#include <string>
#include <OgreRoot.h>
#include <OgreSceneManager.h>
#include <OgreConfigFile.h>
#include "../Log.hpp"
#include "../Util/Assert.hpp"
#include "../Input/GameInputManager.hpp"
#include "../InputDefines.hpp"
#include "../Lua/LuaScriptManager.hpp"


template<> CApplication *Ogre::Singleton<CApplication>::msSingleton = 0;

CApplication::CApplication()
    : mRoot(nullptr),
      mOverlaySystem(nullptr) {
}

CApplication::~CApplication() {
}

void CApplication::go() {
  initApp();
  mRoot->startRendering();
  closeApp();
}

void CApplication::initApp() {
  mFSLayer = OGRE_NEW_T(Ogre::FileSystemLayer,
                        Ogre::MEMCATEGORY_GENERAL)(OGRE_VERSION_NAME);
  LOGI("*** Create root***");
  createRoot();
  LOGI("*** Finished root ***");

  LOGI("*** Creating OverlaySystem ***");
  createOverlaySystem();
  LOGI("*** Finished OverlaySystem***");

  LOGI("*** Requesting configuration***");
  if (oneTimeConfig()) {
    LOGV("User cancelled config dialog");
  }
  LOGI("*** Configuartion loaded***");

  LOGI("*** Setup ***");
  setup();
  LOGI("*** Setup finished ***");
}

void CApplication::createRoot() {
  ASSERT(!mRoot);

  Ogre::String pluginsPath = Ogre::StringUtil::BLANK;
#ifndef OGRE_STATIC_LIB
  pluginsPath = mFSLayer->getConfigFilePath("plugins.cfg");
#endif
  mRoot = OGRE_NEW Ogre::Root(pluginsPath,
                              mFSLayer->getWritablePath("ogre.cfg"),
                              mFSLayer->getWritablePath("ogre.log"));

#ifdef OGRE_STATIC_LIB
  mStaticPluginLoader.load();
#endif
}

void CApplication::createOverlaySystem() {
  ASSERT(!mOverlaySystem);
  mOverlaySystem = OGRE_NEW Ogre::OverlaySystem();
}

bool CApplication::oneTimeConfig() {
  if (!mRoot->restoreConfig()) {
    return mRoot->showConfigDialog();
  }
  return true;
}

void CApplication::setup() {
  LOGI("*** creating window ***");
  mWindow = createWindow();
  LOGI("*** Setting up input ***");
  setupInput(true);
  LOGI("*** locating resources ***");
  locateResources();
  LOGI("*** loading essential resources ***");
  Ogre::ResourceGroupManager::getSingleton()
      .initialiseResourceGroup("Essential");

  Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

  // adds context as listener to process context-level
  // (above the sample level) events
  installWindowEventListener();

  LOGI("*** creating scene ***");
  createScene();
  LOGI("*** setup finished ***");
}

Ogre::RenderWindow *CApplication::createWindow() {
  return mRoot->initialise(true);
}

void CApplication::setupInput(bool nograb) {
  createInputSystem(nograb);
  createInputDevices();
  attachInputDevices();
}

void CApplication::createInputSystem(bool nograb) {
  OIS::ParamList pl;
  size_t winHandle = 0;
  std::ostringstream winHandleStr;

  mWindow->getCustomAttribute("WINDOW", &winHandle);
  winHandleStr << winHandle;

  pl.insert(std::make_pair("WINDOW", winHandleStr.str()));
  if (nograb) {
    pl.insert(std::make_pair("x11_keyboard_grab", "false"));
    pl.insert(std::make_pair("x11_mouse_grab", "false"));
    pl.insert(std::make_pair("w32_mouse", "DISCL_FOREGROUND"));
    pl.insert(std::make_pair("w32_mouse", "DISCL_NONEXCLUSIVE"));
    pl.insert(std::make_pair("w32_keyboard", "DISCL_FOREGROUND"));
    pl.insert(std::make_pair("w32_keyboard", "DISCL_NONEXCLUSIVE"));
  }

  mInputManager = OIS::InputManager::createInputSystem(pl);
}

void CApplication::createInputDevices() {
  mInputContext.mKeyboard = static_cast<OIS::Keyboard*>(
      mInputManager->createInputObject(OIS::OISKeyboard, true));
  mInputContext.mMouse = static_cast<OIS::Mouse*>(
      mInputManager->createInputObject(OIS::OISMouse, true));

}

void CApplication::attachInputDevices() {
  // create game input manager
  CInputListenerManager *pInputManager = new CInputListenerManager(mWindow);
  new CGameInputManager();

  // attach input devices
  windowResized(mWindow);    // do an initial adjustment of mouse area


  if (mInputContext.mKeyboard)
    mInputContext.mKeyboard->setEventCallback(pInputManager);
  if (mInputContext.mMouse)
    mInputContext.mMouse->setEventCallback(pInputManager);
  if (mInputContext.mMultiTouch)
    mInputContext.mMultiTouch->setEventCallback(pInputManager);
}

void CApplication::locateResources() {
  auto &rgm(Ogre::ResourceGroupManager::getSingleton());

  // add custom resource manager
  new CLuaScriptManager();

  // this is first added, that we use these files first if existing
  /*for (const std::string &path : m_vAdditionalLevelDirPaths) {
    rgm.addResourceLocation(path, "FileSystem", "level_user");
    }*/

  // load resource paths from config file
  Ogre::ConfigFile cf;
  cf.load(getConfigFilePath());
  Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
  Ogre::String sec, type, arch;

  // go through all specified resource groups
  while (seci.hasMoreElements()) {
    sec = seci.peekNextKey();
    Ogre::ConfigFile::SettingsMultiMap* settings = seci.getNext();
    Ogre::ConfigFile::SettingsMultiMap::iterator i;

    // go through all resource paths
    for (i = settings->begin(); i != settings->end(); i++) {
      type = i->first;
      arch = i->second;

      rgm.addResourceLocation(arch, type, sec);
    }
  }
}

Ogre::String CApplication::getConfigFilePath() {
  return mFSLayer->getConfigFilePath("resources.cfg");
}

void CApplication::loadResources() {
  // only load required resources for the game menu (cegui resources)
  LOGI("Loading resources start");
  auto &rgm(Ogre::ResourceGroupManager::getSingleton());
  showLoadingBar(5, 0);
  rgm.initialiseResourceGroup("Imagesets");
  rgm.initialiseResourceGroup("Fonts");
  rgm.initialiseResourceGroup("Schemes");
  rgm.initialiseResourceGroup("LookNFeel");
  rgm.initialiseResourceGroup("Layouts");

  hideLoadingBar();
  LOGI("Loading resources end");
}

void CApplication::installWindowEventListener() {
  Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);
}

void CApplication::closeApp() {
  shutdown();
  mRoot->saveConfig();
  if (mRoot) {
    delete Ogre::ResourceGroupManager::getSingleton().
        _getResourceManager("LuaScript");
    OGRE_DELETE mOverlaySystem;
    OGRE_DELETE mRoot;
  }

  OGRE_DELETE_T(mFSLayer, FileSystemLayer, Ogre::MEMCATEGORY_GENERAL);

#ifdef OGRE_STATIC_LIB
    mStaticPluginLoader.unload();
#endif
}

void CApplication::shutdown() {
  if (mRoot->getRenderSystem() != NULL) destroyScene();

  Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
  windowClosed(mWindow);

  shutdownInput();
}

void CApplication::shutdownInput() {
  // detach input devices
  windowResized(mWindow);    // do an initial adjustment of mouse area
  if (mInputContext.mKeyboard)
    mInputContext.mKeyboard->setEventCallback(NULL);
  if (mInputContext.mMouse)
    mInputContext.mMouse->setEventCallback(NULL);
#if OIS_WITH_MULTITOUCH
  if (mInputContext.mMultiTouch)
    mInputContext.mMultiTouch->setEventCallback(NULL);
#endif
  if (mInputContext.mAccelerometer)
    mInputContext.mAccelerometer->setEventCallback(NULL);

  deleteInputDevices();

  // delete input managers
  if (CGameInputManager::getSingletonPtr()) {
    delete CGameInputManager::getSingletonPtr();
  }
  if (CInputListenerManager::getSingletonPtr()) {
    delete CInputListenerManager::getSingletonPtr();
  }
}

void CApplication::deleteInputDevices() {
  if (mInputManager) {
    if (mInputContext.mKeyboard)
      mInputManager->destroyInputObject(mInputContext.mKeyboard);
    if (mInputContext.mMouse)
      mInputManager->destroyInputObject(mInputContext.mMouse);
#if OIS_WITH_MULTITOUCH
    if (mInputContext.mMultiTouch)
      mInputManager->destroyInputObject(mInputContext.mMultiTouch);
#endif
    if (mInputContext.mAccelerometer)
      mInputManager->destroyInputObject(mInputContext.mAccelerometer);

    OIS::InputManager::destroyInputSystem(mInputManager);
    mInputManager = nullptr;
  }
}
