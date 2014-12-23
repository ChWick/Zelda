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

#include "../Util/Assert.hpp"
#include "../Android/Android.hpp"

void CAndroidApplication::initApp() {
  CAppliation::initApp();

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

Ogre::String getConfigFilePath() {
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
