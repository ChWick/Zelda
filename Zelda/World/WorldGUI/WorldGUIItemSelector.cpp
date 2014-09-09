#include "WorldGUIItemSelector.hpp"
#include "../Messages/UserMessageTypes.hpp"
#include "../Messages/MessageItem.hpp"
#include "../Items/ItemStatus.hpp"
#include "../../Common/Util/Assert.hpp"
#include "../Items/ItemData.hpp"

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

CEGUI::Window *CWorldGUIItemSelector::getWindowFromItem(EItemTypes eItemType) {
  ASSERT(eItemType < ITEM_SELECT_COUNT);
  return m_pRoot->getChild(getItemID(eItemType % 5, eItemType / 5));
}

bool CWorldGUIItemSelector::onSelectedItemChanged(const CEGUI::EventArgs &args) {
  return true;
}

void CWorldGUIItemSelector::updateItemStatus(const SItemStatus &itemStatus) {
  ASSERT(itemStatus.eItemPlace < ITEM_SELECT_COUNT);
  Window *pWnd = getWindowFromItem(itemStatus.eItemPlace);

  std::vector<EItemTypes> items = itemStatus.getBestItem();
  if (items.size() == 0) {
    pWnd->setVisible(false);
    return;
  }
  else if (items.size() == 1) {
    pWnd->setVisible(true);
    pWnd->setProperty("Image", "hud/" + ITEM_DATA_MAP.toData(items.front()).sImagesetName);
    return;
  }
}

void CWorldGUIItemSelector::handleMessage(const CMessage &message) {
  if (message.getType() == MSG_ITEM) {
    const CMessageItem &messageItem(dynamic_cast<const CMessageItem&>(message));
    ASSERT(messageItem.getStatusStorage());

    for (int i = 0; i < ITEM_SELECT_COUNT; i++) {
      updateItemStatus(messageItem.getStatusStorage()->getStatus(static_cast<EItemTypes>(i)));
    }
  }
}
