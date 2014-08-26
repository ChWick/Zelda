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
