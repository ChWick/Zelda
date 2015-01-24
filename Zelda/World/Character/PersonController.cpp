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
#include <BulletDynamics/Character/btCharacterControllerInterface.h>
#include <algorithm>
#include "Person.hpp"
#include "../../Common/Log.hpp"
#include "../../Common/Physics/BtOgreExtras.hpp"
#include "../../Common/Util/DebugDrawer.hpp"
#include "../../Common/Message/MessageHandler.hpp"
#include "../../Common/Message/MessageTargetReached.hpp"
#include "CharacterController_Physics.hpp"

const Ogre::Real DEFAULT_PUSHED_BACK_TIME = 0.1f;
const Ogre::Real WALK_SPEED = 6;              //!< constant for the walk speed
const Ogre::Real RUN_SPEED = 18;              //!< constant for the run speed
const Ogre::Real TURN_SPEED = 500;            //!< constant for the turn speed
const Ogre::Real WALK_SPEED_SCALE = 0.001;
const Ogre::Real PUSHED_SPEED = 30;

CPersonController::CPersonController(CPerson * ccPerson)
    : m_fTimer(0),
      mIsFalling(false),
      mJumped(false),
      m_fWalkSpeed(WALK_SPEED),
      m_fRunSpeed(RUN_SPEED),
      m_fCurrentMoveSpeed(WALK_SPEED),
      mCCPhysics(dynamic_cast<CharacterControllerPhysics*>(
          ccPerson->getKinematicCharacterController())),
      mCCPerson(ccPerson),
      mGoalDirection(Ogre::Vector3::ZERO),
      mBodyNode(ccPerson->getSceneNode()),
      mWalkDirection(Ogre::Vector3::ZERO),
      m_uiCurrentMoveState(MS_NORMAL),
  m_vUserData(Ogre::Vector3::ZERO),
  m_fUserData0(0),
  m_fUserData1(1) {
  changeMoveState(MS_NOT_MOVING);  // default state
  mCCPhysics->setSubStepSpeedReference(WALK_SPEED * WALK_SPEED_SCALE);
}

void CPersonController::setPosition(const Ogre::Vector3 &vPos) {
  mCCPhysics->warp(BtOgre::Convert::toBullet(vPos));
}

