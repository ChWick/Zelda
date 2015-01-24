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
#include <BulletCollision/CollisionShapes/btCapsuleShape.h>
#include <BulletCollision/CollisionShapes/btCylinderShape.h>
#include <BulletCollision/CollisionShapes/btBoxShape.h>
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreEntity.h>
#include <OgreLogManager.h>
#include <OgreAnimationState.h>
#include <OgreMeshManager.h>
#include <string>
#include <ParticleUniverseSystemManager.h>
#include "../../Common/Physics/BtOgrePG.hpp"
#include "../../Common/Physics/PhysicsMasks.hpp"
#include "../../Common/Util/DeleteSceneNode.hpp"
#include "Region.hpp"
#include "Entrance.hpp"
#include "../GlobalCollisionShapesTypes.hpp"
#include "../Objects/Object.hpp"
#include "../Objects/Chest.hpp"
#include "BulletCollision/CollisionShapes/btSphereShape.h"
#include "../Character/Person.hpp"
#include "../../Common/Util/Assert.hpp"
#include "../../Common/Util/Sleep.hpp"
#include "../../Common/Message/MessageEntityStateChanged.hpp"
#include "../../Common/DotSceneLoader/UserData.hpp"
#include "../../Common/Util/XMLHelper.hpp"
#include "../../Common/GameLogic/Events/Event.hpp"

#include "../Character/CharacterCreator.hpp"


using XMLHelper::Attribute;
using XMLHelper::IntAttribute;
using XMLHelper::RealAttribute;

