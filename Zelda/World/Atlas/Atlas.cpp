#include "Atlas.hpp"
#include "Map.hpp"
#include <OgreSceneNode.h>
#include "../../Common/Log.hpp"
#include "../../Common/Message/MessageSwitchMap.hpp"

CAtlas::CAtlas(CEntity *pParent, Ogre::SceneNode *pRootSceneNode, CWorldEntity *pPlayer)
  : CWorldEntity("atlas", pParent, nullptr),
    m_pCurrentMap(nullptr),
    m_pNextMap(nullptr),
    m_pPlayer(pPlayer),
    m_bSwitchingMaps(false) {

  m_pSceneNode = pRootSceneNode->createChildSceneNode("Atlas");

  m_pCurrentMap = new CMap(this, CMapPackPtr(new CMapPack("../maps/Atlases/TestMap/", "Map")), m_pSceneNode, pPlayer);
  m_pPlayer->enterMap(m_pCurrentMap);
}
CAtlas::~CAtlas() {
  delete m_pCurrentMap;
}

void CAtlas::update(Ogre::Real tpf) {
  if (m_bSwitchingMaps) {
    delete m_pCurrentMap;
    m_pCurrentMap = m_pNextMap;
    m_pNextMap = nullptr;
    m_bSwitchingMaps = false;
  }
  else {
    // only update if map switching is done
    CWorldEntity::update(tpf);
  }
}

void CAtlas::handleMessage(const CMessage &message) {
  if (message.getType() == MSG_SWITCH_MAP) {
    const CMessageSwitchMap &switch_map_message(dynamic_cast<const CMessageSwitchMap&>(message));
    LOGI("Atlas: changing map to '%s'", switch_map_message.getMap().c_str());


    m_pNextMap = new CMap(this, CMapPackPtr(new CMapPack("../maps/Atlases/TestMap/", switch_map_message.getMap())), m_pSceneNode, m_pPlayer);


    CMapPackPtr nextPack = m_pNextMap->getMapPack();
    CMapPackPtr currPack = m_pCurrentMap->getMapPack();

    Ogre::Vector3 vMapPositionOffset = currPack->getGlobalPosition() - nextPack->getGlobalPosition();
    m_pCurrentMap->moveMapAndDeletePhysics(vMapPositionOffset);


    m_pPlayer->enterMap(m_pNextMap);
    m_pPlayer->setPosition(m_pPlayer->getPosition() + vMapPositionOffset);

    // determine direction
    Ogre::Vector3 vDirection(Ogre::Vector3::ZERO);
    if (abs(nextPack->getGlobalPosition().x + nextPack->getGlobalSize().x - currPack->getGlobalPosition().x) < 0.01) {
      vDirection.x = -1;
    }
    else if (abs(nextPack->getGlobalPosition().z + nextPack->getGlobalSize().y - currPack->getGlobalPosition().z) < 0.01) {
      vDirection.z = -1;
    }
    else  if (abs(currPack->getGlobalPosition().x + currPack->getGlobalSize().x - nextPack->getGlobalPosition().x) < 0.01) {
      vDirection.x = 1;
    }
    else if (abs(currPack->getGlobalPosition().z + currPack->getGlobalSize().y - nextPack->getGlobalPosition().z) < 0.01) {
      vDirection.z = 1;
    }

    m_bSwitchingMaps = true;
  }
}
