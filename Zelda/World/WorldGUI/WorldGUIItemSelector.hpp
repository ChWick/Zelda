#ifndef _WORLD_GUI_ITEM_SELECTOR_
#define _WORLD_GUI_ITEM_SELECTOR_

#include "../../Common/GUI/GUIOverlay.hpp"
#include "../../Common/Message/MessageInjector.hpp"
#include "../Items/ItemTypes.hpp"
#include "../../Common/Input/GameInputListener.hpp"

struct SItemStatus;

class CWorldGUIItemSelector : public CGUIOverlay, public CGameInputListener {
private:
  EItemSlotTypes m_eCurrentItemSlot;
public:
  CWorldGUIItemSelector(CEntity *pParentEntity, CEGUI::Window *pParentWindow);

private:
  CEGUI::Window *createItem(int x, int y, float fButtonSize);

  CEGUI::String getItemID(int x, int y);
  CEGUI::Window *getWindowFromItem(EItemSlotTypes eItemSlotType);

  bool onSelectedItemChanged(const CEGUI::EventArgs &args);

  bool updateItemStatus(const SItemStatus &itemStatus);

  void handleMessage(const CMessage &message);
  void receiveInputCommand(const CGameInputCommand &cmd);

  void selectFirstAvailable();
  void selectNextLeft();
  void selectNextRight();
  void selectNextUp();
  void selectNextDown();
};

#endif // _WORLD_GUI_ITEM_SELECTOR_HPP_
