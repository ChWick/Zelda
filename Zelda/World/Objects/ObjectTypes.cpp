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
#include "../../Common/World/DamageTypes.hpp"
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
using XMLHelper::Vector3Attribute;

CObjectConstructionInfo::CObjectConstructionInfo()
    : CWorldEntityConstructionInfo(),
      mUserHandle(false),
      mPermanentStatic(false),
      mDamageSourceMask(DMG_NONE),
      mNormalTile(TT_COUNT),
      mRemovedTile(TT_COUNT),
      mCollisionShape(GCST_COUNT),
      mPhysicsShapeScale(1, 1, 1) {
}


void CObjectConstructionInfo::parse(const tinyxml2::XMLElement *e) {
  CWorldEntityConstructionInfo::parse(e);

  mUserHandle = BoolAttribute(e, "user_handle", mUserHandle);
  mPermanentStatic = BoolAttribute(e, "permanent_static", mPermanentStatic);
  mMeshName = Attribute(e, "mesh_name", mMeshName);
  mMaterialName = Attribute(e, "material_name", mMaterialName);
  mDamageSourceMask = CDamageTypeIdMap::getSingleton().parseString(
      Attribute(e, "damage_source_mask",
                CDamageTypeIdMap::getSingleton().toString(
                    mDamageSourceMask)));
  mNormalTile = CTileTypeIdMap::getSingleton().parseString(
      Attribute(e, "normal_tile", CTileTypeIdMap::getSingleton().toString(
          mNormalTile)));
  mRemovedTile = CTileTypeIdMap::getSingleton().parseString(
      Attribute(e, "removed_tile", CTileTypeIdMap::getSingleton().toString(
          mRemovedTile)));
  mCollisionShape = CGlobalCollisionShapesTypesIdMap::getSingleton().
      parseString(Attribute(
          e, "collision_shape",
          CGlobalCollisionShapesTypesIdMap::getSingleton().toString(
              mCollisionShape)));
  mPhysicsShapeScale = BtOgre::Convert::toBullet(
      Vector3Attribute(e, "physics_shape_scale",
                       BtOgre::Convert::toOgre(mPhysicsShapeScale)));

  setType(CObjectTypeIdMap::getSingleton().
          parseString(Attribute(e, "type",
                                CObjectTypeIdMap::getSingleton().toString(
                                    static_cast<EObjectTypes>(getType())))));
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
