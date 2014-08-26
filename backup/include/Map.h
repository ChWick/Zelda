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
#include "DotSceneLoaderCallback.h"
#include "ObjectManager.h"
#include "DotsceneLoader.h"
#include "MapUserDataParser.h"
#include "MapLocalisationData.h"

class CMapManager;
class CPlayer;
class CPhysicsManager;
class CCharacter;

/// Base definition for a map
/**
 *  A dungeon and the MainMap will inherit this class.
 *  Loading, inherits DotSceneLoaderCallback for StaticGeometry, creating doors, etc.
 *
 */
class CMap : public CDotSceneLoaderCallback {
public:
private:
	Ogre::StaticGeometry *m_pStaticGeometry;		//!< Static geometry of the map
	Ogre::DotSceneLoader m_DotSceneLoader;
	CMapUserDataParser m_MapUserDataParser;
	std::list<CCharacter*> m_lCharacters;			//!< List of the characters in the map
	std::list<CCharacter*> m_lCharactersToDestroy;	//!< List of the characters to be destroyed in the next frame
	std::list<Ogre::Light*> m_lLights;				//!< List of all light in the map to store for destruction

protected:
	Ogre::SceneManager *m_pSceneManager;
	Ogre::SceneNode *m_pRootSceneNode;
	CObjectManager m_ObjectManager;
	CMapManager &m_MapManager;
	CPhysicsManager *m_pPhysicsManager;
	CPlayer *m_pPlayer;

    const MapLocalisationData m_LocalisationData;      //!< data for finding the mpa on the filesystem
	bool m_bLiftableObjectsMustRecreate;            //!< If using static geometry, liftable objects are not static, on main map, the have to be recreated on lifting


public:
	CMap(CMapManager &mapManager, Ogre::SceneManager *pSceneManager, const MapLocalisationData &localisationData);
	virtual ~CMap();

	void load();
	void unload();
	void update(const Ogre::Real &tpf);

	virtual void start();
	virtual void stop();

	void playerEnters(CPlayer *pPlayer, const Ogre::String &entryPoint);
	virtual void destroyObjectsEntity(CObject *pObject, Ogre::Entity *pEntity, const Ogre::Vector3 &vPos);

	void destroyLater(CCharacter *pCharacter) {assert(pCharacter); m_lCharactersToDestroy.push_back(pCharacter);}

    const MapLocalisationData &getLocalisationData() const {return m_LocalisationData;}
    const Ogre::String &getName() const {return m_LocalisationData.m_sName;}
    const Ogre::String &getPath() const {return m_LocalisationData.m_sPath;}
    const Ogre::String &getResourceGroup() const {return m_LocalisationData.m_sName;}

	CMapManager &getMapManager() {return m_MapManager;}
	CObjectManager &getObjectManager() {return m_ObjectManager;}
	const CObjectManager &getObjectManager() const {return m_ObjectManager;}
	CPhysicsManager *getPhysicsManager() const {return m_pPhysicsManager;}
	CPlayer *getPlayer() {return m_pPlayer;}
	std::list<CCharacter *> getCharacters() const {return m_lCharacters;}
	void addAndInitialiseNewChar(CCharacter *pChar);

	Ogre::SceneManager *getSceneManager() {return m_pSceneManager;}
	Ogre::SceneNode *getRootSceneNode() {return m_pRootSceneNode;}

	std::list<CHitableInterface *> getHitableInterfacesInSphere(const Ogre::Sphere &sphere) const;
	// callback functions of CDotSceneLoaderCallback, can be overwritten
	virtual void preEntityAdded(UserData &userData);
	virtual void postEntityAdded(Ogre::Entity *pEntity, Ogre::SceneNode *pParent, btRigidBody *pRigidBody, const UserData &userData);
	virtual void staticObjectAdded(Ogre::Entity *pEntity, Ogre::SceneNode *pParent);
	virtual void lightAdded(Ogre::Light *pLight) {m_lLights.push_back(pLight);}
protected:
	// virtual internal functions for individual handling of map data
	virtual void preLoadIntern() {}
	virtual void postLoadIntern() {}
	virtual void preUnloadIntern() {}
	virtual void postUnloadIntern() {}
	virtual void updateIntern(const Ogre::Real &tpf) {}

private:
	void processCollisionCheck();
	void objectCollided(const btCollisionObject *pColObj, const btCollisionObject *pPartner);
};
