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


class CPerson;

class CShield : public CHitableInterface {
public:
	enum EShieldTypes {
		ST_SIMPLE_SHIELD,
	};
private:
	const EShieldTypes m_eShieldType;		//!< Type of the shield
	CPerson *m_pOwner;						//!< Owner of the shield
	btCollisionObject *m_pCollisionObject;	//!< The physics collision object
	Ogre::Vector3 m_vOffset;					//!< Offset of the box of the shield to the handle
public:
	CShield(CPerson *pOwner, const Ogre::String &parentBone, Ogre::SceneManager *pCreator, EShieldTypes eShieldType);
	~CShield();
	EReceiveDamageResult receiveDamage(const CDamage &damage) {return RDR_BLOCKED;}
	btCollisionObject *getCollisionObject() const {return m_pCollisionObject;}
	void updateShieldPhysics(const Ogre::Vector3 &vPos, const Ogre::Quaternion &qRotation);
	const Ogre::Vector3 &getPosition() const {return BtOgre::Convert::toOgre(m_pCollisionObject->getWorldTransform().getOrigin());}
protected:
};
