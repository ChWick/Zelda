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

#include "WorldEntityConstructionInfo.hpp"
#include <string>
#include "../Util/XMLHelper.hpp"
#include "../Physics/PhysicsMasks.hpp"

using XMLHelper::Attribute;
using XMLHelper::EnumAttribute;

CWorldEntityConstructionInfo::CWorldEntityConstructionInfo()
    : CEntityConstructionInfo(),
      CHitableInterfaceConstructionInfo(),
      mCollisionMask(MASK_STATIC_COLLIDES_WITH),
      mCollisionGroup(COL_STATIC) {
}

CWorldEntityConstructionInfo::CWorldEntityConstructionInfo(
    const std::string &id)
    : CEntityConstructionInfo(id),
      CHitableInterfaceConstructionInfo(),
      mCollisionMask(MASK_STATIC_COLLIDES_WITH),
      mCollisionGroup(COL_STATIC) {
}

void CWorldEntityConstructionInfo::parse(
    const tinyxml2::XMLElement *e) {
  CEntityConstructionInfo::parse(e);
  CHitableInterfaceConstructionInfo::parse(e);

  mCollisionMask =
      EnumAttribute<CPhysicsMasksIdMap, uint16_t>(
          e, "collision_mask", mCollisionMask);
  mCollisionGroup =
      EnumAttribute<CPhysicsGroupsIdMap, uint16_t>(
          e, "collision_group", mCollisionGroup);
}
