#ifndef __Zelda__GUIDebugPullMenu__
#define __Zelda__GUIDebugPullMenu__

#include "GUIPullMenu.hpp"

class CGUIDebugPullMenu : public CGUIPullMenu {
private:
  CEGUI::Window *m_pContent;
public:
  CGUIDebugPullMenu(CEntity *pParentEntity,
                    CEGUI::Window *pParentWindow,
                    EPullMenuPositions ePosition);

private:
  CEGUI::Window *createButton(const CEGUI::String &wnd, const CEGUI::String &id, const CEGUI::String &text, float &fPos);

  bool onTogglePhysics(const CEGUI::EventArgs &args);
};

#endif /* defined(__Zelda__GUIDebugPullMenu__) */