void CPersonController::setOrientation(const Ogre::Quaternion &vRotation) {
  mBodyNode->setOrientation(vRotation);
  mCCPerson->getCollisionObject()->getWorldTransform().setRotation(
      BtOgre::Convert::toBullet(vRotation));
}
void CPersonController::updateCharacter(const Ogre::Real deltaTime) {
  using Ogre::Vector3;
  using Ogre::Real;
  using Ogre::Quaternion;
  using Ogre::Degree;
  using Ogre::Math;

  // update the move speed
  updateCurrentMoveSpeed();

  // then we can calculate our move step
  m_fTimer -= deltaTime;
  Real posIncrementPerSecond = m_fCurrentMoveSpeed;

  Vector3 playerPos = mCCPerson->getPosition();

  // btVector3 pos = mCCPhysics->getPosition();

  // Vector3 position(pos.x(), pos.y(), pos.z());
  Vector3 position(BtOgre::Convert::toOgre(
      mCCPerson->getCollisionObject()->getWorldTransform().getOrigin()));
  Ogre::Vector3 vTranslateDirection = position - playerPos;
  Vector3 physicsFloorPosition(
      mCCPerson->getFloorPosition() + vTranslateDirection);


  if (position != playerPos) {
    Ogre::Real fTranslateDistance = vTranslateDirection.normalise();
    Ogre::Real fDesiredDistance = 20 * deltaTime * m_fCurrentMoveSpeed
        / WALK_SPEED * fTranslateDistance;

    mBodyNode->translate(vTranslateDirection * std::min<Ogre::Real>(
        fTranslateDistance, fDesiredDistance));
    // mBodyNode->setPosition(position);

    if (!mIsFalling && !mCCPhysics->onGround()) {
      // last frame we were on ground and now we're in "air"
      mIsFalling = true;

      if (!mJumped) {
        // if we jumped, let the CharacterController_Player's
        // updateAnimations take care about this
        mCCPerson->animJumpLoop();
      }
    } else if (mCCPhysics->onGround() && mIsFalling) {
      // last frame we were falling and now we're on the ground
      mIsFalling = false;
      mJumped = false;

      mCCPerson->animJumpEnd();
    }
  }

  if (m_uiCurrentMoveState == MS_NORMAL
      || m_uiCurrentMoveState == MS_MOVE_TO_POINT
      || m_uiCurrentMoveState == MS_MOVE_AROUND_TARGET
      || m_uiCurrentMoveState == MS_AIMING) {
    mGoalDirection = Vector3::ZERO;   // we will calculate this
    Ogre::Vector3 vLookDirection;
    bool bMove = true;
    if (m_uiCurrentMoveState == MS_MOVE_TO_POINT) {
      // the goal direction will be obviously the direction
      // in witch the target is
      mGoalDirection = getTargetPosition() - physicsFloorPosition;
      /*
        LOGI("Pos: %s distance %s",
        Ogre::StringConverter::toString(mGoalDirection).c_str(),
        Ogre::StringConverter::toString(physicsFloorPosition).c_str());
      */
      mGoalDirection.y = 0;  // never move in y direction.
      Ogre::Real distance(mGoalDirection.normalise());
      Ogre::Real stepLength(posIncrementPerSecond * WALK_SPEED_SCALE);

      if (distance - stepLength <= 0) {
        // we will move over target
        bMove = false;
      } else if (distance <= getTargetRadius()) {
        // we reached the correct distance
        // (check for view angle is the next step)
        bMove = false;
      } else {
        // move on
      }
      Ogre::Radian viewAngle(mGoalDirection.angleBetween(
          mBodyNode->_getDerivedOrientation().zAxis()));
      if (abs(viewAngle.valueRadians()) < getMaxTargetLookAngle()) {
        // target reached if bMove == false
        if (bMove == false) {
          targetReached();
        }
      }

      vLookDirection = mGoalDirection;
    } else if (m_uiCurrentMoveState == MS_AIMING) {
      updateGoalDirection();
      vLookDirection = getCameraDirection();
    } else {
      updateGoalDirection();
      vLookDirection = mGoalDirection;
    }

    if (m_uiCurrentMoveState == MS_MOVE_AROUND_TARGET) {
      Ogre::Vector3 vTargetDir =
          getMoveAroundPosition() - mCCPerson->getPosition();
      Quaternion toGoal = mBodyNode->getOrientation().zAxis().getRotationTo(
          vTargetDir);

      // calculate how much the character has to turn to face goal direction
      Real yawToGoal = toGoal.getYaw().valueDegrees();
      // this is how much the character CAN turn this frame
      Real yawAtSpeed = yawToGoal / Math::Abs(yawToGoal)
          * deltaTime * TURN_SPEED;

      // check if we reached our targed (if MS_MOVE_TO_POINT)
      // turn as much as we can, but not more than we need to
      if (yawToGoal < 0) {
        yawToGoal = std::min<Real>(0, std::max<Real>(yawToGoal, yawAtSpeed));
      } else if (yawToGoal > 0) {
        yawToGoal = std::max<Real>(0, std::min<Real>(yawToGoal, yawAtSpeed));
      }

      mBodyNode->yaw(Degree(yawToGoal));
    } else {
      if (vLookDirection != Vector3::ZERO) {
        Quaternion toGoal = mBodyNode->getOrientation().zAxis().getRotationTo(
            vLookDirection);

        // calculate how much the character has to turn to face goal direction
        Real yawToGoal = toGoal.getYaw().valueDegrees();
        // this is how much the character CAN turn this frame
        Real yawAtSpeed = yawToGoal / Math::Abs(yawToGoal)
            * deltaTime * TURN_SPEED;

        // check if we reached our targed (if MS_MOVE_TO_POINT)
        // turn as much as we can, but not more than we need to
        if (yawToGoal < 0) {
          yawToGoal = std::min<Real>(0, std::max<Real>(yawToGoal, yawAtSpeed));
        } else if (yawToGoal > 0) {
          yawToGoal = std::max<Real>(0, std::min<Real>(yawToGoal, yawAtSpeed));
        }

        mBodyNode->yaw(Degree(yawToGoal));
      }
    }

    if (mGoalDirection != Vector3::ZERO) {
      move(bMove, posIncrementPerSecond, mGoalDirection);
    } else {
      move(false);
    }
  } else if (m_uiCurrentMoveState == MS_PUSHED_BACK) {
    if (m_fTimer <= 0) {
      if (getStunnedTimeAfterPushedBack() != 0) {
        changeMoveState(MS_STUNNED,
                        Ogre::Vector3::ZERO,
                        getStunnedTimeAfterPushedBack());
      } else {
        changeMoveState(MS_NORMAL);
      }
    }
    move(true, PUSHED_SPEED, m_vUserData);
  } else if (m_uiCurrentMoveState == MS_STUNNED) {
    if (m_fTimer <= 0) {
      changeMoveState(MS_NORMAL);
    } else {
      move(false);
    }
  } else if (m_uiCurrentMoveState == MS_RUN_START) {
    if (m_fTimer <= 0) {
      changeMoveState(MS_RUNNING);
    } else {
      mGoalDirection = mBodyNode->getOrientation().zAxis();

      move(false);
    }
  } else if (m_uiCurrentMoveState == MS_RUNNING) {
    mGoalDirection = mBodyNode->getOrientation().zAxis();

    move(true, posIncrementPerSecond, mGoalDirection);

    if (mCCPhysics->isStuck()) {
      bool bOneDestroyed = false;
      for (CWorldEntity *pEnt : mCCPhysics->getCollidingWorldEntities()) {
        if (this->mCCPerson->attack(CDamage(mCCPerson, DMG_RUN), pEnt)
            == CHitableInterface::RDR_ACCEPTED) {
          bOneDestroyed = true;
          break;
        }
      }

      if (!bOneDestroyed
          && mCCPhysics->getCollidingWorldEntities().size() > 0) {
        changeMoveState(MS_PUSHED_BACK, -mGoalDirection * 0.05, 1);
      }
    }
  } else if (m_uiCurrentMoveState >= MS_USER_STATE) {
    userUpdateCharacter(deltaTime);
  }

  // give the user the choice to deal with the current state,
  // e.g. change state if MS_NOT_MOVING
  postUpdateCharacter(deltaTime);


  if (m_uiCurrentMoveState == MS_MOVE_TO_POINT) {
    if (m_fTimer < 0) {
      targetReached();
    }
    DebugDrawer::getSingleton().drawSphere(getTargetPosition(),
                                           getTargetRadius(),
                                           Ogre::ColourValue::Blue,
                                           false);
  }
}

