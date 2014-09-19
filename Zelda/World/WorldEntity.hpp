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

#ifndef _WORLD_ENTITY_HPP_
#define _WORLD_ENTITY_HPP_

#include "InteractionInterface.hpp"
#include "../Common/GameLogic/Entity.hpp"
#include "HitableInterface.hpp"

class CMap;

class CWorldEntity : public CEntity, public CHitableInterface, public CInteractionInterface {
protected:
  Ogre::SceneNode *m_pSceneNode;
	btCollisionObject *m_pCollisionObject;
	CMap *m_pMap;

public:
  CWorldEntity(const std::string &sID, CEntity *pParent, CMap *pMap, const std::string &sResourceGroup = Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME);
  CWorldEntity(CEntity *pParent, CMap *pMap, const tinyxml2::XMLElement *pElem, const std::string &sResourceGroup = Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME);
  virtual ~CWorldEntity();

  virtual void exit();

  virtual const SPATIAL_VECTOR &getPosition() const;
  virtual void setPosition(const SPATIAL_VECTOR &vPos);
  virtual void translate(const SPATIAL_VECTOR &vOffset);

  virtual const SPATIAL_VECTOR getFloorPosition() const {return getPosition();}

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
  virtual void enterMap(CMap *pMap, const Ogre::Vector3 &vPosition);

  virtual void update(Ogre::Real tpf);


  void setThisAsCollisionObjectsUserPointer();
  static CWorldEntity *getFromUserPointer(btCollisionObject *pCO);
  static CWorldEntity *getFromUserPointer(const btCollisionObject *pCO);


  virtual SInteractionResult interactOnCollision(const Ogre::Vector3 &vInteractDir, CWorldEntity *pSender);
  virtual SInteractionResult interactOnActivate(const Ogre::Vector3 &vInteractDir, CWorldEntity *pSender);

protected:
  virtual void damageAccepted(const CDamage &damage);
};

#endif // _WORLD_ENTITY_HPP_

