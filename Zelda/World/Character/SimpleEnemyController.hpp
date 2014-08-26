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

class CPlayer;

class CSimpleEnemyController : public CPersonController {
private:
	CPlayer *m_pPlayer;			// the player, needed for KI calculations
public:
	CSimpleEnemyController(CPerson * ccPerson)
		: CPersonController(ccPerson), m_pPlayer(NULL) {
		changeMoveState(MS_USER_STATE);
	}
	void setPlayer(CPlayer *pPlayer) {assert(pPlayer); m_pPlayer = pPlayer;}
protected:
	void userUpdateCharacter(const Ogre::Real tpf);
	void postUpdateCharacter();
};

#endif