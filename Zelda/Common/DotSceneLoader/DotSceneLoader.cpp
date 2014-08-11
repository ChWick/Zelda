#include "DotSceneLoader.hpp"
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreMeshManager.h>
#include <OgreEntity.h>
#include <btBulletDynamicsCommon.h>
#include "../Physics/BtOgreGP.h"
#include "../Physics/BtOgrePG.h"
#include "UserData.hpp"
#include "../Physics/PhysicsManager.h"
#include "../Physics/PhysicsMasks.hpp"
#include <boost/regex.hpp>

using namespace std;
using namespace Ogre;
using namespace tinyxml2;

void DotSceneLoader::cleanup() {
	staticObjects.clear();
    dynamicObjects.clear();
	mStaticObjects.clear();
	if (mSceneMgr) {
		for (auto &entPair : m_lEntityBufferMap) {
			mSceneMgr->destroyEntity(entPair.second);
		}
		mSceneMgr = NULL;
	}
	m_lEntityBufferMap.clear();
}
void DotSceneLoader::parseDotScene(const String &SceneName, const String &groupName, SceneManager *yourSceneMgr, CPhysicsManager *pPhysicsManager, SceneNode *pAttachNode, const String &sPrependNode)
{
	cleanup();

    Ogre::LogManager::getSingleton().logMessage("Loading dot scene: " + SceneName);
    // set up shared object values
    m_sGroupName = groupName;
    mSceneMgr = yourSceneMgr;
	m_pPhysicsManager = pPhysicsManager;
    m_sPrependNode = sPrependNode;


	mStaticSceneNode = pAttachNode->createChildSceneNode(sPrependNode + "StaticSceneNode");

    XMLDocument   *XMLDoc = 0;
    XMLElement   *XMLRoot;

    try
    {
        // Strip the path
        Ogre::String basename, path;
        Ogre::StringUtil::splitFilename(SceneName, basename, path);

        DataStreamPtr pStream = ResourceGroupManager::getSingleton().
            openResource( basename, groupName );

        //DataStreamPtr pStream = ResourceGroupManager::getSingleton().
        //    openResource( SceneName, groupName );

        String data = pStream->getAsString();
        // Open the .scene File
        XMLDoc = new XMLDocument();
        XMLDoc->Parse( data.c_str() );
        pStream->close();
        pStream.setNull();

        if( XMLDoc->Error() )
        {
            //We'll just log, and continue on gracefully
            LogManager::getSingleton().logMessage("[DotSceneLoader] The TiXmlDocument reported an error");
            delete XMLDoc;
            return;
        }
    }
    catch(...)
    {
        //We'll just log, and continue on gracefully
        LogManager::getSingleton().logMessage("[DotSceneLoader] Error creating TiXmlDocument");
        delete XMLDoc;
        return;
    }

    // Validate the File
    XMLRoot = XMLDoc->RootElement();
    if( String( XMLRoot->Value()) != "scene"  ) {
        LogManager::getSingleton().logMessage( "[DotSceneLoader] Error: Invalid .scene File. Missing <scene>" );
        delete XMLDoc;
        return;
    }

    // figure out where to attach any nodes we create
    mAttachNode = pAttachNode;
    if(!mAttachNode)
        mAttachNode = mSceneMgr->getRootSceneNode();

    // Process the scene
    processScene(XMLRoot);

	/*for (auto &m : m_mStaticGeometryMap) {
		m.second->build();
	}*/

	/*for (auto &s : mStaticObjects) {
		for (auto &cb : m_lCallbacks){
			cb->staticObjectAdded(s);
		}
	}
	// remove static geometry entities
	destroySceneNode(mStaticSceneNode);*/


	// apply manually material data
	// ===============================

	// dont cull grass
	auto mat = static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton().getByName("CS_Grass"));
	if (!mat.isNull() && mat->getBestTechnique()) {
		mat->getBestTechnique()->setCullingMode(Ogre::CullingMode::CULL_NONE);
	}

	// make water transparent, add textures
	mat = static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton().getByName("Water"));
	if (!mat.isNull() && mat->getBestTechnique()) {
		mat->getBestTechnique()->setCullingMode(Ogre::CullingMode::CULL_NONE);
		mat->getBestTechnique()->getPass(0)->setDepthCheckEnabled(true);
		mat->getBestTechnique()->getPass(0)->setDepthWriteEnabled(false);
		mat->getBestTechnique()->getPass(0)->setSceneBlending(Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_ONE);
		mat->getBestTechnique()->getPass(0)->getTextureUnitState(0)->setTransformAnimation(
			Ogre::TextureUnitState::TT_TRANSLATE_U, Ogre::WFT_SINE, 0, 0.25, 0, 0.125);
		mat->getBestTechnique()->getPass(0)->getTextureUnitState(0)->setColourOperation(Ogre::LBO_MODULATE);
		mat->getBestTechnique()->getPass(0)->getTextureUnitState(1)->setTransformAnimation(
			Ogre::TextureUnitState::TT_TRANSLATE_V, Ogre::WFT_SINE, 0.1, 0.25, 0.1, 0.125);
		mat->getBestTechnique()->getPass(0)->getTextureUnitState(1)->setTextureRotate(Ogre::Degree(90));
		mat->getBestTechnique()->getPass(0)->getTextureUnitState(1)->setColourOperation(Ogre::LBO_MODULATE);
		mat->getBestTechnique()->getPass(0)->setDiffuse(1, 1, 1, 0.8);
	}

	/*for (auto &m : m_lEntityBufferMap) {
		mSceneMgr->destroyEntity(m.second);
	}*/

    // Close the XML File
    delete XMLDoc;

	// remove callbacks
	m_lCallbacks.clear();
}

