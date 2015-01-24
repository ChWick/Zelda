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

#include <OgreVector3.h>
#include "ItemTypes.hpp"
#include "../DamageTypes.hpp"
#include "../Hitpoints.hpp"


struct SItemSlotData {
  std::vector<EItemVariantTypes> vItemVariants;
};

struct SItemVariantData {
  std::string sImagesetName;
  unsigned char ucItemQuality;            //!< if this item has a variation of another, this gives the quality, to determine the best (higher number is better), if there are same quality but more often (potion), the player can decide witch to chose
  std::string sBasicMeshName;             //!< name of the basic mesh, additional effect are handled separately

  EDamageType eDamageType;                //!< Type of the damage the item deals 
  uint32_t uiDamage;                      //!< Amount of damage the item deals

  Ogre::Vector3 vBlockPhysicsSize;        //!< Size of a block shape for physics (no physics used when |size| == 0
  Ogre::Vector3 vBlockPhysicsOffset;      //!< Offset of the block shape

  Ogre::Real fLength;                     //!< Length of the item 
      
};

class CItemSlotDataMap : public CEnumIdMap<CItemSlotDataMap, EItemSlotTypes, SItemSlotData> {
public:
  void init();

  EItemSlotTypes getFromItemVariant(EItemVariantTypes variantType) const;
};

class CItemVariantDataMap : public CEnumIdMap<CItemVariantDataMap, EItemVariantTypes, SItemVariantData> {
public:
  void init();

  void setData(EItemVariantTypes type, const SItemVariantData &data) {
    m_Map[type] = data;
  }
};

#endif // _ITEM_DATA_HPP_
