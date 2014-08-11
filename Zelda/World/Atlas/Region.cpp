#include "Region.hpp"
#include "RegionInfo.hpp"
#include <BulletCollision/CollisionShapes/btBoxShape.h>
#include "../../Common/Physics/BtOgreExtras.h"
#include "Map.hpp"
#include "../../Common/Physics/PhysicsManager.h"
#include "../../Common/Physics/PhysicsMasks.hpp"

CRegion::CRegion(CWorldEntity *pParent, const SRegionInfo &info)
  : CWorldEntity(info.ID, pParent, pParent->getMap()) {

  btCollisionShape *pShape = nullptr;
  if (info.shape == "box") {
    pShape = new btBoxShape(BtOgre::Convert::toBullet(info.size));
  }
  else {
    throw Ogre::Exception(0, "Region shape '" + info.shape + "' is unknown.", __FILE__);
  }

  m_pCollisionObject = new btRigidBody(0, new btDefaultMotionState(), pShape);
	m_pCollisionObject->setWorldTransform(btTransform(btQuaternion::getIdentity(), BtOgre::Convert::toBullet(info.position)));

  m_pMap->getPhysicsManager()->getCollisionWorld()->addCollisionObject(m_pCollisionObject, COL_STATIC, MASK_STATIC_COLLIDES_WITH);

  setThisAsCollisionObjectsUserPointer();
}