CMap::CMap(CEntity *pAtlas,
           CMapPackPtr mapPack,
           Ogre::SceneNode *pParentSceneNode,
           CWorldEntity *pPlayer)
    : CAbstractMap(pAtlas, mapPack, pParentSceneNode, pPlayer),
      m_pFirstFlowerEntity(nullptr),
      m_pFlowerAnimationState(nullptr) {

  // Create global collision shapes
  m_PhysicsManager.addCollisionShape(
      CGlobalCollisionShapesTypesIdMap::getSingleton().toString(
          GCST_PICKABLE_OBJECT_SPHERE),
      CPhysicsCollisionObject(new btSphereShape(0.04),
                              Ogre::Vector3::NEGATIVE_UNIT_Y * 0.03));
  m_PhysicsManager.addCollisionShape(
      CGlobalCollisionShapesTypesIdMap::getSingleton().toString(GCST_PERSON_CAPSULE),
      CPhysicsCollisionObject(
          new btCapsuleShape(CPerson::PERSON_RADIUS,
                             CPerson::PERSON_HEIGHT
                             - 2 * CPerson::PERSON_RADIUS)));

  // house entrance
  btBoxShape *pHouseEntranceTopShape
      = new btBoxShape(btVector3(0.07, 0.01, 0.02));
  m_PhysicsManager.addCollisionShape(
      CGlobalCollisionShapesTypesIdMap::getSingleton().toString(GCST_HOUSE_ENTRANCE_TOP),
      CPhysicsCollisionObject(pHouseEntranceTopShape));

  btBoxShape *pHouseEntranceSideShape
      = new btBoxShape(btVector3(0.01, 0.08, 0.02));
  m_PhysicsManager.addCollisionShape(
      CGlobalCollisionShapesTypesIdMap::getSingleton().toString(GCST_HOUSE_ENTRANCE_SIDE),
      CPhysicsCollisionObject(pHouseEntranceSideShape));

  btCompoundShape *pHouseEntranceShape = new btCompoundShape();
  pHouseEntranceShape->addChildShape(
      btTransform(btQuaternion::getIdentity(),
                  btVector3(-0.069, 0.08, 0.02)),
      pHouseEntranceSideShape);
  pHouseEntranceShape->addChildShape(
      btTransform(btQuaternion::getIdentity(),
                  btVector3(0.069, 0.08, 0.02)),
      pHouseEntranceSideShape);
  pHouseEntranceShape->addChildShape(
      btTransform(btQuaternion::getIdentity(),
                  btVector3(0.0, 0.16, 0.02)),
      pHouseEntranceTopShape);
  m_PhysicsManager.addCollisionShape(
      CGlobalCollisionShapesTypesIdMap::getSingleton().toString(GCST_HOUSE_ENTRANCE),
      CPhysicsCollisionObject(pHouseEntranceShape, Ogre::Vector3::ZERO));

  // stone pile
  btSphereShape *pPileSingleStoneShape = new btSphereShape(0.04);
  m_PhysicsManager.addCollisionShape(
      CGlobalCollisionShapesTypesIdMap::getSingleton().toString(GCST_STONE_PILE_SINGLE_STONE),
      CPhysicsCollisionObject(pPileSingleStoneShape));

  btCompoundShape *pStonePileShape = new btCompoundShape();
  pStonePileShape->addChildShape(
      btTransform(btQuaternion::getIdentity(), btVector3(0.05, 0.025, 0.05)),
      pPileSingleStoneShape);
  pStonePileShape->addChildShape(
      btTransform(btQuaternion::getIdentity(), btVector3(-0.05, 0.025, 0.05)),
      pPileSingleStoneShape);
  pStonePileShape->addChildShape(
      btTransform(btQuaternion::getIdentity(), btVector3(0.05, 0.025, -0.05)),
      pPileSingleStoneShape);
  pStonePileShape->addChildShape(
      btTransform(btQuaternion::getIdentity(), btVector3(-0.05, 0.025, -0.05)),
      pPileSingleStoneShape);
  pStonePileShape->addChildShape(
      btTransform(btQuaternion::getIdentity(), btVector3(-0.0, 0.06, -0.0)),
      pPileSingleStoneShape);
  m_PhysicsManager.addCollisionShape(
      CGlobalCollisionShapesTypesIdMap::getSingleton().toString(GCST_STONE_PILE),
      CPhysicsCollisionObject(pStonePileShape, Ogre::Vector3::ZERO));

  m_PhysicsManager.addCollisionShape(
      CGlobalCollisionShapesTypesIdMap::getSingleton().toString(GCST_TREE),
      CPhysicsCollisionObject(
          new btCylinderShape(btVector3(0.173, 0.2, 0.173)),
          Ogre::Vector3::NEGATIVE_UNIT_Y * 0.2));
  m_PhysicsManager.addCollisionShape(
      CGlobalCollisionShapesTypesIdMap::getSingleton().toString(GCST_FALLING_OBJECT_SPHERE),
      CPhysicsCollisionObject(new btSphereShape(0.02),
                              Ogre::Vector3::NEGATIVE_UNIT_Y * 0));
  m_PhysicsManager.addCollisionShape(
      CGlobalCollisionShapesTypesIdMap::getSingleton().toString(GCST_SMALL_CHEST_TOP),
      CPhysicsCollisionObject(
          new btCylinderShapeX(btVector3(0.04, 0.025, 0.025)),
          Ogre::Vector3(0, 0.020  , 0.025)));


  // Create global entites
  for (int i = 0; i < TT_COUNT; i++) {
    m_apTileEntities[i] = pParentSceneNode->getCreator()->createEntity(
        CTileDataMap::getSingleton().toData(static_cast<ETileTypes>(i)).sMeshName);
  }

  m_pSceneNode = pParentSceneNode->createChildSceneNode(
      m_MapPack->getName() + "_RootNode");

  m_pStaticGeometryChangedTiles
      = pParentSceneNode->getCreator()->createStaticGeometry(
          m_MapPack->getName() + "_StaticGeometryChangedTiles");
  m_pStaticGeometryChangedTiles->setRegionDimensions(Ogre::Vector3(10, 10, 10));
  m_pStaticGeometryChangedTiles->setCastShadows(false);

  m_pStaticGeometryFixedTiles
      = pParentSceneNode->getCreator()->createStaticGeometry(
          m_MapPack->getName() + "_StaticGeometryFixedTiles");
  m_pStaticGeometryFixedTiles->setRegionDimensions(Ogre::Vector3(10, 10, 10));
  m_pStaticGeometryFixedTiles->setCastShadows(false);


  m_MapPack->init(this);

  m_SceneLoader.addCallback(this);

  CMapPackPtr pack(std::dynamic_pointer_cast<CMapPack>(m_MapPack));

  m_SceneLoader.parseDotScene(pack->getSceneFile(),
                              m_MapPack->getResourceGroup(),
                              m_pSceneNode->getCreator(),
                              &m_PhysicsManager,
                              m_pSceneNode,
                              mPrependNodeName);

  m_MapPack->parse();


  // CreateCube(btVector3(0, 10, 0.2), 1);
  // CreateCube(btVector3(0, 200, 0.3), 100);

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

  rebuildStaticGeometryChangedTiles();

  m_pWaterSideWaveMaterial
      = Ogre::MaterialManager::getSingleton().getByName("water_side_wave");
  m_pWaterSideWaveMaterial->touch();
  m_pWaterSideWaveMaterial->load();
}

