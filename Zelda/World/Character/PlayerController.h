#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include "PersonController.h"
#include "../../Common/Input/GameInputListener.hpp"
#include "OgreVector3.h"
#include "OgreSceneNode.h"
#include "CharacterController_Physics.h"

class CPlayer;
class CCameraController;

using namespace Ogre;

class CPlayerController : public CPersonController, public CGameInputListener {
private:
	SceneManager * mSceneManager;
	CCameraController *m_pCameraController;

	Vector3 mKeyDirection;
public:
    CPlayerController(SceneManager * scnMgr, CCameraController *pCameraController, CPlayer * ccPlayer);
    /** Default destructor */
    virtual ~CPlayerController();
	btCharacterControllerInterface * getCCPhysics();

  virtual void receiveInputCommand(const CGameInputCommand &cmd);

	void updateCharacter(const Real dt);

	void interactionLockedChanged(bool bActivate);

protected:

	virtual void updateGoalDirection();
	virtual void postUpdateCharacter();

private:
	Quaternion updateOrientation(Real deltaTime);
};

#endif // PLAYERCONTROLLER_H
