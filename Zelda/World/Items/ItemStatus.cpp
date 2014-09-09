#include "ItemStatus.hpp"
#include "ItemData.hpp"
#include "../Messages/MessageItem.hpp"
#include "../../Common/Message/MessageHandler.hpp"

std::vector<EItemTypes> SItemStatus::getBestItem() const {
  if (vItems.size() == 0) {return vItems;} // no items available yet

  std::vector<EItemTypes> items;
  int ucQuality = -1;

  for (int i = 0; i < vItems.size(); i++) {
    const SItemData &data(ITEM_DATA_MAP.toData(vItems[i]));
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
  m_Storage.resize(ITEM_SELECT_COUNT);
  for (int i = 0; i < m_Storage.size(); i++) {
    m_Storage[i].eItemPlace = static_cast<EItemTypes>(i);
  }

  // test, add fire rod
  m_Storage[ITEM_FIREROD].vItems.push_back(ITEM_FIREROD);
}

void CItemStatusStorage::load() {
  CMessageHandler::getSingleton().addMessage(new CMessageItem(CMessageItem::IM_STATUS_LOADED, this));
}
