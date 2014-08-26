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
#include "LiftableObject.h"
#include "ObjectManager.h"
#include "PhysicsManager.h"
#include "Map.h"
#include "Util.h"
#include "Damage.h"
#include "PickableObject.h"
#include "UserData.h""

const int LO_DAMAGE_TYPES[CLiftableObject::LOT_SIZE] = {
	CDamage::DMG_WORLD | CDamage::DMG_SWORD | CDamage::DMG_FIRE,		// LOT_GREEN_BUSH
	CDamage::DMG_WORLD,								// LOT_LIGHT_STONE
	CDamage::DMG_WORLD,								// LOT_SIGN
	CDamage::DMG_WORLD | CDamage::DMG_HAMMER,                           // LOT_VASE
};
const int LO_BLOCK_TYPES[CLiftableObject::LOT_SIZE] = {
	CDamage::DMG_ALL ^ CDamage::DMG_ARROW, 		// LOT_GREEN_BUSH
	CDamage::DMG_ALL,								// LOT_LIGHT_STONE
	CDamage::DMG_ALL,								// LOT_SIGN
	CDamage::DMG_ALL,                               // LOT_VASE
};
const int LO_DEFAULT_INNER_OBJECTS[CLiftableObject::LOT_SIZE] = {
};

CLiftableObject::CLiftableObject(Ogre::SceneNode *pNode, CObjectManager &objectManager, btCollisionObject *pCollisionObject, ELiftableObjectTypes eLiftableObjectType, bool bRecreateOnLift, const UserData &userData)
	: CObject(objectManager,LIFTABLE_OBJECT, pCollisionObject, userData.getStringUserData("id")), m_eLiftableObjectType(eLiftableObjectType), m_bRecreateOnLift(bRecreateOnLift) {
    m_eState = LOS_AT_DEFAULT_LOCATION;
    m_InnerObject = InnerObjectGenerator::getInnerObjectByName(userData.getStringUserData("innerObject", "default"));

    switch (eLiftableObjectType) {
    case LOT_VASE:
        m_uiInnerObjectCreateEvent = IOCE_DESTROYED;
        break;
    case LOT_SIGN:
    case LOT_BOMB:
        m_uiInnerObjectCreateEvent = IOCE_NEVER;
        break;
    default:
        m_uiInnerObjectCreateEvent = IOCE_DESTROYED | IOCE_LIFT;
        break;
    }
}
CLiftableObject::~CLiftableObject() {
}
bool CLiftableObject::interactOnKeypress(EInteractType eInteractType, const Ogre::Vector3 &vInteractDir) {
	if (eInteractType == IT_USE) {
		return false;
	}
	else if (eInteractType == IT_ATTACK) {
		if (m_eState == LOS_LIFTED) {
			return throwObject();
		}
		return true;
	}
	else if (eInteractType == IT_LIFT) {
		if (m_eState == LOS_AT_DEFAULT_LOCATION) {
			return lift();
		}
		return false;
	}

	return false;
}
CHitableInterface::EReceiveDamageResult CLiftableObject::receiveDamage(const CDamage &damage) {
	if (LO_DAMAGE_TYPES[m_eLiftableObjectType] & damage.getDamageType()) {
		// check if i can be hit by this damage type
		destroy();
		return RDR_ACCEPTED;
	}
	else if (LO_BLOCK_TYPES[m_eLiftableObjectType] & damage.getDamageType()){
        return RDR_BLOCKED;
    }
    else {
		return RDR_REJECTED;
	}
}
bool CLiftableObject::lift() {
	assert(m_eState == LOS_AT_DEFAULT_LOCATION);

	m_eState = LOS_LIFTED;
	sendCreateInnerObjectEvent(IOCE_LIFT);


    btRigidBody *pRB = btRigidBody::upcast(m_pCollisionObject);
    btRigidBody *pNewRB(NULL);
    assert(pRB);
    if (m_bRecreateOnLift) {

        Ogre::String meshFile;

        // create dynamic close of pRB -> pNewRB, and delete the other one in the next frame

        auto pSN = m_ObjectManager.getMap().getRootSceneNode()->createChildSceneNode();

        CPhysicsCollisionObject *pCO = m_ObjectManager.getMap().getPhysicsManager()->findCollisionShape(m_pCollisionObject->getCollisionShape());
        auto shape = pCO->getShape();
        meshFile = m_ObjectManager.getMap().getPhysicsManager()->getCollisionShapeKey(*pCO);

        float mass = 100.0f;
        btVector3 inertia;
        shape->calculateLocalInertia(mass, inertia);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,
            new BtOgre::RigidBodyState(pSN, m_pCollisionObject->getWorldTransform(), btTransform(btQuaternion::getIdentity(), BtOgre::Convert::toBullet(-pCO->getOffset()))),
                shape,inertia);


        pNewRB = new btRigidBody(rbInfo);
        // object now belongs to player, so dont interact with each other


        // create the entity

        Ogre::Entity *pEnt = m_ObjectManager.getMap().getSceneManager()->createEntity(meshFile);
        pSN->attachObject(pEnt);

        //m_ObjectManager.getMap().destroyObjectsEntity(this, NULL, BtOgre::Convert::toOgre(pNewRB->getWorldTransform().getOrigin()));

        //m_ObjectManager.getMap().getPhysicsManager()->createLater(const_cast<btRigidBody*>(pNewRB));
        //m_ObjectManager.getMap().getPhysicsManager()->deleteLater(const_cast<btRigidBody*>(pRB));
        delete pRB->getMotionState();
        delete pRB;

        m_pCollisionObject = pNewRB;
        setAsUserPointer(pNewRB);
    }
    else {
        pNewRB = pRB;
    }
    assert(pNewRB);
    m_ObjectManager.getMap().getPhysicsManager()->getWorld()->removeCollisionObject(const_cast<btRigidBody*>(pRB));
    m_ObjectManager.getMap().getPhysicsManager()->getWorld()->addRigidBody(pNewRB, COL_DAMAGE_P, COL_STATIC | COL_WALL | COL_CHARACTER_N);

    // is there an linked object to delete on lift?
    Ogre::String sRemoveObjectOnLift(m_UserData.getStringUserData("removeObjectOnLift"));
    if (sRemoveObjectOnLift.length() > 0) {
        m_ObjectManager.getObjectBySafeStateId(sRemoveObjectOnLift)->suicide();
    }

	return true;
}
bool CLiftableObject::throwObject() {
	assert(m_eState == LOS_LIFTED);

	m_eState = LOS_THROWN;

	return true;
}
void CLiftableObject::destroy() {
	m_ObjectManager.getMap().getPhysicsManager()->deleteLater(m_pCollisionObject);
	if (m_eState == LOS_AT_DEFAULT_LOCATION) {
		m_ObjectManager.getMap().destroyObjectsEntity(this, NULL, BtOgre::Convert::toOgre(m_pCollisionObject->getWorldTransform().getOrigin()));
	}
	else {
		if (btRigidBody::upcast(m_pCollisionObject)->getMotionState()) {
			destroySceneNode(dynamic_cast<BtOgre::RigidBodyState*>(btRigidBody::upcast(m_pCollisionObject)->getMotionState())->getNode());
			dynamic_cast<BtOgre::RigidBodyState*>(btRigidBody::upcast(m_pCollisionObject)->getMotionState())->setNode(NULL);
			delete btRigidBody::upcast(m_pCollisionObject)->getMotionState();
			btRigidBody::upcast(m_pCollisionObject)->setMotionState(NULL);
		}
	}

    sendCreateInnerObjectEvent(IOCE_DESTROYED);
	m_ObjectManager.deleteObject(this);
}
bool CLiftableObject::interactOnCollision(const btCollisionObject *pPartner) {
	// only interact if i am thrown, then delete me
	if (getState() == CLiftableObject::LOS_THROWN) {
		this->receiveDamage(CDamage(CDamage::DMG_WORLD));
		if (pPartner->getUserPointer()) {
            CPhysicsUserPointer *pPP = static_cast<CPhysicsUserPointer*>(pPartner->getUserPointer());
            if (pPP) {
                CHitableInterface *pHI = dynamic_cast<CHitableInterface*>(pPP);
                if (pHI) {
                    Ogre::LogManager::getSingleton().logMessage("Liftable object hit other object.");
                    pHI->receiveDamage(CDamage(CDamage::DMG_WORLD,
                                               BtOgre::Convert::toOgre(btRigidBody::upcast(m_pCollisionObject)->getLinearVelocity()),
                                               HP_ONE_HEART));
                }
            }
		}

		return true;
	}

	return false;
}
void CLiftableObject::sendCreateInnerObjectEvent(unsigned int uiEvent) {
    if (m_uiInnerObjectCreateEvent & uiEvent) {
		InnerObjectGenerator::createInnerObject(m_ObjectManager.getMap(), BtOgre::Convert::toOgre(m_pCollisionObject->getWorldTransform().getOrigin()), m_InnerObject);
		m_uiInnerObjectCreateEvent = IOCE_NEVER;
    }
}
