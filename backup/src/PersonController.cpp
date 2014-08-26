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
#include "PersonController.h"
#include "Person.h"
#include "CharacterControllerListener.h"
#include "PlayerData.h"
#include "Ladder.h"
#include "DebugDrawer.h"

const Ogre::Real DEFAULT_PUSHED_BACK_TIME = 0.1f;
const Ogre::Real RUN_SPEED = 40;					//!< constat for the run speed
const Ogre::Real TURN_SPEED = 500;				//!< constant for the turn speed

CPersonController::CPersonController(CPerson * ccPlayer)
: m_Tools(CPlayerData::getSingleton().getPlayerTools()) {
    mCCPerson = ccPlayer;

	m_pLadder = NULL;

	mCCPhysics = mCCPerson->getKinematicCharacterController();
	mBodyNode = mCCPerson->getBodyNode();

	mJumped = false;
	mIsFalling = mCCPhysics->onGround();
	mWalkDirection = Ogre::Vector3::ZERO;

	m_fTimer = 0.0f;
	changeMoveState(MS_NOT_MOVING); // default state
}
void CPersonController::setPosition(const Ogre::Vector3 &vPos) {
    mCCPhysics->warp(BtOgre::Convert::toBullet(vPos));
}
void CPersonController::setOrientation(const Ogre::Quaternion &vRotation) {
    mBodyNode->setOrientation(vRotation);
    mCCPerson->getBodyPhysics()->getWorldTransform().setRotation(BtOgre::Convert::toBullet(vRotation));
}
void CPersonController::updateCharacter(const Ogre::Real deltaTime) {
	using namespace Ogre;

	m_fTimer -= deltaTime;

	Real posIncrementPerSecond = RUN_SPEED * 0.001f;

	Vector3 playerPos = mCCPerson->getPosition();

	//btVector3 pos = mCCPhysics->getPosition();

	//Vector3 position(pos.x(), pos.y(), pos.z());
	Vector3 position(BtOgre::Convert::toOgre(mCCPerson->getBodyPhysics()->getWorldTransform().getOrigin()));


	if (position != playerPos)
	{
		mBodyNode->translate((position - playerPos) * RUN_SPEED * deltaTime);
		//mBodyNode->setPosition(position);

		if (!mIsFalling && !mCCPhysics->onGround()) // last frame we were on ground and now we're in "air"
		{
			mIsFalling = true;

			if (!mJumped) // if we jumped, let the CharacterController_Player's updateAnimations take care about this
				mCCPerson->animJumpLoop();
		}
		else if (mCCPhysics->onGround() && mIsFalling) // last frame we were falling and now we're on the ground
		{
			mIsFalling = false;
			mJumped = false;

			mCCPerson->animJumpEnd();
		}
	}

	if (m_uiCurrentMoveState == MS_NORMAL || m_uiCurrentMoveState == MS_MOVE_TO_POINT
        || m_uiCurrentMoveState == MS_MOVE_AROUND_TARGET || m_uiCurrentMoveState == MS_AIMING) {
		mGoalDirection = Vector3::ZERO;   // we will calculate this
		Ogre::Vector3 vLookDirection;
		bool bMove = true;
		if (m_uiCurrentMoveState == MS_MOVE_TO_POINT) {
			// the goal direction will be obviously the direction in witch the target is
			mGoalDirection = getTargetPosition() - mBodyNode->_getDerivedPosition();
			if (mGoalDirection.squaredLength() <= getTargetRadius() * getTargetRadius()) {
				// we reached the correct distance (check for view angle is the next step)
				bMove = false;
			}
			else {
				// we will move, so normalise direction
				mGoalDirection.normalise();
			}
			Ogre::Radian viewAngle(mGoalDirection.angleBetween(mBodyNode->_getDerivedOrientation().zAxis()));
			if (abs(viewAngle.valueRadians()) < getMaxTargetLookAngle()) {
				// target reached if bMove == false
				if (bMove == false) {
                    targetReached();
				}
			}

			vLookDirection = mGoalDirection;
		}
		else if (m_uiCurrentMoveState == MS_AIMING) {
			updateGoalDirection();
            vLookDirection = getCameraDirection();
		}
		else {
			updateGoalDirection();
			vLookDirection = mGoalDirection;
		}

		if (m_uiCurrentMoveState == MS_MOVE_AROUND_TARGET) {
			Ogre::Vector3 vTargetDir = getMoveAroundPosition() - mCCPerson->getPosition();
			Quaternion toGoal = mBodyNode->getOrientation().zAxis().getRotationTo(vTargetDir);

			// calculate how much the character has to turn to face goal direction
			Real yawToGoal = toGoal.getYaw().valueDegrees();
			// this is how much the character CAN turn this frame
			Real yawAtSpeed = yawToGoal / Math::Abs(yawToGoal) * deltaTime * TURN_SPEED;

			// check if we reached our targed (if MS_MOVE_TO_POINT)
			// turn as much as we can, but not more than we need to
			if (yawToGoal < 0) yawToGoal = std::min<Real>(0, std::max<Real>(yawToGoal, yawAtSpeed));
			else if (yawToGoal > 0) yawToGoal = std::max<Real>(0, std::min<Real>(yawToGoal, yawAtSpeed));


			mBodyNode->yaw(Degree(yawToGoal));

			mCCPhysics->setWalkDirection(BtOgre::Convert::toBullet(mGoalDirection * posIncrementPerSecond));
			mCCPerson->setIsMoving(true);
		}
		else {
            if (vLookDirection != Vector3::ZERO) {
				Quaternion toGoal = mBodyNode->getOrientation().zAxis().getRotationTo(vLookDirection);

				// calculate how much the character has to turn to face goal direction
				Real yawToGoal = toGoal.getYaw().valueDegrees();
				// this is how much the character CAN turn this frame
				Real yawAtSpeed = yawToGoal / Math::Abs(yawToGoal) * deltaTime * TURN_SPEED;

				// check if we reached our targed (if MS_MOVE_TO_POINT)
				// turn as much as we can, but not more than we need to
				if (yawToGoal < 0) yawToGoal = std::min<Real>(0, std::max<Real>(yawToGoal, yawAtSpeed));
				else if (yawToGoal > 0) yawToGoal = std::max<Real>(0, std::min<Real>(yawToGoal, yawAtSpeed));


				mBodyNode->yaw(Degree(yawToGoal));
            }

			if (mGoalDirection != Vector3::ZERO) {
				if (bMove) {
					mCCPhysics->setWalkDirection(BtOgre::Convert::toBullet(mGoalDirection * posIncrementPerSecond));
					mCCPerson->setIsMoving(true);
				}
				else {
					mCCPhysics->setWalkDirection(btVector3(0, 0, 0));
					mCCPerson->setIsMoving(false);
				}
			}
			else
			{
				mCCPhysics->setWalkDirection(btVector3(0, 0, 0));
				mCCPerson->setIsMoving(false);
			}
		}
	}
	else if (m_uiCurrentMoveState == MS_PUSHED_BACK) {
		if (m_fTimer <= 0) {
			if (getStunnedTimeAfterPushedBack() != 0) {
				changeMoveState(MS_STUNNED, Ogre::Vector3::ZERO, getStunnedTimeAfterPushedBack());
			}
			else {
				changeMoveState(MS_NORMAL);
			}
		}
		mCCPhysics->setWalkDirection(BtOgre::Convert::toBullet(m_vUserData) * posIncrementPerSecond);
		mCCPerson->setIsMoving(true);
	}
	else if (m_uiCurrentMoveState == MS_STUNNED) {
		if (m_fTimer <= 0) {
			changeMoveState(MS_NORMAL);
		}
	}
	else if (m_uiCurrentMoveState == MS_LADDER) {
        updateGoalDirection();
        if (getInitialMoveTimeLeft() > 0) {
            getInitialMoveTimeLeft() -= deltaTime;

            getPositionOnLadder() += deltaTime * 1.0f * m_iInitialClimbingPosition;
        }
        else if (mGoalDirection.z != 0) {
            getPositionOnLadder() += deltaTime * 1.0f * (mGoalDirection.z > 0 ? 1 : -1);
        }

        if (m_pLadder->atBottom(getPositionOnLadder())) {
            moveToTarget(m_pLadder->getBottomExitPos(), 0.4f, Ogre::Degree(5), false, 1);
            m_pLadder = NULL;
        }
        else if (m_pLadder->atTop(getPositionOnLadder())) {
            moveToTarget(m_pLadder->getTopExitPos(), 0.4f, Ogre::Degree(5), false, 1);
            m_pLadder = NULL;
        }
        else {
            mCCPerson->setIsMoving(false);
            mCCPhysics->setWalkDirection(btVector3(0, 0, 0));
            mCCPhysics->warp(BtOgre::Convert::toBullet(m_pLadder->getClimberPosition(getPositionOnLadder())));
            //mCCPerson->getCollisionObject()->getWorldTransform().setRotation(BtOgre::Convert::toBullet(Ogre::Quaternion(Ogre::Radian(m_pLadder->getYawRadians()), Ogre::Vector3::UNIT_Y)));
            mBodyNode->setOrientation(Ogre::Quaternion(Ogre::Radian(m_pLadder->getYawRadians() + Ogre::Math::PI), Ogre::Vector3::UNIT_Y));
        }
	}
	else if (m_uiCurrentMoveState >= MS_USER_STATE) {
		userUpdateCharacter(deltaTime);
	}

	// give the user the choice to deal with the current state, e.g. change state if MS_NOT_MOVING
	postUpdateCharacter();


	if (m_uiCurrentMoveState == MS_MOVE_TO_POINT) {
        if (m_fTimer < 0) {
            changeMoveState(MS_NORMAL);
        }
        DebugDrawer::getSingleton().drawSphere(getTargetPosition(), getTargetRadius(), Ogre::ColourValue::Blue, false);
	}
}
void CPersonController::updateGoalDirection() {
}
void CPersonController::changeMoveState(unsigned int uiNewMoveState, const Ogre::Vector3 &vUserData, const Ogre::Real fUserData0, const Ogre::Real fUserData1) {
	m_uiCurrentMoveState = uiNewMoveState;
	m_fTimer = 0;
	m_vUserData = vUserData;
	m_fUserData0 = fUserData0;
	m_fUserData1 = fUserData1;

	if (m_uiCurrentMoveState == MS_PUSHED_BACK) {
		if (getPushedBackDirection().isZeroLength()) {
			// user own direction
			getPushedBackDirection() = -mBodyNode->getOrientation().zAxis();
		}
		if (fUserData0 == 0) {
			m_fUserData0 = DEFAULT_PUSHED_BACK_TIME;
		}
		m_fTimer = getPushedBackTime();
    }
	else if (m_uiCurrentMoveState == MS_STUNNED) {
        m_fTimer = getStunnedTime();
	}
}
void CPersonController::moveToTarget(const Ogre::Vector3 &vPos, const Ogre::Real dRadius, const Ogre::Degree &maxDeviationLookDir, bool bAddCharCOMHeight, const Ogre::Real fMaxDuraion) {
    changeMoveState(MS_MOVE_TO_POINT, vPos + ((bAddCharCOMHeight) ? Ogre::Vector3(0, CPerson::PERSON_HEIGHT, 0): Ogre::Vector3::ZERO), dRadius, maxDeviationLookDir.valueRadians());
    m_fTimer = fMaxDuraion;
}
void CPersonController::stun(const Ogre::Real fTime) {
    changeMoveState(MS_STUNNED, Ogre::Vector3::ZERO, fTime);
}
void CPersonController::selectNextTool() {
    m_Tools.selectNextTool();
    mCCPerson->changeTool(m_Tools.getCurrentTool());
}
void CPersonController::selectPreviousTool() {
    m_Tools.selectPreviousTool();
    mCCPerson->changeTool(m_Tools.getCurrentTool());
}
void CPersonController::targetReached() {
    changeMoveState(MS_NOT_MOVING); // reset our state

    for (CharacterControllerListener *pCL : m_lListeners) {
        pCL->targetReached(mCCPerson);
    }
}
void CPersonController::climbLadder(Ladder *pLadder, const Ogre::Real fPosOnLadder) {
    if (m_uiCurrentMoveState != MS_NORMAL) {return;}
    assert(pLadder);
    if (m_pLadder == pLadder) {return;}
    m_pLadder = pLadder;
    changeMoveState(MS_LADDER);

    getPositionOnLadder() = fPosOnLadder;
    getInitialMoveTimeLeft() = 0.1f;

    m_iInitialClimbingPosition = (fPosOnLadder < 0.1) ? 1 :-1;
}
