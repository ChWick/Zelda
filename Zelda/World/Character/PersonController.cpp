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

#include "PersonController.hpp"
#include "Person.hpp"
#include <BulletDynamics/Character/btCharacterControllerInterface.h>
#include "../../Common/Physics/BtOgreExtras.hpp"
#include "../../Common/Util/DebugDrawer.hpp"
#include "../../Common/Message/MessageHandler.hpp"
#include "../../Common/Message/MessageTargetReached.hpp"
#include "CharacterController_Physics.hpp"

const Ogre::Real DEFAULT_PUSHED_BACK_TIME = 0.1f;
const Ogre::Real WALK_SPEED = 6; 					//!< constant for the walk speed
const Ogre::Real RUN_SPEED = 18;          //!< constant for the run speed
const Ogre::Real TURN_SPEED = 500;				//!< constant for the turn speed
const Ogre::Real WALK_SPEED_SCALE = 0.001;
const Ogre::Real PUSHED_SPEED = 30;

CPersonController::CPersonController(CPerson * ccPlayer) {
  mCCPerson = ccPlayer;

	mCCPhysics = dynamic_cast<CharacterControllerPhysics*>(mCCPerson->getKinematicCharacterController());
	mBodyNode = mCCPerson->getSceneNode();

	mJumped = false;
  m_fMoveSpeed = WALK_SPEED;
	//mIsFalling = mCCPhysics->onGround();
	mWalkDirection = Ogre::Vector3::ZERO;

	m_fTimer = 0.0f;
	changeMoveState(MS_NOT_MOVING); // default state
}
void CPersonController::setPosition(const Ogre::Vector3 &vPos) {
    mCCPhysics->warp(BtOgre::Convert::toBullet(vPos));
}
void CPersonController::setOrientation(const Ogre::Quaternion &vRotation) {
    mBodyNode->setOrientation(vRotation);
    mCCPerson->getCollisionObject()->getWorldTransform().setRotation(BtOgre::Convert::toBullet(vRotation));
}
void CPersonController::updateCharacter(const Ogre::Real deltaTime) {
	using namespace Ogre;

	m_fTimer -= deltaTime;
	Real posIncrementPerSecond = m_fMoveSpeed;

	Vector3 playerPos = mCCPerson->getPosition();

	//btVector3 pos = mCCPhysics->getPosition();

	//Vector3 position(pos.x(), pos.y(), pos.z());
	Vector3 position(BtOgre::Convert::toOgre(mCCPerson->getCollisionObject()->getWorldTransform().getOrigin()));


	if (position != playerPos)
	{
	  Ogre::Vector3 vTranslateDirection = position - playerPos;
	  Ogre::Real fTranslateDistance = vTranslateDirection.normalise();
	  Ogre::Real fDesiredDistance = 20 * deltaTime * m_fMoveSpeed / WALK_SPEED * fTranslateDistance;
	  
	  mBodyNode->translate(vTranslateDirection * std::min<Ogre::Real>(fTranslateDistance, fDesiredDistance));
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

			//Ogre::LogManager::getSingleton().logMessage(Ogre::StringConverter::toString(mGoalDirection));
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
      if (m_uiCurrentMoveState != MS_RUNNING) {
        updateGoalDirection();
      }
      else {
        mGoalDirection = mBodyNode->getOrientation().zAxis();
      }
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
          move(true, posIncrementPerSecond, mGoalDirection);
				}
				else {
					move(false);
				}
			}
			else
			{
				move(false);
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
    move(true, PUSHED_SPEED, m_vUserData);
	}
	else if (m_uiCurrentMoveState == MS_STUNNED) {
		if (m_fTimer <= 0) {
			changeMoveState(MS_NORMAL);
		}
    else {
      move(false);
    }
	}
  else if (m_uiCurrentMoveState == MS_RUN_START) {
    if (m_fTimer <= 0) {
      changeMoveState(MS_RUNNING);
    }
    else {
      mGoalDirection = mBodyNode->getOrientation().zAxis();
      
      move(false);
    }
  }
  else if (m_uiCurrentMoveState == MS_RUNNING) {
    mGoalDirection = mBodyNode->getOrientation().zAxis();

    move(true, RUN_SPEED / WALK_SPEED * posIncrementPerSecond, mGoalDirection);

    if (mCCPhysics->isStuck()) {
      changeMoveState(MS_PUSHED_BACK, -mGoalDirection * 0.5, 1);

      for (CWorldEntity *pEnt : mCCPhysics->getCollidingWorldEntities()) {
        pEnt->hit(CDamage(DMG_RUN));
      }
    }
  }
	else if (m_uiCurrentMoveState >= MS_USER_STATE) {
		userUpdateCharacter(deltaTime);
	}

	// give the user the choice to deal with the current state, e.g. change state if MS_NOT_MOVING
	postUpdateCharacter(deltaTime);


	if (m_uiCurrentMoveState == MS_MOVE_TO_POINT) {
    if (m_fTimer < 0) {
      targetReached();
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

void CPersonController::move(bool bMove, Ogre::Real fSpeed, const Ogre::Vector3 &vDir) {
  if (bMove) {
		mCCPhysics->setWalkDirection(BtOgre::Convert::toBullet(vDir * fSpeed * WALK_SPEED_SCALE));
    mCCPhysics->setSubSteps(std::max<int>(ceil(fSpeed / WALK_SPEED), 1));
  }
  else {
    mCCPhysics->setWalkDirection(btVector3(0, 0, 0));
    mCCPhysics->setSubSteps(1);
  }
  mCCPerson->setIsMoving(bMove);
}

void CPersonController::moveToTarget(const Ogre::Vector3 &vPos, const Ogre::Real dRadius, const Ogre::Degree &maxDeviationLookDir, bool bAddCharCOMHeight, const Ogre::Real fMaxDuraion) {
    changeMoveState(MS_MOVE_TO_POINT, vPos + ((bAddCharCOMHeight) ? Ogre::Vector3(0, CPerson::PERSON_HEIGHT, 0): Ogre::Vector3::ZERO), dRadius, maxDeviationLookDir.valueRadians());
    m_fTimer = fMaxDuraion;
}
void CPersonController::stun(const Ogre::Real fTime) {
    changeMoveState(MS_STUNNED, Ogre::Vector3::ZERO, fTime);
}

void CPersonController::startRunning() {
  changeMoveState(MS_RUN_START);
  m_fTimer = 1;
}

void CPersonController::endRunning() {
  changeMoveState(MS_NORMAL);
}

void CPersonController::targetReached() {
    changeMoveState(MS_NOT_MOVING); // reset our state

  CMessageHandler::getSingleton().addMessage(new CMessageTargetReached(mCCPerson));
}
