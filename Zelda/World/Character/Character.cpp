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
#include <BulletDynamics/Character/btCharacterControllerInterface.h>
#include <string>
#include <OgreEntity.h>
#include <OgreAnimationState.h>
#include <OgreSkeletonInstance.h>
#include "CharacterController.hpp"
#include "../Atlas/Map.hpp"
#include "../../Common/Physics/PhysicsMasks.hpp"
#include "../../Common/Physics/BtOgreExtras.hpp"
#include "../Items/CharacterItem.hpp"
#include "../../Common/Util/Assert.hpp"

// map will be set on enter map, in construction, m_pMap is nullptr
CCharacter::CCharacter(const std::string &sID,
                       CEntity *pParent,
                       CMap *pMap,
                       const SCharacterData &characterData)
    : CWorldEntity(sID, pParent, pMap),
      m_pCharacterController(nullptr),
      mCharacterData(characterData),
      m_uiAnimationCount(characterData.mAnimations.size()),
  m_fTimer(0),
  m_fAnimSpeed(1) {
  m_Anims.resize(m_uiAnimationCount);
  m_uiAnimID = m_uiAnimationCount;

  constructor_impl();
}

CCharacter::CCharacter(const tinyxml2::XMLElement *pElem,
                       CEntity *pParent,
                       CMap *pMap,
                       const SCharacterData &characterData)
    : CWorldEntity(pParent, pMap, pElem),
      m_pCharacterController(nullptr),
      mCharacterData(characterData),
      m_uiAnimationCount(characterData.mAnimations.size()),
      m_fTimer(0),
      m_fAnimSpeed(1) {
  m_Anims.resize(m_uiAnimationCount);
  m_uiAnimID = m_uiAnimationCount;

  constructor_impl();
}


void CCharacter::constructor_impl() {
  mCCPhysics = NULL;
  m_bMoving = false;
  m_fYaw = 0;

  mAnimationProperty.resize(m_uiAnimationCount);
  for (uint8_t i = 0; i < m_uiAnimationCount; ++i) {
    mAnimationProperty[i].mAnimationData = &mCharacterData.mAnimations[i];
  }
}

CCharacter::~CCharacter() {
}
void CCharacter::exit() {
  for (int i = 0; i < CIS_COUNT; i++) {
    mCurrentItems[i].reset();
  }
  CWorldEntity::exit();
  destroyPhysics();
}
void CCharacter::enterMap(CMap *pMap, const Ogre::Vector3 &vInitPosition) {
  // switch map only, if map an scene node are existing
  bool bSwitchMapOnly = m_pMap && m_pSceneNode;

  // destroy old physics
  destroyPhysics();

  // delete tools, will be recreated, but store their current state
  EItemVariantTypes currentItemType[CIS_COUNT];
  std::string weaponBone[CIS_COUNT];
  for (int i = 0; i < CIS_COUNT; i++) {
    currentItemType[i] = ITEM_VARIANT_COUNT;
    if (mCurrentItems[i]) {
      currentItemType[i] = mCurrentItems[i]->getItemVariantType();
      weaponBone[i] = mCurrentItems[i]->getBoneToAttach();
      mCurrentItems[i].reset();
    }
  }

  m_pMap = pMap;

  if (!bSwitchMapOnly) {
    // use atlas scene node
    initBody(m_pMap->getParent()->getSceneNode());
    setupAnimations();
  }
  createPhysics();
  if (m_pCharacterController) {delete m_pCharacterController;}
  m_pCharacterController = createCharacterController();
  setupInternal();

  if (bSwitchMapOnly) {
    m_pCharacterController->moveToTarget(vInitPosition,
                                         0.02f, Ogre::Degree(5),
                                         false,
                                         5.f);
  } else {
    setPosition(vInitPosition);
    m_pCharacterController->start();
  }

  for (int i = 0; i < CIS_COUNT; i++) {
    if (currentItemType[i] != ITEM_VARIANT_COUNT) {
      changeItem(static_cast<ECharacterItemSlots>(i),
                 weaponBone[i], currentItemType[i]);
    }
  }
}

void CCharacter::setupAnimations() {
  ASSERT(m_pBodyEntity);
  // this is very important due to the nature of the exported animations
  m_pBodyEntity->getSkeleton()->setBlendMode(Ogre::ANIMBLEND_CUMULATIVE);

  preSetupAnimations();
  for (SAnimationProperty &animProp : mAnimationProperty) {
    ASSERT(animProp.mAnimationData);
    Ogre::AnimationState *animState
        = m_pBodyEntity->getAnimationState(animProp.mAnimationData->mName);
    animProp.mState = animState;
    animProp.mFadeState = FADE_NONE;

    animState->setLoop(animProp.mAnimationData->mLoop);
    animState->setEnabled(false);
    animState->setWeight(0);

    m_Anims[animProp.mAnimationData->mId] = animState;
  }

  //setAnimation(0, true, true);

  postSetupAnimations();
}

