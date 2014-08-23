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

#include "ObjectTypes.hpp"


CObjectTypeIdMap::CObjectTypeIdMap() {
  m_Map[OBJECT_GREEN_BUSH] = {"GreenBush.mesh", "soil"};
  m_Map[OBJECT_LIGHT_STONE] = {"light_stone.mesh", "soil"};
  
  m_Map[OBJECT_GREEN_RUPEE] = {"rupee.mesh", "Rupee/Green"};
  m_Map[OBJECT_BLUE_RUPEE] = {"rupee.mesh", "Rupee/Blue"};
  m_Map[OBJECT_RED_RUPEE] = {"rupee.mesh", "Rupee/Red"};
}

EObjectTypes CObjectTypeIdMap::getFromMesh(const std::string &mesh) const {
  for (auto &d : m_Map) {
    if (d.second.sMeshName == mesh) {
      return d.first;
    }
  }
  return OBJECT_COUNT;
}

CObjectTypeIdMap OBJECT_TYPE_ID_MAP;
