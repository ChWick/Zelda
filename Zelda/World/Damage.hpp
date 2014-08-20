#ifndef _DAMAGE_HPP_
#define _DAMAGE_HPP_

#include "Hitpoints.hpp"
#include "DamageTypes.hpp"
#include "../Common/Config/TypeDefines.hpp"
#include SPATIAL_VECTOR_HEADER

class CDamage {
public:
    static const Hitpoints DMG_BASE[];
private:
	const EDamageType m_eDamageType;
	const Hitpoints m_hpDamageValue;		//!< Schaden in Prozent an Herzen (100 = 1 Herz, 50 = 1/2 Herz)
	const SPATIAL_VECTOR m_vDamageDirection;
	const float m_fKnockOutTime;			//!< Time how long the target will be stunned

public:
	CDamage(EDamageType eDmgType);
	CDamage(EDamageType eDmgType, const SPATIAL_VECTOR &vDirection);
	CDamage(EDamageType eDmgType, const SPATIAL_VECTOR &vDirection, const float fKnockOutTime);
	CDamage(EDamageType eDmgType, const SPATIAL_VECTOR &vDirection, const Hitpoints hpDamageValue);
	CDamage(EDamageType eDmgType, const SPATIAL_VECTOR &vDirection, const Hitpoints hpDamageValue, const float fKnockOutTime);

	const SPATIAL_VECTOR &getDamageDirection() const {return m_vDamageDirection;}
	const Hitpoints getDamageValue() const {return m_hpDamageValue;}
	const EDamageType getDamageType() const {return m_eDamageType;}
	const float getKnockOutTime() const {return m_fKnockOutTime;}
};

#endif // _DAMAGE_HPP_
