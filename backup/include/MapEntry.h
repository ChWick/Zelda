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