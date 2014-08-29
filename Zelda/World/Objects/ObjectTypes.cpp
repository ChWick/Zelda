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

SObjectTypeData::SObjectTypeData(bool userHandle, const std::string &meshName, const std::string &materialName)
  : bUserHandle(userHandle),
    sMeshName(meshName),
    sMaterialName(materialName),
    eNormalTile(TT_COUNT),
    eRemovedTile(TT_COUNT),
    eCollisionShape(GCST_COUNT),
    vPhysicsShapeScale(1, 1, 1) {
}

SObjectTypeData::SObjectTypeData(bool userHandle, const std::string &meshName, const std::string &materialName, const btVector3 &physicsShapeScale)
  : bUserHandle(userHandle),
    sMeshName(meshName),
    sMaterialName(materialName),
    eNormalTile(TT_COUNT),
    eRemovedTile(TT_COUNT),
    eCollisionShape(GCST_COUNT),
    vPhysicsShapeScale(physicsShapeScale) {
}

SObjectTypeData::SObjectTypeData(bool userHandle, const std::string &meshName, const std::string &materialName, ETileTypes normalTile, ETileTypes removedTile, EGlobalCollisionShapesTypes collisionShape)
  : bUserHandle(userHandle),
    sMeshName(meshName),
    sMaterialName(materialName),
    eNormalTile(normalTile),
    eRemovedTile(removedTile),
    eCollisionShape(collisionShape),
    vPhysicsShapeScale(1, 1, 1) {
}

CObjectTypeIdMap::CObjectTypeIdMap() {
  m_Map[OBJECT_GREEN_BUSH] = SObjectTypeData(true, "GreenBush.mesh", "soil", TT_GREEN_SOIL_BUSH_SHADOW, TT_GREEN_SOIL_GRASS_BL_BR_TL_TR, GCST_PICKABLE_OBJECT_SPHERE);
  m_Map[OBJECT_LIGHT_STONE] = SObjectTypeData(true, "light_stone.mesh", "soil", TT_GREEN_SOIL_STONE_SHADOW, TT_GREEN_SOIL, GCST_PICKABLE_OBJECT_SPHERE);
  m_Map[OBJECT_LIGHT_STONE_PILE] = SObjectTypeData(true, "lw_light_stone_pile.mesh", "soil", TT_GREEN_SOIL_STONE_PILE_SHADOW, TT_GREEN_SOIL, GCST_STONE_PILE);

  m_Map[OBJECT_GREEN_TREE] = SObjectTypeData(false, "green_tree.mesh", "soil", btVector3(0.75, 1, 0.75));
  
  m_Map[OBJECT_GREEN_RUPEE] = SObjectTypeData(true, "rupee.mesh", "Rupee/Green");
  m_Map[OBJECT_BLUE_RUPEE] = SObjectTypeData(true, "rupee.mesh", "Rupee/Blue");
  m_Map[OBJECT_RED_RUPEE] = SObjectTypeData(true, "rupee.mesh", "Rupee/Red");
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
