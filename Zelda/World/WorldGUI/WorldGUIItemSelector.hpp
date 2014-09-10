#ifndef _WORLD_GUI_ITEM_SELECTOR_
#define _WORLD_GUI_ITEM_SELECTOR_

#include "../../Common/GUI/GUIOverlay.hpp"
#include "../../Common/Message/MessageInjector.hpp"
#include "../Items/ItemTypes.hpp"

struct SItemStatus;

class CWorldGUIItemSelector : public CGUIOverlay {
private:
public:
  CWorldGUIItemSelector(CEntity *pParentEntity, CEGUI::Window *pParentWindow);

private:
  CEGUI::Window *createItem(int x, int y, float fButtonSize);

  CEGUI::String getItemID(int x, int y);
  CEGUI::Window *getWindowFromItem(EItemSlotTypes eItemSlotType);

  bool onSelectedItemChanged(const CEGUI::EventArgs &args);

  bool updateItemStatus(const SItemStatus &itemStatus);

  void handleMessage(const CMessage &message);
};

#endif // _WORLD_GUI_ITEM_SELECTOR_HPP_
