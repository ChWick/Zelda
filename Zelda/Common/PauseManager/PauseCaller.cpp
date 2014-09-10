#include "PauseCaller.hpp"
#include "PauseManager.hpp"

CPauseCaller::CPauseCaller()
  : m_uiPausedFlags(0) {
  CPauseManager::getSingleton().addCaller(this);
}
CPauseCaller::~CPauseCaller() {
  CPauseManager::getSingleton().removeCaller(this);
}
void CPauseCaller::pause(unsigned int uiPauseFlags) {
  m_uiPausedFlags |= uiPauseFlags;
  CPauseManager::getSingleton().needUpdate();
}
void CPauseCaller::unpause(unsigned int uiPauseFlags) {
  m_uiPausedFlags ^= uiPauseFlags & m_uiPausedFlags;
  CPauseManager::getSingleton().needUpdate();
}
void CPauseCaller::setPause(unsigned int uiPauseFlags) {
  m_uiPausedFlags = uiPauseFlags;
  CPauseManager::getSingleton().needUpdate();
}
