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

#include "UserData.hpp"
#include <OgreStringConverter.h>

CUserData::CUserData() {
}
CUserData::CUserData(tinyxml2::XMLElement *pNode) {
    parseNode(pNode);
}
CUserData::~CUserData() {
}
void CUserData::parseNode(tinyxml2::XMLElement *pNode) {
    tinyxml2::XMLElement *pElement = pNode->FirstChildElement("user_data");
    while(pElement)
    {
        readAttributes(pElement);
        pElement = pElement->NextSiblingElement("user_data");
    }
}
void CUserData::readAttributes(tinyxml2::XMLElement *pElem) {
    m_lAttributeList.push_back({pElem->Attribute("name"), pElem->Attribute("type"), pElem->Attribute("value")});
}
const Ogre::String CUserData::getStringUserData(const Ogre::String &name, const Ogre::String &defaultValue) const {
    for (auto &attr : m_lAttributeList) {
        Ogre::String nameLabel(attr.m_sLabel);
        Ogre::String type(attr.m_sType);
        if (nameLabel == name && type == "str") {
            return attr.m_sValue;
        }
    }
    return defaultValue;
}
bool CUserData::getBoolUserData(const Ogre::String &name, bool defaultValue) const {
    for (auto &attr : m_lAttributeList) {
        Ogre::String nameLabel(attr.m_sLabel);
        Ogre::String type(attr.m_sType);
        if (nameLabel == name && type == "str") {
            return Ogre::StringConverter::parseBool(attr.m_sValue);
        }
    }
    return defaultValue;
}
void CUserData::setUserData(const Ogre::String &label, bool value) {
     for (auto &attr : m_lAttributeList) {
        Ogre::String nameLabel(attr.m_sLabel);
        Ogre::String type(attr.m_sType);
        if (nameLabel == label && type == "str") {
            attr.m_sValue = Ogre::StringConverter::toString(value);
            return;
        }
     }

     m_lAttributeList.push_back({label, "str", Ogre::StringConverter::toString(value)});
}
void CUserData::setUserData(const Ogre::String &label, const Ogre::String &value) {
     for (auto &attr : m_lAttributeList) {
        Ogre::String nameLabel(attr.m_sLabel);
        Ogre::String type(attr.m_sType);
        if (nameLabel == label && type == "str") {
            attr.m_sValue = value;
            return;
        }
     }

     m_lAttributeList.push_back({label, "str", value});
}
