#include "MessageTypes.hpp"

CMessageTypesMap::CMessageTypesMap() {
  m_Map[MSG_SWITCH_MAP] = "switch_map";
  m_Map[MSG_TARGET_REACHED] = "target_reached";
}

CMessageTypesMap MESSAGE_TYPES_MAP;