void DotSceneLoader::processScene(XMLElement *XMLRoot)
{
    // Process the scene parameters
    String version = getAttrib(XMLRoot, "formatVersion", "unknown");

    String message = "[DotSceneLoader] Parsing dotScene file with version " + version;
    if(XMLRoot->Attribute("ID"))
        message += ", id " + String(XMLRoot->Attribute("ID"));
    if(XMLRoot->Attribute("sceneManager"))
        message += ", scene manager " + String(XMLRoot->Attribute("sceneManager"));
    if(XMLRoot->Attribute("minOgreVersion"))
        message += ", min. Ogre version " + String(XMLRoot->Attribute("minOgreVersion"));
    if(XMLRoot->Attribute("author"))
        message += ", author " + String(XMLRoot->Attribute("author"));

    LogManager::getSingleton().logMessage(message);

    XMLElement *pElement;

    // Process nodes (?)
    pElement = XMLRoot->FirstChildElement("nodes");
    if(pElement)
        processNodes(pElement);

    // Process externals (?)
    pElement = XMLRoot->FirstChildElement("externals");
    if(pElement)
        processExternals(pElement);

    // Process environment (?)
    pElement = XMLRoot->FirstChildElement("environment");
    if(pElement)
        processEnvironment(pElement);

    // Process terrain (?)
    pElement = XMLRoot->FirstChildElement("terrain");
    if(pElement)
        processTerrain(pElement);

    // Process userDataReference (?)
    pElement = XMLRoot->FirstChildElement("userDataReference");
    if(pElement)
        processUserDataReference(pElement);

    // Process octree (?)
    pElement = XMLRoot->FirstChildElement("octree");
    if(pElement)
        processOctree(pElement);

    // Process light (?)
    pElement = XMLRoot->FirstChildElement("light");
    if(pElement)
        processLight(pElement);

    // Process camera (?)
    pElement = XMLRoot->FirstChildElement("camera");
    if(pElement)
        processCamera(pElement);
}

void DotSceneLoader::processNodes(XMLElement *XMLNode)
{
    XMLElement *pElement;

    // Process node (*)
    pElement = XMLNode->FirstChildElement("node");
    while(pElement)
    {
        processNode(pElement);
        pElement = pElement->NextSiblingElement("node");
    }

    // Process position (?)
    pElement = XMLNode->FirstChildElement("position");
    if(pElement)
    {
        mAttachNode->setPosition(parseVector3(pElement));
        mAttachNode->setInitialState();
    }

    // Process rotation (?)
    pElement = XMLNode->FirstChildElement("rotation");
    if(pElement)
    {
        mAttachNode->setOrientation(parseQuaternion(pElement));
        mAttachNode->setInitialState();
    }

    // Process scale (?)
    pElement = XMLNode->FirstChildElement("scale");
    if(pElement)
    {
        mAttachNode->setScale(parseVector3(pElement));
        mAttachNode->setInitialState();
    }
}

void DotSceneLoader::processExternals(XMLElement *XMLNode)
{
    //! @todo Implement this
}

void DotSceneLoader::processEnvironment(XMLElement *XMLNode)
{
    XMLElement *pElement;

    // Process fog (?)
    pElement = XMLNode->FirstChildElement("fog");
    if(pElement)
        processFog(pElement);

    // Process skyBox (?)
    pElement = XMLNode->FirstChildElement("skyBox");
    if(pElement)
        processSkyBox(pElement);

    // Process skyDome (?)
    pElement = XMLNode->FirstChildElement("skyDome");
    if(pElement)
        processSkyDome(pElement);

    // Process skyPlane (?)
    pElement = XMLNode->FirstChildElement("skyPlane");
    if(pElement)
        processSkyPlane(pElement);

    // Process clipping (?)
    pElement = XMLNode->FirstChildElement("clipping");
    if(pElement)
        processClipping(pElement);

    // Process colourAmbient (?)
    pElement = XMLNode->FirstChildElement("colourAmbient");
    if(pElement)
        mSceneMgr->setAmbientLight(parseColour(pElement));

    // Process colourBackground (?)
    //! @todo Set the background colour of all viewports (RenderWindow has to be provided then)
    pElement = XMLNode->FirstChildElement("colourBackground");
    if(pElement)
        ;//mSceneMgr->set(parseColour(pElement));

    // Process userDataReference (?)
    pElement = XMLNode->FirstChildElement("userDataReference");
    if(pElement)
        processUserDataReference(pElement);
}

void DotSceneLoader::processTerrain(XMLElement *XMLNode)
{
    //! @todo Implement this
}

void DotSceneLoader::processUserDataReference(XMLElement *XMLNode, SceneNode *pParent)
{
    //! @todo Implement this
}

void DotSceneLoader::processOctree(XMLElement *XMLNode)
{
    //! @todo Implement this
}

