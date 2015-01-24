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

#include <vector>
#include "../../Common/Util/EnumIdMap.hpp"
#include "../Atlas/TileTypes.hpp"
#include "../GlobalCollisionShapesTypes.hpp"
#include "../../Common/World/WorldEntityConstructionInfo.hpp"
#include <LinearMath/btVector3.h>

enum EObjectTypes {
  // objects on map
  OBJECT_GREEN_BUSH,
  OBJECT_LIGHT_STONE,
  OBJECT_LIGHT_STONE_PILE,
  OBJECT_VASE,

  // pickable objects
  OBJECT_GREEN_TREE,
  OBJECT_GREEN_RUPEE,
  OBJECT_BLUE_RUPEE,
  OBJECT_RED_RUPEE,
  OBJECT_HEART,

  // tools that can occur in chests/store
  OBJECT_TOOL_LAMP,

  OBJECT_COUNT,
};

class CObjectConstructionInfo
    : public CWorldEntityConstructionInfo {
 private:
  bool mUserHandle;
  bool mPermanentStatic;
  std::string mMeshName;
  std::string mMaterialName;

  uint32_t mDamageSourceMask;

  ETileTypes mNormalTile;
  ETileTypes mRemovedTile;

  EGlobalCollisionShapesTypes mCollisionShape;
  btVector3 mPhysicsShapeScale;

 public:
  CObjectConstructionInfo();
  CObjectConstructionInfo(const tinyxml2::XMLElement *e);

  CObjectConstructionInfo &operator=(const CObjectConstructionInfo&) = default;

  bool isUsedHandled() const {return mUserHandle;}
  bool isPermanentStatic() const {return mPermanentStatic;}

  const std::string &getMeshName() const {return mMeshName;}
  const std::string &getMaterialName() const {return mMaterialName;}

  uint32_t getDamageSourceMask() const {return mDamageSourceMask;}

  ETileTypes getNormalTile() const {return mNormalTile;}
  ETileTypes getRemovedTile() const {return mRemovedTile;}

  EGlobalCollisionShapesTypes getCollisionShape() const {return mCollisionShape;}
  const btVector3 &getPhysicsShapeScale() const {return mPhysicsShapeScale;}
};

class CObjectTypeIdMap : public CStringEnumIdMap<CObjectTypeIdMap, EObjectTypes> {
public:
  void init();
};

class CObjectDataMap : public CEnumIdMap<CObjectDataMap, EObjectTypes, CObjectConstructionInfo> {
 public:
  void init();

  EObjectTypes getFromMeshName(const std::string &mesh) const;
  EObjectTypes getFromMeshFileName(const std::string &mesh) const;
};

#endif // _OBJECT_TYPES_HPP_
