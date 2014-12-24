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

#include "AndroidApplication.hpp"

#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID

#include <android_native_app_glue.h>
#include <OgreArchiveManager.h>
#include "Android/OgreAPKFileSystemArchive.h"
#include "Android/OgreAPKZipArchive.h"

#include "../Log.hpp"
#include "../Util/Assert.hpp"
#include "../Android/Android.hpp"

void CAndroidApplication::initAppForAndroid(Ogre::RenderWindow *window,
                                            struct android_app* app,
                                            OIS::MultiTouch *mouse,
                                            OIS::Keyboard *keyboard) {
  LOGV("Initializing App for Android");
  ASSERT(mouse);
  ASSERT(keyboard);
  mApp = app;
  mWindow = window;
  mInputContext.mMultiTouch = mouse;
  mInputContext.mKeyboard = keyboard;

  if (app != NULL) {
    mNativeActivity = app->activity;
    mAssetMgr = app->activity->assetManager;
    Ogre::ArchiveManager::getSingleton().addArchiveFactory(
        new Ogre::APKFileSystemArchiveFactory(app->activity->assetManager));
    Ogre::ArchiveManager::getSingleton().addArchiveFactory(
        new Ogre::APKZipArchiveFactory(app->activity->assetManager));
  }
}

Ogre::DataStreamPtr CAndroidApplication::openAPKFile(
    const Ogre::String& fileName) {
  Ogre::DataStreamPtr stream;
  AAsset* asset = AAssetManager_open(mAssetMgr,
                                     fileName.c_str(),
                                     AASSET_MODE_BUFFER);
  if (asset) {
    off_t length = AAsset_getLength(asset);
    void* membuf = OGRE_MALLOC(length, Ogre::MEMCATEGORY_GENERAL);
    memcpy(membuf, AAsset_getBuffer(asset), length);
    AAsset_close(asset);

    stream = Ogre::DataStreamPtr(
        new Ogre::MemoryDataStream(membuf, length, true, true));
  }
  return stream;
}

void CAndroidApplication::initApp() {
  CApplication::initApp();

  // init render target on our own
  mRoot->getRenderSystem()->_initRenderTargets();
  mRoot->clearEventTimes();
}

void CAndroidApplication::createRoot() {
  mRoot = Ogre::Root::getSingletonPtr();
}

bool CAndroidApplication::oneTimeConfig() {
  // does nothing, static libs, fixed config.
  return true;
}

Ogre::RenderWindow *CAndroidApplication::createWindow() {
  return mWindow;
}

void CAndroidApplication::createInputSystem(bool nograb) {
  // input system should already be initiated
  ASSERT(mInputContext.mMultiTouch);
  ASSERT(mInputContext.mKeyboard);
}

void CAndroidApplication::createInputDevices() {
  // input system should already be initiated
  ASSERT(mInputContext.mMultiTouch);
  ASSERT(mInputContext.mKeyboard);
}

Ogre::DataStreamPtr CAndroidApplication::getConfigFileStream() {
  // open apk file
  return openAPKFile(CApplication::getConfigFilePath());
}

void CAndroidApplication::loadResources() {
  // close loading screen
  OgreAndroidBridge::callJavaVoid("closeLoadDialog");

  // normal loading
  CApplication::loadResources();
}

void CAndroidApplication::installWindowEventListener() {
  // dont install as window event listener
}

void CAndroidApplication::deleteInputDevices() {
  // dont delete
}

#endif  // OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
