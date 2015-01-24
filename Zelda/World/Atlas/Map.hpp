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

#ifndef _MAP_HPP_
#define _MAP_HPP_

#include "../../Common/World/AbstractMap.hpp"
#include "../../Common/PauseManager/PauseListener.hpp"
#include "MapPack.hpp"
#include "../../Common/DotSceneLoader/DotSceneLoader.hpp"
#include "../../Common/DotSceneLoader/DotSceneLoaderCallback.hpp"
#include <btBulletDynamicsCommon.h>
#include "MapPackParserListener.hpp"
#include <OgreStaticGeometry.h>
#include "TileTypes.hpp"
#include <OgreMaterial.h>
#include <map>

class CMap final : public CAbstractMap,
                   protected CMapPackParserListener,
                   private CDotSceneLoaderCallback,
                   public CPauseListener {
private:
  Ogre::DotSceneLoader m_SceneLoader;
  Ogre::StaticGeometry *m_pStaticGeometryChangedTiles;          //!< Here are the tiles added that can be remved (bush in place)
  Ogre::StaticGeometry *m_pStaticGeometryFixedTiles;            //!< Here are new static tiles added, initially blank. Afterwards it is all static

  Ogre::Entity *m_apTileEntities[TT_COUNT];
  Ogre::Entity *m_pFirstFlowerEntity;
  Ogre::AnimationState *m_pFlowerAnimationState;
  Ogre::MaterialPtr m_pWaterSideWaveMaterial;
public:
  CMap(CEntity *pAtlas, CMapPackPtr mapPack, Ogre::SceneNode *pParentSceneNode, CWorldEntity *pPlayer);
  virtual ~CMap();

  virtual void init() override;
  virtual void start() override;
  virtual void exit() override;

  //! this is just a test function, dont use
  void CreateCube(const btVector3 &Position, btScalar Mass);

  //! creates an effect in this map
  void createEffect();
  

  void moveMap(const Ogre::Vector3 &offset);

  void update(Ogre::Real tpf) override;
  bool frameStarted(const Ogre::FrameEvent& evt) override;
  bool frameEnded(const Ogre::FrameEvent& evt) override;

private:
  void handleMessage(const CMessagePtr message) override;
  void updatePause(int iPauseType, bool bPause) override;
  
  void rebuildStaticGeometryChangedTiles();
  void translateStaticGeometry(Ogre::StaticGeometry *pSG, const Ogre::Vector3 &vVec);

  // CMapPackParserListener
  void parseEvent(const tinyxml2::XMLElement *) override;
  void parsePlayer(const tinyxml2::XMLElement *) override;
  void parseRegion(const tinyxml2::XMLElement *pElem) override;
  void parseEntrance(const tinyxml2::XMLElement *pElem) override;
  void parseSceneEntity(const tinyxml2::XMLElement *) override;
  void parseNewEntity(const tinyxml2::XMLElement *) override;

  // CDotSceneLoaderCallback
  void physicsShapeCreated(btCollisionShape *pShape, const std::string &sMeshName) override;
  void worldPhysicsAdded(btRigidBody *pRigidBody) override;
  void postEntityAdded(Ogre::Entity *pEntity, Ogre::SceneNode *pParent, btRigidBody *pRigidBody, const CUserData &userData) override;
  void staticObjectAdded(Ogre::Entity *pEntity, Ogre::SceneNode *pParent) override;
  EResults preEntityAdded(tinyxml2::XMLElement *XMLNode, Ogre::SceneNode *pParent, CUserData &userData) override;

};
#endif // _MAP_HPP_
