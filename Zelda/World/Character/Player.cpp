#include "Player.h"
#include "HUD.h"
#include "PlayerData.h"
#include "PhysicsManager.h"
#include "Map.h"
#include "DebugDrawer.h"
#include "Object.h"
#include "PlayerController.h"
#include "CameraController.h"
#include "InteractionInterface.h"

#define TURN_SCALE 4
#define MAX_SPEED_SCALE 5

Ogre::Real g_fCurrentSpeedScale = 1;
const Ogre::Real PLAYER_ENEMY_NOTIFY_RADIUS_SQR = 100.f; // already squared!

CPlayer::CPlayer(CMap *pMap, CCameraController* camController, Ogre::SceneManager *pPlayerSceneManager)
	: CPerson(pMap, FOE_FRIENDLY), m_pCameraController(camController),
m_pPlayerSceneManager(pPlayerSceneManager), /*m_pGlobalRenderSystem(NULL), m_pCollisionDetector(NULL), m_pInteractingArea(NULL), */m_bShiftPressed(false),
	m_bMoveToInteractivePoint(false)
{
	m_pCameraController->setCharacter(this);
	m_pCameraController->setCameraMoveType(CCameraController::CMT_3RD_PERSON);
	m_pCameraController->setLookAtCharacter(this);
	setMaxHP(CPlayerData::getSingleton().getMaxHP());
	setCurrentHP(CPlayerData::getSingleton().getHP());
	m_bFreeCamera = false;
	//m_pCameraBody = NULL;
	m_bUserMayMove = true;
	m_pLight = NULL;

	/*m_pLight = m_pPlayerSceneManager->createLight();
	m_pLight->setType(Ogre::Light::LT_POINT);
	m_pLight->setDiffuseColour(Ogre::ColourValue(1, 1, 1));
	m_pLight->setSpecularColour(Ogre::ColourValue(1, 1, 1));
    m_pLight->setPosition(0, 0.1, 0.2);
    m_pLight->setAttenuation(100, 1.0, 0.0, 0.0);*/
}
CPlayer::~CPlayer() {
}
void CPlayer::destroy() {
    if (m_pLight) {
        if (m_pLight->getParentSceneNode()) {
            m_pLight->getParentSceneNode()->detachObject(m_pLight);
        }
        m_pPlayerSceneManager->destroyLight(m_pLight);
        m_pLight = NULL;
    }
    CPerson::destroy();
}
CCharacterController *CPlayer::createCharacterController() {
	return new CPlayerController(m_pPlayerSceneManager, m_pCameraController, this);
}
void CPlayer::setupInternal()  {
	// create hair and hat
	//m_pBodyEntity->attachObjectToBone("Hair", sceneMgr->createEntity("LinkHair", "LinkHair.mesh"));
	//m_pBodyEntity->attachObjectToBone("Hair", sceneMgr->createEntity("LinkHat", "LinkHat.mesh"));

	// create swords and attach to sheath
	LogManager::getSingleton().logMessage("Creating swords");
	createTool(CPlayerTool::TOOL_SWORD, false);
	createShield(CShield::ST_SIMPLE_SHIELD, false);
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
	m_pBodyNode->setPosition(playerPosition + Ogre::Vector3::UNIT_Y * PERSON_HEIGHT);
	m_pBodyNode->lookAt(playerLookDirection, Ogre::Node::TS_LOCAL);

	m_pBodyPhysics->setWorldTransform(btTransform(BtOgre::Convert::toBullet(m_pBodyNode->getOrientation()), BtOgre::Convert::toBullet(m_pBodyNode->getPosition())));

	m_pCameraController->setCameraPosition(cameraYaw, cameraPitch);

	if (m_pLight) {
        m_pBodyEntity->attachObjectToBone(PERSON_LEFT_HANDLE, m_pLight);
	}
}
void CPlayer::interact(CObject::EInteractType eInteractType) {
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
}
void CPlayer::preUpdateBoundsCallback(const Ogre::Real deltaTime) {
    CPersonController *pPersonController = dynamic_cast<CPersonController*>(m_pCharacterController);
	// check if player is in war mode (swords drawn), then im enemy is near change view
	if (m_bSwordsDrawn) {
		float nearestCharSqDist = PLAYER_ENEMY_NOTIFY_RADIUS_SQR;
		for (auto *pCharacter : m_pCurrentMap->getCharacters()) {
            if (pCharacter->getFriendOrEnemyState() == FOE_FRIENDLY) {continue;}

			float dstsqr = getPosition().squaredDistance(pCharacter->getPosition());
			if (dstsqr < PLAYER_ENEMY_NOTIFY_RADIUS_SQR && dstsqr < nearestCharSqDist) {
				nearestCharSqDist = dstsqr;
				m_pCameraController->setCameraMoveType(CCameraController::CMT_ATTACK_VIEW);
				m_pCameraController->setLookAtCharacter(pCharacter);
			}
		}
		if (nearestCharSqDist == PLAYER_ENEMY_NOTIFY_RADIUS_SQR) {
			m_pCameraController->setCameraMoveType(CCameraController::CMT_3RD_PERSON);
			m_pCameraController->setLookAtCharacter(this);
		}
	}
	else if (m_pCameraController->getCameraMoveType() == CCameraController::CMT_ATTACK_VIEW) {
		m_pCameraController->setCameraMoveType(CCameraController::CMT_3RD_PERSON);
		m_pCameraController->setLookAtCharacter(this);
	}

	if (m_bAiming) {
        pPersonController->changeMoveState(CPersonController::MS_AIMING, -m_pCameraController->getCameraNode()->getOrientation().zAxis());
	}
	else {
        if (pPersonController->getMoveState() == CPersonController::MS_AIMING) {
            pPersonController->changeMoveState(CPersonController::MS_NORMAL);
        }
        // if shield block, and if there is an emeny near, track him
        if (m_bShieldBlockActive && m_pCameraController->getCameraMoveType() == CCameraController::CMT_ATTACK_VIEW) {
            pPersonController->changeMoveState(CPersonController::MS_MOVE_AROUND_TARGET, m_pCameraController->getLookAtCharacter()->getPosition());
        }
        else if (pPersonController->getMoveState() == CPersonController::MS_MOVE_AROUND_TARGET) {
            pPersonController->changeMoveState(CPersonController::MS_NORMAL);
        }
	}
}
void CPlayer::hitpointsChangedCallback() {
	CHUD::getSingleton().setHP(getCurrentHP());
	CPlayerData::getSingleton().setHP(getCurrentHP());
}
void CPlayer::maxHitpointsChangedCallback() {
	CHUD::getSingleton().setMaxHp(getMaxHP());
	CPlayerData::getSingleton().setMaxHP(getMaxHP());
}
/*void CPlayer::objectChangedMap(CObject *pBefore, CObject *pAfter) {
	CPlayerInteractiveArea *pPIA = dynamic_cast<CPlayerInteractiveArea*>(pBefore);
	if (pPIA && pPIA == m_pInteractingArea) {
		m_pInteractingArea = dynamic_cast<CPlayerInteractiveArea*>(pAfter);
	}
}*/
bool CPlayer::hasMoreArrows() {
    return CPlayerData::getSingleton().getArrowCount() > 0;
}
void CPlayer::removeArrow() {
    assert(CPlayerData::getSingleton().getArrowCount() > 0);
    CPlayerData::getSingleton().removeArrow(1);
}
bool CPlayer::hasMoreBombs() {
    return CPlayerData::getSingleton().getBombCount() > 0;
}
void CPlayer::removeBomb() {
    assert(CPlayerData::getSingleton().getBombCount() > 0);
    CPlayerData::getSingleton().removeBomb(1);
}
