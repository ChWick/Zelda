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
