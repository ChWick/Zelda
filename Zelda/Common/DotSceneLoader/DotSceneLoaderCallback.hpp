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

#ifndef _DOTSCENELOADERCALLBACK_H_
#define _DOTSCENELOADERCALLBACK_H_

#include <OgrePrerequisites.h>

class btRigidBody;
class CUserData;

namespace tinyxml2 {
  class XMLElement;
};

class CDotSceneLoaderCallback {
public:
  enum EResults {
    R_CONTINUE,
    R_CANCEL,
  };

  virtual void worldPhysicsAdded(btRigidBody *pRigidBody) {}
  virtual EResults preEntityAdded(tinyxml2::XMLElement *XMLNode, Ogre::SceneNode *pParent, CUserData &userData) {return R_CONTINUE;}
	virtual void postEntityAdded(Ogre::Entity *pEntity, Ogre::SceneNode *pParent, btRigidBody *pRigidBody, const CUserData &userData) {}
	virtual void staticObjectAdded(Ogre::Entity *pEntity, Ogre::SceneNode *pParent) {}

	virtual void lightAdded(Ogre::Light *pLight) {}
};

#endif
