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

#include "ItemData.hpp"

CItemSlotDataMap ITEM_SLOT_DATA_MAP;
CItemVariantDataMap ITEM_VARIANT_DATA_MAP;


void CItemSlotDataMap::init() {
  m_Map[ITEM_SLOT_BOW] = {{ITEM_VARIANT_NORMAL_BOW, ITEM_VARIANT_SILVER_BOW}};
  m_Map[ITEM_SLOT_BOOMERANG] = {{ITEM_VARIANT_NORMAL_BOOMERANG, ITEM_VARIANT_MAGIC_BOOMERANG}};
  m_Map[ITEM_SLOT_HOOKSHOT] = {{ITEM_VARIANT_HOOKSHOT}};
  m_Map[ITEM_SLOT_BOMB] = {{ITEM_VARIANT_BOMB}};
  m_Map[ITEM_SLOT_MAGICPOWDER] = {{ITEM_VARIANT_MUSHROOM, ITEM_VARIANT_MAGICPOWDER}};
  m_Map[ITEM_SLOT_FIREROD] = {{ITEM_VARIANT_FIREROD}};
  m_Map[ITEM_SLOT_ICEROD] = {{ITEM_VARIANT_ICEROD}};
  m_Map[ITEM_SLOT_BOMBOSMEDALLION] = {{ITEM_VARIANT_BOMBOSMEDALLION}};
  m_Map[ITEM_SLOT_ETHERMEDALLION] = {{ITEM_VARIANT_ETHERMEDALLION}};
  m_Map[ITEM_SLOT_QUAKEMEDALLION] = {{ITEM_VARIANT_QUAKEMEDALLION}};
  m_Map[ITEM_SLOT_LAMP] = {{ITEM_VARIANT_LAMP}};
  m_Map[ITEM_SLOT_HAMMER] = {{ITEM_VARIANT_HAMMER}};
  m_Map[ITEM_SLOT_OCARINA] = {{ITEM_VARIANT_OCARINA}};
  m_Map[ITEM_SLOT_NET] = {{ITEM_VARIANT_NET}};
  m_Map[ITEM_SLOT_BOOK] = {{ITEM_VARIANT_BOOK}};
  m_Map[ITEM_SLOT_BOTTLE] = {{ITEM_VARIANT_BOTTLE_EMPTY, ITEM_VARIANT_BOTTLE_BEE, ITEM_VARIANT_BOTTLE_BLUE, ITEM_VARIANT_BOTTLE_FAIRY, ITEM_VARIANT_BOTTLE_GREEN, ITEM_VARIANT_BOTTLE_RED}};
  m_Map[ITEM_SLOT_CANEOFSOMARIA] = {{ITEM_VARIANT_CANEOFSOMARIA}};
  m_Map[ITEM_SLOT_CANEOFBYRNA] = {{ITEM_VARIANT_CANEOFBYRNA}};
  m_Map[ITEM_SLOT_CLOAK] = {{ITEM_VARIANT_CLOAK}};
  m_Map[ITEM_SLOT_MIRROR] = {{ITEM_VARIANT_MIRROR}};
}

EItemSlotTypes CItemSlotDataMap::getFromItemVariant(
    EItemVariantTypes variantType) const {
  for (auto &data : m_Map) {
    auto &v(data.second.vItemVariants);
    if (std::find(v.begin(), v.end(), variantType) != v.end()) {
      return data.first;
    }
  }
  return ITEM_SLOT_COUNT;
}

void CItemVariantDataMap::init() {
  // this is set in world
  // loaded by WorldDataLoader and ItemDataLoader.
}
