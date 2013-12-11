#ifndef _PICKABLE_OBJECT_H_
#define _PICKABLE_OBJECT_H_

#include "Object.h"
#include "InnerObjectInterface.h"

class CPhysicsManager;
class CMap;
class CPickableObject;

class CPickableObject : public CObject, public InnerObjectInterface {
public:
	enum EPickableObjectTypes {
		POT_EMPTY		= 0,
		POT_HEART,
		POT_GREEN_RUBY,
		POT_BLUE_RUBY,
		POT_RED_RUBY,
		POT_SMALL_POTION,

		// tools that can be picked
		POT_BOW,
		POT_LAMP,

		POT_COUNT,
	};
private:
	EPickableObjectTypes m_ePickableObjectType;
	Ogre::SceneNode *m_pSceneNode;
	bool m_bIsPicked;
public:
	static EPickableObjectTypes parsePickableObjectType(const Ogre::String &text);

	CPickableObject(Ogre::SceneNode *pParentNode, CObjectManager &objectManager, EPickableObjectTypes ePickableObjectType, const Ogre::Vector3 &vPosition);
	~CPickableObject();

	EPickableObjectTypes getPickableObjectType() const {return m_ePickableObjectType;}


	virtual void update(Ogre::Real tpf);
	void pickupObject();
	//virtual bool interactOnCollision(const btCollisionObject *pPartner);


};

#endif
