/*****************************************************************************
 * Copyright 2014 Christoph Wick
 *
 * This file is part of Zelda.
 *
 * Zelda is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * Zelda is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * Zelda. If not, see http://www.gnu.org/licenses/.
 *****************************************************************************/

#include "Common/GUI/GUIManager.hpp"
#include "Zelda.hpp"
#include "Common/GameLogic/GameStateManager.hpp"
#include "MainMenu/MainMenu.hpp"
#include "World/World.hpp"
#include "Common/Log.hpp"
#include "World/Character/PersonTypes.hpp"

void CZelda::initEnumIdMaps() {
  PERSON_TYPE_ID_MAP.init();
}

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
