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
#include "../WorldEntity.hpp"

// drow camera bounds (lines and spheres)
#define WORLD_DEBUG_CAMERA_BOUNDS

const Ogre::Vector3 AERIAL_CAMERA_OFFSET(0, 9e-1, 3e-1);
const Ogre::Vector3 AERIAL_CAMERA_OFFSET_AT_PLATEAU(0, 9e-1, 0.1);
const Ogre::Real AERIAL_CAMERA_PLATEAU_VALUE_AT(0.2);

const Ogre::Real AERIAL_CAMERA_MOVE_SPEED(1);

CAerialCameraPerspective::CAerialCameraPerspective(Ogre::Camera *pCamera,
                                                   CWorldEntity *pTarget)
  : m_pCamera(pCamera),
    m_pTarget(pTarget),
    m_vMinCamPoint(Ogre::Vector2::ZERO),
    m_vMaxCamPoint(Ogre::Vector2::ZERO),
    m_fVisionLevelOffset(0),
    m_bSwitchingMap(false),
    m_bCameraInBounds(false) {
  //m_pSceneNode = pTargetSceneNode->createChildSceneNode(AERIAL_CAMERA_OFFSET);
  //m_pSceneNode->attachObject(pCamera);
  m_pCamera->setPosition(AERIAL_CAMERA_OFFSET);
  m_pCamera->lookAt(0, 0, 0);
}

CAerialCameraPerspective::~CAerialCameraPerspective() {
}

void CAerialCameraPerspective::updateCamera(float tpf) {
  const Ogre::Vector3 vCurrentOffset(AERIAL_CAMERA_OFFSET + (AERIAL_CAMERA_OFFSET_AT_PLATEAU - AERIAL_CAMERA_OFFSET) * std::min<Ogre::Real>(1.f, std::max<Ogre::Real>(0.f, m_pTarget->getFloorPosition().y / AERIAL_CAMERA_PLATEAU_VALUE_AT)));
  Ogre::Vector3 vTargetPosition = m_pTarget->getPosition() + vCurrentOffset;

  // back up position and set it to desired to calculate the bounds
  const Ogre::Vector3 vCamPosBuffer(m_pCamera->getPosition());

  Ogre::Vector3 vRayPos;
  Ogre::Vector3 vDelta;
  // update camera bounds
  m_pCamera->setPosition(vTargetPosition);
  vRayPos = getRayPlaneHitPosition(RAY_TOP_LEFT, m_fVisionLevelOffset);
  vDelta = Ogre::Vector3(std::max<Ogre::Real>(m_vMinCamPoint.x - vRayPos.x, 0), 0, std::max<Ogre::Real>(m_vMinCamPoint.y - vRayPos.z, 0));
  vTargetPosition += vDelta;

  m_pCamera->setPosition(vTargetPosition);
  vRayPos = getRayPlaneHitPosition(RAY_TOP_RIGHT, m_fVisionLevelOffset);
  vDelta = Ogre::Vector3(std::min<Ogre::Real>(m_vMaxCamPoint.x - vRayPos.x, 0), 0, std::max<Ogre::Real>(m_vMinCamPoint.y - vRayPos.z, 0));
  vTargetPosition += vDelta;

  m_pCamera->setPosition(vTargetPosition);
  vRayPos = getRayPlaneHitPosition(RAY_BOTTOM_RIGHT, m_fVisionLevelOffset);
  vDelta = Ogre::Vector3(std::min<Ogre::Real>(m_vMaxCamPoint.x - vRayPos.x, 0), 0, std::min<Ogre::Real>(m_vMaxCamPoint.y - vRayPos.z, 0));
  vTargetPosition += vDelta;

  // only one bottom required, since smaller edge is bottom

  // use cameras backup position and move it
  Ogre::Vector3 vCamMoveDir(vTargetPosition - vCamPosBuffer);
  Ogre::Real fMaxStep = vCamMoveDir.normalise();
  m_bCameraInBounds = fMaxStep < AERIAL_CAMERA_MOVE_SPEED * tpf;
  m_pCamera->setPosition(vCamPosBuffer + vCamMoveDir * std::min<Ogre::Real>(fMaxStep, AERIAL_CAMERA_MOVE_SPEED * tpf));

  m_pCamera->setDirection(-vCurrentOffset);

  //LOGI("targetPos: x=%f y=%f", vTargetPosition.x, vTargetPosition.z);
}

void CAerialCameraPerspective::renderDebug(Ogre::Real tpf) {
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



void CAerialCameraPerspective::sendMessageToAll(const CMessagePtr message) {
  if (message->getType() == MSG_SWITCH_MAP) {
    auto switch_map_message(std::dynamic_pointer_cast<const CMessageSwitchMap>(message));
    if (switch_map_message->getStatus() == CMessageSwitchMap::SWITCHING) {
      if (switch_map_message->getSwitchMapType() == SMT_MOVE_CAMERA) {
        m_bSwitchingMap = true;

        const CMapPackPtr nextPack = switch_map_message->getToMap()->getMapPack();
        const CMapPackPtr currPack = switch_map_message->getFromMap()->getMapPack();

        Ogre::Vector3 vMapPositionOffset = currPack->getGlobalPosition() - nextPack->getGlobalPosition();

        m_pCamera->setPosition(m_pCamera->getPosition() + vMapPositionOffset);
      } else {
        const CMapPackPtr pack = switch_map_message->getFromMap()->getMapPack();
        Ogre::Vector2 vSize(pack->getGlobalSize().x, pack->getGlobalSize().y);
        m_vMinCamPoint = -vSize / 2;
        m_vMaxCamPoint = vSize / 2;

        m_fVisionLevelOffset = pack->getVisionLevelOffset();

        // move camera to new point
        updateCamera(1000);
      }
    }
    else if (switch_map_message->getStatus() == CMessageSwitchMap::FINISHED) {
      m_bSwitchingMap = false;

      const CMapPackPtr pack = switch_map_message->getFromMap()->getMapPack();
      Ogre::Vector2 vSize(pack->getGlobalSize().x, pack->getGlobalSize().y);
      m_vMinCamPoint = -vSize / 2;
      m_vMaxCamPoint = vSize / 2;

      m_fVisionLevelOffset = pack->getVisionLevelOffset();

      if (switch_map_message->getSwitchMapType() != SMT_MOVE_CAMERA) {
        // move camera to new point
        updateCamera(1000);
      }
    }
  }
}
