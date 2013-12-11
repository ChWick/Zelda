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
