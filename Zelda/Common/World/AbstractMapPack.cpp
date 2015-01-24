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

#include "AbstractMapPack.hpp"
#include <OgreResourceGroupManager.h>
#include <string>
#include "../Log.hpp"

CAbstractMapPack::CAbstractMapPack(const std::string &path,
                                   const std::string &name)
    : mPath(path),
      mName(name),
      mResourceGroup(name + "_RG"),
      mInitialized(false),
      mLanguageManager(name + "_RG", "language/", false),
      mListener(nullptr) {
  LOGV("Created map pack for: '%s%s'", path.c_str(), name.c_str());
}

CAbstractMapPack::~CAbstractMapPack() {
  exit();
}

void CAbstractMapPack::init(CAbstractMapPackListener *listener) {
  LOGV("Initializing map pack %s", mName.c_str());
  mListener = listener;
  if (mInitialized) {return;}
  mInitialized = true;

  Ogre::ResourceGroupManager &rgm(
      Ogre::ResourceGroupManager::getSingleton());

  rgm.createResourceGroup(mResourceGroup, false);
#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
  rgm.addResourceLocation(mPath + mName + ".zip", "APKZip",
                          mResourceGroup, true, true);
#else
  rgm.addResourceLocation(mPath + mName + ".zip", "Zip",
                          mResourceGroup, true, true);
#endif  // OGRE_PLATFORM
  Ogre::StringVectorPtr files(rgm.listResourceNames(mResourceGroup));
  for (const Ogre::String &r : *files) {
    if (r.find(".lua") != Ogre::String::npos) {
      rgm.declareResource(r, "LuaScript", mResourceGroup);
      LOGV("Added lua script at '%s' with name '%s'", r.c_str(), r.c_str());
    }
  }
  rgm.initialiseResourceGroup(mResourceGroup);
  rgm.loadResourceGroup(mResourceGroup);

  mLanguageManager.loadLanguage();
}

void CAbstractMapPack::exit() {
  if (!mInitialized) {return;}
  mInitialized = false;

  Ogre::ResourceGroupManager &rgm(
      Ogre::ResourceGroupManager::getSingleton());

  rgm.unloadResourceGroup(mResourceGroup);
  rgm.destroyResourceGroup(mResourceGroup);
}
