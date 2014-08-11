#ifndef _EMITTER_HPP_
#define _EMITTER_HPP_

#include "EmitterTypes.hpp"

namespace tinyxml2 {
  class XMLElement;
}
namespace events {
  class CEvent;

  class CEmitter {
  protected:
    const EEmitterTypes m_eType;
    const CEvent &m_Owner;

  public:
    CEmitter(EEmitterTypes eType, const CEvent &owner);
    CEmitter(const tinyxml2::XMLElement *pElem, const CEvent &owner);
    virtual ~CEmitter() {}

    virtual bool isFiring(float tpf) {return false;}

    EEmitterTypes getType() {return m_eType;}
  };
};
#endif // _EMITTER_HPP_
