#include "Player.h"
#include "../../Common/Physics/PhysicsManager.h"
#include "../Atlas/Map.hpp"
#include "../../Common/Util/DebugDrawer.hpp"
#include "PlayerController.h"
#include <OgreLogManager.h>
#include "../../Common/Physics/BtOgreExtras.h"

#define TURN_SCALE 4
#define MAX_SPEED_SCALE 5

Ogre::Real g_fCurrentSpeedScale = 1;
const Ogre::Real PLAYER_ENEMY_NOTIFY_RADIUS_SQR = 100.f; // already squared!

CPlayer::CPlayer(CEntity *pParent, const Ogre::Camera* pCamera, Ogre::SceneManager *pPlayerSceneManager)
	: CPerson("player", pParent, FOE_FRIENDLY),
		m_pCamera(pCamera),
    m_pPlayerSceneManager(pPlayerSceneManager) {
}
CPlayer::~CPlayer() {
}
void CPlayer::destroy() {
  CPerson::destroy();
}
CCharacterController *CPlayer::createCharacterController() {
	return new CPlayerController(m_pPlayerSceneManager, m_pCamera, this);
}
void CPlayer::setupInternal()  {
	// create hair and hat
	//m_pBodyEntity->attachObjectToBone("Hair", sceneMgr->createEntity("LinkHair", "LinkHair.mesh"));
	//m_pBodyEntity->attachObjectToBone("Hair", sceneMgr->createEntity("LinkHat", "LinkHat.mesh"));

	// create swords and attach to sheath
	Ogre::LogManager::getSingleton().logMessage("Creating swords");
	//createTool(CPlayerTool::TOOL_SWORD, false);
	//createShield(CShield::ST_SIMPLE_SHIELD, false);
	//createHandObject(PERSON_RIGHT_HANDLE, RIGHT_HAND, "SimpleShield");


	/*LogManager::getSingleton().logMessage("Creating the chains");
	// create a couple of ribbon trails for the swords, just for fun
	NameValuePairList params;
	params["numberOfChains"] = "2";
	params["maxElements"] = "80";
	mSwordTrail = (RibbonTrail*)sceneMgr->createMovableObject("RibbonTrail", &params);
	mSwordTrail->setMaterialName("Effects/LightRibbonTrail");
	mSwordTrail->setTrailLength(20 * SCALE);
	mSwordTrail->setVisible(false);
	sceneMgr->getRootSceneNode()->attachObject(mSwordTrail);



	for (int i = 0; i < 2; i++)
	{
		mSwordTrail->setInitialColour(i, 1, 0.8, 0);
		mSwordTrail->setColourChange(i, 0.75, 1.25, 1.25, 1.25);
		mSwordTrail->setWidthChange(i, 1 * SCALE);
		mSwordTrail->setInitialWidth(i, 0.5 * SCALE);
	}*/
}
void CPlayer::startup(const Ogre::Vector3 &playerPosition, const Ogre::Vector3 &playerLookDirection, const Ogre::Real cameraYaw, const Ogre::Real cameraPitch) {
	m_pSceneNode->setPosition(playerPosition + Ogre::Vector3::UNIT_Y * PERSON_HEIGHT);
	m_pSceneNode->lookAt(playerLookDirection, Ogre::Node::TS_LOCAL);

	m_pCollisionObject->setWorldTransform(btTransform(BtOgre::Convert::toBullet(m_pSceneNode->getOrientation()), BtOgre::Convert::toBullet(m_pSceneNode->getPosition())));

	//m_pCameraController->setCameraPosition(cameraYaw, cameraPitch);
}
/*void CPlayer::interact(CObject::EInteractType eInteractType) {
	if (m_pLiftedObject) {return;}
	Ogre::Vector3 startPos(m_pBodyNode->getPosition() - Ogre::Vector3::UNIT_Y * PERSON_HEIGHT * 0.5);
	Ogre::Vector3 endPos(startPos + m_pBodyNode->getOrientation().zAxis() * PERSON_RADIUS * 2.0f);
	// try to interact with the world. So detect an object to interact with
	btCollisionWorld::ClosestRayResultCallback rayCallback(BtOgre::Convert::toBullet(startPos), BtOgre::Convert::toBullet(endPos));
	rayCallback.m_collisionFilterGroup = COL_CHARACTER_P;
	rayCallback.m_collisionFilterMask = COL_INTERACTIVE | COL_CHARACTER_N | COL_CHARACTER_P;
	m_pCurrentMap->getPhysicsManager()->getWorld()->rayTest(BtOgre::Convert::toBullet(startPos), BtOgre::Convert::toBullet(endPos), rayCallback);
	DebugDrawer::getSingleton().drawLine(startPos, endPos, Ogre::ColourValue::Red);
	if (rayCallback.hasHit()) {
		if (rayCallback.m_collisionObject->getUserPointer()) {
			InteractionInterface *pII = dynamic_cast<InteractionInterface*>(static_cast<CPhysicsUserPointer*>(rayCallback.m_collisionObject->getUserPointer()));
			if (pII->interactOnKeypress(eInteractType, m_pBodyNode->getOrientation().zAxis())) {
				if (eInteractType == CObject::IT_ATTACK) {
                    CObject *pObject = dynamic_cast<CObject*>(pII);
                    if (pObject) {
                        if (pObject->getObjectType() == CObject::LIFTABLE_OBJECT) {
                            m_pLiftedObject = btRigidBody::upcast(pObject->getCollisionObject());
                            animStartPickObject();
                        }
                    }
				}
			}
		}
	}
}*/
void CPlayer::preUpdateBoundsCallback(const Ogre::Real deltaTime) {
    CPersonController *pPersonController = dynamic_cast<CPersonController*>(m_pCharacterController);
	// check if player is in war mode (swords drawn), then im enemy is near change view
}
void CPlayer::hitpointsChangedCallback() {
	//CHUD::getSingleton().setHP(getCurrentHP());
	//CPlayerData::getSingleton().setHP(getCurrentHP());
}
void CPlayer::maxHitpointsChangedCallback() {
	//CHUD::getSingleton().setMaxHp(getMaxHP());
	//CPlayerData::getSingleton().setMaxHP(getMaxHP());
}
/*void CPlayer::objectChangedMap(CObject *pBefore, CObject *pAfter) {
	CPlayerInteractiveArea *pPIA = dynamic_cast<CPlayerInteractiveArea*>(pBefore);
	if (pPIA && pPIA == m_pInteractingArea) {
		m_pInteractingArea = dynamic_cast<CPlayerInteractiveArea*>(pAfter);
	}
}*/
