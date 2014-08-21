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

#ifndef _GAME_STATE_H_
#define _GAME_STATE_H_

#include "StdAfx.h"


class CGameStateCallback {
public:
	virtual void textBoxActivationChanged(bool) {}
	virtual void interactionLockedChanged(bool) {}
};

class CGameState : public Ogre::Singleton<CGameState> {
private:
	unsigned int m_uiTextBoxActive;
	unsigned int m_uiInteractionLocked;

	std::list<CGameStateCallback*> m_lCallbacks;
	
public:
    static CGameState& getSingleton(void);
    static CGameState* getSingletonPtr(void);

	CGameState();
	void reset();

	void addCallback(CGameStateCallback *pCallback) {
		assert(!hasCallback(pCallback));
		m_lCallbacks.push_back(pCallback);
	}
	void removeCallback(CGameStateCallback *pCallback) {
		assert(hasCallback(pCallback));
		m_lCallbacks.remove(pCallback);
	}
	bool hasCallback(CGameStateCallback *pCallback) {
		for (auto l : m_lCallbacks) {
			if (l == pCallback) {
				return true;
			}
		}
		return false;
	}

	bool isTextboxActive() const {return m_uiTextBoxActive > 0;}

	bool isInteractionLocked() const {return m_uiInteractionLocked > 0;}

private:
	void setTextBoxActivate(bool bActivate);
	void setInteractionLocked(bool bLock);

	friend class CGameStateChanger;
};

//! This class will take care of changes in a game state
/**
  * since it is possible, that several parst disable e.g. interaction CGameState will count the number of locks.
  * but to prevent two calls of the same source that would remove e.g. 2 locks, this class takes care if the state of this source changed,
  * so that only one lock is removed not 2 after 2 calls of unlockInteraction() e.g.
  * default state: all variables are false
  */
class CGameStateChanger {
private:
	bool m_bTextBoxActive;				//!< State of the text box
	bool m_bInteractionLocked;			//!< State of interaction of the player
protected:
	CGameStateChanger() {
		m_bTextBoxActive = false;
		m_bInteractionLocked = false;
	}

	void lockInteraction() {
		if (!m_bInteractionLocked) {
			m_bInteractionLocked = true;
			CGameState::getSingleton().setInteractionLocked(true);
		}
	}
	void unlockInteraction() {
		if (m_bInteractionLocked) {
			m_bInteractionLocked = false;
			CGameState::getSingleton().setInteractionLocked(false);
		}
	}
	void activateTextBox() {
		if (!m_bTextBoxActive) {
			m_bTextBoxActive = true;
			CGameState::getSingleton().setTextBoxActivate(true);
		}
	}
	void deactivateTextBox() {
		if (m_bTextBoxActive) {
			m_bTextBoxActive = false;
			CGameState::getSingleton().setTextBoxActivate(false);
		}
	}
};

#endif