CMap::~CMap() {
}

void CMap::init() {
  CAbstractMap::init();
}

void CMap::start() {
  CAbstractMap::start();
  sendCallToAll(&CEntity::start, false);
}

void CMap::exit() {
  if (!m_pSceneNode) {return;}
  m_SceneLoader.cleanup();

  Ogre::SceneManager *sceneManager = m_pSceneNode->getCreator();

  for (int i = 0; i < TT_COUNT; i++) {
    sceneManager->destroyEntity(m_apTileEntities[i]);
  }
  sceneManager->destroyStaticGeometry(m_pStaticGeometryChangedTiles);
  sceneManager->destroyStaticGeometry(m_pStaticGeometryFixedTiles);

  // reset shared pointers, to make sure it is deleted in correct order
  m_pWaterSideWaveMaterial.setNull();

  CAbstractMap::exit();
}

void CMap::CreateCube(const btVector3 &Position, btScalar Mass) {
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

    // This gets us the size of the bounding box but since
    // the bounding box in Ogre is a little bigger than the
    // object itself we will cut that down slightly to make
    // the physics more accurate.
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
    // Set the position of the rigid body to match the sceneNode
    Transform.setOrigin(Position);

    // Give it to the motion state
    BtOgre::RigidBodyState *MotionState
        = new BtOgre::RigidBodyState(node, Transform);

    // Being new myself I'm not sure why this happen but we give
    // the rigid body half the size
    // of our cube and tell it to create a BoxShape (cube)
    btVector3 HalfExtents(size.x * 0.5f, size.y * 0.5f, size.z * 0.5f);
    btCollisionShape *Shape = new btCylinderShape(HalfExtents);

    // Add Mass to the object so it is appropriately affected by
    // other objects and gravity
    // In this case we pass the mass into the function when we call it.
    btVector3 LocalInertia;
    Shape->calculateLocalInertia(Mass, LocalInertia);

    // Create the rigid body object
    btRigidBody *RigidBodyState
        = new btRigidBody(Mass, MotionState, Shape, LocalInertia);

    // Add it to the physics world
    m_PhysicsManager.getWorld()->addRigidBody(RigidBodyState, 32, 1023);
}

void CMap::moveMap(const Ogre::Vector3 &offset) {
  m_bPauseUpdate = true;
  m_pSceneNode->translate(offset);
  // move static geometries
  translateStaticGeometry(m_pStaticGeometry, offset);
  translateStaticGeometry(m_pStaticGeometryChangedTiles, offset);
  translateStaticGeometry(m_pStaticGeometryFixedTiles, offset);
}

void CMap::translateStaticGeometry(Ogre::StaticGeometry *pSG,
                                   const Ogre::Vector3 &vVec) {
  Ogre::StaticGeometry::RegionIterator regionIt = pSG->getRegionIterator();
  while (regionIt.hasMoreElements()) {
    Ogre::StaticGeometry::Region *reg = regionIt.getNext();
    Ogre::SceneNode* thisSceneNode = reg->getParentSceneNode();
    thisSceneNode->translate(vVec);
    thisSceneNode->_updateBounds();
    reg->_notifyMoved();
  }
  pSG->setOrigin(vVec);
}

