#ifndef _PERSON_H_
#define _PERSON_H_

#include "Character.h"

//! Class for a general person
/** It will define physics, animations, based on the same skeleton/mesh
  */
class CPerson : public CCharacter {
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
	unsigned int m_uiTakeDamageFlags;               //!< from which dmg types the person can take damage (default all)
	unsigned int m_uiBlockDamageFlags;              //!< which dmg type will be blocked (default all)
public:
	CPerson(const std::string &sID, CEntity *pParent, const EFriendOrEnemyStates foe);
	virtual ~CPerson();

	virtual void destroy();


  virtual bool collidesWith(const std::string &sEntityID) const;

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
