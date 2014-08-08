#include "SimpleEnemyController.h"
#include "Person.h"
#include "Player.h"

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
