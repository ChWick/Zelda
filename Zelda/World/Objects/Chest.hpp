#ifndef _CHEST_HPP_
#define _CHEST_HPP_

#include "../WorldEntity.hpp"
#include "ObjectTypes.hpp"

class CChest : public CWorldEntity {
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
  const EChestType mChestType;
  EStatus mStatus;
  bool mLifting;
  CWorldEntity *mInnerObject;
  Ogre::Real mTimer;
public:
  CChest(const std::string &sID, CWorldEntity *pParent, CMap *pMap, EChestType chestType);

  void start();

  void update(Ogre::Real);

  void createInnerObject(EObjectTypes eType);
private:

  SInteractionResult interactOnActivate(const Ogre::Vector3 &vInteractDir, CWorldEntity *pSender);

  void open();
};

#endif // _CHEST_HPP_
