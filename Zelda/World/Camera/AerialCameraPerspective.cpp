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

#include "AerialCameraPerspective.hpp"
#include <OgreVector3.h>
#include <OgreSceneNode.h>
#include <OgreCamera.h>
#include "../../Common/Util/DebugDrawer.hpp"
#include "../../Common/Message/MessageSwitchMap.hpp"
#include "../../Common/Message/MessageHandler.hpp"
#include "../Atlas/Map.hpp"
#include "../../Common/Log.hpp"

// drow camera bounds (lines and spheres)
#define WORLD_DEBUG_CAMERA_BOUNDS

const Ogre::Vector3 AERIAL_CAMERA_OFFSET(0, 9e0, 4e0);
const Ogre::Real AERIAL_CAMERA_MOVE_SPEED(10);

CAerialCameraPerspective::CAerialCameraPerspective(Ogre::Camera *pCamera,
                                                   Ogre::SceneNode *pTargetSceneNode)
  : m_pCamera(pCamera),
    m_pTargetSceneNode(pTargetSceneNode),
    m_vMinCamPoint(Ogre::Vector2::ZERO),
    m_vMaxCamPoint(Ogre::Vector2::ZERO),
    m_bSwitchingMap(false),
    m_bCameraInBounds(false) {

  CMessageHandler::getSingleton().addInjector(this);
  //m_pSceneNode = pTargetSceneNode->createChildSceneNode(AERIAL_CAMERA_OFFSET);
  //m_pSceneNode->attachObject(pCamera);
  m_pCamera->setPosition(AERIAL_CAMERA_OFFSET);
  m_pCamera->lookAt(0, 0, 0);
}

CAerialCameraPerspective::~CAerialCameraPerspective() {
  CMessageHandler::getSingleton().removeInjector(this);
}

void CAerialCameraPerspective::updateCamera(float tpf) {
  Ogre::Vector3 vTargetPosition = m_pTargetSceneNode->getPosition() + AERIAL_CAMERA_OFFSET;

  // back up position and set it to desired to calculate the bounds
  const Ogre::Vector3 vCamPosBuffer(m_pCamera->getPosition());

  Ogre::Vector3 vRayPos;
  Ogre::Vector3 vDelta;
  // update camera bounds
  m_pCamera->setPosition(vTargetPosition);
  vRayPos = getRayPlaneHitPosition(RAY_BOTTOM_RIGHT);
  vDelta = Ogre::Vector3(std::max<Ogre::Real>(m_vMinCamPoint.x - vRayPos.x, 0), 0, std::max<Ogre::Real>(m_vMinCamPoint.y - vRayPos.z, 0));
  vTargetPosition += vDelta;

  m_pCamera->setPosition(vTargetPosition);
  vRayPos = getRayPlaneHitPosition(RAY_TOP_LEFT);
  vDelta = Ogre::Vector3(std::min<Ogre::Real>(m_vMaxCamPoint.x - vRayPos.x, 0), 0, std::min<Ogre::Real>(m_vMaxCamPoint.y - vRayPos.z, 0));
  vTargetPosition += vDelta;

  // use cameras backup position and move it
  Ogre::Vector3 vCamMoveDir(vTargetPosition - vCamPosBuffer);
  Ogre::Real fMaxStep = vCamMoveDir.normalise();
  m_bCameraInBounds = fMaxStep < AERIAL_CAMERA_MOVE_SPEED * tpf;
  m_pCamera->setPosition(vCamPosBuffer + vCamMoveDir * std::min<Ogre::Real>(fMaxStep, AERIAL_CAMERA_MOVE_SPEED * tpf));

  //LOGI("targetPos: x=%f y=%f", vTargetPosition.x, vTargetPosition.z);


#ifdef WORLD_DEBUG_CAMERA_BOUNDS
  for (int i = 0; i < RAY_COUNT; i++) {
    Ogre::Ray r = m_pCamera->getCameraToViewportRay(i % 2, i / 2);
    DebugDrawer::getSingleton().drawLine(r.getOrigin(), r.getPoint(1000), Ogre::ColourValue::Red);

    DebugDrawer::getSingleton().drawSphere(getRayPlaneHitPosition(i), 0.2, Ogre::ColourValue::Red, false);
  }
#endif
}

Ogre::Vector3 CAerialCameraPerspective::getRayPlaneHitPosition(int iRayIndex, Ogre::Real fPlaneY) const {
  Ogre::Ray r = m_pCamera->getCameraToViewportRay(iRayIndex % 2, iRayIndex / 2);
  std::pair<bool, Ogre::Real> bIntersection = r.intersects(Ogre::Plane(Ogre::Vector3::UNIT_Y, fPlaneY));

  return r.getPoint(bIntersection.second);
}



void CAerialCameraPerspective::sendMessageToAll(const CMessage &message) {
  if (message.getType() == MSG_SWITCH_MAP) {
    const CMessageSwitchMap &switch_map_message(dynamic_cast<const CMessageSwitchMap&>(message));
    if (switch_map_message.getStatus() == CMessageSwitchMap::SWITCHING) {
      m_bSwitchingMap = true;

      const CMapPackPtr nextPack = switch_map_message.getToMap()->getMapPack();
      const CMapPackPtr currPack = switch_map_message.getFromMap()->getMapPack();

      Ogre::Vector3 vMapPositionOffset = currPack->getGlobalPosition() - nextPack->getGlobalPosition();

      m_pCamera->setPosition(m_pCamera->getPosition() + vMapPositionOffset);
    }
    else if (switch_map_message.getStatus() == CMessageSwitchMap::FINISHED) {
      m_bSwitchingMap = false;

      const CMapPackPtr pack = switch_map_message.getFromMap()->getMapPack();
      Ogre::Vector2 vSize(pack->getGlobalSize().x, pack->getGlobalSize().y);
      m_vMinCamPoint = -vSize / 2;
      m_vMaxCamPoint = vSize / 2;
    }
  }
}
