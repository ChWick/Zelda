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

#include "Region.hpp"
#include "RegionInfo.hpp"
#include <BulletCollision/CollisionShapes/btBoxShape.h>
#include "../../Common/Physics/BtOgreExtras.hpp"
#include "Map.hpp"
#include "../../Common/Physics/PhysicsManager.hpp"
#include "../../Common/Physics/PhysicsMasks.hpp"
#include "../../Common/Util/XMLHelper.hpp"

using namespace XMLHelper;

CRegion::CRegion(CWorldEntity *pParent, const SRegionInfo &info)
  : CWorldEntity(info.ID, pParent, pParent->getMap()),
    m_Info(info) {
}

CRegion::CRegion(CWorldEntity *pParent, const tinyxml2::XMLElement *pElem)
  : CWorldEntity(pParent, pParent->getMap(), pElem),
    m_Info({Ogre::StringConverter::parseVector3(Attribute(pElem, "position")),
          Ogre::StringConverter::parseVector3(Attribute(pElem, "size")),
          Attribute(pElem, "id"),
          Attribute(pElem, "shape")}){

}

CRegion::~CRegion() {
}

void CRegion::exit() {
  CWorldEntity::exit();
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

