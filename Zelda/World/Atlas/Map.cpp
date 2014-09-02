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

#include "Map.hpp"
#include "../../Common/Physics/BtOgrePG.hpp"
#include "../../Common/Util/DeleteSceneNode.hpp"
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreEntity.h>
#include <OgreLogManager.h>
#include <OgreAnimationState.h>
#include <OgreMeshManager.h>
#include "Region.hpp"
#include "../GlobalCollisionShapesTypes.hpp"
#include "../Objects/Object.hpp"
#include "BulletCollision/CollisionShapes/btSphereShape.h"
#include <BulletCollision/CollisionShapes/btCapsuleShape.h>
#include <BulletCollision/CollisionShapes/btCylinderShape.h>
#include <BulletCollision/CollisionShapes/btBoxShape.h>
#include "../Character/Person.hpp"
#include "../../Common/Util/Assert.hpp"
#include "../../Common/Message/MessageEntityStateChanged.hpp"
#include "../../Common/DotSceneLoader/UserData.hpp"
#include "../../Common/Util/XMLHelper.hpp"

#include "../Character/SimpleEnemy.hpp"

using namespace XMLHelper;

int MAP_COUNTER = 0; // Counter to make names unique if objects are switched between maps since renaming a scene node is not possible

CMap::CMap(CEntity *pAtlas, CMapPackPtr mapPack, Ogre::SceneNode *pParentSceneNode, CWorldEntity *pPlayer)
  : CWorldEntity(mapPack->getName(), pAtlas, this),
    m_PhysicsManager(pParentSceneNode->getCreator()),
    m_MapPack(mapPack),
    m_pPlayer(pPlayer),
    m_pFirstFlowerEntity(nullptr),
    m_pFlowerAnimationState(nullptr) {

  Ogre::LogManager::getSingleton().logMessage("Construction of map '" + m_MapPack->getName() + "'");

  // Create global collision shapes
  m_PhysicsManager.addCollisionShape(GLOBAL_COLLISION_SHAPES_TYPES_ID_MAP.toString(GCST_PICKABLE_OBJECT_SPHERE),
                                     CPhysicsCollisionObject(new btSphereShape(0.04), Ogre::Vector3::NEGATIVE_UNIT_Y * 0.03));
  m_PhysicsManager.addCollisionShape(GLOBAL_COLLISION_SHAPES_TYPES_ID_MAP.toString(GCST_PERSON_CAPSULE),
                                     CPhysicsCollisionObject(new btCapsuleShape(CPerson::PERSON_RADIUS, CPerson::PERSON_HEIGHT - 2 * CPerson::PERSON_RADIUS)));
  btCompoundShape *pHouseEntranceShape = new btCompoundShape();
  pHouseEntranceShape->addChildShape(btTransform(btQuaternion::getIdentity(), btVector3(-0.069, 0.08, 0.02)), new btBoxShape(btVector3(0.01, 0.08, 0.02)));
  pHouseEntranceShape->addChildShape(btTransform(btQuaternion::getIdentity(), btVector3(0.069, 0.08, 0.02)), new btBoxShape(btVector3(0.01, 0.08, 0.02)));
  pHouseEntranceShape->addChildShape(btTransform(btQuaternion::getIdentity(), btVector3(0.0, 0.16, 0.02)), new btBoxShape(btVector3(0.07, 0.01, 0.02)));
  m_PhysicsManager.addCollisionShape(GLOBAL_COLLISION_SHAPES_TYPES_ID_MAP.toString(GCST_HOUSE_ENTRANCE), CPhysicsCollisionObject(pHouseEntranceShape, Ogre::Vector3::ZERO));

  btCompoundShape *pStonePileShape = new btCompoundShape();
  pStonePileShape->addChildShape(btTransform(btQuaternion::getIdentity(), btVector3(0.05, 0.025, 0.05)), new btSphereShape(0.04));
  pStonePileShape->addChildShape(btTransform(btQuaternion::getIdentity(), btVector3(-0.05, 0.025, 0.05)), new btSphereShape(0.04));
  pStonePileShape->addChildShape(btTransform(btQuaternion::getIdentity(), btVector3(0.05, 0.025, -0.05)), new btSphereShape(0.04));
  pStonePileShape->addChildShape(btTransform(btQuaternion::getIdentity(), btVector3(-0.05, 0.025, -0.05)), new btSphereShape(0.04));
  pStonePileShape->addChildShape(btTransform(btQuaternion::getIdentity(), btVector3(-0.0, 0.06, -0.0)), new btSphereShape(0.04));
  m_PhysicsManager.addCollisionShape(GLOBAL_COLLISION_SHAPES_TYPES_ID_MAP.toString(GCST_STONE_PILE), CPhysicsCollisionObject(pStonePileShape, Ogre::Vector3::ZERO));

  m_PhysicsManager.addCollisionShape(GLOBAL_COLLISION_SHAPES_TYPES_ID_MAP.toString(GCST_TREE),
                                     CPhysicsCollisionObject(new btCylinderShape(btVector3(0.173, 0.2, 0.173)), Ogre::Vector3::NEGATIVE_UNIT_Y * 0.2));
  m_PhysicsManager.addCollisionShape(GLOBAL_COLLISION_SHAPES_TYPES_ID_MAP.toString(GCST_FALLING_OBJECT_SPHERE),
                                     CPhysicsCollisionObject(new btSphereShape(0.02), Ogre::Vector3::NEGATIVE_UNIT_Y * 0));


  // Create global entites
  for (int i = 0; i < TT_COUNT; i++) {
    m_apTileEntities[i] = pParentSceneNode->getCreator()->createEntity(TILE_TYPE_ID_MAP.toData(static_cast<ETileTypes>(i)).sMeshName);
  }

  m_pSceneNode = pParentSceneNode->createChildSceneNode(m_MapPack->getName() + "_RootNode");

  m_pStaticGeometry = pParentSceneNode->getCreator()->createStaticGeometry(m_MapPack->getName() + "_StaticGeometry");
  m_pStaticGeometry->setRegionDimensions(Ogre::Vector3(10, 10, 10));
  m_pStaticGeometry->setCastShadows(false);

  m_pStaticGeometryChangedTiles = pParentSceneNode->getCreator()->createStaticGeometry(m_MapPack->getName() + "_StaticGeometryChangedTiles");
  m_pStaticGeometryChangedTiles->setRegionDimensions(Ogre::Vector3(10, 10, 10));
  m_pStaticGeometryChangedTiles->setCastShadows(false);

  m_pStaticGeometryFixedTiles = pParentSceneNode->getCreator()->createStaticGeometry(m_MapPack->getName() + "_StaticGeometryFixedTiles");
  m_pStaticGeometryFixedTiles->setRegionDimensions(Ogre::Vector3(10, 10, 10));
  m_pStaticGeometryFixedTiles->setCastShadows(false);

  m_MapPack->init(this);


  m_SceneLoader.addCallback(this);
  m_SceneLoader.parseDotScene(m_MapPack->getSceneFile(),
                              m_MapPack->getResourceGroup(),
                              m_pSceneNode->getCreator(),
                              &m_PhysicsManager,
                              m_pSceneNode,
                              m_MapPack->getName() + Ogre::StringConverter::toString(MAP_COUNTER++));

  
  m_MapPack->parse();


  //CreateCube(btVector3(0, 10, 0.2), 1);
  //CreateCube(btVector3(0, 200, 0.3), 100);

  //new CObject(m_MapPack->getName() + "rupee", this, this, OBJECT_GREEN_BUSH);
  CSimpleEnemy *pEnemy = new CSimpleEnemy("enemy", this, CSimpleEnemy::ET_GREEN_SWORD);
  pEnemy->enterMap(this, Ogre::Vector3(0, 4, 1));


  /*btCollisionShape *pBox = new btBoxShape(btVector3(10, 0.1, 10));
  btRigidBody *pRB = new btRigidBody(0, new btDefaultMotionState(), pBox);
  m_pCollisionObject = pRB;
  m_PhysicsManager.getWorld()->addRigidBody(pRB);

  if (mapPack->getName() == "link_house") {
    Ogre::Entity *pEnt = m_pSceneNode->getCreator()->createEntity("link_house.mesh");
    pEnt->setMaterialName("water_side_wave");
    m_pSceneNode->attachObject(pEnt);
    //m_pStaticGeometry->addEntity(pEnt, Ogre::Vector3(0, 0, 0));
    //m_pSceneNode->getCreator()->destroyEntity(pEnt);
  }*/

  m_pStaticGeometry->build();
  rebuildStaticGeometryChangedTiles();

  m_pWaterSideWaveMaterial = Ogre::MaterialManager::getSingleton().getByName("water_side_wave");
  m_pWaterSideWaveMaterial->touch();
  m_pWaterSideWaveMaterial->load();

  // remove entites only used for creating static
  for (auto entpair : m_mStaticEntitiesMap) {
    m_pSceneNode->getCreator()->destroyEntity(entpair.second);
  }
  m_mStaticEntitiesMap.clear();
}

