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


#include <OgrePlatform.h>
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE && !defined(__LP64__)
#  include <Carbon/Carbon.h>
#endif
#include <OgrePrerequisites.h>
#include <OgreSingleton.h>
#include "../Input/InputListener.hpp"
#include "dependencies/OgreSdkUtil/SdkCameraMan.h"
#include "dependencies/OgreSdkUtil/SdkTrays.h"
#include <OgreFileSystemLayer.h>

#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
#include <android_native_app_glue.h>
#include "../Android/OgreAPKFileSystemArchive.h"
#include "../Android/OgreAPKZipArchive.h"
#include <OgreArchiveManager.h>
#endif

#include "../ShaderGenerator.hpp"
#include "../Message/MessageInjector.hpp"
#include <OgreWindowEventUtilities.h>

/**
 * Basic application
 * implements basic construction and deletion methods
 * override virtual methods to make it suitable for all platforms
 */
class CApplication {
public:
  virtual void go();
};


#endif /* APPLICATION_HPP */
