#include "ActionTypes.hpp"

namespace events {
CActionTypesMap::CActionTypesMap() {
  m_Map[ACTION_MESSAGE] = "message";
}

CActionTypesMap ACTION_TYPES_MAP;
};
