#ifndef _AERIAL_CAMERA_PERSPECTIVE_HPP_
#define _AERIAL_CAMERA_PERSPECTIVE_HPP_

#include <OgrePrerequisites.h>

class CAerialCameraPerspective {
protected:
  Ogre::Camera *m_pCamera;
  const Ogre::SceneNode *m_pTargetSceneNode;
  Ogre::SceneNode *m_pSceneNode;
public:
  CAerialCameraPerspective(Ogre::Camera *pCamera, Ogre::SceneNode *pTargetSceneNode);

  void updateCamera(float tpf);
};

#endif // _AERIAL_CAMERA_PERSPECTIVE_HPP_
