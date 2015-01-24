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

#include "PersonTypes.hpp"
#include <string>
#include "../../Common/Util/XMLHelper.hpp"

using XMLHelper::Attribute;
using XMLHelper::RealAttribute;
using XMLHelper::IntAttribute;
using XMLHelper::BoolAttribute;
using XMLHelper::Vector3Attribute;

void CPersonTypeIdMap::init() {
  m_Map[PERSON_LINK] = "link";
  m_Map[PERSON_LINKS_FATHER] = "links_father";
  m_Map[PERSON_SOLDIER_BLOCK] = "soldier_block";
  m_Map[PERSON_SOLDIER_BLUE] = "soldier_blue";
  m_Map[PERSON_SOLDIER_GREEN_SWORD] = "soldier_green_sword";
}

void CPersonDataIdMap::init() {
}

SPersonData::SPersonData(const tinyxml2::XMLElement *e)
    : SCharacterData(e) {
  const std::string id = Attribute(e, "id");
  eType = CPersonTypeIdMap::getSingleton().parseString(id);
  sMeshName = Attribute(e, "mesh_name");
  sMaterialName = Attribute(e, "material_name", "");
  vScale = Vector3Attribute(e, "scale", Ogre::Vector3::UNIT_SCALE);
  hitpoints = HP_ONE_HEART * IntAttribute(e, "hitpoints",
                                          HP_INFINITY / HP_ONE_HEART);
}

SPersonData::SPersonData() {
}

