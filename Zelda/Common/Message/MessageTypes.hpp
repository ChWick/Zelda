#ifndef _MESSAGE_TYPES_HPP_
#define _MESSAGE_TYPES_HPP_

#include "../Util/EnumIdMap.hpp"

enum EMessageTypes {
  MSG_SWITCH_MAP,
  MSG_TARGET_REACHED,
};

class CMessageTypesMap : public CEnumIdMap<EMessageTypes> {
public:
  CMessageTypesMap();
};

extern CMessageTypesMap MESSAGE_TYPES_MAP;

#endif // _MESSAGE_TYPES_HPP_
