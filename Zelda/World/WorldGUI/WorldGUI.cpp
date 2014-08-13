#include "WorldGUI.hpp"
#include "../../Common/GUI/GUIManager.hpp"
#include "HUD.hpp"

CWorldGUI::CWorldGUI(CEntity *pParentEntity)
  : CGUIOverlay("world_gui", pParentEntity, CGUIManager::getSingleton().getRoot()) {

  CGUIManager::getSingleton().addGUIOverlay(this);

  m_pRoot = m_pParentWindow->createChild("DefaultWindow", "world_gui_root");

  m_pHUD = new CHUD(this, m_pRoot);
}
CWorldGUI::~CWorldGUI() {
  CGUIManager::getSingleton().removeGUIOverlay(this);

  delete m_pHUD;

  m_pRoot->destroy();
}