void CMap::update(Ogre::Real tpf) {
  CAbstractMap::update(tpf);

  if (m_pFlowerAnimationState) {
    m_pFlowerAnimationState->addTime(tpf * 5);
  }
}

bool CMap::frameStarted(const Ogre::FrameEvent& evt) {
  return CAbstractMap::frameStarted(evt);
}

bool CMap::frameEnded(const Ogre::FrameEvent& evt) {
  return CAbstractMap::frameEnded(evt);
}


void CMap::handleMessage(const CMessagePtr message) {
  if (message->getType() == MSG_ENTITY_STATE_CHANGED) {
    auto mesc(
        std::dynamic_pointer_cast<const CMessageEntityStateChanged>(message));
    CObject *pObject(dynamic_cast<CObject*>(mesc->getEntity()));
    if (!pObject) {return;}
    if (mesc->getOldState() == EST_NORMAL) {
      // only change request if object was in normal state before

      const CObjectConstructionInfo &data(CObjectDataMap::getSingleton().toData(
          static_cast<EObjectTypes>(pObject->getType())));
      if (data.getRemovedTile() == TT_COUNT) {
        return;
      }

      m_pStaticGeometryFixedTiles->destroy();
      rebuildStaticGeometryChangedTiles();


      m_pStaticGeometryFixedTiles->addEntity(
          m_apTileEntities[data.getRemovedTile()],
          pObject->getSceneNode()->getInitialPosition());

      m_pStaticGeometryFixedTiles->build();
    }
  }
}

void CMap::updatePause(int iPauseType, bool bPause) {
  if (iPauseType & PAUSE_MAP_UPDATE) {
    m_bPauseUpdate = bPause;
  }
  if (iPauseType & PAUSE_MAP_RENDER) {
    m_bPauseRender = bPause;
    m_pStaticGeometry->setVisible(!bPause);
    m_pStaticGeometryFixedTiles->setVisible(!bPause);
    m_pStaticGeometryChangedTiles->setVisible(!bPause);
    m_pSceneNode->setVisible(!bPause);
  }
}

void CMap::rebuildStaticGeometryChangedTiles() {
  m_pStaticGeometryChangedTiles->reset();

  for (CEntity *pChild : getChildren()) {
    CObject *pObject(dynamic_cast<CObject*>(pChild));
    if (!pObject) {continue;}

    if (pObject->getState() == EST_NORMAL) {
      const CObjectConstructionInfo &data(CObjectDataMap::getSingleton().toData(
          static_cast<EObjectTypes>(pObject->getType())));
      if (data.getNormalTile() != TT_COUNT) {
        m_pStaticGeometryChangedTiles->addEntity(
            m_apTileEntities[data.getNormalTile()],
            pObject->getSceneNode()->getInitialPosition());
      }
    }
  }

  m_pStaticGeometryChangedTiles->build();
}

// ############################################################################3
// MapPackParserListener

void CMap::parseEvent(const tinyxml2::XMLElement *pElem) {
  addEvent(new events::CEvent(this, pElem));
}

void CMap::parsePlayer(const tinyxml2::XMLElement *pElem) {
  m_pPlayer->readEventsFromXMLElement(pElem, true);
}

void CMap::parseRegion(const tinyxml2::XMLElement *pElem) {
  new CRegion(this, pElem);
}

void CMap::parseEntrance(const tinyxml2::XMLElement *pElem) {
  new CEntrance(this, pElem);
}

void CMap::parseSceneEntity(const tinyxml2::XMLElement *pElem) {
  CEntity *pEntity = getChild(Attribute(pElem, "id"));
  ASSERT(pEntity);
  if (pEntity) {
    pEntity->readEventsFromXMLElement(pElem, true);
  }
}