void DotSceneLoader::processLight(XMLElement *XMLNode, SceneNode *pParent)
{
    // Process attributes
    String name = getAttrib(XMLNode, "name");
    String id = getAttrib(XMLNode, "id");

    // Create the light
    Light *pLight = mSceneMgr->createLight(m_sPrependNode + name);
    if(pParent)
        pParent->attachObject(pLight);
	else
		mAttachNode->attachObject(pLight);

    String sValue = getAttrib(XMLNode, "type");
    if(sValue == "point")
        pLight->setType(Light::LT_POINT);
    else if(sValue == "directional")
        pLight->setType(Light::LT_DIRECTIONAL);
    else if(sValue == "spot")
        pLight->setType(Light::LT_SPOTLIGHT);
    else if(sValue == "radPoint")
        pLight->setType(Light::LT_POINT);

    pLight->setVisible(getAttribBool(XMLNode, "visible", true));
    pLight->setCastShadows(getAttribBool(XMLNode, "castShadows", true));

    XMLElement *pElement;

    // Process position (?)
    pElement = XMLNode->FirstChildElement("position");
    if(pElement)
        pLight->setPosition(parseVector3(pElement));

    // Process normal (?)
    pElement = XMLNode->FirstChildElement("normal");
    if(pElement)
        pLight->setDirection(parseVector3(pElement));

    // Process direction (?)
    pElement = XMLNode->FirstChildElement("direction");
    if(pElement)
		pLight->setDirection(parseVector3(pElement).normalisedCopy());

    // Process colourDiffuse (?)
    pElement = XMLNode->FirstChildElement("colourDiffuse");
    if(pElement)
        pLight->setDiffuseColour(parseColour(pElement));

    // Process colourSpecular (?)
    pElement = XMLNode->FirstChildElement("colourSpecular");
    if(pElement)
        pLight->setSpecularColour(parseColour(pElement));

    // Process lightRange (?)
    pElement = XMLNode->FirstChildElement("lightRange");
    if(pElement)
        processLightRange(pElement, pLight);

    // Process lightAttenuation (?)
    pElement = XMLNode->FirstChildElement("lightAttenuation");
    if(pElement)
        processLightAttenuation(pElement, pLight);

    // Process userDataReference (?)
    pElement = XMLNode->FirstChildElement("userDataReference");
    if(pElement)
        ;//processUserDataReference(pElement, pLight);

	for (auto pCB : m_lCallbacks) {
		pCB->lightAdded(pLight);
	}
}

void DotSceneLoader::processCamera(XMLElement *XMLNode, SceneNode *pParent)
{
    // Process attributes
    String name = getAttrib(XMLNode, "name");
    String id = getAttrib(XMLNode, "id");
    Real fov = getAttribReal(XMLNode, "fov", 45);
    Real aspectRatio = getAttribReal(XMLNode, "aspectRatio", 1.3333);
    String projectionType = getAttrib(XMLNode, "projectionType", "perspective");

    // Create the camera
    Camera *pCamera = mSceneMgr->createCamera(name);
    if(pParent)
        pParent->attachObject(pCamera);

    // Set the field-of-view
    //! @todo Is this always in degrees?
    pCamera->setFOVy(Ogre::Degree(fov));

    // Set the aspect ratio
    pCamera->setAspectRatio(aspectRatio);

    // Set the projection type
    if(projectionType == "perspective")
        pCamera->setProjectionType(PT_PERSPECTIVE);
    else if(projectionType == "orthographic")
        pCamera->setProjectionType(PT_ORTHOGRAPHIC);

    XMLElement *pElement;

    // Process clipping (?)
    pElement = XMLNode->FirstChildElement("clipping");
    if(pElement)
    {
        Real nearDist = getAttribReal(pElement, "near");
        pCamera->setNearClipDistance(nearDist);

        Real farDist =  getAttribReal(pElement, "far");
        pCamera->setFarClipDistance(farDist);
    }

    // Process position (?)
    pElement = XMLNode->FirstChildElement("position");
    if(pElement)
        pCamera->setPosition(parseVector3(pElement));

    // Process rotation (?)
    pElement = XMLNode->FirstChildElement("rotation");
    if(pElement)
        pCamera->setOrientation(parseQuaternion(pElement));

    // Process normal (?)
    pElement = XMLNode->FirstChildElement("normal");
    if(pElement)
        ;//!< @todo What to do with this element?

    // Process lookTarget (?)
    pElement = XMLNode->FirstChildElement("lookTarget");
    if(pElement)
        ;//!< @todo Implement the camera look target

    // Process trackTarget (?)
    pElement = XMLNode->FirstChildElement("trackTarget");
    if(pElement)
        ;//!< @todo Implement the camera track target

    // Process userDataReference (?)
    pElement = XMLNode->FirstChildElement("userDataReference");
    if(pElement)
        ;//!< @todo Implement the camera user data reference
}

