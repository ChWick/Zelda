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

#include "AttackerInterface.hpp"
#include "HitableInterface.hpp"
#include "Damage.hpp"


CHitableInterface::EReceiveDamageResult CAttackerInterface::attack(
    const CDamage &damage,
    CHitableInterface *hitableInterface) {
  CHitableInterface::EReceiveDamageResult res(hitableInterface->hit(damage));
  switch (res) {
    case CHitableInterface::RDR_BLOCKED:
      damage.getAttacker()->myDamageBlocked(damage, hitableInterface);
      break;
    case CHitableInterface::RDR_IGNORED:
      damage.getAttacker()->myDamageIgnored(damage, hitableInterface);
      break;
    case CHitableInterface::RDR_ACCEPTED:
      damage.getAttacker()->myDamageAccepted(damage, hitableInterface);
      break;
    case CHitableInterface::RDR_REJECTED:
      damage.getAttacker()->myDamageRejected(damage, hitableInterface);
      break;
  }

  return res;
}
