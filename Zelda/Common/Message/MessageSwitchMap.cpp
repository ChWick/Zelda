#include "MessageSwitchMap.hpp"
#include "../Util/XMLHelper.hpp"

using namespace XMLHelper;

CMessageSwitchMap::CMessageSwitchMap(const tinyxml2::XMLElement *pElem)
  : CMessage(pElem),
    m_sMap(Attribute(pElem, "map")),
    m_eStatus(INJECT),
    m_pFromMap(nullptr),
    m_pToMap(nullptr) {
}

CMessageSwitchMap::CMessageSwitchMap(const std::string &sMap,
                    ESwitchMapStatus eStatus,
                    const CMap *pFromMap,
                    const CMap *pToMap)
  : CMessage(MSG_SWITCH_MAP),
    m_sMap(sMap),
    m_eStatus(eStatus),
    m_pFromMap(pFromMap),
    m_pToMap(pToMap) {
}
