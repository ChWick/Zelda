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

#include "World.hpp"
#include "Atlas/Atlas.hpp"
#include "../Common/Game.hpp"
#include "Atlas/Map.hpp"
#include <OgreResourceGroupManager.h>
#include <OgreMaterialManager.h>
#include "../Common/Log.hpp"
#include "WorldGUI/WorldGUI.hpp"
#include "Messages/MessageItem.hpp"
#include "../Common/Message/MessagePlayerPickupItem.hpp"
#include "Objects/ObjectTypes.hpp"

CWorld::CWorld()
  : CGameState(GST_WORLD),
    m_pWorldGUI(nullptr),
    m_fWaderSideWaveMaterialNextImageIn(0),
    m_iWaterSideWaveMaterialCurrentImage(0) {
  Ogre::MaterialManager::getSingleton().setVerbose(true);

  LOGV("Creating World");

  Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("World");

  LOGV("Loading world data");
  mDataLoader.load();

  // create the player
  LOGV("Loading item status storage");
  m_ItemStatusStorage.load();

  // create the atlas
  LOGV("Creating the atlas");
  m_pAtlas = new CAtlas(this,
                        CGame::getSingleton<CGame>().getSceneManager()->getRootSceneNode());

  LOGV("Creating the world gui");
  m_pWorldGUI = new CWorldGUI(this);

  // safe water_side_wave material top adjust image in vertex program
  m_pWaterSideWaveMaterial
      = Ogre::MaterialManager::getSingleton().getByName("water_side_wave");
  m_pWaterSideWaveMaterial->load();
}

CWorld::~CWorld() {
  delete m_pAtlas;
  delete m_pWorldGUI;
}

void CWorld::preRender(Ogre::Real tpf) {
  CGameState::preRender(tpf);
}

void CWorld::update(Ogre::Real tpf) {
  CGameState::update(tpf);

  m_fWaderSideWaveMaterialNextImageIn -= tpf;
  if (m_fWaderSideWaveMaterialNextImageIn <= 0) {
    m_fWaderSideWaveMaterialNextImageIn = 1.0f / 24.f;
    m_iWaterSideWaveMaterialCurrentImage = (m_iWaterSideWaveMaterialCurrentImage + 1) % 30;
    m_pWaterSideWaveMaterial->getSupportedTechnique(0)->getPass(0)->getVertexProgramParameters()->setNamedConstant("selectedTile", m_iWaterSideWaveMaterialCurrentImage);
  }
}

void CWorld::renderDebug(Ogre::Real tpf) {
  CGameState::renderDebug(tpf);
}

bool CWorld::frameRenderingQueued(const Ogre::FrameEvent& evt) {
  return CGameState::frameRenderingQueued(evt);
}

void CWorld::handleMessage(const CMessagePtr message) {
  if (message->getType() == MSG_PLAYER_PICKUP_ITEM) {
    auto msg_pui(
        std::dynamic_pointer_cast<const CMessagePlayerPickupItem>(message));
    switch (msg_pui->getItemType()) {
      case OBJECT_TOOL_LAMP:
        m_ItemStatusStorage.pickupItem(ITEM_VARIANT_LAMP);
        break;
      default:
        // normal objects are handled directly in CPlayer
        break;
    }
  }
}

