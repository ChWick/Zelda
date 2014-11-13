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
  OBJECT_VASE,

  OBJECT_GREEN_TREE,
  OBJECT_GREEN_RUPEE,
  OBJECT_BLUE_RUPEE,
  OBJECT_RED_RUPEE,
  OBJECT_HEART,

  OBJECT_COUNT,
};

struct SObjectTypeData {
  bool bUserHandle;
  bool bPermanetStatic;
  std::string sID;
  std::string sMeshName;
  std::string sMaterialName;

  unsigned int eDamageSourceMask;

  ETileTypes eNormalTile;
  ETileTypes eRemovedTile;

  EGlobalCollisionShapesTypes eCollisionShape;
  btVector3 vPhysicsShapeScale;


  SObjectTypeData() {}
  SObjectTypeData(SObjectTypeData &&) = default;
  SObjectTypeData(bool userHandle, bool isStatic, const std::string &id, const std::string &meshName, const std::string &materialName, unsigned int damageSourceMask);
  SObjectTypeData(bool userHandle, bool isStatic, const std::string &id, const std::string &meshName, const std::string &materialName, unsigned int damageSourceMask, const btVector3 &physicsShapeScale);
  SObjectTypeData(bool userHandle, bool isStatic, const std::string &id, const std::string &meshName, const std::string &materialName, unsigned int damageSourceMask, EGlobalCollisionShapesTypes collisionShape);

  SObjectTypeData(bool userHandle, bool isStatic, const std::string &id, const std::string &meshName, const std::string &materialName, unsigned int damageSourceMask, ETileTypes normalTile, ETileTypes removedTile, EGlobalCollisionShapesTypes collisionShape);

  SObjectTypeData &operator=(const SObjectTypeData&) = default;
};

class CObjectTypeIdMap : public CEnumIdMap<EObjectTypes, SObjectTypeData> {
public:
  CObjectTypeIdMap();

  EObjectTypes getFromID(const std::string &id) const;
  EObjectTypes getFromMeshName(const std::string &mesh) const;
  EObjectTypes getFromMeshFileName(const std::string &mesh) const;
};

extern CObjectTypeIdMap OBJECT_TYPE_ID_MAP;

#endif // _OBJECT_TYPES_HPP_
