#include "StdAfx.h"
#include "Glower.h"
#include "ObjectManager.h"
#include "Map.h"
#include "Util.h"
#include "Damage.h"

const Ogre::Real Glower::DEFAULT_BURNING_TIME = 5.0f;

Glower::Glower(CObjectManager &objectManager, Ogre::SceneNode *pSceneNode, const Ogre::Real fBurningTime)
: CObject(objectManager),
    m_fBurningTime(fBurningTime) {
    m_pFireParticleSystem = pSceneNode->getCreator()->createParticleSystem(pSceneNode->getName() + "Particle", "fire");
    m_pFireSceneNode = objectManager.getMap().getRootSceneNode()->createChildSceneNode();
    m_pFireSceneNode->attachObject(m_pFireParticleSystem);

    m_pFireSceneNode->setPosition(pSceneNode->getPosition() + Ogre::Vector3(0, 0.3f, 0));
    m_pFireSceneNode->setOrientation(pSceneNode->getOrientation());
    m_pFireSceneNode->setScale(pSceneNode->getScale() * 0.01);


    m_fTimer = m_fBurningTime;
    m_pFireParticleSystem->setEmitting(false);
}

Glower::~Glower()
{
    destroySceneNode(m_pFireSceneNode, true);
    m_pFireSceneNode = NULL;
}
void Glower::update(Ogre::Real tpf) {
    if (m_fTimer > 0) {
        m_fTimer -= tpf;
        if (m_fTimer < 0) {
            m_pFireParticleSystem->setEmitting(false);
        }
    }
}
CHitableInterface::EReceiveDamageResult Glower::receiveDamage(const CDamage &damage) {
	if (CDamage::DMG_FIRE & damage.getDamageType()) {
		// check if i can be hit by this damage type
        m_pFireParticleSystem->setEmitting(true);
        m_fTimer = m_fBurningTime;
		return RDR_ACCEPTED;
	}
	else {
        return RDR_BLOCKED;
    }
}
