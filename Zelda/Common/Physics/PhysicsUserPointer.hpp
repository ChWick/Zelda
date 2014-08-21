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

#ifndef _PHYSICS_USER_POINTER_
#define _PHYSICS_USER_POINTER_

#include "StdAfx.h"

//! This is a dummy class, but every pUserData of a btCollisionObject object has to be of this type
/**
  * For real classes inheriet this class and to check type use 'dynamic_cast' and check for NULL
  */
class CPhysicsUserPointer {
public:
	virtual ~CPhysicsUserPointer() {}
	//! Function to get the collision object whose user pointer i am
	virtual btCollisionObject *getCollisionObject() const = 0;
	//! Function to get a connected scene node
	virtual Ogre::SceneNode *getSceneNode() const {return NULL;}

	void setAsUserPointer(btCollisionObject *pColObject) {assert(pColObject); pColObject->setUserPointer(this);}
};

#endif
