#ifndef _UTIL_H_
#define _UTIL_H_

#include "OgrePrerequisites.h"


void destroySceneNode(Ogre::SceneNode* i_pSceneNode, bool destroyItself = true);

Ogre::Vector3 getMaxBB(const Ogre::Vector3 &v1, const Ogre::Vector3 &v2);
Ogre::Vector3 getMinBB(const Ogre::Vector3 &v1, const Ogre::Vector3 &v2);

#endif
