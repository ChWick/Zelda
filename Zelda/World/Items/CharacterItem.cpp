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
#include <ParticleUniverseSystem.h>
#include <BulletCollision/CollisionShapes/btBoxShape.h>
#include <BulletCollision/CollisionShapes/btCollisionShape.h>
#include <BulletCollision/CollisionShapes/btTriangleShape.h>
#include <BulletCollision/CollisionShapes/btCompoundShape.h>
#include <BulletCollision/CollisionShapes/btConvexHullShape.h>
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
  mBlockPhysics(nullptr),
  mDamagePhysics(nullptr),
  mContactResultCallback(this) {
  ASSERT(character);
  ASSERT(boneToAttach.size() > 0);

  if (mCharacter->getCharacterData().mAttitude == ATTITUDE_FRIENDLY) {
    mBlockPhysicsMask = MASK_SHIELD_P_COLLIDES_WITH;
    mBlockPhysicsGroup = COL_SHIELD_P;

    mCollisionMask = MASK_DAMAGE_P_COLLIDES_WITH;
    mCollisionGroup = COL_DAMAGE_P;
  } else {
    mBlockPhysicsMask = MASK_SHIELD_N_COLLIDES_WITH;
    mBlockPhysicsGroup = COL_SHIELD_N;

    mCollisionMask = MASK_DAMAGE_N_COLLIDES_WITH;
    mCollisionGroup = COL_DAMAGE_N;
  }

  const SItemVariantData &itemData(
      CItemVariantDataMap::getSingleton().toData(type));

  mAttachedMesh = mCharacter->getSceneNode()
      ->getCreator()->createEntity(itemData.sBasicMeshName);
  mCharacter->getBodyEntity()
      ->attachObjectToBone(boneToAttach,
                           mAttachedMesh,
                           Ogre::Quaternion(Ogre::Degree(0),
                                            Ogre::Vector3::UNIT_X));
  createPhysics(mCharacter->getMap());

  mContactResultCallback.init();

  // create particle systmes
  for (const CParticleSystemConstructionInfo &pdata : itemData.mParticleData) {
    // do not attach it automatically to scene node, we attach it to the bone
    auto p = createParticleSystem(
        "_ps" + Ogre::StringConverter::toString(mParticleSystems.size()),
        pdata.getType(),
        false);
    mCharacter->getBodyEntity()
      ->attachObjectToBone(boneToAttach,
                           p,
                           Ogre::Quaternion(Ogre::Degree(0),
                                            Ogre::Vector3::UNIT_X));
    p->setScale(Ogre::Vector3::UNIT_SCALE * 0.02);
    p->setScaleVelocity(0.02);
  }

  start();
}

CCharacterItem::~CCharacterItem() {
}

