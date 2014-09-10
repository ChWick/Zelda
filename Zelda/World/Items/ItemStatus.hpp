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
