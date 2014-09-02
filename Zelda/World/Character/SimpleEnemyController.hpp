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

#ifndef _SIMPLE_ENEMY_CONTROLLER_H_
#define _SIMPLE_ENEMY_CONTROLLER_H_

#include "PersonController.hpp"

class CWorldEntity;

class CSimpleEnemyController : public CPersonController {
public:
  enum EKIState {
    KI_SCOUTING,
    KI_PATROLING,
    KI_WALK_TO_PLAYER,
  };
private:

	CWorldEntity *m_pPlayer;			// the player, needed for KI calculations
  Ogre::Vector3 m_vCurrentWalkDir;
  Ogre::Real m_fTimeToNextAction;
  EKIState m_eCurrentKIState;

public:
	CSimpleEnemyController(CPerson * ccPerson);
	void setPlayer(CWorldEntity *pPlayer) {assert(pPlayer); m_pPlayer = pPlayer;}
  void start();
  EKIState getCurrentKIState() const {return m_eCurrentKIState;}
protected:
  void updateGoalDirection();
	void userUpdateCharacter(const Ogre::Real tpf);
	void postUpdateCharacter(Ogre::Real tpf);
  bool notifiedByPlayer();
};

#endif