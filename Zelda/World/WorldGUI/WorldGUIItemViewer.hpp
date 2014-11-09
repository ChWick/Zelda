#ifndef _WORLD_GUI_ITEM_VIEWER_HPP_
#define _WORLD_GUI_ITEM_VIEWER_HPP_

#include "../../Common/GUI/GUIOverlay.hpp"
#include "../../Common/GUI/GUIPullMenu.hpp"
#include "../../Common/PauseManager/PauseCaller.hpp"
#include "../../Common/Input/GameInputListener.hpp"

class CWorldGUIItemSelector;

class CWorldGUIItemViewer : public CGUIPullMenu,
			    public CPauseCaller,
			    public CGameInputListener {
private:
  CWorldGUIItemSelector *m_pWorldGUIItemSelector;
public:
  CWorldGUIItemViewer(CEntity *pParentEntity, CEGUI::Window *pParentWindow);

  void receiveInputCommand(const CGameInputCommand &cmd);
private:
  void onPullStarted();
  void onClosed();
  void onOpened();
};

#endif // _WORLD_GUI_ITEM_VIEWER_HPP_
