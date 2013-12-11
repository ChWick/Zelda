#include "StdAfx.h"
#include "CameraController.h"
#include "Character.h"
#include "Util.h"
#include "Map.h"
#include "PhysicsManager.h"
#include "GameState.h"

const Ogre::Vector3 CAM_GOAL_OFFSET(0, 0, 5);
const Ogre::Real CAM_RADIUS = 0.5f;
const Ogre::Real CAM_HEIGHT = 0.40f;

// constants for smooth look at position
const Ogre::Real CAM_TURN_SPEED = 10.f;
const Ogre::Real CAM_DEFAULT_TURN_SPEED_FACTOR = 2.0f;

// constants for move to target
const Ogre::Real CAM_MOVE_TO_TARGET_FACTOR = 5.5f;
const Ogre::Real CAM_DEFAULT_MOVE_TO_TARGET_FACTOR = 2.0f;

// constants for bounding the camera
const Ogre::Real CAM_MAX_PITCH = 25.0f;
const Ogre::Real CAM_MIN_PITCH = -60.0f;
const Ogre::Real CAM_MAX_DISTANCE = 4.0f;
const Ogre::Real CAM_MIN_DISTANCE = 0.5f;

CCameraController::CCameraController(Ogre::Camera *pCam, Ogre::SceneNode *pParentSceneNode)
	: mCamera(pCam), m_pParentSceneNode(pParentSceneNode),mCameraPivot(NULL), m_pLookAtCharacter(0), mCameraGoal(NULL), mCameraNode(NULL), mPivotPitch(0), m_pCharacter(NULL), m_pCameraPhysics(NULL) {
	m_pMap = NULL;
	m_bTargetReached = false;
	m_bMoveToTarget = false;
	assert(pCam);
	assert(pParentSceneNode);

	CInputListenerManager::getSingleton().addInputListener(this);

	setupCamera();
}
CCameraController::~CCameraController() {
	CInputListenerManager::getSingleton().removeInputListener(this);

	if (m_pMap) {
		leaveMap();
	}

	// detach camera so that the camera will not be destroyed on deleting the camera controller
	mCamera->detachFromParent();
	// but delete all scene nodes
	destroySceneNode(mCameraGoal);
	destroySceneNode(mCameraPivot); // goal is attached to pivot, delete in this order, or dont delete goal
	destroySceneNode(mCameraNode);
}
bool CCameraController::keyPressed( const OIS::KeyEvent &arg ) {
	return true;
}
bool CCameraController::keyReleased( const OIS::KeyEvent &arg ) {
	return true;
}
bool CCameraController::mouseMoved( const OIS::MouseEvent &evt ) {
	// update camera goal based on mouse movement
	updateCameraGoal(-0.5f * evt.state.X.rel, -0.5f * evt.state.Y.rel, -0.0005f * evt.state.Z.rel);
	return true;
}
bool CCameraController::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id ) {
	return true;
}
bool CCameraController::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id ) {
	return true;
}
void CCameraController::updateCamera(Ogre::Real deltaTime) {
	using namespace Ogre;

    if (m_bMoveToTarget) {
        Ogre::Vector3 vTargetDirection(m_vTargetLookAtPoint - m_vTargetPosition);
        const Ogre::Real targetDistance = vTargetDirection.normalise();

        Ogre::Vector3 vDirToGoal(m_vTargetPosition - mCameraNode->getPosition());
        const Ogre::Real fDistToGoal(vDirToGoal.normalise());

        const Ogre::Real speed = deltaTime * CAM_MOVE_TO_TARGET_FACTOR * (fDistToGoal + CAM_DEFAULT_MOVE_TO_TARGET_FACTOR);

        const Ogre::Real stepInDirection = (fDistToGoal > speed) ? speed : fDistToGoal;

        mCameraNode->translate(vDirToGoal * stepInDirection);



        Ogre::Vector3 vCurrentDirection(m_vTargetLookAtPoint - mCameraNode->getPosition());

        updateCameraLookDirection(deltaTime, vCurrentDirection);

        if (stepInDirection < 0.01 && m_bTargetReached) {
            if (!CGameState::getSingleton().isInteractionLocked()) {
                m_bTargetReached = false;
                m_bMoveToTarget = false;
                if (m_pCameraPhysics) {
                    m_pCameraPhysics->setLinearFactor(btVector3(1, 1, 1));
                }
                mCameraGoal->setPosition(mCameraPivot->convertWorldToLocalPosition(mCameraNode->getPosition()));
            }
        }
        else if (m_pCameraPhysics) {
            m_pCameraPhysics->setLinearFactor(btVector3(0, 0, 0));
            m_pCameraPhysics->setLinearVelocity(btVector3(0, 0, 0));
        }
    }
    else {
        if(!m_pCharacter) {return;}

        mCameraPivot->setPosition(m_pCharacter->getPosition() + Vector3::UNIT_Y * CAM_HEIGHT);

        Vector3 goalOffset(Ogre::Vector3::ZERO);
        //if (m_eCameraMoveType == CMT_3RD_PERSON) {
            goalOffset = mCameraGoal->_getDerivedPosition() - mCameraNode->getPosition();
        /*}
        else if (m_eCameraMoveType == CMT_ATTACK_VIEW) {
            goalOffset = mCameraPivot->_getDerivedPosition() - mCameraNode->getPosition();
        }*/
        if (m_pCameraPhysics) {
            m_pCameraPhysics->activate();
            m_pCameraPhysics->setLinearVelocity(BtOgre::Convert::toBullet(goalOffset) * 5);
        }
        else {
            mCameraNode->translate(goalOffset * deltaTime * 9);
        }

        Ogre::Vector3 vGoalDirection(-mCameraNode->getOrientation().zAxis());
        if (m_bLookAtCharacter) {
            vGoalDirection = (m_pLookAtCharacter->getPosition() - mCameraNode->getPosition() + Vector3::UNIT_Y * CAM_HEIGHT);

            // Test implentation to avoid the camera not seeing the player
            Ogre::Vector3 startPos(mCameraNode->getPosition());
            Ogre::Vector3 endPos(mCameraPivot->getPosition());

            btCollisionWorld::ClosestRayResultCallback rayCallback(BtOgre::Convert::toBullet(startPos), BtOgre::Convert::toBullet(endPos));
            rayCallback.m_collisionFilterGroup = COL_CAMERA;
            rayCallback.m_collisionFilterMask = MASK_CAMERA_COLLIDES_WITH;
            m_pMap->getPhysicsManager()->getWorld()->rayTest(BtOgre::Convert::toBullet(startPos), BtOgre::Convert::toBullet(endPos), rayCallback);
            //DebugDrawer::getSingleton().drawLine(startPos, endPos, Ogre::ColourValue::Red);
            if (rayCallback.hasHit()) {
                // '-' because of negativ z axis is look direction of camera
                m_pCameraPhysics->getWorldTransform().setOrigin(rayCallback.m_hitPointWorld - BtOgre::Convert::toBullet(mCameraNode->getOrientation().zAxis() * CAM_RADIUS));
                mCameraGoal->setPosition(0, 0, m_pCameraPhysics->getWorldTransform().getOrigin().distance(BtOgre::Convert::toBullet(mCameraGoal->_getDerivedPosition())));
            }
        }

        updateCameraLookDirection(deltaTime, vGoalDirection);





    }


	if (m_pCameraPhysics) {
		m_pCameraPhysics->getWorldTransform().setRotation(BtOgre::Convert::toBullet(mCameraNode->getOrientation()));
		m_pCameraPhysics->getWorldTransform().setOrigin(BtOgre::Convert::toBullet(mCameraNode->getPosition()));
	}
}
void CCameraController::updateCameraGoal(Ogre::Real deltaYaw, Ogre::Real deltaPitch, Ogre::Real deltaZoom)
{
	using namespace Ogre;

	if (m_eCameraMoveType == CMT_FIXED) {
	}
	else if (m_eCameraMoveType == CMT_3RD_PERSON || m_eCameraMoveType == CMT_ATTACK_VIEW) {
		mCameraPivot->yaw(Degree(deltaYaw), Node::TS_WORLD);

		// bound the pitch
		if (!(mPivotPitch + deltaPitch > CAM_MAX_PITCH && deltaPitch > 0) &&
			!(mPivotPitch + deltaPitch < CAM_MIN_PITCH && deltaPitch < 0))
		{
			mCameraPivot->pitch(Degree(deltaPitch), Node::TS_LOCAL);
			mPivotPitch += deltaPitch;
		}

		Real dist = mCameraGoal->_getDerivedPosition().distance(mCameraPivot->_getDerivedPosition());
		Real distChange = deltaZoom * dist;

		// bound the zoom
		if (!(dist + distChange < CAM_MIN_DISTANCE && distChange < 0) &&
			!(dist + distChange > CAM_MAX_DISTANCE && distChange > 0))
		{
			mCameraGoal->translate(0, 0, distChange, Node::TS_LOCAL);
		}
	}
}
void CCameraController::setupCamera()
{
	mCameraPivot = m_pParentSceneNode->createChildSceneNode("CameraPivot");

	mCameraGoal = mCameraPivot->createChildSceneNode("CameraGoal", CAM_GOAL_OFFSET);

	mCameraNode = m_pParentSceneNode->createChildSceneNode("CameraNode");
	mCameraNode->setPosition(mCameraPivot->getPosition() + mCameraGoal->getPosition());

	mCameraPivot->setFixedYawAxis(true);
	mCameraGoal->setFixedYawAxis(true);
	mCameraNode->setFixedYawAxis(true);

	mCameraNode->attachObject(mCamera);

	mPivotPitch = 0;
}

