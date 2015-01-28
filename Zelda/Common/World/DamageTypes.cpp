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

#include "DamageTypes.hpp"
#include <string>

void CDamageTypeIdMap::init() {
  m_Map[DMG_NONE]    = "none";
  m_Map[DMG_WORLD]   = "world";
  m_Map[DMG_SWORD]   = "sword";
  m_Map[DMG_ARROW]   = "arrow";
  m_Map[DMG_HAMMER]  = "hammer";
  m_Map[DMG_BOMB]    = "bomb";
  m_Map[DMG_FIRE]    = "fire";

  m_Map[DMG_RUN]     = "run";
  m_Map[DMG_FLYING_OBJECT] = "flying_object";

  m_Map[DMG_ALL]     = "all";
}

unsigned int CDamageTypeIdMap::parseString(const std::string &str) const {
  unsigned int uiDmg = DMG_NONE;

  std::string delimiters = " |+";
  size_t current;
  size_t next = -1;
  do {
    current = next + 1;
    next = str.find_first_of(delimiters, current);
    uiDmg |= CStringEnumIdMap<CDamageTypeIdMap, unsigned int>::parseString(
        str.substr(current, next - current));
  } while (next != std::string::npos);

  return uiDmg;
}

void CDamageDataMap::init() {
  m_Map[DMG_NONE] = {HP_NONE};
  m_Map[DMG_WORLD] = {HP_ONE_HEART};
  m_Map[DMG_SWORD] = {HP_HALF_HEART};
  m_Map[DMG_ARROW] = {HP_ONE_HEART};
  m_Map[DMG_HAMMER] = {HP_ONE_HEART};
  m_Map[DMG_BOMB] = {HP_ONE_HEART};
  m_Map[DMG_FIRE] = {HP_ONE_HEART};
  m_Map[DMG_RUN] = {HP_NONE};
}
