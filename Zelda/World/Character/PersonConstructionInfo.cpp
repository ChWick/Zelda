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

#include "PersonConstructionInfo.hpp"
#include <string>
#include "../../Common/Util/XMLHelper.hpp"

using XMLHelper::Attribute;
using XMLHelper::RealAttribute;
using XMLHelper::IntAttribute;
using XMLHelper::BoolAttribute;
using XMLHelper::Vector3Attribute;
using XMLHelper::EnumAttribute;


CPersonConstructionInfo::CPersonConstructionInfo()
    : CCharacterConstructionInfo(),
      mScale(Ogre::Vector3::UNIT_SCALE) {
}

void CPersonConstructionInfo::parse(const tinyxml2::XMLElement *e) {
  CCharacterConstructionInfo::parse(e);

  mPersonType = EnumAttribute<CPersonTypeIdMap, EPersonTypes>(
      e, "person_type", mPersonType);
  mMeshName = Attribute(e, "mesh_name", mMeshName);
  mMaterialName = Attribute(e, "material_name", mMaterialName);
  mScale = Vector3Attribute(e, "scale", mScale);
}
