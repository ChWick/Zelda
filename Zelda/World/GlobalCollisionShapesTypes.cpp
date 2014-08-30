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


CGlobalCollisionShapesTypesIdMap::CGlobalCollisionShapesTypesIdMap() {
  m_Map[GCST_PICKABLE_OBJECT_SPHERE] = "pickable_object_sphere";
  m_Map[GCST_PERSON_CAPSULE] = "person_capsule";
  m_Map[GCST_HOUSE_ENTRANCE] = "house_entrance";
  m_Map[GCST_STONE_PILE] = "stone_pile";
  m_Map[GCST_TREE] = "tree";
  m_Map[GCST_FALLING_OBJECT_SPHERE] = "falling_object_sphere";
}

CGlobalCollisionShapesTypesIdMap GLOBAL_COLLISION_SHAPES_TYPES_ID_MAP;

