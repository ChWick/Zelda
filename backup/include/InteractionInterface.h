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

#ifndef INTERACTION_INTERFACE_H
#define INTERACTION_INTERFACE_H

#include "StdAfx.h"

class InteractionInterface {
public:
	//! Interact Direction
	enum EInteractDirection {
		ID_FRONT,
		ID_REAR,
		ID_LEFT,
		ID_RIGHT,

		ID_COUNT,               //! the number of interaction directions
		ID_ALL = ID_COUNT,      //! interacts from all interaction directions
	};
	//! Interact type
	enum EInteractType {
		IT_USE,
		IT_ATTACK,
		IT_LIFT,
	};
public:
    static EInteractDirection getInteractionDirectionFromString(const Ogre::String &id);
    static Ogre::String getInteractionDirectionFromId(EInteractDirection id);

	//! player interacts with object active
	virtual bool interactOnKeypress(EInteractType eInteractType, const Ogre::Vector3 &vInteractDir = Ogre::Vector3::ZERO) {return false;}
	//! if its a object that interacts on collision (e.g. items, oder a link)
	virtual bool interactOnCollision(const btCollisionObject *pPartner) {return false;}

    //! get the interaction direction as front, rear, left, right, relative to this object
    /**
      * \param vInteractDir The direction from witch the interaction comes from
      * \param fYaw the yaw of this
      * \return The interaction relative to this as front, rear, left or right
      */
	const EInteractDirection getInteractDirection(const Ogre::Vector3 &vInteractDir, const Ogre::Real fYaw = 0);
};

#endif
