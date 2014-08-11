#include "Character.h"
#include "CharacterController.h"
#include "../Atlas/Map.hpp"
#include "../../Common/Physics/PhysicsMasks.hpp"
#include "../../Common/Physics/BtOgreExtras.h"
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
void CCharacter::enterMap(CMap *pMap) {
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
  //preUpdateBoundsCallback(fTime);
  //preAnimationUpdateCallback(fTime);
  //updateAnimations(fTime);
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
  if (m_uiAnimID == ANIM_JUMP_END && m_Anims[ANIM_JUMP_END]->hasEnded()) {
    if (!m_bMoving) {
      setAnimation(ANIM_IDLE);
    }
    else {
      setAnimation(ANIM_RUN);
    }
  }
  else if (m_uiAnimID == ANIM_JUMP_START && m_Anims[ANIM_JUMP_START]->hasEnded()) {
    setAnimation(ANIM_JUMP_LOOP);
  }

  if (m_bMoving && (m_uiAnimID == ANIM_IDLE || m_uiAnimID == ANIM_NONE)) {
    setAnimation(ANIM_RUN);
  }
  else if (!m_bMoving) {
    if (m_uiAnimID == ANIM_RUN || m_uiAnimID == ANIM_NONE) {
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
  //setAnimation(ANIM_SLICE_HORIZONTAL, true);
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

