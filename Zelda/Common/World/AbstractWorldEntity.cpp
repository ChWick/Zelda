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

#include "AbstractWorldEntity.hpp"
#include <OgreSceneNode.h>
#include <ParticleUniverseSystemManager.h>
#include <BulletCollision/CollisionDispatch/btCollisionObject.h>
#include <string>
#include "../Physics/BtOgreExtras.hpp"
#include "../Physics/BtOgrePG.hpp"
#include "../Physics/PhysicsMasks.hpp"
#include "../Util/DeleteSceneNode.hpp"
#include "../GameLogic/Events/Event.hpp"
#include "../GameLogic/Events/Emitter/EmitOnCollision.hpp"
#include "../GameLogic/Events/Emitter/EmitOnReceivedDamage.hpp"
#include "WorldEntityConstructionInfo.hpp"
#include "AbstractMap.hpp"
#include "AbstractMapPack.hpp"
#include "Damage.hpp"

using XMLHelper::Attribute;

CAbstractWorldEntity::CAbstractWorldEntity(const std::string &sID,
                                           CEntity *pParent,
                                           CAbstractMap *pMap,
                                           const std::string &sResourceGroup)
  : CEntity(sID,
            pParent,
            (sResourceGroup == Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME && pMap && pMap->getMapPack().get()) ? pMap->getMapPack()->getResourceGroup() : sResourceGroup),
    m_pSceneNode(nullptr),
    m_pCollisionObject(nullptr),
    m_pMap(pMap),
    mCollisionMask(MASK_STATIC_COLLIDES_WITH),
    mCollisionGroup(COL_STATIC) {
}

CAbstractWorldEntity::CAbstractWorldEntity(CEntity *pParent,
                                           CAbstractMap *pMap,
                                           const tinyxml2::XMLElement *pElem,
                                           const std::string &sResourceGroup)
  : CEntity(pParent, pElem, (sResourceGroup == Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME && pMap && pMap->getMapPack().get()) ? pMap->getMapPack()->getResourceGroup() : sResourceGroup),
    m_pSceneNode(nullptr),
    m_pCollisionObject(nullptr),
    m_pMap(pMap),
    mCollisionMask(CPhysicsMasksIdMap::getSingleton().parseString(Attribute(pElem, "collision_mask", "static"))),
    mCollisionGroup(CPhysicsGroupsIdMap::getSingleton().parseString(Attribute(pElem, "collision_group", CPhysicsGroupsIdMap::getSingleton().toString(COL_STATIC)))) {
}

CAbstractWorldEntity::CAbstractWorldEntity(CAbstractWorldEntity *parent,
                           const CWorldEntityConstructionInfo &info)
    : CEntity(parent, info),
      CHitableInterface(info),
      CAttackerInterface(info),
      m_pSceneNode(nullptr),
      m_pCollisionObject(nullptr),
      m_pMap(parent->getMap()),
      mCollisionMask(info.getCollisionMask()),
      mCollisionGroup(info.getCollisionGroup()) {
}

CAbstractWorldEntity::CAbstractWorldEntity(
    const std::string &sID,
    CAbstractWorldEntity *parent,
    const CWorldEntityConstructionInfo &info)
    : CEntity(sID, parent, info),
      CHitableInterface(info),
      CAttackerInterface(info),
      m_pSceneNode(nullptr),
      m_pCollisionObject(nullptr),
      m_pMap(parent->getMap()),
      mCollisionMask(info.getCollisionMask()),
      mCollisionGroup(info.getCollisionGroup()) {
}

CAbstractWorldEntity::~CAbstractWorldEntity() {
}

void CAbstractWorldEntity::exit() {
  while (mParticleSystems.size() > 0) {
    ParticleUniverse::ParticleSystemManager::getSingleton().
        destroyParticleSystem(mParticleSystems.front(),
                              getMap()->getSceneNode()->getCreator());
    mParticleSystems.pop_front();
  }

  if (m_pSceneNode) {
    destroySceneNode(m_pSceneNode, true);
    m_pSceneNode = nullptr;
  }
  if (m_pCollisionObject) {
    m_pMap->getPhysicsManager()->deleteNow(m_pCollisionObject);
    m_pCollisionObject = nullptr;
  }

  CEntity::exit();
}

void CAbstractWorldEntity::start() {
  CEntity::start();
  for (auto *particleSystem : mParticleSystems) {
    particleSystem->start();
  }
}

