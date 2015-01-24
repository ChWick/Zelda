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

#ifndef ABSTRACTMAP_HPP
#define ABSTRACTMAP_HPP

#include <OgrePrerequisites.h>
#include "../Config/TypeDefines.hpp"
#include WORLD_ENTITY_HEADER
#include "AbstractMapPack.hpp"
#include "../../Common/Physics/PhysicsManager.hpp"


class CAbstractMap
    : public WORLD_ENTITY_CLASS {
 protected:
  CPhysicsManager m_PhysicsManager;
  CAbstractMapPackPtr m_MapPack;
  WORLD_ENTITY_CLASS *m_pPlayer;

  Ogre::StaticGeometry *m_pStaticGeometry;
  std::string mPrependNodeName;
  
  std::map<std::string, Ogre::Entity*> m_mStaticEntitiesMap;
 public:
  CAbstractMap(CEntity *parent,
               CAbstractMapPackPtr mapPack,
               Ogre::SceneNode *pParentSceneNode,
               WORLD_ENTITY_CLASS *pPlayer);
  virtual ~CAbstractMap() {}

  virtual void exit();
  virtual void init();

  virtual void start();

  virtual bool frameStarted(const Ogre::FrameEvent& evt);

  const CPhysicsManager *getPhysicsManager() const {return &m_PhysicsManager;}
  CPhysicsManager *getPhysicsManager() {return &m_PhysicsManager;}
  const CAbstractMapPackPtr getMapPack() const {return m_MapPack;}
  const WORLD_ENTITY_CLASS *getPlayer() const {return m_pPlayer;}
  WORLD_ENTITY_CLASS *getPlayer() {return m_pPlayer;}
  const std::string &getPrependNodeName() const {return mPrependNodeName;}
  
  void addStaticEntity(const std::string &entity, const Ogre::Vector3 &vPosition, const Ogre::Quaternion &vRotation);

 protected:
  // collision checks
  void processCollisionCheck();
};

#endif /* ABSTRACTMAP_HPP */
