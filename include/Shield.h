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
