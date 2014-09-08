#include "WorldGUIItemViewer.hpp"
#include "WorldGUIItemSelector.hpp"

CWorldGUIItemViewer::CWorldGUIItemViewer(CEntity *pParentEntity, CEGUI::Window *pParentWindow)
  : CGUIPullMenu("world_gui_item_viewer", pParentEntity, pParentWindow, PMD_TOP) {

  new CWorldGUIItemSelector(this, getDragWindow());
}
