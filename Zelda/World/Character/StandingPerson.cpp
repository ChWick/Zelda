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

#include "StandingPerson.hpp"
#include "PersonTypes.hpp"
#include <OgreEntity.h>
#include <OgreBone.h>
#include <OgreSkeletonInstance.h>
#include <OgreSceneNode.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include "../../Common/Physics/PhysicsManager.hpp"
#include "../Atlas/Map.hpp"
#include "../GlobalCollisionShapesTypes.hpp"


CStandingPerson::CStandingPerson(const tinyxml2::XMLElement *pElem, CEntity *pParent)
  : CPerson(pElem, pParent, SP_ANIM_COUNT) {
}

void CStandingPerson::createPhysics() {
  using namespace Ogre;

  assert(m_pSceneNode);

  btPairCachingGhostObject * characterGhostObject = new btPairCachingGhostObject();

  const CPhysicsCollisionObject &pco = m_pMap->getPhysicsManager()->getCollisionShape(GLOBAL_COLLISION_SHAPES_TYPES_ID_MAP.toString(GCST_PERSON_CAPSULE));
  btConvexShape * capsule = dynamic_cast<btConvexShape*>(pco.getShape());
  characterGhostObject->setCollisionShape(capsule);

  m_pMap->getPhysicsManager()->getWorld()->addCollisionObject(characterGhostObject, getCollisionGroup(), getCollisionMask());
  m_pCollisionObject = characterGhostObject;

  setThisAsCollisionObjectsUserPointer();
}
void CStandingPerson::destroyPhysics() {
	if (m_pCollisionObject) {
		m_pMap->getPhysicsManager()->getWorld()->removeCollisionObject(m_pCollisionObject);

		m_pMap->getPhysicsManager()->getBroadphase()->resetPool(m_pMap->getPhysicsManager()->getWorld()->getDispatcher());
		m_pMap->getPhysicsManager()->getBroadphase()->getOverlappingPairCache()->cleanProxyFromPairs(m_pCollisionObject->getBroadphaseHandle(), m_pMap->getPhysicsManager()->getWorld()->getDispatcher());

		delete m_pCollisionObject;
		m_pCollisionObject = NULL;
	}
}

void CStandingPerson::setupInternal() {
}

void CStandingPerson::setupAnimations() {
  Ogre::StringVector animNames(SP_ANIM_COUNT);
	animNames[SP_ANIM_STAND] = "Scout";

	// this is very important due to the nature of the exported animations
  m_pBodyEntity->getSkeleton()->setBlendMode(Ogre::ANIMBLEND_CUMULATIVE);

  // populate our animation list
  for (unsigned int i = 0; i < m_uiAnimationCount; i++) {
    m_Anims[i] = m_pBodyEntity->getAnimationState(animNames[i]);
    m_Anims[i]->setLoop(true);
    m_FadingStates[i] = FADE_NONE;
    m_Anims[i]->setEnabled(false);
    m_Anims[i]->setWeight(0);
  }


  // start off in the idle state (top and bottom together)
  setAnimation(SP_ANIM_STAND);
}

CStandingPerson::EReceiveDamageResult CStandingPerson::receiveDamage(const CDamage &dmg) {
  return RDR_IGNORED;
}
