#include "AerialCameraPerspective.hpp"
#include <OgreVector3.h>
#include <OgreSceneNode.h>
#include <OgreCamera.h>

const Ogre::Vector3 AERIAL_CAMERA_OFFSET(0, 1e1, 1e1);

CAerialCameraPerspective::CAerialCameraPerspective(Ogre::Camera *pCamera, Ogre::SceneNode *pTargetSceneNode)
  : m_pCamera(pCamera),
    m_pTargetSceneNode(pTargetSceneNode) {
  //m_pSceneNode = pTargetSceneNode->createChildSceneNode(AERIAL_CAMERA_OFFSET);
  //m_pSceneNode->attachObject(pCamera);
}

void CAerialCameraPerspective::updateCamera(float tpf) {
  m_pCamera->setPosition(m_pTargetSceneNode->getPosition() + AERIAL_CAMERA_OFFSET);
}
