#include "StdAfx.h"
#include "Lamp.h"
#include "Util.h"
#include "PhysicsManager.h"
#include "Person.h"

Lamp::Lamp(Ogre::SceneManager *pSM, Ogre::SceneNode *pParentSceneNode, CPhysicsManager *pPhysicsManager)
: CTool(CPlayerTool::TOOL_LAMP, pSM), m_pPhysicsManager(pPhysicsManager) {
    m_pSceneNodeLamp = pParentSceneNode->createChildSceneNode();
    m_pSceneNodeLamp->attachObject(pSM->createEntity("Lamp.mesh"));
    m_pSceneNodeLamp->setScale(Ogre::Vector3::UNIT_SCALE * CPerson::PERSON_SCALE);
    //m_pSceneNodeLamp->rotate(Ogre::Vector3::UNIT_Z, Ogre::Degree(90));

    Ogre::Real mass = 1;
    btVector3 localInertia( 0.0f, 0.0f, 0.0f );
    btCollisionShape *collisionShape = new btSphereShape(0.1);
    collisionShape->calculateLocalInertia( mass, localInertia );
    btRigidBody::btRigidBodyConstructionInfo info(mass, new btDefaultMotionState(), collisionShape, localInertia);
    m_pPhysicsHandle = new btRigidBody(info);
    m_pPhysicsHandle->setLinearFactor(btVector3(0, 0, 0));
    m_pPhysicsHandle->setAngularFactor(0);

    btRigidBody::btRigidBodyConstructionInfo lampInfo(mass, new BtOgre::RigidBodyState(m_pSceneNodeLamp), new btSphereShape(0.1), localInertia);
    m_pPhysicsLamp = new btRigidBody(lampInfo);
    m_pPhysicsLamp->setDamping(0.9, 0.9);
    m_pPhysicsLamp->setAngularFactor(btVector3(1, 1, 1));
    //m_pPhysicsLamp->setLinearFactor(btVector3(0.1, 0.1, 0.1));

    btTransform localA, localB;
    localA.setIdentity();
    localA.setOrigin(btVector3(0, 0, 0)); // Some point, not zero.
    localB.setIdentity();
    localB.setOrigin(btVector3(1.5 * CPerson::PERSON_SCALE, 0, 0));

    m_pPhysicsHandle->setWorldTransform(localA);
    m_pPhysicsLamp->setWorldTransform(localB);
    btVector3 ax(0, 0, 1);
    m_pConstraint = new btHingeConstraint(*m_pPhysicsHandle, *m_pPhysicsLamp, btVector3(0, 0, 0), btVector3(1.5 * CPerson::PERSON_SCALE, 0, 0), ax, ax, true);
    m_pConstraint->setLimit(-M_PI * 1.0, M_PI * 1, 0.1f, 1, 2.f);
    //m_pConstraint->setAngularOnly(true);
    m_pConstraint->setEnabled(true);
    m_pConstraint->setDbgDrawSize(2);
    //m_pConstraint->enableMotor(false);
    //m_pConstraint->setLimit(M_PI_2, M_PI_2, M_PI, 0.01, 0.01, 0.01);

    pPhysicsManager->getWorld()->addConstraint(m_pConstraint, true);

    pPhysicsManager->getWorld()->addRigidBody(m_pPhysicsHandle, COL_NOTHING, MASK_NONE);
    pPhysicsManager->getWorld()->addRigidBody(m_pPhysicsLamp, COL_NOTHING, MASK_NONE);
}

Lamp::~Lamp() {
    destroySceneNode(m_pSceneNodeLamp, true);
    m_pSceneNodeLamp = NULL;

    m_pPhysicsManager->getWorld()->removeConstraint(m_pConstraint);
    delete m_pConstraint;

    m_pPhysicsManager->getWorld()->removeRigidBody(m_pPhysicsHandle);
    delete m_pPhysicsHandle->getMotionState();
    delete m_pPhysicsHandle->getCollisionShape();
    delete m_pPhysicsHandle;


    m_pPhysicsManager->getWorld()->removeRigidBody(m_pPhysicsLamp);
    delete m_pPhysicsLamp->getMotionState();
    delete m_pPhysicsLamp->getCollisionShape();
    delete m_pPhysicsLamp;
}
void Lamp::start() {
    m_pPhysicsHandle->getWorldTransform().setOrigin(BtOgre::Convert::toBullet(m_pEntity->getParentNode()->_getDerivedPosition()));
    m_pPhysicsLamp->getWorldTransform().setOrigin(m_pPhysicsHandle->getWorldTransform().getOrigin() + btVector3(0, -1.5 * CPerson::PERSON_SCALE, 0));
}
void Lamp::update (Ogre::Real tpf) {
    m_pPhysicsLamp->activate();
    m_pPhysicsHandle->activate();

    m_pPhysicsHandle->getWorldTransform().setOrigin(BtOgre::Convert::toBullet(m_pEntity->getParentNode()->_getDerivedPosition()));
    //m_pSceneNodeLamp->lookAt(BtOgre::Convert::toOgre(m_pPhysicsHandle->getWorldTransform().getOrigin()), Ogre::Node::TS_WORLD);
}
