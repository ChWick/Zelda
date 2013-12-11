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
