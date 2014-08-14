#ifndef _GLOBAL_COLLISION_SHAPES_TYPES_HPP_
#define _GLOBAL_COLLISION_SHAPES_TYPES_HPP_

#include "../Common/Util/EnumIdMap.hpp"

enum EGlobalCollisionShapesTypes {
  GCST_PICKABLE_OBJECT_SPHERE,
};

class CGlobalCollisionShapesTypesIdMap : public CEnumIdMap<EGlobalCollisionShapesTypes> {
public:
  CGlobalCollisionShapesTypesIdMap();
};

extern CGlobalCollisionShapesTypesIdMap GLOBAL_COLLISION_SHAPES_TYPES_ID_MAP;


#endif // _GLOBAL_COLLISION_SHAPES_TYPES_HPP_