bool CCharacter::createDamage(const Ogre::Ray &ray, const CDamage &dmg) {
  // try to interact with the world. So detect an object to interact with
  btCollisionWorld::ClosestRayResultCallback rayCallback(
      BtOgre::Convert::toBullet(ray.getOrigin()),
      BtOgre::Convert::toBullet(ray.getPoint(1)));
  if (mCharacterData.mAttitude == ATTITUDE_FRIENDLY) {
    rayCallback.m_collisionFilterGroup = COL_DAMAGE_P;
    rayCallback.m_collisionFilterMask = MASK_DAMAGE_P_COLLIDES_WITH;
  } else {
    rayCallback.m_collisionFilterGroup = COL_DAMAGE_N;
    rayCallback.m_collisionFilterMask = MASK_DAMAGE_N_COLLIDES_WITH;
  }
  m_pMap->getPhysicsManager()->getWorld()->rayTest(
      BtOgre::Convert::toBullet(ray.getOrigin()),
      BtOgre::Convert::toBullet(ray.getPoint(1)), rayCallback);
  if (rayCallback.hasHit()) {
    CWorldEntity *pWE
        = CWorldEntity::getFromUserPointer(rayCallback.m_collisionObject);
    if (pWE) {
      EReceiveDamageResult res = attack(dmg, pWE);
      if (res == RDR_BLOCKED) {
      }
      return true;
    }
  }
  return false;
}

void CCharacter::destroy() {
  destroyPhysics();
  // destroySceneNode(m_pSceneNode);
  m_pBodyEntity = NULL;
  if (m_pCharacterController) {
    delete m_pCharacterController;
  }
}

