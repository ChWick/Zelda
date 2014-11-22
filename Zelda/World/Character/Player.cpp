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

#include "Player.hpp"
#include "../../Common/Physics/PhysicsManager.hpp"
#include "../../Common/Physics/PhysicsMasks.hpp"
#include "../Atlas/Map.hpp"
#include "../../Common/Util/DebugDrawer.hpp"
#include "PlayerController.hpp"
#include <OgreLogManager.h>
#include "../../Common/Physics/BtOgreExtras.hpp"
#include "../../Common/Util/Assert.hpp"
#include "../../Common/GameLogic/Events/Event.hpp"
#include "../../Common/Message/MessagePlayerPickupItem.hpp"
#include "../../Common/Message/MessageEntityStateChanged.hpp"
#include "PersonTypes.hpp"
#include <OgreEntity.h>
#include <OgreBone.h>
#include <OgreSkeleton.h>
#include <OgreSkeletonInstance.h>
#include "../../Common/Log.hpp"
#include "../Items/CharacterItem.hpp"
#include "../Messages/MessageItem.hpp"
#include "../Messages/UserMessageTypes.hpp"
#include "../Objects/ObjectTypes.hpp"
#include "../UserImplementations/EntityPropertyIds.hpp"


#define TURN_SCALE 4
#define MAX_SPEED_SCALE 5

Ogre::Real g_fCurrentSpeedScale = 1;
const Ogre::Real PLAYER_ENEMY_NOTIFY_RADIUS_SQR = 100.f;  // already squared

CPlayer::CPlayer(CEntity *pParent,
                 const Ogre::Camera* pCamera,
                 Ogre::SceneManager *pPlayerSceneManager)
  : CPerson("player", pParent, nullptr, PERSON_DATA_ID_MAP.toData(PERSON_LINK)),
    m_pCamera(pCamera),
    m_pPlayerSceneManager(pPlayerSceneManager),
    m_pLiftedEntity(nullptr),
    mRupeeCount(*this, ENTITY_PROPERTY_RUPEE, 0, 999, -1) {
}

CPlayer::~CPlayer() {
}

CCharacterController *CPlayer::createCharacterController() {
  return new CPlayerController(m_pPlayerSceneManager, m_pCamera, this);
}
void CPlayer::setupInternal()  {
  // create swords and attach to sheath
  Ogre::LogManager::getSingleton().logMessage("Creating swords");
  // createHandObject(PERSON_LEFT_HANDLE, RIGHT_HAND, "link_sword.mesh");
  changeWeapon(PERSON_LEFT_HANDLE, ITEM_VARIANT_SWORD_SIMPLE);


	/*LogManager::getSingleton().logMessage("Creating the chains");
	// create a couple of ribbon trails for the swords, just for fun
	NameValuePairList params;
	params["numberOfChains"] = "2";
	params["maxElements"] = "80";
	mSwordTrail = (RibbonTrail*)sceneMgr->createMovableObject("RibbonTrail", &params);
	mSwordTrail->setMaterialName("Effects/LightRibbonTrail");
	mSwordTrail->setTrailLength(20 * SCALE);
	mSwordTrail->setVisible(false);
	sceneMgr->getRootSceneNode()->attachObject(mSwordTrail);



	for (int i = 0; i < 2; i++)
	{
		mSwordTrail->setInitialColour(i, 1, 0.8, 0);
		mSwordTrail->setColourChange(i, 0.75, 1.25, 1.25, 1.25);
		mSwordTrail->setWidthChange(i, 1 * SCALE);
		mSwordTrail->setInitialWidth(i, 0.5 * SCALE);
	}*/
}

void CPlayer::setupAnimations() {
  Ogre::StringVector animNames(ANIM_COUNT);
  animNames[ANIM_IDLE] = "Idle";
  animNames[ANIM_RUN] = "RunPegasus";
  animNames[ANIM_SLICE_HORIZONTAL] = "SwordAttack";
  animNames[ANIM_USE_ITEM] = "SwordAttack";
  animNames[ANIM_WALK] = "Walk";
  /*animNames[ANIM_HANDS_CLOSED] = "HandsClosed";
    animNames[ANIM_HANDS_RELAXED] = "HandsRelaxed";
    animNames[ANIM_SLICE_HORIZONTAL] = "SliceHorizontal";
    animNames[ANIM_BOW_SHOT] = "BowShot";
    animNames[ANIM_JUMP_START] = "JumpStart";
    animNames[ANIM_JUMP_LOOP] = "JumpLoop";
    animNames[ANIM_JUMP_END] = "JumpEnd";*/

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

  // m_Anims[ANIM_HANDS_CLOSED]->setWeight(1);
  // m_Anims[ANIM_HANDS_RELAXED]->setWeight(1);

  // m_Anims[ANIM_JUMP_END]->setLoop(false);
  // m_Anims[ANIM_JUMP_START]->setLoop(false);


  // relax the hands since we're not holding anything
  // m_Anims[ANIM_HANDS_RELAXED]->setEnabled(true);


  // start off in the idle state (top and bottom together)
  setAnimation(ANIM_IDLE);

  // we will manually set the direction of the tool in the left hand
  Ogre::Bone *pLHandleBone
      = m_pBodyEntity->getSkeleton()->getBone(PERSON_LEFT_HANDLE);
  pLHandleBone->setManuallyControlled(true);
  int numAnimations = m_pBodyEntity->getSkeleton()->getNumAnimations();
  for (int i = 0; i < numAnimations; i++) {
    // remonveall possible tracks of the bone in the animations
    Ogre::Animation * anim = m_pBodyEntity->getSkeleton()->getAnimation(i);
    anim->destroyNodeTrack(pLHandleBone->getHandle());
  }
}

