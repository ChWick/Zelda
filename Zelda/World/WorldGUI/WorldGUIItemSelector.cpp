/*****************************************************************************
 * Copyright 2014 Christoph Wick
 *
 * This file is part of Zelda.
 *
 * Zelda is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * Zelda is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * Zelda. If not, see http://www.gnu.org/licenses/.
 *****************************************************************************/

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
    CGameInputListener(false),
    m_eCurrentItemSlot(ITEM_SLOT_COUNT),
    m_pMultipleSelector(nullptr) {

  m_pRoot->setText("ITEM");

  //float fButtonSize = std::min<float>(m_pRoot->getInnerRectClipper().getSize().d_width / 5, m_pRoot->getInnerRectClipper().getSize().d_height / 4);
  float fButtonSize = m_pRoot->getPixelSize().d_height / 4;
  for (int x = 0; x < 5; x++) {
    for (int y = 0; y < 4; y++) {
      createItem(x, y, fButtonSize);
    }
  }

  stop();
}
CWorldGUIItemSelector::~CWorldGUIItemSelector() {
  if (m_pMultipleSelector) {
    delete m_pMultipleSelector;
    m_pMultipleSelector = nullptr;
  }
}

void CWorldGUIItemSelector::start() {
  setGameInputListenerEnabled(true);
}

void CWorldGUIItemSelector::stop() {
  setGameInputListenerEnabled(false);
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
  if (m_pMultipleSelector) {delete m_pMultipleSelector; m_pMultipleSelector = nullptr;}

  const WindowEventArgs &wnd_args(dynamic_cast<const WindowEventArgs&>(args));
  ASSERT(wnd_args.window->getUserData());

  const SItemStatus *pItemStatus(static_cast<SItemStatus*>(wnd_args.window->getUserData()));
  m_eCurrentItemSlot = pItemStatus->eItemPlace;
  std::vector<EItemVariantTypes> items(pItemStatus->getBestItem());
  ASSERT(items.size() > 0);
  if (items.size() == 1) {
    // just select the item
    CMessageHandler::getSingleton().addMessage(new CMessageItem(CMessageItem::IM_SELECTION_CHANGED, nullptr, m_eCurrentItemSlot, items.front()));
  } else {
    // display a selection window
    m_pMultipleSelector = new CWorldGUIItemSelectorMultipleSelect(items, m_pRoot, wnd_args.window->getPosition(), wnd_args.window->getPixelSize().d_width);
  }


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
    } else if (messageItem.getItemMessageType() == CMessageItem::IM_SELECTION_CHANGED) {
      //dynamic_cast<ToggleButton*>(getWindowFromItem(messageItem))->setSelected(true);
    } else if (messageItem.getItemMessageType() == CMessageItem::IM_STATUS_CHANGED) {
      ASSERT(messageItem.getStatusStorage());
      updateItemStatus(messageItem.getStatusStorage()->getStatus(messageItem.getItemSlotType()));
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

  if (m_pMultipleSelector) {
    m_pMultipleSelector->selectNextLeft();
    return;
  }

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

  if (m_pMultipleSelector) {
    m_pMultipleSelector->selectNextRight();
    return;
  }

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



// ===============================================================================================
// CWorldGUIItemSelectorMultipleSelect
// ===============================================================================================

CWorldGUIItemSelectorMultipleSelect::CWorldGUIItemSelectorMultipleSelect(const std::vector<EItemVariantTypes> items,
                                                                         CEGUI::Window *pParent, const CEGUI::UVector2 &vCenter, float fSize)
  : m_vItems(items),
    m_iSelectedItem(-1) {
  m_pRoot = pParent->createChild("DefaultWindow", "multiple_select");
  m_pRoot->setSize(USize(UDim(0, m_vItems.size() * fSize), UDim(0, fSize)));
  m_pRoot->setPosition(vCenter + UVector2(UDim(0, fSize / 3), UDim(0, fSize / 3)));
  m_pRoot->setAlwaysOnTop(true);

  for (size_t i = 0; i < m_vItems.size(); i++) {
    createButton(i);
  }

  selectNextRight();
}

CWorldGUIItemSelectorMultipleSelect::~CWorldGUIItemSelectorMultipleSelect() {
  m_pRoot->destroy();
}

void CWorldGUIItemSelectorMultipleSelect::selectNextRight() {
  if (m_iSelectedItem + 1 >= static_cast<int>(m_vItems.size())) {return;}
  m_iSelectedItem++;

  dynamic_cast<ToggleButton*>(m_pRoot->getChild(PropertyHelper<int>::toString(m_iSelectedItem)))->setSelected(true);
}

void CWorldGUIItemSelectorMultipleSelect::selectNextLeft() {
  if (m_iSelectedItem - 1 < 0) {return;}
  m_iSelectedItem--;

  dynamic_cast<ToggleButton*>(m_pRoot->getChild(PropertyHelper<int>::toString(m_iSelectedItem)))->setSelected(true);
}

void CWorldGUIItemSelectorMultipleSelect::createButton(int iIndex) {
  Window *pButton = m_pRoot->createChild("OgreTray/ToggleRadioButton", PropertyHelper<int>::toString(iIndex));
  pButton->setPosition(UVector2(UDim(iIndex * 1.f / m_vItems.size(), 0), UDim(0, 0)));
  pButton->setSize(USize(UDim(1.f / m_vItems.size(), 0), UDim(1, 0)));
  pButton->setProperty("Image", "hud/" + ITEM_VARIANT_DATA_MAP.toData(m_vItems[iIndex]).sImagesetName);
  pButton->setUserData(const_cast<EItemVariantTypes*>(&m_vItems[iIndex]));
  pButton->subscribeEvent(ToggleButton::EventSelectStateChanged, Event::Subscriber(&CWorldGUIItemSelectorMultipleSelect::onSelectedItemChanged, this));
}

bool CWorldGUIItemSelectorMultipleSelect::onSelectedItemChanged(const CEGUI::EventArgs &args) {
  const WindowEventArgs &wnd_args(dynamic_cast<const WindowEventArgs&>(args));
  ASSERT(wnd_args.window->getUserData());

  const EItemVariantTypes eItemVariant(*static_cast<EItemVariantTypes*>(wnd_args.window->getUserData()));
  CMessageHandler::getSingleton().addMessage(
      new CMessageItem(CMessageItem::IM_SELECTION_CHANGED,
                       nullptr,
                       ITEM_SLOT_DATA_MAP.getFromItemVariant(eItemVariant),
                       eItemVariant));

  return true;
}
