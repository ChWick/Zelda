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

#include "Effect.hpp"
#include <OgreStringConverter.h>
#include <OgreSceneNode.h>
#include <ParticleUniverseSystemManager.h>
#include "../Log.hpp"
#include "EffectConstructionInfo.hpp"
#include "ParticleSystem/ParticleSystemConstructionInfo.hpp"

Ogre::NameGenerator CEffect::mNameGenerator("Effect");

CEffect::CEffect(CAbstractWorldEntity *parent,
                 const CEffectConstructionInfo &info)
    : CAbstractWorldEntity(mNameGenerator.generate(), parent, info) {
  m_pSceneNode = parent->getSceneNode()->createChildSceneNode();

  LOGV("Creating effect: '%s'", getID().c_str());

  uint8_t ps_count = 0;
  for (auto &ps_info : info.getParticleSystems()) {
    LOGV(" - Creating particle system");
    ParticleUniverse::ParticleSystem *p =
        createParticleSystem("_ps_" + Ogre::StringConverter::toString(ps_count),
                             ps_info->getType(), true);
    // add this as event listener
    p->addParticleSystemListener(this);
  }
}

void CEffect::handleParticleSystemEvent(
    ParticleUniverse::ParticleSystem* particleSystem,
    ParticleUniverse::ParticleUniverseEvent& particleUniverseEvent) {
  if (particleUniverseEvent.eventType
      == ParticleUniverse::PU_EVT_SYSTEM_STOPPED) {
    deleteParticleSystemLater(particleSystem);
  }
}