void CAbstractWorldEntity::stop() {
  for (auto *particleSystem : mParticleSystems) {
    particleSystem->stop();
  }
  CEntity::stop();
}

ParticleUniverse::ParticleSystem *CAbstractWorldEntity::createParticleSystem(
    const std::string &name_prefix,
    const std::string &type,
    bool autoAttach) {
  ParticleUniverse::ParticleSystem *s
      = ParticleUniverse::ParticleSystemManager::getSingleton().
      createParticleSystem(getID() + name_prefix, type,
                           getMap()->getSceneNode()->getCreator());

  if (autoAttach) {
    ASSERT(m_pSceneNode);
    m_pSceneNode->attachObject(s);
  }

  mParticleSystems.push_back(s);

  return s;
}

void CAbstractWorldEntity::deleteParticleSystemLater(
    ParticleUniverse::ParticleSystem *s) {
  mEntityMutex.lock();
  ASSERT(std::find(mParticleSystems.begin(),
                   mParticleSystems.end(),
                   s)
         != mParticleSystems.end());
  mParticleSystemsToDelete.push_back(s);
  mEntityMutex.unlock();
}

void CAbstractWorldEntity::deleteParticleSystemNow(
    ParticleUniverse::ParticleSystem *s) {
  auto it = std::find(mParticleSystems.begin(),
                      mParticleSystems.end(),
                      s);
  ASSERT(it != mParticleSystems.end());

  s->detachFromParent();

  ParticleUniverse::ParticleSystemManager::getSingleton().
        destroyParticleSystem(s,
                              getMap()->getSceneNode()->getCreator());

  mParticleSystems.erase(it);
}

void CAbstractWorldEntity::warp(const SPATIAL_VECTOR &p,
                                const Ogre::Quaternion &q) {
  bool attached(false);
  if (m_pCollisionObject) {
    // an attached object has to be readded to the collision world
    attached = m_pMap->getPhysicsManager()->hasCollisionObject(m_pCollisionObject);
    if (attached) {
      m_pMap->getPhysicsManager()->secureRemoveCollisionObject(m_pCollisionObject);
    }
  }

  // normally set the position and orientation
  setFloorPosition(p);
  setOrientation(q);

  if (attached) {
    // add it again
    m_pMap->getPhysicsManager()->secureAddCollisionObject(m_pCollisionObject, mCollisionGroup, mCollisionMask);
  }
}

const SPATIAL_VECTOR &CAbstractWorldEntity::getPosition() const {
  if (m_pSceneNode) {
    return m_pSceneNode->getPosition();
  }

  if (m_pCollisionObject) {
    return BtOgre::Convert::toOgre(m_pCollisionObject->getWorldTransform().getOrigin());
  }

  return CEntity::getPosition();
}

void CAbstractWorldEntity::setPosition(const SPATIAL_VECTOR &vPos) {
  if (m_pCollisionObject) {
    if (btRigidBody::upcast(m_pCollisionObject)
        && dynamic_cast<BtOgre::RigidBodyState*>(
            btRigidBody::upcast(m_pCollisionObject)->getMotionState())) {
      BtOgre::RigidBodyState *pRBS = dynamic_cast<BtOgre::RigidBodyState*>(
          btRigidBody::upcast(m_pCollisionObject)->getMotionState());

      m_pCollisionObject->getWorldTransform().setOrigin(
          pRBS->getCenterOfMassOffset().inverse()
          * BtOgre::Convert::toBullet(vPos)
          - pRBS->getLocalTransform().getOrigin());
      pRBS->setWorldTransform(m_pCollisionObject->getWorldTransform());
    } else {
      m_pCollisionObject->getWorldTransform().setOrigin(
          BtOgre::Convert::toBullet(vPos));
      m_pSceneNode->setPosition(vPos);
    }
  } else {
    m_pSceneNode->setPosition(vPos);
  }
}

void CAbstractWorldEntity::translate(const SPATIAL_VECTOR &vOffset) {
  m_pSceneNode->translate(vOffset);
}

const SPATIAL_VECTOR &CAbstractWorldEntity::getCenter() const {
  return getPosition();
}
void CAbstractWorldEntity::setCenter(const SPATIAL_VECTOR &vCenter) {
  setPosition(vCenter);
}

const SPATIAL_VECTOR &CAbstractWorldEntity::getSize() const {
  return getScale();
}
void CAbstractWorldEntity::setSize(const SPATIAL_VECTOR &vSize) {
  setScale(vSize);
}

