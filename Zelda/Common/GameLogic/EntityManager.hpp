#ifndef _ENTITY_MANAGER_HPP_
#define _ENTITY_MANAGER_HPP_

#include <OgreSingleton.h>
#include <list>

class CEntity;

class CEntityManager : public Ogre::Singleton<CEntityManager> {
private:
  std::list<CEntity *> m_lEntitiesToDestroy;

public:
  static CEntityManager &getSingleton();
  static CEntityManager *getSingletonPtr();

  void deleteNow(CEntity *pEntity);
  void deleteLater(CEntity *pEntity);

  void process();
};

#endif // _ENTITY_MANAGER_HPP_
