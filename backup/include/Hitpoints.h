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

#ifndef _HITPOINTS_H_
#define _HITPOINTS_H_

typedef int Hitpoints;
enum EHitpointsConstants {
	HP_ONE_HEART	= 100,
	HP_HALF_HEART	= 50,
	HP_TWO_HEARTS   = HP_ONE_HEART * 2,
	HP_FULL_HEART	= 100 * 20,
	//HP_INFINITY     = std::numeric_limits<Hitpoints>::max(),
	HP_INFINITY = INT_MAX,
};

#define DEFAULT_INVULNERABLE_TIME 0.7f							//!< default time for being invulnerable


class CHitpointImplementation {
private:
	Hitpoints m_uiMaxHP;								//!< maximum of hit points (in hear percentage)
	Hitpoints m_uiCurrentHP;							//!< current hit points (in heart percentage)
	Ogre::Real m_fInvulnerableTimer;					//!< time to be invulnerable
	bool m_bInvulnerable;								//!< invulnerable?

public:
	CHitpointImplementation()
		: m_uiMaxHP(0), m_uiCurrentHP(0), m_fInvulnerableTimer(0), m_bInvulnerable(false) {
	}

	Hitpoints getMaxHP() const {return m_uiMaxHP;}
	Hitpoints getCurrentHP() const {return m_uiCurrentHP;}

	bool isInvulnerable() const {return m_bInvulnerable;}
	void updateInvulnerableTimer(const Ogre::Real tpf) {
		if (m_bInvulnerable) {
			m_fInvulnerableTimer -= tpf;
			if (m_fInvulnerableTimer <= 0.f) {
				m_bInvulnerable = false;
				endBeingInvulnerableCallback();
			}
		}
	}
	void makeInvulnerable(const Ogre::Real fTimeToBeInvulnerable = DEFAULT_INVULNERABLE_TIME) {
		m_bInvulnerable = true;
		m_fInvulnerableTimer = fTimeToBeInvulnerable;
		startBeingInvulnerableCallback();
	}
	void setCurAndMaxHP(Hitpoints hp) {
		setMaxHP(hp);
		setCurrentHP(hp);
	}
	void setMaxHP(Hitpoints uiMaxHP) {
		if (m_uiMaxHP != uiMaxHP) {
			m_uiMaxHP = uiMaxHP;
			maxHitpointsChangedCallback();
		}
		else {
			m_uiMaxHP = uiMaxHP;
		}
	}
	void setCurrentHP(Hitpoints uiHP) {
		if (m_uiCurrentHP != uiHP) {
			m_uiCurrentHP = uiHP;
			hitpointsChangedCallback();
		}
		else {
			m_uiCurrentHP = uiHP;
		}

		if (m_uiCurrentHP == 0) {
			killedCallback();
		}
	}
	void changeHP(Hitpoints uiHP) {
		if (m_uiCurrentHP + uiHP <= 0) {
			m_uiCurrentHP = 0;
			hitpointsChangedCallback();
			killedCallback();
			return;
		}
		else if (m_uiCurrentHP + uiHP >= m_uiMaxHP) {
			m_uiCurrentHP = m_uiMaxHP;
			hitpointsChangedCallback();
			return;
		}

		if (uiHP != 0) {
			m_uiCurrentHP += uiHP;
			hitpointsChangedCallback();
		}
	}

protected:
	virtual void hitpointsChangedCallback() {}
	virtual void maxHitpointsChangedCallback() {}
	virtual void killedCallback() {}
	virtual void startBeingInvulnerableCallback() {}
	virtual void endBeingInvulnerableCallback() {}
};

#endif
