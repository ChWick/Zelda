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

#pragma once

#include "../../Common/tinyxml2/tinyxml2.hpp"
#include "OgrePrerequisites.h"
#include "OgreSingleton.h"
#include "../Hitpoints.hpp"

enum EItemSaveState {
	ISS_STILL_IN_MAP,
	ISS_PLAYER_HAS_IN_INVENTROY,
	ISS_PLAYER_HAS_USED,
	ISS_OPENED,						//!< wie eine Tür oder eine Schatztruhe
	ISS_CLOSED,						//!< wie eine Tür oder eine Schatztruhe
	ISS_ALIVE,
	ISS_DEAD,
};
enum ERubyValues {
	RUBY_VALUE_GREEN	= 1,
	RUBY_VALUE_BLUE		= 5,
	RUBY_VALUE_RED		= 10,
	RUBY_VALUE_200		= 200,

};
class CMapSaveData {
public:
	Ogre::map<Ogre::String, EItemSaveState>::type items;
	EItemSaveState getState(const Ogre::String &itemId, EItemSaveState defaultState) {
		if (items.find(itemId) == items.end()) {
			items[itemId] = defaultState;
		}
		return items[itemId];
	}

	void writeToFile(tinyxml2::XMLDocument &doc, tinyxml2::XMLElement *pMapElement);
	void readFromFile(tinyxml2::XMLElement *pElement);
};
class CPlayerData : public Ogre::Singleton<CPlayerData> {
private:
	Hitpoints m_uiHP;
	Hitpoints m_uiMaxHP;
	int m_iRubyCount;
	int m_iKeyCount;
	int m_iArrowCount;
	int m_iBombCount;
	Ogre::Real m_fMP;           //!< Mana points in interval [0, 1]

	Ogre::map<Ogre::String, CMapSaveData>::type m_mapSaveDataOfMap;
public:
	CPlayerData();
	~CPlayerData();

	void setHP(Hitpoints hp) {m_uiHP = hp;}
	Hitpoints getHP() const {return m_uiHP;}
	void setMaxHP(Hitpoints hp) {m_uiMaxHP = hp;}
	Hitpoints getMaxHP() const {return m_uiMaxHP;}

	void setMP(Ogre::Real mp) {m_fMP = mp;}
	Ogre::Real getMP() const {return m_fMP;}

	int getRubyCount() const {return m_iRubyCount;}
	int getKeyCount() const {return m_iKeyCount;}
	int getArrowCount() const {return m_iArrowCount;}
	int getBombCount() const {return m_iBombCount;}

  //void addArrow(int count) {m_iArrowCount += count; CHUD::getSingleton().setArrowCount(m_iArrowCount);}
	//void removeArrow(int count) {addArrow(-count);}

  //void addBomb(int count) {m_iBombCount += count; CHUD::getSingleton().setBombCount(m_iBombCount);}
	//void removeBomb(int count) {addBomb(-count);}

	//void addRuby(int count) {m_iRubyCount += count; CHUD::getSingleton().setRubyCount(m_iRubyCount);}
	//void addMP(Ogre::Real mp) {m_fMP += mp; if (m_fMP > 1) {m_fMP = 1;} CHUD::getSingleton().setMP(m_fMP);}
	//void addKey() {m_iKeyCount++; CHUD::getSingleton().setKeyCount(m_iKeyCount);}
	//void removeKey() {m_iKeyCount--; CHUD::getSingleton().setKeyCount(m_iKeyCount);}
	EItemSaveState getMapItemState(const Ogre::String &mapName, const Ogre::String &itemId, EItemSaveState defaultState) {
		return m_mapSaveDataOfMap[mapName].getState(itemId, defaultState);
	}
	void setMapItemState(EItemSaveState itemSaveState, const Ogre::String &mapName, const Ogre::String &itemId) {
		m_mapSaveDataOfMap[mapName].items[itemId] = itemSaveState;
	}


	void readFromFile();
	void writeToFile();
private:
    void readPlayerTools(tinyxml2::XMLElement *pElem);
    void writePlayerTools(tinyxml2::XMLElement *pElem, tinyxml2::XMLDocument &doc);
};
