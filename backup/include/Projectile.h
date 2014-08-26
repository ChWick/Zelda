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

#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "Damage.h"
#include "Object.h"

class CPhysicsManager;

//! Class for a projectile, like an arrow, or fire/ice ball
/**
  * We will handle the physics on our own, because, the projectiles are fast and could tunnle through walls
  */
class CProjectile : public CObject, public Ogre::SceneNode::Listener {
private:

    enum EStates {
        S_ATTACHED,         //!< Attached to player, e.g. arrow that is drawn currently
        S_FLYING,           //!< Projectile is moving
        S_STICKED,          //!< projectile is sticked in a wall
        S_DESTROYED,        //!< Projectile is destroyed
    };

    EStates m_eState;                       //!< state of the projectile
    CDamage::EType m_eDamageType;           //!< type of the damage
    Ogre::SceneNode *m_pSceneNode;          //!< scene node
    Ogre::Vector3 m_vVelocity;              //!< current velocity of the projectile
    bool m_bIsAffectedByGravity;            //!< Is Projectile affected by gravity?
    Ogre::Real m_vTimer;                    //!< timer for destruction of the object (optional)
public:
    CProjectile(CObjectManager &objectManager, Ogre::SceneNode *pParentSceneNode, const CDamage::EType dmgType, const Ogre::Vector3 &vPosition, const Ogre::Vector3 &vDirection);
    CProjectile(CObjectManager &objectManager, Ogre::SceneNode *pParentSceneNode, const CDamage::EType dmgType);
    virtual ~CProjectile();

    void launchProjectile();
    void updateAttachedProjectile(const Ogre::Vector3 &vPosition, const Ogre::Vector3 &vDirection);

	void update(Ogre::Real tpf);

	virtual bool interactOnCollision(const btCollisionObject *pPartner, const btVector3 &colPoint);

	// listener to delete the node, if destroyed in the scene graph
	void nodeDestroyed (const Ogre::Node *);
protected:
    void init(Ogre::SceneNode *pParentSceneNode);
private:
    void stickProjectileToObject(const btCollisionObject *pObject, const btVector3 &colPoint);
    CHitableInterface::EReceiveDamageResult hitObject(const btCollisionObject *pObject, const btVector3 &colPoint, const Ogre::Vector3 &vDirection);
};

#endif // PROJECTILE_H
