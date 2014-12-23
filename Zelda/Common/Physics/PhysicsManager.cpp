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

#include "PhysicsManager.hpp"

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <BulletCollision/CollisionShapes/btMultiSphereShape.h>
#include <BulletCollision/CollisionShapes/btCapsuleShape.h>
#include <BulletCollision/BroadphaseCollision/btOverlappingPairCache.h>
#include <BulletCollision/BroadphaseCollision/btCollisionAlgorithm.h>
#include <BulletCollision/CollisionDispatch/btCollisionWorld.h>
#include <LinearMath/btDefaultMotionState.h>
#include <BulletDynamics/Character/btCharacterControllerInterface.h>
#include <BulletDynamics/Character/btKinematicCharacterController.h>
#include "BtOgreExtras.hpp"
#include <OgreSceneManager.h>
#include <OgreLogManager.h>
#include "../Message/MessageHandler.hpp"
#include "../Message/MessageDebug.hpp"

#define PHYSICS_DEBUG 1

const float CPhysicsManager::GRAVITY_FACTOR = 4.f;

CPhysicsManager::CPhysicsManager(Ogre::SceneManager *pSceneManager)
  : m_pSceneManager(pSceneManager),
    m_pGhostPairCallback(NULL) {
  Ogre::LogManager::getSingleton().logMessage("Creating new PhysicsManager");
  m_bDisplayDebugInfo = true;

  // mBroadphase = new btDbvtBroadphase();
  btVector3 worldMin(-1000, -1000, -1000);
  btVector3 worldMax(1000, 1000, 1000);
  btAxisSweep3* sweepBP = new btAxisSweep3(worldMin, worldMax);
  mBroadphaseInterface = sweepBP;
  mCollisionConfig = new btDefaultCollisionConfiguration();
  mDispatcher = new btCollisionDispatcher(mCollisionConfig);
  mSolver = new btSequentialImpulseConstraintSolver();

  m_pPhyWorld = new btDiscreteDynamicsWorld(mDispatcher,
                                            mBroadphaseInterface,
                                            mSolver,
                                            mCollisionConfig);

  m_pPhyWorld->setGravity(btVector3(0, -GRAVITY_FACTOR, 0));
  m_pPhyWorld->getDispatchInfo().m_allowedCcdPenetration = 0.0001f;
  m_pPhyWorld->getSolverInfo().m_splitImpulse = true;

#ifdef PHYSICS_DEBUG
  m_pDbgDraw = new BtOgre::DebugDrawer(m_pSceneManager->getRootSceneNode(),
                                       m_pPhyWorld);
  m_pPhyWorld->setDebugDrawer(m_pDbgDraw);
#endif

  m_pGhostPairCallback = new btGhostPairCallback();
  mBroadphaseInterface->getOverlappingPairCache()
      ->setInternalGhostPairCallback(m_pGhostPairCallback);
  /* sweepBP->getOverlappingPairCache()
     ->setInternalGhostPairCallback(new btGhostPairCallback()); */

  toggleDisplayDebugInfo();

  Ogre::LogManager::getSingleton().logMessage("PhysicsManager created.");
}
CPhysicsManager::~CPhysicsManager() {
  exit();
}

void CPhysicsManager::exit() {
  if (!m_pPhyWorld) {return;}  // already deleted

#ifdef PHYSICS_DEBUG
  if (m_pDbgDraw) {delete m_pDbgDraw; m_pDbgDraw = nullptr;}
#endif  // PHYSICS_DEBUG

  for (int i = m_pPhyWorld->getNumCollisionObjects() - 1; i >= 0; i--) {
    btCollisionObject * obj = m_pPhyWorld->getCollisionObjectArray()[i];

    btRigidBody * body = btRigidBody::upcast(obj);

    if (body && body->getMotionState()) {
        delete body->getMotionState();
        body->setMotionState(nullptr);
    }

    m_pPhyWorld->removeCollisionObject(obj);

    delete obj;
  }

  // destroy collision shapes
  for (auto &sh : m_CollisionObjects) {
    auto triShape = dynamic_cast<btBvhTriangleMeshShape*>(sh.second.getShape());
    if (triShape) {
      delete triShape->getMeshInterface();
    }
    delete sh.second.getShape();
  }
  m_CollisionObjects.clear();

  // destroy ghost pair callback
  if (m_pGhostPairCallback) {
    delete m_pGhostPairCallback;
    m_pGhostPairCallback = nullptr;
  }

  // destroy world
  if (m_pPhyWorld) {
    delete m_pPhyWorld;
    m_pPhyWorld = nullptr;

    delete mSolver;
    delete mBroadphaseInterface;
    delete mDispatcher;
    delete mCollisionConfig;
  }
}
btCollisionWorld *CPhysicsManager::getCollisionWorld() {
  return m_pPhyWorld->getCollisionWorld();
}

