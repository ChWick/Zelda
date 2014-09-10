#include "MessageItem.hpp"
#include "UserMessageTypes.hpp"


CMessageItem::CMessageItem(const EItemMessageTypes eItemMessageType, const EItemVariantTypes eItemVariantType)
  : CMessage(MSG_ITEM),
    m_eItemMessageType(eItemMessageType),
    m_eItemSlotType(ITEM_SLOT_COUNT),
    m_eItemVariantType(eItemVariantType),
    m_pItemStatusStorage(nullptr) {
}

CMessageItem::CMessageItem(const EItemMessageTypes eItemMessageType, const EItemSlotTypes eItemSlotType)
  : CMessage(MSG_ITEM),
    m_eItemMessageType(eItemMessageType),
    m_eItemSlotType(eItemSlotType),
    m_eItemVariantType(ITEM_VARIANT_COUNT),
    m_pItemStatusStorage(nullptr) {
}

CMessageItem::CMessageItem(const EItemMessageTypes eItemMessageType, CItemStatusStorage const * const pItemStatusStorage)
  : CMessage(MSG_ITEM),
    m_eItemMessageType(eItemMessageType),
    m_eItemSlotType(ITEM_SLOT_COUNT),
    m_eItemVariantType(ITEM_VARIANT_COUNT),
    m_pItemStatusStorage(pItemStatusStorage) {
}
