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
#include "PersonController.hpp"


CStandingPerson::CStandingPerson(const tinyxml2::XMLElement *pElem, CEntity *pParent, CMap *pMap)
  : CPerson(pElem, pParent, pMap, SP_ANIM_COUNT) {
}

void CStandingPerson::setupInternal() {
  dynamic_cast<CPersonController*>(m_pCharacterController)->stun();
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
