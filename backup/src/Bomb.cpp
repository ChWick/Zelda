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
#include "Bomb.h"
#include "Util.h"
#include "ObjectManager.h"
#include "Map.h"
#include "PhysicsManager.h"
#include "Person.h"
#include "BombListener.h"
#include "Damage.h"

const Ogre::Real BOMB_TIMER = 3.0f;
const Ogre::Real BOMB_COL_RADIUS = 0.25;
const Ogre::Real BOMB_MASS = 1.0f;
const Ogre::String BOMB_MESH = "Bomb.mesh";
const Ogre::Real BOMB_EXPLOSION_RADIUS = 1.0f;

CBomb::CBomb(Ogre::SceneNode *pParentNode, CObjectManager &objectManager, const Ogre::Vector3 &vPosition)
: CLiftableObject(pParentNode, objectManager, NULL, LOT_BOMB, false, UserData()), m_pListener(NULL) {
    m_pSceneNode = pParentNode->createChildSceneNode();
    m_pSceneNode->attachObject(pParentNode->getCreator()->createEntity(BOMB_MESH));
    m_pSceneNode->setScale(Ogre::Vector3::UNIT_SCALE * CPerson::PERSON_SCALE);
    m_fTimer = BOMB_TIMER;

    btCollisionShape *pShape = new btSphereShape(BOMB_COL_RADIUS);
    btRigidBody::btRigidBodyConstructionInfo ci(BOMB_MASS, new BtOgre::RigidBodyState(m_pSceneNode), pShape);
    btRigidBody *pRB = new btRigidBody(ci);

    objectManager.getMap().getPhysicsManager()->getWorld()->addRigidBody(pRB, COL_INTERACTIVE, MASK_STATIC_COLLIDES_WITH | COL_STATIC);
    m_pCollisionObject = pRB;

    setAsUserPointer(m_pCollisionObject);

    pRB->getWorldTransform().setOrigin(BtOgre::Convert::toBullet(vPosition + Ogre::Vector3::UNIT_Y * BOMB_COL_RADIUS));
}

CBomb::~CBomb() {
    if (m_pListener) {
        m_pListener->bombDestroyed(this);
    }
    if (m_pSceneNode) {
        destroySceneNode(m_pSceneNode);
        m_pSceneNode = NULL;
    }

    if (m_pCollisionObject) {
        btRigidBody *pRB = btRigidBody::upcast(m_pCollisionObject);
        m_ObjectManager.getMap().getPhysicsManager()->getWorld()->removeRigidBody(pRB);
        delete pRB->getCollisionShape();
        delete pRB->getMotionState();
        delete pRB;
        m_pCollisionObject = NULL;
    }
}
void CBomb::update(Ogre::Real tpf) {
    m_fTimer -= tpf;
    if (m_fTimer <= 0) {
        explode();
    }
}
void CBomb::explode() {
    suicide();

    std::list<CHitableInterface *> lObjects(m_ObjectManager.getMap().getHitableInterfacesInSphere(Ogre::Sphere(m_pSceneNode->getPosition(), BOMB_EXPLOSION_RADIUS)));
    for (CHitableInterface *pHI : lObjects) {
        pHI->receiveDamage(CDamage(CDamage::DMG_BOMB, (pHI->getPosition() - getPosition()).normalisedCopy()));
    }
}
