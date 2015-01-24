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

#include "AbstractMap.hpp"
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btCollisionWorld.h>
#include <OgreSceneNode.h>
#include <OgreStaticGeometry.h>
#include <OgreSceneManager.h>
#include <string>
#include "../Physics/BtOgrePG.hpp"
#include "../Log.hpp"

/**
 * Counter to make names unique if objects are switched between
 * maps since renaming a scene node is not possible
 */
int MAP_COUNTER = 0;

CAbstractMap::CAbstractMap(CEntity *parent,
                           CAbstractMapPackPtr mapPack,
                           Ogre::SceneNode *pParentSceneNode,
                           WORLD_ENTITY_CLASS *player)
    : WORLD_ENTITY_CLASS(mapPack->getName(),
                   parent,
                   this,
                   mapPack->getResourceGroup()),
      m_PhysicsManager(pParentSceneNode->getCreator()),
      m_MapPack(mapPack),
      m_pPlayer(player) {
  LOGV("Construction of map '%s'", m_MapPack->getName().c_str());
  mPrependNodeName = m_MapPack->getName()
                              + Ogre::StringConverter::toString(MAP_COUNTER++);
  // initialise static default geometry
  m_pStaticGeometry = pParentSceneNode->getCreator()->createStaticGeometry(
      m_MapPack->getName() + "_StaticGeometry");
  m_pStaticGeometry->setRegionDimensions(Ogre::Vector3(10, 10, 10));
  m_pStaticGeometry->setCastShadows(false);
}

void CAbstractMap::init() {
  WORLD_ENTITY_CLASS::init();
}

void CAbstractMap::start() {
  WORLD_ENTITY_CLASS::start();

  // build the static geometry
  m_pStaticGeometry->build();

  // remove entites only used for creating static
  for (auto entpair : m_mStaticEntitiesMap) {
    m_pSceneNode->getCreator()->destroyEntity(entpair.second);
  }
  m_mStaticEntitiesMap.clear();
}

void CAbstractMap::exit() {
  if (!m_pSceneNode) {return;}

  LOGI("Destruction of map '%s'", m_MapPack->getName().c_str());

  Ogre::SceneManager *sceneManager = m_pSceneNode->getCreator();

  sceneManager->destroyStaticGeometry(m_pStaticGeometry);
  m_pStaticGeometry = nullptr;

  m_PhysicsManager.exit();

  // mappack is last
  m_MapPack.reset();
}

bool CAbstractMap::frameStarted(const Ogre::FrameEvent& evt) {
  if (m_bPauseUpdate) {return true;}
  m_PhysicsManager.update(evt.timeSinceLastFrame);
  processCollisionCheck();
  return WORLD_ENTITY_CLASS::frameStarted(evt);
}



void CAbstractMap::processCollisionCheck() {
  auto dispatcher = m_PhysicsManager.getWorld()->getDispatcher();

  int numManifolds = dispatcher->getNumManifolds();
  for (int i = 0; i < numManifolds; i++) {
    btPersistentManifold* contactManifold
        = dispatcher->getManifoldByIndexInternal(i);
    const btCollisionObject* obA =
        static_cast<const btCollisionObject*>(contactManifold->getBody0());
    const btCollisionObject* obB =
        static_cast<const btCollisionObject*>(contactManifold->getBody1());

    int numContacts = contactManifold->getNumContacts();
    for (int j = 0; j < numContacts; j++) {
      btManifoldPoint& pt = contactManifold->getContactPoint(j);
      if (pt.getDistance() < 0.f) {
        // Contact of 2 objects
        WORLD_ENTITY_CLASS *pWE_A(
            WORLD_ENTITY_CLASS::getFromUserPointer(obA));
        WORLD_ENTITY_CLASS *pWE_B(
            WORLD_ENTITY_CLASS::getFromUserPointer(obB));

        btVector3 vDistance(pt.m_positionWorldOnA - pt.m_positionWorldOnB);
        if (vDistance.fuzzyZero()) {
            continue;
        }
        vDistance.normalize();
        if (pWE_A && pWE_B) {
          pWE_A->interactOnCollision(BtOgre::Convert::toOgre(vDistance),
                                     pWE_B);
          pWE_B->interactOnCollision(-BtOgre::Convert::toOgre(vDistance),
                                     pWE_A);
        }
      }
    }
  }
}

void CAbstractMap::addStaticEntity(const std::string &entity,
                                   const Ogre::Vector3 &vPosition,
                                   const Ogre::Quaternion &vRotation) {
  if (m_mStaticEntitiesMap.find(entity) == m_mStaticEntitiesMap.end()) {
    m_mStaticEntitiesMap[entity]
        = m_pSceneNode->getCreator()->createEntity(entity);
  }

  m_pStaticGeometry->addEntity(m_mStaticEntitiesMap.at(entity),
                               vPosition,
                               vRotation);
}
