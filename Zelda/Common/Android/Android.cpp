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

#include <OgrePlatform.h>

#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID

#include "Android.hpp"
#include GAME_CLASS_HEADER

CAndroidApplication* OgreAndroidBridge::mApplication = NULL;
AndroidInputInjector* OgreAndroidBridge::mInputInjector = NULL;
AndroidMultiTouch* OgreAndroidBridge::mTouch = NULL;
AndroidKeyboard* OgreAndroidBridge::mKeyboard = NULL;
Ogre::RenderWindow* OgreAndroidBridge::mRenderWnd = NULL;
Ogre::Root* OgreAndroidBridge::mRoot = NULL;
bool OgreAndroidBridge::mInit = false;
bool OgreAndroidBridge::m_bRenderPaused = true;
// CSnapshot *OgreAndroidBridge::m_pSnapshot = NULL;
ANativeActivity *OgreAndroidBridge::mActivity = NULL;


void OgreAndroidBridge::start() {
  ASSERT(mActivity);

  if (mInit)
    return;

  LOGI("Initialising Root");
  mRoot = new Ogre::Root("plugins"OGRE_BUILD_SUFFIX".cfg",
                         "ogre.cfg",
                         CFileManager::getValidPath("ogre.log",
                                                    CFileManager::SL_EXTERNAL));
#ifdef OGRE_STATIC_LIB
  LOGI("Loading plugins");
  mStaticPluginLoader = new Ogre::StaticPluginLoader();
  mStaticPluginLoader->load();
  LOGI("plugins loaded");
#endif
  if (mRoot->getAvailableRenderers().size() == 0) {
    LOGW("No available renderer found");
    return;
  }
  mRoot->setRenderSystem(mRoot->getAvailableRenderers().at(0));
  LOGI("Initalising");
  mRoot->initialise(false);
  mInit = true;
}

void OgreAndroidBridge::handleCmd(struct android_app* app, int32_t cmd) {
  if (app->savedState) {
    LOGI("loading snapshot ...");
    // CSnapshotManager::getSingleton().setSnapshot(
    // std::shared_ptr<CSnapshot>(new CSnapshot(app->savedState, app->savedStateSize)))`;
  }
  switch (cmd)
  {
    case APP_CMD_SAVE_STATE:
      LOGI("Saving state");
      //CSnapshotManager::getSingleton().makeBackupSnapshot();
      //CSnapshotManager::getSingleton().makeSnapshot().saveToMemory(app->savedState, app->savedStateSize);
      //if (CSaveStateManager::getSingletonPtr()) {CSaveStateManager::getSingleton().writeXMLFile();}
      break;
    case APP_CMD_INIT_WINDOW:
      m_bRenderPaused = false;
      LOGI("Initialising window command");
      if (app->window && mRoot) {
        LOGI("... creating config");
        AConfiguration* config = AConfiguration_new();
        AConfiguration_fromAssetManager(config, app->activity->assetManager);

        if (!mRenderWnd) {
          LOGI("... creating render window");
          Ogre::NameValuePairList opt;
          opt["externalWindowHandle"] = Ogre::StringConverter::toString((int)app->window);
          opt["androidConfig"] = Ogre::StringConverter::toString((int)config);

          mRenderWnd = Ogre::Root::getSingleton().createRenderWindow("OgreWindow", 0, 0, false, &opt);

          if(!mTouch)
            mTouch = new AndroidMultiTouch();

          if(!mKeyboard)
            mKeyboard = new AndroidKeyboard();

          if(!mApplication) {
            LOGI("... creating game");
            mApplication = OGRE_NEW GAME_CLASS();
            mApplication->initAppForAndroid(mRenderWnd, app, mTouch, mKeyboard);
            mApplication->initApp();

            mInputInjector = new AndroidInputInjector(CInputListenerManager::getSingletonPtr(), mTouch, mKeyboard);
          }
        }
        else {
          LOGI("... recreating render winow");
          static_cast<Ogre::AndroidEGLWindow*>(mRenderWnd)->_createInternalResources(app->window, config);
          if (mApplication) {
            LOGI("... creating resources");
            mApplication->createResources();
          }
        }

        AConfiguration_delete(config);
      }
      break;
    case APP_CMD_TERM_WINDOW:
      if (mApplication)
        mApplication->destroyResources();

      if(mRoot && mRenderWnd)
        static_cast<Ogre::AndroidEGLWindow*>(mRenderWnd)->_destroyInternalResources();
      break;
    case APP_CMD_PAUSE:
      m_bRenderPaused = true;
      break;
    case APP_CMD_RESUME:
      m_bRenderPaused = false;
      break;
    case APP_CMD_GAINED_FOCUS:
      break;
    case APP_CMD_LOST_FOCUS:
      break;
    case APP_CMD_CONFIG_CHANGED:
      break;
  }
}

void OgreAndroidBridge::shutdown() {
  if (!mInit)
    return;

  mInit = false;

  if (mApplication) {
    mApplication->closeApp();
    OGRE_DELETE mApplication;
    mApplication = NULL;
  }

  OGRE_DELETE mRoot;
  mRoot = NULL;
  mRenderWnd = NULL;

  delete mTouch;
  mTouch = NULL;

  delete mKeyboard;
  mKeyboard = NULL;

  delete mInputInjector;
  mInputInjector = NULL;

#ifdef OGRE_STATIC_LIB
  mStaticPluginLoader->unload();
  delete mStaticPluginLoader;
  mStaticPluginLoader = NULL;
#endif
}

#endif  // OGRE_PLATFORM == OGRE_PLATFORM
