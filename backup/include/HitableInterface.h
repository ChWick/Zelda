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
