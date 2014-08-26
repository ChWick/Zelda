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

#include "StdAfx.h"
#include "Door.h"
#include "MapManager.h"
#include "CameraController.h"

CDoor::CDoor(Ogre::SceneNode *pDoorNode, CMapManager &mapManager, CObjectManager &objectManager, const UserData &userData, const Ogre::String &id)
	: CObject(objectManager, DOOR_OBJECT),
	CMapLinkInterface(mapManager, MapLocalisationData(userData.getStringUserData("link_path"), userData.getStringUserData("link_name")), userData.getStringUserData("link_id"), id),
	m_vPosition(pDoorNode->getPosition()),
	m_rYaw(pDoorNode->getOrientation().getYaw()) {
	m_PlayerPosition = pDoorNode->getPosition() + 0.5f * pDoorNode->getOrientation().zAxis();
	m_PlayerLookDirection = -pDoorNode->getOrientation().zAxis();
	m_CameraYaw = pDoorNode->getOrientation().getYaw().valueRadians();
	m_CameraPitch = -Ogre::Math::PI * 0.1;
}
CDoor::~CDoor() {
}
bool CDoor::interactOnKeypress(EInteractType eInteractType, const Ogre::Vector3 &vInteractDir) {
	if (eInteractType == IT_USE) {
		switch (getInteractDirection(vInteractDir, m_rYaw.valueRadians())) {
		case ID_FRONT:
			activateLink();
			CMapManager::getSingleton().getCameraController()->moveCameraToTarget(m_vPosition - vInteractDir * 3 + Ogre::Vector3::UNIT_Y * 2, m_vPosition);
			break;
		}
		return true;
	}
	return false;
}
