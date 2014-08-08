#include "Map.hpp"
#include "Game.hpp"

CMap::CMap(CEntity *pAtlas, CMapPackPtr mapPack)
  : CEntity("map", pAtlas),
    m_PhysicsManager(CGame::getSingleton().getSceneManager()),
    m_MapPack(mapPack) {

  m_MapPack->init();

  m_pRootSceneNode = CGame::getSingleton().getSceneManager()->getRootSceneNode()->createChildSceneNode(m_MapPack->getName() + "_RootNode");
  m_SceneLoader.parseDotScene(m_MapPack->getSceneFile(),
                              m_MapPack->getResourceGroup(),
                              CGame::getSingleton().getSceneManager(),
                              &m_PhysicsManager,
                              m_pRootSceneNode);
}

void CMap::update(float tpf) {
  m_PhysicsManager.update(tpf);
}
