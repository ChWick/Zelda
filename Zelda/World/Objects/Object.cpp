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

#include "Object.hpp"
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>
#include <OgreEntity.h>
#include <BulletCollision/CollisionShapes/btCollisionShape.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include "../../Common/Physics/BtOgrePG.hpp"
#include "../../Common/Physics/PhysicsManager.hpp"
#include "../../Common/Physics/PhysicsMasks.hpp"
#include "../GlobalCollisionShapesTypes.hpp"
#include "../Atlas/Map.hpp"
#include "../../Common/Util/Assert.hpp"

CObject::CObject(const std::string &id, CWorldEntity *pParent, CMap *pMap, EObjectTypes eObjectType, Ogre::SceneNode *pSceneNode)
  : CWorldEntity(id, pParent, pMap) {

  setType(eObjectType);

  if (pSceneNode) {
    m_pSceneNode = pSceneNode;
  }
  else {
    m_pSceneNode = pParent->getSceneNode()->createChildSceneNode(id);
  }
  
  Ogre::SceneManager *pSceneManager = m_pSceneNode->getCreator();

  Ogre::Entity *pEntity(nullptr);

  const SObjectTypeData &objectTypeData(OBJECT_TYPE_ID_MAP.toData(eObjectType));

  // create entity
  pEntity = pSceneManager->createEntity(id + "ent", objectTypeData.sMeshName, "World");
  pEntity->setMaterialName(objectTypeData.sMaterialName);
  m_pSceneNode->attachObject(pEntity);
  pEntity->setCastShadows(false);

  createPhysics();
}

void CObject::destroyPhysics() {
  const SObjectTypeData &objectTypeData(OBJECT_TYPE_ID_MAP.toData(static_cast<EObjectTypes>(m_uiType)));

  if (m_pCollisionObject) {
    btRigidBody *pRigidBody(btRigidBody::upcast(m_pCollisionObject));
    m_pMap->getPhysicsManager()->getWorld()->removeRigidBody(pRigidBody);
    if (objectTypeData.eCollisionShape == GCST_COUNT) {
      delete m_pCollisionObject->getCollisionShape();
    }
    delete pRigidBody->getMotionState();
    delete pRigidBody;
    m_pCollisionObject = nullptr;
  }
}

void CObject::createPhysics() {
  ASSERT(!m_pCollisionObject);
  const SObjectTypeData &objectTypeData(OBJECT_TYPE_ID_MAP.toData(static_cast<EObjectTypes>(m_uiType)));

  btCollisionShape *pCollisionShape(nullptr);
  btVector3 vCollisionShapeOffset;
  btVector3 vInertia;
  float fMass = 0.1;
  if (objectTypeData.eCollisionShape != GCST_COUNT) {
    const CPhysicsCollisionObject &pco = m_pMap->getPhysicsManager()->getCollisionShape(GLOBAL_COLLISION_SHAPES_TYPES_ID_MAP.toString(objectTypeData.eCollisionShape));
    pCollisionShape = pco.getShape();
    vCollisionShapeOffset = BtOgre::Convert::toBullet(pco.getOffset());
  }
  else {
    // Handle shape by user
  }

  ASSERT(pCollisionShape);

  pCollisionShape->calculateLocalInertia(fMass, vInertia);
  btRigidBody *pRigidBody = new btRigidBody(fMass, new BtOgre::RigidBodyState(m_pSceneNode, btTransform(btQuaternion::getIdentity(), btVector3(0, 0, 0)), btTransform(btQuaternion::getIdentity(), vCollisionShapeOffset)), pCollisionShape, vInertia);
  m_pCollisionObject = pRigidBody;

  setThisAsCollisionObjectsUserPointer();
  //m_pSceneNode->setScale(Ogre::Vector3::UNIT_SCALE * 10);


  unsigned short group = COL_INTERACTIVE;
  unsigned short mask = MASK_INTERACIVE_OBJECT_COLLIDES_WITH;

  // post creation
  switch (m_uiType) {
  case OBJECT_GREEN_RUPEE:
  case OBJECT_BLUE_RUPEE:
  case OBJECT_RED_RUPEE:
    pRigidBody->setAngularVelocity(btVector3(0, 2, 0));
    pRigidBody->setAngularFactor(btVector3(0, 1, 0));
    pRigidBody->setLinearFactor(btVector3(1.0f, 1.0f, 1.0f) * 0.25f);
    pRigidBody->setLinearVelocity(btVector3(0, 1.0f, 0));
    break;
  case OBJECT_GREEN_BUSH:
  case OBJECT_LIGHT_STONE:
  case OBJECT_LIGHT_STONE_PILE:
    if (m_eState == EST_LIFTED) {
      group = COL_DAMAGE_P;
      mask = MASK_DAMAGE_P_COLLIDES_WITH;
    }
    pRigidBody->setLinearFactor(btVector3(0, 0, 0));
    pRigidBody->setAngularFactor(0);
    break;
  default:
    break;
  }

  m_pCollisionObject->getWorldTransform().getOrigin() -= vCollisionShapeOffset;


  m_pMap->getPhysicsManager()->getWorld()->addRigidBody(pRigidBody, group, mask);
}

