#include "MessageEntityStateChanged.hpp"


CMessageEntityStateChanged::CMessageEntityStateChanged(EEntityStateTypes eOldState, EEntityStateTypes eNewState, CEntity &entity)
  : CMessage(MSG_ENTITY_STATE_CHANGED),
    m_eOldState(eOldState),
    m_eNewState(eNewState),
    m_Entity(entity) {
}