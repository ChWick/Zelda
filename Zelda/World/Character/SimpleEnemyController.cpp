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

#include "SimpleEnemyController.hpp"
#include "SimpleEnemy.hpp"
#include "CharacterController_Physics.hpp"
#include "Person.hpp"
#include "Player.hpp"
#include <OgreMath.h>
#include <OgreAnimationState.h>

float test = 0.45;
float test2 = 2;
CSimpleEnemyController::CSimpleEnemyController(CPerson * ccPerson)
  : CPersonController(ccPerson), m_pPlayer(NULL) {
  changeMoveState(MS_USER_STATE);

  m_fMoveSpeed = 3;
}
void CSimpleEnemyController::start() {
  m_fTimeToNextAction = 2;
  m_vCurrentWalkDir = Ogre::Vector3::UNIT_X;
  changeMoveState(MS_NORMAL);
  m_eCurrentKIState = KI_PATROLING;
}

void CSimpleEnemyController::postUpdateCharacter(Ogre::Real tpf) {
  m_fTimeToNextAction -= tpf;
  if (dynamic_cast<CharacterControllerPhysics*>(mCCPhysics)->isStuck()) {
    m_vCurrentWalkDir = Ogre::Quaternion(Ogre::Degree(180), Ogre::Vector3::UNIT_Y) * m_vCurrentWalkDir;
  }
  if (m_fTimeToNextAction < 0) {
    if (m_eCurrentKIState == KI_PATROLING) {
      m_eCurrentKIState = KI_SCOUTING;
      m_fTimeToNextAction = mCCPerson->getAnimations()[CSimpleEnemy::SE_ANIM_SCOUT]->getLength();
    }
    else if (m_eCurrentKIState == KI_SCOUTING){
      m_eCurrentKIState = KI_PATROLING;
      
      m_vCurrentWalkDir = Ogre::Quaternion(Ogre::Degree((Ogre::Math::UnitRandom() > 0.5) ? 90 : -90), Ogre::Vector3::UNIT_Y) * m_vCurrentWalkDir;
      m_fTimeToNextAction = 2;
    }
  }
    /*Ogre::Real fDistSqr = m_pPlayer->getPosition().squaredDistance(mCCPerson->getPosition());

    if (fDistSqr < test * test) {
        if (m_uiCurrentMoveState == MS_NOT_MOVING || m_uiCurrentMoveState == MS_NORMAL) {
			// if player is in range attack him, cause he is evil! DIE, DIE, DIE oyu evil person, you never will rescue zelda, never!
			mCCPerson->animAttack();
		}
		else if (fDistSqr < test2 * test2) {
            // hunt him, if he's close enought?
            changeMoveState(MS_MOVE_TO_POINT, m_pPlayer->getPosition(), test, Ogre::Degree(10).valueRadians());
		}
	}
	else if (fDistSqr < test2 * test2) {

        // hunt him, if he's close enought?
        changeMoveState(MS_MOVE_TO_POINT, m_pPlayer->getPosition(), test, Ogre::Degree(10).valueRadians());
	}
	else {
        changeMoveState(MS_NOT_MOVING);
	}*/
}

void CSimpleEnemyController::updateGoalDirection() {
  if (m_eCurrentKIState == KI_PATROLING) {
    mGoalDirection = m_vCurrentWalkDir;
  }
  else if (m_eCurrentKIState == KI_SCOUTING){
    mGoalDirection = Ogre::Vector3::ZERO;
  }
}

void CSimpleEnemyController::userUpdateCharacter(const Ogre::Real tpf) {
	// no user state
	//changeMoveState(MS_MOVE_TO_POINT, m_pPlayer->getPosition(), test, Ogre::Degree(10).valueRadians());
}
