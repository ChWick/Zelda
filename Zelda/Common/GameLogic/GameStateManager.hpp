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

#ifndef GAMESTATEMANAGER_HPP
#define GAMESTATEMANAGER_HPP

#include <OgreSingleton.h>
#include <memory>
#include "Entity.hpp"

class CGameStateManager : public Ogre::Singleton<CGameStateManager>, public CEntity {
public:
  enum EGameStates {
    GS_MAIN_MENU,
    GS_GAME,
    GS_GAME_OVER,
    GS_STATISTICS,
    GS_CREDITS,
    GS_AD,

    GS_COUNT
  };
private:
  EGameStates m_eCurrentGameState;
  EGameStates m_eNextGameState;

  bool m_bForce;
  bool m_bAdShown;
public:
  static CGameStateManager &getSingleton();
  static CGameStateManager *getSingletonPtr();

  CGameStateManager();
  ~CGameStateManager();

  void init();

  void update(Ogre::Real tpf);

  void changeGameState(EGameStates eNewGameState, bool bNow = false, bool bForce = true);

  EGameStates getCurrentGameState() {return m_eCurrentGameState;}

  void setAdShown(bool bShown) {m_bAdShown = bShown;}

private:
  void changeGameStateImpl();
};

#endif
