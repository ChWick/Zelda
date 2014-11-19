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

#ifndef _GAME_LOGIC_GARBAGE_COLLECTOR_HPP_
#define _GAME_LOGIC_GARBAGE_COLLECTOR_HPP_

#include <list>
#include <mutex>
#include <memory>
#include <OgreSingleton.h>

class CMessageInjector;

class CGameLogicGarbageCollector
    : public Ogre::Singleton<CGameLogicGarbageCollector> {
private:
  mutable std::mutex mAddToListMutex;
  std::list<CMessageInjector *> m_lMessageInjetorsToDestroy;
  std::list<std::shared_ptr<CMessageInjector> > m_lMessageInjetorShared;

public:
  static CGameLogicGarbageCollector &getSingleton();
  static CGameLogicGarbageCollector *getSingletonPtr();

  void addShared(std::shared_ptr<CMessageInjector> p);
  
  void deleteNow(CMessageInjector *p);
  void deleteLater(CMessageInjector *p);

  void process();
};

#endif // _GAME_LOGIC_GARBAGE_COLLECTOR_HPP_
