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

#ifndef _PERSON_H_
#define _PERSON_H_

#include "Character.h"
#include "Shield.h"
#include "Object.h"
#include "BombListener.h"


class Ladder;
class CTool;
class CPersonController;
class CPlayerTool;
class CProjectile;

//! Class for a general person
/** It will define physics, animations, based on the same skeleton/mesh
  */
class CPerson : public CCharacter, public CBombListener {
public:
	static const Ogre::Real PERSON_HEIGHT;
	static const Ogre::Real PERSON_RADIUS;
	static const Ogre::Real PERSON_SCALE;
protected:
	static const Ogre::String PERSON_LEFT_HANDLE;
	static const Ogre::String PERSON_RIGHT_HANDLE;
	static const Ogre::String PERSON_SHEATH;
	static const Ogre::String PERSON_SHIELD_PACKED;

	enum EHands {LEFT_HAND, RIGHT_HAND, NUM_HANDS};
private:
    Ogre::Degree m_degLeftHandleCurrentRotation;
    Ogre::Degree m_degLeftHandleRotationToTarget;
    Ogre::Degree m_degLeftHandleRotationSpeed;
protected:
	bool m_bShieldBlockActive;
	bool m_bAiming;                                 //!< aiming fot a target e.g. with the bow
	bool m_bSwordsDrawn;

	btRigidBody *m_pLiftedObject;					//!< Not null if player lifts a object

	Ogre::Entity *m_pHandObjects[NUM_HANDS];		//!< Objecte die der Charakter in der Hand halten kann
	CShield *m_pShield;								//!< Ein eventuelles SChild des Charakters zum Blocken von angriffen

	unsigned int m_uiCurrentTool;                   //!< id of the current tool
	unsigned int m_uiNextTool;                      //!< id of the next tool the user wants to select
	CTool *m_pCurrentTool;                          //!< the current tool of the player (not sword!)
	CProjectile *m_pCurrentProjectile;              //!< current attached porjectile (arrow in hand)
	bool m_bChangeWeapons;                          //!< for draw swords animation, to know whether to draw/put or change

	unsigned int m_uiTakeDamageFlags;               //!< from which dmg types the person can take damage (default all)
	unsigned int m_uiBlockDamageFlags;              //!< which dmg type will be blocked (default all)

public:
	CPerson(CMap *pMap, const EFriendOrEnemyStates foe);
	virtual ~CPerson();

	void animGrabWeapons(unsigned int uiNextTool);
	void animChangeWeapons(unsigned int uiNextTool);
    void animAttack();
    void animAttackEnd();
    void animStartBlock();
    void animEndBlock();
	void animThrowHeavy();
	void animStartLiftObject();
	void animStartPickObject();

	virtual void destroy();
	virtual void interact(CObject::EInteractType eInteractType) {}

	void changeTool(const CPlayerTool &tool);

	virtual void bombDestroyed(CBomb *pBomb);

	EReceiveDamageResult receiveDamage(const CDamage &damage);

    //! This function willmove toplayer on the given ladder
    /** This function will be called out of the ladder object, when an person interacts on collision
      * It will call the samed labled function of the Person Controller
      * @param pLadder The ladder to climb
      * @param fPosOnLadder the initial position on the ladder
      */
	void climbLadder(Ladder *pLadder, const Ogre::Real fPosOnLadder);
protected:
	void createPhysics();
	void destroyPhysics();
	virtual void setupAnimations();
	virtual void initBody(Ogre::SceneNode *pParentSceneNode);
	void createTool(unsigned int uiTool, bool drawn);
	void createShield(CShield::EShieldTypes eShieldType, bool drawn);

    void attack(unsigned int uiTool);

	void updateAnimationsCallback(const Ogre::Real fTime);
	void postUpdateAnimationsCallback(const Ogre::Real fTime);

	void setLeftHandleRotation(const Ogre::Degree &degree, Ogre::Real speed = 0);
	inline void updateLeftHanldeRotation(Ogre::Real tpf);

	virtual void startBeingInvulnerableCallback();
	virtual void endBeingInvulnerableCallback();

	virtual CCharacterController *createCharacterController();
	virtual void damageBlocked(const CDamage &damage);

	void createBlinkingMaterials();
	void removeBlinkingMaterials();


    void detachCurrentTool();
    void attachNewTool();

    // functions to test if its allowed to attack (implemented by player ofc)
    virtual bool hasMoreArrows() {return true;}
    virtual void removeArrow() {}
    virtual bool hasMoreBombs() {return true;}
    virtual void removeBomb() {}
    //bool hasEnoughMana();

	friend class CShield;
private:
	void createHandObject(const Ogre::String &parentBone, EHands handPos, const Ogre::String &meshName);
};

#endif
