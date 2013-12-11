#include "StdAfx.h"
#include "PlayerController.h"
#include "Player.h"
#include "InputListener.h"
#include "CameraController.h"

using namespace Ogre;


CPlayerController::CPlayerController(SceneManager * scnMgr, CCameraController *pCameraController, CPlayer * ccPlayer)
	: CPersonController(ccPlayer), m_pCameraController(pCameraController) {
	assert(pCameraController);
    mCCPerson = ccPlayer;
	mSceneManager = scnMgr;
	mKeyDirection = Vector3::ZERO;

	CInputListenerManager::getSingleton().addInputListener(this);
	CGameState::getSingleton().addCallback(this);
}

CPlayerController::~CPlayerController()
{

	CInputListenerManager::getSingleton().addInputListener(this);
	CGameState::getSingleton().removeCallback(this);
}
btCharacterControllerInterface * CPlayerController::getCCPhysics()
{
	return mCCPhysics;
}


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
void CPlayerController::updateCharacter(const Real deltaTime)
{
	CPersonController::updateCharacter(deltaTime);
}
void CPlayerController::updateGoalDirection() {
	mGoalDirection = Ogre::Vector3::ZERO;

	if (mKeyDirection != Vector3::ZERO)
	{
		// calculate actuall goal direction in world based on player's key directions
		mGoalDirection += mKeyDirection.z * m_pCameraController->getCameraNode()->getOrientation().zAxis();
		mGoalDirection += mKeyDirection.x * m_pCameraController->getCameraNode()->getOrientation().xAxis();
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
		setInputListenerEnabled(true);
	}
	else {
		setInputListenerEnabled(false);
	}
}
