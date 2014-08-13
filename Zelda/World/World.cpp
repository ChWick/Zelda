#include "World.hpp"
#include "Atlas/Atlas.hpp"
#include "Character/Player.h"
#include "../Common/Game.hpp"
#include "Atlas/Map.hpp"
#include <OgreResourceGroupManager.h>
#include <OgreCamera.h>
#include <OgreViewport.h>
#include "Camera/AerialCameraPerspective.hpp"
#include "../Common/Log.hpp"
#include "WorldGUI/WorldGUI.hpp"

CWorld::CWorld()
  : CGameState(GST_WORLD),
    m_pPlayer(nullptr),
    m_pWorldGUI(nullptr) {

  Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("World");

  // create the world camera
  m_pWorldCamera = CGame::getSingleton().getSceneManager()->createCamera("WorldCamera");
  m_pWorldCamera->setNearClipDistance(0.001f);
  m_pWorldCamera->setFarClipDistance(10000.0f);
  // default position
  m_pWorldCamera->setPosition(Ogre::Vector3(0,1,1));
  m_pWorldCamera->lookAt(Ogre::Vector3(0,0,0));
  m_pWorldCamera->setProjectionType(Ogre::PT_ORTHOGRAPHIC);

  Ogre::Viewport *vp = CGame::getSingleton().getMainViewPort();
  m_pWorldCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
  vp->setCamera(m_pWorldCamera);
  m_pWorldCamera->setOrthoWindowWidth(16);

  // create the player
  m_pPlayer = new CPlayer(this, m_pWorldCamera, CGame::getSingleton().getSceneManager());

  // create the atlas
  m_pAtlas = new CAtlas(this, CGame::getSingleton().getSceneManager()->getRootSceneNode(), m_pPlayer, m_pCameraPerspective);

  // initialize the player done in atlas, when new map is created

  //m_pCameraPerspective = new CAerialCameraPerspective(m_pWorldCamera, (Ogre::SceneNode*)m_pAtlas->getChildren().front()->getSceneNode()->getChild(0));
  m_pCameraPerspective = new CAerialCameraPerspective(m_pWorldCamera, m_pPlayer->getSceneNode());

  m_pWorldGUI = new CWorldGUI(this);
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
}

bool CWorld::frameRenderingQueued(const Ogre::FrameEvent& evt) {

  return true;
}


