#include "GameStateTypes.hpp"


CGameStateIdMap::CGameStateIdMap() {
  m_Map[GST_WORLD] = "world";
  m_Map[GST_MAIN_MENU] = "main_menu";
}

CGameStateIdMap GAME_STATE_ID_MAP;
