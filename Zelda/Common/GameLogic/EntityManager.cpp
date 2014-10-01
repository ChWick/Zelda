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

#include "EntityManager.hpp"
#include "Entity.hpp"
#include "../Log.hpp"


template<> CEntityManager *Ogre::Singleton<CEntityManager>::msSingleton = 0;

CEntityManager &CEntityManager::getSingleton() {
  assert(msSingleton);
  return *msSingleton;
}
CEntityManager *CEntityManager::getSingletonPtr() {
  return msSingleton;
}

void CEntityManager::deleteNow(CEntity *pEntity) {
  LOGV("Deleting entity '%s'", pEntity->getID().c_str());
  pEntity->sendCallToAllChildrenFirst(&CEntity::stop, true);
  pEntity->sendCallToAllChildrenFirst(&CEntity::exit, true);
  delete pEntity;
}

void CEntityManager::deleteLater(CEntity *pEntity) {
  if (pEntity->getState() != EST_DELETE) {
    // delete if not deleted
    mAddToListMutex.lock();
    m_lEntitiesToDestroy.push_back(pEntity);
    mAddToListMutex.unlock();
    pEntity->changeState(EST_DELETE);
  }
}

void CEntityManager::process() {
  std::list<CEntity *> l;
  mAddToListMutex.lock();
  l.splice(l.end(), m_lEntitiesToDestroy);
  mAddToListMutex.unlock();

  for (auto p : l) {
    deleteNow(p);
  }
}
