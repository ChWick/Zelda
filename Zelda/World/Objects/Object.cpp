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
  btCollisionShape *pCollisionShape(nullptr);
  btVector3 vCollisionShapeOffset;
  btVector3 vInertia;
  float fMass = 1;

  bool bUsePickableObjectSphere = false;

  switch (eObjectType) {
  case OBJECT_GREEN_RUPEE:
    pEntity = pSceneManager->createEntity("rupee.mesh");
    pEntity->setMaterialName("Rupee/Green");
    bUsePickableObjectSphere = true;
    break;
  case OBJECT_BLUE_RUPEE:
    pEntity = pSceneManager->createEntity("rupee.mesh");
    pEntity->setMaterialName("Rupee/Blue");
    bUsePickableObjectSphere = true;
    break;
  case OBJECT_RED_RUPEE:
    pEntity = pSceneManager->createEntity("rupee.mesh");
    pEntity->setMaterialName("Rupee/Red");
    bUsePickableObjectSphere = true;
    break;
  case OBJECT_GREEN_BUSH:
    pEntity = pSceneManager->createEntity("GreenBush.mesh");
    pEntity->setMaterialName("Game/Soil");
    bUsePickableObjectSphere = true;
    break;
  }

  if (bUsePickableObjectSphere) {
    const CPhysicsCollisionObject &pco = m_pMap->getPhysicsManager()->getCollisionShape(GLOBAL_COLLISION_SHAPES_TYPES_ID_MAP.toString(GCST_PICKABLE_OBJECT_SPHERE));
    pCollisionShape = pco.getShape();
    vCollisionShapeOffset = BtOgre::Convert::toBullet(pco.getOffset());
  }

  assert(pEntity);
  assert(pCollisionShape);

  m_pSceneNode->attachObject(pEntity);
  pEntity->setCastShadows(false);

  pCollisionShape->calculateLocalInertia(fMass, vInertia);
  btRigidBody *pRigidBody = new btRigidBody(fMass, new BtOgre::RigidBodyState(m_pSceneNode, btTransform(btQuaternion::getIdentity(), btVector3(0, 0, 0)), btTransform(btQuaternion::getIdentity(), vCollisionShapeOffset)), pCollisionShape, vInertia);
  m_pCollisionObject = pRigidBody;

  setThisAsCollisionObjectsUserPointer();
  //m_pSceneNode->setScale(Ogre::Vector3::UNIT_SCALE * 10);

  m_pMap->getPhysicsManager()->getWorld()->addRigidBody(pRigidBody, COL_INTERACTIVE, MASK_INTERACIVE_OBJECT_COLLIDES_WITH);


  // post creation
  switch (eObjectType) {
  case OBJECT_GREEN_RUPEE:
  case OBJECT_BLUE_RUPEE:
  case OBJECT_RED_RUPEE:
    pRigidBody->setAngularVelocity(btVector3(0, 2, 0));
    pRigidBody->setAngularFactor(btVector3(0, 1, 0));
    pRigidBody->setLinearFactor(btVector3(1.0f, 1.0f, 1.0f) * 0.25f);
    pRigidBody->setLinearVelocity(btVector3(0, 1.0f, 0));
    break;
  case OBJECT_GREEN_BUSH:
    pRigidBody->setLinearFactor(btVector3(0, 0, 0));
    pRigidBody->setAngularFactor(0);
    break;
  }

  m_pCollisionObject->getWorldTransform().getOrigin() -= vCollisionShapeOffset;
}

void CObject::changeState(EEntityStateTypes eState) {
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
    return SInteractionResult(IR_LIFT);
  default:
    break;
  }

  return CWorldEntity::interactOnActivate(vInteractDir, pSender);
}
