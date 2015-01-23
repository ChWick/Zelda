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
#include <string>
#include "../DamageTypes.hpp"
#include "../../Common/Util/XMLHelper.hpp"
#include "../../Common/tinyxml2/tinyxml2.hpp"
#include "../../Common/Physics/BtOgreExtras.hpp"
#include "../Atlas/TileTypes.hpp"
#include "../GlobalCollisionShapesTypes.hpp"
#include <OgreStringConverter.h>

using tinyxml2::XMLElement;

using XMLHelper::Attribute;
using XMLHelper::BoolAttribute;
using XMLHelper::RealAttribute;

CObjectConstructionInfo::CObjectConstructionInfo()
    : CEntityConstructionInfo("BasicObjectConstructionInfo") {
}

CObjectConstructionInfo::CObjectConstructionInfo(const tinyxml2::XMLElement *e)
    : CEntityConstructionInfo(e),
      mUserHandle(BoolAttribute(e, "user_handle", true)),
      mPermanentStatic(BoolAttribute(e, "permanent_static", false)),
      mMeshName(Attribute(e, "mesh_name")),
      mMaterialName(Attribute(e, "material_name", "")),
      mDamageSourceMask(CDamageTypeIdMap::getSingleton().parseString(
          Attribute(e, "damage_source_mask", "none"))),
      mNormalTile(CTileTypeIdMap::getSingleton().parseString(
          Attribute(e, "normal_tile", "none"))),
      mRemovedTile(CTileTypeIdMap::getSingleton().parseString(
          Attribute(e, "removed_tile", "none"))),
  mCollisionShape(CGlobalCollisionShapesTypesIdMap::getSingleton().
                  parseString(Attribute(e, "collision_shape", "none"))),
  mPhysicsShapeScale(BtOgre::Convert::toBullet(
      Ogre::StringConverter::parseVector3(
          Attribute(e, "physics_shape_scale", "1 1 1")))) {
  setType(CObjectTypeIdMap::getSingleton().
          parseString(Attribute(e, "type")));
}



void CObjectTypeIdMap::init() {
  m_Map[OBJECT_GREEN_BUSH] = "green_bush";
  m_Map[OBJECT_LIGHT_STONE] = "light_stone";
  m_Map[OBJECT_LIGHT_STONE_PILE] = "light_stone_pile";
  m_Map[OBJECT_VASE] = "vase";
  m_Map[OBJECT_GREEN_TREE] = "green_tree";
  m_Map[OBJECT_GREEN_RUPEE] = "rupee_green";
  m_Map[OBJECT_BLUE_RUPEE] = "rupee_blue";
  m_Map[OBJECT_RED_RUPEE] = "rupee_red";
  m_Map[OBJECT_HEART] = "heart";

  m_Map[OBJECT_TOOL_LAMP] = "tool_lamp";

  m_Map[OBJECT_COUNT] = "none";
}

void CObjectDataMap::init() {
}

EObjectTypes CObjectDataMap::getFromMeshName(const std::string &mesh) const {
  for (auto &d : m_Map) {
    if (d.second.getMeshName() == mesh) {
      return d.first;
    }
  }
  return OBJECT_COUNT;
}

EObjectTypes CObjectDataMap::getFromMeshFileName(
    const std::string &mesh) const {
  for (auto &d : m_Map) {
    if (d.second.getMeshName() + ".mesh" == mesh) {
      return d.first;
    }
  }
  return OBJECT_COUNT;
}
