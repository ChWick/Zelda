#include "Chest.hpp"
#include "Object.hpp"
#include "../../Common/Message/MessageHandler.hpp"
#include "../../Common/Message/MessagePlayerPickupItem.hpp"
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
    mChestType(chestType),
    mStatus(STATUS_CLOSED),
    mInnerObject(nullptr),
    mLifting(false),
    mTimer(0) {
  m_pSceneNode = pParent->getSceneNode()->createChildSceneNode(sID);
  mLidSceneNode = m_pSceneNode->createChildSceneNode(sID + "_lid");
  switch (mChestType) {
  case SMALL_CHEST:
    mLidSceneNode->attachObject(m_pSceneNode->getCreator()->createEntity("small_chest_top.mesh"));
    break;
  case BIG_CHEST:
    break;
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
  createInnerObject(OBJECT_RED_RUPEE);
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
  CMessageHandler::getSingleton().addMessage(new CMessagePlayerPickupItem(m_uiType));
}

void CChest::createInnerObject(EObjectTypes eType) {
  mInnerObjectType = eType;
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
