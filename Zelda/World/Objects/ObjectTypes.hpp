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

#ifndef _OBJECT_TYPES_HPP_
#define _OBJECT_TYPES_HPP_

#include "../../Common/Util/EnumIdMap.hpp"
#include "../Atlas/TileTypes.hpp"
#include "../GlobalCollisionShapesTypes.hpp"
#include <LinearMath/btVector3.h>

enum EObjectTypes {
  OBJECT_GREEN_BUSH,
  OBJECT_LIGHT_STONE,
  OBJECT_LIGHT_STONE_PILE,
  OBJECT_GREEN_TREE,
  OBJECT_GREEN_RUPEE,
  OBJECT_BLUE_RUPEE,
  OBJECT_RED_RUPEE,

  OBJECT_COUNT,
};

struct SObjectTypeData {
  bool bUserHandle;
  std::string sMeshName;
  std::string sMaterialName;

  ETileTypes eNormalTile;
  ETileTypes eRemovedTile;

  EGlobalCollisionShapesTypes eCollisionShape;
  btVector3 vPhysicsShapeScale;


  SObjectTypeData() {}
  SObjectTypeData(SObjectTypeData &&) = default;
  SObjectTypeData(bool userHandle, const std::string &meshName, const std::string &materialName);
  SObjectTypeData(bool userHandle, const std::string &meshName, const std::string &materialName, const btVector3 &physicsShapeScale);

  SObjectTypeData(bool userHandle, const std::string &meshName, const std::string &materialName, ETileTypes normalTile, ETileTypes removedTile, EGlobalCollisionShapesTypes collisionShape);

  SObjectTypeData &operator=(const SObjectTypeData&) = default;
};

class CObjectTypeIdMap : public CEnumIdMap<EObjectTypes, SObjectTypeData> {
public:
  CObjectTypeIdMap();

  EObjectTypes getFromMesh(const std::string &mesh) const;
};

extern CObjectTypeIdMap OBJECT_TYPE_ID_MAP;

#endif // _OBJECT_TYPES_HPP_
