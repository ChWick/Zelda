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

#include "GlobalCollisionShapesTypes.hpp"


void CGlobalCollisionShapesTypesIdMap::init() {
  m_Map[GCST_PICKABLE_OBJECT_SPHERE] = "pickable_object_sphere";
  m_Map[GCST_PERSON_CAPSULE] = "person_capsule";
  m_Map[GCST_HOUSE_ENTRANCE_TOP] = "house_entrance_top";
  m_Map[GCST_HOUSE_ENTRANCE_SIDE] = "house_entrance_side";
  m_Map[GCST_HOUSE_ENTRANCE] = "house_entrance";
  m_Map[GCST_STONE_PILE_SINGLE_STONE] = "stone_pile_single_stone";
  m_Map[GCST_STONE_PILE] = "stone_pile";
  m_Map[GCST_TREE] = "tree";
  m_Map[GCST_FALLING_OBJECT_SPHERE] = "falling_object_sphere";
  m_Map[GCST_SMALL_CHEST_TOP] = "small_chest_top";

  m_Map[GCST_COUNT] = "none";
}

