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

#include "GameLogicGarbageCollector.hpp"
#include <list>
#include "Entity.hpp"
#include "../Message/MessageInjector.hpp"
#include "../Log.hpp"


template<>
CGameLogicGarbageCollector *
Ogre::Singleton<CGameLogicGarbageCollector>::msSingleton(0);

CGameLogicGarbageCollector &CGameLogicGarbageCollector::getSingleton() {
  assert(msSingleton);
  return *msSingleton;
}
CGameLogicGarbageCollector *CGameLogicGarbageCollector::getSingletonPtr() {
  return msSingleton;
}

void CGameLogicGarbageCollector::addShared(
    std::shared_ptr<CMessageInjector> p) {
  std::lock_guard<std::mutex> l(mAddToListMutex);
  for (auto ex : m_lMessageInjetorShared) {
    if (p.get() == ex.get()) {
      // already existing
      return;
    }
  }
  m_lMessageInjetorShared.push_back(p);
}

void CGameLogicGarbageCollector::deleteNow(CMessageInjector *p) {
  LOGV("Garbage Collector: Deleting message injector");
  delete p;
}

void CGameLogicGarbageCollector::deleteLater(CMessageInjector *p) {
  mAddToListMutex.lock();
  if (std::find(m_lMessageInjetorsToDestroy.begin(),
                m_lMessageInjetorsToDestroy.end(),
                p) == m_lMessageInjetorsToDestroy.end()) {
    // already added
    m_lMessageInjetorsToDestroy.push_back(p);
  }
  mAddToListMutex.unlock();
}

void CGameLogicGarbageCollector::process() {
  // check shared
  std::list<std::shared_ptr<CMessageInjector> > sl;

  mAddToListMutex.lock();
  for (auto it = m_lMessageInjetorShared.begin();
       it != m_lMessageInjetorShared.end(); ) {
    if ((it)->use_count() == 1) {
      // only I am left,
      // so delete
      LOGV("GarbageCollector: deleting message injector");
      (it)->reset();
      // and erase
      it = m_lMessageInjetorShared.erase(it);
    } else {
      // elsewise update
      it++;
    }
  }
  mAddToListMutex.unlock();


  std::list<CMessageInjector *> l;
  mAddToListMutex.lock();
  l.splice(l.end(), m_lMessageInjetorsToDestroy);
  mAddToListMutex.unlock();

  for (auto p : l) {
    deleteNow(p);
  }
}
