#ifndef _ACTION_TYPES_HPP_
#define _ACTION_TYPES_HPP_

#include "../../../Util/EnumIdMap.hpp"

namespace events {


enum EActionTypes {
  ACTION_MESSAGE,
};

class CActionTypesMap : public CEnumIdMap<EActionTypes> {
public:
  CActionTypesMap();
};

extern CActionTypesMap ACTION_TYPES_MAP;
};

#endif // _ACTION_TYPES_HPP_
