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
#include <string>
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

CChest::CChest(const std::string &sID,
               CWorldEntity *pParent,
               CAbstractMap *pMap,
               EChestType chestType)
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
    mLidSceneNode->attachObject(m_pSceneNode->getCreator()
                                ->createEntity("small_chest_top.mesh"));
    pCO = &m_pMap->getPhysicsManager()->getCollisionShape(
        CGlobalCollisionShapesTypesIdMap::getSingleton().
        toString(GCST_SMALL_CHEST_TOP));
    break;
  case BIG_CHEST:
    break;
  }
  ASSERT(pCO);
  mPhysicsOffset = pCO->getOffset();
  btRigidBody::btRigidBodyConstructionInfo info();
  btVector3 physicsPosition(BtOgre::Convert::toBullet(
      mLidSceneNode->_getDerivedPosition()));
  btRigidBody *pRB = new btRigidBody(0,
                                     new btDefaultMotionState(),
                                     pCO->getShape(),
                                     physicsPosition);
  m_pCollisionObject = pRB;
  setThisAsCollisionObjectsUserPointer(m_pCollisionObject);
  m_pMap->getPhysicsManager()
      ->secureAddRigidBody(pRB,
                           COL_INTERACTIVE,
                           MASK_INTERACIVE_OBJECT_COLLIDES_WITH);
}

CChest::~CChest() {
  // dont destroy chest top!
  if (m_pCollisionObject) {
    btRigidBody *pRigidBody(btRigidBody::upcast(m_pCollisionObject));
    m_pMap->getPhysicsManager()->getWorld()->removeRigidBody(pRigidBody);
    // dont delete collision shape!
    delete pRigidBody->getMotionState();
    delete pRigidBody;
    m_pCollisionObject = nullptr;
  }
}

void CChest::start() {
  switch (mChestType) {
  case SMALL_CHEST:
    m_pSceneNode->translate(Ogre::Vector3(0, 0.04, -0.005) * 0.5);
    break;
  case BIG_CHEST:
    break;
  }
  m_pCollisionObject->getWorldTransform().setOrigin(
      BtOgre::Convert::toBullet(mLidSceneNode->_getDerivedPosition()));
}

void CChest::update(Ogre::Real tpf) {
  if (mStatus == STATUS_OPENING) {
    // rotate lid
    mLidSceneNode->pitch(Ogre::Radian(-tpf));
    if (mLidSceneNode->getOrientation().getPitch().valueDegrees() < -90) {
      mStatus = STATUS_OPENED;
      if (mInnerObject == nullptr) {
        // there is no inner object, empty chest, continue
        onFinished();
      }
    }
  }

  // lift inner object
  if (mLifting) {
    mTimer += tpf;
    if (mTimer > INNER_OBJECT_TIME_TO_SET_IN_LIFT) {
      if (mTimer > INNER_OBJECT_TIME_TO_LIFT
          + INNER_OBJECT_TIME_TO_SET_IN_LIFT) {
        tpf = tpf - (mTimer - INNER_OBJECT_TIME_TO_LIFT
                     - INNER_OBJECT_TIME_TO_SET_IN_LIFT);
        mLifting = false;
        onLifted();
      } else {
        mInnerObject->translate(
            Ogre::Vector3(0,
                          tpf / INNER_OBJECT_TIME_TO_LIFT
                          *  INNER_OBJECT_LIFT_HEIGHT,
                          0));
      }
    }
  }
  m_pCollisionObject->getWorldTransform().setOrigin(
      BtOgre::Convert::toBullet(mLidSceneNode
                                ->convertLocalToWorldPosition(mPhysicsOffset)));
  m_pCollisionObject->getWorldTransform().setRotation(
      BtOgre::Convert::toBullet(mLidSceneNode->_getDerivedOrientation()));
}

void CChest::pauseUpdate(Ogre::Real tpf) {
  update(tpf);
  CWorldEntity::pauseUpdate(tpf);
}

CWorldEntity::SInteractionResult CChest::interactOnActivate(
    const Ogre::Vector3 &vInteractDir, CAbstractWorldEntity *pSender) {
  if (mStatus != STATUS_CLOSED) {
    return CWorldEntity::IR_NONE;
  }
  if (abs(vInteractDir.angleBetween(-getOrientation().zAxis())
          .valueDegrees()) < 5) {
    // player has to be in front of the chest
    open();
  }
  return CWorldEntity::interactOnActivate(vInteractDir, pSender);
}

void CChest::handleMessage(const CMessagePtr message) {
  if (message->getType() == MSG_SHOW_TEXT) {
    auto msg_st(
        std::dynamic_pointer_cast<const CMessageShowText>(message));
    if (msg_st->getStatus() == CMessageShowText::FINISHED) {
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
  mLifting = false;
  if (mTextMessage.size() > 0) {
    CMessageHandler::getSingleton().addMessage(
        std::make_shared<CMessageShowText>(__MSG_LOCATION__, mTextMessage));
  } else {
    onFinished();
  }
}

void CChest::onFinished() {
  unpause(PAUSE_ALL);
  if (mInnerObject) {
    // if the chest has an inner object
    mInnerObject->deleteLater();
    CMessageHandler::getSingleton().addMessage(
        std::make_shared<CMessagePlayerPickupItem>(__MSG_LOCATION__, mInnerObjectType));
    mInnerObject = nullptr;
  }
}

void CChest::createInnerObject(EObjectTypes eType) {
  LOGV("Creating inner object for chest: '%s'",
       CObjectTypeIdMap::getSingleton().toString(eType).c_str());
  if (eType == OBJECT_COUNT) {
    return;  // no object inside
  }
  mLifting = true;
  if (XMLResources::GLOBAL.hasString(CObjectTypeIdMap::getSingleton().toString(eType))) {
    mTextMessage = "${" + CObjectTypeIdMap::getSingleton().toString(eType) + "}";
  } else {
    mTextMessage = "";
    LOGV("%s", ("No item text for chest object "
                + CObjectTypeIdMap::getSingleton().toString(eType)).c_str());
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


  switch (eType) {
    // rotate tools with 90 degree
    case OBJECT_TOOL_LAMP:
      mInnerObject->rotate(Ogre::Quaternion(Ogre::Degree(90),
                                            Ogre::Vector3::UNIT_Z));
      mInnerObject->translate(Ogre::Vector3(0, 0, 0));
      break;
    default:
      break;
  }
}
