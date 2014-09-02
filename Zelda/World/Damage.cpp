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

#include "Damage.hpp"


CDamage::CDamage(EDamageType eDmgType)
	: m_eDamageType(eDmgType), m_hpDamageValue(DAMAGE_DATA_MAP.toData(eDmgType).defaultDamage), m_vDamageDirection(SPATIAL_VECTOR::ZERO), m_fKnockOutTime(0) {
}
CDamage::CDamage(EDamageType eDmgType, const SPATIAL_VECTOR &vDirection)
	: m_eDamageType(eDmgType), m_hpDamageValue(DAMAGE_DATA_MAP.toData(eDmgType).defaultDamage), m_vDamageDirection(vDirection), m_fKnockOutTime(0) {
}
CDamage::CDamage(EDamageType eDmgType, const SPATIAL_VECTOR &vDirection, const float fKnockOutTime)
	: m_eDamageType(eDmgType), m_hpDamageValue(DAMAGE_DATA_MAP.toData(eDmgType).defaultDamage), m_vDamageDirection(vDirection), m_fKnockOutTime(fKnockOutTime) {
}
CDamage::CDamage(EDamageType eDmgType, const SPATIAL_VECTOR &vDirection, const Hitpoints hpDamageValue)
	: m_eDamageType(eDmgType), m_hpDamageValue(hpDamageValue), m_vDamageDirection(vDirection), m_fKnockOutTime(0) {
}
CDamage::CDamage(EDamageType eDmgType, const SPATIAL_VECTOR &vDirection, const Hitpoints hpDamageValue, float fKnockOutTime)
	: m_eDamageType(eDmgType), m_hpDamageValue(hpDamageValue), m_vDamageDirection(vDirection), m_fKnockOutTime(fKnockOutTime) {
}