void DotSceneLoader::processNode(XMLElement *XMLNode, SceneNode *pParent)
{
    // Construct the node's name
    String name = m_sPrependNode + getAttrib(XMLNode, "name");
	boost::regex r("^ST_([A-Za-z]+)_([A-Za-z]+)\\.(\\d{3})$");
	boost::cmatch matches;
	/*Ogre::StaticGeometry *pSG(NULL);
	if (boost::regex_match(name.c_str(), matches, r)) {
		if (m_mStaticGeometryMap.find(matches[1].str()) == m_mStaticGeometryMap.end()) {
			pSG = mSceneMgr->createStaticGeometry("Static_ " + matches[1].str());
			m_mStaticGeometryMap[matches[1].str()] = pSG;
		}
		else {
			pSG = m_mStaticGeometryMap[matches[1].str()];
		}
	}*/

    // Create the scene node
    SceneNode *pNode;
    if(name.empty())
    {
        // Let Ogre choose the name
        if(pParent)
            pNode = pParent->createChildSceneNode();
        else
            pNode = mAttachNode->createChildSceneNode();
    }
    else
    {
        // Provide the name
        if(pParent)
            pNode = pParent->createChildSceneNode(name);
        else
            pNode = mAttachNode->createChildSceneNode(name);
    }

    // Process other attributes
    String id = getAttrib(XMLNode, "id");
    bool isTarget = getAttribBool(XMLNode, "isTarget");
    CUserData userData(XMLNode);

    XMLElement *pElement;

    // Process position (?)
    pElement = XMLNode->FirstChildElement("position");
    if(pElement)
    {
        pNode->setPosition(parseVector3(pElement));
        pNode->setInitialState();
    }

    // Process rotation (?)
    pElement = XMLNode->FirstChildElement("rotation");
    if(pElement)
    {
        pNode->setOrientation(parseQuaternion(pElement));
        pNode->setInitialState();
    }

    // Process scale (?)
    pElement = XMLNode->FirstChildElement("scale");
    if(pElement)
    {
        pNode->setScale(parseVector3(pElement));
        pNode->setInitialState();
    }

    // Process lookTarget (?)
    pElement = XMLNode->FirstChildElement("lookTarget");
    if(pElement)
        processLookTarget(pElement, pNode);

    // Process trackTarget (?)
    pElement = XMLNode->FirstChildElement("trackTarget");
    if(pElement)
        processTrackTarget(pElement, pNode);

    // Process node (*)
    pElement = XMLNode->FirstChildElement("node");
    while(pElement)
    {
        processNode(pElement, pNode);
        pElement = pElement->NextSiblingElement("node");
    }

    // Process entity (*)
    pElement = XMLNode->FirstChildElement("entity");
    while(pElement)
    {
        processEntity(pElement, pNode, userData);
        pElement = pElement->NextSiblingElement("entity");
    }

    // Process light (*)
    pElement = XMLNode->FirstChildElement("light");
    while(pElement)
    {
        processLight(pElement, pNode);
        pElement = pElement->NextSiblingElement("light");
    }

    // Process camera (*)
    pElement = XMLNode->FirstChildElement("camera");
    while(pElement)
    {
        processCamera(pElement, pNode);
        pElement = pElement->NextSiblingElement("camera");
    }

    // Process particleSystem (*)
    pElement = XMLNode->FirstChildElement("particleSystem");
    while(pElement)
    {
        processParticleSystem(pElement, pNode);
        pElement = pElement->NextSiblingElement("particleSystem");
    }

    // Process billboardSet (*)
    pElement = XMLNode->FirstChildElement("billboardSet");
    while(pElement)
    {
        processBillboardSet(pElement, pNode);
        pElement = pElement->NextSiblingElement("billboardSet");
    }

    // Process plane (*)
    pElement = XMLNode->FirstChildElement("plane");
    while(pElement)
    {
        processPlane(pElement, pNode);
        pElement = pElement->NextSiblingElement("plane");
    }

    // Process userDataReference (?)
    pElement = XMLNode->FirstChildElement("userDataReference");
    if(pElement)
        processUserDataReference(pElement, pNode);


	/*if (pSG) {
		pSG->addSceneNode(pNode);
		pNode->getParent()->removeChild(pNode);
		mStaticSceneNode->addChild(pNode);
	}*/
}

void DotSceneLoader::processLookTarget(XMLElement *XMLNode, SceneNode *pParent)
{
    //! @todo Is this correct? Cause I don't have a clue actually

    // Process attributes
    String nodeName = getAttrib(XMLNode, "nodeName");

    Node::TransformSpace relativeTo = Node::TS_PARENT;
    String sValue = getAttrib(XMLNode, "relativeTo");
    if(sValue == "local")
        relativeTo = Node::TS_LOCAL;
    else if(sValue == "parent")
        relativeTo = Node::TS_PARENT;
    else if(sValue == "world")
        relativeTo = Node::TS_WORLD;

    XMLElement *pElement;

    // Process position (?)
    Vector3 position;
    pElement = XMLNode->FirstChildElement("position");
    if(pElement)
        position = parseVector3(pElement);

    // Process localDirection (?)
    Vector3 localDirection = Vector3::NEGATIVE_UNIT_Z;
    pElement = XMLNode->FirstChildElement("localDirection");
    if(pElement)
        localDirection = parseVector3(pElement);

    // Setup the look target
    try
    {
        if(!nodeName.empty())
        {
            SceneNode *pLookNode = mSceneMgr->getSceneNode(nodeName);
            position = pLookNode->_getDerivedPosition();
        }

        pParent->lookAt(position, relativeTo, localDirection);
    }
    catch(Ogre::Exception &/*e*/)
    {
        LogManager::getSingleton().logMessage("[DotSceneLoader] Error processing a look target!");
    }
}

