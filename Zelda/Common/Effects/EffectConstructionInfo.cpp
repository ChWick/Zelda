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

#include "EffectConstructionInfo.hpp"
#include <string>
#include "ParticleSystem/ParticleSystemConstructionInfo.hpp"
#include "../tinyxml2/tinyxml2.hpp"

using tinyxml2::XMLElement;

CEffectConstructionInfo::CEffectConstructionInfo(const std::string &id)
    : CWorldEntityConstructionInfo(id) {
}

CEffectConstructionInfo::CEffectConstructionInfo(const XMLElement *e)
    : CWorldEntityConstructionInfo(e) {
  for (const XMLElement *c = e->FirstChildElement(); c;
       c = c->NextSiblingElement()) {
    if (strcmp(c->Value(), "particle_system") == 0) {
      mParticleSystems.push_back(
          std::make_shared<CParticleSystemConstructionInfo>(c));
    } else if (strcmp(c->Value(), "sound") == 0) {
    }
  }
}
