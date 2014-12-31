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

#ifndef PAUSE_MANAGER_HPP
#define PAUSE_MANAGER_HPP

#include <OgreSingleton.h>
#include <list>
#include <vector>
#include "PauseTypes.hpp"

class CPauseCaller;
class CPauseListener;

class CPauseManager : public Ogre::Singleton<CPauseManager> {
private:
  std::list<CPauseCaller *> m_lPauseCallers;
  std::list<CPauseListener *> m_lPauseListeners;

  PauseInt m_uiOldPauseFlags;
  bool m_bNeedsUpdate;
public:
  static CPauseManager& getSingleton(void);
  static CPauseManager* getSingletonPtr(void);

  CPauseManager();

  void update();

  void addCaller(CPauseCaller *pCaller) {m_lPauseCallers.push_back(pCaller);}
  void removeCaller(CPauseCaller *pCaller) {m_lPauseCallers.remove(pCaller);}

  void addListener(CPauseListener *pListener);
  void removeListener(CPauseListener *pListener) {m_lPauseListeners.remove(pListener);}

  bool isPause(PauseInt uiFlags) {return (m_uiOldPauseFlags & uiFlags) == uiFlags;}

  void needUpdate() {m_bNeedsUpdate = true;}
};

#endif // PAUSE_MANAGER_HPP
