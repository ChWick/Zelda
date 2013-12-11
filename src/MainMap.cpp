#include "StdAfx.h"
#include "Object.h"
#include "MainMap.h"

CMainMap::CMainMap(CMapManager &mapManager, Ogre::SceneManager *pSceneManager, const MapLocalisationData &localisationData)
	: CMap(mapManager, pSceneManager, localisationData) {
	m_pTreesPagedGeometry = NULL;
	m_pTreeLoader = NULL;
	m_pBushPagedGeometry = NULL;
	m_pBushLoader = NULL;

	m_bLiftableObjectsMustRecreate = true;
}
CMainMap::~CMainMap() {
}
void CMainMap::start() {
	auto pCam = m_pSceneManager->getCamera("GameCam");

	// setup environment
	// ====================================================
	// set fog
	Ogre::ColourValue fadeColour(0.9f, 0.9f, 0.9f);
	m_pSceneManager ->setFog(Ogre::FOG_LINEAR, fadeColour, 0.0, 70, 130);
	pCam->getViewport()->setBackgroundColour(fadeColour);
	//pCam->setFarClipDistance(130);

	// set skyplane
	Ogre::Plane plane;
	plane.d = 10;
	plane.normal = Ogre::Vector3::NEGATIVE_UNIT_Y;

	m_pSceneManager->setSkyPlane(true, plane, "Skyplane/CloudySky", 10, 20, true, 0.5, 150, 150);


	// show paged geometry

	m_pTreesPagedGeometry->setVisible(true);
	m_pBushPagedGeometry->setVisible(true);
	m_pTreesPagedGeometry->update();
	m_pBushPagedGeometry->update();

	// call parent method
	CMap::start();
}
void CMainMap::stop() {
	// unset environment
	m_pSceneManager->setFog();
	m_pSceneManager->setSkyPlaneEnabled(false);

	// hide paged geometry
	m_pTreesPagedGeometry->setVisible(false);
	m_pBushPagedGeometry->setVisible(false);
	m_pTreesPagedGeometry->update();
	m_pBushPagedGeometry->update();

	// call parent method
	CMap::stop();
}
void CMainMap::preLoadIntern() {
	auto pCam = m_pSceneManager->getCamera("GameCam");

	// setup OgrePagedGeometry
	// =======================

	// Trees
	m_pTreesPagedGeometry = new PagedGeometry(pCam);
	m_pTreesPagedGeometry->setShadersEnabled(false);
	m_pTreesPagedGeometry->setTempDir("temp/");
	m_pTreesPagedGeometry->setPageSize(25);
	m_pTreesPagedGeometry->setBounds(Forests::TBounds(-200, -200, 200, 200));
	m_pTreesPagedGeometry->addDetailLevel<BatchPage>(50, 10);
	m_pTreesPagedGeometry->addDetailLevel<ImpostorPage>(300, 10);

	m_pTreeLoader = new TreeLoader3D(m_pTreesPagedGeometry, TBounds(-250, -250, 250, 250));
	m_pTreesPagedGeometry->setPageLoader(m_pTreeLoader);

	// Bushed
	m_pBushPagedGeometry = new PagedGeometry(pCam);
	m_pBushPagedGeometry->setShadersEnabled(false);
	m_pBushPagedGeometry->setTempDir("temp/");
	m_pBushPagedGeometry->setPageSize(25);
	m_pBushPagedGeometry->setBounds(Forests::TBounds(-200, -200, 200, 200));
	m_pBushPagedGeometry->addDetailLevel<BatchPage>(30, 10);
	m_pBushPagedGeometry->addDetailLevel<ImpostorPage>(80, 10);

	m_pBushLoader = new TreeLoader3D(m_pBushPagedGeometry, TBounds(-250, -250, 250, 250));
	m_pBushPagedGeometry->setPageLoader(m_pBushLoader);
}
void CMainMap::postLoadIntern() {
}
void CMainMap::preUnloadIntern() {
	if (m_pTreeLoader) {delete m_pTreeLoader; m_pTreeLoader = NULL;}
	if (m_pTreesPagedGeometry) {delete m_pTreesPagedGeometry; m_pTreesPagedGeometry = NULL;}
	if (m_pBushLoader) {delete m_pBushLoader; m_pBushLoader = NULL;}
	if (m_pBushPagedGeometry) {delete m_pBushPagedGeometry; m_pBushPagedGeometry = NULL;}
}
void CMainMap::postUnloadIntern() {
}
void CMainMap::updateIntern(const Ogre::Real &tpf) {
	assert(m_pTreesPagedGeometry);
	assert(m_pBushPagedGeometry);
	m_pTreesPagedGeometry->update();
	m_pBushPagedGeometry->update();
}
void CMainMap::staticObjectAdded(Ogre::Entity *pEntity, Ogre::SceneNode *pParent) {
	const Ogre::String &meshFile(pEntity->getMesh()->getName());
	// let's see where to add this
	if (meshFile == "TreeBush_Green.mesh" || meshFile == "TreeBush_Pink.mesh" || meshFile == "TreeCut.mesh"
        || meshFile == "FenceStake.mesh" || meshFile == "FenceHoriz.mesh" || meshFile == "BigStone.mesh"
        || meshFile == "StoneStatue.mesh" || meshFile == "GreenTree.mesh" || meshFile == "Tree.mesh"
        || meshFile == "StoneStatue.mesh" || meshFile == "RockPeak.mesh" || meshFile == "Picket4.mesh"
        || meshFile == "Picket3.mesh") {
		// list of meshes to add to the "far"-paged geometry (alias tree)
		//treeAdded(pEntity, pParent->_getDerivedPosition(), pParent->_getDerivedOrientation().getYaw(), 1, NULL);

	}
	else if (meshFile == "Sign.001.mesh" || meshFile == "Sign.mesh" || meshFile == "SmallPot.mesh"
        || meshFile == "BigPot.mesh" || meshFile == "Grass.mesh" || meshFile == "LightStone.mesh"
        || meshFile == "GreenBush.mesh"	|| meshFile == "Bush.mesh" || meshFile == "Mushroom.mesh"
        || meshFile == "Flowers.mesh") {
		// list of meshes to add to the "near"-paged geometry (alias bush)
		//bushAdded(pEntity, pParent->_getDerivedPosition(), pParent->_getDerivedOrientation().getYaw(), 1, NULL);
	}
	else {
		// use normal static geometry
		CMap::staticObjectAdded(pEntity, pParent);
	}
}
void CMainMap::destroyObjectsEntity(CObject *pObject, Ogre::Entity *pEntity, const Ogre::Vector3 &vPos) {
	assert (pObject);
	if (pObject->getObjectType() == CObject::LIFTABLE_OBJECT) {
		// it is something inside bush loader
		m_pBushLoader->deleteTrees(vPos, 0.2f, pEntity);
		m_pBushPagedGeometry->update();
	}
}
void CMainMap::treeAdded(Ogre::Entity *pTree, const Ogre::Vector3 &vPos, const Ogre::Degree &yaw, Ogre::Real scale, void *pUserData) {
	assert(pTree);
	assert(m_pTreeLoader);
	m_pTreeLoader->addTree(pTree, vPos, yaw, scale, pUserData);
}
void CMainMap::bushAdded(Ogre::Entity *pBush, const Ogre::Vector3 &vPos, const Ogre::Degree &yaw, Ogre::Real scale, void *pUserData) {
	assert(pBush);
	assert(m_pBushLoader);
	m_pBushLoader->addTree(pBush, vPos, yaw, scale, pUserData);
}
