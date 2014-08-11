#include "Emitter.hpp"
#include <tinyxml2.h>

namespace events {
  CEmitter::CEmitter(EEmitterTypes eType)
    : m_eType(eType)
  {}
  CEmitter::CEmitter(const tinyxml2::XMLElement *pElem)
    : m_eType(EMITTER_TYPES_MAP.parseString(pElem->Attribute("type"))) {
  }

};
