#include "MessageTargetReached.hpp"

CMessageTargetReached::CMessageTargetReached(ENTITY *pEntity)
  : CMessage(MSG_TARGET_REACHED),
    m_pEntity(pEntity){
}
