#include "EmitOnCollision.hpp"
#include "../../Util/XMLHelper.hpp"
#include "Event.hpp"
#include "../Entity.hpp"

namespace events {

CEmitOnCollision::CEmitOnCollision(const tinyxml2::XMLElement *pElem, const CEvent &owner)
  : CEmitter(pElem, owner),
    m_ID(XMLHelper::Attribute(pElem, "id")) {
}

bool CEmitOnCollision::isFiring(float tpf) {
  return m_Owner.getOwner().collidesWith(m_ID);
}

};
