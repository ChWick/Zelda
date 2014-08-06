#include "Zelda.hpp"
#include "Common/GameLogic/GameStateManager.hpp"
#include "MainMenu/MainMenu.hpp"
#include "World/World.hpp"

void CZelda::chooseGameState() {
  //new CMainMenu();
  new CWorld();
  //m_pGameStateManager->changeGameState(CGameStateManager::GS_MAIN_MENU);
}
