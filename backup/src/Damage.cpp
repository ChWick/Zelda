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
#include "Damage.h"

const Hitpoints CDamage::DMG_BASE[] = {
    0,              // None
    0,              // World
    50,            // Base Sword
    100,            // Base Arrow
    100,            // Base hammer
    100,            // Base bomb
};

CDamage::CDamage(EType eDmgType)
	: m_eDamageType(eDmgType), m_hpDamageValue(DMG_BASE[eDmgType]), m_vDamageDirection(Ogre::Vector3::ZERO), m_fKnockOutTime(0) {
}
CDamage::CDamage(EType eDmgType, const Ogre::Vector3 &vDirection)
	: m_eDamageType(eDmgType), m_hpDamageValue(DMG_BASE[eDmgType]), m_vDamageDirection(vDirection), m_fKnockOutTime(0) {
}
CDamage::CDamage(EType eDmgType, const Ogre::Vector3 &vDirection, const Ogre::Real fKnockOutTime)
	: m_eDamageType(eDmgType), m_hpDamageValue(DMG_BASE[eDmgType]), m_vDamageDirection(vDirection), m_fKnockOutTime(fKnockOutTime) {
}
CDamage::CDamage(EType eDmgType, const Ogre::Vector3 &vDirection, const Hitpoints hpDamageValue)
	: m_eDamageType(eDmgType), m_hpDamageValue(hpDamageValue), m_vDamageDirection(vDirection), m_fKnockOutTime(0) {
}
CDamage::CDamage(EType eDmgType, const Ogre::Vector3 &vDirection, const Hitpoints hpDamageValue, const Ogre::Real fKnockOutTime)
	: m_eDamageType(eDmgType), m_hpDamageValue(hpDamageValue), m_vDamageDirection(vDirection), m_fKnockOutTime(fKnockOutTime) {
}
