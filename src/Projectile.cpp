#include "StdAfx.h"
#include "Projectile.h"
#include "ObjectManager.h"
#include "Map.h"
#include "PhysicsManager.h"
#include "Util.h"
#include "Person.h"
#include "DebugDrawer.h"
#include "PhysicsUserPointer.h"

CProjectile::CProjectile(CObjectManager &objectManager, Ogre::SceneNode *pParentSceneNode, const CDamage::EType dmgType, const Ogre::Vector3 &vPosition, const Ogre::Vector3 &vDirection)
: CObject(objectManager, DAMAGE_OBJECT), m_eDamageType(dmgType) {
    init(pParentSceneNode);
    updateAttachedProjectile(vPosition, vDirection);
    launchProjectile();
}
CProjectile::CProjectile(CObjectManager &objectManager, Ogre::SceneNode *pParentSceneNode, const CDamage::EType dmgType)
: CObject(objectManager, DAMAGE_OBJECT), m_eDamageType(dmgType) {
    m_eState = S_ATTACHED;
    init(pParentSceneNode);
}
CProjectile::~CProjectile() {
    if (m_pSceneNode) {
        m_pSceneNode->setListener(NULL);
        destroySceneNode(m_pSceneNode);
        m_pSceneNode = NULL;
    }
}
void CProjectile::init(Ogre::SceneNode *pParentSceneNode) {
    m_pSceneNode = pParentSceneNode->createChildSceneNode();
    m_pSceneNode->setListener(this);

    m_eState = S_ATTACHED;
    m_bIsAffectedByGravity = false;
    switch (m_eDamageType) {
    case CDamage::DMG_ARROW:
        m_pSceneNode->attachObject(m_pSceneNode->getCreator()->createEntity("Arrow.mesh"));
        m_pSceneNode->setScale(Ogre::Vector3::UNIT_SCALE * CPerson::PERSON_SCALE);
        m_bIsAffectedByGravity = true;
        break;
    default:
        throw Ogre::Exception(__LINE__, "Unknown damage type for a projectile", __FILE__);
    }
}
void CProjectile::launchProjectile() {
    m_eState = S_FLYING;
    m_vTimer = 3; // max life time in air (if it flies outside the map)

    m_vVelocity = -m_pSceneNode->getOrientation().zAxis() * 20.f;
}
void CProjectile::nodeDestroyed (const Ogre::Node *pNode) {
    // if scene node is destroyed from the scene graph (not from the destructor), we have to suicide, too
    m_pSceneNode = NULL;
    suicide();
}
void CProjectile::updateAttachedProjectile(const Ogre::Vector3 &vPosition, const Ogre::Vector3 &vDirection) {
    assert(m_eState == S_ATTACHED);

    m_pSceneNode->setPosition(vPosition);
    m_pSceneNode->lookAt(vPosition + vDirection, Ogre::Node::TS_WORLD);
}
void CProjectile::update(Ogre::Real tpf) {
    if (m_eState == S_FLYING) {
        // update physics on our own
        if (m_bIsAffectedByGravity) {
            m_vVelocity += tpf * Ogre::Vector3::NEGATIVE_UNIT_Y * GRAVITY_FACTOR;
        }
        Ogre::Vector3 vOldPosition = m_pSceneNode->getPosition();
        m_pSceneNode->translate(m_vVelocity * tpf);

        m_pSceneNode->lookAt(vOldPosition + m_vVelocity, Ogre::Node::TS_WORLD); // projectile will always look into flying direction


        // set pos a bit back, for collision check
        vOldPosition += m_pSceneNode->getOrientation().zAxis() * 0.3;
        // check for collision in the current step
        // try to interact with the world. So detect an object to interact with
        btCollisionWorld::ClosestRayResultCallback rayCallback(BtOgre::Convert::toBullet(vOldPosition), BtOgre::Convert::toBullet(m_pSceneNode->getPosition()));

        rayCallback.m_collisionFilterGroup = COL_DAMAGE_P;
        rayCallback.m_collisionFilterMask = MASK_DAMAGE_P_COLLIDES_WITH;

        m_ObjectManager.getMap().getPhysicsManager()->getWorld()->rayTest(BtOgre::Convert::toBullet(vOldPosition), BtOgre::Convert::toBullet(m_pSceneNode->getPosition()), rayCallback);
        DebugDrawer::getSingleton().drawLine(vOldPosition, m_pSceneNode->getPosition(), Ogre::ColourValue::Red);
        if (rayCallback.hasHit()) {
            interactOnCollision(rayCallback.m_collisionObject, rayCallback.m_hitPointWorld);
        }
    }
    if (m_eState == S_STICKED || m_eState == S_FLYING){
        // even if the object is flying we gave him a max liftime, if its out of the map e.g.
        m_vTimer -= tpf;
        if (m_vTimer <= 0) {
            suicide();
        }
    }
}
bool CProjectile::interactOnCollision(const btCollisionObject *pPartner, const btVector3 &colPoint) {
    if (hitObject(pPartner, colPoint, m_vVelocity) == CHitableInterface::RDR_REJECTED) {
        return false;
    }
    switch (m_eDamageType) {
    case CDamage::DMG_ARROW:
        m_eState = S_STICKED;
        m_vTimer = 5;
        stickProjectileToObject(pPartner, colPoint);
        break;
    }
    return true;
}
void CProjectile::stickProjectileToObject(const btCollisionObject *pObject, const btVector3 &colPoint) {
    const btRigidBody *pRB = btRigidBody::upcast(pObject);
    Ogre::SceneNode *pSceneNodeToAttachTo(NULL);
    if (pRB) {
        // first check if there is a rigid body state where we can attach to object
        if (pRB->getMotionState()) {
            if (dynamic_cast<const BtOgre::RigidBodyState*>(pRB->getMotionState())) {
                pSceneNodeToAttachTo = dynamic_cast<const BtOgre::RigidBodyState*>(pRB->getMotionState())->getNode();
            }
        }
    }

    // now check whether the PhysicsUserPointer contains a scene node
    if (!pSceneNodeToAttachTo && pObject->getUserPointer()) {
        CPhysicsUserPointer *pPhysicsUserPointer = static_cast<CPhysicsUserPointer*>(pObject->getUserPointer());
        pSceneNodeToAttachTo = pPhysicsUserPointer->getSceneNode();
    }

    // if no scene node, then fix the position
    if (!pSceneNodeToAttachTo) {
        m_pSceneNode->setPosition(BtOgre::Convert::toOgre(colPoint));
        return;
    }
    // we have a rigidbody connected to a scene node
    // we will attach our scene node to this scene node to have a relative poision if the object is moving
    m_pSceneNode->getParent()->removeChild(m_pSceneNode);
    pSceneNodeToAttachTo->addChild(m_pSceneNode);
    m_pSceneNode->setPosition(pSceneNodeToAttachTo->convertWorldToLocalPosition(BtOgre::Convert::toOgre(colPoint)));
}
CHitableInterface::EReceiveDamageResult CProjectile::hitObject(const btCollisionObject *pObject, const btVector3 &colPoint, const Ogre::Vector3 &vDirection) {
    if (!pObject->getUserPointer()) {
        return CHitableInterface::RDR_BLOCKED; // static physics object (e.g. wall)
    }
    CPhysicsUserPointer *pPhysicsUserPointer = static_cast<CPhysicsUserPointer*>(pObject->getUserPointer());
    CHitableInterface *pHitableInterface = dynamic_cast<CHitableInterface*>(pPhysicsUserPointer);
    if (pHitableInterface) {
        return pHitableInterface->receiveDamage(CDamage(m_eDamageType, vDirection));
    }
    return CHitableInterface::RDR_REJECTED;
}
