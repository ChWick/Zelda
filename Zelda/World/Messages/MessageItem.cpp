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

#include "MessageItem.hpp"
#include "UserMessageTypes.hpp"

CMessageItem::CMessageItem(const std::string &creationFile,
                           const EItemMessageTypes eItemMessageType,
                           CItemStatusStorage const * const pItemStatusStorage,
                           const EItemSlotTypes eItemSlotType,
                           const EItemVariantTypes eItemVariantType)
  : CMessage(MSG_ITEM, creationFile),
    m_eItemMessageType(eItemMessageType),
    m_eItemSlotType(eItemSlotType),
    m_eItemVariantType(eItemVariantType),
    m_pItemStatusStorage(pItemStatusStorage) {
}
