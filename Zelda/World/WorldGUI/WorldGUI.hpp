#ifndef _WORLD_GUI_HPP_
#define _WORLD_GUI_HPP_

#include "../../Common/GUI/GUIOverlay.hpp"

class CHUD;

class CWorldGUI : public CGUIOverlay {
private:
  CEGUI::Window *m_pRoot;
  CHUD *m_pHUD;
public:
  CWorldGUI(CEntity *pParentEntity);
  ~CWorldGUI();
};
#endif // _WORLD_GUI_HPP_
