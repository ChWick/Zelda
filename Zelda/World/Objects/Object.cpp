#include "Object.hpp"
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>
#include <OgreEntity.h>
#include <BulletCollision/CollisionShapes/btCollisionShape.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include "../../Common/Physics/BtOgrePG.h"
#include "../../Common/Physics/PhysicsManager.h"
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
    pCollisionShape = m_pMap->getPhysicsManager()->getCollisionShape(GLOBAL_COLLISION_SHAPES_TYPES_ID_MAP.toString(GCST_PICKABLE_OBJECT_SPHERE)).getShape();
    break;
  case OBJECT_RED_RUPEE:
    pEntity = pSceneManager->createEntity("rupee.mesh");
    pEntity->setMaterialName("Rupee/Red");
    pCollisionShape = m_pMap->getPhysicsManager()->getCollisionShape(GLOBAL_COLLISION_SHAPES_TYPES_ID_MAP.toString(GCST_PICKABLE_OBJECT_SPHERE)).getShape();
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
  deleteLater();

  return SInteractionResult();
}
