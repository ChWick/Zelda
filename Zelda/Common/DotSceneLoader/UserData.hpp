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

#ifndef USER_DATA_HPP
#define USER_DATA_HPP

#include <OgreString.h>
#include <vector>
#include "../tinyxml2/tinyxml2.h"

class CUserData {
public:
    struct Attribute {
        Ogre::String m_sLabel;
        Ogre::String m_sType;
        Ogre::String m_sValue;
    };
private:
	std::vector<Attribute> m_lAttributeList;

public:
    CUserData();
	CUserData(tinyxml2::XMLElement *pNode);

    ~CUserData();

    void parseNode(tinyxml2::XMLElement *pNode);

	const Ogre::String getStringUserData(const Ogre::String &name, const Ogre::String &defaultValue = Ogre::StringUtil::BLANK) const;
	bool getBoolUserData(const Ogre::String &name, bool defaultValue = false) const;

	void setUserData(const Ogre::String &label, bool value);
	void setUserData(const Ogre::String &label, const Ogre::String &value);

	const CUserData &operator=(const CUserData &src) {
        m_lAttributeList = src.m_lAttributeList;
        return *this;
	}
private:
    void readAttributes(tinyxml2::XMLElement *pElem);
};

#endif // USER_DATA_HPP
