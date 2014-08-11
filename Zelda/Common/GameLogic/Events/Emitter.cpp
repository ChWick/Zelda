#include "Emitter.hpp"
#include <tinyxml2.h>

namespace events {
  CEmitter::CEmitter(EEmitterTypes eType, const CEvent &owner)
    : m_eType(eType),
      m_Owner(owner)
  {}
  CEmitter::CEmitter(const tinyxml2::XMLElement *pElem, const CEvent &owner)
    : m_eType(EMITTER_TYPES_MAP.parseString(pElem->Attribute("type"))),
      m_Owner(owner) {
  }

};
