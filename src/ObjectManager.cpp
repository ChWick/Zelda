#include "StdAfx.h"
#include "ObjectManager.h"
#include "Object.h"
#include "Map.h"

CObjectManager::CObjectManager(CMap &map)
	: m_Map(map) {
}
CObjectManager::~CObjectManager() {
	deleteAllObjects();
}
CPlayer *CObjectManager::getPlayer() {
    return m_Map.getPlayer();
}
void CObjectManager::deleteAllObjects() {
    for (auto pObject : m_ObjectsToAdd) {
        delete pObject;
	}
	m_ObjectsToAdd.clear();
    for (auto pObject : m_ObjectsList) {
        delete pObject;
	}
	m_ObjectsList.clear();
    for (auto pObject : m_ObjectsToDelete) {
        delete pObject;
	}
	m_ObjectsToDelete.clear();
}
void CObjectManager::start() {
	// add all new objects
	while (m_ObjectsToAdd.size() > 0) {
		m_ObjectsList.push_back(m_ObjectsToAdd.front());
		m_ObjectsToAdd.pop_front();
	}
}
void CObjectManager::update(Ogre::Real fTime) {
    for (auto pObject : m_ObjectsList) {
        pObject->update(fTime);
	}
    /*for (auto it = m_MovingObjects.begin(); it != m_MovingObjects.end();) {
		bool increment = true;
        if (((Ogre::PCZSceneNode*)(*it).pObject->getSceneNode())->getHomeZone() != m_Map.getZone()) {
			// object is now in an other map
            (*it).pLastZone = ((Ogre::PCZSceneNode*)(*it).pObject->getSceneNode())->getHomeZone();
			// new map?
            CMap *pNewMap = m_Map.getDungeon().getMapByZone((*it).pLastZone);
			if (pNewMap) {
                CObject *pNewObject = (*it).pObject->moveToMap(pNewMap);
                m_pPlayer->objectChangedMap((*it).pObject, pNewObject);
				it = m_MovingObjects.erase(it);
				increment = false;
			}
		}
		if (increment) {it++;}
	}*/
	while (m_ObjectsToAdd.size() > 0) {
		m_ObjectsList.push_back(m_ObjectsToAdd.front());
		m_ObjectsToAdd.pop_front();
	}
	while (m_ObjectsToDelete.size() > 0) {
		m_ObjectsList.remove(m_ObjectsToDelete.front());
        /*for (auto it = m_MovingObjects.begin(); it != m_MovingObjects.end(); it++) {
			if ((*it).pObject == m_ObjectsToDelete.front()) {
				m_MovingObjects.erase(it);
				break;
			}
		}*/
		delete m_ObjectsToDelete.front();
		m_ObjectsToDelete.pop_front();
	}
}
CObject *CObjectManager::getObjectBySafeStateId(const Ogre::String &id) const {
    if (id.length() == 0) {return NULL;}

    for (CObject *pObject : m_ObjectsList) {
        if (pObject->getSafeStateID() == id) {
            return pObject;
        }
    }
    for (CObject *pObject : m_ObjectsToAdd) {
        if (pObject->getSafeStateID() == id) {
            return pObject;
        }
    }
    return NULL;
}
