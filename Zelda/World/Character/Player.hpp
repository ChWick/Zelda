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
#include "../../Common/GameLogic/EntityProperty.hpp"
#include "../../Common/PauseManager/PauseListener.hpp"
#include <OgrePrerequisites.h>

class CMap;

class CPlayer : public CPerson,
                public CPauseListener {
private:
  enum EDefaultAnimationIDs {
    P_ANIM_IDLE,
    P_ANIM_WALK,
    P_ANIM_SLICE_HORIZONTAL,
    P_ANIM_USE_ITEM,
    P_ANIM_RUN,
    P_ANIM_SLEEP,
    P_ANIM_AWAKE,

    P_ANIM_COUNT
  };

  const Ogre::Camera *m_pCamera;
  Ogre::SceneManager *m_pPlayerSceneManager;
  CWorldEntity *m_pLiftedEntity;
  
  Ogre::RibbonTrail* mSwordTrail;

  // properties, that notyfy world when updated
  CEntityBoundedIntProperty mRupeeCount;
public:

  CPlayer(CAbstractWorldEntity *pParent, const Ogre::Camera *pCamera, Ogre::SceneManager *pPlayerSceneManager);
  virtual ~CPlayer();
  
  void update(Ogre::Real tpf);
  void renderDebug(Ogre::Real tpf);
  void startup(const Ogre::Vector3 &playerPos, const Ogre::Vector3 &playerLookDirection, const Ogre::Real cameraYaw, const Ogre::Real cameraPitch);
  void loadSaveFile();
protected:
  void enterMap(CMap *pMap, const Ogre::Vector3 &vInitPosition);
  void setupInternal();
  EReceiveDamageResult receiveDamage(const CDamage &dmg);
  void setupBody(Ogre::SceneManager* sceneMgr);
  void setupCamera(Ogre::Camera* cam);
  void preUpdateBoundsCallback(const Ogre::Real fTime);
  void updateWalkAnimation();
  void updateLiftedObject(const Ogre::Real fTime);
  void updateAnimationsCallback(const Ogre::Real fTime);
  void interact();
  void lift(CWorldEntity *pObjectToLift);
  void throwLifted();
  void postStepForwardAndStrafe();
  
  virtual CCharacterController *createCharacterController();
  
private:
  void handleMessage(const CMessagePtr message);

  void updatePause(int iPauseType, bool bPause);
};

#endif // _PLAYER_HPP_
