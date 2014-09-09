#ifndef _ITEM_DATA_HPP_
#define _ITEM_DATA_HPP_

#include "ItemTypes.hpp"

struct SItemData {
  std::string sImagesetName;
  std::vector<EItemTypes> vItemVariants;
  unsigned char ucItemQuality;            //!< if this item has a variation of another, this gives the quality, to determine the best (higher number is better), if there are same quality but more often (potion), the player can decide witch to chose
};

class CItemDataMap : public CEnumIdMap<EItemTypes, SItemData> {
public:
  void init();
};

extern CItemDataMap ITEM_DATA_MAP;

#endif // _ITEM_DATA_HPP_
