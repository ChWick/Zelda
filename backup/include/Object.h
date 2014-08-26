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
#include "HitableInterface.h"
#include "InteractionInterface.h"
#include "UserData.h"

class CObjectManager;
class CDamage;

class CObject : public CHitableInterface, public InteractionInterface {
public:
	//! Pbjecttypse
	enum EObjectType {
		SIMPLE_OBJECT			= 0,
		DOOR_OBJECT,
		LIFTABLE_OBJECT,
		PICKABLE_OBJECT,
		CHEST_OBJECT,
		DAMAGE_OBJECT,                          //!< Projectiles
		LINKABLE_OBJECT,
		CONTACT_LINK_OBJECT,
		LADDER_OBJECT,
	};
protected:
	const Ogre::String m_SafeStateID;			    //!< eine einzigartige Id zur verknüfung mit dem Safestate, falls benötigt
	const EObjectType m_eObjectType;			    //!< der Objecttype
	CObjectManager &m_ObjectManager;			    //!< der Objectmanager
	btCollisionObject *m_pCollisionObject;		    //!< use this to connect this object to a physics object
	UserData m_UserData;                            //!< User data from the map
protected:
	//! Constructor of a object
	/**
	  * \param pCollisionObject[in] If its set, then the user pointer of the collision object with be set as 'this'
	  */
	CObject(CObjectManager &objectManager, EObjectType eObjectType = SIMPLE_OBJECT, btCollisionObject *pCollisionObject = NULL, const Ogre::String &id = Ogre::StringUtil::BLANK);

public:
	virtual ~CObject();

	const Ogre::String &getSafeStateID() const {return m_SafeStateID;}
	const EObjectType getObjectType() const {return m_eObjectType;}
	btCollisionObject *getCollisionObject() const {return m_pCollisionObject;}

	void setUserData(const UserData &userData);

	//! Function to kill the object
	/**
	  * This will call CObjectManager::deleteObject(this)
	  */
	void suicide();

	virtual void update(Ogre::Real tpf) {};

	// possible actions for a object, call it and see what happens, e.g. if the object will interact
	// by default all functions will do nothing
	const Ogre::Vector3 &getPosition() const {return BtOgre::Convert::toOgre(m_pCollisionObject->getWorldTransform().getOrigin());}


	//! object recevies damage
	virtual EReceiveDamageResult receiveDamage(const CDamage &damage) {return RDR_BLOCKED;}
};
