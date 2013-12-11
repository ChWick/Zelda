#ifndef _LIFTABLE_OBJECT_H_
#define _LIFTABLE_OBJECT_H_

#include "Object.h"
#include "InnerObjectContainerInterface.h"

class UserData;

class CLiftableObject : public CObject, public InnerObjectContainerInterface {
public:
	enum ELiftableObjectTypes {
		LOT_GREEN_BUSH = 0,
		LOT_LIGHT_STONE,
		LOT_SIGN,
		LOT_VASE,
		LOT_BOMB,

		LOT_SIZE,
	};
	enum EState {
		LOS_AT_DEFAULT_LOCATION,
		LOS_LIFTED,
		LOS_THROWN,
	};
	enum EInnerObjectCreateEvent {
	    IOCE_NEVER      = 0,
        IOCE_LIFT       = 1,
        IOCE_DESTROYED  = 2,

	};
private:
    unsigned int m_uiInnerObjectCreateEvent;
    inner_object_list_type m_InnerObject;
	const ELiftableObjectTypes m_eLiftableObjectType;
	EState m_eState;
	const bool m_bRecreateOnLift;                           //!< if the object is a bush, or stone,... (static object), the object has to be recreated on lifting it, but only if main map with paged geometry
public:
	CLiftableObject(Ogre::SceneNode *pNode, CObjectManager &objectManager, btCollisionObject *pCollisionObject, ELiftableObjectTypes eLiftableObjectType, bool bRecreateOnLift, const UserData &userData);
	virtual ~CLiftableObject();

	virtual bool interactOnKeypress(EInteractType eInteractType, const Ogre::Vector3 &vInteractDir);
	EReceiveDamageResult receiveDamage(const CDamage &damage);
	bool interactOnCollision(const btCollisionObject *pPartner);

	ELiftableObjectTypes getLiftableObjectType() const {return m_eLiftableObjectType;}
	EState getState() const {return m_eState;}
private:
    void sendCreateInnerObjectEvent(unsigned int uiEvent);

	void destroy();
	bool lift();
	bool throwObject();

};

#endif
