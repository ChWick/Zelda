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

#ifndef _EFFECT_CONSTRUCTION_INFO_HPP_
#define _EFFECT_CONSTRUCTION_INFO_HPP_

#include <vector>
#include "../World/WorldEntityConstructionInfo.hpp"

class CParticleSystemConstructionInfo;

typedef std::vector<std::shared_ptr<CParticleSystemConstructionInfo> >
ParticleSystemConstructionInfoList;

//! Construction information for an effect
class CEffectConstructionInfo
    : public CWorldEntityConstructionInfo {
 private:
  ParticleSystemConstructionInfoList mParticleSystems;
 public:
  //! blank default constructor
  CEffectConstructionInfo(const std::string &id);

  //! constructor from a tinyxml element
  CEffectConstructionInfo(const tinyxml2::XMLElement *e);

  const ParticleSystemConstructionInfoList &getParticleSystems() const {
    return mParticleSystems;
  }
};

#endif /* _EFFECT_CONSTRUCTION_INFO_HPP_ */
