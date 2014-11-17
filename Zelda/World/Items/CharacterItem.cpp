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

#include "CharacterItem.hpp"
#include <BulletCollision/CollisionShapes/btTriangleShape.h>
#include <string>
#include "ItemData.hpp"
#include <OgreBone.h>
#include <OgreEntity.h>
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include "../../Common/Util/Assert.hpp"
#include "../Character/Character.hpp"
#include "../Atlas/Map.hpp"
#include "../../Common/Physics/BtOgrePG.hpp"
#include "../../Common/Physics/PhysicsMasks.hpp"
#include "../../Common/Util/DebugDrawer.hpp"

CCharacterItem::CCharacterItem(CCharacter *character,
                               const std::string &boneToAttach,
                               EItemVariantTypes type)
  : mCharacter(character),
    mVariantType(type),
    mBoneToAttach(boneToAttach) {
  ASSERT(character);
  ASSERT(boneToAttach.size() > 0);

  mAttachedMesh = mCharacter->getSceneNode()
      ->getCreator()->createEntity(
          ITEM_VARIANT_DATA_MAP.toData(type).sBasicMeshName);
  mCharacter->getBodyEntity()
      ->attachObjectToBone(boneToAttach,
                           mAttachedMesh,
                           Ogre::Quaternion(Ogre::Degree(0),
                                            Ogre::Vector3::UNIT_X));
}

CCharacterItem::~CCharacterItem() {
  mCharacter->getBodyEntity()->detachObjectFromBone(mAttachedMesh);
  mCharacter->getSceneNode()->getCreator()->destroyEntity(mAttachedMesh);
}

void CCharacterItem::startDamage() {
  Ogre::Entity *body(mCharacter->getBodyEntity());
  const Ogre::Vector3 vDir(
      body->getParentNode()->convertLocalToWorldOrientation(
          body->getSkeleton()->getBone(mBoneToAttach)
          ->_getDerivedOrientation()).yAxis() * 0.08);
  const Ogre::Vector3 vPos(
      body->getParentNode()->convertLocalToWorldPosition(
          body->getSkeleton()->getBone(mBoneToAttach)
          ->_getDerivedPosition()));
  mOldDamageStartPos = vDir + vPos;
}

void CCharacterItem::updateDamage(Ogre::Real tpf) {
  btTransform from(btTransform::getIdentity());
  btTransform to(btTransform::getIdentity());

  Ogre::Entity *body(mCharacter->getBodyEntity());

  const Ogre::Vector3 vDir(
      body->getParentNode()->convertLocalToWorldOrientation(
          body->getSkeleton()->getBone(mBoneToAttach)
          ->_getDerivedOrientation()).yAxis() * 0.08);
  const Ogre::Vector3 vPos(
      body->getParentNode()->convertLocalToWorldPosition(
          body->getSkeleton()->getBone(mBoneToAttach)
          ->_getDerivedPosition()));
  mOldDamageStartPos = vDir + vPos;

  if (mCharacter->createDamage(Ogre::Ray(vPos, vDir),
                               createDamage()) == false) {
    // no hit, check for low framerates
    mCharacter->createDamage(Ogre::Ray(vPos + vDir,
                                       mOldDamageStartPos), createDamage());
  }

  DebugDrawer::getSingleton().drawLine(vPos, vPos + vDir,
                                       Ogre::ColourValue::Red);
  DebugDrawer::getSingleton().drawLine(vPos, mOldDamageStartPos,
                                       Ogre::ColourValue::Red);
}

void CCharacterItem::show() {
  mAttachedMesh->setVisible(true);
}

void CCharacterItem::hide() {
  mAttachedMesh->setVisible(false);
}

CDamage CCharacterItem::createDamage() {
  return CDamage(DMG_ALL, mCharacter->getOrientation().zAxis(),
                 HP_FULL_HEART, 0);
}

Ogre::Vector3 CCharacterItem::getDamagePosition() {
  return Ogre::Vector3::ZERO;
}