void CObject::enterMap(CMap *pMap, const Ogre::Vector3 &vPosition) {
  // attach to new CEntity
  attachTo(pMap);

  // change scene node owner
  m_pSceneNode->getParent()->removeChild(m_pSceneNode);
  pMap->getSceneNode()->addChild(m_pSceneNode);

  // recreate physics
  destroyPhysics();
  m_pMap = pMap;
  createPhysics();

  setPosition(vPosition);
}

void CObject::changeState(EEntityStateTypes eState) {
  switch (m_uiType) {
  case OBJECT_GREEN_BUSH:
  case OBJECT_LIGHT_STONE:
    if (eState == EST_LIFTED) {
      btRigidBody *pRB = btRigidBody::upcast(this->getCollisionObject());
      ASSERT(pRB);

      m_pMap->getPhysicsManager()->getWorld()->removeCollisionObject(pRB);
      m_pMap->getPhysicsManager()->getWorld()->addRigidBody(pRB, COL_DAMAGE_P, MASK_DAMAGE_P_COLLIDES_WITH);

      // Entities now cast shadows
      Ogre::SceneNode::ObjectIterator itObject = this->getSceneNode()->getAttachedObjectIterator();
      while (itObject.hasMoreElements()) {
        Ogre::MovableObject* pObject = static_cast<Ogre::MovableObject*>(itObject.getNext());
        pObject->setCastShadows(true);
      }
    }
    else if (eState == EST_THROWN) {
      btRigidBody *pRB = btRigidBody::upcast(this->getCollisionObject());
      ASSERT(pRB);

      pRB->setLinearFactor(btVector3(1, 1, 1));
      pRB->setAngularFactor(0);
      pRB->activate();
    }
  break;
  }
  CWorldEntity::changeState(eState);
}

CObject::SInteractionResult CObject::interactOnCollision(const Ogre::Vector3 &vInteractDir, CWorldEntity *pSender) {
  switch (m_uiType) {
  case OBJECT_GREEN_RUPEE:
  case OBJECT_BLUE_RUPEE:
  case OBJECT_RED_RUPEE:
    deleteLater();
    break;
  case OBJECT_GREEN_BUSH:
  case OBJECT_LIGHT_STONE:
    if (m_eState == EST_THROWN) {
      deleteLater();
    }
    break;
  default:
    break;
  }

  return CWorldEntity::interactOnCollision(vInteractDir, pSender);
}

CObject::SInteractionResult CObject::interactOnActivate(const Ogre::Vector3 &vInteractDir, CWorldEntity *pSender) {
  switch (m_uiType) {
  case OBJECT_GREEN_BUSH:
  case OBJECT_LIGHT_STONE:
    return SInteractionResult(IR_LIFT);
  default:
    break;
  }

  return CWorldEntity::interactOnActivate(vInteractDir, pSender);
}

CObject::EReceiveDamageResult CObject::hit(const CDamage &dmg) {
  //deleteLater();
  return RDR_IGNORED;
}