void CPlayer::startup(const Ogre::Vector3 &playerPosition,
                      const Ogre::Vector3 &playerLookDirection,
                      const Ogre::Real cameraYaw,
                      const Ogre::Real cameraPitch) {
  m_pSceneNode->setPosition(playerPosition
                            + Ogre::Vector3::UNIT_Y * PERSON_HEIGHT);
  m_pSceneNode->lookAt(playerLookDirection, Ogre::Node::TS_LOCAL);

  m_pCollisionObject
      ->setWorldTransform(btTransform(
          BtOgre::Convert::toBullet(m_pSceneNode->getOrientation()),
          BtOgre::Convert::toBullet(m_pSceneNode->getPosition())));
}

void CPlayer::loadSaveFile() {
  mRupeeCount.setData(0);  // Will be set later
}

void CPlayer::interact() {
  if (m_pLiftedEntity) {
    throwLifted();
    return;
  }

  // start outside player capsule
  Ogre::Vector3 startPos(getFloorPosition()
                         + Ogre::Vector3::UNIT_Y * PERSON_HEIGHT * 0.4
                         + getOrientation().zAxis() * PERSON_RADIUS * 1.1f);
  Ogre::Vector3 endPos(startPos
                       + getOrientation().zAxis() * PERSON_RADIUS * 1.5f);

  // try to interact with the world. So detect an object to interact with
  btCollisionWorld::ClosestRayResultCallback rayCallback(
      BtOgre::Convert::toBullet(startPos), BtOgre::Convert::toBullet(endPos));
  rayCallback.m_collisionFilterGroup = COL_CHARACTER_P;
  rayCallback.m_collisionFilterMask = MASK_PLAYER_P_COLLIDES_WITH;
  m_pMap->getPhysicsManager()->getWorld()->rayTest(
      BtOgre::Convert::toBullet(startPos),
      BtOgre::Convert::toBullet(endPos), rayCallback);

  if (rayCallback.hasHit()) {
    CWorldEntity *pWE
        = CWorldEntity::getFromUserPointer(rayCallback.m_collisionObject);
    if (pWE) {
      SInteractionResult res(pWE->interactOnActivate(getOrientation().zAxis(),
                                                     this));
      if (res.eResult == IR_LIFT) {
        lift(pWE);
      }
    }
  }
}

void CPlayer::update(Ogre::Real tpf) {
  CPerson::update(tpf);

  for (CWorldEntity *pWE :
           dynamic_cast<CharacterControllerPhysics*>(mCCPhysics)
           ->getCollidingWorldEntities()) {
    // check for collisions, to pickup
    pWE->interactOnCollision(this->getPosition() - pWE->getPosition(), this);
    this->interactOnCollision(pWE->getPosition() - this->getPosition(), pWE);
  }

  updateLiftedObject(tpf);
}

void CPlayer::updateLiftedObject(const Ogre::Real fTime) {
  if (!m_pLiftedEntity) {return;}

  m_pLiftedEntity->setPosition(
      getPosition()
      + Ogre::Vector3::UNIT_Y * (PERSON_HEIGHT - PERSON_FLOOR_OFFSET));
}

void CPlayer::updateAnimationsCallback(const Ogre::Real fTime) {
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
  if (dynamic_cast<CPersonController*>(m_pCharacterController)->getMoveState()
      == CPersonController::MS_RUNNING) {
    if (m_uiAnimID != ANIM_RUN) {
      setAnimation(ANIM_RUN);
    }
  } else if (m_bMoving) {
    if (m_uiAnimID == ANIM_IDLE || m_uiAnimID == ANIM_NONE
        || m_uiAnimID == ANIM_RUN) {
      setAnimation(ANIM_WALK);
    }
  } else if (!m_bMoving) {
    if (m_uiAnimID == ANIM_WALK || m_uiAnimID == ANIM_NONE
        || m_uiAnimID == ANIM_RUN) {
      setAnimation(ANIM_IDLE);
    }
  }

  CPerson::updateAnimationsCallback(fTime);
}

