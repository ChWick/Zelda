#include "Action.hpp"
#include "../../../Util/XMLHelper.hpp"

using namespace XMLHelper;

namespace events {
  CAction::CAction(const tinyxml2::XMLElement *pElem, const CEvent &owner)
    : m_Type(ACTION_TYPES_MAP.parseString(Attribute(pElem, "type"))),
      m_Owner(owner) {
    }
};
