#ifndef _SIGN_H_
#define _SIGN_H_

#include "LiftableObject.h"
#include "ChatableInterface.h"

class CSign : public CLiftableObject, public ChatableInterface {
private:
	const Ogre::Quaternion m_qOrientation;
	const Ogre::Vector3 m_vPosition;
public:
	CSign(Ogre::SceneNode *pNode, CObjectManager &objectManager, btCollisionObject *pCollisionObject, const UserData &userData, const Ogre::String &sMessageFront, const Ogre::String &sMessageRear)
		: CLiftableObject(pNode, objectManager, pCollisionObject, LOT_SIGN, true, userData),
		m_vPosition(pNode->getPosition()),
		m_qOrientation(pNode->getOrientation()) {
		setChatText(sMessageFront, ID_FRONT);
		setChatText(sMessageRear, ID_REAR);
	}
	virtual ~CSign() {
	}

	const Ogre::Vector3 &getPosition() const {return m_vPosition;}
	const Ogre::Quaternion &getOrientation() const {return m_qOrientation;}

	virtual bool interactOnKeypress(EInteractType eInteractType, const Ogre::Vector3 &vInteractDir);
};

#endif
