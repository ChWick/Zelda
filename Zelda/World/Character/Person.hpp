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

#include "Character.hpp"

//! Class for a general person
/** It will define physics, animations, based on the same skeleton/mesh
  */
class CPerson : public CCharacter {
public:
	static const Ogre::Real PERSON_HEIGHT;
	static const Ogre::Real PERSON_RADIUS;
	static const Ogre::Real PERSON_SCALE;
  static const Ogre::Real PERSON_PHYSICS_OFFSET;
  static const Ogre::Real PERSON_FLOOR_OFFSET;
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
	unsigned int m_uiTakeDamageFlags;               //!< from which dmg types the person can take damage (default all)
	unsigned int m_uiBlockDamageFlags;              //!< which dmg type will be blocked (default all)
public:
	CPerson(const std::string &sID, CEntity *pParent, const EFriendOrEnemyStates foe);
	virtual ~CPerson();

	virtual void destroy();

  virtual bool frameEnded(const Ogre::FrameEvent& evt);


  virtual bool collidesWith(const std::string &sEntityID) const;
  virtual void interact() {}

protected:
	void createPhysics();
	void destroyPhysics();
	virtual void setupAnimations();
	virtual void initBody(Ogre::SceneNode *pParentSceneNode);

  void attack(unsigned int uiTool);

	void updateAnimationsCallback(const Ogre::Real fTime);
	void postUpdateAnimationsCallback(const Ogre::Real fTime);

	void setLeftHandleRotation(const Ogre::Degree &degree, Ogre::Real speed = 0);

	virtual void startBeingInvulnerableCallback();
	virtual void endBeingInvulnerableCallback();

	virtual CCharacterController *createCharacterController();

	void createBlinkingMaterials();
	void removeBlinkingMaterials();

	friend class CShield;
private:
	void createHandObject(const Ogre::String &parentBone, EHands handPos, const Ogre::String &meshName);
};

#endif
