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

#ifndef HITABLEINTERFACECONSTRUCTIONINFO_HPP
#define HITABLEINTERFACECONSTRUCTIONINFO_HPP

#include "Hitpoints.hpp"

namespace tinyxml2 {
class XMLElement;
};

class CHitableInterfaceConstructionInfo {
 private:
  Hitpoints mMaximalHitpoints;
  Hitpoints mCurrentHitpoints;
  float mInvulnerableTimer;
  bool mInvulnerable;

 public:
  CHitableInterfaceConstructionInfo();

  virtual void parse(const tinyxml2::XMLElement *e);
  
  Hitpoints getMaximalHitpoints() const {return mMaximalHitpoints;}
  Hitpoints getCurrentHitpoints() const {return mCurrentHitpoints;}
  float getInvulnerableTimer() const {return mInvulnerableTimer;}
  bool isInvulnerable() const {return mInvulnerable;}
};

#endif /* HITABLEINTERFACECONSTRUCTIONINFO_HPP */
