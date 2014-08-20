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
