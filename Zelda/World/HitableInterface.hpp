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

#ifndef _HITABLE_INTERFACE_HPP_
#define _HITABLE_INTERFACE_HPP_

#include "Hitpoints.hpp"

class CDamage;

class CHitableInterface {
private:
	Hitpoints m_uiMaxHP;								  //!< maximum of hit points (in hear percentage)
	Hitpoints m_uiCurrentHP;							//!< current hit points (in heart percentage)
	float m_fInvulnerableTimer;			      //!< time to be invulnerable
	bool m_bInvulnerable;                 //!< invulnerable?

public:
  enum EReceiveDamageResult {
    RDR_ACCEPTED,
    RDR_REJECTED,
    RDR_BLOCKED,
    RDR_IGNORED,
  };

	CHitableInterface()
		: m_uiMaxHP(0), m_uiCurrentHP(0), m_fInvulnerableTimer(0), m_bInvulnerable(false) {
	}

	Hitpoints getMaxHP() const {return m_uiMaxHP;}
	Hitpoints getCurrentHP() const {return m_uiCurrentHP;}

	bool isInvulnerable() const {return m_bInvulnerable;}
	void update(float tpf);
	void makeInvulnerable(float fTimeToBeInvulnerable = DEFAULT_INVULNERABLE_TIME);
	void setCurAndMaxHP(Hitpoints hp);
	void setMaxHP(Hitpoints uiMaxHP);
	void setCurrentHP(Hitpoints uiHP);
	void changeHP(Hitpoints uiHP);


  virtual EReceiveDamageResult hit(const CDamage &damage);

protected:
	virtual void hitpointsChangedCallback() {}
	virtual void maxHitpointsChangedCallback() {}
	virtual void killedCallback() {}
	virtual void startBeingInvulnerableCallback() {}
	virtual void endBeingInvulnerableCallback() {}

  virtual void damageIgnored(const CDamage &damage) {}
  virtual void damageAccepted(const CDamage &damage) {}
  virtual void damageBlocked(const CDamage &damage) {}
  virtual void damageRejected(const CDamage &damage) {}

  virtual EReceiveDamageResult receiveDamage(const CDamage &damage) {return RDR_IGNORED;}
};
#endif // _HITABLE_INTERFACE_HPP_
