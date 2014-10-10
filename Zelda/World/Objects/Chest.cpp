#include "Chest.hpp"
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>
#include <OgreEntity.h>

CChest::CChest(const std::string &sID, CWorldEntity *pParent, CMap *pMap, EChestType chestType)
  : CWorldEntity(sID, pParent, pMap, pParent->getResourceGroup()),
    mChestType(chestType) {
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
  mLidSceneNode->pitch(Ogre::Radian(-tpf));
}
