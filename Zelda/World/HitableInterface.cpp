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

#include "HitableInterface.hpp"
#include "Damage.hpp"
#include "../Common/Message/MessageHandler.hpp"
#include "../Common/Message/MessageHitpointsChanged.hpp"

void CHitableInterface::update(float tpf) {
  if (m_bInvulnerable) {
    m_fInvulnerableTimer -= tpf;
    if (m_fInvulnerableTimer <= 0.f) {
      m_bInvulnerable = false;
      endBeingInvulnerableCallback();
    }
  }
}

void CHitableInterface::makeInvulnerable(float fTimeToBeInvulnerable) {
  m_bInvulnerable = true;
  m_fInvulnerableTimer = fTimeToBeInvulnerable;
  startBeingInvulnerableCallback();
}

void CHitableInterface::setCurAndMaxHP(Hitpoints hp) {
  setMaxHP(hp);
  setCurrentHP(hp);
}

void CHitableInterface::setMaxHP(Hitpoints uiMaxHP) {
  if (m_uiMaxHP != uiMaxHP) {
    m_uiMaxHP = uiMaxHP;
    maxHitpointsChangedCallback();
  }
  else {
    m_uiMaxHP = uiMaxHP;
  }
}

void CHitableInterface::setCurrentHP(Hitpoints uiHP) {
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

void CHitableInterface::changeHP(Hitpoints uiHP) {
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

CHitableInterface::EReceiveDamageResult CHitableInterface::hit(
    const CDamage &damage) {
  if (m_bInvulnerable) {return RDR_IGNORED;}
  bool bWasInvunerable = m_bInvulnerable;
  EReceiveDamageResult r = receiveDamage(damage);

  switch (r) {
  case RDR_ACCEPTED:
    if (!bWasInvunerable) {
      changeHP(-damage.getDamageValue());
    }
    damageAccepted(damage);
    break;
  case RDR_BLOCKED:
    std::cout << "hitblock" << std::endl;
    damageBlocked(damage);
    break;
  case RDR_IGNORED:
    damageIgnored(damage);
    break;
  case RDR_REJECTED:
    damageRejected(damage);
    break;
  }

  return r;
}

void CHitableInterface::maxHitpointsChangedCallback() {
  CMessageHandler::getSingleton().addMessage(
      std::make_shared<CMessageHitpointsChanged>(__MSG_LOCATION__, *this));
}

void CHitableInterface::hitpointsChangedCallback() {
  CMessageHandler::getSingleton().addMessage(
      std::make_shared<CMessageHitpointsChanged>(__MSG_LOCATION__, *this));
}
