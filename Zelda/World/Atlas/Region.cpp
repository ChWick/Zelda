#include "Region.hpp"
#include "RegionInfo.hpp"
#include <BulletCollision/CollisionShapes/btBoxShape.h>
#include "../../Common/Physics/BtOgreExtras.h"
#include "Map.hpp"
#include "../../Common/Physics/PhysicsManager.h"
#include "../../Common/Physics/PhysicsMasks.hpp"

CRegion::CRegion(CWorldEntity *pParent, const SRegionInfo &info)
  : CWorldEntity(info.ID, pParent, pParent->getMap()),
    m_Info(info) {


}

void CRegion::start() {
  btCollisionShape *pShape = nullptr;
  if (m_Info.shape == "box") {
    pShape = new btBoxShape(BtOgre::Convert::toBullet(m_Info.size));
  }
  else {
    throw Ogre::Exception(0, "Region shape '" + m_Info.shape + "' is unknown.", __FILE__);
  }

  m_pCollisionObject = new btRigidBody(0, new btDefaultMotionState(), pShape);
	m_pCollisionObject->setWorldTransform(btTransform(btQuaternion::getIdentity(), BtOgre::Convert::toBullet(m_Info.position)));

  m_pMap->getPhysicsManager()->getCollisionWorld()->addCollisionObject(m_pCollisionObject, COL_STATIC, MASK_STATIC_COLLIDES_WITH);

  setThisAsCollisionObjectsUserPointer();
}

