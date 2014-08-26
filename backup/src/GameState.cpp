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

#include "StdAfx.h"
#include "GameState.h"


template<> CGameState* Ogre::Singleton<CGameState>::msSingleton = 0;

CGameState* CGameState::getSingletonPtr(void)
{
    return msSingleton;
}
CGameState& CGameState::getSingleton(void)
{
    assert( msSingleton );  return ( *msSingleton );
}
CGameState::CGameState() {
	reset();
}
void CGameState::reset() {
	m_uiTextBoxActive = 0;
	m_uiInteractionLocked = 0;
	m_lCallbacks.clear();
}
void CGameState::setTextBoxActivate(bool bActivate) {
	// text box will automatically lock interaction
	setInteractionLocked(bActivate);
	if (bActivate) {
		m_uiTextBoxActive++;
		if (m_uiTextBoxActive == 1) {
			for (auto cb : m_lCallbacks) {
				cb->textBoxActivationChanged(true);
			}
		}
	}
	else {
		m_uiTextBoxActive--;
		assert(m_uiTextBoxActive >= 0);
		if (m_uiTextBoxActive == 0) {
			for (auto cb : m_lCallbacks) {
				cb->textBoxActivationChanged(false);
			}
		}
	}
}
void CGameState::setInteractionLocked(bool bLock) {
	if (bLock) {
		m_uiInteractionLocked++;
		if (m_uiInteractionLocked == 1) {
			for (auto cb : m_lCallbacks) {
				cb->interactionLockedChanged(true);
			}
		}
	}
	else {
		m_uiInteractionLocked--;
		assert(m_uiInteractionLocked >= 0);
		if (m_uiInteractionLocked == 0) {
			for (auto cb : m_lCallbacks) {
				cb->interactionLockedChanged(false);
			}
		}
	}
}