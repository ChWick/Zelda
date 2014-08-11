#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include "../WorldEntity.hpp"
#include <LinearMath/btAlignedObjectArray.h>

class btRigidBody;
class CMap;
class CCharacterController;

class btCollisionShape;
class btCharacterControllerInterface;

#define ANIM_FADE_SPEED 7.5f   // animation crossfade speed in % of full weight per second

//! Klasse f&uuml;r einen animierten Charater mit Standardanimationen
/*!
Elternklassen f&uur: Charakter kann zerst&ouml;rt werden, Charakter kann Schaden verursachen.
*/
class CCharacter : public CWorldEntity {
public:
	enum EDefaultAnimationFlags {
		ANIM_FLAG_NONE			= 0,
		ANIM_FLAG_HAS_IDLE		= 1,
		ANIM_FLAG_HAS_RUN		= 2,
		ANIM_FLAG_HAS_WALK		= 4,
		ANIM_FLAG_HAS_HANDS		= 8,
		ANIM_FLAG_HAS_SLICE_HV	= 16,

		ANIM_FLAG_ALL			= 0xFFFFFF
	};
	//! Animationen die man als Charakter haben kann
	enum EDefaultAnimationIDs {
		ANIM_IDLE,
		ANIM_RUN,
		ANIM_WALK,
		ANIM_HANDS_CLOSED,
		ANIM_HANDS_RELAXED,
		ANIM_SLICE_HORIZONTAL,
		ANIM_BOW_SHOT,
		ANIM_JUMP_START,
		ANIM_JUMP_LOOP,
		ANIM_JUMP_END,
		ANIM_LEFT,
		ANIM_WALK_HEAVY,
		ANIM_RUN_HEAVY,
		ANIM_FALL,
		ANIM_WALK_ON_LADDER,
		ANIM_WALK_ON_LADDER_END,

		ANIM_COUNT
	};
	// states of friendlyness (e.g. for friendly fire)
	enum EFriendOrEnemyStates {
		FOE_FRIENDLY,		//!< Friendly
		FOE_ENEMY,			//!< a evil person, not the player... ganons folks
	};
	static const unsigned int ANIM_NONE = ANIM_COUNT;						//!< ANIM_NONE definition: == m_uiAnimationCount
	enum EFadeStade {
		FADE_NONE = 0,
		FADE_IN = 1,
		FADE_OUT = 2,
	};
protected:
	const EFriendOrEnemyStates m_eFriendOrEnemy;	//!< Is this character a friend or an enemy?

	bool m_bMoving;
	Ogre::Entity *m_pBodyEntity;									//!< The entity of the body
	btCharacterControllerInterface *mCCPhysics;		//!< The bullet character controller (Physics)
	CCharacterController *m_pCharacterController; //!< The implemented character controller (K.I. or user)

	btAlignedObjectArray<btCollisionShape*> mCollisionShapes; //!< Collision shapes of the character

	Ogre::Real m_fYaw;

	// Animation data
	const unsigned int m_uiAnimationCount;							//!< number of animations
	Ogre::vector<Ogre::AnimationState*>::type m_Anims;	//!< master animation list
	unsigned int m_uiAnimID;														//!< current animation
	Ogre::vector<char>::type m_FadingStates;            //!< which animations are fading

	Ogre::Real m_fTimer;																//!< Animation timer (how long is the current animation running)
	Ogre::Real m_fAnimSpeed;														//!< Animation speed

public:

	// getter & setter
	Ogre::Entity *getBodyEntity() const {return m_pBodyEntity;}
	btCharacterControllerInterface *getKinematicCharacterController() const {return mCCPhysics;}
	CCharacterController *getCharacterController() const {return m_pCharacterController;}
	const EFriendOrEnemyStates getFriendOrEnemyState() const {return m_eFriendOrEnemy;}

	// reimplemented
	void setPosition(const Ogre::Vector3 &vPos);
  void setOrientation(const Ogre::Quaternion &vRotation);

protected:
	// all values scaled in world units!
	CCharacter(const std::string &sID, CEntity *pParent, CMap *pMap, const EFriendOrEnemyStates foe);

public:
	virtual ~CCharacter();

	void animRunStart();
	void animRunEnd();
	void animJumpStart();
	void animJumpLoop();
	void animJumpEnd();
	virtual void animAttack();
	virtual void animAttackEnd();

	void setIsMoving(bool isMoving);

	// default init method
	void init(Ogre::SceneNode *pParentSceneNode);
	virtual void destroy();

	void update(Ogre::Real fTime);
protected:
	virtual void setupInternal() {};
	virtual void createPhysics() = 0;
	virtual void destroyPhysics() = 0;
	virtual void initBody(Ogre::SceneNode *pParentSceneNode) = 0;
	virtual void setupAnimations() = 0;


	virtual void attack(const Ogre::Real fTime) {}
protected:
	virtual void preUpdateBoundsCallback(const Ogre::Real fTime) {}
	virtual void preAnimationUpdateCallback(const Ogre::Real fTime) {}
	virtual void updateAnimationsCallback(const Ogre::Real fTime);
	virtual void postUpdateAnimationsCallback(const Ogre::Real fTime) {}
	virtual CCharacterController *createCharacterController() = 0;
	void updateAnimations(const Ogre::Real fTime);

	void setAnimation(unsigned int id, bool reset = false);

	virtual short getCollisionMask();
	virtual short getCollisionGroup();

	void fadeAnimations(const Ogre::Real deltaTime);
};

#endif // _CHARACTER_H_
