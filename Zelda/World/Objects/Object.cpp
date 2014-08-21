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

CObject::CObject(const std::string &id, CWorldEntity *pParent, CMap *pMap, EObjectTypes eObjectType)
  : CWorldEntity(id, pParent, pMap) {

  setType(eObjectType);

  m_pSceneNode = pParent->getSceneNode()->createChildSceneNode(id);
  Ogre::SceneManager *pSceneManager = m_pSceneNode->getCreator();

  Ogre::Entity *pEntity(nullptr);
  btCollisionShape *pCollisionShape(nullptr);
  btVector3 vCollisionShapeOffset;

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

  btRigidBody *pRigidBody = new btRigidBody(1, new BtOgre::RigidBodyState(m_pSceneNode), pCollisionShape);
  m_pCollisionObject = pRigidBody;

  setThisAsCollisionObjectsUserPointer();
  //m_pSceneNode->setScale(Ogre::Vector3::UNIT_SCALE * 10);

  m_pMap->getPhysicsManager()->getWorld()->addRigidBody(pRigidBody, COL_INTERACTIVE, MASK_PICKABLE_OBJECT_COLLIDES_WITH | COL_CHARACTER_P);


  // post creation
  switch (eObjectType) {
  case OBJECT_GREEN_RUPEE:
  case OBJECT_BLUE_RUPEE:
  case OBJECT_RED_RUPEE:
    pRigidBody->setAngularVelocity(btVector3(0, 2, 0));
    pRigidBody->setLinearFactor(btVector3(1.0f, 1.0f, 1.0f) * 0.25f);
    pRigidBody->setLinearVelocity(btVector3(0, 1.0f, 0));
    break;
  }
  setPosition(Ogre::Vector3(0, 5, 0));
}

CObject::SInteractionResult CObject::interactOnCollision(const Ogre::Vector3 &vInteractDir, CWorldEntity *pSender) {
  switch (m_uiType) {
  case OBJECT_GREEN_RUPEE:
  case OBJECT_BLUE_RUPEE:
  case OBJECT_RED_RUPEE:
    deleteLater();
    return SInteractionResult();
    break;
  }

  // Do nothing
  return SInteractionResult();
}
