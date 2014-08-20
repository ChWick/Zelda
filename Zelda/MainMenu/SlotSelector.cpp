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
  delete getParent(GAME_STATE_ID_MAP.toString(GST_MAIN_MENU));

  // create world as new game state
  new CWorld();

  return true;
}

bool CSlotSelector::onExitClicked(const CEGUI::EventArgs &args) {
  CGame::getSingleton().requestShutDown();
  return true;
}
