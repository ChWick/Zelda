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
  Ogre::SceneNode *mLidSceneNode;
  const EChestType mChestType;
public:
  CChest(const std::string &sID, CWorldEntity *pParent, CMap *pMap, EChestType chestType);

  void start();

  void update(Ogre::Real);
};

#endif // _CHEST_HPP_