const SPATIAL_VECTOR &CAbstractWorldEntity::getScale() const {
  return m_pSceneNode->getScale();
}
void CAbstractWorldEntity::setScale(const SPATIAL_VECTOR &vScale) {
  m_pSceneNode->setScale(vScale);
}

const Ogre::Quaternion &CAbstractWorldEntity::getOrientation() const {
  return m_pSceneNode->getOrientation();
}

void CAbstractWorldEntity::setOrientation(const Ogre::Quaternion &quat) {
  m_pSceneNode->setOrientation(quat);
}

void CAbstractWorldEntity::rotate(const Ogre::Quaternion &quat) {
  m_pSceneNode->rotate(quat);
}

Ogre::SceneNode *CAbstractWorldEntity::getSceneNode() const {
  return m_pSceneNode;
}

void CAbstractWorldEntity::setCollisionObject(btCollisionObject *pCollisionObject) {
  m_pCollisionObject = pCollisionObject;
}

btCollisionObject *CAbstractWorldEntity::getCollisionObject() const {
  return m_pCollisionObject;
}

void CAbstractWorldEntity::update(Ogre::Real tpf) {
  // delete particle systems
  mEntityMutex.lock();
  while (mParticleSystemsToDelete.size() > 0) {
    deleteParticleSystemNow(mParticleSystemsToDelete.front());
    mParticleSystemsToDelete.pop_front();
  }
  mEntityMutex.unlock();

  CHitableInterface::update(tpf);
  CEntity::update(tpf);
}

void CAbstractWorldEntity::setThisAsCollisionObjectsUserPointer() {
  setThisAsCollisionObjectsUserPointer(m_pCollisionObject);
}

void CAbstractWorldEntity::setThisAsCollisionObjectsUserPointer(btCollisionObject *pCollisionObject) {
  ASSERT(pCollisionObject);
  pCollisionObject->setUserPointer(this);
}

CAbstractWorldEntity *CAbstractWorldEntity::getFromUserPointer(btCollisionObject *pCO) {
  ASSERT(pCO);
  return static_cast<CAbstractWorldEntity*>(pCO->getUserPointer());
}

CAbstractWorldEntity *CAbstractWorldEntity::getFromUserPointer(const btCollisionObject *pCO) {
  ASSERT(pCO);
  return static_cast<CAbstractWorldEntity*>(pCO->getUserPointer());
}

CAbstractWorldEntity::SInteractionResult CAbstractWorldEntity::interactOnCollision(
    const Ogre::Vector3 &vInteractDir,
    CAbstractWorldEntity *pSender) {
  using events::CEmitOnCollision;

  for (auto &pEvt : getEvents()) {
    for (auto &pEmit : pEvt->getEmitter()) {
      if (pEmit->getType() == events::EMIT_ON_COLLISION) {
        CEmitOnCollision *pEON(dynamic_cast<CEmitOnCollision *>(pEmit));
        if (pEON->getID() == pSender->getID()) {
          pEvt->start();
        }
      }
    }
  }

  return SInteractionResult();
}

CAbstractWorldEntity::SInteractionResult CAbstractWorldEntity::interactOnActivate(const Ogre::Vector3 &vInteractDir, CAbstractWorldEntity *pSender) {
  using namespace events;
  for (auto &pEvt : getEvents()) {
    for (auto &pEmit : pEvt->getEmitter()) {
      if (pEmit->getType() == EMIT_ON_INTERACTION) {
        pEvt->start();
      }
    }
  }

  return CInteractionInterface::interactOnActivate(vInteractDir, pSender);
}

void CAbstractWorldEntity::damageAccepted(const CDamage &damage) {
  using events::CEmitOnReceivedDamage;
  for (auto &pEvt : getEvents()) {
    for (auto &pEmit : pEvt->getEmitter()) {
      if (pEmit->getType() == events::EMIT_ON_RECEIVED_DAMAGE) {
        CEmitOnReceivedDamage *pEORD(
            dynamic_cast<CEmitOnReceivedDamage *>(pEmit));
        if (pEORD->getDamageMask() & damage.getDamageType()) {
          if (!pEORD->wasKilledByDamage() || isDead()) {
            pEvt->start();
          }
        }
      }
    }
  }
}

void CAbstractWorldEntity::enterMap(CAbstractMap *pMap, const Ogre::Vector3 &vPosition) {
  m_pMap = pMap;
  m_sResourceGroup = pMap->getMapPack()->getResourceGroup();
  setPosition(vPosition);
}
