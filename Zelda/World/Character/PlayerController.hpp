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

#include "PersonController.hpp"
#include "../../Common/Input/GameInputListener.hpp"
#include "OgreVector3.h"
#include "OgreSceneNode.h"
#include "CharacterController_Physics.hpp"

class CPlayer;
class CCameraController;

using namespace Ogre;

class CPlayerController : public CPersonController, public CGameInputListener {
private:
	SceneManager * mSceneManager;
	const Ogre::Camera *m_pCamera;

	Vector3 mKeyDirection;
public:
  CPlayerController(SceneManager * scnMgr, const Ogre::Camera *pCamera, CPlayer * ccPlayer);
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
