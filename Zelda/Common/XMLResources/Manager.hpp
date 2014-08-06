/*****************************************************************************
 * Copyright 2014 Christoph Wick
 *
 * This file is part of Mencus.
 *
 * Mencus is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * Mencus is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * Mencus. If not, see http://www.gnu.org/licenses/.
 *****************************************************************************/

#ifndef _XMLRESOURCES_MANAGER_HPP_
#define _XMLRESOURCES_MANAGER_HPP_

#include <map>
#include "tinyxml2/tinyxml2.h"
#include <OgrePlatform.h>
#include <OgreException.h>
#include <OgreResourceGroupManager.h>
#include <CEGUI/String.h>

namespace XMLResources {
  class CManager {
  public:
    static std::string LANGUAGE_CODE;
  private:

    std::map<std::string, std::string> m_lStringResources;
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

    const std::string &getString(const std::string &id) const;
    const CEGUI::String getCEGUIString(const std::string &id) const;
    void loadLanguage() {
      parse(m_sPrefix + "values/strings.xml");
      if (LANGUAGE_CODE.length() > 0) {
	parse(m_sPrefix + "values-" + LANGUAGE_CODE + "/strings.xml");
      }
    }
  private:
    void parse(const Ogre::String &path);
  };

  extern CManager GLOBAL;
};

#endif
