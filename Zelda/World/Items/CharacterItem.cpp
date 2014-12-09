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
#include <BulletCollision/CollisionShapes/btBoxShape.h>
#include <BulletCollision/CollisionShapes/btCollisionShape.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>
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
: CWorldEntity(character->getID() + "item_at_" + boneToAttach,
               character,
               character->getMap()),
  mCharacter(character),
  mVariantType(type),
  mBoneToAttach(boneToAttach),
  mBlockPhysics(nullptr) {
  ASSERT(character);
  ASSERT(boneToAttach.size() > 0);

  if (mCharacter->getAttitude() == ATTITUDE_FRIENDLY) {
    mBlockPhysicsMask = MASK_SHIELD_P_COLLIDES_WITH;
    mBlockPhysicsGroup = COL_SHIELD_P;
  } else {
    mBlockPhysicsMask = MASK_SHIELD_N_COLLIDES_WITH;
    mBlockPhysicsGroup = COL_SHIELD_N;
  }

  mAttachedMesh = mCharacter->getSceneNode()
      ->getCreator()->createEntity(
          ITEM_VARIANT_DATA_MAP.toData(type).sBasicMeshName);
  mCharacter->getBodyEntity()
      ->attachObjectToBone(boneToAttach,
                           mAttachedMesh,
                           Ogre::Quaternion(Ogre::Degree(0),
                                            Ogre::Vector3::UNIT_X));
  createPhysics(mCharacter->getMap());
}

CCharacterItem::~CCharacterItem() {
  ASSERT(mCharacter);
  destroyPhysics(mCharacter->getMap());
  mCharacter->getBodyEntity()->detachObjectFromBone(mAttachedMesh);
  mCharacter->getSceneNode()->getCreator()->destroyEntity(mAttachedMesh);
}

void CCharacterItem::update(Ogre::Real tpf) {
  if (mBlockPhysics) {
    // update physics
    Ogre::Entity *body(mCharacter->getBodyEntity());
    const Ogre::Vector3 position(
        body->getParentNode()->convertLocalToWorldPosition(
            body->getSkeleton()->getBone(mBoneToAttach)
            ->_getDerivedPosition()));
    const Ogre::Quaternion rotation(
        body->getParentNode()->convertLocalToWorldOrientation(
            body->getSkeleton()->getBone(mBoneToAttach)
            ->_getDerivedOrientation()));
    mBlockPhysics->setWorldTransform(
        btTransform(BtOgre::Convert::toBullet(rotation),
                    BtOgre::Convert::toBullet(position
                                              + rotation * mPhysicsOffset)));
  }
  CWorldEntity::update(tpf);
}

void CCharacterItem::enterNewMap(CMap *oldMap, CMap *newMap) {
  destroyPhysics(oldMap);
  createPhysics(newMap);
}


void CCharacterItem::createPhysics(CMap *map) {
  if (ITEM_VARIANT_DATA_MAP.toData(mVariantType)
      .vBlockPhysicsSize.isZeroLength()) {
    return;
  }

  destroyPhysics(map);  // check, that deleted, e.g. when switching

  btCollisionShape *shape = nullptr;
  mPhysicsOffset
      = ITEM_VARIANT_DATA_MAP.toData(mVariantType).vBlockPhysicsOffset;
  shape = new btBoxShape(BtOgre::Convert::toBullet(
      ITEM_VARIANT_DATA_MAP.toData(mVariantType).vBlockPhysicsSize));
  mBlockPhysics = new btRigidBody(0,
                                  new btDefaultMotionState(),
                                  shape);
  CPhysicsManager *physicsManager = map->getPhysicsManager();

  physicsManager->secureAddRigidBody(mBlockPhysics,
                                     mBlockPhysicsGroup,
                                     mBlockPhysicsMask);

  mCharacter->setThisAsCollisionObjectsUserPointer(mBlockPhysics);
}

void CCharacterItem::destroyPhysics(CMap *map) {
  if (!mBlockPhysics) {return;}  // not created

  CPhysicsManager *physicsManager = map->getPhysicsManager();

  delete mBlockPhysics->getMotionState();
  delete mBlockPhysics->getCollisionShape();
  physicsManager->secureRemoveRigidBody(mBlockPhysics);
  delete mBlockPhysics;
}

void CCharacterItem::startDamage() {
  Ogre::Entity *body(mCharacter->getBodyEntity());
  const Ogre::Vector3 vDir(
      body->getParentNode()->convertLocalToWorldOrientation(
          body->getSkeleton()->getBone(mBoneToAttach)
          ->_getDerivedOrientation()).yAxis()
      * ITEM_VARIANT_DATA_MAP.toData(mVariantType).fLength);
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
  setPauseRender(false);
  setPauseUpdate(false);

  if (mBlockPhysics) {
    CPhysicsManager *physicsManager
        = mCharacter->getMap()->getPhysicsManager();
    physicsManager->secureAddRigidBody(mBlockPhysics,
                                       mBlockPhysicsGroup,
                                       mBlockPhysicsMask);
  }
}

void CCharacterItem::hide() {
  mAttachedMesh->setVisible(false);
  setPauseRender(true);
  setPauseUpdate(true);

  if (mBlockPhysics) {
    CPhysicsManager *physicsManager
        = mCharacter->getMap()->getPhysicsManager();
    physicsManager->secureRemoveRigidBody(mBlockPhysics);
  }
}

CDamage CCharacterItem::createDamage() {
  return CDamage(mCharacter,
                 ITEM_VARIANT_DATA_MAP.toData(mVariantType).eDamageType,
                 mCharacter->getOrientation().zAxis(),
                 ITEM_VARIANT_DATA_MAP.toData(mVariantType).uiDamage,
                 0);
}

Ogre::Vector3 CCharacterItem::getDamagePosition() {
  return Ogre::Vector3::ZERO;
}


CCharacterItem::EReceiveDamageResult CCharacterItem::hit(
    const CDamage &damage) {
  return RDR_BLOCKED;
}
