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
#include "Person.hpp"
#include "Player.hpp"

float test = 0.45;
float test2 = 2;
void CSimpleEnemyController::postUpdateCharacter() {
    Ogre::Real fDistSqr = m_pPlayer->getPosition().squaredDistance(mCCPerson->getPosition());

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
	}
}
void CSimpleEnemyController::userUpdateCharacter(const Ogre::Real tpf) {
	// no user state
	//changeMoveState(MS_MOVE_TO_POINT, m_pPlayer->getPosition(), test, Ogre::Degree(10).valueRadians());
}
