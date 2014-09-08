#include "WorldGUIItemSelector.hpp"

CWorldGUIItemSelector::CWorldGUIItemSelector(CEntity *pParentEntity, CEGUI::Window *pParentWindow)
  : CGUIOverlay("world_gui_item_selector", pParentEntity, pParentWindow, pParentWindow->createChild("OgreTray/Group", "item_selector")) {

  m_pRoot->setText("ITEM");
}
