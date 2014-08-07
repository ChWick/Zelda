#include "Damage.hpp"

const Hitpoints CDamage::DMG_BASE[] = {
    0,              // None
    0,              // World
    50,             // Base Sword
    100,            // Base Arrow
    100,            // Base hammer
    100,            // Base bomb
};

CDamage::CDamage(EDamageType eDmgType)
	: m_eDamageType(eDmgType), m_hpDamageValue(DMG_BASE[eDmgType]), m_vDamageDirection(SPATIAL_VECTOR::ZERO), m_fKnockOutTime(0) {
}
CDamage::CDamage(EDamageType eDmgType, const SPATIAL_VECTOR &vDirection)
	: m_eDamageType(eDmgType), m_hpDamageValue(DMG_BASE[eDmgType]), m_vDamageDirection(vDirection), m_fKnockOutTime(0) {
}
CDamage::CDamage(EDamageType eDmgType, const SPATIAL_VECTOR &vDirection, const float fKnockOutTime)
	: m_eDamageType(eDmgType), m_hpDamageValue(DMG_BASE[eDmgType]), m_vDamageDirection(vDirection), m_fKnockOutTime(fKnockOutTime) {
}
CDamage::CDamage(EDamageType eDmgType, const SPATIAL_VECTOR &vDirection, const Hitpoints hpDamageValue)
	: m_eDamageType(eDmgType), m_hpDamageValue(hpDamageValue), m_vDamageDirection(vDirection), m_fKnockOutTime(0) {
}
CDamage::CDamage(EDamageType eDmgType, const SPATIAL_VECTOR &vDirection, const Hitpoints hpDamageValue, float fKnockOutTime)
	: m_eDamageType(eDmgType), m_hpDamageValue(hpDamageValue), m_vDamageDirection(vDirection), m_fKnockOutTime(fKnockOutTime) {
}
