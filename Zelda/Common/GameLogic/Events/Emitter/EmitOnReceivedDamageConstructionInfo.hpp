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

#ifndef _EMIT_ON_RECEIVED_DAMAGE_CONSTRUCTION_INFO_HPP_
#define _EMIT_ON_RECEIVED_DAMAGE_CONSTRUCTION_INFO_HPP_

#include "EmitterConstructionInfo.hpp"
#include <limits>

namespace events {

class CEmitOnReceivedDamageConstructionInfo
    : public CEmitterConstructionInfo {
 private:
  uint32_t mDamageMask;
  bool mKilledByDamage;

 public:
  CEmitOnReceivedDamageConstructionInfo(
      uint32_t mask = std::numeric_limits<uint32_t>::max(),
      bool killedByDamage = false);
  
  CEmitOnReceivedDamageConstructionInfo(const tinyxml2::XMLElement *e);

  uint32_t getDamageMask() const {return mDamageMask;}
  bool wasKilledByDamage() const {return mKilledByDamage;}
};

}  // namespace events

#endif /* _EMIT_ON_RECEIVED_DAMAGE_CONSTRUCTION_INFO_HPP_ */