void CMap::parseNewEntity(const tinyxml2::XMLElement *pElem) {
  CWorldEntity *pNewEnt = CCharacterCreator::createCharacter(
      pElem, this, this, m_pPlayer);
  if (pNewEnt) {return;}

  throw Ogre::Exception(0, "New entity could not be created.", __FILE__);
}

// ############################################################################
// CDotSceneLoaderCallback
void CMap::physicsShapeCreated(btCollisionShape *pShape,
                               const std::string &sMeshName) {
  EObjectTypes objectType(CObjectDataMap::getSingleton().getFromMeshName(sMeshName));
  if (objectType != OBJECT_COUNT) {
    auto scale = CObjectDataMap::getSingleton().toData(objectType).getPhysicsShapeScale();
    pShape->setLocalScaling(pShape->getLocalScaling() * scale);
  }
}

void CMap::worldPhysicsAdded(btRigidBody *pRigidBody) {
  ASSERT(pRigidBody);

  // if user pointer is already set, then collision/interaction
  // is handled in a special way, e.g. chest
  if (pRigidBody->getUserPointer() == nullptr) {
    setThisAsCollisionObjectsUserPointer(pRigidBody);
  }
}

void CMap::postEntityAdded(Ogre::Entity *pEntity,
                           Ogre::SceneNode *pParent,
                           btRigidBody *pRigidBody,
                           const CUserData &userData) {
  if (pEntity->getName().find("flower") != Ogre::String::npos) {
    if (!m_pFirstFlowerEntity) {
      m_pFirstFlowerEntity = pEntity;
      m_pFlowerAnimationState = pEntity->getAnimationState("Action");
      m_pFlowerAnimationState->setLoop(true);
      m_pFlowerAnimationState->setEnabled(true);
    } else {
      pEntity->shareSkeletonInstanceWith(m_pFirstFlowerEntity);
    }
  } else if (pEntity->getMesh()->getName() == "small_chest_bottom.mesh") {
    CChest *pChest = new CChest(userData.getStringUserData("name"),
                                this, this, CChest::SMALL_CHEST);
    pChest->setPosition(pParent->_getDerivedPosition());
    pChest->setThisAsCollisionObjectsUserPointer(pRigidBody);
    pChest->setInnerObject(CObjectTypeIdMap::getSingleton().parseString(
        userData.getStringUserData("inner_object")));
    pChest->init();
    pChest->start();
    m_PhysicsManager.getWorld()->removeRigidBody(pRigidBody);
    m_PhysicsManager.getWorld()->addRigidBody(
        pRigidBody,
        COL_INTERACTIVE,
        MASK_INTERACIVE_OBJECT_COLLIDES_WITH);
  }
}

void CMap::staticObjectAdded(Ogre::Entity *pEntity,
                             Ogre::SceneNode *pParent) {
  m_pStaticGeometry->addEntity(pEntity, pParent->getPosition(),
                               pParent->getOrientation());
  destroySceneNode(pParent, true);
}

CDotSceneLoaderCallback::EResults CMap::preEntityAdded(
    tinyxml2::XMLElement *XMLNode,
    Ogre::SceneNode *pParent,
    CUserData &userData) {
  CWorldEntity *pEntity(nullptr);

  EObjectTypes objectType(CObjectDataMap::getSingleton().getFromMeshFileName(
      XMLNode->Attribute("meshFile")));
  if (objectType != OBJECT_COUNT
      && CObjectDataMap::getSingleton().toData(objectType).isUsedHandled()) {
    CObject *pObject = new CObject(userData.getStringUserData("name"),
                                   this, this, objectType, pParent);
    std::string innerObject(userData.getStringUserData("inner_object"));
    if (!innerObject.empty()) {
      pObject->setInnerObject(CObjectTypeIdMap::getSingleton().parseString(innerObject));
    }
    pEntity = pObject;
    pEntity->init();

    pEntity->setPosition(pParent->getPosition());
    pEntity->getSceneNode()->setInitialState();
    return R_CANCEL;
  }

  if (strcmp(XMLNode->Attribute("meshFile"), "flower.mesh") == 0) {
    userData.setUserData("static", false);
  }

  return R_CONTINUE;
}