void CCharacterItem::exit() {
  ASSERT(mCharacter);
  destroyPhysics(mCharacter->getMap());
  mCharacter->getBodyEntity()->detachObjectFromBone(mAttachedMesh);
  mCharacter->getSceneNode()->getCreator()->destroyEntity(mAttachedMesh);

  for (auto m : mParticleSystems) {
    // detach particle system
    mCharacter->getBodyEntity()->detachObjectFromBone(m);
  }
  CWorldEntity::exit();
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

void CCharacterItem::enterNewMap(CAbstractMap *oldMap, CAbstractMap *newMap) {
  destroyPhysics(oldMap);
  createPhysics(newMap);
}


void CCharacterItem::createPhysics(CAbstractMap *map) {
  destroyPhysics(map);  // check, that deleted, e.g. when switching

  CPhysicsManager *physicsManager = map->getPhysicsManager();

  if (!CItemVariantDataMap::getSingleton().toData(mVariantType)
      .vBlockPhysicsSize.isZeroLength()) {
    btCollisionShape *shape = nullptr;
    mPhysicsOffset
        = CItemVariantDataMap::getSingleton().
        toData(mVariantType).vBlockPhysicsOffset;
    shape = new btBoxShape(BtOgre::Convert::toBullet(
        CItemVariantDataMap::getSingleton().
        toData(mVariantType).vBlockPhysicsSize));
    mBlockPhysics = new btRigidBody(0,
                                    new btDefaultMotionState(),
                                    shape);

    physicsManager->secureAddRigidBody(mBlockPhysics,
                                       mBlockPhysicsGroup,
                                       mBlockPhysicsMask);

    mCharacter->setThisAsCollisionObjectsUserPointer(mBlockPhysics);
  }

  if (mVariantType == ITEM_VARIANT_SWORD_SIMPLE) {
    mDamagePhysics = new btRigidBody(0,
                                     new btDefaultMotionState(),
                                     nullptr);
    physicsManager->secureAddRigidBody(mBlockPhysics,
                                       COL_NOTHING,
                                       MASK_NONE);
  }
}

void CCharacterItem::destroyPhysics(CAbstractMap *map) {
  CPhysicsManager *physicsManager = map->getPhysicsManager();

  if (mBlockPhysics) {
    physicsManager->deleteNow(mBlockPhysics);
    mBlockPhysics = nullptr;
  }
  if (mDamagePhysics) {
    if (mDamagePhysics->getCollisionShape()) {
      delete mDamagePhysics->getCollisionShape();
      mDamagePhysics->setCollisionShape(nullptr);
    }
    physicsManager->deleteNow(mDamagePhysics);
    mDamagePhysics = nullptr;
  }
}

void CCharacterItem::startDamage() {
  Ogre::Entity *body(mCharacter->getBodyEntity());
  const Ogre::Vector3 vDir(
      body->getParentNode()->convertLocalToWorldOrientation(
          body->getSkeleton()->getBone(mBoneToAttach)
          ->_getDerivedOrientation()).yAxis()
      * CItemVariantDataMap::getSingleton().toData(mVariantType).fLength);
  const Ogre::Vector3 vPos(
      body->getParentNode()->convertLocalToWorldPosition(
          body->getSkeleton()->getBone(mBoneToAttach)
          ->_getDerivedPosition()));
  mOldDamageStartPos = vPos;
  mOldDamageEndPos = vDir + vPos;
}

void CCharacterItem::updateDamage(Ogre::Real tpf) {
  btTransform from(btTransform::getIdentity());
  btTransform to(btTransform::getIdentity());

  Ogre::Entity *body(mCharacter->getBodyEntity());

  const Ogre::Vector3 vDir(
      body->getParentNode()->convertLocalToWorldOrientation(
          body->getSkeleton()->getBone(mBoneToAttach)
          ->_getDerivedOrientation()).yAxis()
      * CItemVariantDataMap::getSingleton().toData(mVariantType).fLength);
  const Ogre::Vector3 vPos(
      body->getParentNode()->convertLocalToWorldPosition(
          body->getSkeleton()->getBone(mBoneToAttach)
          ->_getDerivedPosition()));
  mOldDamageStartPos = vPos;
  mOldDamageEndPos = vDir + vPos;

  if (mVariantType == ITEM_VARIANT_SWORD_SIMPLE) {
    // create the new collision shape
    btConvexHullShape *shape = new btConvexHullShape();
    shape->addPoint(BtOgre::Convert::toBullet(vPos));
    shape->addPoint(BtOgre::Convert::toBullet(vPos + vDir));
    shape->addPoint(BtOgre::Convert::toBullet(mOldDamageEndPos));
    shape->addPoint(BtOgre::Convert::toBullet(mOldDamageStartPos));
    shape->setMargin(0);
    mDamagePhysics->setCollisionShape(shape);

    // perform collision check
    CAbstractMap *map = mCharacter->getMap();
    btCollisionWorld *physWorld = map->getPhysicsManager()->getWorld();
    physWorld->contactTest(mDamagePhysics, mContactResultCallback);

    // and delete it
    delete mDamagePhysics->getCollisionShape();
    mDamagePhysics->setCollisionShape(nullptr);
  } else {
    if (mCharacter->createDamage(Ogre::Ray(vPos, vDir),
                               createDamage()) == false) {
      // no hit, check for low framerates
      mCharacter->createDamage(Ogre::Ray(vPos + vDir,
                                        mOldDamageStartPos), createDamage());
    }
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

  for (auto p : mParticleSystems) {
    p->setVisible(true);
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

  for (auto p : mParticleSystems) {
    //p->setVisible(false);
  }
}

CDamage CCharacterItem::createDamage() {
  return CDamage(
      mCharacter,
      CItemVariantDataMap::getSingleton().toData(mVariantType).eDamageType,
      mCharacter->getOrientation().zAxis(),
      CItemVariantDataMap::getSingleton().toData(mVariantType).uiDamage,
      0);
}

Ogre::Vector3 CCharacterItem::getDamagePosition() {
  return Ogre::Vector3::ZERO;
}


CCharacterItem::EReceiveDamageResult CCharacterItem::hit(
    const CDamage &damage) {
  return RDR_BLOCKED;
}




CCharacterItemContactResultCallback::CCharacterItemContactResultCallback(CCharacterItem *c)
  : mCharacterItem(c) {
}

void CCharacterItemContactResultCallback::init() {
  m_collisionFilterGroup = mCharacterItem->getCollisionGroup();
  m_collisionFilterMask = mCharacterItem->getCollisionMask();
}

btScalar CCharacterItemContactResultCallback::addSingleResult(btManifoldPoint &cp,
                           const btCollisionObjectWrapper *colObj0Wrap,
                           int partId0,
                           int index0,
                           const btCollisionObjectWrapper *colObj1Wrap,
                           int partId1,
                           int index1) {

  collision(colObj0Wrap->getCollisionObject());
  collision(colObj1Wrap->getCollisionObject());
  return 0;
}

void CCharacterItemContactResultCallback::collision(const btCollisionObject *obj) {
  if (obj != mCharacterItem->getBlockPhysics()
      && obj != mCharacterItem->getDamagePhysics()
      && obj != mCharacterItem->getCharacter()->getCollisionObject()) {
    CAbstractWorldEntity *pWE
        = CAbstractWorldEntity::getFromUserPointer(obj);
    if (pWE) {
      // let the character attack this entity
      mCharacterItem->getCharacter()->attack(mCharacterItem->createDamage(), pWE);
    }
  }
}
