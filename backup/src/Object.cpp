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

#include "StdAfx.h"
#include "Object.h"
#include "ObjectManager.h"


CObject::CObject(CObjectManager &objectManager, EObjectType eObjectType, btCollisionObject *pCollisionObject, const Ogre::String &id)
	: m_ObjectManager(objectManager), m_eObjectType(eObjectType), m_SafeStateID(id), m_pCollisionObject(pCollisionObject) {

	m_ObjectManager.addObject(this);

	// connect us also to the collision object
	if (m_pCollisionObject) {
		setAsUserPointer(m_pCollisionObject);
	}
}
CObject::~CObject() {
}
void CObject::setUserData(const UserData &userData) {
    m_UserData = userData;
}
void CObject::suicide() {

	m_ObjectManager.deleteObject(this);
}
