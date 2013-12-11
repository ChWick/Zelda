#pragma once

#include "StdAfx.h"
#include "PlayerInteractiveInterface.h"
#include "Map.h"

class CMap;
//! Klasse die einen eintrittspunkt in eine Karte definiert.
/*!
Wenn der spieler solch einen Eintrittspunkt betritt, dann werden Türen geschlossen, etc.
Nicht zu verwechseln mit Portalen die aufschluss darüber geben welche Physik verwendet wird.
*/
class CMapEntry : public CPlayerInteractiveAABox {
protected:
	Ogre::SceneNode *m_pSceneNode;
	CMap *m_pMap;
public:
	void setSceneNode(Ogre::SceneNode *pSceneNode) {m_pSceneNode = pSceneNode;}
	Ogre::SceneNode *getSceneNode() {return m_pSceneNode;}

	CMapEntry(CMap *pMap, const Ogre::AxisAlignedBox &box)
		: CPlayerInteractiveAABox(MAP_ENTRY, &m_pSceneNode, box), m_pMap(pMap) {
	}
	virtual void activate() {
		m_pMap->playerEntersInteractiveRegion();
	}
	virtual void update(const Ogre::Real tpf) {
#if ENABLE_SHOW_INTERACTIVE_AREA_SPHERES_DEBUG
		if (g_bDebugPlayerInteractiveArea) {
			drawDebug();
		}
#endif
	}
};