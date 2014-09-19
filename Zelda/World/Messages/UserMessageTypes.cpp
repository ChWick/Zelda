#include "UserMessageTypes.hpp"

CUserMessageTypesMap::CUserMessageTypesMap() {
  m_Map[MSG_ITEM] = "item";
  m_Map[MSG_SHOW_TEXT] = "show_text";
}

CUserMessageTypesMap USER_MESSAGE_TYPES_MAP;
