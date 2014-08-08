#ifndef _DOTSCENELOADERCALLBACK_H_
#define _DOTSCENELOADERCALLBACK_H_

#include <OgrePrerequisites.h>

class btRigidBody;
class CUserData;

class CDotSceneLoaderCallback {
public:
  virtual void preEntityAdded(CUserData &userData) {}
	virtual void postEntityAdded(Ogre::Entity *pEntity, Ogre::SceneNode *pParent, btRigidBody *pRigidBody, const CUserData &userData) {}
	virtual void staticObjectAdded(Ogre::Entity *pEntity, Ogre::SceneNode *pParent) {}

	virtual void lightAdded(Ogre::Light *pLight) {}
};

#endif
