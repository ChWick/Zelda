/*****************************************************************************
 * Copyright 2014 Christoph Wick
 *
 * This file is part of Zelda.
 *
 * Zelda is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * Zelda is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * Zelda. If not, see http://www.gnu.org/licenses/.
 *****************************************************************************/

#include "MessageSwitchMap.hpp"
#include "../Util/XMLHelper.hpp"

using namespace XMLHelper;

CSwitchMapTypes::CSwitchMapTypes() {
  m_Map[SMT_MOVE_CAMERA] = "move_camera";
  m_Map[SMT_FADE_ELLIPTIC] = "fade_elliptic";
  m_Map[SMT_FADE_ALPHA] = "fade_alpha";
}

const CSwitchMapTypes SWITCH_MAP_TYPES_ID_MAP;


CMessageSwitchMap::CMessageSwitchMap(const tinyxml2::XMLElement *pElem)
  : CMessage(MSG_SWITCH_MAP, pElem),
    m_sMap(Attribute(pElem, "map")),
    m_eStatus(INJECT),
    m_eSwitchMapType(SWITCH_MAP_TYPES_ID_MAP.parseString(Attribute(pElem, "switch_map_type"))),
    m_pFromMap(nullptr),
    m_pToMap(nullptr) {
}

CMessageSwitchMap::CMessageSwitchMap(const std::string &sMap,
                    ESwitchMapStatus eStatus,
                    ESwitchMapTypes eSwitchMapType,
                    const CMap *pFromMap,
                    const CMap *pToMap)
  : CMessage(MSG_SWITCH_MAP),
    m_sMap(sMap),
    m_eStatus(eStatus),
    m_eSwitchMapType(eSwitchMapType),
    m_pFromMap(pFromMap),
    m_pToMap(pToMap) {
}
