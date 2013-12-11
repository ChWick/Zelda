#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include "StdAfx.h"
#include "PhysicsManager.h"
#include "HitableInterface.h"
#include "Hitpoints.h"
#include "ChatableInterface.h"

class btRigidBody;
class CMap;
class CCharacterController;


#define ANIM_FADE_SPEED 7.5f   // animation crossfade speed in % of full weight per second

//! Klasse f&uuml;r einen animierten Charater mit Standardanimationen
/*!
Elternklassen f&uur: Charakter kann zerst&ouml;rt werden, Charakter kann Schaden verursachen.
*/
class CCharacter : public CHitableInterface, public CHitpointImplementation, public ChatableInterface {
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
		ANIM_IDLE_BASE,
		ANIM_IDLE_TOP,
		ANIM_RUN_BASE,
		ANIM_RUN_TOP,
		ANIM_WALK_BASE,
		ANIM_WALK_TOP,
		ANIM_HANDS_CLOSED,
		ANIM_HANDS_RELAXED,
		ANIM_SLICE_VERTICAL,
		ANIM_SLICE_HORIZONTAL,
        ANIM_BOW_AIM_START,
        ANIM_BOW_AIM_IDLE,
		ANIM_BOW_SHOT,
        ANIM_DRAW_SWORDS,
		ANIM_JUMP_START,
		ANIM_JUMP_LOOP,
		ANIM_JUMP_END,
		ANIM_PICK_HEAVY,
		ANIM_LIFT_HEAVY,
		ANIM_WALK_HEAVY_TOP,
		ANIM_THROW_HEAVY,
		ANIM_OPEN_DOOR,
		ANIM_JUMP_INTO_WELL,
		ANIM_SHIELD_BLOCK_START,
		ANIM_SHIELD_BLOCK_IDLE,
		ANIM_BLOCK_SLICE_HORIZONTAL_BASE,
		ANIM_WALK_ON_LADDER,
		ANIM_WALK_ON_LADDER_END,
		ANIM_COUNT
	};
	// states of friendlyness (e.g. for friendly fire)
	enum EFriendOrEnemyStates {
		FOE_FRIENDLY,		//!< Friendly
		FOE_ENEMY,			//!< a evil person, no, no the player... ganons folks
	};
	static const unsigned int ANIM_NONE = ANIM_COUNT;						//!< ANIM_NONE definition: == m_uiAnimationCount
	enum EFadeStade {
		FADE_NONE = 0,
		FADE_IN = 1,
		FADE_OUT = 2,
	};
protected:
	const EFriendOrEnemyStates m_eFriendOrEnemy;	//!< Is this character a friend or an enemy?
	CMap *m_pCurrentMap;						//!< the map the Character belonds to
	static Ogre::NameGenerator m_NameGenerator;

	bool m_bMoving;
	Ogre::Entity *m_pBodyEntity;
	Ogre::SceneNode *m_pBodyNode;
	btCharacterControllerInterface *mCCPhysics;
	//btKinematicCharacterController *mCCPhysics;
	//CharacterController_Physics *mCCPhysics;

	//Critter::Body *m_pBodyPhysics;
	btCollisionObject *m_pBodyPhysics;
	btAlignedObjectArray<btCollisionShape*> mCollisionShapes;

	Ogre::Real m_fYaw;

	// Animation data
	const unsigned int m_uiAnimationCount;
	Ogre::vector<Ogre::AnimationState*>::type m_Anims;	//!< master animation list
	unsigned int m_uiBaseAnimID;						//!< current base (full- or lower-body) animation
	unsigned int m_uiTopAnimID;							//!< current top (upper-body) animation
	Ogre::vector<char>::type m_FadingStates;            //!< which animations are fading
	Ogre::Real m_fTimer;
	Ogre::Real m_fTopAnimSpeed;
	Ogre::Real m_fBaseAnimSpeed;

	CCharacterController *m_pCharacterController;

public:
	Ogre::SceneNode *getBodyNode() const {return m_pBodyNode;}
	Ogre::Entity *getBodyEntity() const {return m_pBodyEntity;}
	btCollisionObject *getBodyPhysics() const {return m_pBodyPhysics;}
	Ogre::SceneNode *getBodySceneNode() const {return m_pBodyNode;}
    Ogre::SceneNode *getSceneNode() const {return m_pBodyNode;}
	const Ogre::Vector3 &getPosition() const {return m_pBodyNode->getPosition();}
	const Ogre::Quaternion &getOrientation() const {return m_pBodyNode->getOrientation();}
	btCharacterControllerInterface *getKinematicCharacterController() const {return mCCPhysics;}
	CCharacterController *getCharacterController() const {return m_pCharacterController;}

    const EFriendOrEnemyStates getFriendOrEnemyState() const {return m_eFriendOrEnemy;}

    void setPosition(const Ogre::Vector3 &vPos);
    void setOrientation(const Ogre::Quaternion &vRotation);

	btCollisionObject *getCollisionObject() const {return m_pBodyPhysics;}
	EReceiveDamageResult receiveDamage(const CDamage &damage);
protected:
	// all values scaled in world units!
	CCharacter(CMap *pMap, const EFriendOrEnemyStates foe);
public:
	virtual ~CCharacter();

	void animRunStart();
    void animRunEnd();
    void animJumpStart();
    void animJumpLoop();
    void animJumpEnd();
    virtual void animAttack();
    virtual void animAttackEnd();
    virtual bool topReadyForNewAction();
	virtual void damageBlocked(const CDamage &damage) {}
    void setIsMoving(bool isMoving);
	// default init method
	void init(Ogre::SceneNode *pParentSceneNode);
	virtual void destroy();

	void update(const Ogre::Real fTime);
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
	void setBaseAnimation(unsigned int id, bool reset = false);
	void setTopAnimation(unsigned int id, bool reset = false);

	virtual short getCollisionMask();
	virtual short getCollisionGroup();

	void fadeAnimations(const Ogre::Real deltaTime);
};

#endif // _CHARACTER_H_
