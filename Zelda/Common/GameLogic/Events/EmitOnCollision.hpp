#ifndef _EMIT_ON_COLLISION_HPP_
#define _EMIT_ON_COLLISION_HPP_

#include "Emitter.hpp"

namespace events {
  class CEmitOnCollision : public CEmitter {
  private:
    const std::string m_ID;
  public:
    CEmitOnCollision(const tinyxml2::XMLElement *pElem, const CEvent &owner);

    bool isFiring(float tpf);

    const std::string &getID() const {return m_ID;}
  };
};

#endif // _EMIT_ON_COLLISION_HPP_
