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

#ifndef ITEM_TYPES_HPP
#define ITEM_TYPES_HPP

#include "../../Common/Util/EnumIdMap.hpp"

enum EItemSlotTypes {
  // list the displayed items in the selector
  ITEM_SLOT_BOW = 0,
  ITEM_SLOT_BOOMERANG,
  ITEM_SLOT_HOOKSHOT,
  ITEM_SLOT_BOMB,
  ITEM_SLOT_MAGICPOWDER,
  ITEM_SLOT_FIREROD,
  ITEM_SLOT_ICEROD,
  ITEM_SLOT_BOMBOSMEDALLION,
  ITEM_SLOT_ETHERMEDALLION,
  ITEM_SLOT_QUAKEMEDALLION,
  ITEM_SLOT_LAMP,
  ITEM_SLOT_HAMMER,
  ITEM_SLOT_OCARINA,
  ITEM_SLOT_NET,
  ITEM_SLOT_BOOK,
  ITEM_SLOT_BOTTLE,
  ITEM_SLOT_CANEOFSOMARIA,
  ITEM_SLOT_CANEOFBYRNA,
  ITEM_SLOT_CLOAK,
  ITEM_SLOT_MIRROR,

  ITEM_SLOT_COUNT,
};

enum EItemVariantTypes {
  // variants of it
  ITEM_VARIANT_SILVER_BOW,
  ITEM_VARIANT_NORMAL_BOW,

  ITEM_VARIANT_NORMAL_BOOMERANG,
  ITEM_VARIANT_MAGIC_BOOMERANG,

  ITEM_VARIANT_HOOKSHOT,

  ITEM_VARIANT_BOMB,

  ITEM_VARIANT_MAGICPOWDER,
  ITEM_VARIANT_MUSHROOM,

  ITEM_VARIANT_FIREROD,
  ITEM_VARIANT_ICEROD,

  ITEM_VARIANT_BOMBOSMEDALLION,
  ITEM_VARIANT_ETHERMEDALLION,
  ITEM_VARIANT_QUAKEMEDALLION,
  ITEM_VARIANT_LAMP,
  ITEM_VARIANT_HAMMER,

  ITEM_VARIANT_SHOVEL,
  ITEM_VARIANT_OCARINA,

  ITEM_VARIANT_NET,
  ITEM_VARIANT_BOOK,

  ITEM_VARIANT_BOTTLE_EMPTY,
  ITEM_VARIANT_BOTTLE_GREEN,
  ITEM_VARIANT_BOTTLE_RED,
  ITEM_VARIANT_BOTTLE_BLUE,
  ITEM_VARIANT_BOTTLE_FAIRY,
  ITEM_VARIANT_BOTTLE_BEE,

  ITEM_VARIANT_CANEOFSOMARIA,
  ITEM_VARIANT_CANEOFBYRNA,
  ITEM_VARIANT_CLOAK,
  ITEM_VARIANT_MIRROR,

  ITEM_VARIANT_SWORD_SIMPLE,
  ITEM_VARIANT_SWORD_MASTER,
  ITEM_VARIANT_SWORD_BRONZE,
  ITEM_VARIANT_SWORD_GOLDEN,

  ITEM_VARIANT_SHIELD_SIMPLE,
  ITEM_VARIANT_SHIELD_RED,
  ITEM_VARIANT_SHIELD_MIRROR,

  // Enemy variants
  ITEM_VARIANT_ENEMY_SWORD_SIMPLE,
  ITEM_VARIANT_ENEMY_SHIELD_SIMPLE,

  ITEM_VARIANT_COUNT
};

class CItemSlotIdMap : public CStringEnumIdMap<CItemSlotIdMap, EItemSlotTypes> {
public:
  void init();
};

class CItemVariantIdMap : public CStringEnumIdMap<CItemVariantIdMap, EItemVariantTypes> {
public:
  void init();
};

#endif // ITEM_TYPES_HPP
