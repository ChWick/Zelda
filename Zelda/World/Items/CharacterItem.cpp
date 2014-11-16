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
