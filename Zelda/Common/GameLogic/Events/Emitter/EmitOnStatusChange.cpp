#include "EmitOnStatusChange.hpp"
#include "../../../Util/XMLHelper.hpp"

using namespace events;
using namespace XMLHelper;

CEmitOnStatusChange::CEmitOnStatusChange(const tinyxml2::XMLElement *pElem, const CEvent &owner) 
  : CEmitter(pElem, owner),
    mState(ENTITY_STATE_ID_MAP.parseString(Attribute(pElem, "state"))) {
}

CEmitOnStatusChange::CEmitOnStatusChange(const EEntityStateTypes state, const CEvent &owner) 
  : CEmitter(EMIT_ON_STATUS_CHANGE, owner),
    mState(state) {
}
