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
#include "../Common/Util/DebugDrawer.hpp"

// drow camera bounds (lines and spheres)
#define WORLD_DEBUG_CAMERA_BOUNDS

CWorld::CWorld()
  : CGameState(GST_WORLD),
    m_pPlayer(nullptr) {

  Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("World");

  // create the world camera
  m_pWorldCamera = CGame::getSingleton().getSceneManager()->createCamera("WorldCamera");
  m_pWorldCamera->setNearClipDistance(0.001f);
  m_pWorldCamera->setFarClipDistance(10000.0f);
  // default position
  m_pWorldCamera->setPosition(Ogre::Vector3(0,1,1));
  m_pWorldCamera->lookAt(Ogre::Vector3(0,0,0));
  //sm_pWorldCamera->setProjectionType(Ogre::PT_ORTHOGRAPHIC);

  Ogre::Viewport *vp = CGame::getSingleton().getMainViewPort();
  m_pWorldCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
  vp->setCamera(m_pWorldCamera);

  // create the player
  m_pPlayer = new CPlayer(this, m_pWorldCamera, CGame::getSingleton().getSceneManager());

  // create the atlas
  m_pAtlas = new CAtlas(this, CGame::getSingleton().getSceneManager()->getRootSceneNode(), m_pPlayer);

  // initialize the player done in atlas, when new map is created

  //m_pCameraPerspective = new CAerialCameraPerspective(m_pWorldCamera, (Ogre::SceneNode*)m_pAtlas->getChildren().front()->getSceneNode()->getChild(0));
  m_pCameraPerspective = new CAerialCameraPerspective(m_pWorldCamera, m_pPlayer->getSceneNode());

  setupEdgeRays();
}

CWorld::~CWorld() {
  delete m_pCameraPerspective;
  delete m_pPlayer;
  delete m_pAtlas;
}

void CWorld::update(Ogre::Real tpf) {
  CGameState::update(tpf);
  m_pCameraPerspective->updateCamera(tpf);

#ifdef WORLD_DEBUG_CAMERA_BOUNDS
  for (int i = 0; i < RAY_COUNT; i++) {
    Ogre::Ray r = m_pWorldCamera->getCameraToViewportRay(i % 2, i / 2);
    DebugDrawer::getSingleton().drawLine(r.getOrigin(), r.getPoint(1000), Ogre::ColourValue::Red);

    DebugDrawer::getSingleton().drawSphere(getRayPlaneHitPosition(i), 0.2, Ogre::ColourValue::Red, false);
  }
#endif
}

bool CWorld::frameRenderingQueued(const Ogre::FrameEvent& evt) {

  return true;
}

void CWorld::setupEdgeRays() {
  const Ogre::Radian beta(m_pWorldCamera->getFOVy());
  const Ogre::Radian alpha(Ogre::Math::ATan(Ogre::Math::Tan(beta) * m_pWorldCamera->getAspectRatio()));

  LOGV("View angles x='%f' y='%f'", alpha.valueDegrees(), beta.valueDegrees());

  const Ogre::Vector3 vLeftRotXAxis(Ogre::Quaternion(alpha, Ogre::Vector3::UNIT_Y).xAxis());
  const Ogre::Vector3 vRightRotXAxis(-(Ogre::Quaternion(-alpha, Ogre::Vector3::UNIT_Y).xAxis()));

  m_vEdgeRayDirections[RAY_TOP_LEFT] = -(Ogre::Quaternion(-beta, vLeftRotXAxis) * Ogre::Quaternion(alpha, Ogre::Vector3::UNIT_Y).zAxis());
  m_vEdgeRayDirections[RAY_BOTTOM_LEFT] = -(Ogre::Quaternion(beta, vLeftRotXAxis) * Ogre::Quaternion(alpha, Ogre::Vector3::UNIT_Y).zAxis());
  m_vEdgeRayDirections[RAY_TOP_RIGHT] = -(Ogre::Quaternion(-beta, vRightRotXAxis) * Ogre::Quaternion(-alpha, Ogre::Vector3::UNIT_Y).zAxis());
  m_vEdgeRayDirections[RAY_BOTTOM_RIGHT] = -(Ogre::Quaternion(beta, vRightRotXAxis) * Ogre::Quaternion(-alpha, Ogre::Vector3::UNIT_Y).zAxis());
}

Ogre::Vector3 CWorld::getRayPlaneHitPosition(int iRayIndex, Ogre::Real fPlaneY) const {
  Ogre::Ray r = m_pWorldCamera->getCameraToViewportRay(iRayIndex % 2, iRayIndex / 2);
  std::pair<bool, Ogre::Real> bIntersection = r.intersects(Ogre::Plane(Ogre::Vector3::UNIT_Y, fPlaneY));

  return r.getPoint(bIntersection.second);
}
