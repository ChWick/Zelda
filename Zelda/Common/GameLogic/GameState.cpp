#include "GameState.hpp"
#include "GameStateManager.hpp"

CGameState::CGameState(EGameStateTypes eType)
  : CEntity(GAME_STATE_ID_MAP.toString(eType), eType, CGameStateManager::getSingletonPtr()) {
}