void CPersonController::updateCurrentMoveSpeed() {
  switch (m_uiCurrentMoveState) {
    case MS_RUNNING:
      m_fCurrentMoveSpeed = m_fRunSpeed;
      break;
    default:
      m_fCurrentMoveSpeed = m_fWalkSpeed;
      break;
  }
}

void CPersonController::updateGoalDirection() {
}

void CPersonController::changeMoveState(unsigned int uiNewMoveState,
                                        const Ogre::Vector3 &vUserData,
                                        const Ogre::Real fUserData0,
                                        const Ogre::Real fUserData1) {
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
  } else if (m_uiCurrentMoveState == MS_STUNNED) {
    m_fTimer = getStunnedTime();
  }
}

void CPersonController::move(bool bMove,
                             Ogre::Real fSpeed,
                             const Ogre::Vector3 &vDir) {
  const SAnimationProperty &prop(mCCPerson->getCurrentAnimationProperty());
  if (!prop.mAnimationData->mAllowMoving) {
    bMove = false;
  }

  if (bMove) {
    mCCPhysics->setWalkDirection(BtOgre::Convert::toBullet(
        vDir * fSpeed * WALK_SPEED_SCALE));
    // mCCPhysics->setSubSteps(std::max<int>(ceil(fSpeed / WALK_SPEED), 1));
  } else {
    mCCPhysics->setWalkDirection(btVector3(0, 0, 0));
    // mCCPhysics->setSubSteps(1);
  }
  mCCPerson->setIsMoving(bMove);
}

void CPersonController::moveToTarget(const Ogre::Vector3 &vPos,
                                     const Ogre::Real dRadius,
                                     const Ogre::Degree &maxDeviationLookDir,
                                     bool bAddCharCOMHeight,
                                     const Ogre::Real fMaxDuraion) {
  changeMoveState(MS_MOVE_TO_POINT,
                  vPos + ((bAddCharCOMHeight)
                          ? Ogre::Vector3(0, CPerson::PERSON_HEIGHT, 0)
                          : Ogre::Vector3::ZERO),
                  dRadius,
                  maxDeviationLookDir.valueRadians());
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
  changeMoveState(MS_NORMAL);  // reset our state

  CMessageHandler::getSingleton().addMessage(
      std::make_shared<CMessageTargetReached>(__MSG_LOCATION__, mCCPerson));
}

void CPersonController::requestingJumpSpeed(float *horizontal,
                                            float *vertical) {
  ASSERT(horizontal);
  ASSERT(vertical);

  switch (m_uiCurrentMoveState) {
    default:
      *horizontal = m_fCurrentMoveSpeed * WALK_SPEED_SCALE;
      // vertical speed default, so dont change
      break;
  }
}

void CPersonController::jumpStart() {
  if (m_uiCurrentMoveState == MS_RUNNING) {
    changeMoveState(MS_NORMAL);
  }
}

void CPersonController::jumpEnd() {
}
