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

#include "PlayerController.hpp"
#include "Player.hpp"
#include "../../Common/Input/GameInputCommand.hpp"
#include <OgreCamera.h>

using namespace Ogre;


CPlayerController::CPlayerController(SceneManager * scnMgr, const Ogre::Camera *pCamera, CPlayer * ccPlayer)
	: CPersonController(ccPlayer),
		m_pCamera(pCamera) {
    mCCPerson = ccPlayer;
	mSceneManager = scnMgr;
	mKeyDirection = Vector3::ZERO;
}

CPlayerController::~CPlayerController()
{
}
btCharacterControllerInterface * CPlayerController::getCCPhysics()
{
	return mCCPhysics;
}
void CPlayerController::receiveInputCommand(const CGameInputCommand &cmd) {
  switch (cmd.getType()) {
	case GIC_RIGHT:
		mKeyDirection.x = cmd.getIntValue();
		break;
	case GIC_LEFT:
		mKeyDirection.x = -cmd.getIntValue();
		break;
	case GIC_FRONT:
		mKeyDirection.z = -cmd.getIntValue();
		break;
	case GIC_REAR:
		mKeyDirection.z = cmd.getIntValue();
		break;
  case GIC_INTERACT:
    if (cmd.getState() == GIS_PRESSED) {
      mCCPerson->interact();
    }
    break;
  case GIC_RUN:
    if (cmd.getState() == GIS_PRESSED) {
      startRunning();
    }
    else if (cmd.getState() == GIS_RELEASED) {
      endRunning();
    }
    break;
  default:
    break;
	}
}
/*
bool CPlayerController::keyPressed(const OIS::KeyEvent & evt)
{

	switch (evt.key)
	{
	case OIS::KC_ADD:
		mCCPerson->changeHP(+25);
		break;
	case OIS::KC_SUBTRACT:
		mCCPerson->changeHP(-25);
		break;
		case OIS::KC_W:
			mKeyDirection.z = -1;
			break;

		case OIS::KC_S:
			mKeyDirection.z = 1;
			break;

		case OIS::KC_A:
			mKeyDirection.x = -1;
			break;

		case OIS::KC_D:
			mKeyDirection.x = 1;
			break;

		case OIS::KC_SPACE:
			if (mCCPhysics->canJump())
			{
				mCCPhysics->jump();
				mCCPerson->animJumpStart();

				mJumped = true;
			}

			break;

		case OIS::KC_E:
			mCCPerson->interact(CObject::IT_USE);
			break;

		case OIS::KC_LCONTROL:
			//mCCPhysics->duck();
			break;
        case OIS::KC_Q:
            mCCPerson->animGrabWeapons(m_Tools.getCurrentTool().getToolType());
            break;
        default:
			break;
	}
	return true;
}

bool CPlayerController::keyReleased(const OIS::KeyEvent & evt)
{
	switch (evt.key)
	{
		case OIS::KC_W:
		case OIS::KC_S:
			mKeyDirection.z = 0;
			break;

		case OIS::KC_A:
		case OIS::KC_D:
			mKeyDirection.x = 0;
			break;

		case OIS::KC_LCONTROL:
			//mCCPhysics->stand();
			break;
        default:
            break;
	}
	return true;
}

bool CPlayerController::mouseMoved(const OIS::MouseEvent & evt) {
    if (evt.state.Z.rel > 0) {
        selectNextTool();
    }
    else if (evt.state.Z.rel < 0) {
        selectPreviousTool();
    }
	return true;
}
bool CPlayerController::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id ) {
    if (id == OIS::MB_Left) {
        mCCPerson->animAttack();
    }
    else if (id == OIS::MB_Right) {
        mCCPerson->animStartBlock();
    }
	return true;
}
bool CPlayerController::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id ) {
    if (id == OIS::MB_Right) {
        mCCPerson->animEndBlock();
    }
	return true;
}
*/
void CPlayerController::updateCharacter(const Real deltaTime)
{
	CPersonController::updateCharacter(deltaTime);
}
void CPlayerController::updateGoalDirection() {
	mGoalDirection = Ogre::Vector3::ZERO;

	if (mKeyDirection != Vector3::ZERO)
	{
		// calculate actuall goal direction in world based on player's key directions
		//mGoalDirection += mKeyDirection.z * m_pCamera->getOrientation().zAxis();
		//mGoalDirection += mKeyDirection.x * m_pCamera->getOrientation().xAxis();
    mGoalDirection = mKeyDirection;
		mGoalDirection.y = 0;
		mGoalDirection.normalise();
	}
}
void CPlayerController::postUpdateCharacter() {
	if (m_uiCurrentMoveState == MS_NOT_MOVING) {
		changeMoveState(MS_NORMAL);
	}
}
void CPlayerController::interactionLockedChanged(bool bActivate) {
	if (!bActivate) {
		setGameInputListenerEnabled(true);
	}
	else {
		setGameInputListenerEnabled(false);
	}
}