CMap::~CMap() {
}

void CMap::start() {
  sendCallToAll(&CEntity::start, false);
}

void CMap::exit() {
  if (!m_pSceneNode) {return;}
  m_SceneLoader.cleanup();
  Ogre::LogManager::getSingleton().logMessage("Destruction of map '" + m_MapPack->getName() + "'");
  
  for (int i = 0; i < TT_COUNT; i++) {
    m_pSceneNode->getCreator()->destroyEntity(m_apTileEntities[i]);
  }
  m_pSceneNode->getCreator()->destroyStaticGeometry(m_pStaticGeometry);
  m_pSceneNode->getCreator()->destroyStaticGeometry(m_pStaticGeometryChangedTiles);
  m_pSceneNode->getCreator()->destroyStaticGeometry(m_pStaticGeometryFixedTiles);
  m_pStaticGeometry = nullptr;

  CWorldEntity::exit();
}

void CMap::CreateCube(const btVector3 &Position, btScalar Mass)
{
    // empty ogre vectors for the cubes size and position
    Ogre::Vector3 size = Ogre::Vector3::ZERO;
    Ogre::Vector3 pos = Ogre::Vector3::ZERO;

    // Convert the bullet physics vector to the ogre vector
    pos.x = Position.getX();
    pos.y = Position.getY();
    pos.z = Position.getZ();


    // Create a cube by using a cube mesh (you can find one in the ogre samples)
    Ogre::Entity *entity =

  m_pSceneNode->getCreator()->createEntity(
        "Cylinder.mesh");

    // This gets us the size of the bounding box but since the bounding box in Ogre
    // is a little bigger than the object itself we will cut that down slightly to make the physics
    // more accurate.
    Ogre::AxisAlignedBox boundingB = entity->getBoundingBox();
    size = boundingB.getSize()*0.95f;

    // apply a material to the cube so it isn't gray like the ground
    entity->setMaterialName("water_side_wave");
    entity->setCastShadows(true);

    // Create a sceneNode and attach the entity for rendering
    Ogre::SceneNode *node = m_pSceneNode->createChildSceneNode();
    node->attachObject(entity);
    node->setPosition(pos);  // This gives it the initial position as provided

    // Physics
    btTransform Transform;
    Transform.setIdentity();
    Transform.setOrigin(Position);  // Set the position of the rigid body to match the sceneNode

    // Give it to the motion state
    BtOgre::RigidBodyState *MotionState = new BtOgre::RigidBodyState(node, Transform);

   // Being new myself I'm not sure why this happen but we give the rigid body half the size
   // of our cube and tell it to create a BoxShape (cube)
    btVector3 HalfExtents(size.x*0.5f,size.y*0.5f,size.z*0.5f);
    btCollisionShape *Shape = new btCylinderShape(HalfExtents);

    // Add Mass to the object so it is appropriately affected by other objects and gravity
    // In this case we pass the mass into the function when we call it.
    btVector3 LocalInertia;
    Shape->calculateLocalInertia(Mass, LocalInertia);

    // Create the rigid body object
    btRigidBody *RigidBody = new btRigidBody(Mass, MotionState, Shape, LocalInertia);

    // Add it to the physics world
    m_PhysicsManager.getWorld()->addRigidBody(RigidBody, 32, 1023);
}

