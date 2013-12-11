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
