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

#include "PauseManager.hpp"
#include "PauseListener.hpp"
#include "PauseCaller.hpp"

template<> CPauseManager* Ogre::Singleton<CPauseManager>::msSingleton = 0;

CPauseManager* CPauseManager::getSingletonPtr(void)
{
  return msSingleton;
}
CPauseManager& CPauseManager::getSingleton(void)
{
  assert( msSingleton );  return ( *msSingleton );
}

CPauseManager::CPauseManager()
: m_uiOldPauseFlags(0),
  m_bNeedsUpdate(true) {
}

void CPauseManager::update() {
  // only update if something changed
  if (m_bNeedsUpdate == false) {return;}

  unsigned int uiTotalPause = 0;
  for (auto pCaller : m_lPauseCallers) {
      uiTotalPause |= pCaller->getPause();
  }

  // walk through all flag to check update
  for (unsigned int i = 0; i < 15; i++) {
    unsigned int uiFlag = 2 << i;
    if ((uiTotalPause & uiFlag) != (m_uiOldPauseFlags & uiFlag)) {
      for (auto pListener : m_lPauseListeners) {
        pListener->updatePause(uiFlag, (uiTotalPause & uiFlag) > 0);
      }
    }
  }

  m_uiOldPauseFlags = uiTotalPause;
  m_bNeedsUpdate = false;
}

void CPauseManager::addListener(CPauseListener *pListener) {
  m_lPauseListeners.push_back(pListener);
}
