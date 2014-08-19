#include "Atlas.hpp"
#include "Map.hpp"
#include <OgreSceneNode.h>
#include "../../Common/Log.hpp"
#include "../../Common/Message/MessageSwitchMap.hpp"
#include "../../Common/Message/MessageTargetReached.hpp"
#include "../../Common/Message/MessageHandler.hpp"
#include "../Camera/AerialCameraPerspective.hpp"
#include "../../Common/FileManager/FileManager.hpp"

CAtlas::CAtlas(CEntity *pParent, Ogre::SceneNode *pRootSceneNode, CWorldEntity *pPlayer, CAerialCameraPerspective* &pCameraPerspective)
  : CWorldEntity("atlas", pParent, nullptr),
    m_pCurrentMap(nullptr),
    m_pNextMap(nullptr),
    m_pPlayer(pPlayer),
    m_pCameraPerspective(pCameraPerspective),
    m_bSwitchingMaps(false),
    m_bPlayerTargetReached(false) {

  LOGV("Creating the atlas");

  m_pSceneNode = pRootSceneNode->createChildSceneNode("Atlas");

  m_pCurrentMap = new CMap(this, CMapPackPtr(new CMapPack(CFileManager::getResourcePath("maps/Atlases/LightWorld/"), "link_house")), m_pSceneNode, pPlayer);
  m_pPlayer->enterMap(m_pCurrentMap, Ogre::Vector3(0, 2, 0));
  m_pCurrentMap->start();

  CMessageHandler::getSingleton().addMessage(new CMessageSwitchMap(m_pCurrentMap->getMapPack()->getName(), CMessageSwitchMap::FINISHED, m_pCurrentMap, nullptr));
}
CAtlas::~CAtlas() {
}

void CAtlas::update(Ogre::Real tpf) {
  if (m_bSwitchingMaps) {
    if (m_pCameraPerspective->isCameraInBounds() && m_bPlayerTargetReached) {
      m_pCurrentMap->deleteNow();
      m_pCurrentMap = m_pNextMap;
      m_pNextMap = nullptr;
      m_bSwitchingMaps = false;
    }
  }
  {
    // only update if map switching is done
    CWorldEntity::update(tpf);
  }
}
bool CAtlas::frameRenderingQueued(const Ogre::FrameEvent& evt) {
  //if (m_bSwitchingMaps) {return true;}
  return CWorldEntity::frameRenderingQueued(evt);
}
bool CAtlas::frameStarted(const Ogre::FrameEvent& evt) {
  //if (m_bSwitchingMaps) {return true;}
  return CWorldEntity::frameStarted(evt);
}
bool CAtlas::frameEnded(const Ogre::FrameEvent& evt) {
  //if (m_bSwitchingMaps) {return true;}
  return CWorldEntity::frameEnded(evt);
}

void CAtlas::handleMessage(const CMessage &message) {
  if (message.getType() == MSG_SWITCH_MAP) {
    if (m_bSwitchingMaps) {return;}
    const CMessageSwitchMap &switch_map_message(dynamic_cast<const CMessageSwitchMap&>(message));
    if (switch_map_message.getStatus() == CMessageSwitchMap::INJECT) {
        LOGI("Atlas: changing map to '%s'", switch_map_message.getMap().c_str());


        m_pNextMap = new CMap(this, CMapPackPtr(new CMapPack("../maps/Atlases/TestMap/", switch_map_message.getMap())), m_pSceneNode, m_pPlayer);


        CMapPackPtr nextPack = m_pNextMap->getMapPack();
        CMapPackPtr currPack = m_pCurrentMap->getMapPack();

        Ogre::Vector3 vMapPositionOffset = currPack->getGlobalPosition() - nextPack->getGlobalPosition();
        m_pCurrentMap->moveMap(vMapPositionOffset);




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

        Ogre::Vector3 vPlayerPos = m_pPlayer->getPosition() + vMapPositionOffset;
        m_pPlayer->enterMap(m_pNextMap, vPlayerPos + vDirection * 2);
        m_pPlayer->setPosition(vPlayerPos);

        m_bPlayerTargetReached = false;

        CMessageHandler::getSingleton().addMessage(new CMessageSwitchMap(m_pNextMap->getMapPack()->getName(), CMessageSwitchMap::SWITCHING, m_pCurrentMap, m_pNextMap));
    }
  }
  else if (message.getType() == MSG_TARGET_REACHED) {
    const CMessageTargetReached &message_target_reached(dynamic_cast<const CMessageTargetReached &>(message));
    if (message_target_reached.getEntity() == m_pPlayer) {
      if (m_bSwitchingMaps) {
        m_bPlayerTargetReached = true;

        m_pNextMap->start();

        CMessageHandler::getSingleton().addMessage(new CMessageSwitchMap(m_pCurrentMap->getMapPack()->getName(), CMessageSwitchMap::FINISHED, m_pCurrentMap, nullptr));
      }
    }
  }
}
