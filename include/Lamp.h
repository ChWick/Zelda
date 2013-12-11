#ifndef LAMP_H
#define LAMP_H

#include "Tool.h"

class CPhysicsManager;

class Lamp : public CTool {
private:
    btRigidBody *m_pPhysicsHandle;
    btRigidBody *m_pPhysicsLamp;
    Ogre::SceneNode *m_pSceneNodeLamp;
    CPhysicsManager *m_pPhysicsManager;
    btHingeConstraint *m_pConstraint;
public:
    Lamp(Ogre::SceneManager *pSM, Ogre::SceneNode *pParentSceneNode, CPhysicsManager *pPhysicsManager);
    virtual ~Lamp();
    void start();
    void update (Ogre::Real tpf);
protected:
private:
};

#endif // LAMP_H
