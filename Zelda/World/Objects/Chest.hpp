#ifndef _CHEST_HPP_
#define _CHEST_HPP_

#include "../WorldEntity.hpp"

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
  Ogre::SceneNode *mLidSceneNode;
  const EChestType mChestType;
  EStatus mStatus;
public:
  CChest(const std::string &sID, CWorldEntity *pParent, CMap *pMap, EChestType chestType);

  void start();

  void update(Ogre::Real);

private:

  SInteractionResult interactOnActivate(const Ogre::Vector3 &vInteractDir, CWorldEntity *pSender);

  void open();
};

#endif // _CHEST_HPP_