void DotSceneLoader::processTrackTarget(XMLElement *XMLNode, SceneNode *pParent)
{
    // Process attributes
    String nodeName = getAttrib(XMLNode, "nodeName");

    XMLElement *pElement;

    // Process localDirection (?)
    Vector3 localDirection = Vector3::NEGATIVE_UNIT_Z;
    pElement = XMLNode->FirstChildElement("localDirection");
    if(pElement)
        localDirection = parseVector3(pElement);

    // Process offset (?)
    Vector3 offset = Vector3::ZERO;
    pElement = XMLNode->FirstChildElement("offset");
    if(pElement)
        offset = parseVector3(pElement);

    // Setup the track target
    try
    {
        SceneNode *pTrackNode = mSceneMgr->getSceneNode(nodeName);
        pParent->setAutoTracking(true, pTrackNode, localDirection, offset);
    }
    catch(Ogre::Exception &/*e*/)
    {
        LogManager::getSingleton().logMessage("[DotSceneLoader] Error processing a track target!");
    }
}

void DotSceneLoader::processEntity(XMLElement *XMLNode, SceneNode *pParent, CUserData &userData) {
  // parse user data
  userData.parseNode(XMLNode);

  for (auto &cb : m_lCallbacks) {cb->preEntityAdded(userData);}

  // Process attributes
  String name = getAttrib(XMLNode, "name");
  String id = getAttrib(XMLNode, "id");
  String meshFile = getAttrib(XMLNode, "meshFile");
  String materialFile = getAttrib(XMLNode, "materialFile");
  String physicsType = getAttrib(XMLNode, "physics_type");

  bool isStatic = getAttribBool(XMLNode, "static", false) || physicsType == "STATIC";
  if (!userData.getBoolUserData("static", true)) {
    isStatic = false;
  }

  bool castShadows = getAttribBool(XMLNode, "castShadows", true);
  bool isGhost = false;

  // user data properties
  bool bIsWall = userData.getBoolUserData("wall", false);               // is this object a wall? this will set COL_WALL as collision group
  String type = userData.getStringUserData("type");
  Ogre::StringUtil::toLowerCase(type);
  if (type == "linkable") {
      isStatic = false;
  }
  else if (type == "contactlink") {
      isGhost = true;
  }



// physics only meshes will only appear as physics and will not be drawn, have to be static!
bool isPhysicsOnly = meshFile.find("Physics_Only_") == 0;
  if (isGhost) {
      isPhysicsOnly = "true";
  }
// rotate grass randomly
if (meshFile == "Grass.mesh") {
  pParent->yaw(Ogre::Radian(Ogre::Math::UnitRandom() * Ogre::Math::TWO_PI));
}

  // TEMP: Maintain a list of static and dynamic objects
if (isPhysicsOnly) {
}
  else if(isStatic) {
      staticObjects.push_back(name);
}
  else
      dynamicObjects.push_back(name);

  XMLElement *pElement;

  // Process vertexBuffer (?)
  pElement = XMLNode->FirstChildElement("vertexBuffer");
  if(pElement)
      ;//processVertexBuffer(pElement);

  // Process indexBuffer (?)
  pElement = XMLNode->FirstChildElement("indexBuffer");
  if(pElement)
      ;//processIndexBuffer(pElement);

  // Create the entity
  Entity *pEntity = 0;
  btRigidBody *pRB(NULL);
  try
  {
      MeshPtr mesh = MeshManager::getSingleton().load(meshFile, m_sGroupName);

  if (isStatic) {
    // for static gemometry (paged geometry), we can use the same entity for each appearance
    if (m_lEntityBufferMap.find(meshFile) == m_lEntityBufferMap.end()) {
      m_lEntityBufferMap[meshFile] = pEntity = mSceneMgr->createEntity("Ent:" + m_sPrependNode + meshFile, meshFile, m_sGroupName);
    }
    else {
      pEntity = m_lEntityBufferMap[meshFile];
    }
  }
  else {
    // elsewise create its own entity
    pEntity = mSceneMgr->createEntity(pParent->getName() + ":" + name, meshFile, m_sGroupName);
  }

  pEntity->setCastShadows(castShadows);

  // okay, now do the physics
      if (!physicsType.empty()) {
          String collisionPrim = getAttrib(XMLNode, "collisionPrim");
          btMotionState *ms(NULL);
          btCollisionShape* shape(NULL);
          Ogre::Vector3 centerOffset(Ogre::Vector3::ZERO);
          Ogre::Real mass = getAttribReal(XMLNode, "mass", 0);
    pParent->attachObject(pEntity);
          BtOgre::StaticMeshToShapeConverter converter(pEntity);

    // create Motion state if is collidable
          if (physicsType == "NO_COLLISION") {
      // this means, dont create physics
          }
          else if (physicsType == "STATIC") {
              if (isStatic) {
                  mass = 0;
                  ms = new btDefaultMotionState();
              }
              else {
                  ms = new BtOgre::RigidBodyState(pParent);
              }
          }
          else if (physicsType == "RIGID_BODY" || physicsType == "DYNAMIC") {
              ms = new BtOgre::RigidBodyState(pParent);
          }
          else {
              Ogre::LogManager::getSingleton().logMessage("Unknown Physics: " + physicsType);
          }


    // if there is a motion state, create the shape
          if (ms) {
      if (m_pPhysicsManager->hasCollisionShape(meshFile)) {
        auto colShape = m_pPhysicsManager->getCollisionShape(meshFile);
        shape = colShape.getShape();
        centerOffset = colShape.getOffset();
      }
      else {
        if (collisionPrim == "triangle_mesh" || collisionPrim == "") {
          shape = converter.createTrimesh();
          //Ogre::LogManager::getSingleton().logMessage("Creating TriMesh");
        }
        else if (collisionPrim == "box") {
          shape = converter.createBox();
          shape->setMargin(0.0);
          //Ogre::LogManager::getSingleton().logMessage("Creating Box");
          centerOffset = converter.getCenter();
        }
        else if (collisionPrim == "cylinder") {
          shape = converter.createCylinder();
          shape->setMargin(0.0);
          //Ogre::LogManager::getSingleton().logMessage("Creating Cylinder");
          centerOffset = converter.getCenter();
        }
        else if (collisionPrim == "sphere") {
          shape = converter.createSphere();
          shape->setMargin(0.0);
          //Ogre::LogManager::getSingleton().logMessage("Creating Sphere");
          centerOffset = converter.getCenter();
        }
        else if (collisionPrim == "convex_hull") {
          shape = converter.createConvex();
          //Ogre::LogManager::getSingleton().logMessage("Creating Convex");
          centerOffset = pParent->getOrientation() * converter.getCenter();
        }
        else {
          throw Ogre::Exception(0, "Unknown Collision Prim: " + collisionPrim, __FILE__);
        }

        m_pPhysicsManager->addCollisionShape(meshFile, CPhysicsCollisionObject(shape, centerOffset));
      }
          }
    pParent->detachObject(pEntity);

    // if we succesfully created the bounderies (and motion state), add it to the world
          if (ms && shape) {
              btVector3 inertia(0, 0, 0);
              if (mass > 0) {
                  shape->calculateLocalInertia(mass, inertia);
              }
              btRigidBody::btRigidBodyConstructionInfo info(mass, ms, shape, inertia);
              //btVector3 fric(getAttribReal(XMLNode, "friction_x", 1), getAttribReal(XMLNode, "friction_y", 1), getAttribReal(XMLNode, "friction_z", 1));
              //btVector3 lock_trans(getAttrib(XMLNode, "lock_trans_x", "False") == "True" ? 0 : 1, getAttrib(XMLNode, "lock_trans_y", "False") == "True" ? 0 : 1, getAttrib(XMLNode, "lock_trans_z", "False") == "True" ? 0 : 1);
              //btVector3 lock_rot(getAttrib(XMLNode, "lock_trans_x", "False") == "True" ? 0 : 1, getAttrib(XMLNode, "lock_trans_y", "False") == "True" ? 0 : 1, getAttrib(XMLNode, "lock_trans_z", "False") == "True"? 0 : 1);
              //info.m_friction = fric.length() * 0.01;
              //info.m_friction = 0.9;
              //info.m_angularDamping = getAttribReal(XMLNode, "damping_rot", 0);
              //info.m_linearDamping = getAttribReal(XMLNode, "damping_trans", 0);
              //info.m_restitution = 0.4;
              //info.m_angularSleepingThreshold = 0.01;
              //info.m_linearSleepingThreshold = 0.01;
              //info.m_additionalDamping = true;
              pRB = new btRigidBody(info);
              pRB->setWorldTransform(
                                     btTransform(
                                                 BtOgre::Convert::toBullet(pParent->getOrientation()),
                                                 BtOgre::Convert::toBullet(pParent->getScale() * (pParent->getOrientation() * centerOffset) + pParent->getPosition())));
              if (dynamic_cast<BtOgre::RigidBodyState*>(ms)) {
                  dynamic_cast<BtOgre::RigidBodyState*>(ms)->setOffset(btTransform(btQuaternion::getIdentity(), BtOgre::Convert::toBullet(-centerOffset)));
              }

              short colGroup = COL_STATIC;
              Ogre::String lcName = pParent->getName();
              Ogre::StringUtil::toLowerCase(lcName);
              if (bIsWall) {
                  colGroup = COL_WALL;
              }
              if (physicsType == "STATIC") {
                  pRB->setLinearFactor(btVector3(0, 0, 0));
                  pRB->setAngularFactor(0);
              }

              short mask = MASK_STATIC_COLLIDES_WITH;
              bool bCameraCollision = userData.getBoolUserData("cameraCollision", true);
              if (!bCameraCollision) {
                  mask ^= COL_CAMERA;
              }
              m_pPhysicsManager->getWorld()->addRigidBody(pRB, colGroup, mask);
              //pRB->setLinearFactor(lock_trans);
              //pRB->setAngularFactor(lock_rot);


      // connect the entity to its Rigidbody
      pEntity->setUserAny(Ogre::Any(pRB));
          }
      }
  }
  catch(Ogre::Exception &e)
  {
      throw Ogre::Exception(0, "[DotSceneLoader] Error loading an entity: " + e.getFullDescription(), __FILE__);
  }


  if (pEntity) {
      for (auto &cb : m_lCallbacks) {cb->postEntityAdded(pEntity, pParent, pRB, userData);}
  }
  // finally add the entity to the scene
  if (!isPhysicsOnly) {
      // if a static geometry, user has to handle it himself! (add to scene graph, ...)
      if (isStatic) {
          for (auto &cb : m_lCallbacks) {cb->staticObjectAdded(pEntity, pParent);}
      }
      else {
          pParent->attachObject(pEntity);
      }

      if(!materialFile.empty())
          pEntity->setMaterialName(materialFile, m_sGroupName);


  }

  // Process userDataReference (?)
  pElement = XMLNode->FirstChildElement("userDataReference");
  if(pElement)
      processUserDataReference(pElement, pEntity);

  if (isGhost) {
      // delete everything
      if (pEntity->getParentSceneNode()) {
          pEntity->getParentSceneNode()->detachObject(pEntity);
          mSceneMgr->destroyEntity(pEntity);
      }
      if (pParent) {
          delete pParent;
      }
      if (pRB) {
          m_pPhysicsManager->getWorld()->removeRigidBody(pRB);
          m_pPhysicsManager->eraseCollisionShape(pRB->getCollisionShape());
          delete pRB->getMotionState();
          delete pRB->getCollisionShape();
          delete pRB;
      }
  }
}

