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

#include "Character.hpp"
#include "CharacterController.hpp"
#include "../Atlas/Map.hpp"
#include "../../Common/Physics/PhysicsMasks.hpp"
#include "../../Common/Physics/BtOgreExtras.hpp"
#include <BulletDynamics/Character/btCharacterControllerInterface.h>
#include <OgreAnimationState.h>

CCharacter::CCharacter(const std::string &sID, CEntity *pParent, const EFriendOrEnemyStates foe)
	: CWorldEntity(sID, pParent, nullptr),
	  m_uiAnimationCount(ANIM_COUNT),
		m_fTimer(0),
		m_fAnimSpeed(1),
    m_eFriendOrEnemy(foe),
    m_pCharacterController(nullptr)
{
  mCCPhysics = NULL;
  m_bMoving = false;
  m_fYaw = 0;
  m_Anims.resize(m_uiAnimationCount);
  m_FadingStates.resize(m_uiAnimationCount);
  m_uiAnimID = m_uiAnimationCount;
}
CCharacter::~CCharacter() {
}
void CCharacter::enterMap(CMap *pMap, const Ogre::Vector3 &vInitPosition) {
  bool bSwitchMapOnly = m_pMap != nullptr;

  m_pMap = pMap;

  if (!bSwitchMapOnly) {
    // use atlas scene node
    initBody(m_pMap->getParent()->getSceneNode());
  }
	createPhysics();
	if (m_pCharacterController) {delete m_pCharacterController;}
  m_pCharacterController = createCharacterController();
	//setupAnimations();
	setupInternal();

	if (bSwitchMapOnly) {
    m_pCharacterController->moveToTarget(vInitPosition, 0.02f, Ogre::Degree(5), false, 5.f);
	}
	else {
    setPosition(vInitPosition);
	}
}

void CCharacter::createDamage(const Ogre::Ray &ray, const CDamage &dmg) const {
	// try to interact with the world. So detect an object to interact with
	btCollisionWorld::ClosestRayResultCallback rayCallback(BtOgre::Convert::toBullet(ray.getOrigin()), BtOgre::Convert::toBullet(ray.getPoint(1)));
  if (m_eFriendOrEnemy == FOE_FRIENDLY) {
    rayCallback.m_collisionFilterGroup = COL_DAMAGE_P;
    rayCallback.m_collisionFilterMask = MASK_DAMAGE_P_COLLIDES_WITH;
  }
  else {
    rayCallback.m_collisionFilterGroup = COL_DAMAGE_N;
    rayCallback.m_collisionFilterMask = MASK_DAMAGE_N_COLLIDES_WITH;
  }
	m_pMap->getPhysicsManager()->getWorld()->rayTest(BtOgre::Convert::toBullet(ray.getOrigin()), BtOgre::Convert::toBullet(ray.getPoint(1)), rayCallback);
	if (rayCallback.hasHit()) {
    CWorldEntity *pWE = CWorldEntity::getFromUserPointer(rayCallback.m_collisionObject);
    if (pWE) {
      pWE->hit(dmg);
		}
	}
}

void CCharacter::destroy() {
	destroyPhysics();
	//destroySceneNode(m_pSceneNode);
	m_pBodyEntity = NULL;
	if (m_pCharacterController) {
		delete m_pCharacterController;
	}
}

void CCharacter::setPosition(const Ogre::Vector3 &vPos) {
  if (mCCPhysics) {
    mCCPhysics->warp(BtOgre::Convert::toBullet(vPos));
    m_pSceneNode->setPosition(BtOgre::Convert::toOgre(m_pCollisionObject->getWorldTransform().getOrigin()));
  }
  else {
    m_pCharacterController->setPosition(vPos);
  }
}

void CCharacter::setOrientation(const Ogre::Quaternion &vRotation) {
    m_pCharacterController->setOrientation(vRotation);
}

void CCharacter::update(Ogre::Real fTime) {
  CWorldEntity::update(fTime);

  m_pCharacterController->updateCharacter(fTime);
  preUpdateBoundsCallback(fTime);
  preAnimationUpdateCallback(fTime);
  updateAnimations(fTime);
}

void CCharacter::updateAnimations(Ogre::Real fTime) {
  m_fTimer += fTime;
  m_fAnimSpeed = 1;

  updateAnimationsCallback(fTime);

  // increment the current animation times
  if (m_uiAnimID != ANIM_NONE) m_Anims[m_uiAnimID]->addTime(fTime * m_fAnimSpeed);

  // apply smooth transitioning between our animations
  fadeAnimations(fTime);

  postUpdateAnimationsCallback(fTime);
}

