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

#include "Chest.hpp"
#include "Object.hpp"
#include "../Atlas/Map.hpp"
#include "../GlobalCollisionShapesTypes.hpp"
#include "../../Common/Physics/PhysicsManager.hpp"
#include "../../Common/Physics/PhysicsMasks.hpp"
#include "../../Common/Physics/BtOgreExtras.hpp"
#include "../../Common/Physics/BtOgrePG.hpp"
#include "../../Common/Message/MessageHandler.hpp"
#include "../../Common/Message/MessagePlayerPickupItem.hpp"
#include "../../Common/Util/Assert.hpp"
#include "../Messages/MessageShowText.hpp"
#include "../Messages/UserMessageTypes.hpp"
#include "../../Common/XMLResources/Manager.hpp"
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>
#include <OgreEntity.h>
#include "../../TextConverter.hpp"

const Ogre::Real CChest::INNER_OBJECT_LIFT_HEIGHT(0.15);
const Ogre::Real CChest::INNER_OBJECT_TIME_TO_LIFT(2);
const Ogre::Real CChest::INNER_OBJECT_TIME_TO_SET_IN_LIFT(1);

CChest::CChest(const std::string &sID, CWorldEntity *pParent, CMap *pMap, EChestType chestType)
  : CWorldEntity(sID, pParent, pMap, pParent->getResourceGroup()),
    mPhysicsOffset(Ogre::Vector3::ZERO),
    mChestType(chestType),
    mStatus(STATUS_CLOSED),
    mLifting(false),
    mInnerObject(nullptr),
    mInnerObjectType(OBJECT_COUNT),
    mTimer(0) {
  m_pSceneNode = pParent->getSceneNode()->createChildSceneNode(sID);
  mLidSceneNode = m_pSceneNode->createChildSceneNode(sID + "_lid");
  CPhysicsCollisionObject *pCO(nullptr);
  switch (mChestType) {
  case SMALL_CHEST:
    mLidSceneNode->attachObject(m_pSceneNode->getCreator()->createEntity("small_chest_top.mesh"));
    pCO = &m_pMap->getPhysicsManager()->getCollisionShape(GLOBAL_COLLISION_SHAPES_TYPES_ID_MAP.toString(GCST_SMALL_CHEST_TOP));
    break;
  case BIG_CHEST:
    break;
  }
  ASSERT(pCO);
  mPhysicsOffset = pCO->getOffset();
  btRigidBody::btRigidBodyConstructionInfo info();
  btRigidBody *pRB = new btRigidBody(0,
                                     new btDefaultMotionState(),
                                     pCO->getShape(),
                                     BtOgre::Convert::toBullet(mLidSceneNode->_getDerivedPosition()));
  m_pCollisionObject = pRB;
  setThisAsCollisionObjectsUserPointer(m_pCollisionObject);
  m_pMap->getPhysicsManager()->getWorld()->addRigidBody(pRB, COL_INTERACTIVE, MASK_INTERACIVE_OBJECT_COLLIDES_WITH);
}

void CChest::start() {
  switch (mChestType) {
  case SMALL_CHEST:
    m_pSceneNode->translate(Ogre::Vector3(0, 0.04, -0.005) * 0.5);
    break;
  case BIG_CHEST:
    break;
  }
  m_pCollisionObject->getWorldTransform().setOrigin(BtOgre::Convert::toBullet(mLidSceneNode->_getDerivedPosition()));
}

void CChest::update(Ogre::Real tpf) {
  if (mStatus == STATUS_OPENING) {
    // rotate lid
    mLidSceneNode->pitch(Ogre::Radian(-tpf));
    if (mLidSceneNode->getOrientation().getPitch().valueDegrees() < -90) {
      mStatus = STATUS_OPENED;
    }
  }

  // lift inner object
  if (mLifting) {
    mTimer += tpf;
    if (mTimer > INNER_OBJECT_TIME_TO_SET_IN_LIFT) {
      if (mTimer > INNER_OBJECT_TIME_TO_LIFT + INNER_OBJECT_TIME_TO_SET_IN_LIFT) {
        tpf = tpf - (mTimer - INNER_OBJECT_TIME_TO_LIFT - INNER_OBJECT_TIME_TO_SET_IN_LIFT);
        mLifting = false;
        onLifted();
      }
      mInnerObject->translate(Ogre::Vector3(0, tpf / INNER_OBJECT_TIME_TO_LIFT * INNER_OBJECT_LIFT_HEIGHT, 0));
    }
  }
  m_pCollisionObject->getWorldTransform().setOrigin(BtOgre::Convert::toBullet(mLidSceneNode->convertLocalToWorldPosition(mPhysicsOffset)));
  m_pCollisionObject->getWorldTransform().setRotation(BtOgre::Convert::toBullet(mLidSceneNode->_getDerivedOrientation()));
}

void CChest::pauseUpdate(Ogre::Real tpf) {
  update(tpf);
  CWorldEntity::pauseUpdate(tpf);
}

CWorldEntity::SInteractionResult CChest::interactOnActivate(const Ogre::Vector3 &vInteractDir, CWorldEntity *pSender) {
  if (mStatus != STATUS_CLOSED) {
    return CWorldEntity::IR_NONE;
  }
  if (abs(vInteractDir.angleBetween(-getOrientation().zAxis()).valueDegrees()) < 5) {
    // player has to be in front of the chest
    open();
  }
  return CWorldEntity::interactOnActivate(vInteractDir, pSender);
}

void CChest::handleMessage(const CMessage &message) {
  if (message.getType() == MSG_SHOW_TEXT) {
    const CMessageShowText &msg_st(dynamic_cast<const CMessageShowText&>(message));
    if (msg_st.getStatus() == CMessageShowText::FINISHED) {
      onFinished();
    }
  }
}

void CChest::open() {
  pause(PAUSE_ALL_ATLAS_UPDATE);
  mStatus = STATUS_OPENING;
  createInnerObject(mInnerObjectType);
}

void CChest::onLifted() {
  if (mTextMessage.size() > 0) {
    CMessageHandler::getSingleton().addMessage(new CMessageShowText(mTextMessage));
  }
  else {
    onFinished();
  }
}

void CChest::onFinished() {
  unpause(PAUSE_ALL);
  mInnerObject->deleteLater();
  CMessageHandler::getSingleton().addMessage(new CMessagePlayerPickupItem(mInnerObjectType));
}

void CChest::createInnerObject(EObjectTypes eType) {
  if (eType == OBJECT_COUNT) {
    return; // no object inside
  }
  mLifting = true;
  if (XMLResources::GLOBAL.hasString(OBJECT_TYPE_ID_MAP.toData(eType).sID)) {
    mTextMessage = "${" + OBJECT_TYPE_ID_MAP.toData(eType).sID + "}";
  }
  else {
    mTextMessage = "";
    LOGV("%s", ("No item text for chest object " + OBJECT_TYPE_ID_MAP.toData(eType).sID).c_str());
  }
  mInnerObject = new CObject(getID() + "_inner_object", this, m_pMap, eType);
  // dont init CObject, since we handle location
  switch (mChestType) {
  case SMALL_CHEST:
    mInnerObject->translate(Ogre::Vector3(0, -0.03, 0.045) * 0.5);
    break;
  case BIG_CHEST:
    break;
  }
}
