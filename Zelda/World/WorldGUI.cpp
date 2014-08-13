#include "WorldGUI.hpp"
#include "../Common/GUI/GUIManager.hpp"

CWorldGUI::CWorldGUI(CEntity *pParentEntity)
  : CGUIOverlay("world_gui", pParentEntity, CGUIManager::getSingleton().getRoot()) {

  CGUIManager::getSingleton().addGUIOverlay(this);

  m_pRoot = m_pParentWindow->createChild("DefaultWindow", "world_gui_root");
}
CWorldGUI::~CWorldGUI() {
  CGUIManager::getSingleton().removeGUIOverlay(this);

  m_pRoot->destroy();
}