void DotSceneLoader::processParticleSystem(XMLElement *XMLNode, SceneNode *pParent)
{
    // Process attributes
    String name = getAttrib(XMLNode, "name");
    String id = getAttrib(XMLNode, "id");
    String file = getAttrib(XMLNode, "file");

    // Create the particle system
    try
    {
        ParticleSystem *pParticles = mSceneMgr->createParticleSystem(name, file);
        pParent->attachObject(pParticles);
    }
    catch(Ogre::Exception &/*e*/)
    {
        LogManager::getSingleton().logMessage("[DotSceneLoader] Error creating a particle system!");
    }
}

void DotSceneLoader::processBillboardSet(XMLElement *XMLNode, SceneNode *pParent)
{
    //! @todo Implement this
}

void DotSceneLoader::processPlane(XMLElement *XMLNode, SceneNode *pParent)
{
    //! @todo Implement this
}

void DotSceneLoader::processFog(XMLElement *XMLNode)
{
    // Process attributes
    Real expDensity = getAttribReal(XMLNode, "expDensity", 0.001);
    Real linearStart = getAttribReal(XMLNode, "linearStart", 0.0);
    Real linearEnd = getAttribReal(XMLNode, "linearEnd", 1.0);

    FogMode mode = FOG_NONE;
    String sMode = getAttrib(XMLNode, "mode");
    if(sMode == "none")
        mode = FOG_NONE;
    else if(sMode == "exp")
        mode = FOG_EXP;
    else if(sMode == "exp2")
        mode = FOG_EXP2;
    else if(sMode == "linear")
        mode = FOG_LINEAR;

    XMLElement *pElement;

    // Process colourDiffuse (?)
    ColourValue colourDiffuse = ColourValue::White;
    pElement = XMLNode->FirstChildElement("colourDiffuse");
    if(pElement)
        colourDiffuse = parseColour(pElement);

    // Setup the fog
    mSceneMgr->setFog(mode, colourDiffuse, expDensity, linearStart, linearEnd);
}

