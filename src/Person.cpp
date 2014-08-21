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

#include "StdAfx.h"
#include "Person.h"
#include "Map.h"
#include "Object.h"
#include "PersonController.h"
#include "BlinkingMaterialManager.h"
#include "Damage.h"
#include "DebugDrawer.h"
#include "Projectile.h"
#include "PlayerData.h"
#include "Bow.h"
#include "Bomb.h"
#include "Lamp.h"
#include "CharacterController_Physics.h"

//const Ogre::Real PERSONS_MASS = 1.0f;
const Ogre::String CPerson::PERSON_SHEATH("Sheath");
const Ogre::String CPerson::PERSON_SHIELD_PACKED("ShieldPacked");
const Ogre::String CPerson::PERSON_LEFT_HANDLE("Handle.L");
const Ogre::String CPerson::PERSON_RIGHT_HANDLE("Handle.R");
const Ogre::Real CPerson::PERSON_HEIGHT = 0.4f;
const Ogre::Real CPerson::PERSON_RADIUS = 0.2f;
const Ogre::Real CPerson::PERSON_SCALE  = 0.15f;


CPerson::CPerson(CMap *pMap, const EFriendOrEnemyStates foe) : CCharacter(pMap, foe) {
	m_bSwordsDrawn = false;
	m_pLiftedObject = NULL;
	m_bShieldBlockActive = false;
	m_bAiming = false;
	m_pShield = NULL;
	m_pCurrentTool = NULL;
	m_pCurrentProjectile = NULL;
	m_degLeftHandleCurrentRotation = 0;
	m_degLeftHandleRotationSpeed = 0;
	m_degLeftHandleRotationToTarget = 0;

	m_uiTakeDamageFlags = m_uiBlockDamageFlags = CDamage::DMG_ALL;
}
CPerson::~CPerson() {
	if (m_pShield) {delete m_pShield; m_pShield = NULL;}
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

    assert(m_pBodyNode);
    Ogre::SceneNode *pPhysicsSceneNode = m_pBodyNode;

    btTransform startTransform;
    startTransform.setIdentity();
    startTransform.setOrigin(btVector3(0, 0, 0));
    Vector3 origin(0, 0, 0);

    btPairCachingGhostObject * characterGhostObject = new btPairCachingGhostObject();
    characterGhostObject->setWorldTransform(startTransform);

    btScalar characterHeight = PERSON_HEIGHT;
	btScalar characterWidth = PERSON_RADIUS;

    btConvexShape * capsule = new btCapsuleShape(characterWidth, characterHeight);

    mCollisionShapes.push_back(capsule);
    characterGhostObject->setCollisionShape(capsule);
    characterGhostObject->setCollisionFlags(getCollisionGroup());

    // duck setup
    btConvexShape * duck = new btCapsuleShape(characterWidth, characterHeight / 3);
    mCollisionShapes.push_back(duck);

    btScalar stepHeight = 0.05f;

    btCylinderShape *pCylShape = new btCylinderShape(btVector3(characterWidth, 0.2, characterWidth) * 0.5);
    mCollisionShapes.push_back(pCylShape);

    btCompoundShape *pComShape = new btCompoundShape();
    mCollisionShapes.push_back(pComShape);
    pComShape->addChildShape(btTransform(btQuaternion::getIdentity(), btVector3(0, - characterHeight - 0.005, 0)), pCylShape);
    pComShape->addChildShape(btTransform::getIdentity(), capsule);


    //mCharacter = new CharacterControllerManager(mSceneManager, mCamera, characterGhostObject, capsule, stepHeight, CPhysicsManager::getSingleton().getCollisionWorld(), origin);
	CharacterControllerPhysics *pCC = new CharacterControllerPhysics(characterGhostObject, capsule, pCylShape, stepHeight, 1);
	mCCPhysics = pCC;
	pCC->setMaxSlope(0.5);
	pCC->setBorderDetectionShapeOffset(btVector3(0, -characterHeight - 0.02, 0));
    //mCCPhysics->setDuckingConvexShape(duck);

    //m_pCurrentMap->getPhysicsManager()->getWorld()->addCollisionObject(characterGhostObject, btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter);
	m_pCurrentMap->getPhysicsManager()->getWorld()->addCollisionObject(characterGhostObject, getCollisionGroup(), getCollisionMask());
    m_pCurrentMap->getPhysicsManager()->getWorld()->addAction(mCCPhysics);

    /// --

    m_pBodyPhysics = characterGhostObject;
    //m_pBodyPhysics->setCollisionShape(pComShape);

	m_pBodyPhysics->setWorldTransform(btTransform(btQuaternion::getIdentity(), btVector3(0, 4, 0)));
	//mCCPlayer->setIsMoving(true);

	setAsUserPointer(m_pBodyPhysics);

	pCC->start();
	pCC->setBorderDetectionShapeGroupAndMask(getCollisionGroup(), COL_WALL);
}
void CPerson::destroyPhysics() {

	if (mCCPhysics) {
		m_pCurrentMap->getPhysicsManager()->getWorld()->removeAction(mCCPhysics);
		delete mCCPhysics;
		mCCPhysics = NULL;
	}
	if (m_pBodyPhysics) {
		m_pCurrentMap->getPhysicsManager()->getWorld()->removeCollisionObject(m_pBodyPhysics);

		m_pCurrentMap->getPhysicsManager()->getBroadphase()->resetPool(m_pCurrentMap->getPhysicsManager()->getWorld()->getDispatcher());
		m_pCurrentMap->getPhysicsManager()->getBroadphase()->getOverlappingPairCache()->cleanProxyFromPairs(m_pBodyPhysics->getBroadphaseHandle(), m_pCurrentMap->getPhysicsManager()->getWorld()->getDispatcher());

		delete m_pBodyPhysics;
		m_pBodyPhysics = NULL;
	}

	for (int i = 0; i <mCollisionShapes.size(); i++) {
		delete mCollisionShapes[i];
	}
	mCollisionShapes.clear();
}
void CPerson::setupAnimations() {
	Ogre::StringVector animNames(ANIM_COUNT);
	animNames[ANIM_IDLE_BASE] = "IdleBase";
	animNames[ANIM_IDLE_TOP] = "IdleTop";
	animNames[ANIM_RUN_BASE] = "RunBase";
	animNames[ANIM_RUN_TOP] = "RunTop";
	animNames[ANIM_WALK_BASE] = "WalkBase";
	animNames[ANIM_WALK_TOP] = "WalkTop";
	animNames[ANIM_HANDS_CLOSED] = "HandsClosed";
	animNames[ANIM_HANDS_RELAXED] = "HandsRelaxed";
	animNames[ANIM_SLICE_VERTICAL] = "SliceVertical";
	animNames[ANIM_SLICE_HORIZONTAL] = "SliceHorizontal";
	animNames[ANIM_BOW_AIM_START] = "BowShotStart";
	animNames[ANIM_BOW_AIM_IDLE] = "BowShotIdle";
	animNames[ANIM_BOW_SHOT]= "BowShot";
	animNames[ANIM_DRAW_SWORDS] = "DrawSwords";
	animNames[ANIM_JUMP_START] = "JumpStart";
	animNames[ANIM_JUMP_LOOP] = "JumpLoop";
	animNames[ANIM_JUMP_END] = "JumpEnd";
	animNames[ANIM_PICK_HEAVY] = "PickHeavy";
	animNames[ANIM_LIFT_HEAVY] = "LiftHeavy";
	animNames[ANIM_WALK_HEAVY_TOP] = "WalkHeavyTop";
	animNames[ANIM_THROW_HEAVY] = "ThrowHeavy";
	animNames[ANIM_OPEN_DOOR] = "OpenDoor";
	animNames[ANIM_JUMP_INTO_WELL] = "JumpIntoWell";
	animNames[ANIM_SHIELD_BLOCK_START] = "ShieldBlockStart";
	animNames[ANIM_SHIELD_BLOCK_IDLE] = "ShieldBlockIdle";
	animNames[ANIM_BLOCK_SLICE_HORIZONTAL_BASE] = "BlockSliceHorizontalBase";
	animNames[ANIM_WALK_ON_LADDER] = "WalkOnLadder";
	animNames[ANIM_WALK_ON_LADDER_END] = "WalkOnLadderEnd";

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
	m_Anims[ANIM_OPEN_DOOR]->setLoop(false);
	m_Anims[ANIM_PICK_HEAVY]->setLoop(false);
	m_Anims[ANIM_LIFT_HEAVY]->setLoop(false);
	m_Anims[ANIM_JUMP_INTO_WELL]->setLoop(false);
	m_Anims[ANIM_THROW_HEAVY]->setLoop(false);


	// relax the hands since we're not holding anything
	m_Anims[ANIM_HANDS_RELAXED]->setEnabled(true);


    // start off in the idle state (top and bottom together)
    setBaseAnimation(ANIM_IDLE_BASE);
    setTopAnimation(ANIM_IDLE_TOP);

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
inline void CPerson::updateLeftHanldeRotation(Ogre::Real tpf) {
    if (m_degLeftHandleRotationToTarget == Ogre::Degree(0)) {
        return;
    }
    m_degLeftHandleCurrentRotation += m_degLeftHandleRotationSpeed * tpf;
    m_degLeftHandleRotationToTarget -= m_degLeftHandleRotationSpeed * tpf;

    // check if we reached the target
    if (m_degLeftHandleRotationSpeed > Ogre::Degree(0) && m_degLeftHandleRotationToTarget < Ogre::Degree(0)) {
        m_degLeftHandleCurrentRotation += m_degLeftHandleRotationToTarget;
        m_degLeftHandleRotationToTarget = Ogre::Degree(0);
    }
    else if (m_degLeftHandleRotationSpeed < Ogre::Degree(0) && m_degLeftHandleRotationToTarget > Ogre::Degree(0)) {
        m_degLeftHandleCurrentRotation += m_degLeftHandleRotationToTarget;
        m_degLeftHandleRotationToTarget = Ogre::Degree(0);
    }


    static const Ogre::Quaternion DEFAULT_ROTATION(Ogre::Degree(-90), Ogre::Vector3::UNIT_Z);

    Ogre::Bone *pLHandleBone = m_pBodyEntity->getSkeleton()->getBone(PERSON_LEFT_HANDLE);
    Ogre::Quaternion r(m_degLeftHandleCurrentRotation, Ogre::Vector3::UNIT_Y);
    pLHandleBone->setOrientation(DEFAULT_ROTATION * r);
}
void CPerson::initBody(Ogre::SceneNode *pParentSceneNode) {
	Ogre::String meshName = "LegoFig";
    // create main model
    m_pBodyNode = pParentSceneNode->createChildSceneNode(m_NameGenerator.generate() + meshName);
    Ogre::SceneNode *pModelSN = m_pBodyNode->createChildSceneNode();
    pModelSN->setScale(PERSON_SCALE, PERSON_SCALE, PERSON_SCALE);
    m_pBodyEntity = pParentSceneNode->getCreator()->createEntity(pModelSN->getName() + ".mesh", meshName + ".mesh");
    m_pBodyEntity->setCastShadows(true);
    pModelSN->attachObject(m_pBodyEntity);

    m_pBodyNode->setUserAny(Ogre::Any(dynamic_cast<CCharacter*>(this)));
    m_pBodyEntity->setUserAny(Ogre::Any(dynamic_cast<CCharacter*>(this)));

	createBlinkingMaterials();

	pModelSN->setPosition(0, -PERSON_HEIGHT, 0);
}

void CPerson::createHandObject(const Ogre::String &parentBone, EHands handPos, const Ogre::String &meshName) {
    m_pHandObjects[handPos] = m_pBodyNode->getCreator()->createEntity(meshName);
    m_pBodyEntity->attachObjectToBone(parentBone, m_pHandObjects[handPos]);
}
void CPerson::createTool(unsigned int uiTool, bool drawn) {
    createHandObject((drawn) ? PERSON_LEFT_HANDLE : PERSON_SHEATH, LEFT_HAND, CPlayerTool::MESH_FILES[uiTool]);
    if (drawn) {
        m_uiNextTool = uiTool;
        m_uiCurrentTool = uiTool;
        m_bSwordsDrawn = true;
    }
}
void CPerson::createShield(CShield::EShieldTypes eShieldType, bool drawn) {
	assert(!m_pShield);
    m_pShield = new CShield(this, (drawn) ? PERSON_RIGHT_HANDLE : PERSON_SHIELD_PACKED, m_pBodyNode->getCreator(), eShieldType);
}
void CPerson::animGrabWeapons(unsigned int uiNextTool) {
    if (m_uiTopAnimID == ANIM_DRAW_SWORDS) {return;}
    m_uiNextTool = uiNextTool;
    if (m_bSwordsDrawn) {
        if (uiNextTool == m_uiCurrentTool) {
            m_bChangeWeapons = false;
        }
        else {
            m_bChangeWeapons = true;
        }
        // put weapon away
        if (topReadyForNewAction()) {
            setTopAnimation(ANIM_DRAW_SWORDS, true);
            m_fTimer = 0;
        }
       // m_uiNextTool = CPlayerTool::TOOL_BLANK; // weapon put away is blank
    }
    else {
        if (m_uiNextTool != CPlayerTool::TOOL_BLANK) {
            // draw the weapon/tool
            if (topReadyForNewAction()) {
                setTopAnimation(ANIM_DRAW_SWORDS, true);
                m_fTimer = 0;
                m_bChangeWeapons = false;
            }
        }
    }
}
void CPerson::animChangeWeapons(unsigned int uiNextTool) {
    /*if (m_bAiming || m_bShieldBlockActive) {return;}
    m_uiNextTool = uiNextTool;
    if (m_bSwordsDrawn) {
        if (m_uiNextTool == CPlayerTool::TOOL_BLANK) {
            // put the weapon away
            animGrabWeapons(CPlayerTool::TOOL_BLANK);
        }
        else {
            // change the weapons
            if (topReadyForNewAction()) {
                setTopAnimation(ANIM_DRAW_SWORDS, true);
                m_fTimer = 0;
                m_bChangeWeapons = true;
            }
        }
    }
    else {
        if (m_uiNextTool != CPlayerTool::TOOL_BLANK) {
            // draw weapon
            animGrabWeapons(uiNextTool);
        }
    }*/
}
void CPerson::animAttack() {
	if (m_pLiftedObject) {
		if (m_uiTopAnimID == ANIM_WALK_HEAVY_TOP) {
			animThrowHeavy();
		}
		return;
	}
    if (m_bSwordsDrawn && topReadyForNewAction()) {
        // attack anim
		if (m_uiCurrentTool == CPlayerTool::TOOL_BOW) {
            if (m_uiBaseAnimID == ANIM_BOW_AIM_IDLE) {
                setTopAnimation(ANIM_NONE, true);
                setBaseAnimation(ANIM_BOW_SHOT, true);
                attack(CPlayerTool::TOOL_BOW);
                m_fTimer = 0;
            }
		}
		else if (m_uiCurrentTool == CPlayerTool::TOOL_SWORD || m_uiCurrentTool == CPlayerTool::TOOL_LAMP) {
		    setTopAnimation(ANIM_SLICE_HORIZONTAL, true);
            if (m_bShieldBlockActive) {
                setBaseAnimation(ANIM_BLOCK_SLICE_HORIZONTAL_BASE, true);
            }
            m_fTimer = 0;
		}
		else if (m_uiCurrentTool == CPlayerTool::TOOL_BOMB) {
            attack(m_uiCurrentTool);
		}
    }
	else if (!m_bSwordsDrawn) {
        // interact: e.g. throw object, etc.
		interact(CObject::IT_ATTACK);
	}
}
void CPerson::animAttackEnd() {
    switch (m_uiCurrentTool) {
    case CPlayerTool::TOOL_LAMP:
    case CPlayerTool::TOOL_SWORD:
        setTopAnimation(ANIM_NONE);
        break;
    case CPlayerTool::TOOL_BOW:
        setBaseAnimation(ANIM_BOW_AIM_IDLE, true);
        break;
    }
}
void CPerson::animStartBlock() {
    if (m_bShieldBlockActive || m_bAiming || m_uiBaseAnimID == ANIM_SHIELD_BLOCK_START) {return;}
    if (m_bSwordsDrawn && topReadyForNewAction()) {
        switch (m_uiCurrentTool) {
        case CPlayerTool::TOOL_SWORD:
            m_bShieldBlockActive = true;
            setTopAnimation(ANIM_NONE);
            setBaseAnimation(ANIM_SHIELD_BLOCK_START, true);
            m_fTimer = 0;
            break;
        case CPlayerTool::TOOL_BOW:
            m_bAiming = true;
            setTopAnimation(ANIM_NONE);
            setBaseAnimation(ANIM_BOW_AIM_START, true);
            m_fTimer = 0;
            setLeftHandleRotation(Ogre::Degree(155));
            break;
        }
    }
}
void CPerson::animEndBlock() {
    if (m_bShieldBlockActive) {
        m_bShieldBlockActive = false;
        if (m_uiBaseAnimID != ANIM_SHIELD_BLOCK_START) {
            setBaseAnimation(ANIM_SHIELD_BLOCK_START, true);
            setTopAnimation(ANIM_NONE);
            m_fTimer = 0;

        }
        else {
            m_fTimer = m_Anims[ANIM_SHIELD_BLOCK_START]->getLength() - m_fTimer;
        }
    }
    else if (m_bAiming) {
        m_bAiming = false;
        if (m_uiBaseAnimID != ANIM_BOW_AIM_START) {
            setBaseAnimation(ANIM_BOW_AIM_START, true);
            setTopAnimation(ANIM_NONE);
            m_fTimer = 0;

        }
        else {
            m_fTimer = m_Anims[ANIM_BOW_AIM_START]->getLength() - m_fTimer;
        }
        setLeftHandleRotation(Ogre::Degree(113));
        dynamic_cast<CBow*>(m_pCurrentTool)->releaseString();
    }
}
void CPerson::postUpdateAnimationsCallback(const Ogre::Real fTime) {
	if (m_uiBaseAnimID == ANIM_WALK_ON_LADDER) {
		//m_Anims[ANIM_WALK_ON_LADDER]->setTimePosition(m_Anims[ANIM_WALK_ON_LADDER]->getLength() * (m_fLadderPos * m_pLadder->getLength() - 0.8) / 0.266667 * 0.5);
	}

	if (m_pShield) {
		if (m_bSwordsDrawn) {
			m_pShield->updateShieldPhysics(
				m_pBodyEntity->getParentNode()->convertLocalToWorldPosition(m_pBodyEntity->getSkeleton()->getBone(PERSON_RIGHT_HANDLE)->_getDerivedPosition()),
				m_pBodyEntity->getParentNode()->convertLocalToWorldOrientation(m_pBodyEntity->getSkeleton()->getBone(PERSON_RIGHT_HANDLE)->_getDerivedOrientation()));
		}
		else {
			m_pShield->updateShieldPhysics(
				m_pBodyEntity->getParentNode()->convertLocalToWorldPosition(m_pBodyEntity->getSkeleton()->getBone(PERSON_SHIELD_PACKED)->_getDerivedPosition()),
				m_pBodyEntity->getParentNode()->convertLocalToWorldOrientation(m_pBodyEntity->getSkeleton()->getBone(PERSON_SHIELD_PACKED)->_getDerivedOrientation()));
		}
	}


    // update tools animation, if needed
    if (m_pCurrentTool) {
        m_pCurrentTool->update(fTime);
    }
    if (m_pCurrentProjectile) {
        const Ogre::Vector3 vDir = -m_pBodyEntity->getParentNode()->convertLocalToWorldOrientation(m_pBodyEntity->getSkeleton()->getBone(PERSON_RIGHT_HANDLE)->_getDerivedOrientation()).xAxis();
        const Ogre::Vector3 vPos = m_pBodyEntity->getParentNode()->convertLocalToWorldPosition(m_pBodyEntity->getSkeleton()->getBone(PERSON_RIGHT_HANDLE)->_getDerivedPosition());
        m_pCurrentProjectile->updateAttachedProjectile(
            vPos + vDir * 0.3,
            vDir
        );
    }
    updateLeftHanldeRotation(fTime);
}
void CPerson::updateAnimationsCallback(const Ogre::Real fTime) {
	 if (m_uiTopAnimID == ANIM_SLICE_HORIZONTAL || m_uiTopAnimID == ANIM_SLICE_VERTICAL) {
		if (m_Anims[m_uiTopAnimID]->getLength() < m_fTimer) {
			animAttackEnd();

			if (m_bShieldBlockActive) {
				setBaseAnimation(ANIM_SHIELD_BLOCK_IDLE);
			}
		}
		else {
            if (m_uiCurrentTool == CPlayerTool::TOOL_SWORD) {
                // check time
                if (m_Anims[m_uiTopAnimID]->getLength() * 0.45 < m_fTimer && m_Anims[m_uiTopAnimID]->getLength() * 0.80 > m_fTimer) {
                    attack(m_uiCurrentTool);
                }
            }
            else if (m_uiCurrentTool == CPlayerTool::TOOL_LAMP) {
                // check time
                if (m_Anims[m_uiTopAnimID]->getLength() * 0.45f < m_fTimer && m_Anims[m_uiTopAnimID]->getLength() * 0.80f + fTime > m_fTimer) {
                    attack(m_uiCurrentTool);
                }
            }
        }
    }
    else if (m_uiBaseAnimID == ANIM_BOW_SHOT) {
        if (m_Anims[m_uiBaseAnimID]->getLength() < m_fTimer) {
            animAttackEnd();
		}
		else {
            // check for thime position of the animation
            if (m_fTimer >= m_Anims[m_uiBaseAnimID]->getLength() * 0.2f &&
                m_fTimer - fTime < m_Anims[m_uiBaseAnimID]->getLength() * 0.2f) {
                if (hasMoreArrows()) {
                    m_pCurrentProjectile = new CProjectile(m_pCurrentMap->getObjectManager(), m_pCurrentMap->getRootSceneNode(), CDamage::DMG_ARROW);
                }
            }
            else if (m_fTimer >= m_Anims[m_uiBaseAnimID]->getLength() * 0.5f &&
                m_fTimer - fTime < m_Anims[m_uiBaseAnimID]->getLength() * 0.5f) {
                dynamic_cast<CBow*>(m_pCurrentTool)->pullString();
            }
        }
    }
    if (m_uiTopAnimID == ANIM_DRAW_SWORDS) {
        // flip the draw swords animation if we need to put it back
		m_fTopAnimSpeed = (m_uiNextTool != m_uiCurrentTool) ? -1.0f : 1.0f;

		//Ogre::LogManager::getSingleton().logMessage("Timer: " + Ogre::StringConverter::toString(m_fTimer) + ", dT: " + Ogre::StringConverter::toString(fTime) + ". haltAnim: " + Ogre::StringConverter::toString(m_Anims[ANIM_DRAW_SWORDS]->getLength() / 2 ));

		// half-way through the animation is when the hand grasps the handles...
		if (m_fTimer >= m_Anims[ANIM_DRAW_SWORDS]->getLength() / 2.0 &&
			m_fTimer - fTime < m_Anims[ANIM_DRAW_SWORDS]->getLength() / 2.0)
		{
            if (m_bChangeWeapons) {
                detachCurrentTool();
                attachNewTool();
                m_bSwordsDrawn = true;
            }
            else {
                if (m_bSwordsDrawn) {
                    detachCurrentTool();
                    m_uiCurrentTool = 0;
                }
                else {
                    attachNewTool();
                }
                m_bSwordsDrawn = !m_bSwordsDrawn;
            }
			// change the hand state to grab or let go
			/*m_Anims[ANIM_HANDS_CLOSED]->setEnabled(!mSwordsDrawn);
			m_Anims[ANIM_HANDS_RELAXED]->setEnabled(mSwordsDrawn);*/

			// toggle sword trails
			/*if (mSwordsDrawn)
			{
				mSwordTrail->setVisible(false);
				mSwordTrail->removeNode(mSword1->getParentNode());
				mSwordTrail->removeNode(mSword2->getParentNode());
			}
			else
			{
				mSwordTrail->setVisible(true);
				mSwordTrail->addNode(mSword1->getParentNode());
				mSwordTrail->addNode(mSword2->getParentNode());
			}*/
		}
        else if (m_fTimer >= m_Anims[ANIM_DRAW_SWORDS]->getLength()) {
            setTopAnimation(ANIM_NONE);
            m_fTimer = 0;
        }
    }
    if (m_uiBaseAnimID == ANIM_SHIELD_BLOCK_START) {
        if (m_fTimer >= m_Anims[m_uiBaseAnimID]->getLength()) {
            setTopAnimation(ANIM_NONE);
            if (m_bShieldBlockActive) {
                setBaseAnimation(ANIM_SHIELD_BLOCK_IDLE, true);
            }
            else {
                setBaseAnimation(ANIM_NONE);
            }
            m_fTimer = 0;
        }
        m_fBaseAnimSpeed = (m_bShieldBlockActive) ? +1.0f : -1.0f;
    }
    else if (m_uiBaseAnimID == ANIM_BOW_AIM_START) {
        if (m_fTimer >= m_Anims[m_uiBaseAnimID]->getLength()) {
            setTopAnimation(ANIM_NONE);
            if (m_bAiming) {
                setBaseAnimation(ANIM_BOW_AIM_IDLE, true);
            }
            else {
                setBaseAnimation(ANIM_NONE);
            }
            m_fTimer = 0;
        }
        else {
            if (m_bAiming) {
                if (m_fTimer >= m_Anims[m_uiBaseAnimID]->getLength() * 0.2f &&
                    m_fTimer - fTime < m_Anims[m_uiBaseAnimID]->getLength() * 0.2f) {
                    if (hasMoreArrows()) {
                        m_pCurrentProjectile = new CProjectile(m_pCurrentMap->getObjectManager(), m_pCurrentMap->getRootSceneNode(), CDamage::DMG_ARROW);
                    }
                }
                else if (m_fTimer >= m_Anims[m_uiBaseAnimID]->getLength() * 0.6f &&
                    m_fTimer - fTime < m_Anims[m_uiBaseAnimID]->getLength() * 0.6f) {
                    dynamic_cast<CBow*>(m_pCurrentTool)->pullString();
                }
            }
            else {
                if (m_fTimer >= m_Anims[m_uiBaseAnimID]->getLength() * 0.8f &&
                    m_fTimer - fTime < m_Anims[m_uiBaseAnimID]->getLength() * 0.8f) {
                    if (m_pCurrentProjectile) {
                        m_pCurrentProjectile->suicide();
                        m_pCurrentProjectile = NULL;
                    }
                }
            }
        }
        m_fBaseAnimSpeed = (m_bAiming) ? +1.0f : -1.0f;
    }
	else if (m_uiBaseAnimID == ANIM_PICK_HEAVY) {
		if (m_fTimer >= m_Anims[m_uiBaseAnimID]->getLength()) {
			CObject *pObject = dynamic_cast<CObject*>(static_cast<CPhysicsUserPointer*>(m_pLiftedObject->getUserPointer()));
			if (pObject->interactOnKeypress(CObject::IT_LIFT)) {
				animStartLiftObject();
				m_pLiftedObject = btRigidBody::upcast(pObject->getCollisionObject());
			}
		}
	}
	else if (m_uiBaseAnimID == ANIM_LIFT_HEAVY) {
		if (m_fTimer >= m_Anims[m_uiBaseAnimID]->getLength()) {
			setTopAnimation(ANIM_WALK_HEAVY_TOP);
			setBaseAnimation(ANIM_NONE);
		}
	}
	else if (m_uiBaseAnimID == ANIM_THROW_HEAVY) {
		if (m_fTimer >= m_Anims[m_uiBaseAnimID]->getLength()) {
			setTopAnimation(ANIM_IDLE_TOP, true);
			setBaseAnimation(ANIM_IDLE_BASE, true);
		}
		else if (m_pLiftedObject && m_fTimer>= m_Anims[m_uiBaseAnimID]->getLength() * 0.4) {

			m_pLiftedObject->setLinearFactor(btVector3(1, 1, 1));
			m_pLiftedObject->setAngularFactor(1);
			m_pLiftedObject->setLinearVelocity(BtOgre::Convert::toBullet(m_pBodyNode->_getDerivedOrientation() * Ogre::Vector3(0, 3, 5)));

			dynamic_cast<CObject*>(static_cast<CPhysicsUserPointer*>(m_pLiftedObject->getUserPointer()))->interactOnKeypress(CObject::IT_ATTACK);
			m_pLiftedObject = NULL;
		}
	}

	if (m_uiTopAnimID == ANIM_IDLE_TOP && m_bShieldBlockActive) {
		m_bShieldBlockActive = false;
		animStartBlock();
	}

    CCharacter::updateAnimationsCallback(fTime);



	// set the position of the lifted object
	if (m_pLiftedObject && m_uiBaseAnimID != ANIM_PICK_HEAVY) {
		Ogre::Vector3 handleL(m_pBodyEntity->getParentNode()->convertLocalToWorldPosition(m_pBodyEntity->getSkeleton()->getBone(PERSON_LEFT_HANDLE)->_getDerivedPosition()));
		Ogre::Vector3 handleR(m_pBodyEntity->getParentNode()->convertLocalToWorldPosition(m_pBodyEntity->getSkeleton()->getBone(PERSON_RIGHT_HANDLE)->_getDerivedPosition()));
		Ogre::Vector3 pos = 0.50f * (handleL + handleR) + 0.20f * Ogre::Vector3::UNIT_Y;
		m_pLiftedObject->getWorldTransform().setOrigin(BtOgre::Convert::toBullet(pos));
		m_pLiftedObject->getWorldTransform().setRotation(BtOgre::Convert::toBullet(m_pBodyNode->_getDerivedOrientation()));
		m_pLiftedObject->activate(true);
		m_pLiftedObject->setLinearFactor(btVector3(0, 0, 0));
		m_pLiftedObject->setAngularFactor(0);
	}
}
void CPerson::damageBlocked(const CDamage &damage) {
	dynamic_cast<CPersonController*>(m_pCharacterController)->changeMoveState(CPersonController::MS_PUSHED_BACK);
}
void CPerson::createBlinkingMaterials() {
	assert(m_pBodyEntity);

	for (unsigned int i = 0; i < m_pBodyEntity->getNumSubEntities(); i++) {
		auto pSubEnt = m_pBodyEntity->getSubEntity(i);
		CBlinkingMaterialManager::getSingleton().createMaterial(pSubEnt->getMaterialName());
	}
}
void CPerson::removeBlinkingMaterials() {
	assert(m_pBodyEntity);

	for (unsigned int i = 0; i < m_pBodyEntity->getNumSubEntities(); i++) {
		auto pSubEnt = m_pBodyEntity->getSubEntity(i);
		CBlinkingMaterialManager::getSingleton().removeMaterial(pSubEnt->getMaterialName());
	}
}
void CPerson::startBeingInvulnerableCallback() {
	for (unsigned int i = 0; i < m_pBodyEntity->getNumSubEntities(); i++) {
		m_pBodyEntity->getSubEntity(i)->setMaterialName(CBlinkingMaterialManager::getSingleton().getBlinkingMat(m_pBodyEntity->getSubEntity(i)->getMaterialName()));
	}
}
void CPerson::endBeingInvulnerableCallback() {
	for (unsigned int i = 0; i < m_pBodyEntity->getNumSubEntities(); i++) {
		m_pBodyEntity->getSubEntity(i)->setMaterialName(CBlinkingMaterialManager::getSingleton().getNonBlinkingMat(m_pBodyEntity->getSubEntity(i)->getMaterialName()));
	}
}
CPerson::EReceiveDamageResult CPerson::receiveDamage(const CDamage &damage) {
	if (!isInvulnerable() && damage.getDamageType() & m_uiTakeDamageFlags) {
		changeHP(-damage.getDamageValue());
		dynamic_cast<CPersonController*>(m_pCharacterController)->changeMoveState(CPersonController::MS_PUSHED_BACK, damage.getDamageDirection(), 0.25, damage.getKnockOutTime());
		makeInvulnerable();
        return RDR_ACCEPTED;
	}
	else if (damage.getDamageType() & m_uiBlockDamageFlags) {
        return RDR_BLOCKED;
	}

	return RDR_REJECTED;
}
void CPerson::animStartLiftObject() {
	setTopAnimation(ANIM_NONE);
	setBaseAnimation(ANIM_LIFT_HEAVY, true);
    m_fTimer = 0;
}
void CPerson::animStartPickObject() {
	setTopAnimation(ANIM_NONE);
	setBaseAnimation(ANIM_PICK_HEAVY, true);
    m_fTimer = 0;
}
void CPerson::animThrowHeavy() {
	setTopAnimation(ANIM_NONE);
	setBaseAnimation(ANIM_THROW_HEAVY, true);
    m_fTimer = 0;
}
void CPerson::changeTool(const CPlayerTool &tool) {
    m_uiNextTool = tool.getToolType();
    if (tool.getToolType() == m_uiCurrentTool) {
        // tool did not change, cancel
        return;
    }
    animChangeWeapons(tool.getToolType());
}
void CPerson::detachCurrentTool() {
    switch (m_uiCurrentTool) {
    case CPlayerTool::TOOL_SWORD:
        // add sword and shield to rear
        m_pBodyEntity->detachObjectFromBone(m_pHandObjects[LEFT_HAND]);
        m_pBodyEntity->detachObjectFromBone(m_pHandObjects[RIGHT_HAND]);
        m_pBodyEntity->attachObjectToBone(PERSON_SHEATH, m_pHandObjects[LEFT_HAND]);
        m_pBodyEntity->attachObjectToBone(PERSON_SHIELD_PACKED, m_pHandObjects[RIGHT_HAND]);
        break;
    case CPlayerTool::TOOL_LAMP:
    case CPlayerTool::TOOL_BOW:
    case CPlayerTool::TOOL_HAMMER:
        assert(m_pCurrentTool);
        m_pBodyEntity->detachObjectFromBone(m_pCurrentTool->getEntity());
        delete m_pCurrentTool;
        m_pCurrentTool = NULL;
        break;
    }
}
void CPerson::attachNewTool() {
    assert(m_pCurrentTool == NULL);

    switch (m_uiNextTool) {
    case CPlayerTool::TOOL_SWORD:
        // attach sword and shield to hands
        m_pBodyEntity->detachObjectFromBone(m_pHandObjects[LEFT_HAND]);
        m_pBodyEntity->detachObjectFromBone(m_pHandObjects[RIGHT_HAND]);
        m_pBodyEntity->attachObjectToBone(PERSON_LEFT_HANDLE, m_pHandObjects[LEFT_HAND]);
        m_pBodyEntity->attachObjectToBone(PERSON_RIGHT_HANDLE, m_pHandObjects[RIGHT_HAND]);
        setLeftHandleRotation(Ogre::Degree(0));
        break;
    case CPlayerTool::TOOL_BOW:
        m_pCurrentTool = new CBow(m_pCurrentMap->getSceneManager());
        m_pBodyEntity->attachObjectToBone(PERSON_LEFT_HANDLE, m_pCurrentTool->getEntity());
        setLeftHandleRotation(Ogre::Degree(113));
        break;
    case CPlayerTool::TOOL_LAMP:
        m_pCurrentTool = new Lamp(m_pCurrentMap->getSceneManager(), m_pCurrentMap->getRootSceneNode(), m_pCurrentMap->getPhysicsManager());
        m_pBodyEntity->attachObjectToBone(PERSON_LEFT_HANDLE, m_pCurrentTool->getEntity());
        setLeftHandleRotation(Ogre::Degree(0));
        break;
    case CPlayerTool::TOOL_HAMMER:
        m_pCurrentTool = new CTool(CPlayerTool::TOOL_HAMMER, m_pCurrentMap->getSceneManager());
        m_pBodyEntity->attachObjectToBone(PERSON_LEFT_HANDLE, m_pCurrentTool->getEntity());
        setLeftHandleRotation(Ogre::Degree(0));
        break;

    }
    if (m_pCurrentTool) {
        m_pCurrentTool->start();
    }

    m_uiCurrentTool = m_uiNextTool;
}
void CPerson::attack(unsigned int uiTool) {
    if (uiTool == CPlayerTool::TOOL_SWORD || uiTool == CPlayerTool::TOOL_HAMMER || uiTool == CPlayerTool::TOOL_LAMP) {
        Ogre::Vector3 direction(m_pBodyEntity->getParentNode()->convertLocalToWorldOrientation(m_pBodyEntity->getSkeleton()->getBone("Handle.L")->_getDerivedOrientation()) * Ogre::Vector3::UNIT_Y);
        Ogre::Vector3 startPos(m_pBodyEntity->getParentNode()->convertLocalToWorldPosition(m_pBodyEntity->getSkeleton()->getBone("Handle.L")->_getDerivedPosition()));
        Ogre::Vector3 endPos(startPos + direction * 0.4f); // length of the sword

        if (uiTool == CPlayerTool::TOOL_LAMP) {
            // adjust position
            Ogre::Vector3 vOffsetForLamp(0, -0.2, 0);
            startPos += vOffsetForLamp;
            endPos += vOffsetForLamp;
        }
        else {
            startPos -= direction * 0.2f; // move start point a bit back
        }
        // try to interact with the world. So detect an object to interact with
        btCollisionWorld::ClosestRayResultCallback rayCallback(BtOgre::Convert::toBullet(startPos), BtOgre::Convert::toBullet(endPos));

        if (m_eFriendOrEnemy == FOE_FRIENDLY) {
            rayCallback.m_collisionFilterGroup = COL_DAMAGE_P;
            rayCallback.m_collisionFilterMask = MASK_DAMAGE_P_COLLIDES_WITH;
        }
        else {
            rayCallback.m_collisionFilterGroup = COL_DAMAGE_N;
            rayCallback.m_collisionFilterMask = MASK_DAMAGE_N_COLLIDES_WITH;
        }
        m_pCurrentMap->getPhysicsManager()->getWorld()->rayTest(BtOgre::Convert::toBullet(startPos), BtOgre::Convert::toBullet(endPos), rayCallback);
        DebugDrawer::getSingleton().drawLine(startPos, endPos, Ogre::ColourValue::Red);
        if (rayCallback.hasHit()) {
            CDamage damage(CPlayerTool::DMG_TYPE[uiTool], m_pBodyNode->getOrientation().zAxis(), (m_eFriendOrEnemy == FOE_FRIENDLY) ? 0.5f : 0.0f);
            if (rayCallback.m_collisionObject->getUserPointer()) {
                CHitableInterface *pHI = dynamic_cast<CHitableInterface*>(static_cast<CPhysicsUserPointer*>(rayCallback.m_collisionObject->getUserPointer()));
                if (pHI) {
                    auto res = pHI->receiveDamage(damage);
                    switch (res) {
                    case CHitableInterface::RDR_BLOCKED:
                        damageBlocked(damage);
                        break;
                    }
                }
            }
            else {
                damageBlocked(damage);
            }
        }
    }
    else if (uiTool == CPlayerTool::TOOL_BOW) { // create an arrow
        // create flying arrow
        if (m_pCurrentProjectile) {
            removeArrow(); // remove an arrow
            m_pCurrentProjectile->launchProjectile();
            dynamic_cast<CBow*>(m_pCurrentTool)->releaseShot();
            m_pCurrentProjectile = NULL;
        }
    }
    else if (uiTool == CPlayerTool::TOOL_BOMB) {
        // create bomb
        CBomb *pBomb = new CBomb(m_pCurrentMap->getRootSceneNode(), m_pCurrentMap->getObjectManager(), getPosition());
        pBomb->setListener(this);
    }
}
void CPerson::bombDestroyed(CBomb *pBomb) {
    if (m_pLiftedObject && pBomb->getCollisionObject() == m_pLiftedObject) {
        m_pLiftedObject = NULL;
        setTopAnimation(ANIM_IDLE_TOP, true);
        setBaseAnimation(ANIM_IDLE_BASE, true);
    }
}
void CPerson::climbLadder(Ladder *pLadder, const Ogre::Real fPosOnLadder) {
    dynamic_cast<CPersonController*>(getCharacterController())->climbLadder(pLadder, fPosOnLadder);
}
