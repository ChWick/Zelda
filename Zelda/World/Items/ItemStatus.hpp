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

#ifndef _ITEM_STATUS_HPP_
#define _ITEM_STATUS_HPP_

#include "ItemTypes.hpp"

struct SItemStatus {
  EItemSlotTypes eItemPlace;                            //!< Item place/type (sword, bow, ...)
  std::vector<EItemVariantTypes> vItems;                //!< store different variants of this item (potions, bows, boomerangs, ...), if the item has no variants, the item has not be found yet

  std::vector<EItemVariantTypes> getBestItem() const;
};

class CItemStatusStorage {
private:
  std::vector<SItemStatus> m_Storage;

public:
  CItemStatusStorage();

  void load();

  const SItemStatus &getStatus(EItemSlotTypes eItemType) const {return m_Storage.at(eItemType);}
};

#endif // _ITEM_STATUS_HPP_
