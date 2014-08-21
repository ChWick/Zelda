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
