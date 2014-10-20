#include "PlayerItem.hpp"
#include "ItemData.hpp"
#include <OgreBone.h>
#include <OgreEntity.h>
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include "../Character/Player.hpp"

CPlayerItem::CPlayerItem(CPlayer &player, const std::string &boneToAttach, EItemVariantTypes type)
  : mPlayer(player),
    mVariantType(type) {
  mAttachedMesh = player.getSceneNode()->getCreator()->createEntity(ITEM_VARIANT_DATA_MAP.toData(type).sBasicMeshName);
  player.getBodyEntity()->attachObjectToBone(boneToAttach, mAttachedMesh, Ogre::Quaternion(Ogre::Degree(90), Ogre::Vector3::UNIT_X));
}

CPlayerItem::~CPlayerItem() {
  mPlayer.getBodyEntity()->detachObjectFromBone(mAttachedMesh);
  mPlayer.getSceneNode()->getCreator()->destroyEntity(mAttachedMesh);
}

void CPlayerItem::show() {
  mAttachedMesh->setVisible(true);
}

void CPlayerItem::hide() {
  mAttachedMesh->setVisible(false);
}

CDamage CPlayerItem::createDamage() {
  return CDamage(DMG_ALL, mPlayer.getOrientation().zAxis(), HP_FULL_HEART, 0);
}

Ogre::Vector3 CPlayerItem::getDamagePosition() {
  return Ogre::Vector3::ZERO;
}
