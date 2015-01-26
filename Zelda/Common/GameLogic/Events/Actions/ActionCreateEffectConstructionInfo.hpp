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

#ifndef _ACTION_CREATE_EFFECT_CONSTRUCTION_INFO_HPP_
#define _ACTION_CREATE_EFFECT_CONSTRUCTION_INFO_HPP_

#include "ActionConstructionInfo.hpp"
#include "../../../DataContainers/AttachedParentData.hpp"

class CEffectConstructionInfo;

namespace events {

typedef std::list<std::shared_ptr<CEffectConstructionInfo> > EffectConstructionInfoList;

class CActionCreateEffectConstructionInfo
    : public CActionConstructionInfo {
 private:
  EffectConstructionInfoList mEffectConstructionInfos;
  DataContainers::CAttachedParentData mAttachedParentData;
 public:
  CActionCreateEffectConstructionInfo(
      const tinyxml2::XMLElement *e);

  const EffectConstructionInfoList &getEffectConstructionInfos() const {
    return mEffectConstructionInfos;
  }

  const DataContainers::CAttachedParentData &getAttachedParentData() const {
    return mAttachedParentData;
  }
};

}  // namespace events

#endif /* _ACTION_CREATE_EFFECT_CONSTRUCTION_INFO_HPP_ */
