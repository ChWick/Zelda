#include "Map.hpp"
#include "Game.hpp"

CMap::CMap(CEntity *pAtlas)
  : CEntity("map", pAtlas),
    m_PhysicsManager(CGame::getSingleton().getSceneManager()) {
}

void CMap::update(float tpf) {
  m_PhysicsManager.update(tpf);
}
