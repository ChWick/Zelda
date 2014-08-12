#ifndef _AERIAL_CAMERA_PERSPECTIVE_HPP_
#define _AERIAL_CAMERA_PERSPECTIVE_HPP_

#include <OgrePrerequisites.h>
#include "../../Common/Message/MessageInjector.hpp"
#include <OgreVector2.h>

class CAerialCameraPerspective : public CMessageInjector {
protected:
  Ogre::Camera *m_pCamera;
  const Ogre::SceneNode *m_pTargetSceneNode;
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
public:
  CAerialCameraPerspective(Ogre::Camera *pCamera,
                           Ogre::SceneNode *pTargetSceneNode);

  ~CAerialCameraPerspective();

  void updateCamera(float tpf);

private:
  void sendMessageToAll(const CMessage &message);

  Ogre::Vector3 getRayPlaneHitPosition(int iRayIndex, Ogre::Real fPlaneY = 0) const;
};

#endif // _AERIAL_CAMERA_PERSPECTIVE_HPP_
