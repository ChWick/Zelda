#include "EntityManager.hpp"
#include "Entity.hpp"


template<> CEntityManager *Ogre::Singleton<CEntityManager>::msSingleton = 0;

CEntityManager &CEntityManager::getSingleton() {
  assert(msSingleton);
  return *msSingleton;
}
CEntityManager *CEntityManager::getSingletonPtr() {
  return msSingleton;
}

void CEntityManager::deleteNow(CEntity *pEntity) {
  pEntity->sendCallToAllChildrenFirst(&CEntity::stop, true);
  pEntity->sendCallToAllChildrenFirst(&CEntity::exit, true);
  delete pEntity;
}

void CEntityManager::deleteLater(CEntity *pEntity) {
  m_lEntitiesToDestroy.push_back(pEntity);
}

void CEntityManager::process() {
  while (m_lEntitiesToDestroy.size() > 0) {
    deleteNow(m_lEntitiesToDestroy.front());
    m_lEntitiesToDestroy.pop_front();
  }
}
