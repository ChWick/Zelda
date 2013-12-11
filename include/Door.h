#pragma once

#include "Object.h"
#include "MapLinkInterface.h"

class CMap;

class CDoor : public CObject, public CMapLinkInterface {
private:
	const Ogre::Radian m_rYaw;
	const Ogre::Vector3 m_vPosition;
public:
	CDoor(Ogre::SceneNode *pDoorNode, CMapManager &mapManager, CObjectManager &objectManager, const UserData &userData, const Ogre::String &id);
	virtual ~CDoor();

private:

	//! player interacts with object active
	bool interactOnKeypress(EInteractType eInteractType, const Ogre::Vector3 &vInteractDir);
	//! if its a object that interacts on collision (e.g. items, oder a link)
	//bool interactOnCollision() {return false;}
};
