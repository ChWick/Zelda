#ifndef _ITEM_STATUS_HPP_
#define _ITEM_STATUS_HPP_

#include "ItemTypes.hpp"

struct SItemStatus {
  EItemTypes eItemPlace;                        //!< Item place/type (sword, bow, ...)
  std::vector<EItemTypes> vItems;               //!< store different variants of this item (potions, bows, boomerangs, ...), if the item has no variants, the item has not be found yet

  std::vector<EItemTypes> getBestItem() const;
};

class CItemStatusStorage {
private:
  std::vector<SItemStatus> m_Storage;

public:
  CItemStatusStorage();

  void load();

  const SItemStatus &getStatus(EItemTypes eItemType) const {return m_Storage.at(eItemType);}
};

#endif // _ITEM_STATUS_HPP_