void CMap::moveMap(const Ogre::Vector3 &offset) {
  m_bPauseUpdate = true;
  m_pSceneNode->translate(offset);
  // move static geometries
  translateStaticGeometry(m_pStaticGeometry, offset);
  translateStaticGeometry(m_pStaticGeometryChangedTiles, offset);
  translateStaticGeometry(m_pStaticGeometryFixedTiles, offset);
}

void CMap::addStaticEntity(const std::string &entity, const Ogre::Vector3 &vPosition, const Ogre::Quaternion &vRotation) {
  if (m_mStaticEntitiesMap.find(entity) == m_mStaticEntitiesMap.end()) {
    m_mStaticEntitiesMap[entity] = m_pSceneNode->getCreator()->createEntity(entity);
  }

  m_pStaticGeometry->addEntity(m_mStaticEntitiesMap.at(entity), vPosition, vRotation);
}

void CMap::translateStaticGeometry(Ogre::StaticGeometry *pSG, const Ogre::Vector3 &vVec) {
  Ogre::StaticGeometry::RegionIterator regionIt = pSG->getRegionIterator();
  while (regionIt.hasMoreElements() ) {
    Ogre::StaticGeometry::Region * reg=regionIt.getNext();
    Ogre::SceneNode* thisSceneNode = reg->getParentSceneNode();
    thisSceneNode->translate(vVec);
    thisSceneNode->_updateBounds();
    reg->_notifyMoved();
  }
  pSG->setOrigin(vVec);
}

