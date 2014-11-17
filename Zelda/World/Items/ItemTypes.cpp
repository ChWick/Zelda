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
}
