#include "../Common/GUI/GUIManager.hpp"
#include "MainMenu.hpp"
#include "SlotSelector.hpp"

CMainMenu::CMainMenu()
: CGameState(GST_MAIN_MENU) {
  m_pRootWindow = CGUIManager::getSingleton().getRoot()->createChild("DefaultWindow", "main_menu_root");

  new CSlotSelector(this, m_pRootWindow);
}
CMainMenu::~CMainMenu() {
  m_pRootWindow->destroy();
}
