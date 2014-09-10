#include "ItemStatus.hpp"
#include "ItemData.hpp"
#include "../Messages/MessageItem.hpp"
#include "../../Common/Message/MessageHandler.hpp"

std::vector<EItemVariantTypes> SItemStatus::getBestItem() const {
  if (vItems.size() == 0) {return vItems;} // no items available yet

  std::vector<EItemVariantTypes> items;
  int ucQuality = -1;

  for (int i = 0; i < vItems.size(); i++) {
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
  for (int i = 0; i < m_Storage.size(); i++) {
    m_Storage[i].eItemPlace = static_cast<EItemSlotTypes>(i);
  }

  // test, add fire rod
  m_Storage[ITEM_SLOT_FIREROD].vItems.push_back(ITEM_VARIANT_FIREROD);
  m_Storage[ITEM_SLOT_BOMB].vItems.push_back(ITEM_VARIANT_BOMB);
  m_Storage[ITEM_SLOT_BOTTLE].vItems.push_back(ITEM_VARIANT_BOTTLE_BEE);
  m_Storage[ITEM_SLOT_BOTTLE].vItems.push_back(ITEM_VARIANT_BOTTLE_EMPTY);
}

void CItemStatusStorage::load() {
  CMessageHandler::getSingleton().addMessage(new CMessageItem(CMessageItem::IM_STATUS_LOADED, this));
}
