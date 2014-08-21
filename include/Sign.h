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
