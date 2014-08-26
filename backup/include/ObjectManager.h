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

#include "StdAfx.h"

class CMap;
class CObject;
class CPlayer;

class CObjectManager {
private:
	Ogre::list<CObject*>::type m_ObjectsList;
	Ogre::list<CObject*>::type m_ObjectsToAdd;
	Ogre::list<CObject*>::type m_ObjectsToDelete;

	CMap &m_Map;

public:
	CObjectManager(CMap &map);
	~CObjectManager();

	void update(const Ogre::Real fTime);

	void start();

	CMap &getMap() {return m_Map;}
	CPlayer *getPlayer();

	const Ogre::list<CObject*>::type &getObjects() const {return m_ObjectsList;}
	CObject *getObjectBySafeStateId(const Ogre::String &id) const;
	void deleteAllObjects();
	void addObject(CObject *pObject) {
#ifdef _DEBUG
		// check if object exists already
		for (auto pExistingObject : m_ObjectsToAdd) {
            assert(pExistingObject != pObject);
        }
		for (auto pExistingObject : m_ObjectsList) {
            assert(pExistingObject != pObject);
        }
#endif
		// add to add list
		m_ObjectsToAdd.push_back(pObject);
	}

	//! Function that will delete the given object in the next frame
	/**
	  * this will allow objects to delete themselves
	  */
	void deleteObject(CObject *pObject) {
        for (auto pExistingObject : m_ObjectsToDelete) {
            if (pExistingObject == pObject) {
                return;
            }
        }
		m_ObjectsToDelete.push_back(pObject);
	}

	void destroyObjectOnContact(CObject *pObject);
};
