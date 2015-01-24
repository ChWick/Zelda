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

#ifndef _ABSTRACT_WORLD_ENTITY_HPP_
#define _ABSTRACT_WORLD_ENTITY_HPP_

#include <list>
#include <string>
#include "../GameLogic/Entity.hpp"
#include "InteractionInterface.hpp"
#include "HitableInterface.hpp"
#include "AttackerInterface.hpp"

class CAbstractMap;
namespace ParticleUniverse {
class ParticleSystem;
};

class CWorldEntityConstructionInfo;

class CAbstractWorldEntity : public CEntity,
                     public CHitableInterface,
                     public CAttackerInterface,
                     public CInteractionInterface {
protected:
  Ogre::SceneNode *m_pSceneNode;
  btCollisionObject *m_pCollisionObject;
  CAbstractMap *m_pMap;
  uint16_t mCollisionMask;
  uint16_t mCollisionGroup;

  std::list<ParticleUniverse::ParticleSystem*> mParticleSystems;

public:
  CAbstractWorldEntity(const std::string &sID, CEntity *pParent, CAbstractMap *pMap, const std::string &sResourceGroup = Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME);
  CAbstractWorldEntity(CEntity *pParent, CAbstractMap *pMap, const tinyxml2::XMLElement *pElem, const std::string &sResourceGroup = Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME);
  CAbstractWorldEntity(CAbstractWorldEntity *parent, const CWorldEntityConstructionInfo &info);
  
  virtual ~CAbstractWorldEntity();
  
  virtual void exit();
  
  virtual void start();
  virtual void stop();

  ParticleUniverse::ParticleSystem *createParticleSystem(
      const std::string &name_prefix,
      const std::string &type,
      bool autoAttach = true);

  virtual const SPATIAL_VECTOR &getPosition() const;
  virtual void setPosition(const SPATIAL_VECTOR &vPos);
  virtual void translate(const SPATIAL_VECTOR &vOffset);

  virtual const SPATIAL_VECTOR getFloorPosition() const {return getPosition();}
  virtual void setFloorPosition(const SPATIAL_VECTOR &p) {setPosition(p);}

  virtual const SPATIAL_VECTOR &getCenter() const;
  virtual void setCenter(const SPATIAL_VECTOR &vCenter);

  virtual const SPATIAL_VECTOR &getSize() const;
  virtual void setSize(const SPATIAL_VECTOR &vSize);

  virtual const SPATIAL_VECTOR &getScale() const;
  virtual void setScale(const SPATIAL_VECTOR &vScale);

  virtual const Ogre::Quaternion &getOrientation() const;
  virtual void setOrientation(const Ogre::Quaternion &quat);
  virtual void rotate(const Ogre::Quaternion &quat);

  virtual void warp(const SPATIAL_VECTOR &p, const Ogre::Quaternion &q);

  virtual Ogre::SceneNode *getSceneNode() const;
  virtual btCollisionObject *getCollisionObject() const;
  virtual void setCollisionObject(btCollisionObject *pCollisionObject);
  virtual CAbstractMap *getMap() const {return m_pMap;}
  virtual void enterMap(CAbstractMap *pMap, const Ogre::Vector3 &vPosition);

  virtual void update(Ogre::Real tpf);

  virtual SInteractionResult interactOnCollision(const Ogre::Vector3 &vInteractDir, CAbstractWorldEntity *pSender);
  virtual SInteractionResult interactOnActivate(const Ogre::Vector3 &vInteractDir, CAbstractWorldEntity *pSender);

  uint16_t getCollisionMask() const {return mCollisionMask;}
  uint16_t getCollisionGroup() const {return mCollisionGroup;}

 protected:
  virtual void damageAccepted(const CDamage &damage);


 public:
  void setThisAsCollisionObjectsUserPointer();
  void setThisAsCollisionObjectsUserPointer(btCollisionObject *pCollsionObject);
  
  static CAbstractWorldEntity *getFromUserPointer(btCollisionObject *pCO);
  static CAbstractWorldEntity *getFromUserPointer(const btCollisionObject *pCO);
};

#endif // _ABSTRACT_WORLD_ENTITY_HPP_

