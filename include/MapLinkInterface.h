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

#include "MapManager.h"
#include "MapLocalisationData.h"

class CMapLinkInterface {
public:
    enum ECamPos {
        CP_UNSET,
        CP_TOP,
        CP_REAR,
        CP_USER,
    };
    static ECamPos parseCamPos(const Ogre::String &sCamPos) {
        Ogre::String s = sCamPos;
        Ogre::StringUtil::toLowerCase(s);
        if (s == "top") {return CP_TOP;}
        else if (s == "rear") {return CP_REAR;}
        else if (s == "user") {return CP_USER;}

        throw Ogre::Exception(__LINE__, s + " cannot be parsed as cam pos", __FILE__);
    }
protected:
	const MapLocalisationData m_NextMap;
	const Ogre::String m_NextID;
	const Ogre::String m_ID;

	CMapManager &m_MapManager;

    const ECamPos m_eCamPos;
	Ogre::Real m_CameraPitch, m_CameraYaw;
	Ogre::Real m_fCameraDistance;
	Ogre::Vector3 m_PlayerPosition;
	Ogre::Vector3 m_PlayerLookDirection;
public:
	CMapLinkInterface(CMapManager &mapManager, const MapLocalisationData &nextMap, const Ogre::String &nextId, const Ogre::String &id, ECamPos camPos = CP_UNSET)
		: m_MapManager(mapManager), m_NextMap(nextMap),
		m_NextID(nextId), m_ID(id), m_CameraPitch(0),
		m_CameraYaw(0), m_eCamPos(camPos),
		m_fCameraDistance(2.0f) {

	}
	virtual ~CMapLinkInterface() {
	}

	void activateLink() {
		m_MapManager.changeMap(m_NextMap, m_NextID);
	}


    const Ogre::Real &getCameraDistance() const {return m_fCameraDistance;}
	const Ogre::Real &getCameraPitch() const {return m_CameraPitch;}
	const Ogre::Real &getCameraYaw() const {return m_CameraYaw;}
	const Ogre::Vector3 &getPlayerPosition() const {return m_PlayerPosition;}
	const Ogre::Vector3 &getPlayerLook() const {return m_PlayerLookDirection;}
	const Ogre::String &getID() const {return m_ID;}
	const Ogre::String &getNextID() const {return m_NextID;}
	const MapLocalisationData &getNextMap() const {return m_NextMap;}

protected:
	void initLink(const Ogre::SceneNode *pLinkNode, const Ogre::Real fCameraDistance = 2.0f) {
        switch (m_eCamPos) {
        case CP_TOP:
            m_fCameraDistance = fCameraDistance;
            m_CameraYaw = pLinkNode->getOrientation().getYaw().valueRadians();
            m_CameraPitch = -Ogre::Math::HALF_PI;
            break;
        case CP_REAR:
            m_fCameraDistance = fCameraDistance;
            m_CameraYaw = pLinkNode->getOrientation().getYaw().valueRadians();
            m_CameraPitch = 0;
            break;
        }

	}
};
