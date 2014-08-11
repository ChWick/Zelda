#ifndef _EMITTER_HPP_
#define _EMITTER_HPP_

#include "EmitterTypes.hpp"

namespace tinyxml2 {
  class XMLElement;
}
namespace events {

  class CEmitter {
  protected:
    const EEmitterTypes m_eType;

  public:
    CEmitter(EEmitterTypes eType);
    CEmitter(const tinyxml2::XMLElement *pElem);
    virtual ~CEmitter() {}

    EEmitterTypes getType() {return m_eType;}
  };
};
#endif // _EMITTER_HPP_
