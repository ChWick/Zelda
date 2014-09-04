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

#include "Person.hpp"
#include "PersonController.hpp"
#include "../Atlas/Map.hpp"
//#include "BlinkingMaterialManager.h"
#include "../../Common/Util/DebugDrawer.hpp"
#include "CharacterController_Physics.hpp"
#include <OgreStringVector.h>
#include <OgreStringConverter.h>
#include <OgreEntity.h>
#include <OgreBone.h>
#include <OgreSkeletonInstance.h>
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>
#include "../../Common/Physics/PhysicsMasks.hpp"
#include <BulletDynamics/Character/btKinematicCharacterController.h>
#include "../GlobalCollisionShapesTypes.hpp"
#include "PersonTypes.hpp"
#include "../../Common/Log.hpp"

//const Ogre::Real PERSONS_MASS = 1.0f;
const Ogre::String CPerson::PERSON_SHEATH("Sheath");
const Ogre::String CPerson::PERSON_SHIELD_PACKED("ShieldPacked");
const Ogre::String CPerson::PERSON_LEFT_HANDLE("Handle_L");
const Ogre::String CPerson::PERSON_RIGHT_HANDLE("Handle_R");
const Ogre::Real CPerson::PERSON_HEIGHT = 0.10f;
const Ogre::Real CPerson::PERSON_RADIUS = 0.03f;
const Ogre::Real CPerson::PERSON_PHYSICS_OFFSET = PERSON_HEIGHT / 2 + 0.005;
const Ogre::Real CPerson::PERSON_FLOOR_OFFSET = PERSON_HEIGHT / 2;


CPerson::CPerson(const std::string &sID, CEntity *pParent, const SPersonData &personData, unsigned int uiAnimationCount)
  : CCharacter(sID, pParent, personData.eFriendOrEnemyState, uiAnimationCount),
  m_PersonData(personData) {
	m_degLeftHandleCurrentRotation = 0;
	m_degLeftHandleRotationSpeed = 0;
	m_degLeftHandleRotationToTarget = 0;

	m_uiTakeDamageFlags = m_uiBlockDamageFlags = DMG_ALL;
  setCurAndMaxHP(personData.hitpoints);
}
CPerson::~CPerson() {
}
CCharacterController *CPerson::createCharacterController() {
	return new CPersonController(this);
}


const Ogre::Vector3 CPerson::getFloorPosition() const {
  return m_pSceneNode->getPosition() - Ogre::Vector3(0, PERSON_FLOOR_OFFSET, 0);
}