void CCameraController::setCameraPosition(Ogre::Real pivotYaw, Ogre::Real pivotPitch) {
	assert(m_pCharacter);

	mCameraPivot->setPosition(m_pCharacter->getPosition() + Ogre::Vector3::UNIT_Y * CAM_HEIGHT);
	Ogre::Quaternion q;
	q.FromAngleAxis(Ogre::Radian(pivotYaw), Ogre::Vector3::UNIT_Y);
	mCameraPivot->setOrientation(q);
	mCameraPivot->pitch(Ogre::Radian(pivotPitch), Ogre::Node::TS_LOCAL);
	//mCameraGoal->setPosition(q * CAM_GOAL_OFFSET);
	mPivotPitch = Ogre::Radian(pivotPitch).valueDegrees();
	//mCameraGoal->setPosition(0, 0, 0);
	mCameraNode->setPosition(mCameraPivot->getPosition() + mCameraGoal->getPosition());

	m_bTargetReached = true;
	m_bMoveToTarget = false;
}
void CCameraController::enterMap(CMap *pMap) {
	assert(pMap);
	assert(!m_pMap);

	m_pMap = pMap;

	// create physics for the camera
	btCollisionShape *pColShape = new btSphereShape(CAM_RADIUS);
	m_pCameraPhysics = new btRigidBody(1, new BtOgre::RigidBodyState(mCameraNode), pColShape);
	m_pCameraPhysics->setGravity(btVector3(0, 0, 0));
	//m_pCameraPhysics->setLinearFactor(btVector3(0, 0, 0));
	m_pCameraPhysics->setAngularFactor(0);
	m_pCameraPhysics->setDamping(0.5f, 0);

	m_pMap->getPhysicsManager()->getWorld()->addRigidBody(m_pCameraPhysics, COL_CAMERA, MASK_CAMERA_COLLIDES_WITH);
}
void CCameraController::leaveMap() {
	if (!m_pMap) {return;} // can happen on a crash (missing files)

	m_pMap->getPhysicsManager()->getWorld()->removeRigidBody(m_pCameraPhysics);
	m_pMap = 0;
	delete m_pCameraPhysics->getCollisionShape();
	delete m_pCameraPhysics->getMotionState();
	delete m_pCameraPhysics;

	m_pCameraPhysics = NULL;
}
void CCameraController::setCharacter(CCharacter *pCharacter) {
	if (m_pCharacter != pCharacter) {
		m_pCharacter = pCharacter;
		m_bTargetReached = false;
	}
}
void CCameraController::setLookAtCharacter(CCharacter *pLookAtCharacter, bool bLookAtCharacter) {
	m_bLookAtCharacter = bLookAtCharacter;
	if (m_pLookAtCharacter != pLookAtCharacter) {
		m_bTargetReached = false; // smoothly change to this target
		m_pLookAtCharacter = pLookAtCharacter;
	}
}
void CCameraController::setCameraMoveType(ECameraMoveType eCMT) {
	m_eCameraMoveType = eCMT;
}
void CCameraController::updateCameraLookDirection(Ogre::Real deltaTime, const Ogre::Vector3 &vGoalDirection) {
	using namespace Ogre;

	if (m_bTargetReached) {
		mCameraNode->lookAt(vGoalDirection + mCameraNode->getPosition(), Ogre::Node::TS_WORLD);
	}
	else {
		Vector3 deltaDirection = vGoalDirection.normalisedCopy() + mCameraNode->getOrientation().zAxis();
		Real length = deltaDirection.normalise();
		Real speed = CAM_TURN_SPEED * deltaTime * (length + CAM_DEFAULT_TURN_SPEED_FACTOR);
		if (speed > length) {
			m_bTargetReached = true; // Target reached
			speed = length;
		}
		Vector3 newDirection = -mCameraNode->getOrientation().zAxis() + deltaDirection * speed;



		mCameraNode->lookAt(newDirection + mCameraNode->getPosition(), Ogre::Node::TS_WORLD);
	}
}
void CCameraController::interactionLockedChanged(bool bActivate) {
	if (!bActivate) {
		setInputListenerEnabled(true);
	}
	else {
		setInputListenerEnabled(false);
	}
}
void CCameraController::moveCameraToTarget(const Ogre::Vector3 &vTargetPosition, const Ogre::Vector3 &vTargetLookAt) {
	m_bMoveToTarget = true;
	m_bTargetReached = false;
	m_vTargetPosition = vTargetPosition;
	m_vTargetLookAtPoint = vTargetLookAt;
}
