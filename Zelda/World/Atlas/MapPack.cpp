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
#include <OgreStringConverter.h>
#include <string>
#include "../../Common/tinyxml2/tinyxml2.hpp"
#include "../../Common/Util/XMLHelper.hpp"
#include "MapPackParserListener.hpp"
#include "../../Common/Log.hpp"

using tinyxml2::XMLElement;
using tinyxml2::XMLDocument;

using XMLHelper::Attribute;
using XMLHelper::RealAttribute;

CMapPack::CMapPack(const std::string &path, const std::string &name)
    : CAbstractMapPack(path, name) {
}

void CMapPack::init(CAbstractMapPackListener *listener) {
  CAbstractMapPack::init(listener);

  m_sSceneFile = mName + ".scene";
}

void CMapPack::parse() {
  parseXMLFile();
}

void CMapPack::parseXMLFile() {
  Ogre::DataStreamPtr dataStream
    = Ogre::ResourceGroupManager::getSingleton().openResource(
        mName + ".xml", mResourceGroup, false);

  if (dataStream.isNull()) {
    throw Ogre::Exception(0,
                          "File " + mName + ".zip not found in resource group "
                          + mResourceGroup + "!", __FILE__);
  }

  LOGV("Reading map xml file.");

  XMLDocument doc;
  doc.Parse(dataStream->getAsString().c_str());

  XMLElement *pMapElem = doc.FirstChildElement();
  m_vGlobalPosition = Ogre::StringConverter::parseVector3(
      Attribute(pMapElem, "global_position"));
  m_vGlobalSize = Ogre::StringConverter::parseVector2(
      Attribute(pMapElem, "global_size"));
  m_fVisionLevelOffset = RealAttribute(pMapElem, "vision_level_offset", 0.f);

  CMapPackParserListener *listener(
      dynamic_cast<CMapPackParserListener*>(mListener));

  ASSERT(listener);

  for (const XMLElement *pElem = pMapElem->FirstChildElement(); pElem;
       pElem = pElem->NextSiblingElement()) {
    if (strcmp(pElem->Value(), "event") == 0) {
      if (listener) {listener->parseEvent(pElem);}
    } else if (strcmp(pElem->Value(), "region") == 0) {
      if (listener) {listener->parseRegion(pElem);}
    } else if (strcmp(pElem->Value(), "entrance") == 0) {
      if (listener) {listener->parseEntrance(pElem);}
    } else if (strcmp(pElem->Value(), "player") == 0) {
      if (listener) {listener->parsePlayer(pElem);}
    } else if (strcmp(pElem->Value(), "scene_entity") == 0) {
      if (listener) {listener->parseSceneEntity(pElem);}
    } else if (strcmp(pElem->Value(), "new_entity") == 0) {
      if (listener) {listener->parseNewEntity(pElem);}
    } else {
      LOGW("Unknown entity in map pack '%s'", pElem->Value());
    }
  }
}
