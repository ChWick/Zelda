#include "../../Common/GUI/GUIManager.hpp"
#include "WorldGUI.hpp"
#include "../../Common/GUI/GUIPullMenu.hpp"
#include "HUD.hpp"

CWorldGUI::CWorldGUI(CEntity *pParentEntity)
  : CGUIOverlay("world_gui", pParentEntity, CGUIManager::getSingleton().getRoot(),
                CGUIManager::getSingleton().getRoot()->createChild("DefaultWindow", "world_gui_root")) {

  CGUIManager::getSingleton().addGUIOverlay(this);

  m_pHUD = new CHUD(this, m_pRoot);
  new CGUIPullMenu("test", this, m_pRoot, CGUIPullMenu::PMD_RIGHT, 400);
}
CWorldGUI::~CWorldGUI() {
  CGUIManager::getSingleton().removeGUIOverlay(this);
}
