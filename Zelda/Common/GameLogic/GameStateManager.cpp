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
#include "../Util/Sleep.hpp"
#include "../Message/MessageHandler.hpp"

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

  CMessageHandler::getSingleton().addInjector(this);
}
CGameStateManager::~CGameStateManager() {
  CMessageHandler::getSingleton().removeInjector(this);
}
void CGameStateManager::init() {
  m_bAdShown = false;
}
void CGameStateManager::changeGameState(EGameStates eNewGameState, bool bNow, bool bForce) {
}

void CGameStateManager::changeGameStateImpl() {
}
void CGameStateManager::update(Ogre::Real tpf) {
  CEntity::update(tpf);
}