void CMap::update(Ogre::Real tpf) {
  CWorldEntity::update(tpf);

  if (m_pFlowerAnimationState) {
    m_pFlowerAnimationState->addTime(tpf * 5);
  }
}

bool CMap::frameStarted(const Ogre::FrameEvent& evt) {
  if (m_bPauseUpdate) {return true;}
  m_PhysicsManager.update(evt.timeSinceLastFrame);
  processCollisionCheck();
  return CWorldEntity::frameStarted(evt);
}

bool CMap::frameEnded(const Ogre::FrameEvent& evt) {
  return CWorldEntity::frameEnded(evt);
}


void CMap::handleMessage(const CMessage &message) {
  if (message.getType() == MSG_ENTITY_STATE_CHANGED) {
    const CMessageEntityStateChanged &mesc(dynamic_cast<const CMessageEntityStateChanged&>(message));
    CObject *pObject(dynamic_cast<CObject*>(&mesc.getEntity()));
    if (!pObject) {return;}
    if (mesc.getOldState() == EST_NORMAL) {
      // only change request if object was in normal state before

      const SObjectTypeData &data(OBJECT_TYPE_ID_MAP.toData(static_cast<EObjectTypes>(pObject->getType())));
      if (data.eRemovedTile == TT_COUNT) {
        return;
      }

      m_pStaticGeometryFixedTiles->destroy();
      rebuildStaticGeometryChangedTiles();
      
      if (pObject->getType() == OBJECT_GREEN_BUSH) {
        m_pStaticGeometryFixedTiles->addEntity(m_apTileEntities[TT_GREEN_SOIL + std::rand() % (TT_GREEN_SOIL_GRASS_BR_TL_TR - TT_GREEN_SOIL + 1)], pObject->getSceneNode()->getInitialPosition());
        m_pStaticGeometryFixedTiles->addEntity(m_apTileEntities[TT_GREEN_BUSH_TRUNK], pObject->getSceneNode()->getInitialPosition());
      }
      else {
        m_pStaticGeometryFixedTiles->addEntity(m_apTileEntities[data.eRemovedTile], pObject->getSceneNode()->getInitialPosition());
      }

      m_pStaticGeometryFixedTiles->build();
    }
  }
}

void CMap::rebuildStaticGeometryChangedTiles() {
  m_pStaticGeometryChangedTiles->reset();

  for (CEntity *pChild : m_lChildren) {
    CObject *pObject(dynamic_cast<CObject*>(pChild));
    if (!pObject) {continue;}
    
    if (pObject->getState() == EST_NORMAL) {
      const SObjectTypeData &data(OBJECT_TYPE_ID_MAP.toData(static_cast<EObjectTypes>(pObject->getType())));
      if (data.eNormalTile != TT_COUNT) {
        m_pStaticGeometryChangedTiles->addEntity(m_apTileEntities[data.eNormalTile], pObject->getSceneNode()->getInitialPosition());
      }
    }
  }

  m_pStaticGeometryChangedTiles->build();
}

