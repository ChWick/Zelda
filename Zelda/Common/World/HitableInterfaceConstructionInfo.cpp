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

#include "HitableInterfaceConstructionInfo.hpp"
#include "../Util/XMLHelper.hpp"

using XMLHelper::IntAttribute;
using XMLHelper::RealAttribute;
using XMLHelper::BoolAttribute;

CHitableInterfaceConstructionInfo::CHitableInterfaceConstructionInfo()
    : mMaximalHitpoints(HP_INFINITY / HP_ONE_HEART),
      mCurrentHitpoints(HP_INFINITY / HP_ONE_HEART),
      mInvulnerableTimer(0),
      mInvulnerable(false) {
}

void CHitableInterfaceConstructionInfo::parse(const tinyxml2::XMLElement *e) {
  mMaximalHitpoints = HP_ONE_HEART * IntAttribute(
      e, "hitpoints", mMaximalHitpoints);
  mCurrentHitpoints = HP_ONE_HEART * IntAttribute(
      e, "current_hitpoints", mCurrentHitpoints);
  mInvulnerableTimer = RealAttribute(e, "invulnerable_timer",
                                     mInvulnerableTimer);
  mInvulnerable = BoolAttribute(e, "is_invulnerable", mInvulnerable);
}
