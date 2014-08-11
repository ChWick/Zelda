#include "Atlas.hpp"
#include "Map.hpp"
#include <OgreSceneNode.h>
#include "../../Common/Log.hpp"
#include "../../Common/Message/MessageSwitchMap.hpp"

CAtlas::CAtlas(CEntity *pParent, Ogre::SceneNode *pRootSceneNode, CWorldEntity *pPlayer)
  : CWorldEntity("atlas", pParent, nullptr),
    m_pCurrentMap(nullptr),
    m_pNextMap(nullptr),
    m_pPlayer(pPlayer) {

  m_pSceneNode = pRootSceneNode->createChildSceneNode("Atlas");

  m_pCurrentMap = new CMap(this, CMapPackPtr(new CMapPack("../maps/Atlases/TestMap/", "Map")), m_pSceneNode, pPlayer);
  m_pPlayer->enterMap(m_pCurrentMap);
}
CAtlas::~CAtlas() {
  delete m_pCurrentMap;
}

void CAtlas::handleMessage(const CMessage &message) {
  if (message.getType() == MSG_SWITCH_MAP) {
    const CMessageSwitchMap &switch_map_message(dynamic_cast<const CMessageSwitchMap&>(message));
    LOGI("Atlas: changing map to '%s'", switch_map_message.getMap().c_str());

    m_pNextMap = new CMap(this, CMapPackPtr(new CMapPack("../maps/Atlases/TestMap/", switch_map_message.getMap())), m_pSceneNode, m_pPlayer);

    Ogre::Vector3 vMapPositionOffset = m_pCurrentMap->getMapPack()->getGlobalPosition() - m_pNextMap->getMapPack()->getGlobalPosition();
    m_pCurrentMap->moveMapAndDeletePhysics(vMapPositionOffset);


    m_pPlayer->enterMap(m_pNextMap);
    //m_pPlayer->setPosition(Ogre::Vector3(0, 10, 0));
  }
}
