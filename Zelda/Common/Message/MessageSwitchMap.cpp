#include "MessageSwitchMap.hpp"
#include "../Util/XMLHelper.hpp"

using namespace XMLHelper;

CMessageSwitchMap::CMessageSwitchMap(const tinyxml2::XMLElement *pElem)
  : CMessage(pElem),
    m_sMap(Attribute(pElem, "map")) {
}
