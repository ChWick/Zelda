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

#include "ItemStatus.hpp"
#include <vector>
#include "ItemData.hpp"
#include "../Messages/MessageItem.hpp"
#include "../../Common/Message/MessageHandler.hpp"

std::vector<EItemVariantTypes> SItemStatus::getBestItem() const {
  if (vItems.size() == 0) {return vItems;}  // no items available yet

  std::vector<EItemVariantTypes> items;
  int ucQuality = -1;

  for (size_t i = 0; i < vItems.size(); i++) {
    const SItemVariantData &data(ITEM_VARIANT_DATA_MAP.toData(vItems[i]));
    if (ucQuality == data.ucItemQuality) {
      items.push_back(vItems[i]);
    } else if (ucQuality < data.ucItemQuality) {
      ucQuality = data.ucItemQuality;
      items.clear();
      items.push_back(vItems[i]);
    }
  }

  return items;
}

CItemStatusStorage::CItemStatusStorage() {
  // initialize empty storage
  m_Storage.resize(ITEM_SLOT_COUNT);
  for (size_t i = 0; i < m_Storage.size(); i++) {
    m_Storage[i].eItemPlace = static_cast<EItemSlotTypes>(i);
  }

  // test, add fire rod
  m_Storage[ITEM_SLOT_FIREROD].vItems.push_back(ITEM_VARIANT_FIREROD);
  m_Storage[ITEM_SLOT_BOMB].vItems.push_back(ITEM_VARIANT_BOMB);
  m_Storage[ITEM_SLOT_BOTTLE].vItems.push_back(ITEM_VARIANT_BOTTLE_BEE);
  m_Storage[ITEM_SLOT_BOTTLE].vItems.push_back(ITEM_VARIANT_BOTTLE_EMPTY);
}

void CItemStatusStorage::load() {
  CMessageHandler::getSingleton().addMessage(
      std::make_shared<CMessageItem>(__MSG_LOCATION__, CMessageItem::IM_STATUS_LOADED, this));
}

void CItemStatusStorage::pickupItem(EItemVariantTypes eItemVariantType) {
  EItemSlotTypes slot = ITEM_SLOT_DATA_MAP.getFromItemVariant(eItemVariantType);
  ASSERT(slot != ITEM_SLOT_COUNT);
  m_Storage[slot].vItems.push_back(eItemVariantType);

  CMessageHandler::getSingleton().addMessage(
      std::make_shared<CMessageItem>(
          __MSG_LOCATION__,
          CMessageItem::IM_STATUS_CHANGED,
          this,
          slot,
          eItemVariantType));
}

bool CItemStatusStorage::hasItem(EItemVariantTypes eItemVariantType) const {
  ASSERT(eItemVariantType != ITEM_VARIANT_COUNT);
  EItemSlotTypes slot = ITEM_SLOT_DATA_MAP.getFromItemVariant(eItemVariantType);
  ASSERT(slot != ITEM_SLOT_COUNT);
  auto &v(m_Storage[slot].vItems);
  return std::find(v.begin(), v.end(), eItemVariantType) != v.end();
}
