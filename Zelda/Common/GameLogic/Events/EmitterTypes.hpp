#ifndef _EMITTER_TYPES_HPP_
#define _EMITTER_TYPES_HPP_

#include "../../Util/EnumIdMap.hpp"

namespace events {


enum EEmitterTypes {
  EMIT_ON_COLLISION,
};

class CEmitterTypesMap : public CEnumIdMap<EEmitterTypes> {
public:
  CEmitterTypesMap();
};

extern CEmitterTypesMap EMITTER_TYPES_MAP;
};

#endif // _EMITTER_TYPES_HPP_
