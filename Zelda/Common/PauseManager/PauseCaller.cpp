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
