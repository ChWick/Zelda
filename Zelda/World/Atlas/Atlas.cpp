/*****************************************************************************
 * Copyright 2014 Christoph Wick
 *
 * This file is part of Zelda.
 *
 * Zelda is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * Zelda is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * Zelda. If not, see http://www.gnu.org/licenses/.
 *****************************************************************************/

#include "Atlas.hpp"
#include "Map.hpp"
#include <OgreSceneNode.h>
#include "../../Common/Log.hpp"
#include "../../Common/Message/MessageSwitchMap.hpp"
#include "../../Common/Message/MessageTargetReached.hpp"
#include "../../Common/Message/MessageHandler.hpp"
#include "../Camera/AerialCameraPerspective.hpp"
#include "../../Common/FileManager/FileManager.hpp"
#include "../Character/Player.hpp"
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include "../../Common/Game.hpp"

CAtlas::CAtlas(CEntity *pParent, Ogre::SceneNode *pRootSceneNode)
  : CWorldEntity("atlas", pParent, nullptr),
    m_pCurrentMap(nullptr),
    m_pNextMap(nullptr),
    m_pPlayer(nullptr),
    m_pCameraPerspective(nullptr),
    m_bSwitchingMaps(false),
    m_bPlayerTargetReached(false),
    mFader("EllipticFade", this) {

  LOGV("Creating the Atlas");
  m_pSceneNode = pRootSceneNode->createChildSceneNode("Atlas");

  // create the world camera
  m_pWorldCamera = pRootSceneNode->getCreator()->createCamera("WorldCamera");
  m_pWorldCamera->setNearClipDistance(0.001f);
  m_pWorldCamera->setFarClipDistance(1000.0f);
  // default position
  m_pWorldCamera->setPosition(Ogre::Vector3(0,1,1));
  m_pWorldCamera->lookAt(Ogre::Vector3(0,0,0));
  //m_pWorldCamera->setProjectionType(Ogre::PT_ORTHOGRAPHIC);

  Ogre::Viewport *vp = CGame::getSingleton().getMainViewPort();
  m_pWorldCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
  vp->setCamera(m_pWorldCamera);
  m_pWorldCamera->setOrthoWindowWidth(1.6);

  LOGV(" - Creating Player");
  m_pPlayer = new CPlayer(this, m_pWorldCamera, CGame::getSingleton().getSceneManager());

  //m_pCameraPerspective = new CAerialCameraPerspective(m_pWorldCamera, (Ogre::SceneNode*)m_pAtlas->getChildren().front()->getSceneNode()->getChild(0));
  m_pCameraPerspective = new CAerialCameraPerspective(m_pWorldCamera, m_pPlayer);

  LOGV(" - Creating initial map");
  m_pCurrentMap = new CMap(this, CMapPackPtr(new CMapPack(CFileManager::getResourcePath("maps/Atlases/LightWorld/"), "inner_house_link")), m_pSceneNode, m_pPlayer);
  m_pPlayer->enterMap(m_pCurrentMap, Ogre::Vector3(0, 2, 0));
  m_pCurrentMap->start();

  CMessageHandler::getSingleton().addMessage(new CMessageSwitchMap(m_pCurrentMap->getMapPack()->getName(), CMessageSwitchMap::FINISHED, m_pCurrentMap, nullptr));

  mFader.startFadeOut(1);
}
CAtlas::~CAtlas() {
  delete m_pCameraPerspective;
  delete m_pPlayer;
}

void CAtlas::update(Ogre::Real tpf) {
  mFader.fade(tpf);

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

void CAtlas::renderDebug(Ogre::Real tpf) {
  m_pCameraPerspective->renderDebug(tpf);
  CWorldEntity::renderDebug(tpf);
}

void CAtlas::preRender(Ogre::Real tpf) {
  m_pCameraPerspective->updateCamera(tpf);
  CWorldEntity::preRender(tpf);
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


        m_pNextMap = new CMap(this, CMapPackPtr(new CMapPack(CFileManager::getResourcePath("maps/Atlases/LightWorld/"), switch_map_message.getMap())), m_pSceneNode, m_pPlayer);


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
        m_pPlayer->enterMap(m_pNextMap, vPlayerPos + vDirection * 0.2);
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

void CAtlas::updatePause(int iPauseType, bool bPause) {
  if (iPauseType & PAUSE_ATLAS_RENDER) {
    m_bPauseRender = bPause;
    m_pSceneNode->setVisible(!bPause);
  }
  if (iPauseType & PAUSE_ATLAS_UPDATE) {
    m_bPauseUpdate = bPause;
  }
}
