#ifndef _WORLD_GUI_ITEM_VIEWER_HPP_
#define _WORLD_GUI_ITEM_VIEWER_HPP_

#include "../../Common/GUI/GUIOverlay.hpp"
#include "../../Common/GUI/GUIPullMenu.hpp"

class CWorldGUIItemViewer : public CGUIPullMenu {
public:
  CWorldGUIItemViewer(CEntity *pParentEntity, CEGUI::Window *pParentWindow);
};

#endif // _WORLD_GUI_ITEM_VIEWER_HPP_
