#include "WorldGUIItemSelector.hpp"
#include "../Messages/UserMessageTypes.hpp"
#include "../Messages/MessageItem.hpp"
#include "../Items/ItemStatus.hpp"
#include "../../Common/Util/Assert.hpp"
#include "../Items/ItemData.hpp"
#include "../../Common/Message/MessageHandler.hpp"
#include "../../Common/Input/GameInputCommand.hpp"

using namespace CEGUI;

CWorldGUIItemSelector::CWorldGUIItemSelector(CEntity *pParentEntity, CEGUI::Window *pParentWindow)
  : CGUIOverlay("world_gui_item_selector", pParentEntity, pParentWindow, pParentWindow->createChild("OgreTray/Group", "item_selector")),
    CGameInputListener(true),
    m_eCurrentItemSlot(ITEM_SLOT_COUNT) {

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
  m_eCurrentItemSlot = pItemStatus->eItemPlace;
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

      for (int i = 0; i < ITEM_SLOT_COUNT; i++) {
        updateItemStatus(messageItem.getStatusStorage()->getStatus(static_cast<EItemSlotTypes>(i)));
      }

      selectFirstAvailable();
    }
    else if (messageItem.getItemMessageType() == CMessageItem::IM_SELECTION_CHANGED) {
      //dynamic_cast<ToggleButton*>(getWindowFromItem(messageItem))->setSelected(true);
    }
  }
}

void CWorldGUIItemSelector::receiveInputCommand(const CGameInputCommand &cmd) {
  if (cmd.getState() != GIS_PRESSED) {return;}
  switch (cmd.getType()) {
  case GIC_LEFT:
    selectNextLeft();
    break;
  case GIC_RIGHT:
    selectNextRight();
    break;
  case GIC_FRONT:
    selectNextUp();
    break;
  case GIC_REAR:
    selectNextDown();
    break;
  default:
    break;
  }
}

void CWorldGUIItemSelector::selectFirstAvailable() {
  for (int i = 0; i < ITEM_SLOT_COUNT; i++) {
    if (getWindowFromItem(static_cast<EItemSlotTypes>(i))->isVisible()) {
      dynamic_cast<ToggleButton*>(getWindowFromItem(static_cast<EItemSlotTypes>(i)))->setSelected(true);
      return;
    }
  }
}

void CWorldGUIItemSelector::selectNextLeft() {
  if (m_eCurrentItemSlot == ITEM_SLOT_COUNT) {selectFirstAvailable(); return;}

  for (int i = 1; i < ITEM_SLOT_COUNT; i++) {
    EItemSlotTypes eNewSlot = static_cast<EItemSlotTypes>((m_eCurrentItemSlot - i + ITEM_SLOT_COUNT) % ITEM_SLOT_COUNT);
    if (getWindowFromItem(eNewSlot)->isVisible()) {
      dynamic_cast<ToggleButton*>(getWindowFromItem(eNewSlot))->setSelected(true);
      return;
    }
  }
}

void CWorldGUIItemSelector::selectNextRight() {
  if (m_eCurrentItemSlot == ITEM_SLOT_COUNT) {selectFirstAvailable(); return;}

  for (int i = 1; i < ITEM_SLOT_COUNT; i++) {
    EItemSlotTypes eNewSlot = static_cast<EItemSlotTypes>((i + m_eCurrentItemSlot) % ITEM_SLOT_COUNT);
    if (getWindowFromItem(eNewSlot)->isVisible()) {
      dynamic_cast<ToggleButton*>(getWindowFromItem(eNewSlot))->setSelected(true);
      return;
    }
  }
}

void CWorldGUIItemSelector::selectNextUp() {
  if (m_eCurrentItemSlot == ITEM_SLOT_COUNT) {selectFirstAvailable(); return;}

  for (int i = 1; i < ITEM_SLOT_COUNT; i++) {
    EItemSlotTypes eNewSlot = static_cast<EItemSlotTypes>((m_eCurrentItemSlot - i * 5+ ITEM_SLOT_COUNT) % ITEM_SLOT_COUNT);
    if (getWindowFromItem(eNewSlot)->isVisible()) {
      dynamic_cast<ToggleButton*>(getWindowFromItem(eNewSlot))->setSelected(true);
      return;
    }
  }
}

void CWorldGUIItemSelector::selectNextDown() {
  for (int i = 1; i < ITEM_SLOT_COUNT; i++) {
    EItemSlotTypes eNewSlot = static_cast<EItemSlotTypes>((i * 5 + m_eCurrentItemSlot) % ITEM_SLOT_COUNT);
    if (getWindowFromItem(eNewSlot)->isVisible()) {
      dynamic_cast<ToggleButton*>(getWindowFromItem(eNewSlot))->setSelected(true);
      return;
    }
  }
}
