#include "StdAfx.h"
#include "MapUserDataParser.h"
#include "tinyxml2.h"

using namespace tinyxml2;

void CMapUserDataParser::reset() {
	m_vChestUserData.clear();
	m_vSignUserData.clear();
}
void CMapUserDataParser::load(const Ogre::String &file, const Ogre::String &groupName) {
	reset();
	XMLDocument doc;
	try {
		Ogre::String basename, path;
	    Ogre::StringUtil::splitFilename(file, basename, path);

	    Ogre::DataStreamPtr pStream = Ogre::ResourceGroupManager::getSingleton().
	        openResource( basename, groupName );


	    Ogre::String data = pStream->getAsString();
		auto res = doc.Parse(data.c_str());
		if (res != XML_NO_ERROR) {
			throw Ogre::Exception(0, data + " could not be parsed, error code:" + Ogre::StringConverter::toString(res), __FILE__);
		}
	}
	catch(...)
    {
        //We'll just log, and continue on gracefully
        Ogre::LogManager::getSingleton().logMessage("[MapUserDataParser] Error creating TiXmlDocument for file: " + file);
        return;
    }
	XMLElement *pRoot = doc.FirstChildElement("mapUserData");

	XMLElement *pElement = pRoot->FirstChildElement("chests");
	if (pElement) {
		parseChests(pElement);
	}

	pElement = pRoot->FirstChildElement("signs");
	if (pElement) {
		parseSigns(pElement);
	}
}
void CMapUserDataParser::parseChests(tinyxml2::XMLElement *pElem) {
    XMLElement *pElement = pElem->FirstChildElement("chest");
    while(pElement)
    {
        parseChest(pElement);
        pElement = pElement->NextSiblingElement("chest");
    }
}
void CMapUserDataParser::parseChest(tinyxml2::XMLElement *pElem) {
	Ogre::String id(pElem->Attribute("id"));
	Ogre::String innerObject(pElem->Attribute("innerObject"));

	if (id.empty()) {
		throw Ogre::Exception(__LINE__, "Id of chest is not defined!", __FILE__);
	}

	m_vChestUserData.push_back(SChestUserData(id, InnerObjectGenerator::getInnerObjectByName(innerObject)));
}
void CMapUserDataParser::parseSigns(tinyxml2::XMLElement *pElem) {
	XMLElement *pElement = pElem->FirstChildElement("sign");
    while(pElement)
    {
        parseSign(pElement);
        pElement = pElement->NextSiblingElement("sign");
    }
}
void CMapUserDataParser::parseSign(tinyxml2::XMLElement *pElem) {
	Ogre::String id(pElem->Attribute("id"));
	Ogre::String front(pElem->Attribute("front"));
	Ogre::String rear(pElem->Attribute("rear"));

	if (id.empty()) {
		throw Ogre::Exception(__LINE__, "Id of sign is not defined!", __FILE__);
	}

	m_vSignUserData.push_back(SSignUserData(id, front, rear));
}
const CMapUserDataParser::SSignUserData &CMapUserDataParser::getSignUserDataById(const Ogre::String &id) {
	for (auto &ud : m_vSignUserData) {
		if (ud._id == id) {
			return ud;
		}
	}

	throw Ogre::Exception(__LINE__, "No sign with id " + id + " found", __FILE__);
}
const CMapUserDataParser::SChestUserData &CMapUserDataParser::getChestUserDataById(const Ogre::String &id) {
	for (auto &cud : m_vChestUserData) {
		if (cud._id == id) {
			return cud;
		}
	}

	throw Ogre::Exception(__LINE__, "No chest with id " + id + " found", __FILE__);
}
