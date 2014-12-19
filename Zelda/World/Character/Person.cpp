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
#include <BulletDynamics/Character/btKinematicCharacterController.h>
#include <string>
#include "PersonController.hpp"
#include "../Atlas/Map.hpp"
#include "../../Common/Util/DebugDrawer.hpp"
#include "CharacterController_Physics.hpp"
#include <OgreStringVector.h>
#include <OgreStringConverter.h>
#include <OgreEntity.h>
#include <OgreBone.h>
#include <OgreSkeletonInstance.h>
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>
#include <OgreSubMesh.h>
#include "../../Common/Physics/PhysicsMasks.hpp"
#include "../GlobalCollisionShapesTypes.hpp"
#include "PersonTypes.hpp"
#include "../../Common/Log.hpp"
#include "../../Common/Util/XMLHelper.hpp"
#include "../Items/CharacterItem.hpp"

using XMLHelper::Attribute;

// const Ogre::Real PERSONS_MASS = 1.0f;
const Ogre::String CPerson::PERSON_SHEATH("Sheath");
const Ogre::String CPerson::PERSON_SHIELD_PACKED("ShieldPacked");
const Ogre::String CPerson::PERSON_LEFT_HANDLE("Handle_L");
const Ogre::String CPerson::PERSON_RIGHT_HANDLE("Handle_R");
const Ogre::Real CPerson::PERSON_HEIGHT = 0.10f;
const Ogre::Real CPerson::PERSON_RADIUS = 0.03f;
const Ogre::Real CPerson::PERSON_PHYSICS_OFFSET = PERSON_HEIGHT / 2 + 0.005;
const Ogre::Real CPerson::PERSON_FLOOR_OFFSET = PERSON_HEIGHT / 2;


CPerson::CPerson(const std::string &sID,
                   CEntity *pParent,
                   CMap *pMap,
                   const SPersonData &personData,
                   unsigned int uiAnimationCount)
    : CCharacter(sID,
                 pParent,
                 pMap,
                 personData),
      m_PersonData(personData) {
  m_degLeftHandleCurrentRotation = 0;
  m_degLeftHandleRotationSpeed = 0;
  m_degLeftHandleRotationToTarget = 0;

  m_uiTakeDamageFlags = m_uiBlockDamageFlags = DMG_ALL;
  setCurAndMaxHP(personData.hitpoints);
}

CPerson::CPerson(const tinyxml2::XMLElement *pElem,
                 CEntity *pParent,
                 CMap *pMap,
                 unsigned int uiAnimationCount)
  : CCharacter(pElem,
               pParent,
               pMap,
               CPersonDataIdMap::getSingleton().toData(
                   CPersonTypeIdMap::getSingleton().parseString(
                       Attribute(pElem, "person_type")))),
    m_PersonData(CPersonDataIdMap::getSingleton().toData(
        CPersonTypeIdMap::getSingleton().parseString(Attribute(pElem, "person_type")))) {
  m_degLeftHandleCurrentRotation = 0;
  m_degLeftHandleRotationSpeed = 0;
  m_degLeftHandleRotationToTarget = 0;

  m_uiTakeDamageFlags = m_uiBlockDamageFlags = DMG_ALL;
  setCurAndMaxHP(m_PersonData.hitpoints);
}

CPerson::~CPerson() {
}

void CPerson::exit() {
  removeBlinkingMaterials();
  CCharacter::exit();
}

CCharacterController *CPerson::createCharacterController() {
  return new CPersonController(this);
}


const Ogre::Vector3 CPerson::getFloorPosition() const {
  return m_pSceneNode->getPosition() - Ogre::Vector3(0, PERSON_FLOOR_OFFSET, 0);
}

