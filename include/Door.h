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

#include "Object.h"
#include "MapLinkInterface.h"

class CMap;

class CDoor : public CObject, public CMapLinkInterface {
private:
	const Ogre::Radian m_rYaw;
	const Ogre::Vector3 m_vPosition;
public:
	CDoor(Ogre::SceneNode *pDoorNode, CMapManager &mapManager, CObjectManager &objectManager, const UserData &userData, const Ogre::String &id);
	virtual ~CDoor();

private:

	//! player interacts with object active
	bool interactOnKeypress(EInteractType eInteractType, const Ogre::Vector3 &vInteractDir);
	//! if its a object that interacts on collision (e.g. items, oder a link)
	//bool interactOnCollision() {return false;}
};
