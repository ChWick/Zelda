#include "AerialCameraPerspective.hpp"
#include <OgreVector3.h>
#include <OgreSceneNode.h>
#include <OgreCamera.h>

const Ogre::Vector3 AERIAL_CAMERA_OFFSET(0, 9e0, -4e0);

CAerialCameraPerspective::CAerialCameraPerspective(Ogre::Camera *pCamera, Ogre::SceneNode *pTargetSceneNode)
  : m_pCamera(pCamera),
    m_pTargetSceneNode(pTargetSceneNode) {
  //m_pSceneNode = pTargetSceneNode->createChildSceneNode(AERIAL_CAMERA_OFFSET);
  //m_pSceneNode->attachObject(pCamera);
  m_pCamera->setPosition(AERIAL_CAMERA_OFFSET);
  m_pCamera->lookAt(0, 0, 0);
}

void CAerialCameraPerspective::updateCamera(float tpf) {
  m_pCamera->setPosition(m_pTargetSceneNode->getPosition() + AERIAL_CAMERA_OFFSET);
}
