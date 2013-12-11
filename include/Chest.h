#ifndef _CHEST_H_
#define _CHEST_H_

#include "Object.h"
#include "PickableObject.h"
#include "HUD.h"
#include "InnerObjectContainerInterface.h"

class CChest : public CObject, public CGameStateChanger, public InnerObjectContainerInterface {
public:
	enum EChestType {
		CT_SMALL,
		CT_BIG,
	};
	enum EChestState {
		CS_CLOSED,
		CS_OPENING,
		CS_OPENED,
	};
private:
	const EChestType m_eChestType;
	EChestState m_eChestState;
	btRigidBody *m_pLidPhysics;
	Ogre::SceneNode *m_pLidSceneNode;

	CPickableObject *m_pInnerObject;

	Ogre::Real m_fOpenPitch;
	Ogre::Vector3 m_vInnerObjectPos;
	Ogre::Vector3 m_vPhysicsOffset;
public:
	CChest(Ogre::SceneNode *pNode, CObjectManager &objectManager, btCollisionObject *pCollisionObject, EChestType eChestType, const Ogre::String &id);
	~CChest();


	virtual void update(Ogre::Real tpf);

	virtual bool interactOnKeypress(EInteractType eInteractType, const Ogre::Vector3 &vInteractDir);
};

#endif
