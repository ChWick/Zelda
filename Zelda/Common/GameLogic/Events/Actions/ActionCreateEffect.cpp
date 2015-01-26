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

#include "ActionCreateEffect.hpp"
#include <OgreException.h>
#include "ActionCreateEffectConstructionInfo.hpp"
#include "../Event.hpp"
#include "../../../Effects/Effect.hpp"
#include "../../../World/AbstractWorldEntity.hpp"
#include "../../../World/AbstractMap.hpp"


namespace events {

CActionCreateEffect::CActionCreateEffect(
      const std::shared_ptr<CActionCreateEffectConstructionInfo> info,
      const CEvent &owner)
    : CAction(ACTION_CREATE_EFFECT, owner),
      mEffectConstructionInfos(info->getEffectConstructionInfos()),
      mAttachedParentData(info->getAttachedParentData()) {
}

void CActionCreateEffect::start() {
  ASSERT(dynamic_cast<CAbstractWorldEntity*>(m_Owner.getOwner()));
  for (auto &ei : mEffectConstructionInfos) {
    CEffect *effect = nullptr;
    if (mAttachedParentData.getType() == DataContainers::APT_MAP) {
      effect = new CEffect(
        static_cast<CAbstractWorldEntity*>(m_Owner.getOwner())->getMap(),
        *ei.get());
    } else {
      throw Ogre::Exception(mAttachedParentData.getType(),
                            "Attached parent type is not implemented",
                            __FILE__);
    }
    effect->init();
    effect->setPosition(m_Owner.getOwner()->getPosition());
    effect->start();
  }
}

}  // namespace events
