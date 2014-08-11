#ifndef _MAP_HPP_
#define _MAP_HPP_

#include "../WorldEntity.hpp"
#include "../../Common/Physics/PhysicsManager.h"
#include "MapPack.hpp"
#include "../../Common/DotSceneLoader/DotSceneLoader.hpp"
#include <btBulletDynamicsCommon.h>
#include "MapPackParserListener.hpp"

class CMap : public CWorldEntity, CMapPackParserListener {
private:
  CPhysicsManager m_PhysicsManager;
  CMapPackPtr m_MapPack;
  Ogre::DotSceneLoader m_SceneLoader;
  CWorldEntity *m_pPlayer;
public:
  CMap(CEntity *pAtlas, CMapPackPtr mapPack, Ogre::SceneNode *pParentSceneNode, CWorldEntity *pPlayer);
  virtual ~CMap();

  void CreateCube(const btVector3 &Position, btScalar Mass);

  void moveMapAndDeletePhysics(const Ogre::Vector3 &offset);

  const CPhysicsManager *getPhysicsManager() const {return &m_PhysicsManager;}
  CPhysicsManager *getPhysicsManager() {return &m_PhysicsManager;}

  void update(Ogre::Real tpf);
  bool frameStarted(const Ogre::FrameEvent& evt);
  bool frameEnded(const Ogre::FrameEvent& evt);

private:
  void parsePlayer(const tinyxml2::XMLElement *);
  void parseRegion(const SRegionInfo &);
};
#endif // _MAP_HPP_
