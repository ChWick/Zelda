#include "StdAfx.h"
#include "PickableObject.h"
#include "ObjectManager.h"
#include "Map.h"
#include "MapManager.h"
#include "Util.h"
#include "Player.h"
#include "PlayerData.h"
#include "PlayerTool.h"


const float PICKABLE_OBJECT_DEFAULT_MASS	= 1.0f;
const float PICKABLE_OBJECT_DEFAULT_RADIUS	= 0.25f;
const float PICKABLE_OBJECT_PICKUP_DISTANCE	= 0.30f;


CPickableObject::CPickableObject(Ogre::SceneNode *pParentNode, CObjectManager &objectManager, EPickableObjectTypes ePickableObjectType, const Ogre::Vector3 &vPosition)
	: CObject(objectManager,LIFTABLE_OBJECT), m_ePickableObjectType(ePickableObjectType), m_bIsPicked(false) {

	// create the object physics and model
	Ogre::Entity *pEntity(NULL);
	btCollisionShape *pCollisionShape(0);

	m_pSceneNode = pParentNode->createChildSceneNode();

	switch (m_ePickableObjectType) {
	case POT_HEART:
		pEntity = m_pSceneNode->getCreator()->createEntity("Heart.mesh");
		break;
	case POT_GREEN_RUBY:
		pEntity = m_pSceneNode->getCreator()->createEntity("ruby.mesh");
		pEntity->setMaterialName("Ruppee/Green");
		break;
	case POT_BLUE_RUBY:
		pEntity = m_pSceneNode->getCreator()->createEntity("ruby.mesh");
		pEntity->setMaterialName("Ruppee/Blue");
		break;
	case POT_RED_RUBY:
		pEntity = m_pSceneNode->getCreator()->createEntity("ruby.mesh");
		pEntity->setMaterialName("Ruppee/Red");
		break;
    case POT_SMALL_POTION:
        pEntity = m_pSceneNode->getCreator()->createEntity("SmallPotion.mesh");
        break;
    case POT_BOW:
        pEntity = m_pSceneNode->getCreator()->createEntity(CPlayerTool::MESH_FILES[CPlayerTool::TOOL_BOW]);
        m_pSceneNode->setScale(Ogre::Vector3::UNIT_SCALE * CPerson::PERSON_SCALE);
        break;
    case POT_LAMP:
        pEntity = m_pSceneNode->getCreator()->createEntity(CPlayerTool::MESH_FILES[CPlayerTool::TOOL_LAMP]);
        m_pSceneNode->setScale(Ogre::Vector3::UNIT_SCALE * CPerson::PERSON_SCALE);
        m_pSceneNode->roll(Ogre::Degree(90));
        m_pSceneNode->createChildSceneNode(Ogre::Vector3(-1.5, 0, 0))->attachObject(m_pSceneNode->getCreator()->createEntity("Lamp.mesh"));
        break;
	};
	pCollisionShape = new btSphereShape(PICKABLE_OBJECT_DEFAULT_RADIUS);

	assert(pEntity);
	assert(pCollisionShape);

	btRigidBody *pRB = new btRigidBody(PICKABLE_OBJECT_DEFAULT_MASS, new BtOgre::RigidBodyState(m_pSceneNode), pCollisionShape);
	m_pSceneNode->attachObject(pEntity);


	m_ObjectManager.getMap().getPhysicsManager()->getWorld()->addRigidBody(pRB, COL_INTERACTIVE, MASK_PICKABLE_OBJECT_COLLIDES_WITH);
	m_pCollisionObject = pRB;
	setAsUserPointer(pRB);

	pRB->getWorldTransform().setOrigin(BtOgre::Convert::toBullet(vPosition));

	pRB->setAngularVelocity(btVector3(0, 2, 0));
	pRB->setLinearFactor(btVector3(1.0f, 1.0f, 1.0f) * 0.25f);
	pRB->setLinearVelocity(btVector3(0, 1.0f, 0));
}
CPickableObject::~CPickableObject() {
	m_ObjectManager.getMap().getPhysicsManager()->getWorld()->removeCollisionObject(m_pCollisionObject);
	delete btRigidBody::upcast(m_pCollisionObject)->getMotionState();
	delete m_pCollisionObject->getCollisionShape();
	delete m_pCollisionObject;
	m_pCollisionObject = 0;

	destroySceneNode(m_pSceneNode, true);
}
void CPickableObject::update(Ogre::Real tpf) {
	Ogre::Vector3 playerPos(m_ObjectManager.getMap().getPlayer()->getPosition());
	Ogre::Vector3 myPos(m_pSceneNode->getPosition());

	playerPos.y = myPos.y = 0;
	if (!m_bIsPicked && playerPos.squaredDistance(myPos) < PICKABLE_OBJECT_PICKUP_DISTANCE * PICKABLE_OBJECT_PICKUP_DISTANCE) {
		pickupObject();
	}
}
void CPickableObject::pickupObject() {
	suicide();

	switch (m_ePickableObjectType) {
	case POT_HEART:
		m_ObjectManager.getMap().getPlayer()->changeHP(HP_ONE_HEART);
		break;
	case POT_GREEN_RUBY:
		CPlayerData::getSingleton().addRuby(RUBY_VALUE_GREEN);
		break;
	case POT_RED_RUBY:
		CPlayerData::getSingleton().addRuby(RUBY_VALUE_RED);
		break;
	case POT_BLUE_RUBY:
		CPlayerData::getSingleton().addRuby(RUBY_VALUE_BLUE);
		break;
    case POT_SMALL_POTION:
        CPlayerData::getSingleton().addMP(0.1f);
        break;
    case POT_BOW:
        CPlayerData::getSingleton().getPlayerTools().getTool(CPlayerTool::TOOL_BOW).setExists(true);
        break;
    case POT_LAMP:
        CPlayerData::getSingleton().getPlayerTools().getTool(CPlayerTool::TOOL_LAMP).setExists(true);
        break;
	};

	m_bIsPicked = true;
}
CPickableObject::EPickableObjectTypes CPickableObject::parsePickableObjectType(const Ogre::String &text) {
	Ogre::String lcName(text);
	Ogre::StringUtil::toLowerCase(lcName);
	if (lcName == "empty") {
		return POT_EMPTY;
	}
	else if (lcName == "greenrupee") {
		return POT_GREEN_RUBY;
	}
	else if (lcName == "redrupee") {
		return POT_RED_RUBY;
	}
	else if (lcName == "bluerupee") {
		return POT_BLUE_RUBY;
	}
	else if (lcName == "heart") {
		return POT_HEART;
	}
	else if (lcName == "smallpotion") {
        return POT_SMALL_POTION;
	}
	else if (lcName == "lamp") {
        return POT_LAMP;
	}
	else if (lcName == "bow") {
        return POT_BOW;
	}

	return POT_COUNT;
}
/*bool CPickableObject::interactOnCollision(const btCollisionObject *pPartner) {
	if (!m_bIsPicked && pPartner->getUserPointer() && dynamic_cast<CPlayer*>(static_cast<CPhysicsUserPointer*>(pPartner->getUserPointer()))) {
		CPlayer *pPlayer = dynamic_cast<CPlayer*>(static_cast<CPhysicsUserPointer*>(pPartner->getUserPointer()));
		m_ObjectManager.deleteObject(this);
		switch (m_ePickableObjectType) {
		case POT_HEART:
			pPlayer->changeHP(HP_ONE_HEART);
			break;
		case POT_GREEN_RUBY:
			CPlayerData::getSingleton().addRuby(RUBY_VALUE_GREEN);
			break;
		case POT_RED_RUBY:
			CPlayerData::getSingleton().addRuby(RUBY_VALUE_RED);
			break;
		case POT_BLUE_RUBY:
			CPlayerData::getSingleton().addRuby(RUBY_VALUE_BLUE);
			break;
		};

		m_bIsPicked = true;
	}
	return true;
}*/
