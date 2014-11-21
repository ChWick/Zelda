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

#include "ObjectDataLoader.hpp"
#include <string>
#include "../../Common/Util/XMLHelper.hpp"
#include "../../Common/Physics/BtOgreExtras.hpp"
#include "ObjectTypes.hpp"
#include "../DamageTypes.hpp"
#include "../Atlas/TileTypes.hpp"
#include "../GlobalCollisionShapesTypes.hpp"
#include <OgreStringConverter.h>

using XMLHelper::Attribute;
using XMLHelper::IntAttribute;
using XMLHelper::RealAttribute;
using XMLHelper::BoolAttribute;

void CObjectDataLoader::readGroupElement(const tinyxml2::XMLElement *e) {
  SObjectTypeData data;
  data.eObjectType = OBJECT_TYPE_ID_MAP.parseString(Attribute(e, "type"));
  data.bUserHandle = BoolAttribute(e, "user_handle", true);
  data.bPermanentStatic = BoolAttribute(e, "permanent_static", false);
  data.sMeshName = Attribute(e, "mesh_name");
  data.sMaterialName = Attribute(e, "material_name", "");
  data.eDamageSourceMask = DAMAGE_TYPE_ID_MAP.parseString(
      Attribute(e, "damage_source_mask", "none"));
  data.eNormalTile = TILE_TYPE_ID_MAP.parseString(
      Attribute(e, "normal_tile", "none"));
  data.eRemovedTile = TILE_TYPE_ID_MAP.parseString(
      Attribute(e, "removed_tile", "none"));
  data.eCollisionShape = GLOBAL_COLLISION_SHAPES_TYPES_ID_MAP.parseString(
      Attribute(e, "collision_shape", "pickable_object_shape"));
  data.vPhysicsShapeScale = BtOgre::Convert::toBullet(
      Ogre::StringConverter::parseVector3(
          Attribute(e, "physics_shape_scale", "1 1 1")));

  OBJECT_DATA_MAP.setData(data.eObjectType, data);
}









