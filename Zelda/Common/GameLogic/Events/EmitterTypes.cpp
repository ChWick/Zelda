#include "EmitterTypes.hpp"

namespace events {
CEmitterTypesMap::CEmitterTypesMap() {
  m_Map[EMIT_ON_COLLISION] = "collision";
}

CEmitterTypesMap EMITTER_TYPES_MAP;
};