void CPerson::destroy() {
	removeBlinkingMaterials();
	CCharacter::destroy();
}
void CPerson::createPhysics() {
    using namespace Ogre;

    assert(m_pSceneNode);

    btTransform startTransform;
    startTransform.setIdentity();
    startTransform.setOrigin(btVector3(0, 0, 0));
    Vector3 origin(0, 0, 0);

    btPairCachingGhostObject * characterGhostObject = new btPairCachingGhostObject();
    characterGhostObject->setWorldTransform(startTransform);

    btScalar stepHeight = 0.0005f;

    const CPhysicsCollisionObject &pco = m_pMap->getPhysicsManager()->getCollisionShape(GLOBAL_COLLISION_SHAPES_TYPES_ID_MAP.toString(GCST_PERSON_CAPSULE));
    btConvexShape * capsule = dynamic_cast<btConvexShape*>(pco.getShape());
    characterGhostObject->setCollisionShape(capsule);
    //characterGhostObject->setCollisionFlags(getCollisionGroup());

    /*// duck setup
    btConvexShape * duck = new btCapsuleShape(characterWidth, characterHeight / 3);
    mCollisionShapes.push_back(duck);

    btCylinderShape *pCylShape = new btCylinderShape(btVector3(characterWidth, 0.2, characterWidth) * 0.5);
    mCollisionShapes.push_back(pCylShape);

    btCompoundShape *pComShape = new btCompoundShape();
    mCollisionShapes.push_back(pComShape);
    pComShape->addChildShape(btTransform(btQuaternion::getIdentity(), btVector3(0, - characterHeight - 0.005, 0)), pCylShape);
    pComShape->addChildShape(btTransform::getIdentity(), capsule);*/


    //mCharacter = new CharacterControllerManager(mSceneManager, mCamera, characterGhostObject, capsule, stepHeight, CPhysicsManager::getSingleton().getCollisionWorld(), origin);
    btCharacterControllerInterface *pCC = new CharacterControllerPhysics(*this, characterGhostObject, capsule, stepHeight);
    //btCharacterControllerInterface *pCC = new btKinematicCharacterController(characterGhostObject, capsule, stepHeight, 1);
    mCCPhysics = pCC;
    //pCC->setMaxSlope(0.5);
    //pCC->setBorderDetectionShapeOffset(btVector3(0, -characterHeight - 0.02, 0));
    //mCCPhysics->setDuckingConvexShape(duck);

    //m_pCurrentMap->getPhysicsManager()->getWorld()->addCollisionObject(characterGhostObject, btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter);
    m_pMap->getPhysicsManager()->getWorld()->addCollisionObject(characterGhostObject, getCollisionGroup(), getCollisionMask());
    m_pMap->getPhysicsManager()->getWorld()->addAction(mCCPhysics);

    /// --

    m_pCollisionObject = characterGhostObject;
    //m_pBodyPhysics->setCollisionShape(pComShape);

    m_pCollisionObject->setWorldTransform(btTransform(btQuaternion::getIdentity(), btVector3(0, 20, 0)));
    //mCCPlayer->setIsMoving(true);

    //pCC->start();
    //pCC->setBorderDetectionShapeGroupAndMask(getCollisionGroup(), COL_WALL);

  setThisAsCollisionObjectsUserPointer();
}
void CPerson::destroyPhysics() {

	if (mCCPhysics) {
		m_pMap->getPhysicsManager()->getWorld()->removeAction(mCCPhysics);
		delete mCCPhysics;
		mCCPhysics = NULL;
	}
	if (m_pCollisionObject) {
		m_pMap->getPhysicsManager()->getWorld()->removeCollisionObject(m_pCollisionObject);

		m_pMap->getPhysicsManager()->getBroadphase()->resetPool(m_pMap->getPhysicsManager()->getWorld()->getDispatcher());
		m_pMap->getPhysicsManager()->getBroadphase()->getOverlappingPairCache()->cleanProxyFromPairs(m_pCollisionObject->getBroadphaseHandle(), m_pMap->getPhysicsManager()->getWorld()->getDispatcher());

		delete m_pCollisionObject;
		m_pCollisionObject = NULL;
	}

	for (int i = 0; i <mCollisionShapes.size(); i++) {
		delete mCollisionShapes[i];
	}
	mCollisionShapes.clear();
}

void CPerson::setLeftHandleRotation(const Ogre::Degree &degree, Ogre::Real speed) {
    m_degLeftHandleRotationToTarget = degree - m_degLeftHandleCurrentRotation;
    if (speed == 0) {
        // find shortest direction and give auto speed
        if (m_degLeftHandleRotationToTarget > Ogre::Degree(180)) {
            m_degLeftHandleRotationToTarget -= Ogre::Degree(360);
        }
        else if (m_degLeftHandleRotationToTarget < Ogre::Degree(-180)) {
            m_degLeftHandleRotationToTarget += Ogre::Degree(360);
        }
        m_degLeftHandleRotationSpeed = m_degLeftHandleRotationToTarget / abs(m_degLeftHandleRotationToTarget.valueDegrees()) * 500;
    }
    else {
        m_degLeftHandleRotationSpeed = Ogre::Degree(speed);
    }
}

