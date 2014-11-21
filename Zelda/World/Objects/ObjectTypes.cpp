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
#include "../DamageTypes.hpp"


CObjectTypeIdMap OBJECT_TYPE_ID_MAP;
CObjectDataMap OBJECT_DATA_MAP;

CObjectTypeIdMap::CObjectTypeIdMap() {
  m_Map[OBJECT_GREEN_BUSH] = "green_bush";
  m_Map[OBJECT_LIGHT_STONE] = "light_stone";
  m_Map[OBJECT_LIGHT_STONE_PILE] = "light_stone_pile";
  m_Map[OBJECT_VASE] = "vase";
  m_Map[OBJECT_GREEN_TREE] = "green_tree";
  m_Map[OBJECT_GREEN_RUPEE] = "rupee_green";
  m_Map[OBJECT_BLUE_RUPEE] = "rupee_blue";
  m_Map[OBJECT_RED_RUPEE] = "rupee_red";
  m_Map[OBJECT_HEART] = "heart";
  m_Map[OBJECT_COUNT] = "none";
}

SObjectTypeData::SObjectTypeData(bool userHandle, bool isStatic, const std::string &id, const std::string &meshName, const std::string &materialName, unsigned int damageSourceMask)
  : bUserHandle(userHandle),
    bPermanentStatic(isStatic),
    sMeshName(meshName),
    sMaterialName(materialName),
    eDamageSourceMask(damageSourceMask),
    eNormalTile(TT_COUNT),
    eRemovedTile(TT_COUNT),
    eCollisionShape(GCST_COUNT),
    vPhysicsShapeScale(1, 1, 1) {
}

SObjectTypeData::SObjectTypeData(bool userHandle, bool isStatic, const std::string &id, const std::string &meshName, const std::string &materialName, unsigned int damageSourceMask, const btVector3 &physicsShapeScale)
  : bUserHandle(userHandle),
    bPermanentStatic(isStatic),
    sMeshName(meshName),
    sMaterialName(materialName),
    eDamageSourceMask(damageSourceMask),
    eNormalTile(TT_COUNT),
    eRemovedTile(TT_COUNT),
    eCollisionShape(GCST_COUNT),
    vPhysicsShapeScale(physicsShapeScale) {
}

SObjectTypeData::SObjectTypeData(bool userHandle, bool isStatic, const std::string &id, const std::string &meshName, const std::string &materialName, unsigned int damageSourceMask, EGlobalCollisionShapesTypes collisionShape)
  : bUserHandle(userHandle),
    bPermanentStatic(isStatic),
    sMeshName(meshName),
    sMaterialName(materialName),
    eDamageSourceMask(damageSourceMask),
    eNormalTile(TT_COUNT),
    eRemovedTile(TT_COUNT),
    eCollisionShape(collisionShape),
    vPhysicsShapeScale(1, 1, 1) {
}

SObjectTypeData::SObjectTypeData(bool userHandle, bool isStatic, const std::string &id, const std::string &meshName, const std::string &materialName, unsigned int damageSourceMask, ETileTypes normalTile, ETileTypes removedTile, EGlobalCollisionShapesTypes collisionShape)
  : bUserHandle(userHandle),
    bPermanentStatic(isStatic),
    sMeshName(meshName),
    sMaterialName(materialName),
    eDamageSourceMask(damageSourceMask),
    eNormalTile(normalTile),
    eRemovedTile(removedTile),
    eCollisionShape(collisionShape),
    vPhysicsShapeScale(1, 1, 1) {
}

CObjectDataMap::CObjectDataMap() {
  /*
  m_Map[OBJECT_GREEN_BUSH] = SObjectTypeData(true, false, "green_bush", "GreenBush", "soil", DMG_SWORD, TT_GREEN_SOIL_BUSH_SHADOW, TT_GREEN_SOIL_GRASS_BL_BR_TL_TR, GCST_PICKABLE_OBJECT_SPHERE);
  m_Map[OBJECT_LIGHT_STONE] = SObjectTypeData(true, false, "light_stone", "light_stone", "soil", DMG_NONE, TT_GREEN_SOIL_STONE_SHADOW, TT_GREEN_SOIL, GCST_PICKABLE_OBJECT_SPHERE);
  m_Map[OBJECT_LIGHT_STONE_PILE] = SObjectTypeData(true, false, "lw_light_stone_pile", "lw_light_stone_pile", "soil", DMG_WORLD | DMG_RUN, TT_GREEN_SOIL_STONE_PILE_SHADOW, TT_GREEN_SOIL, GCST_STONE_PILE);

  m_Map[OBJECT_VASE] = SObjectTypeData(true, false, "vase", "vase", "VaseSide", DMG_WORLD, GCST_PICKABLE_OBJECT_SPHERE);

  m_Map[OBJECT_GREEN_TREE] = SObjectTypeData(true, true, "green_tree", "green_tree", "soil", DMG_RUN, GCST_TREE);
  
  m_Map[OBJECT_GREEN_RUPEE] = SObjectTypeData(true, false, "rupee_green", "rupee", "Rupee/Green", DMG_NONE, GCST_FALLING_OBJECT_SPHERE);
  m_Map[OBJECT_BLUE_RUPEE] = SObjectTypeData(true, false, "rupee_blue", "rupee", "Rupee/Blue", DMG_NONE, GCST_FALLING_OBJECT_SPHERE);
  m_Map[OBJECT_RED_RUPEE] = SObjectTypeData(true, false, "rupee_red", "rupee", "Rupee/Red", DMG_NONE, GCST_FALLING_OBJECT_SPHERE);
  m_Map[OBJECT_HEART] = SObjectTypeData(true, false, "heart", "heart", "global", DMG_NONE, GCST_FALLING_OBJECT_SPHERE);
  */
}

EObjectTypes CObjectDataMap::getFromMeshName(const std::string &mesh) const {
  for (auto &d : m_Map) {
    if (d.second.sMeshName == mesh) {
      return d.first;
    }
  }
  return OBJECT_COUNT;
}
EObjectTypes CObjectDataMap::getFromMeshFileName(const std::string &mesh) const {
  for (auto &d : m_Map) {
    if (d.second.sMeshName + ".mesh" == mesh) {
      return d.first;
    }
  }
  return OBJECT_COUNT;
}
