#ifndef _PERSON_CONTROLLER_H_
#define _PERSON_CONTROLLER_H_

#include "CharacterController.h"
#include "PlayerTool.h"

class btCharacterControllerInterface;
class CPerson;
class CDamage;

class CPersonController : public CCharacterController {
public:
	//! Different states of motion of a person
	enum EMoveState {
		MS_NOT_MOVING = 0,		//!< Not moving at all
		MS_NORMAL,				//!< Normal control of moving (call of updateGoalDirection() )
		MS_MOVE_TO_POINT,		//!< Person will move to the target position (m_vUserData)
		MS_PUSHED_BACK,			//!< Person is pushed back
		MS_STUNNED,				//!< The person is stunned
		MS_MOVE_AROUND_TARGET,	//!< the person will move around a target
		MS_AIMING,              //!< player not moving, but looking in camera's direction
		MS_LADDER,              //!< person is on a ladder

		MS_USER_STATE,			//!< user has to controll motion (call of userUpdateCharacter(Ogre::Real) )
	};
protected:

	Ogre::Real m_fTimer;							//!< timer
	bool mIsFalling;								//!< is the person falling?
	bool mJumped;									//!< is the person jumping?


	btCharacterControllerInterface * mCCPhysics;	//!< pointer to the kinematic character controller
	CPerson * mCCPerson;							//!< pointer to the person to controll
	Ogre::Vector3 mGoalDirection;					//!< direction in witch the person wants to walk

	Ogre::SceneNode * mBodyNode;					//!< the scene node of the body
	Ogre::Vector3 mWalkDirection;					//!< the direction in witch the person is actually walking
	unsigned int m_uiCurrentMoveState;				//!< the current Move state

	Ogre::Vector3 m_vUserData;						//!< Storage for a user data given for the move state
	Ogre::Real m_fUserData0;						//!< Storage for a user data given for the move state
	Ogre::Real m_fUserData1;						//!< Storage for a user data given for the move state

	// aliase, union does not work with classes (Ogre::Vector3)
	inline Ogre::Vector3	&getPushedBackDirection() {return m_vUserData;}
	inline Ogre::Real		&getPushedBackTime() {return m_fUserData0;}
	inline Ogre::Real		&getStunnedTimeAfterPushedBack() {return m_fUserData1;}

	inline Ogre::Vector3	&getTargetPosition() {return m_vUserData;}
	inline Ogre::Real		&getTargetRadius() {return m_fUserData0;}
	inline Ogre::Real		&getMaxTargetLookAngle() {return m_fUserData1;}

	inline Ogre::Real		&getStunnedTime() {return m_fUserData0;}

	inline Ogre::Vector3    &getCameraDirection() {return m_vUserData;}

	inline Ogre::Vector3	&getMoveAroundPosition() {return m_vUserData;}

	inline Ogre::Real       &getPositionOnLadder() {return m_fUserData0;}
	inline Ogre::Real       &getInitialMoveTimeLeft() {return m_fUserData1;}

public:
	CPersonController(CPerson * ccPlayer);
	virtual ~CPersonController() {}

	CPerson *getCCPerson() const {return mCCPerson;}
	Ogre::SceneNode *getBodyNode() const {return mBodyNode;}
	btCharacterControllerInterface *getCCPhysics() {return mCCPhysics;}

	virtual void setPosition(const Ogre::Vector3 &vPos);
    virtual void setOrientation(const Ogre::Quaternion &vRotation);


	void updateCharacter(const Ogre::Real tpf);

	//!< change the move state
	/**
	 * if MS_MOVE_TO:				vUserData is the target position and fUserData0 a radius around the position, fUserData1 will be the maximal deviation of the look dir from the goal direction
	 * if MS_PUSHED_BACK:			vUserData is the direction in witch the person will be pushed, fUserData0 is the time the player will be pushed back, if fUserData1 is not one the state will change afterwards in a stunned state
	 * if MS_STUNNED:				fUserData0 is the time the person will be stunned
	 * if MS_MOVE_AROUND_TARGET:	vUserData is the point to move around
	 */
	void changeMoveState(unsigned int uiNewMoveState, const Ogre::Vector3 &vUserData = Ogre::Vector3::ZERO, const Ogre::Real fUserData0 = 0, const Ogre::Real fUserData1 = 0);

	unsigned int getMoveState() const {return m_uiCurrentMoveState;}

  void stun(const Ogre::Real fTime = std::numeric_limits<Ogre::Real>::max());
	void moveToTarget(const Ogre::Vector3 &vPos, const Ogre::Real dRadius = 0.1f, const Ogre::Degree &maxDeviationLookDir = Ogre::Degree(5), bool bAddCharCOMHeight = true, const Ogre::Real fMaxDuraion = 10.0f);

protected:

	virtual void updateGoalDirection();
	virtual void userUpdateCharacter(const Ogre::Real tpf) {}
	virtual void postUpdateCharacter() {} // one can handle the current state, e.g. if it's MS_NOT_MOVING or MS_NORMAL

	void selectNextTool();
	void selectPreviousTool();

	void targetReached();
};

#endif
