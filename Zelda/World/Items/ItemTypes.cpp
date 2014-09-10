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
