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
#include "Chest.h"
#include "ObjectManager.h"
#include "Map.h"
#include "PhysicsManager.h"
#include "GameState.h"
#include "TextLoader.h"
#include "MapManager.h"
#include "CameraController.h"
#include "PlayerData.h"

const Ogre::Real CHEST_OPEN_SPEED = 2.0f;
const Ogre::Real CHEST_INNER_OBJECT_MOVE_UP_SPEED = 1.0f;
const Ogre::Real CHEST_MAX_ANGLE = Ogre::Math::HALF_PI;

CChest::CChest(Ogre::SceneNode *pNode, CObjectManager &objectManager, btCollisionObject *pCollisionObject, EChestType eChestType, const Ogre::String &id)
	: CObject(objectManager, CHEST_OBJECT, pCollisionObject, id), m_eChestType(eChestType) {

	m_eChestState = CS_CLOSED;
	//setInnerObject(&innerObject);
	m_pInnerObject = NULL;

	m_pLidSceneNode = objectManager.getMap().getRootSceneNode()->createChildSceneNode();
	m_pLidSceneNode->setPosition(pNode->getPosition());
	m_pLidSceneNode->setOrientation(pNode->getOrientation());
	m_pLidSceneNode->setScale(pNode->getScale());
	m_vInnerObjectPos = m_pLidSceneNode->getPosition();
	btCollisionShape *pColShape(0);
	switch (m_eChestType) {
	case CT_SMALL:
		m_pLidSceneNode->attachObject(pNode->getCreator()->createEntity("Chest.Small.Upper.mesh"));
		m_pLidSceneNode->translate(0, 0.37465f, -0.30862f, Ogre::Node::TS_LOCAL);
		m_vInnerObjectPos += m_pLidSceneNode->getOrientation() * Ogre::Vector3(0, 0.15f, 0);
		pColShape = new btBoxShape(btVector3(0.3f, 0.08f, 0.2f));
		m_vPhysicsOffset = Ogre::Vector3(0, 0.15f, 0.25f);
		break;
	default:
		throw Ogre::Exception(0, "Unknown chest type", __FILE__);
		break;
	}
	m_pLidPhysics = new btRigidBody(0, new btDefaultMotionState(), pColShape);
	m_pLidPhysics->getWorldTransform().setOrigin(BtOgre::Convert::toBullet(m_pLidSceneNode->getPosition() + m_pLidSceneNode->getOrientation() * m_vPhysicsOffset));
	m_pLidPhysics->getWorldTransform().setRotation(BtOgre::Convert::toBullet(m_pLidSceneNode->getOrientation()));

	m_ObjectManager.getMap().getPhysicsManager()->getWorld()->addRigidBody(m_pLidPhysics, COL_STATIC, MASK_STATIC_COLLIDES_WITH);

	// check for safe state
	if (id.length() > 0) {
        EItemSaveState iss = CPlayerData::getSingleton().getMapItemState(m_ObjectManager.getMap().getName(), id, ISS_CLOSED);
        if (iss == ISS_OPENED) {
            // open chest as initial state
            m_eChestState = CS_OPENED;
            m_pLidSceneNode->pitch(Ogre::Radian(-CHEST_MAX_ANGLE));
            m_pLidPhysics->getWorldTransform().setOrigin(BtOgre::Convert::toBullet(m_pLidSceneNode->getPosition() + m_pLidSceneNode->getOrientation() * m_vPhysicsOffset));
            m_pLidPhysics->getWorldTransform().setRotation(BtOgre::Convert::toBullet(m_pLidSceneNode->getOrientation()));
        }
    }
}
CChest::~CChest() {
	m_ObjectManager.getMap().getPhysicsManager()->getWorld()->removeRigidBody(m_pLidPhysics);
	delete m_pLidPhysics->getMotionState();
	delete m_pLidPhysics->getCollisionShape();
	delete m_pLidPhysics;
	m_pLidPhysics = 0;
}
void CChest::update(Ogre::Real tpf) {
	if (m_eChestState == CS_OPENING) {
		m_fOpenPitch += tpf * CHEST_OPEN_SPEED;
		m_pLidSceneNode->pitch(Ogre::Radian(-tpf * CHEST_OPEN_SPEED));

		if (m_fOpenPitch >= CHEST_MAX_ANGLE) {
			m_eChestState = CS_OPENED;

			CHUD::getSingleton().showText(CTextLoader::getSingleton().getInnerObjectTexts(m_pInnerObject->getPickableObjectType()));
		}


		m_pLidPhysics->getWorldTransform().setOrigin(BtOgre::Convert::toBullet(m_pLidSceneNode->getPosition() + m_pLidSceneNode->getOrientation() * m_vPhysicsOffset));
		m_pLidPhysics->getWorldTransform().setRotation(BtOgre::Convert::toBullet(m_pLidSceneNode->getOrientation()));

		m_vInnerObjectPos.y += CHEST_INNER_OBJECT_MOVE_UP_SPEED * tpf;
		m_pInnerObject->getCollisionObject()->getWorldTransform().setOrigin(BtOgre::Convert::toBullet(m_vInnerObjectPos));

	}
	else if (m_eChestState == CS_OPENED && m_pInnerObject) {
		m_pInnerObject->getCollisionObject()->getWorldTransform().setOrigin(BtOgre::Convert::toBullet(m_vInnerObjectPos));
		if (!CGameState::getSingleton().isTextboxActive()) {

			m_pInnerObject->pickupObject();
			CPlayerData::getSingleton().setMapItemState(ISS_OPENED, m_ObjectManager.getMap().getName(), m_SafeStateID);
			unlockInteraction();
			m_pInnerObject = NULL;

		}
	}
}
bool CChest::interactOnKeypress(EInteractType eInteractType, const Ogre::Vector3 &vInteractDir) {
	if (eInteractType == IT_USE && m_eChestState == CS_CLOSED) {
		if (getInteractDirection(vInteractDir, m_pLidSceneNode->getOrientation().getYaw().valueRadians()) == ID_FRONT) {
			m_eChestState = CS_OPENING;
			m_fOpenPitch = 0;
			m_pInnerObject = InnerObjectGenerator::createInnerObject(m_ObjectManager.getMap(), m_vInnerObjectPos, getInnerObject());
			btRigidBody::upcast(m_pInnerObject->getCollisionObject())->setLinearFactor(btVector3(0, 0, 0));
			btRigidBody::upcast(m_pInnerObject->getCollisionObject())->setLinearVelocity(btVector3(0, 0, 0));

			const Ogre::Vector3 vPos(m_vInnerObjectPos);
            CMapManager::getSingleton().getCameraController()->moveCameraToTarget(vPos - vInteractDir * 3 + Ogre::Vector3::UNIT_Y * 1, vPos);

			lockInteraction();
		}
		return true;
	}
	return false;
}
