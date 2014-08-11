#ifndef _WORLD_ENTITY_HPP_
#define _WORLD_ENTITY_HPP_

#include "../Common/GameLogic/Entity.hpp"
#include "HitableInterface.hpp"

class CMap;

class CWorldEntity : public CEntity, public CHitableInterface {
protected:
  Ogre::SceneNode *m_pSceneNode;
	btCollisionObject *m_pCollisionObject;
	CMap *m_pMap;

public:
  CWorldEntity(const std::string &sID, CEntity *pParent, CMap *pMap);

  virtual const SPATIAL_VECTOR &getPosition() const;
  virtual void setPosition(const SPATIAL_VECTOR &vPos);
  virtual void translate(const SPATIAL_VECTOR &vOffset);

  virtual const SPATIAL_VECTOR &getCenter() const;
  virtual void setCenter(const SPATIAL_VECTOR &vCenter);

  virtual const SPATIAL_VECTOR &getSize() const;
  virtual void setSize(const SPATIAL_VECTOR &vSize);

  virtual const SPATIAL_VECTOR &getScale() const;
  virtual void setScale(const SPATIAL_VECTOR &vScale);

	virtual const Ogre::Quaternion &getOrientation() const;
	virtual void setOrientation(const Ogre::Quaternion &quat);

  virtual Ogre::SceneNode *getSceneNode() const;
  virtual btCollisionObject *getCollisionObject() const;
  virtual CMap *getMap() const {return m_pMap;}
  virtual void enterMap(CMap *pMap, const Ogre::Vector3 &vPosition) {m_pMap = pMap; setPosition(vPosition);}

  virtual void update(Ogre::Real tpf);


  void setThisAsCollisionObjectsUserPointer();
  static CWorldEntity *getFromUserPointer(btCollisionObject *pCO);
  static const CWorldEntity *getFromUserPointer(const btCollisionObject *pCO);
};

#endif // _WORLD_ENTITY_HPP_

