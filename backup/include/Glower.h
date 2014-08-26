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

#ifndef GLOWER_H
#define GLOWER_H

#include <Object.h>


class Glower : public CObject {
public:
    static const Ogre::Real DEFAULT_BURNING_TIME;
private:
    Ogre::ParticleSystem *m_pFireParticleSystem;
    Ogre::SceneNode *m_pFireSceneNode;
    Ogre::Real m_fTimer;
    const Ogre::Real m_fBurningTime;
public:
    Glower(CObjectManager &objectManager, Ogre::SceneNode *pSceneNode, const Ogre::Real fBurningTime = DEFAULT_BURNING_TIME);
    virtual ~Glower();

	virtual void update(Ogre::Real tpf);

	EReceiveDamageResult receiveDamage(const CDamage &damage);
protected:
private:
};

#endif // GLOWER_H
