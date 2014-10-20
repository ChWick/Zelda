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
