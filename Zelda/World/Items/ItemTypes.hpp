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

enum EItemTypes {
  // list the displayed items in the selector
  ITEM_BOW = 0,
  ITEM_BOOMERANG,
  ITEM_HOOKSHOT,
  ITEM_BOMB,
  ITEM_MAGICPOWDER,
  ITEM_FIREROD,
  ITEM_ICEROD,
  ITEM_BOMBOSMEDALLION,
  ITEM_ETHERMEDALLION,
  ITEM_QUAKEMEDALLION,
  ITEM_LAMP,
  ITEM_HAMMER,
  ITEM_OCARINA,
  ITEM_NET,
  ITEM_BOOK,
  ITEM_BOTTLE,
  ITEM_CANEOFSOMARIA,
  ITEM_CANEOFBYRNA,
  ITEM_CLOAK,
  ITEM_MIRROR,

  // variants of it
  ITEM_SHOVEL,
  ITEM_MUSHROOM,

  // bow
  ITEM_SILVER_BOW,
  ITEM_NORMAL_BOW,

  ITEM_NORMAL_BOOMERANG,
  ITEM_MAGIC_BOOMERANG,

  // boomerang
  ITEM_BOTTLE_GREEN,
  ITEM_BOTTLE_RED,
  ITEM_BOTTLE_BLUE,
  ITEM_BOTTLE_FAIRY,
  ITEM_BOTTLE_BEE,

  ITEM_COUNT
};

//! Items to store data for, having variants, etc
const unsigned int ITEM_SELECT_COUNT = ITEM_MIRROR + 1; // last item + 1

class CItemTypeIdMap : public CEnumIdMap<EItemTypes> {
public:
  void init();
};

extern CItemTypeIdMap ITEM_TYPE_ID_MAP;

#endif // ITEM_TYPES_HPP
