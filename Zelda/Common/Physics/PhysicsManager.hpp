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

#ifndef _PHYSICS_MANAGER_H_
#define _PHYSICS_MANAGER_H_

#include <OgreVector3.h>
#include <OgreException.h>
#include "../Input/InputListener.hpp"
#include "../Message/MessageInjector.hpp"

#define PHYSICS_MANAGER_DEBUG 1

namespace BtOgre {class DebugDrawer;}
class btDiscreteDynamicsWorld;
class btBroadphaseInterface;
class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btSequentialImpulseConstraintSolver;
class btGhostPairCallback;
class btCollisionObject;
class btCollisionShape;
class btCollisionWorld;
class btRigidBody;

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
	btCollisionShape *getShape() const {return m_pShape;}
	const Ogre::Vector3 &getOffset() const {return m_vOffset;}

	void operator=(const CPhysicsCollisionObject &colObj) {
		m_pShape = colObj.m_pShape;
		m_vOffset = colObj.m_vOffset;
	}
	bool operator==(const CPhysicsCollisionObject &colObj) {
		return (m_pShape == colObj.m_pShape && m_vOffset == colObj.m_vOffset);
	}
};
class CPhysicsManager
#if PHYSICS_MANAGER_DEBUG == 1
: public CInputListener,
  public CMessageInjector
#endif
{
public:
  static const float GRAVITY_FACTOR;
private:
	//NxOgre::World *mWorld;
	btDiscreteDynamicsWorld *m_pPhyWorld;
  BtOgre::DebugDrawer *m_pDbgDraw;
  btBroadphaseInterface *mBroadphaseInterface;
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

	void exit();

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
	void createLater(btCollisionObject *pCO) {m_Messages.push_back(new CPhysicsMessage(CPhysicsMessage::PMT_CREATE, pCO));}

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


#if PHYSICS_MANAGER_DEBUG == 1
  void sendMessageToAll(const CMessage &message);
  bool keyPressed( const OIS::KeyEvent &arg ) {
  	if (arg.key == OIS::KC_P) {
			toggleDisplayDebugInfo();
  	}
  	return true;
	}
#endif
};

#endif
