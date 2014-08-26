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

#ifndef _MAINMAP_H_
#define _MAINMAP_H_

#include "StdAfx.h"
#include "Map.h"

using namespace Forests;

/// Class that handles the main map
/**
  *
  */
class CMainMap : public CMap {
private:
	PagedGeometry *m_pTreesPagedGeometry;
	TreeLoader3D *m_pTreeLoader;
	PagedGeometry *m_pBushPagedGeometry;
	TreeLoader3D *m_pBushLoader;
public:
	CMainMap(CMapManager &mapManager, Ogre::SceneManager *pSceneManager, const MapLocalisationData &localisationData);
	~CMainMap();

	void update(const double &tpf);

	virtual void start();
	virtual void stop();

	// Callbacks
	virtual void staticObjectAdded(Ogre::Entity *pEnt, Ogre::SceneNode *pParent);
	virtual void destroyObjectsEntity(CObject *pObject, Ogre::Entity *pEntity, const Ogre::Vector3 &vPos);

protected:
	// internal functions
	virtual void preLoadIntern();
	virtual void postLoadIntern();
	virtual void preUnloadIntern();
	virtual void postUnloadIntern();
	virtual void updateIntern(const Ogre::Real &tpf);

	// paged geometry
	void treeAdded(Ogre::Entity *pTree, const Ogre::Vector3 &vPos, const Ogre::Degree &yaw, Ogre::Real scale, void *pUserData);
	void bushAdded(Ogre::Entity *pBush, const Ogre::Vector3 &vPos, const Ogre::Degree &yaw, Ogre::Real scale, void *pUserData);
};

#endif