void CCharacter::setPosition(const Ogre::Vector3 &vPos) {
  if (mCCPhysics) {
    mCCPhysics->warp(BtOgre::Convert::toBullet(vPos));
    m_pSceneNode->setPosition(BtOgre::Convert::toOgre(
        m_pCollisionObject->getWorldTransform().getOrigin()));
  } else {
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

  // update item/weapon
}

void CCharacter::updateAnimations(Ogre::Real fTime) {
  if (m_uiAnimID >= m_uiAnimationCount) {return;}

  m_fTimer += fTime;
  m_fAnimSpeed = 1;

  updateAnimationsCallback(fTime);

  // increment the current animation times
  if (m_uiAnimID != ANIM_NONE) {
    m_Anims[m_uiAnimID]->addTime(fTime * m_fAnimSpeed);
  }

  // apply smooth transitioning between our animations
  fadeAnimations(fTime);

  postUpdateAnimationsCallback(fTime);
}

void CCharacter::updateAnimationsCallback(const Ogre::Real fTime) {
  if (m_uiAnimID == ANIM_SLICE_HORIZONTAL) {
    if (m_fTimer >= m_Anims[ANIM_SLICE_HORIZONTAL]->getLength()) {
      animAttackEnd();
    }
  }
  else if (m_uiAnimID == ANIM_USE_ITEM) {
    if (m_fTimer >= m_Anims[ANIM_USE_ITEM]->getLength()) {
      animUseToolEnd();
    }
  }
}

void CCharacter::setAnimation(unsigned int id, bool reset, bool force) {
  if (force) {
    // no fading

    // disable all animations
    for (unsigned int i = 0; i < m_uiAnimationCount; i++) {
      mAnimationProperty[m_uiAnimID].mFadeState = FADE_OUT;
      m_Anims[id]->setEnabled(false);
      m_Anims[id]->setWeight(0);
    }

    // only show current
    m_uiAnimID = id;
    if (id != m_uiAnimationCount) {
      // if we have a new animation, enable it and fade it in
      m_Anims[id]->setEnabled(true);
      mAnimationProperty[id].mFadeState = FADE_IN;
      m_Anims[id]->setWeight(1);
      if (reset) m_Anims[id]->setTimePosition(0);
    }
  } else {
    // fadeing

    if (m_uiAnimID < m_uiAnimationCount) {
      // if we have an old animation, fade it out
      mAnimationProperty[m_uiAnimID].mFadeState = FADE_OUT;
    }

    m_uiAnimID = id;

    if (id != m_uiAnimationCount) {
      // if we have a new animation, enable it and fade it in
      m_Anims[id]->setEnabled(true);
      // m_Anims[id]->setWeight(0);
      mAnimationProperty[id].mFadeState = FADE_IN;
      if (reset) m_Anims[id]->setTimePosition(0);
    }
  }
}


const Ogre::AnimationState *CCharacter::getAnimation(unsigned int id) const {
  return m_Anims[id];
}

unsigned int CCharacter::getAnimationIdFromString(const std::string &id) const {
  if (id == "idle") {
    return ANIM_IDLE;
  } else if (id == "walk") {
    return ANIM_WALK;
  } else if (id == "slice_horizontal") {
    return ANIM_SLICE_HORIZONTAL;
  } else if (id == "use_item") {
    return ANIM_USE_ITEM;
  } else if (id == "run") {
    return ANIM_RUN;
  }
  LOGW("Animation id '%s' is unknown.", id.c_str());
  return ANIM_COUNT;
}

void CCharacter::fadeAnimations(const Ogre::Real deltaTime) {
  for (unsigned int i = 0; i < m_uiAnimationCount; i++) {
    if (mAnimationProperty[i].mFadeState == FADE_NONE) {
      continue;
    } else if (mAnimationProperty[i].mFadeState == FADE_IN) {
      // slowly fade this animation in until it has full weight
      Ogre::Real newWeight = m_Anims[i]->getWeight()
          + deltaTime * ANIM_FADE_SPEED;
      m_Anims[i]->setWeight(Ogre::Math::Clamp<Ogre::Real>(newWeight, 0, 1));
      if (newWeight >= 1) mAnimationProperty[i].mFadeState = FADE_NONE;
    } else if (mAnimationProperty[i].mFadeState == FADE_OUT) {
      // slowly fade this animation out until it has no weight,
      // and then disable it
      Ogre::Real newWeight = m_Anims[i]->getWeight()
          - deltaTime * ANIM_FADE_SPEED;
      m_Anims[i]->setWeight(Ogre::Math::Clamp<Ogre::Real>(newWeight, 0, 1));
      if (newWeight <= 0) {
        m_Anims[i]->setEnabled(false);
        mAnimationProperty[i].mFadeState = FADE_NONE;
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
void CCharacter::animJumpEnd() {
  setAnimation(ANIM_IDLE);
  m_fTimer = 0;
}
void CCharacter::animAttack() {
  if (!isReadyForNewAction()) {return;}
  setAnimation(ANIM_SLICE_HORIZONTAL, true);
  mCurrentItems[CIS_WEAPON]->startDamage();
  m_fTimer = 0;
}
void CCharacter::animAttackEnd() {
  setAnimation(ANIM_IDLE);
  m_fTimer = 0;
}

void CCharacter::animUseToolStart() {
  if (!isReadyForNewAction()) {return;}
  setAnimation(ANIM_USE_ITEM, true);
  mCurrentItems[CIS_TOOL]->show();
  mCurrentItems[CIS_TOOL]->startDamage();
  mCurrentItems[CIS_WEAPON]->hide();
  m_fTimer = 0;
}

void CCharacter::animUseToolEnd() {
  setAnimation(ANIM_IDLE);
  mCurrentItems[CIS_TOOL]->hide();
  mCurrentItems[CIS_WEAPON]->show();
  m_fTimer = 0;
}


void CCharacter::setIsMoving(bool isMoving) {
  m_bMoving = isMoving;
}

bool CCharacter::isReadyForNewAction() {
  return m_uiAnimID == ANIM_NONE || m_uiAnimID == ANIM_IDLE || m_uiAnimID == ANIM_WALK;
}

short CCharacter::getCollisionMask() {
    switch (mCharacterData.mAttitude) {
    case ATTITUDE_ENEMY:
        return MASK_PLAYER_N_COLLIDES_WITH;
    case ATTITUDE_FRIENDLY:
        return MASK_PLAYER_P_COLLIDES_WITH;
    }
    throw Ogre::Exception(0, "Unknown collision mask", __FUNCTION__);
}

short CCharacter::getCollisionGroup() {
    switch (mCharacterData.mAttitude) {
      case ATTITUDE_ENEMY:
        return COL_CHARACTER_N;
      case ATTITUDE_FRIENDLY:
        return COL_CHARACTER_P;
    }
    throw Ogre::Exception(0, "Unknown collision mask", __FUNCTION__);
}

void CCharacter::useCurrentItem() {
  ASSERT(mCurrentItems[CIS_TOOL]);
  useItem(mCurrentItems[CIS_TOOL]->getItemVariantType());
}

void CCharacter::useCurrentWeapon() {
  ASSERT(mCurrentItems[CIS_TOOL]);
  if (isReadyForNewAction()) {
    animAttack();
  }
}

void CCharacter::changeItem(ECharacterItemSlots slot,
                            const std::string &bone,
                            EItemVariantTypes item) {
  mCurrentItems[slot] = std::shared_ptr<CCharacterItem>(
      new CCharacterItem(this, bone, item));
  switch (slot) {
    case CIS_TOOL:
      mCurrentItems[slot]->hide();
      break;
    default:
      mCurrentItems[slot]->show();
      break;
  }
}

void CCharacter::useItem(EItemVariantTypes item) {
  if (isReadyForNewAction()) {
    animUseToolStart();
  }
}













