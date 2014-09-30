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

#include "Manager.hpp"
#include "../FileManager/FileManager.hpp"
#include "../Log.hpp"

namespace XMLResources {
  std::string CManager::LANGUAGE_CODE;

#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
  CManager GLOBAL("", "language/", false);
#else
  CManager GLOBAL("", "../language/", false);
#endif

  void CManager::parse(const Ogre::String &path) {
    using namespace tinyxml2;

    XMLDocument doc;
    Ogre::DataStreamPtr pData;
    if (m_sResourceGroup.length() == 0) {
      pData = CFileManager::openDataStream(path);
    }
    else {
      pData = Ogre::ResourceGroupManager::getSingleton().openResource(path, m_sResourceGroup, false);
    }

    if (pData.isNull()) {
      throw Ogre::Exception(0, path + " not found in resource group " + m_sResourceGroup, __FILE__);
    }

    doc.Parse(pData->getAsString().c_str());

    if (doc.Error()) {
      throw Ogre::Exception(doc.ErrorID(), doc.GetErrorStr1(), __FILE__);
    }

    XMLElement *pResourcesElem(doc.FirstChildElement());
    for (XMLElement *pElem = pResourcesElem->FirstChildElement();
	 pElem;
	 pElem = pElem->NextSiblingElement()) {
      if (strcmp(pElem->Value(), "string") == 0) {
	m_lStringResources[pElem->Attribute("name")] = pElem->GetText();
      }
    }
  }

  const std::string &CManager::getString(const CEGUI::String &id, bool searchGlobal) const {
    if (m_lStringResources.count(id) != 1) {
      if (searchGlobal && this != &GLOBAL) {
        return GLOBAL.getString(id, false);
      }
      throw Ogre::Exception(0, ("String resource with id '" + id + "' was not found").c_str(), __FILE__);
    }
    return m_lStringResources.at(id);
  }

  const CEGUI::String CManager::getCEGUIString(const CEGUI::String &id, bool searchGlobal) const {
    LOGI("%s", id.c_str());
    if (m_lStringResources.count(id) != 1) {
      if (searchGlobal && this != &GLOBAL) {
        return GLOBAL.getCEGUIString(id, false);
      }
      throw Ogre::Exception(0, ("String resource with id '" + id + "' was not found").c_str(), __FILE__);
    }
    return reinterpret_cast<const CEGUI::utf8*>(m_lStringResources.at(id).c_str());
  }
};
