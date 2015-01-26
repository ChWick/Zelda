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

#include "AttachedParentData.hpp"
#include "../Util/XMLHelper.hpp"

using XMLHelper::Attribute;

namespace DataContainers {

CAttachedParentData::CAttachedParentData()
    : mType(APT_LOCAL),
      mEntityID("") {
}

CAttachedParentData::CAttachedParentData(const tinyxml2::XMLElement *e)
    : mType(CAttachedParentTypesMap::getSingleton().parseString(
          Attribute(e, "attached_parent_type"))),
      mEntityID(Attribute(e, "attached_entity_id", "")) {
  ASSERT((mType != APT_ENTITY && mEntityID.size() == 0)
         || (mType == APT_ENTITY && mEntityID.size() > 0));
}

void CAttachedParentTypesMap::init() {
  m_Map[APT_MAP] = "map";
  m_Map[APT_ATLAS] = "atlas";
  m_Map[APT_WORLD] = "world";
  m_Map[APT_LOCAL] = "local";
  m_Map[APT_ENTITY] = "entity";
}

}  // namespace DataContainers
