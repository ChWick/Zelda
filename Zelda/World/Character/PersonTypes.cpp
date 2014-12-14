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

CPersonTypeIdMap PERSON_TYPE_ID_MAP;
CPersonDataIdMap PERSON_DATA_ID_MAP;


void CPersonTypeIdMap::init() {
  m_Map[PERSON_LINK] = "link";
  m_Map[PERSON_LINKS_FATHER] = "links_father";
  m_Map[PERSON_SOLDIER_BLOCK] = "soldier_block";
  m_Map[PERSON_SOLDIER_BLUE] = "soldier_blue";
  m_Map[PERSON_SOLDIER_GREEN_SWORD] = "soldier_green_sword";
}

void CPersonDataIdMap::init() {
  m_Map[PERSON_LINK] = {PERSON_LINK, "link", "link_armor", Ogre::Vector3::UNIT_SCALE * 1.5f, ATTITUDE_FRIENDLY, HP_ONE_HEART * 3};
  m_Map[PERSON_LINKS_FATHER] = {PERSON_LINKS_FATHER, "soldier", "soldier_blue", Ogre::Vector3::UNIT_SCALE, ATTITUDE_FRIENDLY, HP_INFINITY};
  m_Map[PERSON_SOLDIER_BLOCK] = {PERSON_SOLDIER_BLOCK, "soldier", "soldier_green", Ogre::Vector3::UNIT_SCALE, ATTITUDE_FRIENDLY, HP_INFINITY};
  m_Map[PERSON_SOLDIER_BLUE] = {PERSON_SOLDIER_BLUE, "soldier", "soldier_blue", Ogre::Vector3::UNIT_SCALE, ATTITUDE_ENEMY, HP_HALF_HEART * 3};
  m_Map[PERSON_SOLDIER_GREEN_SWORD] = {PERSON_SOLDIER_GREEN_SWORD, "soldier", "soldier_green", Ogre::Vector3::UNIT_SCALE, ATTITUDE_ENEMY, HP_HALF_HEART * 2};
}