void CMap::processCollisionCheck() {
	int numManifolds = m_PhysicsManager.getWorld()->getDispatcher()->getNumManifolds();
	for (int i=0;i<numManifolds;i++)
	{
		btPersistentManifold* contactManifold =  m_PhysicsManager.getWorld()->getDispatcher()->getManifoldByIndexInternal(i);
		const btCollisionObject* obA = static_cast<const btCollisionObject*>(contactManifold->getBody0());
		const btCollisionObject* obB = static_cast<const btCollisionObject*>(contactManifold->getBody1());

		int numContacts = contactManifold->getNumContacts();
		for (int j=0;j<numContacts;j++)
		{
			btManifoldPoint& pt = contactManifold->getContactPoint(j);
			if (pt.getDistance()<0.f)
			{
				// Contact of 2 objects
        CWorldEntity *pWE_A(CWorldEntity::getFromUserPointer(obA));
        CWorldEntity *pWE_B(CWorldEntity::getFromUserPointer(obB));
        btVector3 vDistance(pt.m_positionWorldOnA - pt.m_positionWorldOnB);
        vDistance.normalize();
        if (pWE_A && pWE_B) {
          pWE_A->interactOnCollision(BtOgre::Convert::toOgre(vDistance), pWE_B);
          pWE_B->interactOnCollision(-BtOgre::Convert::toOgre(vDistance), pWE_A);
        }
			}
		}

	}
}

// ############################################################################3
// MapPackParserListener

void CMap::parsePlayer(const tinyxml2::XMLElement *pElem) {
  m_pPlayer->readEventsFromXMLElement(pElem, true);
}

void CMap::parseRegion(const tinyxml2::XMLElement *pElem) {
  new CRegion(this, pElem);
}

void CMap::parseSceneEntity(const tinyxml2::XMLElement *pElem) {
  CEntity *pEntity = getChild(Attribute(pElem, "id"));
  ASSERT(pEntity);
  if (pEntity) {
    pEntity->readEventsFromXMLElement(pElem, true);
  }
}

// ############################################################################3
// CDotSceneLoaderCallback
void CMap::physicsShapeCreated(btCollisionShape *pShape, const std::string &sMeshName) {
  EObjectTypes objectType(OBJECT_TYPE_ID_MAP.getFromMeshName(sMeshName));
  if (objectType != OBJECT_COUNT) {
    pShape->setLocalScaling(pShape->getLocalScaling() * OBJECT_TYPE_ID_MAP.toData(objectType).vPhysicsShapeScale);
  }
}

void CMap::worldPhysicsAdded(btRigidBody *pRigidBody) {
  ASSERT(pRigidBody);

  pRigidBody->setUserPointer(dynamic_cast<CWorldEntity*>(this));
}

void CMap::postEntityAdded(Ogre::Entity *pEntity, Ogre::SceneNode *pParent, btRigidBody *pRigidBody, const CUserData &userData) {
  if (pEntity->getName().find("flower") != Ogre::String::npos) {
    if (!m_pFirstFlowerEntity) {
      m_pFirstFlowerEntity = pEntity;
      m_pFlowerAnimationState = pEntity->getAnimationState("Action");
      m_pFlowerAnimationState->setLoop(true);
      m_pFlowerAnimationState->setEnabled(true);
    }
    else {
      pEntity->shareSkeletonInstanceWith(m_pFirstFlowerEntity);
    }
  }
}

void CMap::staticObjectAdded(Ogre::Entity *pEntity, Ogre::SceneNode *pParent) {
  m_pStaticGeometry->addEntity(pEntity, pParent->getPosition(), pParent->getOrientation());
  destroySceneNode(pParent, true);
}

CDotSceneLoaderCallback::EResults CMap::preEntityAdded(tinyxml2::XMLElement *XMLNode, Ogre::SceneNode *pParent, CUserData &userData) {
  CWorldEntity *pEntity(nullptr);
  
  EObjectTypes objectType(OBJECT_TYPE_ID_MAP.getFromMeshFileName(XMLNode->Attribute("meshFile")));
  if (objectType != OBJECT_COUNT && OBJECT_TYPE_ID_MAP.toData(objectType).bUserHandle) {
    pEntity = new CObject(userData.getStringUserData("name"), this, this, objectType, pParent);

    pEntity->setPosition(pParent->getPosition());
    pEntity->getSceneNode()->setInitialState();
    return R_CANCEL;
  }

  if (strcmp(XMLNode->Attribute("meshFile"), "flower.mesh") == 0) {
    userData.setUserData("static", false);
  }

  return R_CONTINUE;
}
