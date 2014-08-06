#ifndef _SLOT_SELECTOR_HPP_
#define _SLOT_SELECTOR_HPP_


#include "../Common/GUI/GUIOverlay.hpp"

class CSlotSelector : public CGUIOverlay {
public:
  CSlotSelector(CEntity *pParent, CEGUI::Window *pParentWindow);

protected:
  bool onStartClicked(const CEGUI::EventArgs &args);
  bool onExitClicked(const CEGUI::EventArgs &args);
};
#endif // _SLOT_SELECTOR_HPP_
