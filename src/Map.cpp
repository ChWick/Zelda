#include "StdAfx.h"
#include "Map.h"
#include "PhysicsManager.h"
#include "Door.h"
#include "Sign.h"
#include "Chest.h"
#include "Util.h"
#include "Player.h"
#include "MainMap.h"
#include "LiftableObject.h"
#include "SimpleEnemy.h"
#include "CameraController.h"
#include "LinkableObject.h"
#include "ContactLink.h"
#include "Ladder.h"
#include "Glower.h"

CMap::CMap(CMapManager &mapManager, Ogre::SceneManager *pSceneManager, const MapLocalisationData &localisationData)
	: m_MapManager(mapManager), m_pStaticGeometry(NULL), m_pSceneManager(pSceneManager),
	m_ObjectManager(*this), m_pRootSceneNode(NULL), m_LocalisationData(localisationData), m_pPhysicsManager(NULL), m_pPlayer(0) {
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation("../" + getPath(), "FileSystem", getResourceGroup());
	Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup(getResourceGroup());

	m_bLiftableObjectsMustRecreate = false;
}
CMap::~CMap() {
	unload();

	Ogre::ResourceGroupManager::getSingleton().destroyResourceGroup(getResourceGroup());
}
void CMap::start() {
	assert(m_pRootSceneNode);
	assert(m_pStaticGeometry);

	for (auto *pLight : m_lLights) {
		pLight->setVisible(true);
	}
	m_pRootSceneNode->setVisible(true);
	m_pStaticGeometry->setVisible(true);
}
void CMap::stop() {
	assert(m_pRootSceneNode);
	assert(m_pStaticGeometry);

	for (auto *pLight : m_lLights) {
		pLight->setVisible(false);
	}
	m_pRootSceneNode->setVisible(false);
	m_pStaticGeometry->setVisible(false);
}
void CMap::load() {
	using namespace Ogre;

	unload();

	m_pPhysicsManager = new CPhysicsManager(m_pSceneManager);
	m_pRootSceneNode = m_pSceneManager->getRootSceneNode()->createChildSceneNode(getName() + "/" + "MainMapRootSceneNode");

	preLoadIntern();


	auto pCam = m_pSceneManager->getCamera("GameCam");

	m_pStaticGeometry = m_pSceneManager->createStaticGeometry(getName() + "/" + "StaticGeometry");
	//m_pStaticGeometry->setVisible(false);
	m_pStaticGeometry->setRenderingDistance(2000);
	m_pStaticGeometry->setRegionDimensions(Ogre::Vector3(150, 50, 150) * 2);
	m_pStaticGeometry->setRenderQueueGroup(Ogre::RENDER_QUEUE_WORLD_GEOMETRY_1);



	//m_MapUserDataParser.load(getName() + ".userData", getResourceGroup());
	m_DotSceneLoader.addCallback(this);
	//loader.parseDotScene("maps/Map2/map.scene", "General", m_pSceneManager, m_pSceneManager->getRootSceneNode());
	m_DotSceneLoader.parseDotScene(getName() + ".scene", getResourceGroup(), m_pSceneManager, m_pPhysicsManager, m_pRootSceneNode, getName());
	//loader.parseDotScene("maps/caves/LinkHouse.scene", "General", m_pSceneManager, m_pSceneManager->getRootSceneNode());


	m_pStaticGeometry->build();

	postLoadIntern();

	m_ObjectManager.start();
}
void CMap::unload() {
	preUnloadIntern();

	while (m_lCharacters.size() > 0) {
		m_lCharacters.front()->destroy();
		delete m_lCharacters.front();
		m_lCharacters.pop_front();
	}

	m_ObjectManager.deleteAllObjects();
	if (m_pStaticGeometry) {
		m_pSceneManager->destroyStaticGeometry(m_pStaticGeometry);
		m_pStaticGeometry = NULL;
	}
	m_DotSceneLoader.cleanup();
	postUnloadIntern();

	if (m_pPhysicsManager) {
		delete m_pPhysicsManager;
		m_pPhysicsManager = NULL;
	}

	while (m_lLights.size() > 0) {
		m_pSceneManager->destroyLight(m_lLights.front());
		m_lLights.pop_front();
	}

	if (m_pRootSceneNode) {
		destroySceneNode(m_pRootSceneNode);
		m_pRootSceneNode = NULL;
	}
}
void CMap::update(const Ogre::Real &tpf) {
	updateIntern(tpf);
	m_ObjectManager.update(tpf);
	m_pPhysicsManager->update(tpf);

	while (m_lCharactersToDestroy.size() > 0) {
		for (auto it = m_lCharacters.begin(); it != m_lCharacters.end(); it++) {
			if (*it == m_lCharactersToDestroy.front()) {
				m_lCharacters.erase(it);
				break;
			}
		}
		m_lCharactersToDestroy.front()->destroy();
		delete m_lCharactersToDestroy.front();
		m_lCharactersToDestroy.pop_front();
	}
	for (auto pChar : m_lCharacters) {pChar->update(tpf);}

	processCollisionCheck();
}
void CMap::addAndInitialiseNewChar(CCharacter *pChar) {
	m_lCharacters.push_back(pChar);
	pChar->init(m_pRootSceneNode);
	if (dynamic_cast<CSimpleEnemy*>(pChar)) {
        dynamic_cast<CSimpleEnemy*>(pChar)->setPlayer(m_pPlayer);
    }
}
void CMap::processCollisionCheck() {
	int numManifolds = m_pPhysicsManager->getWorld()->getDispatcher()->getNumManifolds();
	for (int i=0;i<numManifolds;i++)
	{
		btPersistentManifold* contactManifold =  m_pPhysicsManager->getWorld()->getDispatcher()->getManifoldByIndexInternal(i);
		const btCollisionObject* obA = static_cast<const btCollisionObject*>(contactManifold->getBody0());
		const btCollisionObject* obB = static_cast<const btCollisionObject*>(contactManifold->getBody1());

		int numContacts = contactManifold->getNumContacts();
		for (int j=0;j<numContacts;j++)
		{
			btManifoldPoint& pt = contactManifold->getContactPoint(j);
			if (pt.getDistance()<0.f)
			{
				// Contact of 2 objects
				objectCollided(obA, obB);
				objectCollided(obB, obA);
			}
		}

	}
}
void CMap::objectCollided(const btCollisionObject *pColObj, const btCollisionObject *pPartner) {
	if (pColObj->getUserPointer()) {
		CObject *pObj = dynamic_cast<CObject*>(static_cast<CPhysicsUserPointer*>(pColObj->getUserPointer()));
		if (pObj){
			pObj->interactOnCollision(pPartner);
		}
	}
}
void CMap::destroyObjectsEntity(CObject *pObject, Ogre::Entity *pEntity, const Ogre::Vector3 &vPos) {
	assert(pEntity);
	assert(pEntity->getParentSceneNode());
	destroySceneNode(pEntity->getParentSceneNode());
}
void CMap::preEntityAdded(UserData &userData) {
	Ogre::String sType = userData.getStringUserData("type");
	Ogre::StringUtil::toLowerCase(sType);

    if (sType == "vase"
        || sType == "greenbush"
        || sType == "lightstone"
        || sType == "sign") {

		userData.setUserData("static", m_bLiftableObjectsMustRecreate);
	}
}
void CMap::postEntityAdded(Ogre::Entity *pEntity, Ogre::SceneNode *pParent, btRigidBody *pRigidBody, const UserData &userData) {
	assert(pEntity && pParent);

	boost::cmatch matches;
	CObject* pObject(NULL);

	const Ogre::String id(userData.getStringUserData("id"));
	Ogre::String sType = userData.getStringUserData("type");
	Ogre::StringUtil::toLowerCase(sType);

	// Door name definition: Door_ID_NEXTMAP ('/' will be changed in '-', since direct '/' is not possible in exporting from Ogre)
	boost::regex doorRegEx("^" + m_DotSceneLoader.getPrependNode() + "Door_(\\w*)_([\\-/\\w]*)$");
	if (sType == "door") {
        pObject = new CDoor(pParent, m_MapManager, m_ObjectManager, userData, id);
	}
	/*else if (boost::regex_match(pParent->getName().c_str(), matches, doorRegEx)) {
		// there is a door, lets create a door object, to handle
		std::string nextMap = matches[2];
		boost::replace_all(nextMap, "-", "/");

		if (nextMap == "MAINMAP") {nextMap = getName();}

		pObject = new CDoor(pParent, m_MapManager, m_ObjectManager, nextMap, id, id);
	}*/
	else if (sType == "linkable") {
        pObject = new LinkableObject(m_ObjectManager, pRigidBody, pParent, pEntity, id);
	}
	else if (sType == "contactlink") {
        Ogre::Vector3 bbHalfSize(pEntity->getBoundingBox().getHalfSize());
        Ogre::Real radius = std::min<Ogre::Real>(bbHalfSize.x, std::min<Ogre::Real>(bbHalfSize.y, bbHalfSize.z));

        pObject = new ContactLink(m_MapManager, m_ObjectManager, pParent, radius, userData, id);
	}
	else if (sType == "ladder") {
        pObject = new Ladder(m_ObjectManager, pParent->getPosition(), pParent->getOrientation().getYaw().valueRadians());
	}
	else if (sType == "glower") {
        pObject = new Glower(m_ObjectManager, pParent);
	}
	else if (sType == "vase") {
		pObject = new CLiftableObject(pParent, m_ObjectManager, pRigidBody, CLiftableObject::LOT_VASE, m_bLiftableObjectsMustRecreate, userData);
	}
	else if (pParent->getName().find("GreenBush") != Ogre::String::npos) {
		pObject = new CLiftableObject(pParent, m_ObjectManager, pRigidBody, CLiftableObject::LOT_GREEN_BUSH, m_bLiftableObjectsMustRecreate, userData);
	}
	else if (pParent->getName().find("LightStone") != Ogre::String::npos) {
		pObject = new CLiftableObject(pParent, m_ObjectManager, pRigidBody, CLiftableObject::LOT_LIGHT_STONE, m_bLiftableObjectsMustRecreate, userData);
	}
	else if (pParent->getName().find("Sign") != Ogre::String::npos) {
		/*auto data(m_MapUserDataParser.getSignUserDataById(id));
		pObject = new CSign(pParent, m_ObjectManager, pRigidBody, id, data._front, data._rear);*/
	}
	else if (pParent->getName().find("Chest") != Ogre::String::npos) {
		//auto data(m_MapUserDataParser.getChestUserDataById(id));
		CChest::EChestType ct;
		if (pParent->getName().find("Small") != Ogre::String::npos) {
			ct = CChest::CT_SMALL;
		}
		else if (pParent->getName().find("Big") != Ogre::String::npos) {
			ct = CChest::CT_BIG;
		}
		else {
			throw Ogre::Exception(0, "Unknown chest type: " + pParent->getName(), __FILE__);
		}
		pObject = new CChest(pParent, m_ObjectManager, pRigidBody, ct, id);
	}
	if (pRigidBody && pObject) {
		pObject->setAsUserPointer(pRigidBody);
		pRigidBody->getBroadphaseHandle()->m_collisionFilterGroup |= COL_INTERACTIVE;
	}

	if (pObject) {
        pObject->setUserData(userData);
	}
}
void CMap::staticObjectAdded(Ogre::Entity *pEntity, Ogre::SceneNode *pParent) {
	assert(pEntity);
	assert(pParent);
	assert(m_pStaticGeometry);

	// if the object is water, then we have to draw it later
	if (pEntity->getMesh()->getName().find("Water") == 0) {
		pEntity->setRenderQueueGroup(Ogre::RENDER_QUEUE_OVERLAY);
	}

	// add the object to the static geometry
	m_pStaticGeometry->addEntity(pEntity, pParent->_getDerivedPosition(),
		pParent->_getDerivedOrientation(), pParent->_getDerivedScale());

	// we dont need the scene node anymore!
	m_pSceneManager->destroySceneNode(pParent);
}
void CMap::playerEnters(CPlayer *pPlayer, const Ogre::String &entryPoint) {
	m_pPlayer = pPlayer;
	for (auto pCharacter : m_lCharacters) {
        if (dynamic_cast<CSimpleEnemy*>(pCharacter)) {
            dynamic_cast<CSimpleEnemy*>(pCharacter)->setPlayer(pPlayer);
        }
	}

	// find door in entries
	for (auto pObject : m_ObjectManager.getObjects()) {
		auto pMapLink = dynamic_cast<CMapLinkInterface*>(pObject);
		if (!pMapLink) {continue;}
		if (pMapLink->getID() == entryPoint) {
			pPlayer->startup(pMapLink->getPlayerPosition(), pMapLink->getPlayerLook(), pMapLink->getCameraYaw(), pMapLink->getCameraPitch());
			return;
		}
	}

	// the entry point has to exist!
	throw Ogre::Exception(0, "Entry point \"" + entryPoint + "\" not fount in objects.", __FILE__);
}
std::list<CHitableInterface *> CMap::getHitableInterfacesInSphere(const Ogre::Sphere &sphere) const {
    std::list<CHitableInterface *> lObjects;
    // loop through all object
    for (CObject *pObject : m_ObjectManager.getObjects()) {
        if (pObject->getCollisionObject() && sphere.intersects(pObject->getPosition())) {
            // object in spehere
            lObjects.push_back(pObject);
        }
    }

    // loop through all characters
    for (CCharacter *pCharacter : m_lCharacters) {
        if (sphere.intersects(pCharacter->getPosition())) {
            lObjects.push_back(pCharacter);
        }
    }
    // and the player
    if (sphere.intersects(m_pPlayer->getPosition())) {
        lObjects.push_back(m_pPlayer);
    }

    return lObjects;
}