void CPerson::createPhysics() {
    ASSERT(m_pSceneNode);
    ASSERT(!mCCPhysics);

    btTransform startTransform;
    startTransform.setIdentity();
    startTransform.setOrigin(btVector3(0, 0, 0));
    Ogre::Vector3 origin(0, 0, 0);

    btPairCachingGhostObject * characterGhostObject
        = new btPairCachingGhostObject();
    characterGhostObject->setWorldTransform(startTransform);

    btScalar stepHeight = 0.0005f;

    const CPhysicsCollisionObject &pco
        = m_pMap->getPhysicsManager()->getCollisionShape(
            CGlobalCollisionShapesTypesIdMap::getSingleton().toString(GCST_PERSON_CAPSULE));
    btConvexShape * capsule = dynamic_cast<btConvexShape*>(pco.getShape());
    characterGhostObject->setCollisionShape(capsule);
    // characterGhostObject->setCollisionFlags(getCollisionGroup());

    /*// duck setup
    btConvexShape * duck = new btCapsuleShape(characterWidth, characterHeight / 3);
    mCollisionShapes.push_back(duck);

    btCylinderShape *pCylShape = new btCylinderShape(btVector3(characterWidth, 0.2, characterWidth) * 0.5);
    mCollisionShapes.push_back(pCylShape);

    btCompoundShape *pComShape = new btCompoundShape();
    mCollisionShapes.push_back(pComShape);
    pComShape->addChildShape(btTransform(btQuaternion::getIdentity(), btVector3(0, - characterHeight - 0.005, 0)), pCylShape);
    pComShape->addChildShape(btTransform::getIdentity(), capsule);*/


    // mCharacter = new CharacterControllerManager(mSceneManager, mCamera, characterGhostObject, capsule, stepHeight, CPhysicsManager::getSingleton().getCollisionWorld(), origin);
    btCharacterControllerInterface *pCC = new CharacterControllerPhysics(*this, characterGhostObject, capsule, stepHeight);
    // btCharacterControllerInterface *pCC = new btKinematicCharacterController(characterGhostObject, capsule, stepHeight, 1);
    mCCPhysics = pCC;
    // pCC->setMaxSlope(0.5);
    // pCC->setBorderDetectionShapeOffset(btVector3(0, -characterHeight - 0.02, 0));
    // mCCPhysics->setDuckingConvexShape(duck);

    // m_pCurrentMap->getPhysicsManager()->getWorld()->addCollisionObject(characterGhostObject, btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter);
    m_pMap->getPhysicsManager()->getWorld()->addCollisionObject(characterGhostObject, getCollisionGroup(), getCollisionMask());
    m_pMap->getPhysicsManager()->getWorld()->addAction(mCCPhysics);

    /// --

    m_pCollisionObject = characterGhostObject;
    // m_pBodyPhysics->setCollisionShape(pComShape);

    m_pCollisionObject->setWorldTransform(
        btTransform(btQuaternion::getIdentity(), btVector3(0, 20, 0)));
    // mCCPlayer->setIsMoving(true);

    // pCC->start();
    // pCC->setBorderDetectionShapeGroupAndMask(getCollisionGroup(), COL_WALL);

  setThisAsCollisionObjectsUserPointer();
}
void CPerson::destroyPhysics() {
  CPhysicsManager *phyManager(m_pMap->getPhysicsManager());
  if (mCCPhysics) {
    phyManager->getWorld()->removeAction(mCCPhysics);
    delete mCCPhysics;
    mCCPhysics = NULL;
  }
  if (m_pCollisionObject) {
    phyManager->getWorld()->removeCollisionObject(m_pCollisionObject);
    phyManager->getBroadphase()->resetPool(
        m_pMap->getPhysicsManager()->getWorld()->getDispatcher());
    phyManager->getBroadphase()->getOverlappingPairCache()->cleanProxyFromPairs(
        m_pCollisionObject->getBroadphaseHandle(),
        m_pMap->getPhysicsManager()->getWorld()->getDispatcher());

    delete m_pCollisionObject;
    m_pCollisionObject = NULL;
  }

  for (int i = 0; i <mCollisionShapes.size(); i++) {
    delete mCollisionShapes[i];
  }
  mCollisionShapes.clear();
}