void CPerson::initBody(Ogre::SceneNode *pParentSceneNode) {
	Ogre::String meshName = m_PersonData.sMeshName;
    // create main model
    m_pSceneNode = pParentSceneNode->createChildSceneNode(m_sID + meshName);
    Ogre::SceneNode *pModelSN = m_pSceneNode->createChildSceneNode();
    pModelSN->setScale(m_PersonData.vScale);
    pModelSN->setPosition(0, -PERSON_PHYSICS_OFFSET, 0);
    m_pBodyEntity = pParentSceneNode->getCreator()->createEntity(pModelSN->getName() + ".mesh", meshName + ".mesh");
    m_pBodyEntity->setCastShadows(true);
    pModelSN->attachObject(m_pBodyEntity);

    m_pSceneNode->setUserAny(Ogre::Any(dynamic_cast<CCharacter*>(this)));
    m_pBodyEntity->setUserAny(Ogre::Any(dynamic_cast<CCharacter*>(this)));

	createBlinkingMaterials();
}

void CPerson::createHandObject(const Ogre::String &parentBone, EHands handPos, const Ogre::String &meshName) {
  m_pBodyEntity->attachObjectToBone(parentBone, m_pSceneNode->getCreator()->createEntity(meshName));
}

void CPerson::postUpdateAnimationsCallback(const Ogre::Real fTime) {

}
void CPerson::updateAnimationsCallback(const Ogre::Real fTime) {
    CCharacter::updateAnimationsCallback(fTime);

  if (m_uiAnimID == ANIM_SLICE_HORIZONTAL) {
    Ogre::Real fAnimPart = m_fTimer / m_Anims[ANIM_SLICE_HORIZONTAL]->getLength();
    if (fAnimPart > 0.8) {
    }
    else if (fAnimPart > 0.4) {
      const Ogre::Vector3 vDir = m_pBodyEntity->getParentNode()->convertLocalToWorldOrientation(m_pBodyEntity->getSkeleton()->getBone(PERSON_LEFT_HANDLE)->_getDerivedOrientation()).yAxis();
      const Ogre::Vector3 vPos = m_pBodyEntity->getParentNode()->convertLocalToWorldPosition(m_pBodyEntity->getSkeleton()->getBone(PERSON_LEFT_HANDLE)->_getDerivedPosition());

      DebugDrawer::getSingleton().drawLine(vPos, vPos + vDir * 0.08, Ogre::ColourValue::Blue);
      createDamage(Ogre::Ray(vPos, vDir * 0.08), CDamage(DMG_SWORD, m_pSceneNode->getOrientation().zAxis()));
    }
  }
}

bool CPerson::collidesWith(const std::string &sEntityID) const {
  for (const CWorldEntity *pWE : dynamic_cast<CharacterControllerPhysics*>(mCCPhysics)->getCollidingWorldEntities()) {
    // check if objects are part of the same map (problems when switching maps elsewise) and the ids match
    if (pWE->getMap() == m_pMap && pWE->getID() == sEntityID) {
      return true;
    }
  }
  return false;
}

bool CPerson::frameEnded(const Ogre::FrameEvent& evt) {
  dynamic_cast<CharacterControllerPhysics*>(mCCPhysics)->prepare();
  return true;
}

void CPerson::createBlinkingMaterials() {
	assert(m_pBodyEntity);
  m_pMaterial = Ogre::MaterialManager::getSingleton().getByName(m_PersonData.sMaterialName)->clone(m_sID + "mat_" + m_PersonData.sMaterialName);
  m_pBodyEntity->setMaterial(m_pMaterial);
}
void CPerson::removeBlinkingMaterials() {
	Ogre::MaterialManager::getSingleton().remove(m_pMaterial->getName());
}
void CPerson::startBeingInvulnerableCallback() {
	m_pMaterial->getSupportedTechnique(0)->getPass(0)->getFragmentProgramParameters()->setNamedConstant("blinking_intensity", 1.f);
}
void CPerson::endBeingInvulnerableCallback() {
	m_pMaterial->getSupportedTechnique(0)->getPass(0)->getFragmentProgramParameters()->setNamedConstant("blinking_intensity", 0.f);
}


void CPerson::attack(unsigned int uiTool) {
  if (isReadyForNewAction()) {
    animAttack();
    dynamic_cast<CPersonController*>(m_pCharacterController)->stun(m_Anims[ANIM_SLICE_HORIZONTAL]->getLength());
  }
}
