#include "MessageDebug.hpp"


CMessageDebug::CMessageDebug(const EDebugMessageTypes type, bool bActive)
  : CMessage(MSG_DEBUG),
    m_eDebugType(type),
    m_bActive(bActive) {
}