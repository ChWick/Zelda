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

#include "PersonDataLoader.hpp"
#include <string>
#include "../../Common/Util/XMLHelper.hpp"
#include "PersonTypes.hpp"
#include <OgreStringConverter.h>

using XMLHelper::Attribute;
using XMLHelper::IntAttribute;
using XMLHelper::RealAttribute;
using XMLHelper::Vector3Attribute;

void CPersonDataLoader::readGroupElement(const tinyxml2::XMLElement *e) {
  SPersonData data;
  const std::string id = Attribute(e, "id");
  data.eType = PERSON_TYPE_ID_MAP.parseString(id);
  data.sMeshName = Attribute(e, "mesh_name");
  data.sMaterialName = Attribute(e, "material_name", "");
  data.vScale = Vector3Attribute(e, "scale", Ogre::Vector3::UNIT_SCALE);
  data.eAttitude = CHARACTER_ATTITUDE_ID_MAP.parseString(
      Attribute(e, "attitude"));
  data.hitpoints = HP_ONE_HEART * IntAttribute(e, "hitpoints",
                                               HP_INFINITY / HP_ONE_HEART);


  PERSON_DATA_ID_MAP.setData(data.eType, data);
}
