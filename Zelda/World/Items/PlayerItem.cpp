#include "PlayerItem.hpp"
#include "ItemData.hpp"
#include <OgreBone.h>
#include <OgreEntity.h>
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include "../Character/Character.hpp"

CCharacterItem::CCharacterItem(CCharacter &character, const std::string &boneToAttach, EItemVariantTypes type)
  : mCharacter(character),
    mVariantType(type) {
  mAttachedMesh = mCharacter.getSceneNode()->getCreator()->createEntity(ITEM_VARIANT_DATA_MAP.toData(type).sBasicMeshName);
  mCharacter.getBodyEntity()->attachObjectToBone(boneToAttach, mAttachedMesh, Ogre::Quaternion(Ogre::Degree(0), Ogre::Vector3::UNIT_X));
}

CCharacterItem::~CCharacterItem() {
  mCharacter.getBodyEntity()->detachObjectFromBone(mAttachedMesh);
  mCharacter.getSceneNode()->getCreator()->destroyEntity(mAttachedMesh);
}

void CCharacterItem::show() {
  mAttachedMesh->setVisible(true);
}

void CCharacterItem::hide() {
  mAttachedMesh->setVisible(false);
}

CDamage CCharacterItem::createDamage() {
  return CDamage(DMG_ALL, mCharacter.getOrientation().zAxis(), HP_FULL_HEART, 0);
}

Ogre::Vector3 CCharacterItem::getDamagePosition() {
  return Ogre::Vector3::ZERO;
}
