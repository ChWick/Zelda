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

#include "ItemDataLoader.hpp"
#include <string>
#include "../../Common/Util/XMLHelper.hpp"
#include "ItemData.hpp"
#include "ItemTypes.hpp"
#include <OgreStringConverter.h>

using XMLHelper::Attribute;
using XMLHelper::IntAttribute;
using XMLHelper::RealAttribute;

void CItemDataLoader::readGroupElement(const tinyxml2::XMLElement *e) {
  SItemVariantData data;
  const std::string id = Attribute(e, "id");
  data.sImagesetName = Attribute(e, "imageset_name", "");
  data.ucItemQuality = IntAttribute(e, "item_quality", 1);
  data.sBasicMeshName = Attribute(e, "basic_mesh_name");
  data.eDamageType = static_cast<EDamageType>(
      CDamageTypeIdMap::getSingleton().parseString(
      Attribute(e, "damage_type", "none")));
  data.uiDamage = IntAttribute(e, "damage_value", HP_NONE);

  data.vBlockPhysicsSize = Ogre::StringConverter::parseVector3(
      Attribute(e, "block_physics_size", ""));
  data.vBlockPhysicsOffset = Ogre::StringConverter::parseVector3(
      Attribute(e, "block_physics_offset", ""));

  data.fLength = RealAttribute(e, "length", 0);

  CItemVariantDataMap::getSingleton().
      setData(CItemVariantIdMap::getSingleton().parseString(id),
              data);
}
