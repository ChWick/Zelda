#ifndef GAME_STATE_TYPES_HPP
#define GAME_STATE_TYPES_HPP

#include "Util/EnumIdMap.hpp"

enum EGameStateTypes {
  GST_WORLD,
  GST_MAIN_MENU
};

class CGameStateIdMap : public CEnumIdMap<EGameStateTypes> {
public:
  CGameStateIdMap();
};

extern CGameStateIdMap GAME_STATE_ID_MAP;

#endif // GAME_STATE_TYPES_HPP
