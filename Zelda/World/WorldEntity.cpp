#include "WorldEntity.hpp"
#include <OgreSceneNode.h>
#include <BulletCollision/CollisionDispatch/btCollisionObject.h>

CWorldEntity::CWorldEntity(const std::string &sID, CEntity *pParent, CMap *pMap)
  : CEntity(sID, pParent),
    m_pSceneNode(nullptr),
    m_pCollisionObject(nullptr),
    m_pMap(pMap) {
}

const SPATIAL_VECTOR &CWorldEntity::getPosition() const {
  return m_pSceneNode->getPosition();
}
void CWorldEntity::setPosition(const SPATIAL_VECTOR &vPos) {
  m_pSceneNode->setPosition(vPos);
}
void CWorldEntity::translate(const SPATIAL_VECTOR &vOffset) {
  m_pSceneNode->translate(vOffset);
}

const SPATIAL_VECTOR &CWorldEntity::getCenter() const {
  return getPosition();
}
void CWorldEntity::setCenter(const SPATIAL_VECTOR &vCenter) {
  setPosition(vCenter);
}

const SPATIAL_VECTOR &CWorldEntity::getSize() const {
  return getScale();
}
void CWorldEntity::setSize(const SPATIAL_VECTOR &vSize) {
  setScale(vSize);
}

const SPATIAL_VECTOR &CWorldEntity::getScale() const {
  return m_pSceneNode->getScale();
}
void CWorldEntity::setScale(const SPATIAL_VECTOR &vScale) {
  m_pSceneNode->setScale(vScale);
}

const Ogre::Quaternion &CWorldEntity::getOrientation() const {
  return m_pSceneNode->getOrientation();
}
void CWorldEntity::setOrientation(const Ogre::Quaternion &quat) {
  m_pSceneNode->setOrientation(quat);
}

Ogre::SceneNode *CWorldEntity::getSceneNode() const {
  return m_pSceneNode;
}
btCollisionObject *CWorldEntity::getCollisionObject() const {
  return m_pCollisionObject;
}

void CWorldEntity::update(Ogre::Real tpf) {
  CHitableInterface::update(tpf);
  CEntity::update(tpf);
}

void CWorldEntity::setThisAsCollisionObjectsUserPointer() {
  assert(m_pCollisionObject);
  m_pCollisionObject->setUserPointer(this);
}

CWorldEntity *CWorldEntity::getFromUserPointer(btCollisionObject *pCO) {
  assert(pCO);
  return static_cast<CWorldEntity*>(pCO->getUserPointer());
}
