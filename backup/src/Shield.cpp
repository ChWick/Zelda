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
#include "Shield.h"
#include "Person.h"
#include "Map.h"
#include "PhysicsManager.h"

CShield::CShield(CPerson *pOwner, const Ogre::String &parentBone, Ogre::SceneManager *pCreator, EShieldTypes eShieldType)
	: m_eShieldType(eShieldType),
	m_pOwner(pOwner), m_vOffset(0, 0, 0) {

	btCollisionShape *pColShape(NULL);
	btRigidBody *pRB(NULL);
	switch (m_eShieldType) {
	case ST_SIMPLE_SHIELD:
		m_pOwner->createHandObject(parentBone, CPerson::RIGHT_HAND, "SimpleShield.mesh");
		pColShape = new btBoxShape(btVector3(0.05f, 0.20f, 0.15f));
		pRB = new btRigidBody(0, new btDefaultMotionState(), pColShape);
		m_vOffset = Ogre::Vector3(-0.06f, 0.01f, 0.f);
		break;
	};
	pRB->setLinearFactor(btVector3(0, 0, 0));
	pRB->setAngularFactor(0);

	short int mask = 0;
	short int group = 0;
	if (pOwner->getFriendOrEnemyState() == CCharacter::FOE_FRIENDLY) {
        group = COL_SHIELD_P;
        mask = MASK_SHIELD_P_COLLIDES_WITH;
	}
	else {
        group = COL_SHIELD_N;
        mask = MASK_SHIELD_N_COLLIDES_WITH;
	}
	pOwner->m_pCurrentMap->getPhysicsManager()->getWorld()->addCollisionObject(pRB, group, mask);
	setAsUserPointer(pRB);
	m_pCollisionObject = pRB;
}
CShield::~CShield() {
	if (m_pCollisionObject) {
		m_pOwner->m_pCurrentMap->getPhysicsManager()->getWorld()->removeCollisionObject(m_pCollisionObject);
		if (btRigidBody::upcast(m_pCollisionObject)) {
			delete btRigidBody::upcast(m_pCollisionObject)->getMotionState();
		}
		delete m_pCollisionObject->getCollisionShape();
		delete m_pCollisionObject;
		m_pCollisionObject = NULL;
	}
}
void CShield::updateShieldPhysics(const Ogre::Vector3 &vPos, const Ogre::Quaternion &qRotation) {
	m_pCollisionObject->setWorldTransform(btTransform(BtOgre::Convert::toBullet(qRotation), BtOgre::Convert::toBullet(vPos + qRotation * m_vOffset)));

}
