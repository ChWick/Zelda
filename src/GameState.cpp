#include "StdAfx.h"
#include "GameState.h"


template<> CGameState* Ogre::Singleton<CGameState>::msSingleton = 0;

CGameState* CGameState::getSingletonPtr(void)
{
    return msSingleton;
}
CGameState& CGameState::getSingleton(void)
{
    assert( msSingleton );  return ( *msSingleton );
}
CGameState::CGameState() {
	reset();
}
void CGameState::reset() {
	m_uiTextBoxActive = 0;
	m_uiInteractionLocked = 0;
	m_lCallbacks.clear();
}
void CGameState::setTextBoxActivate(bool bActivate) {
	// text box will automatically lock interaction
	setInteractionLocked(bActivate);
	if (bActivate) {
		m_uiTextBoxActive++;
		if (m_uiTextBoxActive == 1) {
			for (auto cb : m_lCallbacks) {
				cb->textBoxActivationChanged(true);
			}
		}
	}
	else {
		m_uiTextBoxActive--;
		assert(m_uiTextBoxActive >= 0);
		if (m_uiTextBoxActive == 0) {
			for (auto cb : m_lCallbacks) {
				cb->textBoxActivationChanged(false);
			}
		}
	}
}
void CGameState::setInteractionLocked(bool bLock) {
	if (bLock) {
		m_uiInteractionLocked++;
		if (m_uiInteractionLocked == 1) {
			for (auto cb : m_lCallbacks) {
				cb->interactionLockedChanged(true);
			}
		}
	}
	else {
		m_uiInteractionLocked--;
		assert(m_uiInteractionLocked >= 0);
		if (m_uiInteractionLocked == 0) {
			for (auto cb : m_lCallbacks) {
				cb->interactionLockedChanged(false);
			}
		}
	}
}