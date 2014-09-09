#include "ItemData.hpp"

CItemDataMap ITEM_DATA_MAP;

void CItemDataMap::init() {
  m_Map[ITEM_BOW] = {"Bow", {ITEM_NORMAL_BOW, ITEM_SILVER_BOW}, 0};
  m_Map[ITEM_NORMAL_BOW] = {"Bow", {ITEM_NORMAL_BOW}, 1};
  m_Map[ITEM_SILVER_BOW] = {"Bow", {ITEM_SILVER_BOW}, 2};
}
