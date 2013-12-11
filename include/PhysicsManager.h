#ifndef _PHYSICS_MANAGER_H_
#define _PHYSICS_MANAGER_H_

#include "StdAfx.h"

const Ogre::Real GRAVITY_FACTOR = 8.0f;

namespace BtOgre {class DebugDrawer;}

enum ECollisionTypes {
    COL_NOTHING     = 0,    //!< Collide with nothing
    COL_STATIC      = 1,    //!< Collide with static objects
    COL_WALL        = 2,    //!< Collides with walls (for detection of borders)
	COL_CAMERA      = 4,   //!< Camera collision object
	COL_INTERACTIVE = 8,   //!< Collide with interactive


    COL_CHARACTER_P = 16,   //!< Friendly character (Positive)
    COL_CHARACTER_N = 32,   //!< Enemy character (Negative)
	COL_SHIELD_P    = 64,   //!< Collide with shield (only ray cast!)
	COL_SHIELD_N    = 128,  //!<
	COL_DAMAGE_P    = 256,  //!< Damage
	COL_DAMAGE_N    = 512,  //!<
};
const unsigned int MASK_NONE = COL_NOTHING;

const unsigned int MASK_PLAYER_P_COLLIDES_WITH = COL_STATIC | COL_WALL | COL_INTERACTIVE | COL_CHARACTER_P | COL_CHARACTER_N | COL_DAMAGE_N;
const unsigned int MASK_PLAYER_N_COLLIDES_WITH = COL_STATIC | COL_WALL | COL_INTERACTIVE | COL_CHARACTER_P | COL_CHARACTER_N | COL_DAMAGE_P;

const unsigned int MASK_PICKABLE_OBJECT_COLLIDES_WITH = COL_STATIC | COL_WALL;

const unsigned int MASK_BLOCKADE_COLLIDES_WITH = COL_CHARACTER_P;

const unsigned int MASK_STATIC_COLLIDES_WITH = COL_CHARACTER_N | COL_CHARACTER_P | COL_INTERACTIVE | COL_CAMERA | COL_DAMAGE_P | COL_DAMAGE_N;

const unsigned int MASK_SHIELD_P_COLLIDES_WITH = COL_DAMAGE_N;
const unsigned int MASK_SHIELD_N_COLLIDES_WITH = COL_DAMAGE_P;

const unsigned int MASK_CAMERA_COLLIDES_WITH = COL_STATIC | COL_WALL;
const unsigned int MASK_DAMAGE_P_COLLIDES_WITH = COL_CHARACTER_N | COL_SHIELD_N | COL_STATIC | COL_WALL | COL_INTERACTIVE;
const unsigned int MASK_DAMAGE_N_COLLIDES_WITH = COL_CHARACTER_P | COL_SHIELD_P | COL_STATIC | COL_WALL | COL_INTERACTIVE;
//int powerupCollidesWith = COL_SHIP | COL_WALL;

class CPhysicsMessage {
public:
	enum EType {
		PMT_CREATE,
		PMT_DELETE,
	};
private:
	const EType m_type;
	const btCollisionObject *m_pCollisionObject;

public:
	CPhysicsMessage(EType type, const btCollisionObject *pCollisionObject) :
		m_type(type), m_pCollisionObject(pCollisionObject) {
	}
	const EType getType() const {return m_type;}
	const btCollisionObject *getCollisionObject() const {return m_pCollisionObject;}
};
class CPhysicsCollisionObject {
private:
	btCollisionShape *m_pShape;
	Ogre::Vector3 m_vOffset;
public:
	CPhysicsCollisionObject(btCollisionShape *pShape = NULL, const Ogre::Vector3 &vOffset = Ogre::Vector3::ZERO)
		: m_pShape(pShape), m_vOffset(vOffset) {

	}
	CPhysicsCollisionObject(const CPhysicsCollisionObject &colObj)
		: m_pShape(colObj.m_pShape), m_vOffset(colObj.m_vOffset) {
	}
	btCollisionShape *getShape() {return m_pShape;}
	const Ogre::Vector3 &getOffset() const {return m_vOffset;}

	void operator=(const CPhysicsCollisionObject &colObj) {
		m_pShape = colObj.m_pShape;
		m_vOffset = colObj.m_vOffset;
	}
	bool operator==(const CPhysicsCollisionObject &colObj) {
		return (m_pShape == colObj.m_pShape && m_vOffset == colObj.m_vOffset);
	}
};
class CPhysicsManager {
public:
private:
	//NxOgre::World *mWorld;
    btDiscreteDynamicsWorld *m_pPhyWorld;
    BtOgre::DebugDrawer *m_pDbgDraw;
    btBroadphaseInterface *mBroadphase;
	btDefaultCollisionConfiguration *mCollisionConfig;
	btCollisionDispatcher *mDispatcher;
	btSequentialImpulseConstraintSolver *mSolver;
	btGhostPairCallback *m_pGhostPairCallback;

	Ogre::SceneManager *m_pSceneManager;

	bool m_bDisplayDebugInfo;

	std::map<Ogre::String, CPhysicsCollisionObject> m_CollisionObjects;

	Ogre::list<CPhysicsMessage*>::type m_Messages;
public:
    CPhysicsManager(Ogre::SceneManager *pSceneManager);
    ~CPhysicsManager();

    void update(Ogre::Real tpf);
	void disableDebugInfo() {
		m_bDisplayDebugInfo = true;
		toggleDisplayDebugInfo();
	}
	void toggleDisplayDebugInfo();

	inline btDiscreteDynamicsWorld *getWorld() const {return m_pPhyWorld;}
	btCollisionWorld * getCollisionWorld();
    btBroadphaseInterface * getBroadphase();

	void deleteLater(const btCollisionObject *pCO);
	void createLater(btRigidBody *pRB) {m_Messages.push_back(new CPhysicsMessage(CPhysicsMessage::PMT_CREATE, pRB));}

	// Collision shape handling
	bool hasCollisionShape(const Ogre::String &id) const {
		return m_CollisionObjects.find(id) != m_CollisionObjects.end();
	}
	CPhysicsCollisionObject &getCollisionShape(const Ogre::String &id) {
		assert(hasCollisionShape(id));
		return m_CollisionObjects[id];
	}
	void eraseCollisionShape(const btCollisionShape *pShape) {
        for (auto it = m_CollisionObjects.begin(); it != m_CollisionObjects.end(); it++) {
			if ((*it).second.getShape() == pShape) {
                m_CollisionObjects.erase(it);
                return;
			}
		}
	}
	void addCollisionShape(const Ogre::String &id, const CPhysicsCollisionObject &colobj) {
		assert(!hasCollisionShape(id));
		m_CollisionObjects[id] = colobj;
	}
	CPhysicsCollisionObject *findCollisionShape(const btCollisionShape *pShape) {
		assert(pShape);
		for (auto &co : m_CollisionObjects) {
			if (co.second.getShape() == pShape) {return &co.second;}
		}
		return NULL;
	}
	const Ogre::String &getCollisionShapeKey(const CPhysicsCollisionObject &colobj) {
		for (auto &co : m_CollisionObjects) {
			if (co.second == colobj) {return co.first;}
		}
		throw Ogre::Exception(0, "Collision shape key not found", __FILE__);
	}
};

#endif
