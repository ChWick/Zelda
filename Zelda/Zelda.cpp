#include "Common/GUI/GUIManager.hpp"
#include "Zelda.hpp"
#include "Common/GameLogic/GameStateManager.hpp"
#include "MainMenu/MainMenu.hpp"
#include "World/World.hpp"
#include "Common/Log.hpp"

void CZelda::chooseGameState() {
  LOGV("choosing came state");
  //new CMainMenu();
  new CWorld();
  //m_pGameStateManager->changeGameState(CGameStateManager::GS_MAIN_MENU);
}

void CZelda::postGUIManagerInitialised() {
  // add default/global imagesets
  CGUIManager::getSingleton().addImagesetResource(CGUIManager::SImagesetResource("hud", "hud.png"));
}

bool CZelda::keyPressed( const OIS::KeyEvent &arg ) {
  // change camera with F1 F2
  if (arg.key == OIS::KC_F1) {
    m_pMainViewPort->setCamera(mSceneMgr->getCamera("GameCamera"));
  }
  else if (arg.key == OIS::KC_F2) {
    m_pMainViewPort->setCamera(mSceneMgr->getCamera("WorldCamera"));
  }
  else {
    return CGame::keyPressed(arg);
  }
  return true;
}