void DotSceneLoader::processSkyBox(XMLElement *XMLNode)
{
    // Process attributes
    String material = getAttrib(XMLNode, "material");
    Real distance = getAttribReal(XMLNode, "distance", 5000);
    bool drawFirst = getAttribBool(XMLNode, "drawFirst", true);

    XMLElement *pElement;

    // Process rotation (?)
    Quaternion rotation = Quaternion::IDENTITY;
    pElement = XMLNode->FirstChildElement("rotation");
    if(pElement)
        rotation = parseQuaternion(pElement);

    // Setup the sky box
    mSceneMgr->setSkyBox(true, material, distance, drawFirst, rotation, m_sGroupName);
}

void DotSceneLoader::processSkyDome(XMLElement *XMLNode)
{
    // Process attributes
    String material = XMLNode->Attribute("material");
    Real curvature = getAttribReal(XMLNode, "curvature", 10);
    Real tiling = getAttribReal(XMLNode, "tiling", 8);
    Real distance = getAttribReal(XMLNode, "distance", 4000);
    bool drawFirst = getAttribBool(XMLNode, "drawFirst", true);

    XMLElement *pElement;

    // Process rotation (?)
    Quaternion rotation = Quaternion::IDENTITY;
    pElement = XMLNode->FirstChildElement("rotation");
    if(pElement)
        rotation = parseQuaternion(pElement);

    // Setup the sky dome
    mSceneMgr->setSkyDome(true, material, curvature, tiling, distance, drawFirst, rotation, 16, 16, -1, m_sGroupName);
}

void DotSceneLoader::processSkyPlane(XMLElement *XMLNode)
{
    // Process attributes
    String material = getAttrib(XMLNode, "material");
    Real planeX = getAttribReal(XMLNode, "planeX", 0);
    Real planeY = getAttribReal(XMLNode, "planeY", -1);
    Real planeZ = getAttribReal(XMLNode, "planeX", 0);
    Real planeD = getAttribReal(XMLNode, "planeD", 5000);
    Real scale = getAttribReal(XMLNode, "scale", 1000);
    Real bow = getAttribReal(XMLNode, "bow", 0);
    Real tiling = getAttribReal(XMLNode, "tiling", 10);
    bool drawFirst = getAttribBool(XMLNode, "drawFirst", true);

    // Setup the sky plane
    Plane plane;
    plane.normal = Vector3(planeX, planeY, planeZ);
    plane.d = planeD;
    mSceneMgr->setSkyPlane(true, plane, material, scale, tiling, drawFirst, bow, 1, 1, m_sGroupName);
}

