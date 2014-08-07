#include "HitableInterface.hpp"
#include "Damage.hpp"

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

CHitableInterface::EReceiveDamageResult CHitableInterface::hit(const CDamage &damage) {
  EReceiveDamageResult r = receiveDamage(damage);
  switch (r) {
  case RDR_ACCEPTED:
    changeHP(damage.getDamageValue());
    damageAccepted(damage);
    break;
  case RDR_BLOCKED:
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
