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
#include "Character/Player.hpp"
#include "../Common/Game.hpp"
#include "Atlas/Map.hpp"
#include <OgreResourceGroupManager.h>
#include <OgreCamera.h>
#include <OgreMaterialManager.h>
#include <OgreViewport.h>
#include "Camera/AerialCameraPerspective.hpp"
#include "../Common/Log.hpp"
#include "WorldGUI/WorldGUI.hpp"

CWorld::CWorld()
  : CGameState(GST_WORLD),
    m_pPlayer(nullptr),
    m_pWorldGUI(nullptr),
    m_fWaderSideWaveMaterialNextImageIn(0),
    m_iWaterSideWaveMaterialCurrentImage(0) {

  LOGV("Creating World");

  Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("World");

  // create the world camera
  m_pWorldCamera = CGame::getSingleton().getSceneManager()->createCamera("WorldCamera");
  m_pWorldCamera->setNearClipDistance(0.001f);
  m_pWorldCamera->setFarClipDistance(10000.0f);
  // default position
  m_pWorldCamera->setPosition(Ogre::Vector3(0,1,1));
  m_pWorldCamera->lookAt(Ogre::Vector3(0,0,0));
  //m_pWorldCamera->setProjectionType(Ogre::PT_ORTHOGRAPHIC);

  Ogre::Viewport *vp = CGame::getSingleton().getMainViewPort();
  m_pWorldCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
  vp->setCamera(m_pWorldCamera);
  m_pWorldCamera->setOrthoWindowWidth(1.6);

  // create the player
  LOGV("Creating Player");
  m_pPlayer = new CPlayer(this, m_pWorldCamera, CGame::getSingleton().getSceneManager());

  // create the atlas
  LOGV("Creating the atlas");
  m_pAtlas = new CAtlas(this, CGame::getSingleton().getSceneManager()->getRootSceneNode(), m_pPlayer, m_pCameraPerspective);

  // initialize the player done in atlas, when new map is created

  //m_pCameraPerspective = new CAerialCameraPerspective(m_pWorldCamera, (Ogre::SceneNode*)m_pAtlas->getChildren().front()->getSceneNode()->getChild(0));
  m_pCameraPerspective = new CAerialCameraPerspective(m_pWorldCamera, m_pPlayer);

  LOGV("Creating the world gui");
  m_pWorldGUI = new CWorldGUI(this);

  // safe water_side_wave material top adjust image in vertex program
  m_pWaterSideWaveMaterial = Ogre::MaterialManager::getSingleton().getByName("water_side_wave").get();
}

CWorld::~CWorld() {
  delete m_pCameraPerspective;
  delete m_pPlayer;
  delete m_pAtlas;
  delete m_pWorldGUI;
}

void CWorld::preRender(Ogre::Real tpf) {
  m_pCameraPerspective->updateCamera(tpf);
}

void CWorld::update(Ogre::Real tpf) {
  CGameState::update(tpf);

  m_fWaderSideWaveMaterialNextImageIn -= tpf;
  if (m_fWaderSideWaveMaterialNextImageIn <= 0) {
    m_fWaderSideWaveMaterialNextImageIn += 1.0f / 24.f;
    m_iWaterSideWaveMaterialCurrentImage = (m_iWaterSideWaveMaterialCurrentImage + 1) % 30;
  }
    m_pWaterSideWaveMaterial->getSupportedTechnique(0)->getPass(0)->getVertexProgramParameters()->setNamedConstant("selectedTile", m_iWaterSideWaveMaterialCurrentImage);
}

bool CWorld::frameRenderingQueued(const Ogre::FrameEvent& evt) {

  return true;
}


