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
