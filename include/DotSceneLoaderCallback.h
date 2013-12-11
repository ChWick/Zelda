#ifndef _DOTSCENELOADERCALLBACK_H_
#define _DOTSCENELOADERCALLBACK_H_

#include <OgrePrerequisites.h>

class UserData;

class CDotSceneLoaderCallback {
public:
    virtual void preEntityAdded(UserData &userData) {}
	virtual void postEntityAdded(Ogre::Entity *pEntity, Ogre::SceneNode *pParent, btRigidBody *pRigidBody, const UserData &userData) {}
	virtual void staticObjectAdded(Ogre::Entity *pEntity, Ogre::SceneNode *pParent) {}

	virtual void lightAdded(Ogre::Light *pLight) {}
};

#endif
