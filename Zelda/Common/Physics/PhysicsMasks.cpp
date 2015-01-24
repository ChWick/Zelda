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

#include "PhysicsMasks.hpp"

void CPhysicsGroupsIdMap::init() {
  m_Map[COL_BORDER] = "border";
  m_Map[COL_CAMERA] = "camera";
  m_Map[COL_CHARACTER_N] = "character_n";
  m_Map[COL_CHARACTER_P] = "character_p";
  m_Map[COL_DAMAGE_N] = "damage_n";
  m_Map[COL_DAMAGE_P] = "damage_p";
  m_Map[COL_INTERACTIVE] = "interactive";
  m_Map[COL_NOTHING] = "nothing";
  m_Map[COL_SHIELD_N] = "shield_n";
  m_Map[COL_SHIELD_P] = "shield_p";
  m_Map[COL_STATIC] = "static";
}

void CPhysicsMasksIdMap::init() {
  m_Map[MASK_NONE] = "none";
  m_Map[MASK_PLAYER_P_COLLIDES_WITH] = "player_p";
  m_Map[MASK_PLAYER_N_COLLIDES_WITH] = "player_n";
  m_Map[MASK_PICKABLE_OBJECT_COLLIDES_WITH] = "pickable_object";
  m_Map[MASK_BLOCKADE_COLLIDES_WITH] = "blockade";
  m_Map[MASK_STATIC_COLLIDES_WITH] = "static";
  m_Map[MASK_INTERACIVE_OBJECT_COLLIDES_WITH] = "interactive";
  m_Map[MASK_SHIELD_N_COLLIDES_WITH] = "shield_n";
  m_Map[MASK_SHIELD_P_COLLIDES_WITH] = "shield_p";
  m_Map[MASK_CAMERA_COLLIDES_WITH] = "camera";
  m_Map[MASK_DAMAGE_N_COLLIDES_WITH] = "damage_n";
  m_Map[MASK_DAMAGE_P_COLLIDES_WITH] = "damage_p";
}
