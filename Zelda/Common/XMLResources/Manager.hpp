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

#ifndef _XMLRESOURCES_MANAGER_HPP_
#define _XMLRESOURCES_MANAGER_HPP_

#include <map>
#include "../tinyxml2/tinyxml2.h"
#include <OgrePlatform.h>
#include <OgreException.h>
#include <OgreResourceGroupManager.h>
#include <CEGUI/String.h>
#include "../Log.hpp"

namespace XMLResources {
  class CManager {
  public:
    static std::string LANGUAGE_CODE;
  private:

    std::map<CEGUI::String, std::string> m_lStringResources;
    const std::string m_sResourceGroup;
    const std::string m_sPrefix;
  public:
    CManager(const std::string &sResourceGroup,
	     const std::string sPrefix = "",
	     bool bLoadOnCreate = true)
      : m_sResourceGroup(sResourceGroup),
	m_sPrefix(sPrefix) {
      if (bLoadOnCreate) {
	loadLanguage();
      }
    }

    bool hasString(const CEGUI::String &id, bool searchGlobal = true) const;
    const std::string &getString(const CEGUI::String &id, bool searchGlobal = true) const;
    const CEGUI::String getCEGUIString(const CEGUI::String &id, bool searchGlobal = true) const;

    void loadLanguage() {
      parse(m_sPrefix + "values/strings.xml");
      if (LANGUAGE_CODE.length() > 0) {
        try {
          parse(m_sPrefix + "values-" + LANGUAGE_CODE + "/strings.xml");
        }
        catch (const Ogre::Exception &e) {
          LOGW("Language file not found. Falling back to default language.");
        }
      }
    }
  private:
    void parse(const Ogre::String &path);
  };

  extern CManager GLOBAL;
};

#endif
