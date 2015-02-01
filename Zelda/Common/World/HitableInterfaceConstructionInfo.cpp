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
using XMLHelper::EnumAttribute;

CHitableInterfaceConstructionInfo::CHitableInterfaceConstructionInfo()
    : mDamageTypeMask(DMG_NONE),           // does not take damage by default
      mDamageAttitudeMask(ATTITUDE_NONE),  // no attitude (not hittable)
      mMaximalHitpoints(HP_INFINITY),      // infinite hitpoints by default
      mCurrentHitpoints(HP_INFINITY),      // infinite hitpoints by default
      mInvulnerableTimer(0),
      mInvulnerable(false) {
}

void CHitableInterfaceConstructionInfo::parse(const tinyxml2::XMLElement *e) {
  mDamageTypeMask = EnumAttribute<CDamageTypeIdMap, DamageTypeMask>(
      e, "hit_by_damage_type_mask", mDamageTypeMask);
  mDamageAttitudeMask = EnumAttribute<CDamageAttitudeIdMap, DamageAttitudeMask>(
      e, "hit_by_damage_attitude_mask", mDamageAttitudeMask);
  mMaximalHitpoints = HP_ONE_HEART * IntAttribute(
      e, "hitpoints", mMaximalHitpoints / HP_ONE_HEART);
  mCurrentHitpoints = HP_ONE_HEART * IntAttribute(
      e, "current_hitpoints", mCurrentHitpoints / HP_ONE_HEART);
  mInvulnerableTimer = RealAttribute(e, "invulnerable_timer",
                                     mInvulnerableTimer);
  mInvulnerable = BoolAttribute(e, "is_invulnerable", mInvulnerable);
}