void CCharacter::updateAnimationsCallback(const Ogre::Real fTime) {
  if (m_uiAnimID == ANIM_SLICE_HORIZONTAL && m_fTimer >= m_Anims[ANIM_SLICE_HORIZONTAL]->getLength()) {
    setAnimation(ANIM_IDLE);
  }
  /*if (m_uiAnimID == ANIM_JUMP_END && m_Anims[ANIM_JUMP_END]->hasEnded()) {
    if (!m_bMoving) {
      setAnimation(ANIM_IDLE);
    }
    else {
      setAnimation(ANIM_RUN);
    }
  }
  else if (m_uiAnimID == ANIM_JUMP_START && m_Anims[ANIM_JUMP_START]->hasEnded()) {
    setAnimation(ANIM_JUMP_LOOP);
  }*/

  if (m_bMoving && (m_uiAnimID == ANIM_IDLE || m_uiAnimID == ANIM_NONE)) {
    setAnimation(ANIM_WALK);
  }
  else if (!m_bMoving) {
    if (m_uiAnimID == ANIM_WALK || m_uiAnimID == ANIM_NONE) {
      setAnimation(ANIM_IDLE);
    }
  }
}
void CCharacter::setAnimation(unsigned int id, bool reset) {
  if (m_uiAnimID < m_uiAnimationCount)
  {
    // if we have an old animation, fade it out
    m_FadingStates[m_uiAnimID] = FADE_OUT;
  }

  m_uiAnimID = id;

  if (id != m_uiAnimationCount)
  {
    // if we have a new animation, enable it and fade it in
    m_Anims[id]->setEnabled(true);
    //m_Anims[id]->setWeight(0);
    m_FadingStates[m_uiAnimID] = FADE_IN;
    if (reset) m_Anims[id]->setTimePosition(0);
  }
}

void CCharacter::fadeAnimations(const Ogre::Real deltaTime)
{
    for (unsigned int i = 0; i < m_uiAnimationCount; i++)
    {
        if (m_FadingStates[i] == FADE_NONE) {
            continue;
        }
        else if (m_FadingStates[i] == FADE_IN)
        {
            // slowly fade this animation in until it has full weight
            Ogre::Real newWeight = m_Anims[i]->getWeight() + deltaTime * ANIM_FADE_SPEED;
            m_Anims[i]->setWeight(Ogre::Math::Clamp<Ogre::Real>(newWeight, 0, 1));
            if (newWeight >= 1) m_FadingStates[i] = FADE_NONE;
        }
        else if (m_FadingStates[i] == FADE_OUT)
        {
            // slowly fade this animation out until it has no weight, and then disable it
            Ogre::Real newWeight = m_Anims[i]->getWeight() - deltaTime * ANIM_FADE_SPEED;
            m_Anims[i]->setWeight(Ogre::Math::Clamp<Ogre::Real>(newWeight, 0, 1));
            if (newWeight <= 0)
            {
                m_Anims[i]->setEnabled(false);
                m_FadingStates[i] = FADE_NONE;
            }
        }
    }
}

void CCharacter::animRunStart()
{
    /*if (m_uiTopAnimID ==)
	setTopAnimation(ANIM_RUN_TOP);
	setBaseAnimation(ANIM_RUN_BASE);*/
}
void CCharacter::animRunEnd()
{
	/*setTopAnimation(ANIM_IDLE_TOP);
	setBaseAnimation(ANIM_IDLE_BASE, true);*/
}
void CCharacter::animJumpStart()
{
	//setAnimation(ANIM_JUMP_START, true);
	m_fTimer = 0;
}
void CCharacter::animJumpLoop()
{
	//setAnimation(ANIM_JUMP_LOOP, true);
}
void CCharacter::animJumpEnd()
{
	//setAnimation(ANIM_JUMP_END, true);
	m_fTimer = 0;
}
void CCharacter::animAttack() {
  setAnimation(ANIM_SLICE_HORIZONTAL, true);
  m_fTimer = 0;
}
void CCharacter::animAttackEnd() {
}
void CCharacter::setIsMoving(bool isMoving) {
	m_bMoving = isMoving;
}

short CCharacter::getCollisionMask() {
    switch (m_eFriendOrEnemy) {
    case FOE_ENEMY:
        return MASK_PLAYER_N_COLLIDES_WITH;
    case FOE_FRIENDLY:
        return MASK_PLAYER_P_COLLIDES_WITH;
    }
}
short CCharacter::getCollisionGroup() {
    switch (m_eFriendOrEnemy) {
    case FOE_ENEMY:
        return COL_CHARACTER_N;
    case FOE_FRIENDLY:
        return COL_CHARACTER_P;;
    }
}

