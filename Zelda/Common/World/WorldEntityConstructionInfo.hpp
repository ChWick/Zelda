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

#ifndef _WORLD_ENTITY_CONSTRUCTION_INFO_HPP_
#define _WORLD_ENTITY_CONSTRUCTION_INFO_HPP_

#include "../GameLogic/EntityConstructionInfo.hpp"
#include "HitableInterfaceConstructionInfo.hpp"
#include "AttackerInterfaceConstructionInfo.hpp"

class CWorldEntityConstructionInfo
    : public CEntityConstructionInfo,
      public CHitableInterfaceConstructionInfo,
      public CAttackerInterfaceConstructionInfo {
 private:
  uint16_t mCollisionMask;
  uint16_t mCollisionGroup;
  
 public:
  CWorldEntityConstructionInfo();
  CWorldEntityConstructionInfo(const std::string &id);
  
  virtual void parse(const tinyxml2::XMLElement *e) override;

  uint16_t getCollisionMask() const {return mCollisionMask;}
  uint16_t getCollisionGroup() const {return mCollisionGroup;}
};

#endif /* _WORLD_ENTITY_CONSTRUCTION_INFO_HPP_ */
