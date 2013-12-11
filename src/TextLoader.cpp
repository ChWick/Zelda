#include "StdAfx.h"
#include "TextLoader.h"
#include "PickableObject.h"
#include "MapManager.h"
#include "Map.h"

const Ogre::String MAP_NOT_FOUND_TEXT("Map texts have not been set...");
const Ogre::String ERROR_TEXT("This text has not been set...");
const Ogre::String INNER_OBJECT_TEXT_NOT_FOUND("Description of the inner Object not found...");


using namespace tinyxml2;

template<> CTextLoader* Ogre::Singleton<CTextLoader>::msSingleton = 0;

CTextLoader* CTextLoader::getSingletonPtr(void)
{
    return msSingleton;
}
CTextLoader& CTextLoader::getSingleton(void)
{
    assert( msSingleton );  return ( *msSingleton );
}

CTextLoader::CTextLoader() {
	m_LanguageIds[L_GERMAN] = "de";
	m_LanguageIds[L_ENGLISH] = "en";
}
const Ogre::String &CTextLoader::getInnerObjectTexts(unsigned int pickableObjectType) const {
    if (m_mInnerObjectTexts.find(pickableObjectType) == m_mInnerObjectTexts.end()) {
        return INNER_OBJECT_TEXT_NOT_FOUND;
    }
    return m_mInnerObjectTexts.at(pickableObjectType);
}
const Ogre::String &CTextLoader::getChatText(const Ogre::String &sMapName, const Ogre::String &sId) const {
    if (m_mChats.find(sMapName) == m_mChats.end()) {return MAP_NOT_FOUND_TEXT;}

    const std::vector<ChatEntry> ceList = m_mChats.at(sMapName);

    for (const ChatEntry &chat : ceList) {
        if (chat.m_sId == sId) {
            return chat.m_sText;
        }
    }
    return ERROR_TEXT;
}
void CTextLoader::replaceVariablesInMapText(Ogre::String &sText) const {
    Ogre::String sMapName = CMapManager::getSingleton().getCurrentMap()->getName();

    // $variables for chats
    boost::regex chatReplaceRegEx("\\$\\{([\\d\\w]+)\\}");
    boost::cmatch matches;
    if (boost::regex_match(sText.c_str(), matches, chatReplaceRegEx)){
        std::string m2 = matches[1];
        std::string replaceWith = getChatText(sMapName, m2);
        sText = Ogre::StringUtil::replaceAll(sText, "${" + m2 + "}", replaceWith);
    }

    boost::replace_all(sText, "${name}", "Dofter");
    boost::replace_all(sText, "\\n", "\n");
}
void CTextLoader::load(ELanguage eLanguage) {
	Ogre::String file("texts/" + m_LanguageIds[eLanguage] + "_texts.xml");
	XMLDocument doc;
	try {
		Ogre::String basename, path;
	    Ogre::StringUtil::splitFilename(file, basename, path);

	    Ogre::DataStreamPtr pStream = Ogre::ResourceGroupManager::getSingleton().
	        openResource( basename, "General" );


	    Ogre::String data = pStream->getAsString();
		auto res = doc.Parse(data.c_str());
		if (res != XML_NO_ERROR) {
			throw Ogre::Exception(0, data + " could not be parsed, error code:" + Ogre::StringConverter::toString(res), __FILE__);
		}
	}
	catch(...)
    {
        //We'll just log, and continue on gracefully
        Ogre::LogManager::getSingleton().logMessage("[TextLoader] Error creating TiXmlDocument for file: " + file);
        return;
    }

	XMLElement *pElement = doc.FirstChildElement("innerObjects");
	if (pElement) {
		parseInnerObjects(pElement);
	}

	pElement = doc.FirstChildElement("chats");
	if (pElement) {
		parseChats(pElement);
	}
}
void CTextLoader::parseInnerObjects(tinyxml2::XMLElement *pElem) {
	XMLElement *pElement = pElem->FirstChildElement("innerObject");
    while(pElement)
    {
        parseInnerObject(pElement);
        pElement = pElement->NextSiblingElement("innerObject");
    }
}
void CTextLoader::parseInnerObject(tinyxml2::XMLElement *pElem) {
	Ogre::String name(pElem->Attribute("name"));
	Ogre::String text(pElem->Attribute("text"));

	m_mInnerObjectTexts[CPickableObject::parsePickableObjectType(name)] = text;
}
void CTextLoader::parseChats(tinyxml2::XMLElement *pElem) {
    XMLElement *pElement = pElem->FirstChildElement("mapChats");
    while(pElement)
    {
        parseMapChats(pElement);
        pElement = pElement->NextSiblingElement("mapChats");
    }
}
void CTextLoader::parseMapChats(tinyxml2::XMLElement *pElem) {
    Ogre::String mapName = pElem->Attribute("name");
    std::vector<ChatEntry> &chats = m_mChats[mapName];

    XMLElement *pElement = pElem->FirstChildElement("chat");
    while(pElement)
    {
        parseChat(pElement, chats);
        pElement = pElement->NextSiblingElement("chat");
    }
}
void CTextLoader::parseChat(tinyxml2::XMLElement *pElem, ChatEntryList &entryList) {
    Ogre::String id = pElem->Attribute("id");
    Ogre::String text = pElem->Attribute("text");

    entryList.push_back({id, text});
}
