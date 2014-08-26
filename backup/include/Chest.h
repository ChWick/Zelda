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

#ifndef _CHEST_H_
#define _CHEST_H_

#include "Object.h"
#include "PickableObject.h"
#include "HUD.h"
#include "InnerObjectContainerInterface.h"

class CChest : public CObject, public CGameStateChanger, public InnerObjectContainerInterface {
public:
	enum EChestType {
		CT_SMALL,
		CT_BIG,
	};
	enum EChestState {
		CS_CLOSED,
		CS_OPENING,
		CS_OPENED,
	};
private:
	const EChestType m_eChestType;
	EChestState m_eChestState;
	btRigidBody *m_pLidPhysics;
	Ogre::SceneNode *m_pLidSceneNode;

	CPickableObject *m_pInnerObject;

	Ogre::Real m_fOpenPitch;
	Ogre::Vector3 m_vInnerObjectPos;
	Ogre::Vector3 m_vPhysicsOffset;
public:
	CChest(Ogre::SceneNode *pNode, CObjectManager &objectManager, btCollisionObject *pCollisionObject, EChestType eChestType, const Ogre::String &id);
	~CChest();


	virtual void update(Ogre::Real tpf);

	virtual bool interactOnKeypress(EInteractType eInteractType, const Ogre::Vector3 &vInteractDir);
};

#endif
