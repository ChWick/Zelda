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

#ifndef _GLOBAL_COLLISION_SHAPES_TYPES_HPP_
#define _GLOBAL_COLLISION_SHAPES_TYPES_HPP_

#include "../Common/Util/EnumIdMap.hpp"

enum EGlobalCollisionShapesTypes {
  GCST_PICKABLE_OBJECT_SPHERE,
  GCST_PERSON_CAPSULE,
  GCST_HOUSE_ENTRANCE,

  GCST_COUNT,
};

class CGlobalCollisionShapesTypesIdMap : public CEnumIdMap<EGlobalCollisionShapesTypes> {
public:
  CGlobalCollisionShapesTypesIdMap();
};

extern CGlobalCollisionShapesTypesIdMap GLOBAL_COLLISION_SHAPES_TYPES_ID_MAP;


#endif // _GLOBAL_COLLISION_SHAPES_TYPES_HPP_
