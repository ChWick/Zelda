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
#include "ItemData.hpp"
#include "../Messages/MessageItem.hpp"
#include "../../Common/Message/MessageHandler.hpp"

std::vector<EItemVariantTypes> SItemStatus::getBestItem() const {
  if (vItems.size() == 0) {return vItems;} // no items available yet

  std::vector<EItemVariantTypes> items;
  int ucQuality = -1;

  for (size_t i = 0; i < vItems.size(); i++) {
    const SItemVariantData &data(ITEM_VARIANT_DATA_MAP.toData(vItems[i]));
    if (ucQuality == data.ucItemQuality) {
      items.push_back(vItems[i]);
    }
    else if (ucQuality < data.ucItemQuality) {
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
  m_Storage[ITEM_SLOT_LAMP].vItems.push_back(ITEM_VARIANT_LAMP);
}

void CItemStatusStorage::load() {
  CMessageHandler::getSingleton().addMessage(new CMessageItem(CMessageItem::IM_STATUS_LOADED, this));
}
