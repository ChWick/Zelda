/*****************************************************************************
 * Copyright 2014 Christoph Wick
 *
 * This file is part of Mencus.
 *
 * Mencus is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * Mencus is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * Mencus. If not, see http://www.gnu.org/licenses/.
 *****************************************************************************/

#include "GameStateManager.hpp"
#include <OgreException.h>
#include <OgreLogManager.h>
#include "Util/Sleep.hpp"

using namespace std;

template<> CGameStateManager *Ogre::Singleton<CGameStateManager>::msSingleton = 0;

CGameStateManager &CGameStateManager::getSingleton() {
  assert(msSingleton);
  return *msSingleton;
}
CGameStateManager *CGameStateManager::getSingletonPtr() {
  return msSingleton;
}

CGameStateManager::CGameStateManager()
: CEntity("game_state_manager", NULL),
  m_eCurrentGameState(GS_COUNT),
  m_eNextGameState(GS_COUNT),
  m_bForce(true),
  m_bAdShown(false) {
}
CGameStateManager::~CGameStateManager() {
}
void CGameStateManager::init() {
  m_bAdShown = false;
}
void CGameStateManager::changeGameState(EGameStates eNewGameState, bool bNow, bool bForce) {
  m_eNextGameState = eNewGameState;

  if (bNow) {
    changeGameStateImpl();
  }
}

void CGameStateManager::changeGameStateImpl() {
  if (!m_bForce && m_eCurrentGameState == m_eNextGameState) {
    return;
  }
  auto ePreviousGameState = m_eCurrentGameState;

  switch (m_eCurrentGameState) {
  case GS_GAME:
    if (!(m_eNextGameState == GS_STATISTICS || m_eNextGameState == GS_GAME_OVER)) {
    }
    m_bAdShown = false;
    break;
  case GS_AD:
    m_bAdShown = false;
    break;
  case GS_MAIN_MENU:
    break;
  case GS_GAME_OVER:
    if (!(m_eNextGameState == GS_STATISTICS || m_eNextGameState == GS_GAME || m_eNextGameState == GS_AD)) {
    }
    break;
  case GS_STATISTICS:
    if (!(m_eNextGameState == GS_GAME || m_eNextGameState == GS_GAME_OVER || m_eNextGameState == GS_AD)) {
    }
    break;
  case GS_CREDITS:
    break;
  default:
    break;
  }
  try {
    m_eCurrentGameState = m_eNextGameState;
    switch (m_eCurrentGameState) {
    case GS_GAME:
      break;
    case GS_MAIN_MENU:
      break;
    case GS_GAME_OVER:
      break;
    case GS_STATISTICS:
      break;
    case GS_AD:
      break;
    case GS_CREDITS:
      break;
    default:
      break;
    }
  }
  catch (const Ogre::Exception &e) {
    Ogre::LogManager::getSingleton().logMessage("Exception while changing the game state");
    Ogre::LogManager::getSingleton().logMessage(e.getFullDescription());
    m_eNextGameState = ePreviousGameState;
    changeGameStateImpl();
  }
}
void CGameStateManager::update(Ogre::Real tpf) {
  if (m_eNextGameState != m_eCurrentGameState) {
    changeGameStateImpl();
  }

  switch (m_eCurrentGameState) {
  case GS_GAME:
    break;
  case GS_GAME_OVER:
    break;
  case GS_STATISTICS:
    break;
  case GS_AD:
    break;
  default:
    break;
  }
}