btBroadphaseInterface *CPhysicsManager::getBroadphase() {
  return mBroadphaseInterface;
}


bool CPhysicsManager::hasRigidBody(btRigidBody *body) {
  return m_pPhyWorld->getCollisionObjectArray().findLinearSearch(body)
      != m_pPhyWorld->getCollisionObjectArray().size();
}

void CPhysicsManager::secureAddRigidBody(btRigidBody *body,
                                         uint16_t group,
                                         uint16_t mask) {
  if (hasRigidBody(body)) {return;}
  m_pPhyWorld->addRigidBody(body, group, mask);
}

void CPhysicsManager::secureRemoveRigidBody(btRigidBody *body) {
  if (!hasRigidBody(body)) {return;}
  m_pPhyWorld->removeRigidBody(body);
}

void CPhysicsManager::update(Ogre::Real tpf) {
  // handle Messages
  while (m_Messages.size() > 0) {
    const btCollisionObject *co = m_Messages.front()->getCollisionObject();
    const btRigidBody *rb = btRigidBody::upcast(co);
    switch (m_Messages.front()->getType()) {
      case CPhysicsMessage::PMT_CREATE:
        m_pPhyWorld->addCollisionObject(const_cast<btCollisionObject*>(
            m_Messages.front()->getCollisionObject()));
        m_Messages.front()->getCollisionObject()->activate();
        break;
      case CPhysicsMessage::PMT_DELETE:
        if (rb) {
          if (rb->getMotionState()) {
            delete  rb->getMotionState();
            const_cast<btRigidBody*>(rb)->setMotionState(nullptr);
          }
        }
        m_pPhyWorld->removeCollisionObject(const_cast<btCollisionObject*>(co));
        delete co;
        break;
    }
    delete m_Messages.front();
    m_Messages.pop_front();
  }

  float fixedsubstep = 1.f/60.f;
  int maxNumSubsteps = 2;

  // Update Bullet world. Don't forget the debugDrawWorld() part!
  m_pPhyWorld->stepSimulation(tpf, maxNumSubsteps);


#ifdef PHYSICS_DEBUG
    m_pPhyWorld->debugDrawWorld();
    m_pDbgDraw->step();
#endif
}
void CPhysicsManager::toggleDisplayDebugInfo() {
#ifdef PHYSICS_DEBUG
    m_bDisplayDebugInfo = !m_bDisplayDebugInfo;

    m_pDbgDraw->setDebugMode(m_bDisplayDebugInfo);
    // Ogre::LogManager::getSingleton().logMessage(
    // Ogre::String("PhsicsDebug: ") + (m_bDisplayDebugInfo ? "yes" : "no"));
#endif
}
void CPhysicsManager::deleteLater(const btCollisionObject *pCO) {
  for (auto pMsg : m_Messages) {
    if (pMsg->getType() == CPhysicsMessage::PMT_DELETE
        && pCO == pMsg->getCollisionObject()) {
      // already queued
      return;
    }
  }

  m_Messages.push_back(new CPhysicsMessage(CPhysicsMessage::PMT_DELETE, pCO));
}

void CPhysicsManager::deleteNow(btCollisionObject *co) {
  ASSERT(co);
  btRigidBody *rb = btRigidBody::upcast(co);

  if (rb) {
    if (rb->getMotionState()) {
      delete  rb->getMotionState();
      const_cast<btRigidBody*>(rb)->setMotionState(nullptr);
    }
    if (rb->getCollisionShape()) {
      if (!hasCollisionShape(rb->getCollisionShape())) {
        delete rb->getCollisionShape();
      }
      rb->setCollisionShape(nullptr);
    }
    secureRemoveRigidBody(rb);
  } else {
    m_pPhyWorld->removeCollisionObject(co);
  }
  delete co;
}

#if PHYSICS_MANAGER_DEBUG == 1
void CPhysicsManager::sendMessageToAll(const CMessagePtr message) {
  if (message->getType() == MSG_DEBUG) {
    auto msg_dbg(std::dynamic_pointer_cast<const CMessageDebug>(message));
    if (msg_dbg->getDebugType() == CMessageDebug::DM_TOGGLE_PHYSICS) {
      toggleDisplayDebugInfo();
    }
  }
}
#endif
