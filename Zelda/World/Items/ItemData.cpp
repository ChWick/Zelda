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

#include "ItemData.hpp"

CItemSlotDataMap ITEM_SLOT_DATA_MAP;
CItemVariantDataMap ITEM_VARIANT_DATA_MAP;


void CItemSlotDataMap::init() {
  m_Map[ITEM_SLOT_BOW] = {{ITEM_VARIANT_NORMAL_BOW, ITEM_VARIANT_SILVER_BOW}};
  m_Map[ITEM_SLOT_BOOMERANG] = {{ITEM_VARIANT_NORMAL_BOOMERANG, ITEM_VARIANT_MAGIC_BOOMERANG}};
  m_Map[ITEM_SLOT_HOOKSHOT] = {{ITEM_VARIANT_HOOKSHOT}};
  m_Map[ITEM_SLOT_BOMB] = {{ITEM_VARIANT_BOMB}};
  m_Map[ITEM_SLOT_MAGICPOWDER] = {{ITEM_VARIANT_MUSHROOM, ITEM_VARIANT_MAGICPOWDER}};
  m_Map[ITEM_SLOT_FIREROD] = {{ITEM_VARIANT_FIREROD}};
  m_Map[ITEM_SLOT_ICEROD] = {{ITEM_VARIANT_ICEROD}};
  m_Map[ITEM_SLOT_BOMBOSMEDALLION] = {{ITEM_VARIANT_BOMBOSMEDALLION}};
  m_Map[ITEM_SLOT_ETHERMEDALLION] = {{ITEM_VARIANT_ETHERMEDALLION}};
  m_Map[ITEM_SLOT_QUAKEMEDALLION] = {{ITEM_VARIANT_QUAKEMEDALLION}};
  m_Map[ITEM_SLOT_LAMP] = {{ITEM_VARIANT_LAMP}};
  m_Map[ITEM_SLOT_HAMMER] = {{ITEM_VARIANT_HAMMER}};
  m_Map[ITEM_SLOT_OCARINA] = {{ITEM_VARIANT_OCARINA}};
  m_Map[ITEM_SLOT_NET] = {{ITEM_VARIANT_NET}};
  m_Map[ITEM_SLOT_BOOK] = {{ITEM_VARIANT_BOOK}};
  m_Map[ITEM_SLOT_BOTTLE] = {{ITEM_VARIANT_BOTTLE_EMPTY, ITEM_VARIANT_BOTTLE_BEE, ITEM_VARIANT_BOTTLE_BLUE, ITEM_VARIANT_BOTTLE_FAIRY, ITEM_VARIANT_BOTTLE_GREEN, ITEM_VARIANT_BOTTLE_RED}};
  m_Map[ITEM_SLOT_CANEOFSOMARIA] = {{ITEM_VARIANT_CANEOFSOMARIA}};
  m_Map[ITEM_SLOT_CANEOFBYRNA] = {{ITEM_VARIANT_CANEOFBYRNA}};
  m_Map[ITEM_SLOT_CLOAK] = {{ITEM_VARIANT_CLOAK}};
  m_Map[ITEM_SLOT_MIRROR] = {{ITEM_VARIANT_MIRROR}};
}

void CItemVariantDataMap::init() {
  m_Map[ITEM_VARIANT_NORMAL_BOW] = {"Bow", 1, "link_sword.mesh"};
  m_Map[ITEM_VARIANT_SILVER_BOW] = {"Bow", 2, "link_sword.mesh"};
  m_Map[ITEM_VARIANT_NORMAL_BOOMERANG] = {"Boomerang", 1, "link_sword.mesh"};
  m_Map[ITEM_VARIANT_MAGIC_BOOMERANG] = {"Boomerang", 2, "link_sword.mesh"};
  m_Map[ITEM_VARIANT_HOOKSHOT] = {"Hookshot", 1, "link_sword.mesh"};
  m_Map[ITEM_VARIANT_BOMB] = {"Bomb", 1, "link_sword.mesh"};
  m_Map[ITEM_VARIANT_MAGICPOWDER] = {"MagicPowder", 2, "link_sword.mesh"};
  m_Map[ITEM_VARIANT_MUSHROOM] = {"Mushroom", 1, "link_sword.mesh"};
  m_Map[ITEM_VARIANT_FIREROD] = {"FireRod", 1, "link_sword.mesh"};
  m_Map[ITEM_VARIANT_ICEROD] = {"IceRod", 1, "link_sword.mesh"};
  m_Map[ITEM_VARIANT_BOMBOSMEDALLION] = {"BombosMedallion", 1, "link_sword.mesh"};
  m_Map[ITEM_VARIANT_ETHERMEDALLION] = {"EtherMedallion", 1, "link_sword.mesh"};
  m_Map[ITEM_VARIANT_QUAKEMEDALLION] = {"QuakeMedallion", 1, "link_sword.mesh"};
  m_Map[ITEM_VARIANT_LAMP] = {"Lamp", 1, "lamp.mesh"};
  m_Map[ITEM_VARIANT_HAMMER] = {"Hammer", 1, "link_sword.mesh"};
  m_Map[ITEM_VARIANT_SHOVEL] = {"Shovel", 1, "link_sword.mesh"};
  m_Map[ITEM_VARIANT_OCARINA] = {"Ocarina", 1, "link_sword.mesh"};
  m_Map[ITEM_VARIANT_NET] = {"Net", 1, "link_sword.mesh"};
  m_Map[ITEM_VARIANT_BOOK] = {"Book", 1, "link_sword.mesh"};
  m_Map[ITEM_VARIANT_BOTTLE_BEE] = {"Bottle", 1, "link_sword.mesh"};
  m_Map[ITEM_VARIANT_BOTTLE_BLUE] = {"Bottle", 1, "link_sword.mesh"};
  m_Map[ITEM_VARIANT_BOTTLE_EMPTY] = {"Bottle", 1, "link_sword.mesh"};
  m_Map[ITEM_VARIANT_BOTTLE_FAIRY] = {"Bottle", 1, "link_sword.mesh"};
  m_Map[ITEM_VARIANT_BOTTLE_GREEN] = {"Bottle", 1, "link_sword.mesh"};
  m_Map[ITEM_VARIANT_BOTTLE_RED] = {"Bottle", 1, "link_sword.mesh"};
  m_Map[ITEM_VARIANT_CANEOFSOMARIA] = {"CaneOfSomaria", 1, "link_sword.mesh"};
  m_Map[ITEM_VARIANT_CANEOFBYRNA] = {"CaneOfByrna", 1, "link_sword.mesh"};
  m_Map[ITEM_VARIANT_CLOAK] = {"Cloak", 1, "link_sword.mesh"};
  m_Map[ITEM_VARIANT_MIRROR] = {"Mirror", 1, "link_sword.mesh"};
  m_Map[ITEM_VARIANT_SWORD_SIMPLE] = {"SwordSimple", 1, "link_sword.mesh"};
  m_Map[ITEM_VARIANT_SWORD_MASTER] = {"SwordMaster", 2, "link_sword.mesh"};
  m_Map[ITEM_VARIANT_SWORD_BRONZE] = {"SwordBronze", 3, "link_sword.mesh"};
  m_Map[ITEM_VARIANT_SWORD_GOLDEN] = {"SwordGolden", 4, "link_sword.mesh"};

  // Enemy variants
  m_Map[ITEM_VARIANT_ENEMY_SWORD_SIMPLE] = {"EnemySwordSimple", 1, "soldier_sword.mesh"};
}
