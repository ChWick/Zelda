#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include "PersonController.h"
#include "InputListener.h"
#include "OgreVector3.h"
#include "OgreSceneNode.h"
#include "CharacterController_Physics.h"
#include "GameState.h"

class CPlayer;
class CCameraController;

using namespace Ogre;

class CPlayerController : public CPersonController, public CInputListener, public CGameStateCallback {
private:
	SceneManager * mSceneManager;
	CCameraController *m_pCameraController;

	Vector3 mKeyDirection;
public:
    CPlayerController(SceneManager * scnMgr, CCameraController *pCameraController, CPlayer * ccPlayer);
    /** Default destructor */
    virtual ~CPlayerController();
	btCharacterControllerInterface * getCCPhysics();

    virtual bool keyPressed( const OIS::KeyEvent &arg );
    virtual bool keyReleased( const OIS::KeyEvent &arg );
    virtual bool mouseMoved( const OIS::MouseEvent &arg );
    virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
    virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

	void updateCharacter(const Real dt);

	void interactionLockedChanged(bool bActivate);

protected:

	virtual void updateGoalDirection();
	virtual void postUpdateCharacter();

private:
	Quaternion updateOrientation(Real deltaTime);
};

#endif // PLAYERCONTROLLER_H
