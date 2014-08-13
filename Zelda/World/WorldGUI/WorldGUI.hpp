#ifndef _WORLD_GUI_HPP_
#define _WORLD_GUI_HPP_

#include "../Common/GUI/GUIOverlay.hpp"

class CWorldGUI : public CGUIOverlay {
private:
  CEGUI::Window *m_pRoot;
public:
  CWorldGUI(CEntity *pParentEntity);
  ~CWorldGUI();
};
#endif // _WORLD_GUI_HPP_