void CPlayer::renderDebug(Ogre::Real tpf) {
  CPerson::renderDebug(tpf);

  // draw interaction
  Ogre::Vector3 startPos(getFloorPosition()
                         + Ogre::Vector3::UNIT_Y * PERSON_HEIGHT * 0.4);
  Ogre::Vector3 endPos(startPos
                       + getOrientation().zAxis() * PERSON_RADIUS * 2.5f);
  DebugDrawer::getSingleton().drawLine(startPos, endPos,
                                       Ogre::ColourValue::Red);
}

void CPlayer::preUpdateBoundsCallback(const Ogre::Real deltaTime) {
}

/*void CPlayer::objectChangedMap(CObject *pBefore, CObject *pAfter) {
	CPlayerInteractiveArea *pPIA = dynamic_cast<CPlayerInteractiveArea*>(pBefore);
	if (pPIA && pPIA == m_pInteractingArea) {
		m_pInteractingArea = dynamic_cast<CPlayerInteractiveArea*>(pAfter);
	}
}*/

void CPlayer::enterMap(CMap *pMap, const Ogre::Vector3 &vInitPosition) {
  CPerson::enterMap(pMap, vInitPosition);

  if (m_pLiftedEntity) {
      m_pLiftedEntity->enterMap(pMap, vInitPosition);
  }
}

void CPlayer::lift(CWorldEntity *pObjectToLift) {
  ASSERT(!m_pLiftedEntity);
  m_pLiftedEntity = pObjectToLift;

  m_pLiftedEntity->changeState(EST_LIFTED);
}

void CPlayer::throwLifted() {
  ASSERT(m_pLiftedEntity);

  m_pLiftedEntity->changeState(EST_THROWN);

  btRigidBody *pRB = btRigidBody::upcast(m_pLiftedEntity->getCollisionObject());
  ASSERT(pRB);
  pRB->setLinearVelocity(BtOgre::Convert::toBullet(
      getOrientation() * Ogre::Vector3(0, 0.5, 1)));

  m_pLiftedEntity = nullptr;
}

CPlayer::EReceiveDamageResult CPlayer::receiveDamage(const CDamage &dmg) {
  this->makeInvulnerable(1);
  dynamic_cast<CPersonController*>(m_pCharacterController)
      ->changeMoveState(CPersonController::MS_PUSHED_BACK,
                        dmg.getDamageDirection(), 0.2, 0.4);
  return RDR_ACCEPTED;
}

void CPlayer::postStepForwardAndStrafe() {
  CPersonController *pPersonController(
      dynamic_cast<CPersonController*>(m_pCharacterController));
  if (pPersonController->getMoveState() == CPersonController::MS_RUNNING) {
    const Ogre::Vector3 vDir = m_pSceneNode->getOrientation().zAxis();
    const Ogre::Vector3 vPos =
        BtOgre::Convert::toOgre(
            m_pCollisionObject->getWorldTransform().getOrigin())
        + Ogre::Vector3::UNIT_Y
        * (PERSON_HEIGHT * 0.15 - PERSON_PHYSICS_OFFSET);

    DebugDrawer::getSingleton().drawLine(vPos,
                                         vPos + vDir * 0.08,
                                         Ogre::ColourValue::Blue);
    createDamage(Ogre::Ray(vPos, vDir * 0.08),
                 CDamage(this, DMG_SWORD,
                         m_pSceneNode->getOrientation().zAxis()));
  }
}

void CPlayer::handleMessage(const CMessage &message) {
  if (message.getType() == MSG_ITEM) {
    const CMessageItem &msg_item(dynamic_cast<const CMessageItem&>(message));
    if (msg_item.getItemMessageType() == CMessageItem::IM_SELECTION_CHANGED) {
      changeItem(PERSON_LEFT_HANDLE, msg_item.getItemVariantType());
    }
  } else if (message.getType() == MSG_PLAYER_PICKUP_ITEM) {
    const CMessagePlayerPickupItem &msg_pui(
        dynamic_cast<const CMessagePlayerPickupItem&>(message));
    if (msg_pui.getItemType() == OBJECT_HEART) {
      changeHP(HP_ONE_HEART);
    } else if (msg_pui.getItemType() == OBJECT_RED_RUPEE) {
      mRupeeCount.addData(10);
    } else if (msg_pui.getItemType() == OBJECT_BLUE_RUPEE) {
      mRupeeCount.addData(5);
    } else if (msg_pui.getItemType() == OBJECT_GREEN_RUPEE) {
      mRupeeCount.addData(1);
    }
  } else if (message.getType() == MSG_ENTITY_STATE_CHANGED) {
    const CMessageEntityStateChanged &msg_esc(
        dynamic_cast<const CMessageEntityStateChanged&>(message));
    if (m_pLiftedEntity && &msg_esc.getEntity() == m_pLiftedEntity) {
      if (msg_esc.getNewState() == EST_DELETE) {
        // entity was deleted, reset it
        m_pLiftedEntity = nullptr;
      }
    }
  }

  CPerson::handleMessage(message);
}
