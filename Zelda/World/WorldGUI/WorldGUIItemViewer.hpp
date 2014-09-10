#ifndef _WORLD_GUI_ITEM_VIEWER_HPP_
#define _WORLD_GUI_ITEM_VIEWER_HPP_

#include "../../Common/GUI/GUIOverlay.hpp"
#include "../../Common/GUI/GUIPullMenu.hpp"
#include "../../Common/PauseManager/PauseCaller.hpp"

class CWorldGUIItemSelector;

class CWorldGUIItemViewer : public CGUIPullMenu, public CPauseCaller {
private:
  CWorldGUIItemSelector *m_pWorldGUIItemSelector;
public:
  CWorldGUIItemViewer(CEntity *pParentEntity, CEGUI::Window *pParentWindow);

private:
  void onPullStarted();
  void onClosed();
  void onOpened();
};

#endif // _WORLD_GUI_ITEM_VIEWER_HPP_
