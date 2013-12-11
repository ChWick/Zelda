#include "StdAfx.h"
#include "Character.h"
#include "Util.h"
#include "PhysicsManager.h"
#include "Map.h"
#include "PhysicsManager.h"
#include "LiftableObject.h"
#include "Damage.h"
#include "CharacterController.h"

Ogre::NameGenerator CCharacter::m_NameGenerator("Char_");

CCharacter::CCharacter(CMap *pMap, const EFriendOrEnemyStates foe)
	: m_uiAnimationCount(ANIM_COUNT),
	m_pBodyPhysics(NULL), m_fTimer(0),	m_fTopAnimSpeed(1), m_fBaseAnimSpeed(1), m_pCurrentMap(pMap), m_pBodyNode(NULL),
	m_eFriendOrEnemy(foe)
{
	m_pCharacterController = NULL;
    mCCPhysics = NULL;
    m_bMoving = false;
    m_fYaw = 0;
    m_Anims.resize(m_uiAnimationCount);
    m_FadingStates.resize(m_uiAnimationCount);
    m_uiTopAnimID = m_uiBaseAnimID = m_uiAnimationCount;
}
CCharacter::~CCharacter() {
	assert(!m_pBodyNode);
	assert(!m_pBodyEntity);
}
void CCharacter::init(Ogre::SceneNode *pParentSceneNode) {
	initBody(pParentSceneNode);
	createPhysics();
	m_pCharacterController = createCharacterController();
	setupAnimations();
	setupInternal();
}
void CCharacter::destroy() {
	destroyPhysics();
	destroySceneNode(m_pBodyNode);
	m_pBodyNode = NULL;
	m_pBodyEntity = NULL;
	if (m_pCharacterController) {
		delete m_pCharacterController;
	}
}
void CCharacter::setPosition(const Ogre::Vector3 &vPos) {
    m_pCharacterController->setPosition(vPos);
}
void CCharacter::setOrientation(const Ogre::Quaternion &vRotation) {
    m_pCharacterController->setOrientation(vRotation);
}
void CCharacter::update(const Ogre::Real fTime) {
	updateInvulnerableTimer(fTime);
	m_pCharacterController->updateCharacter(fTime);
    preUpdateBoundsCallback(fTime);
    //updateBounds(m_pBodyNode);
    //if (m_pShield) {m_pShield->updateBounds(m_pShield->getShieldEntity()->getParentNode());}
    preAnimationUpdateCallback(fTime);
    updateAnimations(fTime);
}
void CCharacter::updateAnimations(const Ogre::Real fTime) {
    m_fTimer += fTime;
    m_fBaseAnimSpeed = m_fTopAnimSpeed = 1;

    updateAnimationsCallback(fTime);

    //if (m_fTimer > 0) {
        // increment the current base and top animation times
        if (m_uiBaseAnimID != ANIM_NONE) m_Anims[m_uiBaseAnimID]->addTime(fTime * m_fBaseAnimSpeed);
        if (m_uiTopAnimID != ANIM_NONE) m_Anims[m_uiTopAnimID]->addTime(fTime * m_fTopAnimSpeed);

        // apply smooth transitioning between our animations
        fadeAnimations(fTime);
    /*}
    else {
        if (m_uiBaseAnimID != ANIM_NONE) m_Anims[m_uiBaseAnimID]->addTime(0);
        if (m_uiTopAnimID != ANIM_NONE) m_Anims[m_uiTopAnimID]->addTime(0);
        fadeAnimations(0);
    }*/

    postUpdateAnimationsCallback(fTime);

	Ogre::MaterialPtr ptr;
}
void CCharacter::updateAnimationsCallback(const Ogre::Real fTime) {

    if (m_uiBaseAnimID == ANIM_JUMP_END && m_Anims[ANIM_JUMP_END]->hasEnded()) {
        if (!m_bMoving) {
            setBaseAnimation(ANIM_IDLE_BASE);
            setTopAnimation(ANIM_IDLE_TOP);
        }
        else {
            setBaseAnimation(ANIM_RUN_BASE);
            setTopAnimation(ANIM_RUN_TOP);
        }
    }
    else if (m_uiBaseAnimID == ANIM_JUMP_START && m_Anims[ANIM_JUMP_END]->hasEnded()) {
        setBaseAnimation(ANIM_JUMP_LOOP);
        setTopAnimation(ANIM_NONE);
    }

    //Ogre::LogManager::getSingleton().logMessage(((m_bMoving) ? "true" : "false") + Ogre::StringConverter::toString((int)m_uiBaseAnimID));
    if (m_bMoving && (m_uiBaseAnimID == ANIM_IDLE_BASE || m_uiBaseAnimID == ANIM_NONE)) {
        setBaseAnimation(ANIM_RUN_BASE);
    }
    else if (!m_bMoving) {
        //Ogre::LogManager::getSingleton().logMessage(Ogre::StringConverter::toString(ANIM_RUN_BASE));
        if (m_uiBaseAnimID == ANIM_RUN_BASE || m_uiBaseAnimID == ANIM_NONE) {
            setBaseAnimation(ANIM_IDLE_BASE);
        }
    }
    if (m_uiBaseAnimID == ANIM_RUN_BASE && (m_uiTopAnimID == ANIM_IDLE_TOP || m_uiTopAnimID == ANIM_NONE)) {
        setTopAnimation(ANIM_RUN_TOP);
    }
    else if (m_uiBaseAnimID == ANIM_IDLE_BASE && (m_uiTopAnimID == ANIM_NONE || m_uiTopAnimID == ANIM_RUN_TOP)) {
        setTopAnimation(ANIM_IDLE_TOP);
    }
}
void CCharacter::setBaseAnimation(unsigned int id, bool reset) {
    if (m_uiBaseAnimID < m_uiAnimationCount)
    {
        // if we have an old animation, fade it out
        m_FadingStates[m_uiBaseAnimID] = FADE_OUT;
    }

    m_uiBaseAnimID = id;

    if (id != m_uiAnimationCount)
    {
        // if we have a new animation, enable it and fade it in
        m_Anims[id]->setEnabled(true);
        //m_Anims[id]->setWeight(0);
        m_FadingStates[m_uiBaseAnimID] = FADE_IN;
        if (reset) m_Anims[id]->setTimePosition(0);
    }
}
void CCharacter::setTopAnimation(unsigned int id, bool reset)
{
    if (m_uiTopAnimID < m_uiAnimationCount)
    {
        // if we have an old animation, fade it out
        m_FadingStates[m_uiTopAnimID] = FADE_OUT;
    }

    m_uiTopAnimID = id;

    if (id != m_uiAnimationCount)
    {
        // if we have a new animation, enable it and fade it in
        m_Anims[id]->setEnabled(true);
        //m_Anims[id]->setWeight(0);
        m_FadingStates[id] = FADE_IN;
        if (reset) m_Anims[id]->setTimePosition(0);
    }
}
void CCharacter::fadeAnimations(const Ogre::Real deltaTime)
{
    for (unsigned int i = 0; i < m_uiAnimationCount; i++)
    {
        if (m_FadingStates[i] == FADE_NONE) {
            continue;
        }
        else if (m_FadingStates[i] == FADE_IN)
        {
            // slowly fade this animation in until it has full weight
            Ogre::Real newWeight = m_Anims[i]->getWeight() + deltaTime * ANIM_FADE_SPEED;
            m_Anims[i]->setWeight(Ogre::Math::Clamp<Ogre::Real>(newWeight, 0, 1));
            if (newWeight >= 1) m_FadingStates[i] = FADE_NONE;
        }
        else if (m_FadingStates[i] == FADE_OUT)
        {
            // slowly fade this animation out until it has no weight, and then disable it
            Ogre::Real newWeight = m_Anims[i]->getWeight() - deltaTime * ANIM_FADE_SPEED;
            m_Anims[i]->setWeight(Ogre::Math::Clamp<Ogre::Real>(newWeight, 0, 1));
            if (newWeight <= 0)
            {
                m_Anims[i]->setEnabled(false);
                m_FadingStates[i] = FADE_NONE;
            }
        }
    }
}
bool CCharacter::topReadyForNewAction() {
    return m_uiTopAnimID == ANIM_NONE || m_uiTopAnimID == ANIM_IDLE_TOP || m_uiTopAnimID == ANIM_RUN_TOP;
}
void CCharacter::animRunStart()
{
    /*if (m_uiTopAnimID ==)
	setTopAnimation(ANIM_RUN_TOP);
	setBaseAnimation(ANIM_RUN_BASE);*/
}
void CCharacter::animRunEnd()
{
	/*setTopAnimation(ANIM_IDLE_TOP);
	setBaseAnimation(ANIM_IDLE_BASE, true);*/
}
void CCharacter::animJumpStart()
{
	setBaseAnimation(ANIM_JUMP_START, true);
	setTopAnimation(ANIM_NONE);
	m_fTimer = 0;
}
void CCharacter::animJumpLoop()
{
    setTopAnimation(ANIM_NONE);
	setBaseAnimation(ANIM_JUMP_LOOP, true);
}
void CCharacter::animJumpEnd()
{
    setTopAnimation(ANIM_NONE);
	setBaseAnimation(ANIM_JUMP_END, true);
	m_fTimer = 0;
}
void CCharacter::animAttack() {
    if (topReadyForNewAction()) {
        setTopAnimation(ANIM_SLICE_VERTICAL, true);
        m_fTimer = 0;
    }
}
void CCharacter::animAttackEnd() {
    setTopAnimation(ANIM_NONE);
}
void CCharacter::setIsMoving(bool isMoving)
{
	m_bMoving = isMoving;
}
CHitableInterface::EReceiveDamageResult CCharacter::receiveDamage(const CDamage &damage) {
	return RDR_REJECTED;
}
short CCharacter::getCollisionMask() {
    switch (m_eFriendOrEnemy) {
    case FOE_ENEMY:
        return MASK_PLAYER_N_COLLIDES_WITH;
    case FOE_FRIENDLY:
        return MASK_PLAYER_P_COLLIDES_WITH;
    }
}
short CCharacter::getCollisionGroup() {
    switch (m_eFriendOrEnemy) {
    case FOE_ENEMY:
        return COL_CHARACTER_N;
    case FOE_FRIENDLY:
        return COL_CHARACTER_P;;
    }
}

