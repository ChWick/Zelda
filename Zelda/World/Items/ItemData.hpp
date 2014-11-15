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

#ifndef _ITEM_DATA_HPP_
#define _ITEM_DATA_HPP_

#include "ItemTypes.hpp"

struct SItemSlotData {
  std::vector<EItemVariantTypes> vItemVariants;
};

struct SItemVariantData {
  std::string sImagesetName;
  unsigned char ucItemQuality;            //!< if this item has a variation of another, this gives the quality, to determine the best (higher number is better), if there are same quality but more often (potion), the player can decide witch to chose
  std::string sBasicMeshName;             //!< name of the basic mesh, additional effect are handled separately
};

class CItemSlotDataMap : public CEnumIdMap<EItemSlotTypes, SItemSlotData> {
public:
  void init();
};

class CItemVariantDataMap : public CEnumIdMap<EItemVariantTypes, SItemVariantData> {
public:
  void init();
};

extern CItemSlotDataMap ITEM_SLOT_DATA_MAP;
extern CItemVariantDataMap ITEM_VARIANT_DATA_MAP;

#endif // _ITEM_DATA_HPP_
