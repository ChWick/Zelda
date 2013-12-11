#include "StdAfx.h"
#include "UserData.h"

UserData::UserData() {
}
UserData::UserData(tinyxml2::XMLElement *pNode) {
    parseNode(pNode);
}
UserData::~UserData() {
}
void UserData::parseNode(tinyxml2::XMLElement *pNode) {
    tinyxml2::XMLElement *pElement = pNode->FirstChildElement("user_data");
    while(pElement)
    {
        readAttributes(pElement);
        pElement = pElement->NextSiblingElement("user_data");
    }
}
void UserData::readAttributes(tinyxml2::XMLElement *pElem) {
    m_lAttributeList.push_back({pElem->Attribute("name"), pElem->Attribute("type"), pElem->Attribute("value")});
}
const Ogre::String UserData::getStringUserData(const Ogre::String &name, const Ogre::String &defaultValue) const {
    for (auto &attr : m_lAttributeList) {
        Ogre::String nameLabel(attr.m_sLabel);
        Ogre::String type(attr.m_sType);
        if (nameLabel == name && type == "str") {
            return attr.m_sValue;
        }
    }
    return defaultValue;
}
bool UserData::getBoolUserData(const Ogre::String &name, bool defaultValue) const {
    for (auto &attr : m_lAttributeList) {
        Ogre::String nameLabel(attr.m_sLabel);
        Ogre::String type(attr.m_sType);
        if (nameLabel == name && type == "str") {
            return Ogre::StringConverter::parseBool(attr.m_sValue);
        }
    }
    return defaultValue;
}
void UserData::setUserData(const Ogre::String &label, bool value) {
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
void UserData::setUserData(const Ogre::String &label, const Ogre::String &value) {
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
