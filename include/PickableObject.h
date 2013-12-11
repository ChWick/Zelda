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

#ifndef _PICKABLE_OBJECT_H_
#define _PICKABLE_OBJECT_H_

#include "Object.h"
#include "InnerObjectInterface.h"

class CPhysicsManager;
class CMap;
class CPickableObject;

class CPickableObject : public CObject, public InnerObjectInterface {
public:
	enum EPickableObjectTypes {
		POT_EMPTY		= 0,
		POT_HEART,
		POT_GREEN_RUBY,
		POT_BLUE_RUBY,
		POT_RED_RUBY,
		POT_SMALL_POTION,

		// tools that can be picked
		POT_BOW,
		POT_LAMP,

		POT_COUNT,
	};
private:
	EPickableObjectTypes m_ePickableObjectType;
	Ogre::SceneNode *m_pSceneNode;
	bool m_bIsPicked;
public:
	static EPickableObjectTypes parsePickableObjectType(const Ogre::String &text);

	CPickableObject(Ogre::SceneNode *pParentNode, CObjectManager &objectManager, EPickableObjectTypes ePickableObjectType, const Ogre::Vector3 &vPosition);
	~CPickableObject();

	EPickableObjectTypes getPickableObjectType() const {return m_ePickableObjectType;}


	virtual void update(Ogre::Real tpf);
	void pickupObject();
	//virtual bool interactOnCollision(const btCollisionObject *pPartner);


};

#endif
