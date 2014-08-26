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
#include "Hitpoints.h"

class CDamage {
public:
    static const Hitpoints DMG_BASE[];
	enum EType {
		DMG_NONE	= 0,
		DMG_WORLD	= 1,
		DMG_SWORD   = 2,
		DMG_ARROW   = 4,
		DMG_HAMMER  = 8,
		DMG_BOMB    = 16,
		DMG_FIRE    = 32,

		DMG_ALL     = 511,                  //!< Flag for take/block all damage types
	};
private:
	const Hitpoints m_hpDamageValue;		//!< Schaden in Prozent an Herzen (100 = 1 Herz, 50 = 1/2 Herz)
	const EType m_eDamageType;
	const Ogre::Vector3 m_vDamageDirection;
	const Ogre::Real m_fKnockOutTime;			//!< Time how long the target will be stunned

public:
    CDamage(EType eDmgType);
    CDamage(EType eDmgType, const Ogre::Vector3 &vDirection);
	CDamage(EType eDmgType, const Ogre::Vector3 &vDirection, const Ogre::Real fKnockOutTime);
	CDamage(EType eDmgType, const Ogre::Vector3 &vDirection, const Hitpoints hpDamageValue);
	CDamage(EType eDmgType, const Ogre::Vector3 &vDirection, const Hitpoints hpDamageValue, const Ogre::Real fKnockOutTime);

	const Ogre::Vector3 &getDamageDirection() const {return m_vDamageDirection;}
	const Hitpoints getDamageValue() const {return m_hpDamageValue;}
	const EType getDamageType() const {return m_eDamageType;}
	const Ogre::Real getKnockOutTime() const {return m_fKnockOutTime;}
};
