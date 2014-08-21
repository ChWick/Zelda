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

#include "PlayerData.hpp"

using namespace tinyxml2;

template<> CPlayerData* Ogre::Singleton<CPlayerData>::msSingleton = 0;

// ============================================================================================================================
// CMapSaveData
// ============================================================================================================================
void CMapSaveData::writeToFile(tinyxml2::XMLDocument &doc, tinyxml2::XMLElement *pMapElement) {
    for (auto it = items.begin(); it != items.end(); it++) {
		XMLElement *pEntry = doc.NewElement("entry");
		pEntry->SetAttribute("id", (*it).first.c_str());
		pEntry->SetAttribute("value", (*it).second);
		pMapElement->InsertEndChild(pEntry);
	}
}
void CMapSaveData::readFromFile(XMLElement *pElement) {
	for (XMLElement *pItem = pElement->FirstChildElement(); pItem; pItem = pItem->NextSiblingElement()) {
		if (Ogre::String(pItem->Value()) == "entry") {
			items[pItem->Attribute("id")] = static_cast<EItemSaveState>(pItem->IntAttribute("value"));
		}
	}
}
// ============================================================================================================================
// CPlayerSaveData
// ============================================================================================================================
CPlayerData::CPlayerData()
    : m_uiHP(300),          // This is the default hp (since hp >= 25, or dead if 0)
    m_uiMaxHP(300),
    m_fMP(0),
    m_iRubyCount(0),
    m_iKeyCount(0),
    m_iArrowCount(0),
    m_iBombCount(0) {
	readFromFile();
}
CPlayerData::~CPlayerData() {
	writeToFile();
}
void CPlayerData::writeToFile() {
	XMLDocument doc;
	doc.InsertEndChild(doc.NewDeclaration());
	XMLElement *pRoot = doc.NewElement("data");
	doc.InsertEndChild(pRoot);

    pRoot->SetAttribute("bombCount", m_iBombCount);
    pRoot->SetAttribute("arrowCount", m_iArrowCount);
	pRoot->SetAttribute("rubyCount", m_iRubyCount);
	pRoot->SetAttribute("keyCount", m_iKeyCount);
	pRoot->SetAttribute("maxHp", m_uiMaxHP);
	pRoot->SetAttribute("hp", m_uiHP);
	pRoot->SetAttribute("mp", m_fMP);
    for (auto it = m_mapSaveDataOfMap.begin(); it != m_mapSaveDataOfMap.end(); it++) {
		XMLElement *pMapElement = doc.NewElement("map");
		pMapElement->SetAttribute("name", (*it).first.c_str());
		pRoot->InsertEndChild(pMapElement);

		(*it).second.writeToFile(doc, pMapElement);
	}

	XMLElement *pPlayerToolsNode = doc.NewElement("tools");
	pRoot->InsertEndChild(pPlayerToolsNode);
	writePlayerTools(pPlayerToolsNode, doc);

	doc.SaveFile("playerData.xml");
}
void CPlayerData::readFromFile() {
	XMLDocument doc;
	if (doc.LoadFile("playerData.xml")) {
	    //Ogre::LogManager::getSingleton().logMessage("playerData.xml can not be read");
	    return;
	}

	XMLElement *pRoot = doc.FirstChildElement("data");
	assert(pRoot);

    m_iArrowCount = pRoot->IntAttribute("arrowCount");
	m_iRubyCount = pRoot->IntAttribute("rubyCount");
	m_iKeyCount = pRoot->IntAttribute("keyCount");
	m_uiMaxHP = pRoot->IntAttribute("maxHp");
	m_uiHP = pRoot->IntAttribute("hp");
	m_fMP = pRoot->FloatAttribute("mp");
    m_iBombCount = pRoot->IntAttribute("bombCount");

	assert(m_uiMaxHP > 4); // this helps if one accendently sets the hp to be 3 instead of 300! (25 is possible ofc)

	for (XMLElement *pElement = pRoot->FirstChildElement(); pElement; pElement = pElement->NextSiblingElement()) {
		if (Ogre::String(pElement->Value()) == "map") {
			m_mapSaveDataOfMap[pElement->Attribute("name")].readFromFile(pElement);
		}
		else if (Ogre::String(pElement->Value()) == "tools") {
            readPlayerTools(pElement);
		}
	}
}
void CPlayerData::readPlayerTools(XMLElement *pElem) {
    for (XMLElement *pTool = pElem->FirstChildElement(); pTool; pTool = pTool->NextSiblingElement()) {
        if (Ogre::String(pTool->Value()) == "tool") {
            int type = pTool->IntAttribute("type");
            bool exists = pTool->BoolAttribute("exists");
            //m_PlayerTools.getTool(type).setExists(exists);
        }
    }
}
void CPlayerData::writePlayerTools(XMLElement *pElem, tinyxml2::XMLDocument &doc) {
/*for (unsigned int i = 0; i < m_PlayerTools.getNumTools(); i++) {
        XMLElement *pToolElem = doc.NewElement("tool");
        pElem->InsertEndChild(pToolElem);

//pToolElem->SetAttribute("type", m_PlayerTools.getTool(i).getToolType());
      //pToolElem->SetAttribute("exists", m_PlayerTools.getTool(i).doesExists());
    }*/
}
