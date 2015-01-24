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

#ifndef ANDROIDAPPLICATION_HPP
#define ANDROIDAPPLICATION_HPP

#include "Application.hpp"

#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID

#include "../Android/Android.hpp"

class CAndroidApplication
    : public CApplication {
 protected:
  struct android_app* mApp;
  //! Android asset manager to access files inside apk
  AAssetManager* mAssetMgr;
  ANativeActivity* mNativeActivity;
 public:
  void initAppForAndroid(Ogre::RenderWindow *window,
                         struct android_app* app,
                         OIS::MultiTouch *mouse,
                         OIS::Keyboard *keyboard);

  Ogre::DataStreamPtr openAPKFile(const Ogre::String& fileName);

  
  virtual void initApp();
 protected:
  virtual void createRoot();
  
  virtual bool oneTimeConfig();
  
  virtual Ogre::RenderWindow *createWindow();
  
  virtual void createInputSystem(bool nograb);
  virtual void createInputDevices();
  
  virtual Ogre::DataStreamPtr getConfigFileStream();
  virtual void loadResources();
  
  virtual void installWindowEventListener();

  virtual void deleteInputDevices();
};

#endif  // OGRE_PLATFORM == OGRE_PLATFORM_ANDROID

#endif /* ANDROIDAPPLICATION_HPP */