void CPerson::setLeftHandleRotation(const Ogre::Degree &degree,
                                    Ogre::Real speed) {
    m_degLeftHandleRotationToTarget = degree - m_degLeftHandleCurrentRotation;
    if (speed == 0) {
        // find shortest direction and give auto speed
        if (m_degLeftHandleRotationToTarget > Ogre::Degree(180)) {
            m_degLeftHandleRotationToTarget -= Ogre::Degree(360);
        } else if (m_degLeftHandleRotationToTarget < Ogre::Degree(-180)) {
            m_degLeftHandleRotationToTarget += Ogre::Degree(360);
        }
        m_degLeftHandleRotationSpeed
            = m_degLeftHandleRotationToTarget
            / abs(m_degLeftHandleRotationToTarget.valueDegrees()) * 500;
    } else {
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
    m_pBodyEntity = pParentSceneNode->getCreator()
        ->createEntity(pModelSN->getName() + ".mesh", meshName + ".mesh");
    m_pBodyEntity->setCastShadows(true);
    pModelSN->attachObject(m_pBodyEntity);

    createBlinkingMaterials();
}

void CPerson::moveToTarget(const SPATIAL_VECTOR &vPosition,
                           const Ogre::Quaternion &qRotation,
                           const Ogre::Real fMaxDistanceDeviation,
                           const Ogre::Radian fMaxAngleDeviation) {
  CPersonController *pPC(getCharacterController<CPersonController>());
  ASSERT(pPC);
  pPC->moveToTarget(vPosition,
                    fMaxDistanceDeviation, fMaxAngleDeviation, false);
}

void CPerson::postUpdateAnimationsCallback(const Ogre::Real fTime) {
}
void CPerson::updateAnimationsCallback(const Ogre::Real fTime) {
    CCharacter::updateAnimationsCallback(fTime);

  if (m_uiAnimID == ANIM_SLICE_HORIZONTAL) {
    Ogre::Real fAnimPart = m_fTimer
        / m_Anims[ANIM_SLICE_HORIZONTAL]->getLength();
    if (fAnimPart > 0.8) {
    } else if (fAnimPart > 0.4) {
      getCurrentItem(CIS_WEAPON)->updateDamage(fTime);
    }
  } else if (m_uiAnimID == ANIM_USE_ITEM) {
    Ogre::Real fAnimPart = m_fTimer
        / m_Anims[ANIM_USE_ITEM]->getLength();
    if (fAnimPart > 0.8) {
    } else if (fAnimPart > 0.4) {
      getCurrentItem(CIS_TOOL)->updateDamage(fTime);
    }
  }
}

bool CPerson::collidesWith(const std::string &sEntityID) const {
  for (const CWorldEntity *pWE :
           dynamic_cast<CharacterControllerPhysics*>(mCCPhysics)
           ->getCollidingWorldEntities()) {
    // check if objects are part of the same map
    // (problems when switching maps elsewise) and the ids match
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
  ASSERT(m_pBodyEntity);

  // find name of material to clone it
  Ogre::String materialName = m_PersonData.sMaterialName;
  if (materialName.size() == 0) {
    // use the applied material of the entity
    materialName = m_pBodyEntity->getMesh()->getSubMesh(0)->getMaterialName();
  }
 
  // get the material
  Ogre::MaterialPtr srcMat
      = Ogre::MaterialManager::getSingleton().getByName(materialName);
  if (srcMat.isNull()) {
    // material does not exist, error!
    throw Ogre::Exception(Ogre::Exception::ERR_ITEM_NOT_FOUND,
                          "Material with name '"
                          + materialName + "' not found",
                          __FILE__);
  }
  // clone the material and apply it to the entity
  m_pMaterial = srcMat->clone(m_sID + "mat_" + materialName);
  m_pBodyEntity->setMaterial(m_pMaterial);
}

void CPerson::removeBlinkingMaterials() {
  Ogre::MaterialManager::getSingleton().remove(m_pMaterial->getName());
}

void CPerson::startBeingInvulnerableCallback() {
  m_pMaterial->getSupportedTechnique(0)->getPass(0)
      ->getFragmentProgramParameters()
      ->setNamedConstant("blinking_intensity", 1.f);
}

void CPerson::endBeingInvulnerableCallback() {
  m_pMaterial->getSupportedTechnique(0)->getPass(0)
      ->getFragmentProgramParameters()
      ->setNamedConstant("blinking_intensity", 0.f);
}

void CPerson::myDamageBlocked(const CDamage &damage,
                              CHitableInterface *hitInterface) {
  getCharacterController<CPersonController>()->changeMoveState(
      CPersonController::MS_PUSHED_BACK,
      -damage.getDamageDirection() * 0.2f,
      0.25f);
}

void CPerson::requestingJumpSpeed(float *horizontal, float *vertical) {
  getCharacterController<CPersonController>()
      ->requestingJumpSpeed(horizontal, vertical);
}

void CPerson::jumpStart() {
  getCharacterController<CPersonController>()->jumpStart();
}

void CPerson::jumpEnd() {
  getCharacterController<CPersonController>()->jumpEnd();
}
