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
