#include "ItemTypes.hpp"

CItemTypeIdMap ITEM_TYPE_ID_MAP;

void CItemTypeIdMap::init() {
  m_Map[ITEM_BOW] = "bow";
  m_Map[ITEM_BOOMERANG] = "boomerang";
  m_Map[ITEM_HOOKSHOT] = "hookshot";
  m_Map[ITEM_BOMB] = "bomb";
  m_Map[ITEM_MAGICPOWDER] = "magic_powder";
  m_Map[ITEM_FIREROD] = "fire_rod";
  m_Map[ITEM_ICEROD] = "ice_rod";
  m_Map[ITEM_BOMBOSMEDALLION] = "bombos_medallion";
  m_Map[ITEM_ETHERMEDALLION] = "ether_medallion";
  m_Map[ITEM_QUAKEMEDALLION] = "quake_medallion";
  m_Map[ITEM_LAMP] = "lamp";
  m_Map[ITEM_HAMMER] = "hammer";
  m_Map[ITEM_OCARINA] = "ocarina";
  m_Map[ITEM_NET] = "net";
  m_Map[ITEM_BOOK] = "book";
  m_Map[ITEM_BOTTLE] = "bottle";
  m_Map[ITEM_CANEOFSOMARIA] = "cane_of_somaria";
  m_Map[ITEM_CANEOFBYRNA] = "cane_of_byrna";
  m_Map[ITEM_CLOAK] = "cloak";
  m_Map[ITEM_MIRROR] = "mirror";
  m_Map[ITEM_SHOVEL] = "shovel";
  m_Map[ITEM_MUSHROOM] = "mushroom";
}