void DotSceneLoader::processClipping(XMLElement *XMLNode)
{
    //! @todo Implement this

    // Process attributes
    Real fNear = getAttribReal(XMLNode, "near", 0);
    Real fFar = getAttribReal(XMLNode, "far", 1);
}

void DotSceneLoader::processLightRange(XMLElement *XMLNode, Light *pLight)
{
    // Process attributes
    Real inner = getAttribReal(XMLNode, "inner");
    Real outer = getAttribReal(XMLNode, "outer");
    Real falloff = getAttribReal(XMLNode, "falloff", 1.0);

    // Setup the light range
    pLight->setSpotlightRange(Angle(inner), Angle(outer), falloff);
}

void DotSceneLoader::processLightAttenuation(XMLElement *XMLNode, Light *pLight)
{
    // Process attributes
    Real range = getAttribReal(XMLNode, "range");
    Real constant = getAttribReal(XMLNode, "constant");
    Real linear = getAttribReal(XMLNode, "linear");
    Real quadratic = getAttribReal(XMLNode, "quadratic");

    // Setup the light attenuation
    pLight->setAttenuation(range, constant, linear, quadratic);
}

String DotSceneLoader::getAttrib(XMLElement *XMLNode, const String &attrib, const String &defaultValue)
{
    if(XMLNode->Attribute(attrib.c_str()))
        return XMLNode->Attribute(attrib.c_str());
    else
        return defaultValue;
}

Real DotSceneLoader::getAttribReal(XMLElement *XMLNode, const String &attrib, Real defaultValue)
{
    if(XMLNode->Attribute(attrib.c_str()))
        return StringConverter::parseReal(XMLNode->Attribute(attrib.c_str()));
    else
        return defaultValue;
}

bool DotSceneLoader::getAttribBool(XMLElement *XMLNode, const String &attrib, bool defaultValue)
{
    if(!XMLNode->Attribute(attrib.c_str()))
        return defaultValue;

    if(String(XMLNode->Attribute(attrib.c_str())) == "true")
        return true;

    return false;
}

Vector3 DotSceneLoader::parseVector3(XMLElement *XMLNode)
{
    return Vector3(
        StringConverter::parseReal(XMLNode->Attribute("x")),
        StringConverter::parseReal(XMLNode->Attribute("y")),
        StringConverter::parseReal(XMLNode->Attribute("z"))
    );
}

Quaternion DotSceneLoader::parseQuaternion(XMLElement *XMLNode)
{
    //! @todo Fix this crap!

    Quaternion orientation;

    if(XMLNode->Attribute("qx"))
    {
        orientation.x = StringConverter::parseReal(XMLNode->Attribute("qx"));
        orientation.y = StringConverter::parseReal(XMLNode->Attribute("qy"));
        orientation.z = StringConverter::parseReal(XMLNode->Attribute("qz"));
        orientation.w = StringConverter::parseReal(XMLNode->Attribute("qw"));
    }
    else if(XMLNode->Attribute("axisX"))
    {
        Vector3 axis;
        axis.x = StringConverter::parseReal(XMLNode->Attribute("axisX"));
        axis.y = StringConverter::parseReal(XMLNode->Attribute("axisY"));
        axis.z = StringConverter::parseReal(XMLNode->Attribute("axisZ"));
        Real angle = StringConverter::parseReal(XMLNode->Attribute("angle"));;
        orientation.FromAngleAxis(Ogre::Angle(angle), axis);
    }
    else if(XMLNode->Attribute("angleX"))
    {
        Vector3 axis;
        axis.x = StringConverter::parseReal(XMLNode->Attribute("angleX"));
        axis.y = StringConverter::parseReal(XMLNode->Attribute("angleY"));
        axis.z = StringConverter::parseReal(XMLNode->Attribute("angleZ"));
        //orientation.FromAxes(&axis);
        //orientation.F
    }

    return orientation;
}

ColourValue DotSceneLoader::parseColour(XMLElement *XMLNode)
{
    return ColourValue(
        StringConverter::parseReal(XMLNode->Attribute("r")),
        StringConverter::parseReal(XMLNode->Attribute("g")),
        StringConverter::parseReal(XMLNode->Attribute("b")),
        XMLNode->Attribute("a") != NULL ? StringConverter::parseReal(XMLNode->Attribute("a")) : 1
    );
}

String DotSceneLoader::getProperty(const String &ndNm, const String &prop)
{
    for ( unsigned int i = 0 ; i < nodeProperties.size(); i++ )
    {
        if ( nodeProperties[i].nodeName == ndNm && nodeProperties[i].propertyNm == prop )
        {
            return nodeProperties[i].valueName;
        }
    }

    return "";
}

void DotSceneLoader::processUserDataReference(XMLElement *XMLNode, Entity *pEntity)
{
    String str = XMLNode->Attribute("id");
    pEntity->setUserAny(Any(str));
}
