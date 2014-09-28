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

#ifndef _MESSAGE_SWITCH_MAP_HPP_
#define _MESSAGE_SWITCH_MAP_HPP_

#include "Message.hpp"
#include <string>
#include "../Util/EnumIdMap.hpp"

enum ESwitchMapTypes {
  SMT_MOVE_CAMERA,
  SMT_FADE_ELLIPTIC,
  SMT_FADE_ALPHA,
};

class CSwitchMapTypes : public CEnumIdMap<ESwitchMapTypes> {
public:
  CSwitchMapTypes();
};

extern const CSwitchMapTypes SWITCH_MAP_TYPES_ID_MAP;


class CMap;

class CMessageSwitchMap : public CMessage {
public:
  enum ESwitchMapStatus {
    INJECT,
    SWITCHING,
    FINISHED,
  };
protected:
  const std::string m_sMap;
  ESwitchMapStatus m_eStatus;
  ESwitchMapTypes m_eSwitchMapType;

  const CMap *m_pFromMap;
  const CMap *m_pToMap;
public:
  CMessageSwitchMap(const tinyxml2::XMLElement *pElem);
  CMessageSwitchMap(const std::string &sMap,
                    ESwitchMapStatus eStatus,
                    ESwitchMapTypes eSwitchMapType,
                    const CMap *pFromMap,
                    const CMap *pToMap);

  const std::string &getMap() const {return m_sMap;}
  const ESwitchMapStatus getStatus() const {return m_eStatus;}
  const ESwitchMapTypes getSwitchMapType() const {return m_eSwitchMapType;}
  const CMap *getFromMap() const {return m_pFromMap;}
  const CMap *getToMap() const {return m_pToMap;}
};

#endif // _MESSAGE_SWITCH_MAP_HPP_
