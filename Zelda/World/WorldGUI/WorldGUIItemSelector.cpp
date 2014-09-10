#include "WorldGUIItemSelector.hpp"
#include "../Messages/UserMessageTypes.hpp"
#include "../Messages/MessageItem.hpp"
#include "../Items/ItemStatus.hpp"
#include "../../Common/Util/Assert.hpp"
#include "../Items/ItemData.hpp"
#include "../../Common/Message/MessageHandler.hpp"

using namespace CEGUI;

CWorldGUIItemSelector::CWorldGUIItemSelector(CEntity *pParentEntity, CEGUI::Window *pParentWindow)
  : CGUIOverlay("world_gui_item_selector", pParentEntity, pParentWindow, pParentWindow->createChild("OgreTray/Group", "item_selector")) {

  m_pRoot->setText("ITEM");

  //float fButtonSize = std::min<float>(m_pRoot->getInnerRectClipper().getSize().d_width / 5, m_pRoot->getInnerRectClipper().getSize().d_height / 4);
  float fButtonSize = m_pRoot->getPixelSize().d_height / 4;
  for (int x = 0; x < 5; x++) {
    for (int y = 0; y < 4; y++) {
      createItem(x, y, fButtonSize);
    }
  }
}

CEGUI::Window *CWorldGUIItemSelector::createItem(int x, int y, float fButtonSize) {
  Window *pItemWindow = m_pRoot->createChild("OgreTray/ToggleRadioButton", getItemID(x, y));
  pItemWindow->setPosition(UVector2(UDim(0, x * fButtonSize), UDim(0, y * fButtonSize)));
  pItemWindow->setSize(USize(UDim(0, fButtonSize), UDim(0, fButtonSize)));
  pItemWindow->setProperty("Image", "hud/Bow");
  pItemWindow->subscribeEvent(ToggleButton::EventSelectStateChanged, Event::Subscriber(&CWorldGUIItemSelector::onSelectedItemChanged, this));

  return pItemWindow;
}

CEGUI::String CWorldGUIItemSelector::getItemID(int x, int y) {
  return "Item(" + PropertyHelper<int>::toString(x) + "," + PropertyHelper<int>::toString(y) + ")";
}

CEGUI::Window *CWorldGUIItemSelector::getWindowFromItem(EItemSlotTypes eItemSlotType) {
  ASSERT(eItemSlotType < ITEM_SLOT_COUNT);
  return m_pRoot->getChild(getItemID(eItemSlotType % 5, eItemSlotType / 5));
}

bool CWorldGUIItemSelector::onSelectedItemChanged(const CEGUI::EventArgs &args) {
  const WindowEventArgs &wnd_args(dynamic_cast<const WindowEventArgs&>(args));
  ASSERT(wnd_args.window->getUserData());

  const SItemStatus *pItemStatus(static_cast<SItemStatus*>(wnd_args.window->getUserData()));
  CMessageHandler::getSingleton().addMessage(new CMessageItem(CMessageItem::IM_SELECTION_CHANGED, pItemStatus->getBestItem().front()));

  return true;
}

bool CWorldGUIItemSelector::updateItemStatus(const SItemStatus &itemStatus) {
  ASSERT(itemStatus.eItemPlace < ITEM_SLOT_COUNT);
  Window *pWnd = getWindowFromItem(itemStatus.eItemPlace);
  pWnd->setUserData(const_cast<SItemStatus*>(&itemStatus));

  std::vector<EItemVariantTypes> items = itemStatus.getBestItem();
  if (items.size() == 0) {
    pWnd->setVisible(false);
    return false;
  }
  else if (items.size() == 1) {
    pWnd->setVisible(true);
    pWnd->setProperty("Image", "hud/" + ITEM_VARIANT_DATA_MAP.toData(items.front()).sImagesetName);
  }
  else {
    pWnd->setVisible(true);
    pWnd->setProperty("Image", "hud/" + ITEM_VARIANT_DATA_MAP.toData(items.front()). sImagesetName);
  }

  return true;
}

void CWorldGUIItemSelector::handleMessage(const CMessage &message) {
  if (message.getType() == MSG_ITEM) {
    const CMessageItem &messageItem(dynamic_cast<const CMessageItem&>(message));
    if (messageItem.getItemMessageType() == CMessageItem::IM_STATUS_LOADED) {
      ASSERT(messageItem.getStatusStorage());

      EItemSlotTypes eFirstAvailableItem = ITEM_SLOT_COUNT;

      for (int i = 0; i < ITEM_SLOT_COUNT; i++) {
        bool bAvailable = updateItemStatus(messageItem.getStatusStorage()->getStatus(static_cast<EItemSlotTypes>(i)));
        if (eFirstAvailableItem == ITEM_SLOT_COUNT && bAvailable) {
          eFirstAvailableItem = static_cast<EItemSlotTypes>(i);
        }
      }

      dynamic_cast<ToggleButton*>(getWindowFromItem(eFirstAvailableItem))->setSelected(true);
    }
    else if (messageItem.getItemMessageType() == CMessageItem::IM_SELECTION_CHANGED) {
      //dynamic_cast<ToggleButton*>(getWindowFromItem(messageItem))->setSelected(true);
    }
  }
}
