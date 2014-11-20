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

#include "ItemTypes.hpp"

CItemSlotIdMap ITEM_SLOT_ID_MAP;
CItemVariantIdMap ITEM_VARIANT_ID_MAP;


void CItemSlotIdMap::init() {
  m_Map[ITEM_SLOT_BOW] = "bow";
  m_Map[ITEM_SLOT_BOOMERANG] = "boomerang";
  m_Map[ITEM_SLOT_HOOKSHOT] = "hookshot";
  m_Map[ITEM_SLOT_BOMB] = "bomb";
  m_Map[ITEM_SLOT_MAGICPOWDER] = "magic_powder";
  m_Map[ITEM_SLOT_FIREROD] = "fire_rod";
  m_Map[ITEM_SLOT_ICEROD] = "ice_rod";
  m_Map[ITEM_SLOT_BOMBOSMEDALLION] = "bombos_medallion";
  m_Map[ITEM_SLOT_ETHERMEDALLION] = "ether_medallion";
  m_Map[ITEM_SLOT_QUAKEMEDALLION] = "quake_medallion";
  m_Map[ITEM_SLOT_LAMP] = "lamp";
  m_Map[ITEM_SLOT_HAMMER] = "hammer";
  m_Map[ITEM_SLOT_OCARINA] = "ocarina";
  m_Map[ITEM_SLOT_NET] = "net";
  m_Map[ITEM_SLOT_BOOK] = "book";
  m_Map[ITEM_SLOT_BOTTLE] = "bottle";
  m_Map[ITEM_SLOT_CANEOFSOMARIA] = "cane_of_somaria";
  m_Map[ITEM_SLOT_CANEOFBYRNA] = "cane_of_byrna";
  m_Map[ITEM_SLOT_CLOAK] = "cloak";
  m_Map[ITEM_SLOT_MIRROR] = "mirror";
}

void CItemVariantIdMap::init() {
  m_Map[ITEM_VARIANT_SILVER_BOW] = "silver_bow";
  m_Map[ITEM_VARIANT_NORMAL_BOW] = "normal_bow";

  m_Map[ITEM_VARIANT_NORMAL_BOOMERANG] = "normal_boomerang";
  m_Map[ITEM_VARIANT_MAGIC_BOOMERANG] = "magic_boomerang";

  m_Map[ITEM_VARIANT_HOOKSHOT] = "hookshot";

  m_Map[ITEM_VARIANT_BOMB] = "bomb";

  m_Map[ITEM_VARIANT_MAGICPOWDER] = "magic_powder";
  m_Map[ITEM_VARIANT_MUSHROOM] = "mushroom";

  m_Map[ITEM_VARIANT_FIREROD] = "fire_rod";
  m_Map[ITEM_VARIANT_ICEROD] = "ice_rod";

  m_Map[ITEM_VARIANT_BOMBOSMEDALLION] = "bombos_medallion";
  m_Map[ITEM_VARIANT_ETHERMEDALLION] = "ether_medallion";
  m_Map[ITEM_VARIANT_QUAKEMEDALLION] = "quake_medallion";
  m_Map[ITEM_VARIANT_LAMP] = "lamp";
  m_Map[ITEM_VARIANT_HAMMER] = "hammer";

  m_Map[ITEM_VARIANT_SHOVEL] = "shovel";
  m_Map[ITEM_VARIANT_OCARINA] = "ocarina";

  m_Map[ITEM_VARIANT_NET] = "net";
  m_Map[ITEM_VARIANT_BOOK] = "book";

  m_Map[ITEM_VARIANT_BOTTLE_EMPTY] = "bottle_empty";
  m_Map[ITEM_VARIANT_BOTTLE_GREEN] = "bottle_green";
  m_Map[ITEM_VARIANT_BOTTLE_RED] = "bottle_red";
  m_Map[ITEM_VARIANT_BOTTLE_BLUE] = "bottle_blue";
  m_Map[ITEM_VARIANT_BOTTLE_FAIRY] = "bottle_fairy";
  m_Map[ITEM_VARIANT_BOTTLE_BEE] = "bottle_bee";

  m_Map[ITEM_VARIANT_CANEOFSOMARIA] = "cane_of_somaria";
  m_Map[ITEM_VARIANT_CANEOFBYRNA] = "cane_of_byrna";
  m_Map[ITEM_VARIANT_CLOAK] = "cloak";
  m_Map[ITEM_VARIANT_MIRROR] = "mirror";

  m_Map[ITEM_VARIANT_SWORD_SIMPLE] = "sword_simple";
  m_Map[ITEM_VARIANT_SWORD_MASTER] = "sword_master";
  m_Map[ITEM_VARIANT_SWORD_BRONZE] = "sword_bronze";
  m_Map[ITEM_VARIANT_SWORD_GOLDEN] = "sword_golden";

  // Enemy variants
  m_Map[ITEM_VARIANT_ENEMY_SWORD_SIMPLE] = "enemy_sword_simple";
}
