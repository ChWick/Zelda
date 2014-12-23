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

class CAndroidApplication
    : public CApplication {
 public:
 protected:
  virtual void initApp();
  virtual void createRoot();
  
  virtual bool oneTimeConfig();
  
  virtual Ogre::RenderWindow *createWindow();
  
  virtual void createInputSystem(bool nograb);
  virtual void createInputDevices();
  
  virtual Ogre::String getConfigFilePath();
  virtual void loadResources();
  
  virtual void installWindowEventListener();

  virtual void deleteInputDevices();
};

#endif  // OGRE_PLATFORM == OGRE_PLATFORM_ANDROID

#endif /* ANDROIDAPPLICATION_HPP */
