#ifndef _CAMERA_CONTROLLER_H_
#define _CAMERA_CONTROLLER_H_

#include "StdAfx.h"
#include "InputListener.h"
#include "GameState.h"


class CCharacter;
class CMap;

//! This calls will controll the camera
/** There are several cameras, camera fixed at a point, moving in a physics sphere (controllabe by player), free, ...
  * Inherits InputListener to handle user input
  */
class CCameraController : public CInputListener, public CGameStateCallback {
public:
	//! Definition of the different move states
	enum ECameraMoveType {
		CMT_FREE,
		CMT_3RD_PERSON,
		CMT_ATTACK_VIEW,
		CMT_FIXED,
	};
private:
	CCharacter *m_pCharacter;			//!< Assotiation to a character (e.g. the player)
	CCharacter *m_pLookAtCharacter;		//!< An other character to look at (e.g. the player)
	bool m_bLookAtCharacter;			//!< Force the camera to look a the player
	ECameraMoveType m_eCameraMoveType;	//!< Current move type of the camera
	CMap *m_pMap;						//!< Current map (physics etc...)

	bool m_bTargetReached;				//!< Smooth change to a new target, no jump!

	// move to target data
	bool m_bMoveToTarget;
	Ogre::Vector3 m_vTargetPosition;
	Ogre::Vector3 m_vTargetLookAtPoint;

	// camera setup
	btRigidBody *m_pCameraPhysics;		//!< The physics around the camera
	Ogre::Camera * mCamera;					//!< The camera to controll
	Ogre::SceneNode * m_pParentSceneNode;	//!< Not where to add the created noted
	Ogre::SceneNode * mCameraPivot;			//!<
	Ogre::SceneNode * mCameraGoal;
	Ogre::SceneNode * mCameraNode;
	Ogre::Real mPivotPitch;
public:
	CCameraController(Ogre::Camera *pCam, Ogre::SceneNode *pParentSceneNode);
	~CCameraController();

	virtual bool keyPressed( const OIS::KeyEvent &arg );
    virtual bool keyReleased( const OIS::KeyEvent &arg );
    virtual bool mouseMoved( const OIS::MouseEvent &arg );
    virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
    virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

	void setCameraPosition( Ogre::Real pivotYaw, Ogre::Real pivotPitch );

	const Ogre::SceneNode *getCameraNode() const {return mCameraNode;}

	void enterMap(CMap *pMap);
	void leaveMap();

	void setCharacter(CCharacter *pCharacter);
	CCharacter *getCharacter() const {return m_pCharacter;}

	void setLookAtCharacter(CCharacter *pLookAtCharacter, bool bLookAtCharacter = true);
	bool isLookAtCharacter() const {return m_bLookAtCharacter;}

	CCharacter *getLookAtCharacter() const {return m_pLookAtCharacter;}

	void setCameraMoveType(ECameraMoveType eCMT);
	ECameraMoveType getCameraMoveType() const {return m_eCameraMoveType;}

	void moveCameraToTarget(const Ogre::Vector3 &vTargetPosition, const Ogre::Vector3 &vTargetLookAt);

	void updateCamera(Ogre::Real deltaTime);

	void interactionLockedChanged(bool bActivate);

private:
	void setupCamera();
	void updateCameraGoal(Ogre::Real deltaYaw, Ogre::Real deltaPitch, Ogre::Real deltaZoom);
	void updateCameraLookDirection(Ogre::Real deltaTime, const Ogre::Vector3 &vGoalDirection);
};


#endif
