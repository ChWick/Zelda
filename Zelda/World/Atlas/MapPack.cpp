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

#include "MapPack.hpp"
#include <OgreResourceGroupManager.h>
#include "../../Common/tinyxml2/tinyxml2.hpp"
#include "../../Common/Util/XMLHelper.hpp"
#include <OgreLogManager.h>
#include "MapPackParserListener.hpp"
#include "RegionInfo.hpp"
#include <OgreStringConverter.h>
#include "../../Common/Log.hpp"

using namespace tinyxml2;
using namespace XMLHelper;

CMapPack::CMapPack(const std::string &path, const std::string &name)
  : m_sPath(path),
    m_sName(name),
    m_sResourceGroup(name + "_RG"),
    m_bInitialized(false),
    m_pListener(nullptr) {

  LOGV("Created map pack for: '%s%s'", path.c_str(), name.c_str());
}

CMapPack::~CMapPack() {
  exit();
}

void CMapPack::init(CMapPackParserListener *pListener) {
  LOGV("Initializing map pack %s", m_sName.c_str());
  m_pListener = pListener;
  if (m_bInitialized) {return;}
  m_bInitialized = true;

  Ogre::ResourceGroupManager::getSingleton().createResourceGroup(m_sResourceGroup, false);
#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
  Ogre::ResourceGroupManager::getSingleton().addResourceLocation(m_sPath + m_sName + ".zip", "APKZip", m_sResourceGroup);
#else
  Ogre::ResourceGroupManager::getSingleton().addResourceLocation(m_sPath + m_sName + ".zip", "Zip", m_sResourceGroup);
#endif // OGRE_PLATFORM
  Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup(m_sResourceGroup);
  Ogre::ResourceGroupManager::getSingleton().loadResourceGroup(m_sResourceGroup);

  m_sSceneFile = m_sName + ".scene";
}

void CMapPack::parse() {
  parseXMLFile();
}

void CMapPack::exit() {
  if (!m_bInitialized) {return;}
  m_bInitialized = false;

  Ogre::ResourceGroupManager::getSingleton().unloadResourceGroup(m_sResourceGroup);
  Ogre::ResourceGroupManager::getSingleton().destroyResourceGroup(m_sResourceGroup);
}

void CMapPack::parseXMLFile() {
  Ogre::DataStreamPtr dataStream
    = Ogre::ResourceGroupManager::getSingleton().openResource(m_sName + ".xml", m_sResourceGroup, false);

  if (dataStream.isNull()) {
    throw Ogre::Exception(0, "File " + m_sName + ".zip not found in resource group " + m_sResourceGroup + "!", __FILE__);
  }

  Ogre::LogManager::getSingleton().logMessage("Reading map xml file.");

  XMLDocument doc;
  doc.Parse(dataStream->getAsString().c_str());

  XMLElement *pMapElem = doc.FirstChildElement();
  m_vGlobalPosition = Ogre::StringConverter::parseVector3(Attribute(pMapElem, "global_position"));
  m_vGlobalSize = Ogre::StringConverter::parseVector2(Attribute(pMapElem, "global_size"));

  for (XMLElement *pElem = pMapElem->FirstChildElement(); pElem; pElem = pElem->NextSiblingElement()) {
    if (strcmp(pElem->Value(), "region") == 0) {
      SRegionInfo region;

      region.position = Ogre::StringConverter::parseVector3(Attribute(pElem, "position"));
      region.size = Ogre::StringConverter::parseVector3(Attribute(pElem, "size"));
      region.ID = Attribute(pElem, "id");
      region.shape = Attribute(pElem, "shape");

      if (m_pListener) {m_pListener->parseRegion(region);}
    }
    else if (strcmp(pElem->Value(), "player") == 0) {
      if (m_pListener) {m_pListener->parsePlayer(pElem);}
    }
    else if (strcmp(pElem->Value(), "scene_entity") == 0) {
      if (m_pListener) {m_pListener->parseSceneEntity(pElem);}
    }
  }
}
