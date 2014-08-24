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

#ifndef _AERIAL_CAMERA_PERSPECTIVE_HPP_
#define _AERIAL_CAMERA_PERSPECTIVE_HPP_

#include <OgrePrerequisites.h>
#include "../../Common/Message/MessageInjector.hpp"
#include <OgreVector2.h>

class CWorldEntity;

class CAerialCameraPerspective : public CMessageInjector {
protected:
  Ogre::Camera *m_pCamera;
  const CWorldEntity *m_pTarget;
  Ogre::SceneNode *m_pSceneNode;

  enum ECameraEdgeRays {
    RAY_TOP_LEFT = 0,
    RAY_TOP_RIGHT,
    RAY_BOTTOM_LEFT,
    RAY_BOTTOM_RIGHT,
    RAY_COUNT
  };

  Ogre::Vector2 m_vMinCamPoint;
  Ogre::Vector2 m_vMaxCamPoint;

  bool m_bSwitchingMap;
  bool m_bCameraInBounds;
public:
  CAerialCameraPerspective(Ogre::Camera *pCamera,
                           CWorldEntity *pTarget);

  virtual ~CAerialCameraPerspective();

  void updateCamera(float tpf);

  bool isCameraInBounds() const {return m_bCameraInBounds;}

private:
  void sendMessageToAll(const CMessage &message);

  Ogre::Vector3 getRayPlaneHitPosition(int iRayIndex, Ogre::Real fPlaneY = 0) const;
};

#endif // _AERIAL_CAMERA_PERSPECTIVE_HPP_
