#include "Atlas.hpp"
#include "Map.hpp"
#include <OgreSceneNode.h>

CAtlas::CAtlas(CEntity *pParent, Ogre::SceneNode *pRootSceneNode, CWorldEntity *pPlayer)
  : CWorldEntity("atlas", pParent, nullptr),
    m_pCurrentMap(nullptr),
    m_pNextMap(nullptr) {

  m_pSceneNode = pRootSceneNode->createChildSceneNode("Atlas");

  m_pCurrentMap = new CMap(this, CMapPackPtr(new CMapPack("../maps/Atlases/TestMap/", "Map")), m_pSceneNode, pPlayer);
}
CAtlas::~CAtlas() {
  delete m_pCurrentMap;
}
