#ifndef _EMIT_ON_COLLISION_HPP_
#define _EMIT_ON_COLLISION_HPP_

#include "Emitter.hpp"
#include "../../Util/XMLHelper.hpp"

namespace events {
  class CEmitOnCollision : public CEmitter {
  private:
    const std::string &m_ID;
  public:
    CEmitOnCollision(const tinyxml2::XMLElement *pElem)
      : CEmitter(pElem),
        m_ID(XMLHelper::Attribute(pElem, "id")) {
      }

    const std::string &getID() const {return m_ID;}
  };
};

#endif // _EMIT_ON_COLLISION_HPP_
