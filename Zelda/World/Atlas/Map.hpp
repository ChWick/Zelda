#ifndef _MAP_HPP_
#define _MAP_HPP_

#include "../../Common/GameLogic/Entity.hpp"
#include "../../Common/Physics/PhysicsManager.h"
#include "MapPack.hpp"
#include "../../Common/DotSceneLoader/DotSceneLoader.hpp"

class CMap : public CEntity {
private:
  CPhysicsManager m_PhysicsManager;
  CMapPackPtr m_MapPack;
  Ogre::DotSceneLoader m_SceneLoader;
  Ogre::SceneNode *m_pRootSceneNode;
public:
  CMap(CEntity *pAtlas, CMapPackPtr mapPack);

  const CPhysicsManager *getPhysicsManager() const {return &m_PhysicsManager;}
  CPhysicsManager *getPhysicsManager() {return &m_PhysicsManager;}

  void update(float tpf);
};
#endif // _MAP_HPP_
