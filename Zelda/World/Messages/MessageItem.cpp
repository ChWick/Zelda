#include "MessageItem.hpp"
#include "UserMessageTypes.hpp"

CMessageItem::CMessageItem(const EItemMessageTypes eItemMessageType, const EItemTypes eItemType)
  : CMessage(MSG_ITEM),
    m_eItemMessageType(eItemMessageType),
    m_eItemType(eItemType),
    m_pItemStatusStorage(nullptr) {
}

CMessageItem::CMessageItem(const EItemMessageTypes eItemMessageType, CItemStatusStorage const * const pItemStatusStorage)
  : CMessage(MSG_ITEM),
    m_eItemMessageType(eItemMessageType),
    m_eItemType(ITEM_COUNT),
    m_pItemStatusStorage(pItemStatusStorage) {
}
