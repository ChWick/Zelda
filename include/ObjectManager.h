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
