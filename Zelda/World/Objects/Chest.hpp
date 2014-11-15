#ifndef _CHEST_HPP_
#define _CHEST_HPP_

#include "../WorldEntity.hpp"
#include "ObjectTypes.hpp"
#include "../../Common/PauseManager/PauseCaller.hpp"

class CChest : public CWorldEntity,
               public CPauseCaller {
public:
  enum EChestType {
    SMALL_CHEST,
    BIG_CHEST,
  };
private:
  enum EStatus {
    STATUS_CLOSED,
    STATUS_OPENING,
    STATUS_OPENED,
  };
  static const Ogre::Real INNER_OBJECT_LIFT_HEIGHT;
  static const Ogre::Real INNER_OBJECT_TIME_TO_LIFT;
  static const Ogre::Real INNER_OBJECT_TIME_TO_SET_IN_LIFT;

  Ogre::SceneNode *mLidSceneNode;
  Ogre::Vector3 mPhysicsOffset;
  const EChestType mChestType;
  EStatus mStatus;
  bool mLifting;
  CWorldEntity *mInnerObject;
  EObjectTypes mInnerObjectType;
  std::string mTextMessage;
  Ogre::Real mTimer;
public:
  CChest(const std::string &sID, CWorldEntity *pParent, CMap *pMap, EChestType chestType);

  void start();
  void update(Ogre::Real);
  void pauseUpdate(Ogre::Real);

  void setInnerObject(EObjectTypes eType) {mInnerObjectType = eType;}
  void createInnerObject(EObjectTypes eType);
private:

  SInteractionResult interactOnActivate(const Ogre::Vector3 &vInteractDir, CWorldEntity *pSender);
  void handleMessage(const CMessage &message);

  void open();
  void onLifted();
  void onFinished();
};

#endif // _CHEST_HPP_
