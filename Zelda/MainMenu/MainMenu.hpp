#ifndef _MAIN_MENU_HPP_
#define _MAIN_MENU_HPP_

#include <CEGUI/CEGUI.h>
#include "../Common/GameLogic/GameState.hpp"

class CMainMenu : public CGameState {
private:
  CEGUI::Window *m_pRootWindow;
public:
  CMainMenu();
  ~CMainMenu();
};
#endif // _MAIN_MENU_HPP_

