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

#include "StdAfx.h"
#include "LinkableObject.h"
#include "ObjectManager.h"
#include "Map.h"
#include "Util.h"
#include "PhysicsManager.h"

LinkableObject::LinkableObject(CObjectManager &objectManager, btCollisionObject *pObject, Ogre::SceneNode *pSceneNode, Ogre::Entity *pEntity, const Ogre::String &id)
    : CObject(objectManager, LINKABLE_OBJECT, pObject, id), m_pSceneNode(pSceneNode), m_pEntity(pEntity) {
    //ctor
}

LinkableObject::~LinkableObject()
{
    if (m_pEntity) {
        Ogre::SceneNode *pParentNodeOfEnt = m_pEntity->getParentSceneNode();
        if (pParentNodeOfEnt) {
            pParentNodeOfEnt->detachObject(m_pEntity);
        }

        m_ObjectManager.getMap().getSceneManager()->destroyEntity(m_pEntity);
        m_pEntity = NULL;
    }
    destroySceneNode(m_pSceneNode, true);
    m_pSceneNode = NULL;

    if (m_pCollisionObject) {
        btRigidBody *pRB = btRigidBody::upcast(m_pCollisionObject);
        assert(pRB);

        m_ObjectManager.getMap().getPhysicsManager()->getWorld()->removeRigidBody(pRB);
        m_ObjectManager.getMap().getPhysicsManager()->eraseCollisionShape(pRB->getCollisionShape());
        delete pRB->getCollisionShape();
        delete pRB->getMotionState();
        delete pRB;

        m_pCollisionObject = 0;
    }
}
