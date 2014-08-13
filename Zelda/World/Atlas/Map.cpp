#include "Map.hpp"
#include "../../Common/Physics/BtOgrePG.h"
#include "../../Common/Util/DeleteSceneNode.hpp"
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreEntity.h>
#include <OgreLogManager.h>
#include <OgreMeshManager.h>
#include "Region.hpp"

CMap::CMap(CEntity *pAtlas, CMapPackPtr mapPack, Ogre::SceneNode *pParentSceneNode, CWorldEntity *pPlayer)
  : CWorldEntity(mapPack->getName(), pAtlas, this),
    m_PhysicsManager(pParentSceneNode->getCreator()),
    m_MapPack(mapPack),
    m_pPlayer(pPlayer) {

  Ogre::LogManager::getSingleton().logMessage("Construction of map '" + m_MapPack->getName() + "'");

  m_pSceneNode = pParentSceneNode->createChildSceneNode(m_MapPack->getName() + "_RootNode");

  m_MapPack->init(this);


  m_SceneLoader.parseDotScene(m_MapPack->getSceneFile(),
                              m_MapPack->getResourceGroup(),
                              m_pSceneNode->getCreator(),
                              &m_PhysicsManager,
                              m_pSceneNode,
                              m_MapPack->getName());


  CreateCube(btVector3(0, 10, 0.2), 1);
  CreateCube(btVector3(0, 200, 0.3), 100);
}

CMap::~CMap() {
  Ogre::LogManager::getSingleton().logMessage("Destruction of map '" + m_MapPack->getName() + "'");
  destroySceneNode(m_pSceneNode, true);
}

void CMap::start() {
  sendCallToAll(&CEntity::start, false);
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
    entity->setMaterialName("Examples/Rockwall");
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

void CMap::moveMapAndDeletePhysics(const Ogre::Vector3 &offset) {
  m_bPauseUpdate = true;
  m_PhysicsManager.exit();
  m_pSceneNode->translate(offset);
}

void CMap::update(Ogre::Real tpf) {
  CWorldEntity::update(tpf);
}

bool CMap::frameStarted(const Ogre::FrameEvent& evt) {
  if (m_bPauseUpdate) {return true;}
  m_PhysicsManager.update(evt.timeSinceLastFrame);
  return CWorldEntity::frameStarted(evt);
}

bool CMap::frameEnded(const Ogre::FrameEvent& evt) {
  return CWorldEntity::frameEnded(evt);
}

// ############################################################################3
// MapPackParserListener

void CMap::parsePlayer(const tinyxml2::XMLElement *pElem) {
  m_pPlayer->readEventsFromXMLElement(pElem, true);
}

void CMap::parseRegion(const SRegionInfo &region) {
  new CRegion(this, region);
}
