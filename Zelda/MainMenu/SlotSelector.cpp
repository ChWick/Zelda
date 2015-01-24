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

#include "SlotSelector.hpp"
#include "../Common/Game.hpp"
#include "../Common/GameLogic/GameStateTypes.hpp"
#include "../World/World.hpp"

using namespace CEGUI;

CSlotSelector::CSlotSelector(CEntity *pParent, CEGUI::Window *pParentWindow)
: CGUIOverlay("slot_selector", pParent, pParentWindow, pParentWindow->createChild("DefaultWindow", "SlotSelector_Root")) {
  Window *pStartButton = m_pRoot->createChild("OgreTray/Button", "Start");
  pStartButton->setText("Start");
  pStartButton->setPosition(UVector2(UDim(0, 10), UDim(0, 10)));
  pStartButton->subscribeEvent(PushButton::EventClicked,
                               Event::Subscriber(&CSlotSelector::onStartClicked, this));

  Window *pExitButton = m_pRoot->createChild("OgreTray/Button", "Exit");
  pExitButton->setText("Exit");
  pExitButton->setPosition(UVector2(UDim(0, 10), UDim(0, 110)));
  pExitButton->subscribeEvent(PushButton::EventClicked,
                              Event::Subscriber(&CSlotSelector::onExitClicked, this));

}

bool CSlotSelector::onStartClicked(const CEGUI::EventArgs &args) {
  // delete the game state: main menu
  delete getParent(CGameStateIdMap::getSingleton().toString(GST_MAIN_MENU));

  // create world as new game state
  new CWorld();

  return true;
}

bool CSlotSelector::onExitClicked(const CEGUI::EventArgs &args) {
  CGame::getSingleton<CGame>().requestShutDown();
  return true;
}
