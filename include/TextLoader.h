#ifndef _TEXT_LOADER_H_
#define _TEXT_LOADER_H_

#include "StdAfx.h"


class CTextLoader : public Ogre::Singleton<CTextLoader> {
public:
	enum ELanguage {
		L_GERMAN = 0,
		L_ENGLISH,

		L_NUM,
	};
	struct ChatEntry {
        Ogre::String m_sId;
        Ogre::String m_sText;
	};
    typedef std::vector<ChatEntry> ChatEntryList;
private:
	Ogre::String m_LanguageIds[L_NUM];
	std::map<unsigned int, Ogre::String> m_mInnerObjectTexts;
	std::map<Ogre::String, ChatEntryList > m_mChats;
public:
	static CTextLoader& getSingleton(void);
    static CTextLoader* getSingletonPtr(void);
	CTextLoader();

	void load(ELanguage eLanguage);

	const Ogre::String &getInnerObjectTexts(unsigned int pickableObjectType) const;
	const Ogre::String &getChatText(const Ogre::String &sMapName, const Ogre::String &sId) const;

	void replaceVariablesInMapText(Ogre::String &sText) const;

private:
	void parseInnerObjects(tinyxml2::XMLElement *pElem);
	void parseInnerObject(tinyxml2::XMLElement *pElem);

	void parseChats(tinyxml2::XMLElement *pElem);
	void parseMapChats(tinyxml2::XMLElement *pElem);
	void parseChat(tinyxml2::XMLElement *pElem, ChatEntryList &entryList);
};

#endif
