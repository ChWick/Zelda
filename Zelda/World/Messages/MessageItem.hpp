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

#ifndef _MESSAGE_ITEM_HPP_
#define _MESSAGE_ITEM_HPP_

#include "../../Common/Message/Message.hpp"
#include "../Items/ItemTypes.hpp"

class CItemStatusStorage;

class CMessageItem: public CMessage {
public:
  enum EItemMessageTypes {
    IM_SELECTION_CHANGED,
    IM_STATUS_CHANGED,
    IM_STATUS_LOADED,
  };
protected:
  const EItemMessageTypes m_eItemMessageType;
  const EItemSlotTypes m_eItemSlotType;
  const EItemVariantTypes m_eItemVariantType;
  const CItemStatusStorage *m_pItemStatusStorage;
public:
  CMessageItem(const EItemMessageTypes eItemMessageType,
               CItemStatusStorage const * const pItemStatusStorage,
               const EItemSlotTypes eItemSlotType = ITEM_SLOT_COUNT,
               const EItemVariantTypes eItemVariantType = ITEM_VARIANT_COUNT);

  EItemMessageTypes getItemMessageType() const {return m_eItemMessageType;}
  EItemSlotTypes getItemSlotType() const {return m_eItemSlotType;}
  EItemVariantTypes getItemVariantType() const {return m_eItemVariantType;}
  const CItemStatusStorage *getStatusStorage() const {return m_pItemStatusStorage;}

};

#endif // _MESSAGE_ITEM_HPP_
