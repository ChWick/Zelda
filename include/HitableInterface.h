#pragma once

#include "StdAfx.h"
#include "PhysicsUserPointer.h"

class CDamage;

//! Everything that is possible to hit by damage
/**
  * E.g. a bush, Enemys, the player itself, but also the shield of the player/enemy.
  * This interface inherity CPhysicsUserPointer to use it as a user pointer for a physics object
  */
class CHitableInterface : public CPhysicsUserPointer {
public:
	enum EReceiveDamageResult {
		RDR_ACCEPTED,	//!< The hitable interface received the damage
		RDR_REJECTED,	//!< The hitable interface rejected the damage (invulnerable, etc.)
		RDR_BLOCKED,	//!< The hitable interface blocked the damage (e.g. shield), thus the attacker will be pushed back e.g.
	};
	virtual EReceiveDamageResult receiveDamage(const CDamage &damage) = 0;
	virtual const Ogre::Vector3 &getPosition() const = 0;
};
