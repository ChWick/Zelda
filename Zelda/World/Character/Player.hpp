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

#ifndef _PLAYER_HPP_
#define _PLAYER_HPP_

#include "Person.hpp"
#include <OgrePrerequisites.h>

class CMap;

using namespace Ogre;

class CPlayer : public CPerson {
private:
	const Ogre::Camera *m_pCamera;
	Ogre::SceneManager *m_pPlayerSceneManager;
  CWorldEntity *m_pLiftedEntity;

	RibbonTrail* mSwordTrail;
public:

	CPlayer(CEntity *pParent, const Ogre::Camera *pCamera, Ogre::SceneManager *pPlayerSceneManager);
	virtual ~CPlayer();
	void destroy();
	void update(Ogre::Real tpf);
  void renderDebug(Ogre::Real tpf);
	void startup(const Ogre::Vector3 &playerPos, const Ogre::Vector3 &playerLookDirection, const Ogre::Real cameraYaw, const Ogre::Real cameraPitch);
protected:
  void enterMap(CMap *pMap, const Ogre::Vector3 &vInitPosition);
	void setupInternal();
  void setupAnimations();
	void hitpointsChangedCallback();
	void maxHitpointsChangedCallback();
	void setupBody(SceneManager* sceneMgr);
	void setupCamera(Camera* cam);
	void preUpdateBoundsCallback(const Ogre::Real fTime);
	void updateWalkAnimation();
	void updateLiftedObject(const Ogre::Real fTime);
  void interact();
  void lift(CWorldEntity *pObjectToLift);
  void throwLifted();

	virtual CCharacterController *createCharacterController();
};

#endif // _PLAYER_HPP_
