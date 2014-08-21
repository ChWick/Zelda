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
