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

#ifndef _DAMAGE_HPP_
#define _DAMAGE_HPP_

#include "Hitpoints.hpp"
#include "DamageTypes.hpp"
#include "../Config/TypeDefines.hpp"
#include SPATIAL_VECTOR_HEADER

class CAttackerInterface;

class CDamage {
public:
private:
  CAttackerInterface *mAttacker;  //!< Attacker, origin of the damage 
  const EDamageType m_eDamageType;
  const Hitpoints m_hpDamageValue;		//!< Schaden in Prozent an Herzen (100 = 1 Herz, 50 = 1/2 Herz)
  const SPATIAL_VECTOR m_vDamageDirection;
  const float m_fKnockOutTime;			//!< Time how long the target will be stunned
  
 public:
  CDamage(CAttackerInterface *attacker, EDamageType eDmgType);
  CDamage(CAttackerInterface *attacker, EDamageType eDmgType, const SPATIAL_VECTOR &vDirection);
  CDamage(CAttackerInterface *attacker, EDamageType eDmgType, const SPATIAL_VECTOR &vDirection, const float fKnockOutTime);
  CDamage(CAttackerInterface *attacker, EDamageType eDmgType, const SPATIAL_VECTOR &vDirection, const Hitpoints hpDamageValue);
  CDamage(CAttackerInterface *attacker, EDamageType eDmgType, const SPATIAL_VECTOR &vDirection, const Hitpoints hpDamageValue, const float fKnockOutTime);

  CAttackerInterface *getAttacker() const {return mAttacker;}
  const SPATIAL_VECTOR &getDamageDirection() const {return m_vDamageDirection;}
  const Hitpoints getDamageValue() const {return m_hpDamageValue;}
  const EDamageType getDamageType() const {return m_eDamageType;}
  const float getKnockOutTime() const {return m_fKnockOutTime;}
};

#endif // _DAMAGE_HPP_
