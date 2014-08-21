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
#include "CharacterController_Physics.hpp"
#include <OgreStringVector.h>
#include <OgreEntity.h>
#include <OgreBone.h>
#include <OgreSkeletonInstance.h>
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>
#include "../../Common/Physics/PhysicsMasks.hpp"
#include <BulletDynamics/Character/btKinematicCharacterController.h>

//const Ogre::Real PERSONS_MASS = 1.0f;
const Ogre::String CPerson::PERSON_SHEATH("Sheath");
const Ogre::String CPerson::PERSON_SHIELD_PACKED("ShieldPacked");
const Ogre::String CPerson::PERSON_LEFT_HANDLE("Handle.L");
const Ogre::String CPerson::PERSON_RIGHT_HANDLE("Handle.R");
const Ogre::Real CPerson::PERSON_HEIGHT = 0.04f;
const Ogre::Real CPerson::PERSON_RADIUS = 0.015f;
const Ogre::Real CPerson::PERSON_SCALE  = 1.f;


CPerson::CPerson(const std::string &sID, CEntity *pParent, const EFriendOrEnemyStates foe)
  : CCharacter(sID, pParent, foe) {
	m_degLeftHandleCurrentRotation = 0;
	m_degLeftHandleRotationSpeed = 0;
	m_degLeftHandleRotationToTarget = 0;

	m_uiTakeDamageFlags = m_uiBlockDamageFlags = DMG_ALL;
}
CPerson::~CPerson() {
}
CCharacterController *CPerson::createCharacterController() {
	return new CPersonController(this);
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

    btScalar characterHeight = PERSON_HEIGHT;
	btScalar characterWidth = PERSON_RADIUS;

    btConvexShape * capsule = new btCapsuleShape(characterWidth / 2, characterHeight / 2);
    capsule->setMargin(0.0);

    mCollisionShapes.push_back(capsule);
    characterGhostObject->setCollisionShape(capsule);
    characterGhostObject->setCollisionFlags(getCollisionGroup());

    // duck setup
    btConvexShape * duck = new btCapsuleShape(characterWidth, characterHeight / 3);
    mCollisionShapes.push_back(duck);

    btScalar stepHeight = 0.0005f;

    btCylinderShape *pCylShape = new btCylinderShape(btVector3(characterWidth, 0.2, characterWidth) * 0.5);
    mCollisionShapes.push_back(pCylShape);

    btCompoundShape *pComShape = new btCompoundShape();
    mCollisionShapes.push_back(pComShape);
    pComShape->addChildShape(btTransform(btQuaternion::getIdentity(), btVector3(0, - characterHeight - 0.005, 0)), pCylShape);
    pComShape->addChildShape(btTransform::getIdentity(), capsule);


    //mCharacter = new CharacterControllerManager(mSceneManager, mCamera, characterGhostObject, capsule, stepHeight, CPhysicsManager::getSingleton().getCollisionWorld(), origin);
  btCharacterControllerInterface *pCC = new CharacterControllerPhysics(characterGhostObject, capsule, stepHeight);
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
void CPerson::setupAnimations() {
	Ogre::StringVector animNames(ANIM_COUNT);
	animNames[ANIM_IDLE] = "Idle";
	animNames[ANIM_RUN] = "Run";
	animNames[ANIM_WALK] = "Walk";
	animNames[ANIM_HANDS_CLOSED] = "HandsClosed";
	animNames[ANIM_HANDS_RELAXED] = "HandsRelaxed";
	animNames[ANIM_SLICE_HORIZONTAL] = "SliceHorizontal";
	animNames[ANIM_BOW_SHOT] = "BowShot";
	animNames[ANIM_JUMP_START] = "JumpStart";
	animNames[ANIM_JUMP_LOOP] = "JumpLoop";
	animNames[ANIM_JUMP_END] = "JumpEnd";

	// this is very important due to the nature of the exported animations
    m_pBodyEntity->getSkeleton()->setBlendMode(Ogre::ANIMBLEND_CUMULATIVE);

    // populate our animation list
    for (unsigned int i = 0; i < m_uiAnimationCount; i++) {
        m_Anims[i] = m_pBodyEntity->getAnimationState(animNames[i]);
        m_Anims[i]->setLoop(true);
        m_FadingStates[i] = FADE_NONE;
        m_Anims[i]->setEnabled(false);
        m_Anims[i]->setWeight(0);
    }

    m_Anims[ANIM_HANDS_CLOSED]->setWeight(1);
    m_Anims[ANIM_HANDS_RELAXED]->setWeight(1);

    m_Anims[ANIM_JUMP_END]->setLoop(false);
    m_Anims[ANIM_JUMP_START]->setLoop(false);


	// relax the hands since we're not holding anything
	m_Anims[ANIM_HANDS_RELAXED]->setEnabled(true);


    // start off in the idle state (top and bottom together)
    setAnimation(ANIM_IDLE);

    // we will manually set the direction of the tool in the left hand
    Ogre::Bone *pLHandleBone = m_pBodyEntity->getSkeleton()->getBone(PERSON_LEFT_HANDLE);
    pLHandleBone->setManuallyControlled(true);
    int numAnimations = m_pBodyEntity->getSkeleton()->getNumAnimations();
    for(int i=0;i<numAnimations;i++){
        // remonveall possible tracks of the bone in the animations
       Ogre::Animation * anim = m_pBodyEntity->getSkeleton()->getAnimation(i);
       anim->destroyNodeTrack(pLHandleBone->getHandle());
    }
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
	Ogre::String meshName = "Cylinder";
    // create main model
    m_pSceneNode = pParentSceneNode->createChildSceneNode(m_sID + meshName);
    Ogre::SceneNode *pModelSN = m_pSceneNode->createChildSceneNode();
    pModelSN->setScale(PERSON_SCALE, PERSON_SCALE, PERSON_SCALE);
    m_pBodyEntity = pParentSceneNode->getCreator()->createEntity(pModelSN->getName() + ".mesh", meshName + ".mesh");
    m_pBodyEntity->setCastShadows(true);
    pModelSN->attachObject(m_pBodyEntity);

    m_pSceneNode->setUserAny(Ogre::Any(dynamic_cast<CCharacter*>(this)));
    m_pBodyEntity->setUserAny(Ogre::Any(dynamic_cast<CCharacter*>(this)));

	createBlinkingMaterials();

	pModelSN->setPosition(0, -PERSON_HEIGHT, 0);
}

void CPerson::createHandObject(const Ogre::String &parentBone, EHands handPos, const Ogre::String &meshName) {
}

void CPerson::postUpdateAnimationsCallback(const Ogre::Real fTime) {

}
void CPerson::updateAnimationsCallback(const Ogre::Real fTime) {
    CCharacter::updateAnimationsCallback(fTime);
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
/*	assert(m_pBodyEntity);

	for (unsigned int i = 0; i < m_pBodyEntity->getNumSubEntities(); i++) {
		auto pSubEnt = m_pBodyEntity->getSubEntity(i);
		CBlinkingMaterialManager::getSingleton().createMaterial(pSubEnt->getMaterialName());
	}*/
}
void CPerson::removeBlinkingMaterials() {
	/*assert(m_pBodyEntity);

	for (unsigned int i = 0; i < m_pBodyEntity->getNumSubEntities(); i++) {
		auto pSubEnt = m_pBodyEntity->getSubEntity(i);
		CBlinkingMaterialManager::getSingleton().removeMaterial(pSubEnt->getMaterialName());
	}*/
}
void CPerson::startBeingInvulnerableCallback() {
	/*for (unsigned int i = 0; i < m_pBodyEntity->getNumSubEntities(); i++) {
		m_pBodyEntity->getSubEntity(i)->setMaterialName(CBlinkingMaterialManager::getSingleton().getBlinkingMat(m_pBodyEntity->getSubEntity(i)->getMaterialName()));
	}*/
}
void CPerson::endBeingInvulnerableCallback() {
	/*for (unsigned int i = 0; i < m_pBodyEntity->getNumSubEntities(); i++) {
		m_pBodyEntity->getSubEntity(i)->setMaterialName(CBlinkingMaterialManager::getSingleton().getNonBlinkingMat(m_pBodyEntity->getSubEntity(i)->getMaterialName()));
	}*/
}
