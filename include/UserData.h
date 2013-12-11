#ifndef USERDATA_H
#define USERDATA_H

#include "StdAfx.h"

class UserData {
public:
    struct Attribute {
        Ogre::String m_sLabel;
        Ogre::String m_sType;
        Ogre::String m_sValue;
    };
private:
	std::vector<Attribute> m_lAttributeList;

public:
    UserData();
	UserData(tinyxml2::XMLElement *pNode);

    ~UserData();

    void parseNode(tinyxml2::XMLElement *pNode);

	const Ogre::String getStringUserData(const Ogre::String &name, const Ogre::String &defaultValue = Ogre::StringUtil::BLANK) const;
	bool getBoolUserData(const Ogre::String &name, bool defaultValue = false) const;

	void setUserData(const Ogre::String &label, bool value);
	void setUserData(const Ogre::String &label, const Ogre::String &value);

	const UserData &operator=(const UserData &src) {
        m_lAttributeList = src.m_lAttributeList;
        return *this;
	}
private:
    void readAttributes(tinyxml2::XMLElement *pElem);
};

#endif // USERDATA_H
